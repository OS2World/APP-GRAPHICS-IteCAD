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

#ifndef WIN_REPORT_H
	#define WIN_REPORT_H

        #include <QGridLayout>
        #include <QHBoxLayout>
        #include <QVBoxLayout>
        #include <QLabel>
        #include <QPushButton>
        #include <QSpacerItem>
        #include <QGroupBox>
        #include <QInputDialog>
        #include "tipi_dato.h"
        #include "win_main.h"
        #include "win_add_document.h"

	class win_report : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
		win_report(QWidget *parent, Qt::WFlags tipo);
		~win_report()
		{}

		QHBoxLayout	*setup_buttons();

		QListWidget	info;

                QLabel		info_pads_vias_nets;
                QLabel		info_ratsnests;

                int		info_pads;
                int             info_vias;
                int             info_nets;
                int             info_wired_ratsnests;
                int             info_active_ratsnests;

		int		num_error;
		int		num_warning;

                void            insert_row(QString text, char mode);
                void            finalize();
		
	public slots:

	};

#endif
