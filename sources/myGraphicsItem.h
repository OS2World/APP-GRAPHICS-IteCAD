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

#ifndef MY_GRAPHICSITEM_H
#define MY_GRAPHICSITEM_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>
#include "utilities.h"
#include "math.h"

/////////////////////////////////////////////////////////
// La funzione beautiful_45_degrees() servirà ancora nelle future versioni di QT (spero migliorino la grafica)
/////////////////////////////////////////////////////////
void	beautiful_45_degrees(QPoint &pstart, QPoint &pend);
/////////////////////////////////////////////////////////

class myGraphicsItem //: public QGraphicsItem
{	
//	Q_OBJECT	//Necessario per gestire Signals e Slots
public:
    myGraphicsItem(char tipo_input);
	~myGraphicsItem()
	{}

        char            tipo;
        bool            is_selected;
        int             comp;
        int             number;         //Qui dentro vi è l'ID del pin o del pad (1,2,3,4 e così via...)
        int		net_id;
        int             connect_id;

        unsigned char   layer_from;     //Serve nei pad e nelle vie. Viene poi eseguito setZValue((qreal) layer) per ordinare correttamente i layer
        unsigned char   layer_to;       //Serve nei pad e nelle vie.

        node_data	*node_start;
	node_data	*node_end;

        QPoint          start;		//E' il punto attorno al quale ruota l'oggetto se non facente parte di un gruppo
        QPoint          end;

	QPolygon	polygon;	//Serve per le polilinee, i poligoni e le zone del PCB.

        bool            drag_or_move_active;
	
        char            font_height;
        bool            font_bold;
        bool            font_italic;
	
        qreal           pen_width;	//Conviene che sia di tipo qreal

        char            shape;      //Usato da Pad/Via/Pin/Port/Power
        int		width;      //Usato da Pad/Via/Rect/Ellipse
        int		height;     //Usato da Pad/Via/Rect/Ellipse
        int		drill;      //Usato da Pad/Via

        bool	fill_polygon;       //Serve anche per indicare show_name nei Power

	QRect	info_br;
	QPoint	info_start_text;
	QPoint	info_start_pin_number_text;
	void	info_update();
	
	QString text;
	char	text_position;

	QRect	boundingRect();
	void	paint(QPainter *painter, QMatrix *m);
        bool	hitTest(QPoint point);
	void	set_selected(bool sel);
	void	move(QPoint delta);

        void	segment_paint(QPainter *painter, QMatrix *m);
        bool	segment_hitTest(QPoint point);

	void	rotate(QPoint center, bool clockwise);
	void	mirror(QPoint center, bool horizontal);

        void    polygon_interpolation(QPolygon *pol);

        void    text_to_polygon(QString *t, QPoint text_start, QPolygon *tp, QPolygon *tl);
	QRect	text_rect(QString *t);
        int	text_length(QString *t);
	QRect	text_br(QRect r);
        void	text_draw(QPainter *painter, QMatrix *m, QString *t, QPoint text_start);	//E' una funzione inline per velocizzare
	QPoint	text_char_orientation(int x, int y);

	bool	highlight_active;

        bool	mirror_text_on;		//Se TRUE allora il testo viene specchiato rispetto al suo centro

        void    port_points_update(QPoint s);

        bool    layer_intersect(char l_from, char l_to);
        bool    is_showed();

protected:
     
};

#endif
