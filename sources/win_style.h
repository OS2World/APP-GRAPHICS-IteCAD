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

#ifndef WIN_STYLE_H
	#define WIN_STYLE_H

        #include <QVariant>
        #include <QFont>
        #include <QFontComboBox>
        #include <QDialog>
        #include <QGridLayout>
        #include <QHBoxLayout>
        #include <QVBoxLayout>
        #include <QLabel>
        #include <QPushButton>
        #include <QCheckBox>
        #include <QSpacerItem>
        #include <QGroupBox>
        #include <QLineEdit>
        #include <QRadioButton>
        #include "win_main.h"
        #include "tipi_dato.h"
        #include "undo_redo.h"
	#include "layout_length.h"
	#include "layout_font.h"

	class win_style : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
		win_style(QWidget *parent, Qt::WFlags tipo);
		~win_style()
		{}

                QFontComboBox           *font;
		QComboBox		*size;
		QToolButton		*bold;
		QToolButton		*italic;	
		QToolButton		*underline;	
		QToolButton		*left;
		QToolButton		*center;
		QToolButton		*right;
		QToolButton		*justify;

		void			mergeFormatOnWordOrSelection(const QTextCharFormat &format);
		
	public slots:
		void	change_family(const QString &f);
		void	change_size(const QString &p);
		void	change_bold();
		void	change_italic();
		void	change_underline();	
		void	currentCharFormatChanged(const QTextCharFormat &format);

		void	change_left();
		void	change_center();
		void	change_right();
		void	change_justify();
		void	cursorPositionChanged();
	};

#endif
