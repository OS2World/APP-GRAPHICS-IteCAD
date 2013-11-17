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

#ifndef MY_GRAPHICSSCENE_H
#define MY_GRAPHICSSCENE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QStyleOptionGraphicsItem>
#include "myGraphicsItem.h"

class myGraphicsScene : public QGraphicsScene
{	
	Q_OBJECT	//Necessario per gestire Signals e Slots
public:
    myGraphicsScene();
	~myGraphicsScene()
	{clear();}

        void	clear();
	void	add_item(myGraphicsItem *obj);
	void	remove_item(myGraphicsItem *obj);

	QList<myGraphicsItem *> all_obj;

        int     layer_index_start[LAYER_DATA_COUNT];
        int     layer_index_end[LAYER_DATA_COUNT];
        void    layer_index_update();

        void	draw_layer(QPainter *painter, QMatrix *m, QRect visible_rect, bool for_screen, unsigned char layer);

        void	draw_for_screen(QPainter *painter, QMatrix *m);
        void	draw_for_printer(QPainter *painter, QMatrix *m);

public slots:
	
protected:

};

#endif
