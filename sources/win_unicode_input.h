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

#ifndef WIN_UNICODE_INPUT_H
	#define WIN_UNICODE_INPUT_H

        #include <QGridLayout>
        #include <QHBoxLayout>
        #include <QVBoxLayout>
        #include <QLabel>
        #include <QPushButton>
        #include <QCheckBox>
        #include <QSpacerItem>
        #include <QGroupBox>
        #include <QLineEdit>
        #include <QToolTip>
        #include "tipi_dato.h"
        #include "win_main.h"

	class CharacterWidget : public QWidget
		{
		Q_OBJECT

		public:
			CharacterWidget(QWidget *parent = 0);			

			QFont	displayFont;
			int		columns;
			int		squareSize;
			char	group;
			int		group_value(int position);
			int		group_count;
			
			QSize	sizeHint() const;
			int		position(int x, int y);
			

			QLineEdit	*LE;

		signals:
			void	characterSelected(const QString &character);

		protected:
			void	mouseMoveEvent(QMouseEvent *event);
			void	mouseDoubleClickEvent(QMouseEvent *event);
			void	paintEvent(QPaintEvent *event);
		};

	
	class win_unicode_input : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
                win_unicode_input(QWidget *parent, Qt::WFlags tipo, QPoint winpos);
                ~win_unicode_input();

		QGroupBox	*setup_group();
		QGroupBox	*setup_list();
		QHBoxLayout	*setup_buttons();

		QRadioButton	*electronic;
		QRadioButton	*accented;

                CharacterWidget	*cw;

                QPalette        palette;

	public slots:	
		void	set_electronic(bool inutile);
		void	set_accented(bool inutile);
                void    set_lineedit(QLineEdit *le);
        };

#endif
