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

#include "win_gerber.h"

extern	QString prog_version_string;

extern	win_main	*w_main;
extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

extern  QString file_name;
extern  QString file_path;

extern  QPolygon    text_points;
extern  QPolygon    text_lines;

QPolygon    polygon_interpolation;

win_gerber::win_gerber(QWidget *parent, Qt::WFlags tipo, bool is_board_and_drills_input)
        : QDialog(parent, tipo)
{
        // Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

        is_board_and_drills = is_board_and_drills_input;

        if (is_board_and_drills)
                setWindowTitle(tr("Board and Drills"));
        else
                setWindowTitle(tr("Gerber"));
	setModal(true);

	QGridLayout	*layout = new QGridLayout(this);

        if (is_board_and_drills)
                {
                layout->addWidget(setup_options(), 0, 0);
                layout->addLayout(setup_buttons(),1,0,TRUE,FALSE);
                }
        else
                {
                tab_lay = new My_Layers();
                tab_lay->tipo_tabella = 'G';
                tab_lay->load_data();

                //Imposto, se possibile, l'altezza giusta per i layer
                QSize right_size = tab_lay->right_size();
                int right_height = right_size.height();
                QRect screen = QApplication::desktop()->screenGeometry();
                if (right_height < screen.height() * 2 / 3)
                        tab_lay->setFixedHeight(right_size.height());

                layout->addWidget(tab_lay, 0, 0);
                layout->addWidget(setup_options(), 1, 0);
                layout->addLayout(setup_buttons(),2,0,TRUE,FALSE);
                }

	layout->setSpacing(6);
        layout->setMargin(8);
}

QGroupBox *win_gerber::setup_options()
{
        QGroupBox *groupBox = new QGroupBox(tr("Options"));
	QGridLayout	*layout_general = new QGridLayout();

        QLabel *root_filename = new QLabel(this);
        if (is_board_and_drills)
                root_filename->setText(tr("Enter Filename:"));
        else
                root_filename->setText(tr("Enter Root Filename:"));
        QHBoxLayout *riga_root_filename = new QHBoxLayout(0);
        if (is_board_and_drills)
                Root_filename = new QLineEdit(QFileInfo(file_path).completeBaseName() + "_board_and_drills");
        else
                Root_filename = new QLineEdit(QFileInfo(file_path).completeBaseName());
        riga_root_filename->addSpacing(30);
        riga_root_filename->addWidget(Root_filename);

        QLabel *directory = new QLabel(this);
        directory->setText(tr("Enter Directory Path:"));
        QHBoxLayout *riga_dir = new QHBoxLayout(0);
        Directory = new QLineEdit(QFileInfo(file_path).path());
        QToolButton	*dir_change = new QToolButton(this);
        //dir_change->setText(tr("Browse"));
        dir_change->setIcon(QIcon(":/Resources/folder.png"));
        dir_change->setAutoRaise(TRUE);
        dir_change->setCheckable(FALSE);
        connect(dir_change,SIGNAL(clicked()),this,SLOT(change_directory()));
        riga_dir->addSpacing(30);
        riga_dir->addWidget(Directory);
        riga_dir->addWidget(dir_change);

        layout_general->addWidget(root_filename, 0, 0);
        layout_general->addLayout(riga_root_filename, 1, 0, TRUE, FALSE);
        QSpacerItem *spazio = new QSpacerItem(20,20);
        layout_general->addItem(spazio,2,0);
        layout_general->addWidget(directory, 3, 0);
        layout_general->addLayout(riga_dir, 4, 0, TRUE, FALSE);
        layout_general->setRowStretch(4, TRUE);
	
	groupBox->setLayout(layout_general);

        return groupBox;
}

QHBoxLayout	*win_gerber::setup_buttons()
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

