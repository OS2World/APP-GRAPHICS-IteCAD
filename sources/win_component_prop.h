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

#ifndef WIN_COMPONENT_PROP_H
	#define WIN_COMPONENT_PROP_H

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
        #include "win_ungated_pins.h"
        #include "win_devices.h"

	class win_component_prop : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
		win_component_prop(QWidget *parent, Qt::WFlags tipo, myComponent *comp_input, bool alias_is_visible_input);
		~win_component_prop()
		{}

		myComponent	*comp;
		bool		alias_is_visible;

		QWidget		*setup_general();
		QWidget		*setup_ungated_pins();
		QHBoxLayout	*setup_buttons();

		QTabWidget	*tw;

		QComboBox	*alias;
                QLineEdit	*ident;
		QLineEdit	*value;
                QToolButton	*unicode_input_ident;
		QToolButton	*unicode_input_value;

		QCheckBox	*number_is_visible;
		QCheckBox	*name_is_visible;

		QLineEdit	*fp_name;	//E' il nome originale del footprint che è stato caricato
		
		QString		new_alias;
                QString		new_ident;
		QString		new_value;
		bool		new_pin_number_is_visible;
		bool		new_pin_name_is_visible;
		QString		new_up;
                QString		new_devices;
		QString		new_footprint_link;

		bool		is_modified();

		QTableWidget	*up_table;
                void		up_table_load();

                QTableWidget	*devices_table;
                void		devices_table_load();

                void            schcomp_or_pcbcomp_update_with_new_data();

	public slots:
		void		alias_index_changed(int index);
		void		browse();

                void		unicode_input_ident_pressed();
		void		unicode_input_value_pressed();

		void		ungated_pins_add();
		void		ungated_pins_remove();
		void		ungated_pins_edit();

                void		devices_add();
                void		devices_remove();
                void		devices_edit();

	protected:
        void accept();
	};
#endif
