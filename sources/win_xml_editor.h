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

#ifndef WIN_XML_EDITOR_H
	#define WIN_XML_EDITOR_H

        #include <QGridLayout>
        #include <QHBoxLayout>
        #include <QVBoxLayout>
        #include <QLabel>
        #include <QPushButton>
        #include <QSpacerItem>
        #include <QGroupBox>
        #include <QTextEdit>
        #include <QSyntaxHighlighter>
        #include <QMenuBar>
        #include "tipi_dato.h"
        #include "win_main.h"
        #include "win_find_text.h"

	class Highlighter : public QSyntaxHighlighter
	 {
		 Q_OBJECT

	 public:
		 Highlighter(QTextDocument *parent = 0);

	 protected:
		 void highlightBlock(const QString &text);

	 private:
		 struct HighlightingRule
		 {
			 QRegExp pattern;
			 QTextCharFormat format;
		 };
		 QVector<HighlightingRule> highlightingRules;

		 QTextCharFormat keywordFormat;
		 QTextCharFormat quotationFormat;
	 };

	
	class win_xml_editor : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
		win_xml_editor(QWidget *parent, Qt::WFlags tipo);
                ~win_xml_editor();

		QMenuBar	*setup_menuBar();
		QStatusBar	*setup_statusBar();
		bool		ask_if_save();
				
		QAction		*actions_open_prjAct;
		QAction		*actions_open_ssfAct;
		QAction		*actions_open_pffAct;
		QAction		*actions_saveAct;
		QAction		*actions_save_asAct;
		QAction		*actions_exitAct;
		
		QTextEdit	 xml;
		Highlighter	*highlighter;

		QString		file_path;
		char		file_type_choosed;

		QMenuBar	*mb;
		QMenu		*fileMenu;
		QMenu		*editMenu;
		QAction		*file_open_prjAct;
		QAction		*file_open_ssfAct;
		QAction		*file_open_pffAct;
		QAction		*file_saveAct;	
                QAction		*file_save_asAct;
		QAction		*file_exitAct;
		QAction		*edit_undoAct;
		QAction		*edit_redoAct;
		QAction		*edit_cutAct;
		QAction		*edit_copyAct;
		QAction		*edit_pasteAct;
                QAction		*edit_findAct;

		QLabel		*sb_row;
		QLabel		*sb_column;

	public slots:
		void	file_open_prj();	
		void	file_open_ssf();	
		void	file_open_pff();	
		void	file_save();
		void	file_save_as();
                void	edit_find();
                void	menu_edit_open();

		void	changed_cursor_pos();

	protected:
		virtual void	closeEvent(QCloseEvent *e);
		void			reject();
	};

#endif
