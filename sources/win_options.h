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

#ifndef WIN_OPTIONS_H
	#define WIN_OPTIONS_H

        #include <QAction>
        #include <QApplication>
        #include <QDialog>
        #include <QGroupBox>
        #include <QButtonGroup>
        #include <QGridLayout>
        #include <QHBoxLayout>
        #include <QVBoxLayout>
        #include <QTabWidget>
        #include <QTabBar>
        #include <QSpinBox>
        #include <QLabel>
        #include <QCheckBox>
        #include <QLineEdit>
        #include <QPushButton>
        #include <QSpacerItem>
        #include <QWidget>
        #include <QRadioButton>
        #include <QDesktopWidget>
        #include <QSettings>
        #include <QFileDialog>
        #include "tipi_dato.h"
        #include "win_main.h"
        #include "layout_length.h"

	class win_options : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
		win_options(QWidget *parent, Qt::WFlags tipo);
		~win_options()
		{}

		QLineEdit		*dir_PRJ;

		QComboBox		*language_cb;

                QRadioButton            *y_axis_up;
                QRadioButton            *y_axis_down;

		QTabWidget		*tw;

		QWidget			*setup_general();
		QWidget			*setup_directories();
		QHBoxLayout		*setup_buttons();
		
	public slots:
		void		change_prj_directory();		
		void		change_ssf_directory();	
		void		change_pff_directory();	
                void		change_ivf_directory();

	protected:
        void accept();
	};

#endif
