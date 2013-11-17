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

#ifndef WIN_DEVICES_H
        #define WIN_DEVICES_H

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
        #include <QTableWidget>
	#include "win_main.h"
	#include "tipi_dato.h"
	#include "undo_redo.h"
	#include "layout_length.h"

        class win_devices_edit : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
                win_devices_edit(QWidget *parent, Qt::WFlags tipo, int cr, QString device_text, QString footprint_text);
                ~win_devices_edit()
		{}

                QLineEdit		*device;
                QLineEdit		*footprint;

		QGridLayout		*setup_general();
		QHBoxLayout		*setup_buttons();

        public slots:
                void                    change_footprint();

	protected:
                void accept();
	};
#endif
