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

#ifndef WIN_CHAR_EDITOR_H
	#define WIN_CHAR_EDITOR_H

        #include <QGridLayout>
        #include <QHBoxLayout>
        #include <QVBoxLayout>
        #include <QLabel>
        #include <QPushButton>
        #include <QCheckBox>
        #include <QSpacerItem>
        #include <QGroupBox>
        #include <QPixmap>
	#include "char_editor.h"
        #include "tipi_dato.h"
        #include "win_main.h"

	class win_char_editor : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
		win_char_editor(QWidget *parent, Qt::WFlags tipo);
		~win_char_editor()
		{}

		QMenuBar	*setup_menuBar();
		QGridLayout	*setup_editor();
		
		char_editor	*c;
		QComboBox	*weight;
		
		QLabel			*mode;
		QRadioButton	*tool_draw;
		QRadioButton	*tool_erase;

		QLabel		*w;
		QComboBox	*char_width;
		
		QCheckBox	*background;
		QComboBox	*bg_list;

		QMenuBar	*mb;
		QMenu		*fileMenu;
		QMenu		*editMenu;
		QAction		*file_new_Act;
		QAction		*file_open_Act;
		QAction		*file_saveAct;	
                QAction		*file_save_asAct;
		QAction		*file_exitAct;
		QAction		*edit_undoAct;
		QAction		*edit_redoAct;

		bool		ask_if_save();
		bool		never_saved;

		QAction		*actions_escapeAct;	

		QString		file;
		void		file_s(QString filename);
		void		bg_list_fill();

	public slots:	
		void		set_tool_erase(bool flag);
		void		set_tool_draw(bool flag);
		void		file_new();
		void		file_open();
		void		file_save();
		void		file_save_as();
		void		edit_undo();
		void		edit_redo();
		void		menu_file_open();
		void		edit_can_undo_changed(bool new_value);
		void		edit_can_redo_changed(bool new_value);
		void		background_on_off(int state);
		void		background_changed(int index);
		void		actions_escape();
		void		char_width_changed(int index);

	protected:
		void			reject();
		virtual void	closeEvent(QCloseEvent *e);

	};

#endif
