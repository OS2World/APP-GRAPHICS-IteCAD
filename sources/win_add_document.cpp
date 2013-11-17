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

#include "win_add_document.h"

extern	win_main		*w_main;

extern	QList<myGraphicsView *> doc_sch;
extern	myGraphicsView			*doc_pcb;
extern	QList<myGraphicsView *> doc_sch_and_pcb;
extern	QList<QTextEdit *>		doc_txt;
extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

win_add_document::win_add_document(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

        setWindowTitle(tr("Add"));
	//setSizeGripEnabled(true);
	setModal(true);

	QGridLayout	*layout = new QGridLayout(this);
	
	layout->addWidget(setup_type(),0,0);
	layout->addLayout(setup_buttons(),1,0);
	layout->setSpacing(6);
    layout->setMargin(8);
}

QGroupBox *win_add_document::setup_type()
{
	QGroupBox *groupBox = new QGroupBox(tr("Type"));

	QGridLayout	*layout_type = new QGridLayout();

        T_sch = new QLineEdit(this);
        T_sch->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	T_sch->setText(w_main->documents_new_name(DT_SCH));

        T_pcb = new QLineEdit(this);
        T_pcb->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	T_pcb->setText(w_main->documents_new_name(DT_PCB));
	T_pcb->setVisible(FALSE);

        T_text = new QLineEdit(this);
        T_text->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	T_text->setText(w_main->documents_new_name(DT_TXT));
	T_text->setVisible(FALSE);

        doc_is_sch = new QRadioButton(tr("New Schematic"));
	doc_is_sch->setIconSize(QSize(24,22));
	doc_is_sch->setIcon(QIcon(":/Resources/schematic.png"));
	doc_is_sch->setChecked(TRUE);

	doc_is_pcb = new QRadioButton(tr("New Board"));
	doc_is_pcb->setIconSize(QSize(24,22));
	doc_is_pcb->setIcon(QIcon(":/Resources/board.png"));
	//Controllo se è possibile inserire un Board
	if (doc_pcb)
		doc_is_pcb->setEnabled(FALSE);
	else
		doc_is_pcb->setEnabled(TRUE);
	
	doc_is_text = new QRadioButton(tr("New Info"));
	doc_is_text->setIconSize(QSize(24,22));
	doc_is_text->setIcon(QIcon(":/Resources/script.png"));
	
	QObject::connect(doc_is_sch, SIGNAL(toggled(bool)), this, SLOT(change_type(bool)));
	QObject::connect(doc_is_pcb, SIGNAL(toggled(bool)), this, SLOT(change_type(bool)));
	QObject::connect(doc_is_text, SIGNAL(toggled(bool)), this, SLOT(change_type(bool)));
	
	layout_type->addWidget(doc_is_sch,0,0);
	layout_type->addWidget(T_sch,0,1);
	layout_type->addWidget(doc_is_pcb,1,0);
	layout_type->addWidget(T_pcb,1,1);
	layout_type->addWidget(doc_is_text,2,0);
	layout_type->addWidget(T_text,2,1);

	groupBox->setLayout(layout_type);

	return groupBox;
}

QHBoxLayout	*win_add_document::setup_buttons()
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

	return L_comandi;
}

void	win_add_document::accept()
{
	if (doc_is_sch->isChecked())
		{
		if (w_main->documents_valid_name(T_sch->text(), -1))
			w_main->documents_add_new(DT_SCH, T_sch->text());
		else
			return;
		}
	else if (doc_is_pcb->isChecked())
		{
		if (w_main->documents_valid_name(T_pcb->text(), -1))
			w_main->documents_add_new(DT_PCB, T_pcb->text());
		else
			return;
		}
	else if (doc_is_text->isChecked())
		{
		if (w_main->documents_valid_name(T_text->text(), -1))
			w_main->documents_add_new(DT_TXT, T_text->text());
		else
			return;
		}

	QDialog::accept(); 
}

void	win_add_document::change_type(bool checked)
{
	if (doc_is_sch->isChecked())
		{
		T_sch->setVisible(TRUE);
		T_pcb->setVisible(FALSE);
		T_text->setVisible(FALSE);
		}
	else if (doc_is_pcb->isChecked())
		{
		T_sch->setVisible(FALSE);
		T_pcb->setVisible(TRUE);
		T_text->setVisible(FALSE);
		}
	else if (doc_is_text->isChecked())
		{
		T_sch->setVisible(FALSE);
		T_pcb->setVisible(FALSE);
		T_text->setVisible(TRUE);
		}
}



