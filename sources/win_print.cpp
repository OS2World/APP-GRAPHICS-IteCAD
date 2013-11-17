/****************************************************************************
**
** Copyright (C) 2010-2011 Iteco Trading Srl
** All rights reserved.
** Contact: Iteco Trading Srl (info@itecad.it)
**
** This file is part of the Itecad EDA software.
**
** Itecad is free software. You can redistribute it and/or modify it under the terms of
** the GNU Lesser General Public License (LGPLv2.1) as published by the Free Software Foundation
**
** This software is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
** without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
** See the GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License along with this software;
** if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** If you have questions regarding the use of this file, please contact
** Iteco Trading Srl at info@itecad.it
**
****************************************************************************/

#include <QAbstractPrintDialog>
#include "win_print.h"

extern	char	file_type;
extern	win_main	*w_main;
extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;
extern	QPrinter	*default_printer;
extern	QPrinter	*pdf_printer;

extern	lay_data	layer_data[LAYER_DATA_COUNT];
extern	lay_data	workspace_data[WORKSPACE_DATA_COUNT];
extern	QPen	pen_layer;
extern	QBrush	brush_layer;

extern	stampa_formato stampa_formati[];

win_print::win_print(QWidget *parent, Qt::WFlags tipo, bool pdf)
        : QDialog(parent, tipo)
	{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni
	is_pdf = pdf;
	if (pdf)
		setWindowTitle(tr("Print to PDF"));	
	else
		setWindowTitle(tr("Print"));	
	setModal(true);

	layers = new My_Layers(); 
	layers->tipo_tabella = 'P';
	layers->load_data();

	//Imposto, se possibile, l'altezza giusta per i layer
	QSize right_size = layers->right_size();
        int right_height = right_size.height();
	QRect screen = QApplication::desktop()->screenGeometry();
	if (right_height < screen.height() * 2 / 3)
                layers->setFixedHeight(right_size.height());

	QGridLayout	*layout = new QGridLayout(this);
	layout->addLayout(setup_printer(),0,0);
	QVBoxLayout *l_layout = new QVBoxLayout();
	if (!GV->is_sch)
		{
		l_layout->addWidget(layers);
		l_layout->addStretch(1);
		layout->addLayout(l_layout,0,1);	
		}
	layout->addLayout(setup_buttons(),2,0,TRUE,FALSE);
	layout->setSpacing(6);
    layout->setMargin(8);

	}

