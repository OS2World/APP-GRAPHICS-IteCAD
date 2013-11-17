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

#include "win_drill.h"

extern	QString prog_version_string;

extern	win_main	*w_main;
extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

extern  QString file_name;
extern  QString file_path;

extern  QPolygon    text_points;
extern  QPolygon    text_lines;



//////////////////////////////////////////////////////////////////////////////////////////////////
// Informazioni utili:
//
// - Si costruisce un file per ogni tipo di foratura
// - meno files si generano meno lavorazioni ha il PCB per cui costa meno
// - si può andare con il foro da un qualsiasi layer di piste ad un qualsiasi layer di piste
// - i formati più usati sono l'Excellon che uso qui oppure il Sieb&Mayer
// - normalmente viene fornito il file con la foratura di tutto il PCB insieme ad altri file tipo 01-02 dove i numeri indicano i layer di piste
//
// - Non si possono fare fori di diametro superiore ai 6,7mm perchè le macchine non lo consentono. Lo si riesce comunque a fare di fresa.
// - Abbiamo deciso di generare comunque i fori di diametro superiore ai 6,7mm e non avvisare l'utente.
//
// - I fori ciechi (blind) ossia quelli visibili solo da un lato del PCB devono avere diametro inferiore alla loro lunghezza
// - Questo vincolo è dovuto ai processi di metallizzazione
// - I fori passanti e quelli sommersi (buried) non hanno nessun vincolo
// - Non confondere le blind vias con le buried vias
//////////////////////////////////////////////////////////////////////////////////////////////////



win_drill::win_drill(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

        setWindowTitle(tr("Drill"));
	setModal(true);

	QGridLayout	*layout = new QGridLayout(this);

        layout->addWidget(setup_options(), 0, 0);
        layout->addLayout(setup_buttons(),1,0,TRUE,FALSE);
	layout->setSpacing(6);
        layout->setMargin(8);
}