void win_gerber::accept()
{
        //Controllo se i file sono già presenti e nel qual caso chiedo all'utente se vuole sovrascriverli
        int num_file_to_overwrite = 0;
        if (is_board_and_drills)
                {
                QFileInfo fi(Directory->text() + "/" + Root_filename->text() + ".gbr");   //L'estensione corretta è .gbr
                if (fi.exists())
                        num_file_to_overwrite++;
                }
        else
                {
                for (int i=LAYER_DATA_SCH_COUNT; i < LAYER_DATA_COUNT; i++)
                        if (GV->layer_settings[i].gerber)
                                {
                                QFileInfo fi(Directory->text() + "/" + Root_filename->text() + "_" + xml_write_objlayer(i) + ".gbr");   //L'estensione corretta è .gbr
                                if (fi.exists())
                                        num_file_to_overwrite++;
                                }
                QFileInfo fi(Directory->text() + "/" + Root_filename->text() + "_Hole" + ".drl");
                if (fi.exists())
                        num_file_to_overwrite++;
                }
        if (num_file_to_overwrite == 1)
                switch (QMessageBox::warning(this, "Itecad", QObject::tr("File already exists. Overwrite?"), tr("&Yes"), tr("&No"), 0))
                {
                case 0:		// 0 = YES
                        break;
                case 1:		// 1 = NO
                        return;
                }
        else if (num_file_to_overwrite == 1)
                switch (QMessageBox::warning(this, "Itecad", QObject::tr("%1 file(s) already exists. Overwrite?").arg(QString::number(num_file_to_overwrite)), tr("&Yes"), tr("&No"), 0))
                {
                case 0:		// 0 = YES
                        break;
                case 1:		// 1 = NO
                        return;
                }

        if (is_board_and_drills)
                {
                QFile file(Directory->text() + "/" + Root_filename->text() + ".gbr"); //L'estensione corretta è .gbr
                if (file.open(QFile::WriteOnly | QFile::Truncate))
                        {
                        QTextStream out(&file);
                        out.setCodec("UTF-8");	//Attenzione: UTF-8 usa 8bit per carattere ma riesce a scrivere tutto l'unicode usando sequenze multibyte quando necessario
                        gerber_write(out, L_PCBBoard);
                        }
                else
                        QMessageBox::warning(w_main, "Itecad", QObject::tr("Cannot write file %1:\n%2").arg(file.fileName()).arg(file.errorString()));
                }
        else
                for (int i=LAYER_DATA_SCH_COUNT; i < LAYER_DATA_COUNT; i++)
                        if (GV->layer_settings[i].gerber)
                                {
                                QFile file(Directory->text() + "/" + Root_filename->text() + "_" + xml_write_objlayer(i) + ".gbr"); //L'estensione corretta è .gbr
                                if (file.open(QFile::WriteOnly | QFile::Truncate))
                                        {
                                        QTextStream out(&file);
                                        out.setCodec("UTF-8");	//Attenzione: UTF-8 usa 8bit per carattere ma riesce a scrivere tutto l'unicode usando sequenze multibyte quando necessario
                                        gerber_write(out, i);
                                        }
                                else
                                        QMessageBox::warning(w_main, "Itecad", QObject::tr("Cannot write file %1:\n%2").arg(file.fileName()).arg(file.errorString()));
                                }

        QDialog::accept();
}

void win_gerber::change_directory()
{
        QString directory = open_dir(this, tr("Choose Directory Path"), Directory->text());
        if (!directory.isEmpty())
                Directory->setText(directory);
}

QString win_gerber::gerber_number(int length, bool use_decimal_separator)
{
        //FSLAX24Y24
        //Approssimo con 2 interi e 4 decimali
        //L'unità di misura usata da questo programma è 1/100thou (10^-5 pollici) ossia 10 volte inferiore per cui devo dividere per 10
        //E' inutile produrre Gerber più risoluti perchè il processo chimico non può arrivare a precisioni superiori

        QString text = QString::number(abs(length/10));
        QString sign;
        if (length < 0)
                sign = "-";
        else
                sign = "";
        switch (text.length())
        {
        case 1:
                if (text == "0")
                        text = "1";
                if (use_decimal_separator)
                        return sign + "0.000" + text;
                else
                        return sign + "0000" + text;
        case 2:
                if (use_decimal_separator)
                        return sign + "0.00" + text;
                else
                        return sign + "000" + text;
        case 3:
                if (use_decimal_separator)
                        return sign + "0.0" + text;
                else
                        return sign + "00" + text;
        case 4:
                if (use_decimal_separator)
                        return sign + "0." + text;
                else
                        return sign + "00" + text;
        case 5:
                if (use_decimal_separator)
                        return sign + text.insert(1,".");
                else
                        return sign + "0" + text;
        case 6:
                if (use_decimal_separator)
                        return sign + text.insert(2,".");
                else
                        return sign + text;
        }
}

QString    win_gerber::gerber_ad_definition(myGraphicsItem *obj)
{
        switch (obj->tipo)
        {
        case O_PAD:
        case O_VIA:
                if (is_board_and_drills)
                        return "C," + gerber_number(obj->drill,TRUE);
                else
                        switch (obj->shape)
                        {
                        case PS_ROUND:
                                if (obj->width == obj->height)
                                        return "C," + gerber_number(obj->width,TRUE);
                                else
                                        return "O," + gerber_number(obj->width,TRUE) + "X" + gerber_number(obj->height,TRUE);
                                break;
                        case PS_RECTANGLE:
                                return "R," + gerber_number(obj->width,TRUE) + "X" + gerber_number(obj->height,TRUE);
                                break;
                        case PS_ROUNDED_RECTANGLE:
                                if (obj->width < obj->height)
                                        return "C," + gerber_number(obj->width,TRUE);
                                else
                                        return "C," + gerber_number(obj->height,TRUE);
                                break;
                        }
                break;
        case O_CONNECTION:
        case O_POLYLINE:
        case O_POLYGON:
        case O_RECT:
        case O_ELLIPSE:
                return "C," + gerber_number(obj->pen_width,TRUE);
        case O_TEXT:
        case O_IDENT:
                return "C," + gerber_number(obj->font_height*50,TRUE);
        }
        return "";
}

