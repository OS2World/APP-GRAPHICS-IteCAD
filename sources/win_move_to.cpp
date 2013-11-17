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

#include "win_move_to.h"

extern	myGraphicsView	*GV;
extern	win_main	*w_main;

win_move_to::win_move_to(QWidget *parent, Qt::WFlags tipo, bool show_a, int ax, int ay)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help			//Non serve perchè uso tw->setUsesScrollButtons(FALSE); 
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni		//Non serve perchè uso tw->setUsesScrollButtons(FALSE); 

	setWindowTitle(w_main->actions_moveToAct->text());	
	//setSizeGripEnabled(true);
    setModal(true);

	//Imposto la larghezza minima pari a metà dello schermo
	//QRect screen = QApplication::desktop()->screenGeometry();
	//setMinimumWidth(screen.width() / 2);

	show_absolute = show_a;
	absolute_x = ax;
	absolute_y = ay;

	QGridLayout	*layout = new QGridLayout(this);
	layout->addWidget(setup_position(),0,0);
	layout->addLayout(setup_buttons(),1,0);
	layout->setSpacing(6);
    layout->setMargin(8);
	//QObject::connect(lista, SIGNAL(itemSelectionChanged()), this, SLOT(selection_changed()));
}

QGroupBox		*win_move_to::setup_position()
{
	QGroupBox *groupBox = new QGroupBox(tr("Move"));

	if (show_absolute)
		{
		is_relative = new QCheckBox(tr("Relative Position"));
		connect(is_relative,SIGNAL(stateChanged(int)),this,SLOT(change_is_relative(int)));
		}
	p_x = new QLabel(this);	
	p_x->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	pos_x = new layout_length("", 0, -2000000, 2000000, FALSE);
	p_y = new QLabel(this);	
	p_y->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	pos_y = new layout_length("", 0, -2000000, 2000000, FALSE);
	QGridLayout *gbox = new QGridLayout();
	int row = 0;
	if (show_absolute)
		{
		gbox->addWidget(is_relative,row++,0, TRUE, FALSE);
		is_relative->setChecked(TRUE);
		}
	change_is_relative(TRUE);
	gbox->addWidget(p_x,row,0);
	gbox->addLayout(pos_x,row++,1);
	gbox->addWidget(p_y,row,0);
	gbox->addLayout(pos_y,row,1);
	groupBox->setLayout(gbox);	
	
	return groupBox;
}

QHBoxLayout	*win_move_to::setup_buttons()
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

	return L_comandi;
}

void	win_move_to::change_is_relative(int new_value)
{
	if (new_value)
		{
		//Movimento relativo
		p_x->setText("      " + QString(&QChar(916),1) + "X");
		p_y->setText("      " + QString(&QChar(916),1) + "Y");
		pos_x->setLength(0);
		pos_y->setLength(0);
		}
	else
		{
		//Movimento assoluto
		p_x->setText("      X");
		p_y->setText("      Y");
		pos_x->setLength(absolute_x);
		pos_y->setLength(absolute_y);
		}
}

void	win_move_to::accept()
{
	QDialog::accept();
}
