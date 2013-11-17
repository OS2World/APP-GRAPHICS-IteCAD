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

#include "win_edit_layers.h"

extern	win_main	*w_main;
extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

win_edit_layers::win_edit_layers(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

        setWindowTitle(tr("Layers"));
	setModal(true);

	QGridLayout	*layout = new QGridLayout(this);

	My_Layers	*tab_lay;	
	tab_lay = new My_Layers();
	tab_lay->tipo_tabella = 'V';
	tab_lay->load_data();

	//Imposto, se possibile, l'altezza giusta per i layer
        QSize right_size = tab_lay->right_size();
        int right_height = right_size.height();
	QRect screen = QApplication::desktop()->screenGeometry();
	if (right_height < screen.height() * 2 / 3)
                tab_lay->setFixedHeight(right_size.height());
	
	layout->addWidget(tab_lay, 0, 0);
	//layout->addWidget(setup_options(), 0, 1);
	layout->addLayout(setup_buttons(),1,0,TRUE,FALSE);
	layout->setSpacing(6);
    layout->setMargin(8);
}

/*QGroupBox *win_edit_layers::setup_options()
{
	QGroupBox *groupBox = new QGroupBox(tr("Options"));
	QGridLayout	*layout_general = new QGridLayout();

	QLabel *num_copper_layers = new QLabel(this);
	num_copper_layers->setText(tr("Copper Layers:"));
	num_copper_layers->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	Num_copper_layers = new QSpinBox(this);
	Num_copper_layers->setRange(1, 16);
	QObject::connect(Num_copper_layers, SIGNAL(valueChanged(int)), this, SLOT(changed_copper_layers(int)));

	QLabel *num_help_layers = new QLabel(this);
	num_help_layers->setText(tr("Help Layers:"));
	num_help_layers->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	Num_help_layers = new QSpinBox(this);
	Num_help_layers->setRange(0, 4);	
	QObject::connect(Num_help_layers, SIGNAL(valueChanged(int)), this, SLOT(changed_help_layers(int)));
	
	layout_general->addWidget(num_copper_layers, 0, 0);
	layout_general->addWidget(Num_copper_layers, 0, 1);
	layout_general->addWidget(num_help_layers, 1, 0);
	layout_general->addWidget(Num_help_layers, 1, 1);
	layout_general->setRowStretch(2, TRUE);
	
	groupBox->setLayout(layout_general);

	return groupBox;
}*/

QHBoxLayout	*win_edit_layers::setup_buttons()
{
	QHBoxLayout	*L_comandi = new QHBoxLayout();
	L_comandi->setSpacing(6);
	L_comandi->setMargin(0);
	QSpacerItem	*L_comandi_molla = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	L_comandi->addItem(L_comandi_molla);
	QPushButton	*L_comandi_OK = new QPushButton(this);
	L_comandi_OK->setText(tr("Ok"));
	L_comandi->addWidget(L_comandi_OK);
	
	QObject::connect(L_comandi_OK, SIGNAL(clicked()), this, SLOT(accept()));

	L_comandi_OK->setAutoDefault(TRUE);
	
	return L_comandi;
}

void win_edit_layers::changed_copper_layers(int inutile)
{
	return;

	QMessageBox::warning(0,tr("Warning"),"Se si sta eliminando un layer di copper contenente oggetti allora bisogna chiedere conferma prima di procedere.\n\n I layer di un multistrato a 4 strati sono 4 e si chiamano Copper Top, Copper 2, Copper 3, Copper Bottom");
}

void win_edit_layers::changed_help_layers(int inutile)
{
	return;

	QMessageBox::warning(0,tr("Warning"),"Se si sta eliminando un layer di help contenente oggetti allora bisogna chiedere conferma prima di procedere.");
}

void win_edit_layers::accept()
{
	w_main->layer_set_current();
	QDialog::accept();
}
