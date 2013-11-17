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

#ifndef WIN_CONVERTER_H
	#define WIN_CONVERTER_H

        #include <QGridLayout>
        #include <QHBoxLayout>
        #include <QVBoxLayout>
        #include <QLabel>
        #include <QPushButton>
        #include <QCheckBox>
        #include <QSpacerItem>
        #include <QGroupBox>
        #include <QLineEdit>
        #include "tipi_dato.h"
        #include "win_main.h"

	class win_converter : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
		win_converter(QWidget *parent, Qt::WFlags tipo);
		~win_converter()
		{}

		QGroupBox	*setup_imperial_group();
		QGroupBox	*setup_metric_group();
		QHBoxLayout	*setup_buttons();
		
		QLineEdit	*T;
		QComboBox	*U;

		//Metric System
		QLabel		*cm1;
		QLabel		*cm2;
		QLabel		*dm1;
		QLabel		*dm2;
		QLabel		*metre1;
		QLabel		*metre2;
		QLabel		*micron1;
		QLabel		*micron2;
		QLabel		*mm1;
		QLabel		*mm2;

		//Imperial System
		QLabel		*feet1;
		QLabel		*feet2;
		QLabel		*inch1;
		QLabel		*inch2;
		QLabel		*thou1;
		QLabel		*thou2;
		QLabel		*yards1;
		QLabel		*yards2;

	public slots:	
		void			T_edited(const QString &stringa);
		void			U_change(int inutile);

	};

#endif
