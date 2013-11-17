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

#ifndef WIN_HELP_ABOUT_H
	#define WIN_HELP_ABOUT_H

        #include <QAction>
        #include <QApplication>
        #include <QButtonGroup>
        #include <QDialog>
        #include <QGridLayout>
        #include <QHBoxLayout>
        #include <QPushButton>
        #include <QSpacerItem>
        #include <QVBoxLayout>
        #include <QWidget>
        #include <QBasicTimer>
        #include <QLabel>

	class win_help_about : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots
	public:
                win_help_about(QWidget *parent, Qt::WFlags tipo);

                void            	setup();
                QHBoxLayout     	*setup_button();

                QLabel          *label_scroll;
                bool            first_scroll;

                QColor	colore_sfondo;
                QColor	colore_sfondo_attenuato;
                int		frame;
                QFont		font_release;
                QFont		font_title;
                QFont		font_body;

                QString         release;

                QPixmap         *pixmap_itecad;
                QPixmap         *pixmap_logo;
                QPixmap         *pixmap_iteco;
                QPixmap         *pixmap_scroll;

        protected:
                void			timerEvent(QTimerEvent *event);

        private:
                QBasicTimer		timer_frame;
		
	};

#endif
