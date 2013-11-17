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

#ifndef WIN_ADD_COMPONENT_H
	#define WIN_ADD_COMPONENT_H

        #include <QVariant>
        #include <QAction>
        #include <QApplication>
        #include <QGroupBox>
        #include <QButtonGroup>
        #include <QGridLayout>
        #include <QHBoxLayout>
        #include <QVBoxLayout>
        #include <QDirIterator>
        #include <QLabel>
        #include <QPushButton>
        #include <QSpacerItem>
        #include <QWidget>
        #include <QDesktopWidget>
        #include <QTreeWidget>
        #include <QLineEdit>
        #include <QCheckBox>
        #include "win_main.h"
	#include "tipi_dato.h"
        #include "xml_sax_handler_read_devices.h"
	
	class win_add_component : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
		win_add_component(QWidget *parent, Qt::WFlags tipo, bool footprint_change);
		~win_add_component()
		{}

		bool			fp_change;
		bool			is_sch;
		
		QGroupBox		*setup_part();
		QGroupBox		*setup_preview();
		QHBoxLayout		*setup_buttons();
		
                QTreeWidget		*components;
                QList<QTreeWidgetItem *>    all_row;

                QLineEdit		*dir;
                QDir			sch_directory;
                QDir			pcb_directory;
                QDir			*root_directory;
		
                QCheckBox		*filter;
		QLineEdit		*filter_text;

                QLabel			*preview_label;
                QLabel			*preview_label2;

                void                    show_up(QTreeWidgetItem *item);
                void                    show_down(QTreeWidgetItem *item);

                void			load_components(QTreeWidgetItem *father, QDir *dir);
                void			load_preview();

                QString                 filepath_read();
                void                    filepath_write(QString fp);
		
		QPushButton		*L_comandi_Add;

	public slots:
                void	apply_filter(int);
		void	apply_filter();
                void	component_changed(QTreeWidgetItem *current, QTreeWidgetItem *previous);

	protected:
        void	accept();
		void	reject();
	};

#endif