void        win_gerber::gerber_ad_insert(QString definition)
{
        if (definition.isEmpty())
                return;
        if (gerber_ad_find(definition).isEmpty())
                {
                ad_code.append("D" + QString::number(ad_code.count() + 10));
                ad_definition.append(definition);
                }
}

QString     win_gerber::gerber_ad_find(QString definition)
{
        for (int i = 0; i < ad_code.size(); ++i)
                if (ad_definition.at(i) == definition)
                        {
                        return ad_code.at(i);
                        }

        return "";
}

void    win_gerber::gerber_write_object(QTextStream &out, myGraphicsItem *obj)
{
        int x, y;
        switch (obj->tipo)
        {
        case O_PAD:
        case O_VIA:
                switch (obj->shape)
                {
                case PS_ROUND:
                case PS_RECTANGLE:
                        out << gerber_ad_find(gerber_ad_definition(obj)) << "*\n";
                        x = obj->node_start->point.x();
                        y = -obj->node_start->point.y();//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D03*\n";
                        break;
                case PS_ROUNDED_RECTANGLE:
                        out << gerber_ad_find(gerber_ad_definition(obj)) << "*\n";
                        if (obj->width > obj->height)
                                {
                                x = obj->node_start->point.x() - obj->width/2;
                                y = -obj->node_start->point.y();//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                                }
                        else
                                {
                                x = obj->node_start->point.x();
                                y = -obj->node_start->point.y()-obj->height/2;//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                                }
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D02*\n";
                        if (obj->width > obj->height)
                                x +=  obj->width;
                        else
                                y += obj->height;
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D01*\n";
                        break;
                }
                break;
        case O_CONNECTION:
                out << gerber_ad_find(gerber_ad_definition(obj)) << "*\n";
                x = obj->node_start->point.x();
                y = -obj->node_start->point.y();//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D02*\n";
                x = obj->node_end->point.x();
                y = -obj->node_end->point.y();//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D01*\n";
                break;
        case O_POLYLINE:
                out << gerber_ad_find(gerber_ad_definition(obj)) << "*\n";
                x = obj->polygon.at(0).x();
                y = -obj->polygon.at(0).y();//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D02*\n";
                for (int i = 1; i < obj->polygon.count();i++)
                        {
                        x = obj->polygon.at(i).x();
                        y = -obj->polygon.at(i).y();//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D01*\n";
                        }
                break;
        case O_POLYGON:
                if (obj->fill_polygon)
                        ///////////////////////////////////
                        ;   //Scrivere il codice in seguito
                        ///////////////////////////////////
                else
                        {
                        out << gerber_ad_find(gerber_ad_definition(obj)) << "*\n";
                        x = obj->polygon.at(0).x();
                        y = -obj->polygon.at(0).y();//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D02*\n";
                        for (int i = 1; i < obj->polygon.count();i++)
                                {
                                x = obj->polygon.at(i).x();
                                y = -obj->polygon.at(i).y();//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                                out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D01*\n";
                                }
                        x = obj->polygon.at(0).x();
                        y = -obj->polygon.at(0).y();//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D01*\n";
                        }
                break;
        case O_ELLIPSE:
                if (obj->fill_polygon)
                        ///////////////////////////////////
                        ;   //Scrivere il codice in seguito
                        ///////////////////////////////////
                else
                        {
                        obj->polygon_interpolation(&polygon_interpolation);
                        out << gerber_ad_find(gerber_ad_definition(obj)) << "*\n";
                        x = polygon_interpolation.at(0).x();
                        y = -polygon_interpolation.at(0).y();//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D02*\n";
                        for (int i = 1; i < polygon_interpolation.count();i++)
                                {
                                x = polygon_interpolation.at(i).x();
                                y = -polygon_interpolation.at(i).y();//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                                out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D01*\n";
                                }
                        x = polygon_interpolation.at(0).x();
                        y = -polygon_interpolation.at(0).y();//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D01*\n";
                        }
                break;
        case O_RECT:
                if (obj->fill_polygon)
                        ///////////////////////////////////
                        ;   //Scrivere il codice in seguito
                        ///////////////////////////////////
                else
                        {
                        out << gerber_ad_find(gerber_ad_definition(obj)) << "*\n";
                        x = obj->start.x();// - obj->width/2;
                        y = -obj->start.y();// - obj->height/2;
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D02*\n";
                        x += obj->width;
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D01*\n";
                        y -= obj->height;
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D01*\n";
                        x -= obj->width;
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D01*\n";
                        y += obj->height;
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D01*\n";
                        }
                break;
        case O_TEXT:
        case O_IDENT:
                out << gerber_ad_find(gerber_ad_definition(obj)) << "*\n";
                obj->text_to_polygon(&obj->text, obj->info_start_text, &text_points, &text_lines);
                for(int i = 0; i < text_points.count(); i++)
                        {
                        x = text_points.at(i).x();
                        y = -text_points.at(i).y();//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D03*\n";
                        }
                for(int i = 0; i < text_lines.count(); i++)
                        {
                        x = text_lines.at(i).x();
                        y = -text_lines.at(i++).y();//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D02*\n";
                        x = text_lines.at(i).x();
                        y = -text_lines.at(i).y();//URKA Inverto l'asse Y (cambiare direzione in seguito)//
                        out << "X" << gerber_number(x,FALSE) << "Y" << gerber_number(y,FALSE) << "D01*\n";
                        }
                break;
        }
}

void    win_gerber::gerber_write(QTextStream &out, int layer)
{
        out << "*G04 Gerber R-274X file produced by Itecad " + prog_version_string + "*\n";
        out << "%FSLAX24Y24*%\n";     //Format statement - leading zeroes omitted, absolute coordinates, X2.3, Y2.3.
        out << "%MOIN*%\n";           //Set units to inches.

        //Ricalcolo tutte le aperture definition utilizzate
        ad_code.clear();
        ad_definition.clear();
        myGraphicsItem *obj;
        int size = GS->all_obj.size();
        for (int i = 0; i < size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
                {
                obj = GS->all_obj.at(i);		//Nelle QList l'operatore at() è il più veloce
                switch (obj->tipo)
                {
                case O_CONNECTION:
                case O_POLYLINE:
                case O_POLYGON:
                case O_ELLIPSE:
                case O_RECT:
                case O_TEXT:
                case O_IDENT:
                        if (obj->layer_from == layer)
                                gerber_ad_insert(gerber_ad_definition(obj));
                        break;
                case O_PAD:
                case O_VIA:
                        if (is_board_and_drills && (obj->layer_from != obj->layer_to))
                                gerber_ad_insert(gerber_ad_definition(obj));
                        else if (obj->layer_intersect(layer, layer))
                                gerber_ad_insert(gerber_ad_definition(obj));
                        else if ((layer == L_PCBMaskTop) && (obj->layer_intersect(L_PCBCopper1, L_PCBCopper1)))
                                gerber_ad_insert(gerber_ad_definition(obj));
                        else if ((layer == L_PCBMaskBottom) && (obj->layer_intersect(L_PCBCopper16, L_PCBCopper16)))
                                gerber_ad_insert(gerber_ad_definition(obj));
                        break;
                }
                }

        //Scrivo tutte le aperture_definition
        size = ad_code.size();
        if (size)
                for (int i = 0; i < size; ++i)
                        out << "%AD" << ad_code.at(i) + ad_definition.at(i) + "*%\n";
        else
                out << "%ADD10C,0.0550*%";  //Aggiungo un apertura inutile perchè se non lo facessi i lettori di file Gerber come GC vanno in errore

        //Scrivo tutti gli oggetti
        size = GS->all_obj.size();
        for (int i = 0; i < size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
                {
                obj = GS->all_obj.at(i);		//Nelle QList l'operatore at() è il più veloce
                switch (obj->tipo)
                {
                case O_CONNECTION:
                case O_POLYLINE:
                case O_POLYGON:
                case O_ELLIPSE:
                case O_RECT:
                case O_TEXT:
                case O_IDENT:
                        if (obj->layer_from == layer)
                                gerber_write_object(out, obj);
                        break;
                case O_PAD:
                case O_VIA:
                        if (is_board_and_drills && (obj->layer_from != obj->layer_to))
                                gerber_write_object(out, obj);
                        else if (obj->layer_intersect(layer, layer))
                                gerber_write_object(out, obj);
                        else if ((layer == L_PCBMaskTop) && (obj->layer_intersect(L_PCBCopper1, L_PCBCopper1)))
                                gerber_write_object(out, obj);
                        else if ((layer == L_PCBMaskBottom) && (obj->layer_intersect(L_PCBCopper16, L_PCBCopper16)))
                                gerber_write_object(out, obj);
                        break;
                }
                }

        out << "M02*\n";
}