QVBoxLayout	*win_print::setup_printer()
{
	QVBoxLayout *v_layout = new QVBoxLayout();

	if (is_pdf)
		{
		if (pdf_printer == 0)
			{
			printer = new QPrinter(QPrinter::HighResolution);
			pdf_printer = printer;
			}
		else
			printer = pdf_printer;
		}
	else
		{
		if (default_printer == 0)
			{	
			printer = new QPrinter(QPrinter::HighResolution);	
			default_printer = printer;
			}
		else
			printer = default_printer;
		}

	if (file_type == FT_PROJECT)
		if (GV->is_sch)
			{
                        //Nel caso il documento non abbia una pagina custom provo a impostare la stampante con setPaperSize
                        //Non è detto che il valore che io assegno con setPaperSize venga accettato dalla stampante perchè questa potrebbe non supportarlo
			//In seguito uso la funzione read_printer_settings() per sapere quali sono le impostazioni della stampante
			//Solamente se il documento è uno schematico di un progetto (quindi pensato per un particolare tipo di foglio)
			//imposto il formato della pagina come definito nelle proprietà del documento
			//Diversamente se è un simbolo o se è un PCB uso i valori di default della stampante di default
		/*	if (stampa_formati[GV->page_size].formato != QPrinter::Custom)
				{
                                printer->setPaperSize(stampa_formati[GV->page_size].formato);
				if (GV->page_width < GV->page_height)
					printer->setOrientation(QPrinter::Portrait);
				else
					printer->setOrientation(QPrinter::Landscape);	
				}*/
			}	
	
	printer_size_and_orientation_to  = new QLabel();	
	printer_size_and_orientation_from  = new QLabel(tr("Original format: ") + stampa_formati[GV->page_size_index()].descrizione);

	/*if (is_pdf)
		{
		printer->setFullPage(TRUE);
		printer->setOutputFormat(QPrinter::PdfFormat);
		printer_name = new QLabel();
		printer_name->setPixmap(QPixmap(":/Resources/pdf.png"));
		for (int i = 0; i< STAMPA_FORMATI_COUNT; i++)
			{
                        if (printer->paperSize() == stampa_formati[i].formato)
				pdf_page = new layout_size(i, (printer->widthMM()*100000)/25.4, (printer->heightMM()*100000)/25.4, FALSE);
			}

		v_layout->addWidget(printer_name);
		v_layout->addLayout(pdf_page);

		printer_size_and_orientation_to  = new QLabel();
		}
	else*/
	
	QGroupBox *gb_printer = new QGroupBox(QObject::tr("Printer"));
	QGridLayout *l_printer = new QGridLayout();
		
	printer->setOutputFormat(QPrinter::NativeFormat);
	if (is_pdf)
		{
		printer->setFullPage(TRUE);
		printer->setOutputFormat(QPrinter::PdfFormat);
		printer_name = new QLabel();
		printer_name->setPixmap(QPixmap(":/Resources/pdf.png"));
		}
	else
		printer_name = new QLabel(printer->printerName());
	QHBoxLayout	*L_comandi = new QHBoxLayout();
	L_comandi->setSpacing(6);
	L_comandi->setMargin(0);
	//page_setup = new QPushButton(QObject::tr("Page Setup"));
	//page_setup->setAutoDefault(FALSE);
	//QObject::connect(page_setup, SIGNAL(clicked()), this, SLOT(pagesetup_dialog()));		
	//L_comandi->addWidget(page_setup);	
	QSpacerItem	*L_comandi_molla = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	L_comandi->addItem(L_comandi_molla);
	printer_setup = new QPushButton(QObject::tr("Print Setup"));
	printer_setup->setAutoDefault(FALSE);
	QObject::connect(printer_setup, SIGNAL(clicked()), this, SLOT(printer_dialog()));
	L_comandi->addWidget(printer_setup);
	l_printer->addWidget(printer_name, 0, 0);		
	if (GV->is_sch)
		l_printer->addLayout(L_comandi, 1, 0);
	else
		{
		l_printer->addWidget(printer_size_and_orientation_to, 1, 0);
		l_printer->addLayout(L_comandi, 2, 0);
		}
	gb_printer->setLayout(l_printer);
	v_layout->addWidget(gb_printer);
	
	QSpacerItem	*molla = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
	v_layout->addItem(molla);
	
	if (GV->is_sch)
		{		
		//Inserisco la QGroupBox della scala
		QGroupBox *gb_scale = new QGroupBox(QObject::tr("Scale"));
		QGridLayout *l_scale = new QGridLayout();
		
		l_scale->addWidget(printer_size_and_orientation_from, 1, 0, Qt::AlignHCenter);
		QLabel	*arrow1 = new QLabel();
		arrow1->setPixmap(QPixmap(":/Resources/arrow_down.png"));
		l_scale->addWidget(arrow1, 2, 0, Qt::AlignHCenter);
		printer_scale  = new QLabel();
		read_scale_settings();
		l_scale->addWidget(printer_scale, 3, 0, Qt::AlignHCenter);
		QLabel	*arrow2 = new QLabel();
		arrow2->setPixmap(QPixmap(":/Resources/arrow_down.png"));
		l_scale->addWidget(arrow2, 4, 0, Qt::AlignHCenter);
		l_scale->addWidget(printer_size_and_orientation_to, 5, 0, Qt::AlignHCenter);
		
		gb_scale->setLayout(l_scale);
		v_layout->addWidget(gb_scale);
		}
	else
		{
		//Inserisco la QGroupBox della scala
		QGroupBox *gb_scale = new QGroupBox(QObject::tr("Scale"));
		QVBoxLayout *l_scale = new QVBoxLayout();
		scale_1_1 = new QRadioButton(tr("1:1 - Real size"));
		scale_2_1 = new QRadioButton(tr("2:1 - Double size"));
		scale_1_2 = new QRadioButton(tr("1:2 - Half size"));
		scale_1_1->setChecked(TRUE);
		painter_scale = 1.0;
		QObject::connect(scale_1_1, SIGNAL(toggled(bool)), this, SLOT(change_pcb_scale(bool)));
		QObject::connect(scale_2_1, SIGNAL(toggled(bool)), this, SLOT(change_pcb_scale(bool)));
		QObject::connect(scale_1_2, SIGNAL(toggled(bool)), this, SLOT(change_pcb_scale(bool)));
		l_scale->addWidget(scale_1_1);
		l_scale->addWidget(scale_2_1);
		l_scale->addWidget(scale_1_2);
		gb_scale->setLayout(l_scale);
		v_layout->addWidget(gb_scale);
		
		//Inserisco la QGroupBox dell'offset
		QGroupBox *gb_offset = new QGroupBox(QObject::tr("Offset"));
		QVBoxLayout *l_offset = new QVBoxLayout();
		offset_x = new layout_length(QObject::tr("x:"), 0, -1000000, 1000000, FALSE);
		offset_x->setUM(stampa_formati[GV->page_size_index()].UM);
		offset_y = new layout_length(QObject::tr("y:"), 0, -1000000, 1000000, FALSE);
		offset_y->setUM(stampa_formati[GV->page_size_index()].UM);
		l_offset->addLayout(offset_x);
		l_offset->addLayout(offset_y);
		gb_offset->setLayout(l_offset);
		v_layout->addWidget(gb_offset);
		v_layout->addStretch(1);
		}

        {
        //Inserisco la QGroupBox del color mode
        QGroupBox *gb_colormode = new QGroupBox(QObject::tr("Color Mode"));
        QVBoxLayout *l_colormode = new QVBoxLayout();
        colormode_color = new QRadioButton(tr("Color"));
        colormode_grayscale = new QRadioButton(tr("Grayscale"));
        colormode_black = new QRadioButton(tr("All colors as black"));
        //*** Cambiato di default la stampa è black
        colormode_black->setChecked(TRUE);
        colormode = 'B';
        QObject::connect(colormode_color, SIGNAL(toggled(bool)), this, SLOT(change_colormode(bool)));
        QObject::connect(colormode_grayscale, SIGNAL(toggled(bool)), this, SLOT(change_colormode(bool)));
        QObject::connect(colormode_black, SIGNAL(toggled(bool)), this, SLOT(change_colormode(bool)));
        l_colormode->addWidget(colormode_color);
        l_colormode->addWidget(colormode_grayscale);
        l_colormode->addWidget(colormode_black);
        gb_colormode->setLayout(l_colormode);
        v_layout->addWidget(gb_colormode);
        }


	//Aggiorno i valori letti
        if (GV->is_sch)
		{
		printer_size_and_orientation_to->setText(read_printer_settings());
		read_scale_settings();
		}
	
	return v_layout;
}

