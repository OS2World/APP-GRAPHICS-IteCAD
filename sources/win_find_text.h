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

#ifndef WIN_FIND_TEXT_H
        #define WIN_FIND_TEXT_H

        #include <QVariant>
        #include <QApplication>
        #include <QGroupBox>
        #include <QButtonGroup>
        #include <QGridLayout>
        #include <QHBoxLayout>
        #include <QVBoxLayout>
        #include <QLabel>
        #include <QPushButton>
        #include <QMessageBox>
        #include <QSpacerItem>
        #include <QWidget>
        #include <QTextEdit>
        #include "tipi_dato.h"
        #include "myGraphicsItem.h"
        #include "myComponent.h"
        #include "myGraphicsView.h"
        #include "win_main.h"

        class win_find_text : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
                win_find_text(QWidget *parent, Qt::WFlags tipo, QTextEdit *te_input);
                ~win_find_text()
		{}

                QTextEdit       *te;

                QGridLayout	*setup_options();
                QHBoxLayout	*setup_buttons();

                QLineEdit	*text;
                QCheckBox	*case_sensitive;
                QRadioButton    *up;
                QRadioButton    *down;

        public slots:
                void		find();

	protected:
                void            reject();
	};

#endif
