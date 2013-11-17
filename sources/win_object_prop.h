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

#ifndef WIN_OBJECT_PROP_H
	#define WIN_OBJECT_PROP_H

        #include <QVariant>
        #include <QFont>
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
        #include <QRadioButton>
	#include "win_main.h"
	#include "tipi_dato.h"
	#include "undo_redo.h"
	#include "layout_length.h"
	#include "layout_font.h"
        #include "layout_pad_via.h"
	#include "win_unicode_input.h"

	class win_object_prop : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
		win_object_prop(QWidget *parent, Qt::WFlags tipo, myGraphicsItem *obj_input);
		~win_object_prop()
		{}

		myGraphicsItem	*obj;
		
		QGroupBox	*setup_connection();
		QGroupBox	*setup_font_properties();
		QGroupBox	*setup_text();
		QGroupBox	*setup_polyline();
		QGroupBox	*setup_polygon();
		QGroupBox	*setup_pin_general();
		QGroupBox	*setup_pin_orientation();
		QGroupBox	*setup_port_general();
		QGroupBox	*setup_port_orientation();
                QGroupBox	*setup_power_general();
                QGridLayout	*setup_pad();
                QGridLayout	*setup_via();
                QHBoxLayout	*setup_buttons();
		
                layout_font	*font_layout;
		
		layout_length	*Pen_width;
                QCheckBox	*Fill_polygon;

                QLineEdit	*Value;
		QRadioButton	*orientation_top;
		QRadioButton	*orientation_bottom;
		QRadioButton	*orientation_left;
		QRadioButton	*orientation_right;
                QLabel		*orientation_image;
                QComboBox	*Pin_shape;
                QComboBox	*Port_shape;

                QComboBox	*Power_shape;
                QCheckBox	*Show_name;
		
                layout_pad_via  *pad;
                layout_pad_via  *via;

                QLineEdit	*TextText;
                QLineEdit	*TextNumber;
                QLineEdit	*TextName;
                QCheckBox	*TextMirror;

                QToolButton	*unicode_input;
		
                qreal		new_pen_width;
                bool		new_fill_polygon;
                QString		new_text;
                char		new_text_position;
                bool            new_mirror_text_on;
                char		new_font_height;
                bool		new_font_bold;
                bool		new_font_italic;
                char		new_shape;
                int		new_width;
                int		new_height;
                unsigned char   new_layer_from;
                unsigned char   new_layer_to;
                int             new_drill;
                int		new_number;

                bool		is_modified();
		
	protected:
        void accept();

	public slots:
		void	change_orientation(bool checked);
                void	change_fill_polygon(int state);
                void	unicode_input_pressed();
	};

#endif