QString win_print::read_printer_settings()
{
        QPrinter::PaperSize  ps = printer->paperSize();
        QString s;
	for (int i = 0; i< STAMPA_FORMATI_COUNT; i++)
                if (stampa_formati[i].formato == ps)
			{
			s = stampa_formati[i].descrizione;
			if (!GV->is_sch)
				{
				offset_x->setUM(stampa_formati[i].UM);
				offset_y->setUM(stampa_formati[i].UM);
				}
                        }
	return tr("Destination format: ") + s;
}

void	win_print::read_scale_settings()
{
	//Nello chematico trovo la scala ed imposto un'eventuale rotazione
	qreal	original_w = (GV->page_width * printer->logicalDpiX()) / 100000.0;
	qreal	original_h = (GV->page_height * printer->logicalDpiY()) / 100000.0;
	bool	original_is_portrait = (original_h >= original_w);

	qreal	destination_w = printer->paperRect().width();	
	qreal	destination_h = printer->paperRect().height();
	bool	destination_is_portrait = (destination_h >= destination_w);
	
	if (original_is_portrait != destination_is_portrait)
		swap_qreal(destination_w, destination_h);
	
	qreal scale_w = destination_w / original_w;
	qreal scale_h = destination_h / original_h;

	if (scale_w < scale_h)
		painter_scale = scale_w;
	else
		painter_scale = scale_h;	
	
	printer_scale->setText(QString::number(arrotonda_a_intero(painter_scale * 100.0)) + "%");
}

