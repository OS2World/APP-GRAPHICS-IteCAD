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

#ifndef WIN_PAGE_H
        #define WIN_PAGE_H

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

        class win_page : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
                win_page(QWidget *parent, Qt::WFlags tipo);
                ~win_page()
                {};

                QGroupBox	*setup_labelling();
                QHBoxLayout	*setup_buttons();

                layout_length	*size;

                layout_size	*page;

                QLineEdit	*organization;
                QToolButton	*unicode_input_organization;
                QLineEdit	*title;
                QToolButton	*unicode_input_title;
                QLineEdit	*doc_num;
                QToolButton	*unicode_input_doc_num;
                QLineEdit	*rev;
                QToolButton	*unicode_input_rev;

                bool		is_modified();

        public slots:
                void		unicode_input_organization_pressed();
                void		unicode_input_title_pressed();
                void		unicode_input_doc_num_pressed();
                void		unicode_input_rev_pressed();

	protected:
                void accept();
	};

#endif
