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

#ifndef WIN_GERBER_H
        #define WIN_GERBER_H

        #include <QVariant>
        #include <QAction>
        #include <QApplication>
        #include <QGroupBox>
        #include <QButtonGroup>
        #include <QGridLayout>
        #include <QHBoxLayout>
        #include <QVBoxLayout>
        #include <QLabel>
        #include <QSpinBox>
        #include <QPushButton>
        #include <QMessageBox>
        #include <QRadioButton>
        #include <QSpacerItem>
        #include <QWidget>
        #include <QDesktopWidget>
        #include <QTextStream>
        #include "tipi_dato.h"
        #include "myGraphicsItem.h"
        #include "myGraphicsView.h"
        #include "widget_layers.h"
        #include "win_main.h"

        class win_gerber : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
                win_gerber(QWidget *parent, Qt::WFlags tipo, bool is_board_and_drills_input);
                ~win_gerber()
		{}

                bool            is_board_and_drills;

                My_Layers	*tab_lay;
                QGroupBox	*setup_options();
		QHBoxLayout	*setup_buttons();

                QLineEdit	*Root_filename;
                QLineEdit	*Directory;

                QStringList     ad_code;
                QStringList     ad_definition;

                void            gerber_write(QTextStream &out, int layer);
                void            gerber_write_object(QTextStream &out, myGraphicsItem *obj);
                QString         gerber_number(int length, bool use_decimal_separator);
                QString         gerber_ad_definition(myGraphicsItem *obj);
                void            gerber_ad_insert(QString definition);
                QString         gerber_ad_find(QString definition);

        public slots:
                void		change_directory();

	protected:
        void accept();
	};

#endif
