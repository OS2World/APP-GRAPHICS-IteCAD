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

#ifndef WIN_MOVE_TO_H
	#define WIN_MOVE_TO_H

        #include <QVariant>
        #include <QAction>
        #include <QApplication>
        #include <QDialog>
        #include <QGroupBox>
        #include <QButtonGroup>
        #include <QGridLayout>
        #include <QHBoxLayout>
        #include <QVBoxLayout>
        #include <QTabWidget>
        #include <QTableWidget>
        #include <QSpinBox>
        #include <QLabel>
        #include <QCheckBox>
        #include <QLineEdit>
        #include <QPushButton>
        #include <QSpacerItem>
        #include <QWidget>
        #include <QRadioButton>
        #include <QDesktopWidget>
        #include <QFileDialog>
        #include "tipi_dato.h"
        #include "layout_length.h"
        #include "win_main.h"
	#include "drc.h"

	class win_move_to : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
		win_move_to(QWidget *parent, Qt::WFlags tipo, bool show_a, int ax, int ay);
		~win_move_to()
		{}

		bool			show_absolute;
		int				absolute_x;
		int				absolute_y;

		QCheckBox		*is_relative;
		QLabel			*p_x;
		QLabel			*p_y;
		layout_length	*pos_x;
		layout_length	*pos_y;
		
		QGroupBox		*setup_position();
		
		QHBoxLayout		*setup_buttons();

	public slots:
		void change_is_relative(int new_value);

	protected:
        void accept();
	};

#endif
