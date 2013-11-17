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

#ifndef WIN_PRINT_H
	#define WIN_PRINT_H

        #include <QPrinter>
        #include <QPrintDialog>
        #include <QPageSetupDialog>
        #include <QGridLayout>
        #include <QHBoxLayout>
        #include <QVBoxLayout>
        #include <QCheckBox>
        #include <QLabel>
        #include <QPushButton>
        #include <QGroupBox>
        #include "win_main.h"
        #include "tipi_dato.h"
        #include "layout_length.h"
        #include "widget_layers.h"
        #include "win_pdf_print_dialog.h"

	class win_print : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
		win_print(QWidget *parent, Qt::WFlags tipo, bool pdf);
		~win_print()
		{}

		bool		is_pdf;

		QPrinter	*printer;
		
		My_Layers	*layers;		
		QLabel		*printer_name;
		QLabel		*printer_size_and_orientation_from;
		QLabel		*printer_scale;
		QLabel		*printer_size_and_orientation_to;
                QPushButton     *printer_setup;

		qreal		painter_scale;
		
		QRadioButton	*scale_1_1;
		QRadioButton	*scale_2_1;
		QRadioButton	*scale_1_2;
		
		layout_length	*offset_x;
		layout_length	*offset_y;

                char            colormode;

                QRadioButton	*colormode_color;
                QRadioButton	*colormode_grayscale;
                QRadioButton	*colormode_black;

		QVBoxLayout	*setup_printer();
		QHBoxLayout	*setup_buttons();
		QString		read_printer_settings();
		void		read_scale_settings();

	public slots:
                void	printer_dialog();
		void	change_pcb_scale(bool inutile);
                void	change_colormode(bool inutile);

	protected:
        void accept();
	};

#endif
