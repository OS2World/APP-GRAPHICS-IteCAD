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

#include "win_doc_manager.h"

extern	win_main		*w_main;

extern	QList<myGraphicsView *> doc_sch;
extern	myGraphicsView			*doc_pcb;
extern	QList<myGraphicsView *> doc_sch_and_pcb;
extern	QList<QTextEdit *>		doc_txt;
extern	myGraphicsView	*GV;

win_doc_manager::win_doc_manager(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

	setWindowTitle(tr("Documents Manager"));	
	//setSizeGripEnabled(true);
	setModal(true);	

	QGridLayout	*layout = new QGridLayout(this);
	
	layout->addWidget(setup_documents(),0,0);
	QGridLayout *command = new QGridLayout();
	//command->addWidget(setup_add(),0,0);
	//command->addWidget(setup_remove(),1,0);
	//command->addWidget(setup_rename(),2,0);
	//QSpacerItem *molla = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
	//command->addItem(molla,3,0,TRUE,FALSE);
	layout->addLayout(command,0,1);
	layout->addLayout(setup_buttons(),1,0, TRUE, FALSE);
	layout->setSpacing(6);
    layout->setMargin(8);

	setup_popupmenu();
}

void	win_doc_manager::load_data()
{
	documents.clear();
	
	for (int i = 0; i < w_main->tw->count(); i++)
		{
		QListWidgetItem *descr;
		switch (w_main->doc_type(w_main->tw->widget(i)))
		{
		case DT_SCH:
			{
			myGraphicsView *GVi = (myGraphicsView *) w_main->tw->widget(i);
			descr = new QListWidgetItem(QIcon(":/Resources/schematic.png"), GVi->name);
			}
			break;
		case DT_PCB:
			{
			myGraphicsView *GVi = (myGraphicsView *) w_main->tw->widget(i);
			descr = new QListWidgetItem(QIcon(":/Resources/board.png"), GVi->name);
			}
			break;
		case DT_TXT:
			{
			QTextEdit *TEi = (QTextEdit *) w_main->tw->widget(i);
			descr = new QListWidgetItem(QIcon(":/Resources/script.png"), TEi->objectName());
			}
			break;
		}
		documents.addItem(descr);
		}
	documents.setCurrentRow(w_main->tw->currentIndex());
}

QGroupBox *win_doc_manager::setup_documents()
{
	QGroupBox *groupBox = new QGroupBox(tr("Documents"));

	QGridLayout	*layout_documents = new QGridLayout();
	
	documents.setSelectionMode(QAbstractItemView::SingleSelection);
	documents.setIconSize(QSize(24,22));
	/////////////////////////////////////////////////////////////
	//Non riesco ad utilizzare il drag&drop
	//documents.setDragDropMode(QAbstractItemView::InternalMove);
	//Magari con le future versioni di Qt si riuscirà a fare un Drag&Drop anche su w_main->tw e draggare i tab
	//Il problema del drag&drop di documents è che non riesco a rilevare quando è avvenuto...
	//Meglio usare solo up e down
	/////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////
	// Per editare direttamente basta fare
	// descr->setFlags(descr->flags() | Qt::ItemIsEditable);	//Grazie a questa riga è editabile
	// documents.setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
	// ma purtroppo non è molto usabile 
	/////////////////////////////////////////////////////////////
	documents.setContextMenuPolicy(Qt::CustomContextMenu);
	
	L_add = new QPushButton(this);
	L_add->setText(tr("Add"));
	L_add->setIcon(QIcon(":/Resources/add.png"));
	L_add->setAutoDefault(FALSE);
	QObject::connect(L_add, SIGNAL(clicked()), this, SLOT(add()));
	
	QPushButton	*L_remove_doc = new QPushButton(this);
	L_remove_doc->setText(tr("Remove"));
	L_remove_doc->setIcon(QIcon(":/Resources/delete.png"));
	L_remove_doc->setAutoDefault(FALSE);
	QObject::connect(L_remove_doc, SIGNAL(clicked()), this, SLOT(remove_doc()));

	QPushButton	*L_rename_doc = new QPushButton(this);
	L_rename_doc->setText(tr("Rename"));
	L_rename_doc->setIcon(QIcon(":/Resources/textfield_rename.png"));
	L_rename_doc->setAutoDefault(FALSE);
	QObject::connect(L_rename_doc, SIGNAL(clicked()), this, SLOT(rename_doc()));

	QHBoxLayout	*L_move = new QHBoxLayout();
	L_move_up = new QPushButton(this);
	L_move_up->setIcon(QIcon(":/Resources/arrow_up.png"));
	L_move_up->setAutoDefault(FALSE);
	QObject::connect(L_move_up, SIGNAL(clicked()), this, SLOT(up_doc()));
	L_move_down = new QPushButton(this);
	L_move_down->setIcon(QIcon(":/Resources/arrow_down.png"));
	L_move_down->setAutoDefault(FALSE);
	QObject::connect(L_move_down, SIGNAL(clicked()), this, SLOT(down_doc()));
	L_move->addWidget(L_move_up);
	L_move->addWidget(L_move_down);

	layout_documents->addWidget(&documents, 0, 0, FALSE, TRUE);
	layout_documents->addWidget(L_add, 0, 1);
//	layout_documents->setRowMinimumHeight(1, L_add->height());
	layout_documents->addWidget(L_remove_doc, 2, 1);
//	layout_documents->setRowMinimumHeight(3, L_add->height());
	layout_documents->addWidget(L_rename_doc, 4, 1);
//	layout_documents->setRowMinimumHeight(5, L_add->height());
	layout_documents->addLayout(L_move, 6, 1);
	QSpacerItem *molla = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
	layout_documents->addItem(molla, 7, 1);

	groupBox->setLayout(layout_documents);

	QObject::connect(&documents, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(popup_menu(const QPoint &)));
	QObject::connect(&documents, SIGNAL(currentRowChanged(int)), this, SLOT(changed_row(int)));	
	load_data();
	
	return groupBox;
}

