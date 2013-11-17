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

#ifndef WIDGET_LAYERS_H
#define WIDGET_LAYERS_H

#include <QTableView>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QPainter>
#include "tipi_dato.h"
#include "utilities.h"
#include "myGraphicsView.h"

class My_Layers : public QTableWidget
{	
	Q_OBJECT	//Necessario per gestire Signals e Slots	
public:
    My_Layers();	
	~My_Layers()
	{}

	char	tipo_tabella;	//V=Video P=Printer M=Make

        void	load_data();
	
        void	toogle_select(int tab_row);
	void	toogle_show(int tab_row);
	void	toogle_print(int tab_row);
        void	toogle_gerber(int tab_row);

	QSize	right_size();

        int	row_conversion[LAYER_DATA_COUNT]; //E' sovradimensionata, ma va bene lo stesso
		
protected:
	void	resizeEvent(QResizeEvent *e);
	void	mousePressEvent(QMouseEvent *e);
};

#endif
