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

#ifndef WIN_FIND_COMP_H
        #define WIN_FIND_COMP_H

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
        #include "tipi_dato.h"
        #include "myGraphicsItem.h"
        #include "myComponent.h"
        #include "myGraphicsView.h"
        #include "win_main.h"

        class win_find_comp : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
                win_find_comp(QWidget *parent, Qt::WFlags tipo);
                ~win_find_comp()
		{}

                QGroupBox	*setup_options();
		QHBoxLayout	*setup_buttons();

                QComboBox	*sch_comp;
                QComboBox	*pcb_comp;

                int             last_number(QString text, int last_number_position);
                int             last_number_position(QString text);
                void            insert_alphabetically(QComboBox * cb, QString text);

        public slots:
                void		find_sch_comp();
                void		find_pcb_comp();

	protected:
                void            reject();
	};

#endif
