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

#ifndef WIN_DOC_MANAGER_H
	#define WIN_DOC_MANAGER_H

        #include <QGridLayout>
        #include <QHBoxLayout>
        #include <QVBoxLayout>
        #include <QLabel>
        #include <QPushButton>
        #include <QSpacerItem>
        #include <QGroupBox>
        #include <QInputDialog>
        #include <QListWidget>
        #include "tipi_dato.h"
        #include "win_main.h"
        #include "win_add_document.h"

	class win_doc_manager : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
		win_doc_manager(QWidget *parent, Qt::WFlags tipo);
		~win_doc_manager()
		{}

		QGroupBox	*setup_documents();
		void		setup_popupmenu();
		QHBoxLayout	*setup_buttons();

		QPushButton	*L_add;
		QPushButton	*L_move_up;
		QPushButton	*L_move_down;

		QAction *actions_addAct;
		QAction *actions_add_pcbAct;
		QAction *actions_removeAct;
		QAction *actions_renameAct;
		QAction *actions_upAct;
		QAction *actions_downAct;
		QMenu *popupmenu;

		QListWidget	documents;

		void	load_data();
		void	edit_row();		

	public slots:
		void	remove_doc();	
		void	rename_doc();	
		void	add();
		void	up_doc();
		void	down_doc();
		void	changed_row(int row);	
		void	popup_menu(const QPoint &punto);

	};

#endif
