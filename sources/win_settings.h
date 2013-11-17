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

#ifndef WIN_SETTINGS_H
        #define WIN_SETTINGS_H

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
        #include <QRadioButton>
        #include <QDesktopWidget>
        #include <QFileDialog>
        #include <QFontComboBox>
        #include "tipi_dato.h"
        #include "layout_length.h"
        #include "layout_size.h"
        #include "win_main.h"
	#include "layout_font.h"
        #include "layout_pad_via.h"

        class win_settings : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
                win_settings(QWidget *parent, Qt::WFlags tipo, char tab_index);
                ~win_settings();

                QTabWidget	*tw;

                QTableWidget	*table_sch_workspace;
                QTableWidget	*table_sch_layers;
                QTableWidget	*table_pcb_workspace;
                QTableWidget	*table_pcb_layers;

                QWidget		*setup_sch_drawing_style();
                QWidget         *setup_sch_artistic_grid();
                QWidget         *setup_sch_colours();
                QWidget         *setup_sch_workspace();
                QWidget		*setup_pcb_drawing_style();
                QWidget		*setup_pcb_grid();
                QWidget         *setup_pcb_colours();
                QWidget         *setup_pcb_workspace();
                QHBoxLayout	*setup_buttons();

                QGroupBox	*setup_workspace(bool is_sch);
                QGroupBox	*setup_layers(bool is_sch);

                QTimer          *timer_selection;

                QCheckBox	*visible;
                QComboBox	*grid_size;
                QComboBox	*grid_UM;
                QComboBox	*grid_artistic;

                layout_pad_via  *pad;
                layout_pad_via  *via;

                QRadioButton	*snap_none;
		QRadioButton	*snap_grid;
                QRadioButton	*snap_nodes_and_grid;

                layout_length	*Pen_width_normal;
                layout_length	*Pen_width_wire;
		layout_length	*Pen_width_frame;                
		layout_length	*Pen_width_copper;
		layout_length	*Pen_width_silk;
		layout_length	*Pen_width_paste;
		layout_length	*Pen_width_mask;
		layout_length	*Pen_width_board;
		layout_length	*Pen_width_help;

                QCheckBox	*sch_Fill_polygon;
                QCheckBox	*pcb_Fill_polygon;

                layout_font	*sch_font_layout;
                layout_font	*pcb_font_layout;

                QRadioButton    *draw_mode_gerber;
                QRadioButton    *draw_mode_draft;

                char            ti;

                void            load_data();

        public slots:
                void		load_factory_settings();
                void		load_general_default_settings();
                void		change_color(QTableWidgetItem *item);
                void		timeout();
                void            grid_UM_changed(int inutile);

	protected:
                void accept();
	};

#endif
