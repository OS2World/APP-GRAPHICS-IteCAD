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

#include "win_undo_redo_history.h"

extern	win_main	*w_main;
extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

win_undo_redo_history::win_undo_redo_history(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
	{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

	setWindowTitle(tr("Undo/Redo History"));	
	setModal(true);

	QGridLayout	*layout = new QGridLayout(this);

        myUndoView *view = new myUndoView(&GV->ur, 0);
	view->scrollToTop();//(view->currentIndex(), QAbstractItemView::PositionAtCenter);
	
	layout->addWidget(view, 0, 0);
	layout->addLayout(setup_buttons(),1,0);
	layout->setSpacing(6);
    layout->setMargin(8);

	QObject::connect(&(GV->ur), SIGNAL(indexChanged(int)), this, SLOT(change_index(int)));

	//Imposto la larghezza massima pari a metà dello schermo
	//QRect screen = QApplication::desktop()->screenGeometry();
	//this->setFixedWidth(screen.width() / 2);

	}

QHBoxLayout	*win_undo_redo_history::setup_buttons()
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

void win_undo_redo_history::accept()
{
        GV->select_clear();

	QDialog::accept();
}

void	win_undo_redo_history::change_index(int index)
{
	//Ogni volta che cambio la posizione all interno di ur devo deselezionare tutto.
	//Se non lo facessi avrei delle selezioni completamente sballate.

        GV->select_clear();
}