QGroupBox *win_drill::setup_options()
{
        QGroupBox *groupBox = new QGroupBox(tr("Options"));
	QGridLayout	*layout_general = new QGridLayout();

        QLabel *root_filename = new QLabel(this);
        root_filename->setText(tr("Enter Root Filename:"));
        QHBoxLayout *riga_root_filename = new QHBoxLayout(0);
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

QHBoxLayout	*win_drill::setup_buttons()
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

void win_drill::accept()
{
        //Controllo se i file sono già presenti e nel qual caso chiedo all'utente se vuole sovrascriverli
        //Conviene cercare tutti i nomi possibili e non usare QDir con un filtro perchè potrei beccare qualche file gerber o altri file che non sono di tipo drill
        int num_file_to_overwrite = 0;
        for (int i=L_PCBCopper1; i < L_PCBCopper16; i++)
                for (int j=i+1; j <= L_PCBCopper16; j++)
                        {
                        QFileInfo fi(drill_filepath(i,j));
                        if (fi.exists())
                                num_file_to_overwrite++;
                        }
        if (num_file_to_overwrite)
                switch (QMessageBox::warning(this, "Itecad", QObject::tr("All %1 previous drill file(s) will be deleted. Proceed?").arg(QString::number(num_file_to_overwrite)), tr("&Yes"), tr("&No"), 0))
                        {
                        case 0:		// 0 = YES
                                break;
                        case 1:		// 1 = NO
                                return;
                        }

        //Cancello tutti i file precedenti
        for (int i=L_PCBCopper1; i < L_PCBCopper16; i++)
                for (int j=i+1; j <= L_PCBCopper16; j++)
                        {
                        QFileInfo fi(drill_filepath(i,j));
                        if (fi.exists())
                                {
                                QDir d;
                                d.remove(drill_filepath(i,j));
                                }
                        }

        //Trovo tutti i layer file che devo generare
        layer_from_to_list.clear();
        myGraphicsItem *obj;
        int size = GS->all_obj.size();
        for (int i = 0; i < size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
                {
                obj = GS->all_obj.at(i);		//Nelle QList l'operatore at() è il più veloce
                switch (obj->tipo)
                {
                case O_PAD:
                case O_VIA:
                        if (obj->layer_from != obj->layer_to)
                                {
                                int layer_from_to = obj->layer_from * 256 + obj->layer_to;
                                if (!layer_from_to_list.contains(layer_from_to))
                                        layer_from_to_list.append(layer_from_to);
                                }
                        break;
                }
                }

        foreach (int layer_from_to, layer_from_to_list)
                {
                char layer_from = layer_from_to / 256;
                char layer_to = layer_from_to % 256;
                QFile file(drill_filepath(layer_from, layer_to));
                if (file.open(QFile::WriteOnly | QFile::Truncate))
                        {
                        QTextStream out(&file);
                        out.setCodec("UTF-8");	//Attenzione: UTF-8 usa 8bit per carattere ma riesce a scrivere tutto l'unicode usando sequenze multibyte quando necessario
                        drill_write(out, layer_from, layer_to);
                        }
                else
                        QMessageBox::warning(w_main, "Itecad", QObject::tr("Cannot write file %1:\n%2").arg(file.fileName()).arg(file.errorString()));
                }

	QDialog::accept();
}

void win_drill::change_directory()
{
        QString directory = open_dir(this, tr("Choose Directory Path"), Directory->text());
        if (!directory.isEmpty())
                Directory->setText(directory);
}

void    win_drill::drill_write(QTextStream &out, char layer_from, char layer_to)
{
        //out << "%\n";   //Eagle lo fa ma non mi sembra che serve
        out << "M48\n"; //Start
        out << "M72\n"; //English Imperial Mode

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
                case O_PAD:
                case O_VIA:
                        if ((obj->layer_from == layer_from) && (obj->layer_to == layer_to))
                                drill_ad_insert(drill_ad_definition(obj));
                        break;
                }
                }

        //Scrivo tutte le aperture_definition
        size = ad_code.size();
        if (size)
                for (int i = 0; i < size; ++i)
                        out << ad_code.at(i) + ad_definition.at(i) + "\n";
        else
                out << "T01C0.0550\n";  //Aggiungo un apertura inutile perchè se non lo facessi i lettori di file Excellon Drill come GC vanno in errore

        out << "%\n";
        out << "G05\n";
        //Scrivo tutti i fori
        for (int j = 0; j < ad_code.size(); j++)
                {
                out << ad_code.at(j) << "\n";
                size = GS->all_obj.size();
                for (int i = 0; i < size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
                        {
                        obj = GS->all_obj.at(i);		//Nelle QList l'operatore at() è il più veloce
                        switch (obj->tipo)
                        {
                        case O_PAD:
                        case O_VIA:
                                if ((obj->layer_from == layer_from) && (obj->layer_to == layer_to))
                                        if (drill_ad_find(drill_ad_definition(obj)) == ad_code.at(j))
                                                {
                                                out << "X" + drill_number(obj->node_start->point.x()) << "Y" + drill_number(-obj->node_start->point.y()) << "\n";
                                                }
                                break;
                        }
                        }
                }
        out << "T00\n"; //Tool unload
        out << "M30\n"; //End
}

QString win_drill::drill_number(int length)
{
        //E' diversa da FSLAX24Y24 del Gerber
        QString text = QString::number(abs(length/10));
        QString sign;
        if (length < 0)
                sign = "-";
        else
                sign = "";

        return sign + text;
}

QString win_drill::drill_ad_definition(myGraphicsItem *obj)
{
        return "C" + drill_number(obj->drill);
}

void    win_drill::drill_ad_insert(QString definition)
{
        if (definition.isEmpty())
                return;
        if (drill_ad_find(definition).isEmpty())
                {
                ad_code.append("T" + QString::number(ad_definition.count() + 1));
                ad_definition.append(definition);
                }
}

QString win_drill::drill_ad_find(QString definition)
{
        int length = definition.length();
        for (int i = 0; i < ad_code.size(); ++i)
                if (ad_definition.at(i).right(length) == definition)
                        return ad_code.at(i);

        return "";
}

QString win_drill::drill_filepath(char layer_from, char layer_to)
{
        //Penso che conviene scrivere da che layer a quale layer si va in maniera prolissa (elimino solo la parola Copper per non dilungarmi troppo)
        QString from = xml_write_objlayer(layer_from).remove("Copper");
        QString to = xml_write_objlayer(layer_to).remove("Copper");


        return Directory->text() + "/" + Root_filename->text() + "_from_" + from + "_to_" + to + ".drl";
}