void win_doc_manager::setup_popupmenu()
{
	actions_addAct = new QAction(tr("Add"), this);
    connect(actions_addAct, SIGNAL(triggered()), this, SLOT(add()));
	actions_removeAct = new QAction(tr("Remove"), this);
    connect(actions_removeAct, SIGNAL(triggered()), this, SLOT(remove_doc()));
	actions_renameAct = new QAction(tr("Rename"), this);
    connect(actions_renameAct, SIGNAL(triggered()), this, SLOT(rename_doc()));
	actions_upAct = new QAction(tr("Move Up"), this);
    connect(actions_upAct, SIGNAL(triggered()), this, SLOT(up_doc()));
	actions_downAct = new QAction(tr("Move Down"), this);
    connect(actions_downAct, SIGNAL(triggered()), this, SLOT(down_doc()));

	popupmenu = new QMenu();
	popupmenu->addAction(actions_addAct);
	popupmenu->addSeparator();
	popupmenu->addAction(actions_removeAct);
	popupmenu->addSeparator();
	popupmenu->addAction(actions_renameAct);
	popupmenu->addSeparator();
	popupmenu->addAction(actions_upAct);
	popupmenu->addAction(actions_downAct);
}

QHBoxLayout	*win_doc_manager::setup_buttons()
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

void	win_doc_manager::add()
{
	win_add_document dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
	if (dialogo.exec())
		{
		load_data();	
		documents.setFocus();
		}
}

void	win_doc_manager::rename_doc()
{
	edit_row();
}

void	win_doc_manager::remove_doc()
{
	if (documents.count() == 1)
		{
		QMessageBox::warning(this, tr("Error"), tr("Can't erase all documents"));
		documents.setFocus();
		return;
		}
	
	QString question = tr("Do you want to definitively remove %1?").arg(documents.currentItem()->text());

	switch (QMessageBox::warning(this, "Itecad", question, tr("&Yes"), tr("&No"), 0))
	{
	case 0:		// 0 = YES
		w_main->documents_remove(documents.currentRow());
		break;
	case 1:		// 1 = NO
		break;
	}		

	load_data();	
	documents.setFocus();
}

void	win_doc_manager::changed_row(int row)
{
	//Seleziono il documento anche dentro w_main
	w_main->tw->setCurrentIndex(row);
	
	L_move_up->setEnabled(documents.currentRow() != 0);
	L_move_down->setEnabled(documents.currentRow() != documents.count() - 1);
}

void	win_doc_manager::edit_row()
{
	//Cambio il nome del documento anche dentro w_main
	QListWidgetItem *item = documents.currentItem();
	bool ok;
    QString text = QInputDialog::getText(this, "Itecad", tr("New name:"), QLineEdit::Normal, item->text(), &ok);
	if (ok && !text.isEmpty())
		{
		//Controllo che il nome inserito non sia già presente
		if (!w_main->documents_valid_name(text, documents.row(item)))
			{
			documents.setFocus();
			return;
			}
		item->setText(text);
		w_main->tw->setTabText(documents.row(item), text);	
		GV->name = text;
		documents.setFocus();
		}
}

void	win_doc_manager::down_doc()
{
	w_main->tw->setUpdatesEnabled(FALSE);
	int row = documents.currentRow();
	documents.setCurrentRow(row+1);
	up_doc();
	documents.setCurrentRow(row+1);
}

void	win_doc_manager::up_doc()
{
	int row = documents.currentRow();
	QListWidgetItem *item = documents.takeItem(row);
	documents.insertItem(row-1, item);
	documents.setCurrentRow(row-1);

	w_main->tw->setUpdatesEnabled(FALSE);
	QWidget *w = w_main->tw->widget(row);
	w_main->tw->removeTab(row);
	switch (w_main->doc_type(w))
	{
	case DT_SCH:
		w_main->tw->insertTab(row-1, w, QIcon(":/Resources/schematic.png"), documents.item(row-1)->text());
		break;
	case DT_PCB:
		w_main->tw->insertTab(row-1, w, QIcon(":/Resources/board.png"), documents.item(row-1)->text());
		break;
	case DT_TXT:
		w_main->tw->insertTab(row-1, w, QIcon(":/Resources/script.png"), documents.item(row-1)->text());
		break;
	}
	w_main->tw->setCurrentIndex(row-1);
	w_main->tw->setUpdatesEnabled(TRUE);
}

void	win_doc_manager::popup_menu(const QPoint &punto)
{
	actions_upAct->setEnabled(documents.currentRow() != 0);
	actions_downAct->setEnabled(documents.currentRow() != documents.count() - 1);

	popupmenu->popup(QCursor::pos());	
}
