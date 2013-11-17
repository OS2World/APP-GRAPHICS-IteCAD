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

#ifndef WIN_EDIT_LAYERS_H
	#define WIN_EDIT_LAYERS_H

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
        #include "tipi_dato.h"
        #include "widget_layers.h"
        #include "win_main.h"
	
	class win_edit_layers : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
		win_edit_layers(QWidget *parent, Qt::WFlags tipo);
		~win_edit_layers()
		{}

		//QGroupBox	*setup_options();
		QHBoxLayout	*setup_buttons();
		
		QLabel		*num_copper_layers;
		QSpinBox	*Num_copper_layers;
		QLabel		*num_help_layers;
		QSpinBox	*Num_help_layers;

	public slots:
		void		changed_copper_layers(int inutile);
		void		changed_help_layers(int inutile);

	protected:
        void accept();
	};

#endif