QHBoxLayout	*win_print::setup_buttons()
{
	QHBoxLayout	*L_comandi = new QHBoxLayout();
	L_comandi->setSpacing(6);
	L_comandi->setMargin(0);
        QSpacerItem	*L_comandi_molla = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	L_comandi->addItem(L_comandi_molla);
	QPushButton	*L_comandi_OK = new QPushButton(this);
	L_comandi_OK->setText(tr("Ok"));
	L_comandi->addWidget(L_comandi_OK);
	QPushButton	*L_comandi_Cancel = new QPushButton(this);
	L_comandi_Cancel->setText(tr("Cancel"));
	L_comandi->addWidget(L_comandi_Cancel);

	QObject::connect(L_comandi_OK, SIGNAL(clicked()), this, SLOT(accept()));
	QObject::connect(L_comandi_Cancel, SIGNAL(clicked()), this, SLOT(reject()));

        L_comandi_OK->setAutoDefault(TRUE);
	L_comandi_Cancel->setAutoDefault(FALSE);

	return L_comandi;
}

void	win_print::printer_dialog()
{
	//Stranamente per cambiare il formato pagina della fotocopiatrice bisogna andare su "formato..." e non basta cliccare subito su A3
	//Per le altre stampanti, invece si riesce benissimo a fare il giochetto
	if (is_pdf)
		{
		win_pdf_print_dialog	Dialog(printer, this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
		if (Dialog.exec()) 
			{
			printer_size_and_orientation_to->setText(read_printer_settings());	//Lo metto per sicurezza
			if (GV->is_sch)
				read_scale_settings();
			}
		}
	else
		{
		QPrintDialog	Dialog(printer, this);
		if (Dialog.exec()) 
			{
			printer_name->setText(printer->printerName());
			printer_size_and_orientation_to->setText(read_printer_settings());	//Lo metto per sicurezza
			if (GV->is_sch)
				read_scale_settings();
			}
		}	
}

void	win_print::change_pcb_scale(bool inutile)
{
	if (scale_1_1->isChecked())
		painter_scale = 1.0;
	else if (scale_2_1->isChecked())
		painter_scale = 2.0;
	else if (scale_1_2->isChecked())
		painter_scale = 0.5;
}

void	win_print::change_colormode(bool inutile)
{
        if (colormode_color->isChecked())
                colormode = 'C';
        else if (colormode_grayscale->isChecked())
                colormode = 'G';
        else if (colormode_black->isChecked())
                colormode = 'B';
}

void win_print::accept()
{
        QColor layer_black[LAYER_DATA_COUNT];
        QColor workspace_black[WORKSPACE_DATA_COUNT];

        if (!GV->is_sch)
                {
                //Controllo prima che tutti i parametri inseriti siano corretti
                if (offset_x->warning_not_valid_parameter("X"))
                        return;
                if (offset_y->warning_not_valid_parameter("Y"))
                        return;
                }

	printer->setFullPage(TRUE);

        if (GV->is_sch && (file_type == FT_PROJECT))
                {
                if (GV->page_width < GV->page_height)
                        printer->setOrientation(QPrinter::Portrait);
                else
                        printer->setOrientation(QPrinter::Landscape);
                }
	
	QString file_choosen;
	if (is_pdf)
		{
                file_choosen = save_pdf(this, "");

                if (file_choosen.isEmpty())
			return;	
                printer->setOutputFileName(file_choosen);
		//Imposto printer alla risoluzione 1200 che è molto valida
                printer->setResolution(1200);
		}
        else
                //Stampo attraverso un immagine per cui come impegno di memoria, ma anche come qualità del risultato conviene lavorare a 300dpi
                printer->setResolution(300);
        ////////////////////////////////////////////////////////////////////////////
        // Attenzione: quando si stampa con comandi grafici e non si passa da un immagine
        // mai usare setResolution su stampanti fisiche (non pdf) perchè
        // mi genera dei problemi come linee inclinate con spessore diverso...
        // Non usare printer->setResolution(printer->supportedResolutions().last());
        ////////////////////////////////////////////////////////////////////////////

        qreal scalex = (qreal) printer->logicalDpiX() / 100000.0;
	qreal scaley = (qreal) printer->logicalDpiY() / 100000.0;

	QMatrix m;

        //Mi traslo al centro della pagina di stampa
        QPoint center_page = printer->paperRect().center();
        m.translate(center_page.x(), center_page.y());
        if (GV->is_sch && (file_type == FT_PROJECT))
                m.scale(scalex, scaley);
        else
		{
                QRect all_rect = QRect(0,0,0,0);
                foreach(myGraphicsItem *obj, GS->all_obj)
                        all_rect = all_rect.unite(obj->boundingRect());
                QPoint center_GS = all_rect.center();
                QPoint offset = QPoint(offset_x->length, offset_y->length);
                if (!GV->is_sch)
                        {
                        if (scale_1_2->isChecked())
                                {
                                center_GS = center_GS / 2;
                                offset = offset / 2;
                                scalex /= 2;
                                scaley /= 2;
                                }
                        else if (scale_2_1->isChecked())
                                {
                                center_GS = center_GS * 2;
                                offset = offset * 2;
                                scalex *= 2;
                                scaley *= 2;
                                }
                        }
                m.scale(scalex, scaley);
                //Mi traslo al centro della scena
                m.translate(-center_GS.x(), -center_GS.y());
		//Mi traslo di offset_x e offset_y
                if (!GV->is_sch)
                        m.translate(offset.x(), offset.y());
		}
	
        m = m.scale(painter_scale, painter_scale);

        if (colormode == 'C')
                printer->setColorMode(QPrinter::Color);
        else
                printer->setColorMode(QPrinter::GrayScale);

        if (colormode == 'B')
                {
                for (int i = 0; i < LAYER_DATA_COUNT; i++)
                        {
                        layer_black[i] = QColor(0,0,0);
                        swap_qcolor(layer_data[i].color, layer_black[i]);
                        }
                for (int i = 0; i < WORKSPACE_DATA_COUNT; i++)
                        {
                        workspace_black[i] = QColor(0,0,0);
                        swap_qcolor(workspace_data[i].color, workspace_black[i]);
                        }
                workspace_data[W_PCBbackground].color = QColor(255,255,255);//Con questa riga eseguo i drill
                }

        QMatrix old_matrix = GV->matrix();
        QPainter painter_printer;
        if (is_pdf)
                {
                //Disegno direttamente senza creare grosse immagini in memoria
                if (!painter_printer.begin(printer))
                        {
                        QMessageBox::warning(this, QObject::tr("Error"), QObject::tr("Print error.").arg(file_choosen));
                        return;
                        }
                GS->draw_for_printer(&painter_printer, &m);
                painter_printer.end();
                }
        else
                {
                //Creo una grossa immagine in memoria, ci disegno e poi la stampo. In questo modo la stampa è quasi uguale per qualsiasi stampante
                //Sicuramente è più uguale da stampante a stampante che non se usassi i comandi grafici.
                //Stampare attraverso un immagine mi consentirebbe di visualizzare le sovrapposizioni delle piste nei PCB, ma è una cosa brutta a vedersi
                QImage image(printer->width(), printer->height(), QImage::Format_ARGB32_Premultiplied);
                QPainter painter_image;
                painter_image.begin(&image);
                GS->draw_for_printer(&painter_image, &m);
                painter_image.end();
                if (!painter_printer.begin(printer))
                        {
                        QMessageBox::warning(this, QObject::tr("Error"), QObject::tr("Print error.").arg(file_choosen));
                        return;
                        }
                painter_printer.drawImage(QPoint(0,0), image);
                painter_printer.end();
                }

        GV->setMatrix(old_matrix);

        if (colormode == 'B')
               {
               for (int i = 0; i < LAYER_DATA_COUNT; i++)
                        swap_qcolor(layer_data[i].color, layer_black[i]);
               for (int i = 0; i < WORKSPACE_DATA_COUNT; i++)
                        swap_qcolor(workspace_data[i].color, workspace_black[i]);
               }

        QDialog::accept();
}
