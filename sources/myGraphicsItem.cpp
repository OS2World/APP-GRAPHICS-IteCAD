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

#include "myGraphicsItem.h"
#include "myGraphicsView.h"

extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;
extern	win_main		*w_main;
extern	char			file_type;	
extern	lay_data		layer_data[LAYER_DATA_COUNT];
extern	lay_data		workspace_data[WORKSPACE_DATA_COUNT];
extern	QPen			pen_layer;
extern	QBrush			brush_layer;

extern	QFont	font_pin;
extern	char	font_pin_height;

extern	QPoint	drag_or_move_delta;

extern  bool    pcb_pad_via_filled;

extern  QPolygon    text_points;
extern  QPolygon    text_lines;

extern  QPoint port_l, port_r, port_tl, port_tr, port_bl, port_br;

inline	void	beautiful_45_degrees(QPoint &pstart, QPoint &pend)
{
	//Conviene avere questa funzione di tipo inline
	int pstartx = pstart.x();
	int pstarty = pstart.y();
	int pendx = pend.x();
	int pendy = pend.y();
		if (pstartx != pendx)
			if (pstarty != pendy)
				{
				int deltax = abs(pstartx - pendx);
				int deltay = abs(pstarty - pendy);
				if ((deltax - deltay) == 1)
					{
					//Diminuisco deltax (conviene sempre disegnare di meno...)
					if (pstartx > pendx)
						pendx++;
					else
						pendx--;
					pend.setX(pendx);
					}
				else if ((deltay - deltax) == 1)
					{
					//Diminuisco deltay (conviene sempre disegnare di meno...)
					if (pstarty > pendy)
						pendy++;
					else
						pendy--;
					pend.setY(pendy);
					}
				}
}

myGraphicsItem::myGraphicsItem(char tipo_input)
        //: QGraphicsItem()
{
	tipo = tipo_input;
        is_selected = FALSE;
	drag_or_move_active = FALSE;
	comp = 0;
	node_start = 0;
	node_end = 0;
	mirror_text_on = FALSE;
        shape = PIN_SHAPE_NONE;
        fill_polygon = FALSE;
	
//	setCursor(Qt::IBeamCursor);
//	setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable);
//	setFlags(QGraphicsItem::ItemIsSelectable);
}

QRect myGraphicsItem::boundingRect()
{
	if (drag_or_move_active)
		return info_br.translated(drag_or_move_delta);
	else
		return info_br;
}

bool myGraphicsItem::hitTest(QPoint point)
{
	static QImage image = QImage(7, 7, QImage::Format_RGB32);	
		
        if (!is_showed())
                return FALSE;

        QPainter *painter = new QPainter(&image);
	QColor	back_color = QColor(0, 0, 0);
	painter->fillRect(0,0,7,7,back_color);
	
	QMatrix m = GV->matrix();
	m.translate(-point.x(), -point.y());
	qreal pixel4 = 4.0 / GV->matrix().m11();
	m.translate(pixel4, pixel4);	//Traslo di 4 pixel per essere al centro dell'immagine di 7x7
	painter->setMatrix(m);
	painter->setMatrixEnabled(FALSE);	//Lavoro in scala 1 a 1 senza nessuna matrice di trasformazione
	pen_layer.setColor(QColor(255,255,255));
	painter->setPen(pen_layer);
	brush_layer.setColor(QColor(255,255,255));
	painter->setBrush(brush_layer);
	if (file_type == FT_PROJECT)
		{
                switch (tipo)
		{
		case O_POLYGON:
		case O_RECT:
		case O_ELLIPSE:
                        {
                        bool	old_fill_polygon = fill_polygon;
                        if (comp)
                                fill_polygon = TRUE;
                        paint(painter, &m);
                        fill_polygon = old_fill_polygon;
                        }
                        break;
                case O_PORT:
                        fill_polygon = TRUE;
                        paint(painter, &m);
                        fill_polygon = FALSE;
                        break;
                case O_PAD:
                case O_VIA:
                        {
                        bool	old_pcb_pad_via_filled = pcb_pad_via_filled;
                        pcb_pad_via_filled = TRUE;
                        paint(painter, &m);
                        pcb_pad_via_filled = old_pcb_pad_via_filled;
                        }
                        break;
                default:
                        paint(painter, &m);
                        break;
		}
                }
	else
		paint(painter, &m);
	painter->setMatrixEnabled(TRUE);

	delete painter;
	
	for (int x = 0; x < 7; x++)
		for (int y = 0; y < 7; y++)
			{
			QColor c = image.pixel(x,y);
			if (c.red())
				return TRUE;
			}
	return FALSE;
}

void	myGraphicsItem::set_selected(bool sel)
{
	if (is_selected == sel)
		return;		//Se il valore è lo stesso di prima allora esco senza fare niente
	
	is_selected = sel;	
	
	if (!comp)
		{
		//Se non appartengo a un gruppo aggiungo o rimuovo questo oggetto alla lista degli oggetti selezionati
		if (is_selected)
			{
			if (!GV->selected_obj.contains(this))
				GV->selected_obj.append(this);
			}
		else
			GV->selected_obj.removeAll(this);
		}
}

void	myGraphicsItem::move(QPoint delta)
{
	start += delta;
	end += delta;
	for (int i = 0; i < polygon.count(); i++)
		polygon[i] += delta;
}

void	myGraphicsItem::rotate(QPoint center, bool clockwise)
{
	switch (tipo)
	{
	case O_POLYLINE:
	case O_POLYGON:
		for (int i = 0; i < polygon.count();i++)
			polygon[i] = rotate_point(polygon[i], center, clockwise);
		break;
	case O_RECT:
	case O_ELLIPSE:
		start = rotate_point(start, center, clockwise);
		if (clockwise)
                        start.setX(start.x() - height);
		else
                        start.setY(start.y() - width);
                swap_int(width, height);
		break;
	case O_TEXT:
	case O_IDENT:
	case O_VALUE:
		start = rotate_point(start, center, clockwise);
		if (clockwise)
			switch (text_position)
			{
			case 'L':
				if (GV->is_sch)
                                        start.setX(start.x() + 12 *font_height * 60);	//Il carattere 'A' ed i numeri sono tutti alti 13 punti in char_editor per cui moltiplico il passo per 12
				text_position = 'T';
				break;
			case 'T':
				text_position = 'R';
				break;
			case 'R':
				if (GV->is_sch)
                                        start.setX(start.x() + 12 *font_height * 60);	//Il carattere 'A' ed i numeri sono tutti alti 13 punti in char_editor per cui moltiplico il passo per 12
				text_position = 'B';
				break;
			case 'B':
				text_position = 'L';
				break;
			}
		else
			switch (text_position)
			{
			case 'L':
				text_position = 'B';
				break;
			case 'B':
				if (GV->is_sch)
                                        start.setY(start.y() + 12 *font_height * 60);	//Il carattere 'A' ed i numeri sono tutti alti 13 per cui moltiplico il passo per 12
				text_position = 'R';
				break;
			case 'R':
				text_position = 'T';
				break;
			case 'T':
				if (GV->is_sch)
                                        start.setY(start.y() + 12 *font_height * 60);	//Il carattere 'A' ed i numeri sono tutti alti 13 per cui moltiplico il passo per 12
				text_position = 'L';
				break;
			}
		break;
        case O_PAD:
                swap_int(width, height);
                break;
	case O_PIN:
	case O_PORT:
        case O_POWER:
		if (clockwise)
			switch (text_position)
			{
			case 'L':
				text_position = 'T';
				break;
			case 'T':
				text_position = 'R';
				break;
			case 'R':
				text_position = 'B';
				break;
			case 'B':
				text_position = 'L';
				break;
			}
		else
			switch (text_position)
			{
			case 'L':
				text_position = 'B';
				break;
			case 'B':
				text_position = 'R';
				break;
			case 'R':
				text_position = 'T';
				break;
			case 'T':
				text_position = 'L';
				break;
			}
		break;

	}
}

void	myGraphicsItem::mirror(QPoint center, bool horizontal)
{
	switch (tipo)
	{
	case O_POLYLINE:
	case O_POLYGON:
		for (int i = 0; i < polygon.count();i++)
			polygon[i] = mirror_point(polygon[i], center, horizontal);
		break;
	case O_RECT:
	case O_ELLIPSE:
		start = mirror_point(start, center, horizontal);
		if (horizontal)
                        start.setX(start.x() - width);
		else
                        start.setY(start.y() - height);
		break;
	case O_TEXT:
	case O_IDENT:
	case O_VALUE:
		start = mirror_point(start, center, horizontal);
		if (GV->is_sch)
			switch (text_position)
			{
			case 'L':
				if (horizontal)
					text_position = 'R';
				else if (GV->is_sch)
                                        start.setY(start.y() + 12 *font_height * 60);	//Il carattere 'A' ed i numeri sono tutti alti 13 per cui moltiplico il passo per 12
				break;
			case 'T':
				if (!horizontal)
					text_position = 'B';
				else if (GV->is_sch)
                                        start.setX(start.x() + 12 *font_height * 60);	//Il carattere 'A' ed i numeri sono tutti alti 13 per cui moltiplico il passo per 12
				break;
			case 'R':
				if (horizontal)
					text_position = 'L';
				else if (GV->is_sch)
                                        start.setY(start.y() + 12 *font_height * 60);	//Il carattere 'A' ed i numeri sono tutti alti 13 per cui moltiplico il passo per 12
				break;
			case 'B':
				if (!horizontal)
					text_position = 'T';
				else if (GV->is_sch)
                                        start.setX(start.x() + 12 *font_height * 60);	//Il carattere 'A' ed i numeri sono tutti alti 13 per cui moltiplico il passo per 12
				break;
			}
		else
			mirror_text_on = !mirror_text_on;
		break;
	case O_PIN:
        case O_POWER:
	case O_PORT:
		switch (text_position)
		{
		case 'L':
			if (horizontal)
				text_position = 'R';
			break;
		case 'T':
			if (!horizontal)
				text_position = 'B';
			break;
		case 'R':
			if (horizontal)
				text_position = 'L';
			break;
		case 'B':
			if (!horizontal)
				text_position = 'T';
			break;
		}
		break;
	}
}

QRect	myGraphicsItem::text_rect(QString *t)
{
        text_to_polygon(t, QPoint(0, 0), &text_points, &text_lines);

        int xleft = 10000;  //Metto un valore molto alto
        int xright = 0;
        int ytop = 0;
        int ybottom = 0;

        int x, y;

        int size = text_points.count();
        for(int i = 0; i < size; i++)
                {
                x = text_points.at(i).x();
                y = text_points.at(i).y();
                if (xleft > x)
                        xleft = x;
                if (xright < x)
                        xright = x;
                if (ybottom > y)
                        ybottom = y;
                if (ytop < y)
                        ytop = y;
                }

        size = text_lines.count();
        for(int i = 0; i < size; i++)
                {
                x = text_lines.at(i).x();
                y = text_lines.at(i).y();
                if (xleft > x)
                        xleft = x;
                if (xright < x)
                        xright = x;
                if (ybottom > y)
                        ybottom = y;
                if (ytop < y)
                        ytop = y;
                }

        return QRect(QPoint(xleft, ybottom), QPoint(xright, ytop));
}

int		myGraphicsItem::text_length(QString *t)
{
	//Questa funzione è diversa da text_rect perchè tiene conto degli eventuali spazi vuoti che distanziano un carattere dall'altro
	//Senza di essa il pin number dei componenti sarebbe male allineato.	
	int w = 0;
        qreal	passo = font_height * 60;
	for(int i = 0; i < t->count(); i++)
		{
		char_symbol *cs;
		cs = w_main->alphabet.value((*t)[i].unicode(),0);
		if (cs == 0)
			cs = w_main->alphabet.value(63,0);	//Per i caratteri non definiti uso il carattere '?'	
		w += cs->width;
		}
	return w * passo;
}

QRect	myGraphicsItem::text_br(QRect r)
{
	//Aggiungo lo spessore delle linee
        qreal	passo = font_height * 60;
        int pen_width_div_2;
	if (font_bold)
                pen_width_div_2 = 1.3 * passo;
        else
                pen_width_div_2 = 0.9 * passo;
	return r.adjusted(-pen_width_div_2, -pen_width_div_2, pen_width_div_2, pen_width_div_2);
}

void    myGraphicsItem::text_to_polygon(QString *t, QPoint text_start, QPolygon *tp, QPolygon *tl)
{
        tp->clear();
        tl->clear();

        qreal	passo = font_height * 60;
        QPoint	char_start = text_start;

        int pos = 0;
        for(int i = 0; i < t->count(); i++)
        {
                char_symbol *cs;
                cs = w_main->alphabet.value(t->at(i).unicode(),0);
                if (cs == 0)
                        cs = w_main->alphabet.value(63,0);	//Per i caratteri non definiti uso il carattere '?'
                QPoint pstart, pend;
                foreach(char_point cp, cs->char_points)
                        {
                        int x = cp.x * passo;
                        int y = cp.y * passo;
                        if (font_italic)
                                x = x - y/4;
                        pstart = char_start + text_char_orientation(x, y);
                        tp->append(pstart);
                        }
                foreach(char_line cl, cs->char_lines)
                        {
                        int x1 = cl.x1 * passo;
                        int x2 = cl.x2 * passo;
                        int y1 = cl.y1 * passo;
                        int y2 = cl.y2 * passo;
                        if (font_italic)
                                {
                                x1 = x1 - y1/4;
                                x2 = x2 - y2/4;
                                }
                        pstart = char_start + text_char_orientation(x1, y1);
                        pend = char_start + text_char_orientation(x2, y2);
                        tl->append(pstart);
                        tl->append(pend);
                        }

                pos += cs->width;
                char_start = text_start + text_char_orientation(pos*passo, 0);
        }
}

inline	void	myGraphicsItem::text_draw(QPainter *painter, QMatrix *m, QString *t, QPoint text_start)
{
        if (drag_or_move_active)
                text_start += drag_or_move_delta;
        text_to_polygon(t, text_start, &text_points, &text_lines);

        qreal	passo = font_height * 60 * m->m11();
        int	pixel_width;
	if (font_bold)
                pixel_width = 2.6 * passo;
        else
                pixel_width = 1.8 * passo;
        if (pixel_width <= 2)
		pixel_width = 0;	
	pen_layer.setWidth(pixel_width);
	painter->setPen(pen_layer);

        int size = text_points.count();
        for(int i = 0; i < size; i++)
                painter->drawPoint(m->map(text_points.at(i)));

        size = text_lines.count();
        for(int i = 0; i < size; i++)
                {
                QPoint pstart = m->map(text_lines.at(i++));
                QPoint pend = m->map(text_lines.at(i));
                painter->drawLine(QPointF(pstart), QPointF(pend));
                }
}

inline	QPoint	myGraphicsItem::text_char_orientation(int x, int y)
{
	QPoint	pos;
	switch (text_position)
		{
		case 'R':
			pos = QPoint(x, y);
			if (mirror_text_on)
				pos.setX(-pos.x()); 
			break;
		case 'L':
                        if (GV->is_sch)
                                pos = QPoint(x, y);
                        else
                                pos = QPoint(-x, -y);
                        if (mirror_text_on)
                                pos.setX(-pos.x());
                        break;
		case 'T':
			pos = QPoint(y, -x);
			if (mirror_text_on)
				pos.setY(-pos.y()); 
			break;
		case 'B':
			if (GV->is_sch)
				pos = QPoint(y, -x);
			else
				pos = QPoint(-y, +x);
			if (mirror_text_on)
				pos.setY(-pos.y());
			break;
		}	
	return pos;
}

void	myGraphicsItem::info_update()
{
	QPoint s, e;
	
	//Ricalcolo tutte le variabili che cominciano per info 
	switch (tipo)
	{
	case O_CONNECTION:
		start = node_start->point;
		end = node_end->point;
		int xleft, xright;
		int ytop, ybottom;
		xleft = start.x();
		xright = end.x();
		if (xleft > xright)
			swap_int(xleft,xright);
		ytop = start.y();
		ybottom = end.y();
		if (ytop < ybottom)
			swap_int(ytop,ybottom);
		info_br = QRect(xleft - pen_width/2, ybottom - pen_width/2, xright - xleft + pen_width, ytop - ybottom + pen_width);
		break;
	case O_POLYLINE:
		{
		info_br = polygon.boundingRect();
		int pw_div_2 = pen_width/2;
		info_br.adjust(- pw_div_2, - pw_div_2, pw_div_2, pw_div_2);
		}
		break;
	case O_POLYGON:
		{
		info_br = polygon.boundingRect();
                if (!fill_polygon)
                        {
                        int pw_div_2 = pen_width/2;
                        info_br.adjust(- pw_div_2, - pw_div_2, pw_div_2, pw_div_2);
                        }
                }
		break;
	case O_RECT:
	case O_ELLIPSE:
		{
                info_br = QRect(start.x(), start.y(), width, height);
		int pw_div_2 = pen_width/2;
		info_br.adjust(- pw_div_2, - pw_div_2, pw_div_2, pw_div_2);
		}
		break;
	case O_IDENT:
	case O_VALUE:
	case O_TEXT:
		{
                switch (tipo)
		{
                case O_IDENT:
			if (comp)
                                text = ((myComponent *) comp)->ident;
			else
                                text = GV->schcomp_or_pcbcomp->ident;
			break;
		case O_VALUE:
			if (comp)
                                text = ((myComponent *) comp)->value;
			else
                                text = GV->schcomp_or_pcbcomp->value;
			break;
		}
                if (text.isEmpty())
                        info_br = QRect(0,0,0,0);
                else
                        {
                        QRect tr = text_rect(&text);
                        info_br = text_br(tr);
                        info_start_text = start;
                        if (GV->is_sch)
                                switch (text_position)
                                {
                                case 'L':
                                        info_start_text.setX(start.x() - tr.width());
                                        break;
                                case 'B':
                                        info_start_text.setY(start.y() + tr.height());
                                        break;
                                }
                        info_br.translate(info_start_text);
                        }
                }
		break;
	case O_PIN:
		{
		info_br = QRect(node_start->point.x() - 1000, node_start->point.y() - 1000, 2000, 2000);
		QRect	shaperect; 
		QPoint s = node_start->point;
		int x = s.x();
		int y = s.y();
                switch (shape)
		{
		case PIN_SHAPE_LINE:
			if (text_position == 'R')
				shaperect = QRect(x, y, 20000, 0);
			else if (text_position == 'L')
				shaperect = QRect(x - 20000, y, 20000, 0);
			else if (text_position == 'T')
				shaperect = QRect(x, y - 20000, 0, 20000);
			else if (text_position == 'B')
				shaperect = QRect(x, y, 0, 20000);
			break;
		case PIN_SHAPE_DOT:
			if (text_position == 'R')
				shaperect = QRect(x, y - 2500, 20000, 5000);
			else if (text_position == 'L')
				shaperect = QRect(x - 20000, y - 2500, 20000, 5000);
			else if (text_position == 'T')
				shaperect = QRect(x - 2500, y - 20000, 5000, 20000);
			else if (text_position == 'B')
				shaperect = QRect(x - 2500, y, 5000, 20000);
			break;
		case PIN_SHAPE_CLOCK:
		case PIN_SHAPE_DOT_CLOCK:
			if (text_position == 'R')
				shaperect = QRect(x, y - 4000, 24000, 8000);
			else if (text_position == 'L')
				shaperect = QRect(x - 24000, y - 4000, 24000, 8000);
			else if (text_position == 'T')
				shaperect = QRect(x - 4000, y - 24000, 8000, 24000);
			else if (text_position == 'B')
				shaperect = QRect(x - 4000, y, 8000, 24000);
			break;
			}
		info_br = info_br.united(shaperect);
		myComponent *c = (myComponent *) comp;
		if (file_type == FT_SCH_SYMBOL)
			c = GV->schcomp_or_pcbcomp;
		if (c->pin_number_is_visible)
                        {
                        QString num_to_qstr = QString::number(number);
                        QRect	numberrect = text_rect(&num_to_qstr);
			info_start_pin_number_text = node_start->point;
                        switch (text_position)
			{
			case 'R':
				info_start_pin_number_text += QPoint(0, -2000);
				break;
			case 'L':
                                info_start_pin_number_text += QPoint(-text_length(&num_to_qstr), -2000);
				break;
			case 'T':
				info_start_pin_number_text += QPoint(-2000, 0);
				break;
			case 'B':
                                info_start_pin_number_text += QPoint(-2000, text_length(&num_to_qstr));
				break;
			}
			numberrect.translate(info_start_pin_number_text);
			info_br = info_br.united(text_br(numberrect));
                        }
		if (c->pin_name_is_visible)
                        if (!text.isEmpty())
				{
                                QRect	namerrect = text_br(text_rect(&text));
				info_start_text = node_start->point;
                                switch (text_position)
				{
				case 'R':
					info_start_text += QPoint(25000, 2500);
					break;
				case 'L':
					info_start_text += QPoint(-25000 - text_length(&text), 2500);
					break;
				case 'T':
					info_start_text += QPoint(2500, -25000);
					break;
				case 'B':
					info_start_text += QPoint(2500, 25000 + text_length(&text));
					break;
				}
				namerrect.translate(info_start_text);
                                info_br = info_br.united(namerrect);
				}
		}
		break;
        case O_POWER:
                {
                info_br = QRect(node_start->point.x() - 1000, node_start->point.y() - 1000, 2000, 2000);
                QPoint sym_tl, sym_br;
                switch (shape)
                {
                case SYMBOL_GROUND_POWER:
                        switch (text_position)
                        {
                        case 'R':
                                sym_tl = QPoint(node_start->point.x() - 6000, node_start->point.y());
                                sym_br = QPoint(node_start->point.x() + 6000, node_start->point.y() - 6500);
                                break;
                        case 'L':
                                sym_tl = QPoint(node_start->point.x() - 6000, node_start->point.y());
                                sym_br = QPoint(node_start->point.x() + 6000, node_start->point.y() + 6500);
                                break;
                        case 'T':
                                sym_tl = QPoint(node_start->point.x(), node_start->point.y() - 6000);
                                sym_br = QPoint(node_start->point.x() - 6500, node_start->point.y() + 6000);
                                break;
                        case 'B':
                                sym_tl = QPoint(node_start->point.x(), node_start->point.y() - 6000);
                                sym_br = QPoint(node_start->point.x() + 6500, node_start->point.y() + 6000);
                                break;
                        }
                        break;
                case SYMBOL_GROUND_SIGNAL:
                        switch (text_position)
                        {
                        case 'R':
                                sym_tl = QPoint(node_start->point.x() - 4500, node_start->point.y());
                                sym_br = QPoint(node_start->point.x() + 4500, node_start->point.y() - 7000);
                                break;
                        case 'L':
                                sym_tl = QPoint(node_start->point.x() - 4500, node_start->point.y());
                                sym_br = QPoint(node_start->point.x() + 4500, node_start->point.y() + 7000);
                                break;
                        case 'T':
                                sym_tl = QPoint(node_start->point.x(), node_start->point.y() - 4500);
                                sym_br = QPoint(node_start->point.x() - 7000, node_start->point.y() + 4500);
                                break;
                        case 'B':
                                sym_tl = QPoint(node_start->point.x(), node_start->point.y() - 4500);
                                sym_br = QPoint(node_start->point.x() + 7000, node_start->point.y() + 4500);
                                break;
                        }
                        break;
                case SYMBOL_GROUND_EARTH:
                        switch (text_position)
                        {
                        case 'R':
                                sym_tl = QPoint(node_start->point.x() - 8000, node_start->point.y());
                                sym_br = QPoint(node_start->point.x() + 6000, node_start->point.y() - 6500);
                                break;
                        case 'L':
                                sym_tl = QPoint(node_start->point.x() - 8000, node_start->point.y());
                                sym_br = QPoint(node_start->point.x() + 6000, node_start->point.y() + 6500);
                                break;
                        case 'T':
                                sym_tl = QPoint(node_start->point.x(), node_start->point.y() - 8000);
                                sym_br = QPoint(node_start->point.x() - 6500, node_start->point.y() + 6000);
                                break;
                        case 'B':
                                sym_tl = QPoint(node_start->point.x(), node_start->point.y() - 8000);
                                sym_br = QPoint(node_start->point.x() + 6500, node_start->point.y() + 6000);
                                break;
                        }
                        break;
                case SYMBOL_POWER_CIRCLE:
                        switch (text_position)
                        {
                        case 'R':
                                sym_tl = QPoint(node_start->point.x() - 2000, node_start->point.y());
                                sym_br = QPoint(node_start->point.x() + 2000, node_start->point.y() - 7000);
                                break;
                        case 'L':
                                sym_tl = QPoint(node_start->point.x() - 2000, node_start->point.y());
                                sym_br = QPoint(node_start->point.x() + 2000, node_start->point.y() + 7000);
                                break;
                        case 'T':
                                sym_tl = QPoint(node_start->point.x(), node_start->point.y() - 2000);
                                sym_br = QPoint(node_start->point.x() - 7000, node_start->point.y() + 2000);
                                break;
                        case 'B':
                                sym_tl = QPoint(node_start->point.x(), node_start->point.y() - 2000);
                                sym_br = QPoint(node_start->point.x() + 7000, node_start->point.y() + 2000);
                                break;
                        }
                        break;
                case SYMBOL_POWER_BAR:
                        switch (text_position)
                        {
                        case 'R':
                                sym_tl = QPoint(node_start->point.x() - 5000, node_start->point.y());
                                sym_br = QPoint(node_start->point.x() + 5000, node_start->point.y() - 5000);
                                break;
                        case 'L':
                                sym_tl = QPoint(node_start->point.x() - 5000, node_start->point.y());
                                sym_br = QPoint(node_start->point.x() + 5000, node_start->point.y() + 5000);
                                break;
                        case 'T':
                                sym_tl = QPoint(node_start->point.x(), node_start->point.y() - 5000);
                                sym_br = QPoint(node_start->point.x() - 5000, node_start->point.y() + 5000);
                                break;
                        case 'B':
                                sym_tl = QPoint(node_start->point.x(), node_start->point.y() - 5000);
                                sym_br = QPoint(node_start->point.x() + 5000, node_start->point.y() + 5000);
                                break;
                        }
                        break;
                }
                info_br = info_br.united(QRect(sym_tl, sym_br));
                if (fill_polygon)
                        {
                        QRect	namerrect = text_br(text_rect(&text));
                        info_start_text = node_start->point;
                        switch (text_position)
                        {
                        case 'R':
                                info_start_text += QPoint(-text_length(&text)/2, -15000 + 12 *font_height * 60);//- namerrect.width()
                                namerrect.translate(info_start_text);
                                info_br = info_br.united(namerrect);
                                break;
                        case 'L':
                                info_start_text += QPoint(-text_length(&text)/2, 15000);
                                namerrect.translate(info_start_text);
                                info_br = info_br.united(namerrect);
                                break;
                        case 'T':
                                info_start_text += QPoint(-15000 + 12 *font_height * 60, text_length(&text)/2);//+ namerrect.width()
                                namerrect.translate(info_start_text);
                                info_br = info_br.united(namerrect);
                                break;
                        case 'B':
                                info_start_text += QPoint(15000, text_length(&text)/2);
                                namerrect.translate(info_start_text);
                                info_br = info_br.united(namerrect);
                                break;
                        }
                        }
                }
                break;
	case O_PORT:
		{
                QPoint s = node_start->point;
                info_br = QRect(s.x() - 1000, s.y() - 1000, 2000, 2000);
                port_points_update(s);
                info_start_text = s;
                switch (text_position)
                {
                case 'R':
                        info_start_text += QPoint(7500, 2500);
                        break;
                case 'L':
                        info_start_text += QPoint(-7500 - text_length(&text), 2500);
                        break;
                case 'T':
                        info_start_text += QPoint(2500, -7500);
                        break;
                case 'B':
                        info_start_text += QPoint(2500, 7500 + text_length(&text));
                        break;
                }
                info_br = info_br.united(QRect(QPoint(port_l.x(), port_tl.y()), QPoint(port_r.x(), port_br.y())));
                }
		break;
	case O_PAD:
	case O_VIA:
                info_br = QRect(node_start->point.x() - width/2, node_start->point.y() - height/2, width, height);
                if (layer_from != layer_to)
                        info_br = info_br.united(QRect(node_start->point.x() - drill/2, node_start->point.y() - drill/2, drill, drill));
                break;
	}

	//A questo punto eseguo info_update() del componente
	if (comp)
		{
		myComponent *c = (myComponent *) comp;
		c->info_update();

		}
}

void myGraphicsItem::paint(QPainter *painter, QMatrix *m)
{
        QPoint s, e;
        switch (tipo)
	{
        case O_POWER:
                {
                QPoint s = node_start->point;
                if (drag_or_move_active)
                        s += drag_or_move_delta;
                int x = s.x();
                int y = s.y();
                if (node_start->connected_wires.count() == 0)
                        {
                        pen_layer.setWidth(0);
                        painter->setPen(pen_layer);
                        painter->drawLine(m->map(QPoint(x - 1000, y - 1000)), m->map(QPoint(x + 1000, y + 1000)));
                        painter->drawLine(m->map(QPoint(x - 1000, y + 1000)), m->map(QPoint(x + 1000, y - 1000)));
                        }
                int pixel_width = SCH_PEN_NORMAL * m->m11();
                pen_layer.setWidth(pixel_width);
                painter->setPen(pen_layer);
                painter->setBrush(Qt::NoBrush);

                switch (shape)
                {
                case SYMBOL_GROUND_POWER:
                        if (text_position == 'R')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y - 2500)));
                                painter->drawLine(m->map(QPoint(x - 6000, y - 2500)), m->map(QPoint(x + 6000, y - 2500)));
                                painter->drawLine(m->map(QPoint(x - 4000, y - 4500)), m->map(QPoint(x + 4000, y - 4500)));
                                painter->drawLine(m->map(QPoint(x - 2000, y - 6500)), m->map(QPoint(x + 2000, y - 6500)));
                                }
                        else if (text_position == 'L')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y + 2500)));
                                painter->drawLine(m->map(QPoint(x - 6000, y + 2500)), m->map(QPoint(x + 6000, y + 2500)));
                                painter->drawLine(m->map(QPoint(x - 4000, y + 4500)), m->map(QPoint(x + 4000, y + 4500)));
                                painter->drawLine(m->map(QPoint(x - 2000, y + 6500)), m->map(QPoint(x + 2000, y + 6500)));
                                }
                        else if (text_position == 'T')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x -2500, y)));
                                painter->drawLine(m->map(QPoint(x - 2500, y - 6000)), m->map(QPoint(x - 2500, y + 6000)));
                                painter->drawLine(m->map(QPoint(x - 4500, y - 4000)), m->map(QPoint(x - 4500, y + 4000)));
                                painter->drawLine(m->map(QPoint(x - 6500, y - 2000)), m->map(QPoint(x - 6500, y + 2000)));
                                }
                        else if (text_position == 'B')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x + 2500, y)));
                                painter->drawLine(m->map(QPoint(x + 2500, y - 6000)), m->map(QPoint(x + 2500, y + 6000)));
                                painter->drawLine(m->map(QPoint(x + 4500, y - 4000)), m->map(QPoint(x + 4500, y + 4000)));
                                painter->drawLine(m->map(QPoint(x + 6500, y - 2000)), m->map(QPoint(x + 6500, y + 2000)));
                                }
                        break;
                case SYMBOL_GROUND_SIGNAL:
                        if (text_position == 'R')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y - 2500)));
                                painter->drawLine(m->map(QPoint(x - 4500, y - 2500)), m->map(QPoint(x + 4500, y - 2500)));
                                painter->drawLine(m->map(QPoint(x - 4500, y - 2500)), m->map(QPoint(x, y - 7000)));
                                painter->drawLine(m->map(QPoint(x + 4500, y - 2500)), m->map(QPoint(x, y - 7000)));
                                }
                        else if (text_position == 'L')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y + 2500)));
                                painter->drawLine(m->map(QPoint(x - 4500, y + 2500)), m->map(QPoint(x + 4500, y + 2500)));
                                painter->drawLine(m->map(QPoint(x - 4500, y + 2500)), m->map(QPoint(x, y + 7000)));
                                painter->drawLine(m->map(QPoint(x + 4500, y + 2500)), m->map(QPoint(x, y + 7000)));
                                }
                        else if (text_position == 'T')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x - 2500, y)));
                                painter->drawLine(m->map(QPoint(x - 2500, y - 4500)), m->map(QPoint(x - 2500, y + 4500)));
                                painter->drawLine(m->map(QPoint(x - 2500, y - 4500)), m->map(QPoint(x - 7000, y)));
                                painter->drawLine(m->map(QPoint(x - 2500, y + 4500)), m->map(QPoint(x - 7000, y)));
                                }
                        else if (text_position == 'B')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x + 2500, y)));
                                painter->drawLine(m->map(QPoint(x + 2500, y - 4500)), m->map(QPoint(x + 2500, y + 4500)));
                                painter->drawLine(m->map(QPoint(x + 2500, y - 4500)), m->map(QPoint(x + 7000, y)));
                                painter->drawLine(m->map(QPoint(x + 2500, y + 4500)), m->map(QPoint(x + 7000, y)));
                                }
                        break;
                case SYMBOL_GROUND_EARTH:
                        if (text_position == 'R')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y - 2500)));
                                painter->drawLine(m->map(QPoint(x - 6000, y - 2500)), m->map(QPoint(x + 6000, y - 2500)));
                                painter->drawLine(m->map(QPoint(x - 4000, y - 2500)), m->map(QPoint(x - 8000, y - 6500)));
                                painter->drawLine(m->map(QPoint(x, y - 2500)), m->map(QPoint(x - 4000, y - 6500)));
                                painter->drawLine(m->map(QPoint(x + 4000, y - 2500)), m->map(QPoint(x, y - 6500)));
                                }
                        else if (text_position == 'L')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y + 2500)));
                                painter->drawLine(m->map(QPoint(x - 6000, y + 2500)), m->map(QPoint(x + 6000, y + 2500)));
                                painter->drawLine(m->map(QPoint(x - 4000, y + 2500)), m->map(QPoint(x - 8000, y + 6500)));
                                painter->drawLine(m->map(QPoint(x, y + 2500)), m->map(QPoint(x - 4000, y + 6500)));
                                painter->drawLine(m->map(QPoint(x + 4000, y + 2500)), m->map(QPoint(x, y + 6500)));
                                }
                        else if (text_position == 'T')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x - 2500, y)));
                                painter->drawLine(m->map(QPoint(x - 2500, y - 6000)), m->map(QPoint(x - 2500, y + 6000)));
                                painter->drawLine(m->map(QPoint(x - 2500, y - 4000)), m->map(QPoint(x - 6500, y - 8000)));
                                painter->drawLine(m->map(QPoint(x - 2500, y)), m->map(QPoint(x - 6500, y - 4000)));
                                painter->drawLine(m->map(QPoint(x - 2500, y + 4000)), m->map(QPoint(x - 6500, y)));
                                }
                        else if (text_position == 'B')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x + 2500, y)));
                                painter->drawLine(m->map(QPoint(x + 2500, y - 6000)), m->map(QPoint(x + 2500, y + 6000)));
                                painter->drawLine(m->map(QPoint(x + 2500, y - 4000)), m->map(QPoint(x + 6500, y - 8000)));
                                painter->drawLine(m->map(QPoint(x + 2500, y)), m->map(QPoint(x + 6500, y - 4000)));
                                painter->drawLine(m->map(QPoint(x + 2500, y + 4000)), m->map(QPoint(x + 6500, y)));
                                }
                        break;
                case SYMBOL_POWER_CIRCLE:
                        if (text_position == 'R')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y - 3000)));
                                painter->drawEllipse(QRect(m->map(QPoint(x - 2000, y - 3000)), m->map(QPoint(x + 2000, y - 7000))));
                                }
                        else if (text_position == 'L')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y + 3000)));
                                painter->drawEllipse(QRect(m->map(QPoint(x - 2000, y + 3000)), m->map(QPoint(x + 2000, y + 7000))));
                                }
                        else if (text_position == 'T')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x - 3000, y)));
                                painter->drawEllipse(QRect(m->map(QPoint(x - 3000, y - 2000)), m->map(QPoint(x - 7000, y + 2000))));
                                }
                        else if (text_position == 'B')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x + 3000, y)));
                                painter->drawEllipse(QRect(m->map(QPoint(x + 3000, y - 2000)), m->map(QPoint(x + 7000, y + 2000))));
                                }
                        break;
                case SYMBOL_POWER_BAR:
                        if (text_position == 'R')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y - 5000)));
                                painter->drawLine(m->map(QPoint(x - 5000, y - 5000)), m->map(QPoint(x + 5000, y - 5000)));
                                }
                        else if (text_position == 'L')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y + 5000)));
                                painter->drawLine(m->map(QPoint(x - 5000, y + 5000)), m->map(QPoint(x + 5000, y + 5000)));
                                }
                        else if (text_position == 'T')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x - 5000, y)));
                                painter->drawLine(m->map(QPoint(x - 5000, y - 5000)), m->map(QPoint(x - 5000, y + 5000)));
                                }
                        else if (text_position == 'B')
                                {
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x + 5000, y)));
                                painter->drawLine(m->map(QPoint(x + 5000, y - 5000)), m->map(QPoint(x + 5000, y + 5000)));
                                }
                        break;
                }
                if (fill_polygon)
                        text_draw(painter, m, &text, info_start_text);
                }
                break;
        case O_CONNECTION:
		{
                s = start;
                e = end;
		if (drag_or_move_active)
			{
			if (node_start->drag_or_move_active)
				s += drag_or_move_delta; 
			if (node_end->drag_or_move_active)
				e += drag_or_move_delta;
			}
		int pixel_width = pen_width * m->m11();	
		pen_layer.setWidth(pixel_width);
		painter->setPen(pen_layer);
		QPoint pend = m->map(e);
		QPoint pstart = m->map(s); 

                beautiful_45_degrees(pstart, pend);
		painter->drawLine(pstart, pend);
		}
		break;
	case O_POLYLINE:
		{
		int pixel_width = pen_width * m->m11();	
		pen_layer.setWidth(pixel_width);
		painter->setPen(pen_layer);
		QPoint pstart = polygon.at(0);
		if (drag_or_move_active)
                        {
                        if (GV->tool == T_EDIT_NODE)
                                {
                                if (GV->wand_pol_index == 0)
                                        pstart += drag_or_move_delta;
                                }
                        else
                                pstart += drag_or_move_delta;
                        }
                pstart = m->map(pstart);
		for (int i = 1; i < polygon.count();i++)
			{
			QPoint pend = polygon.at(i);
			if (drag_or_move_active)
                                {
                                if (GV->tool == T_EDIT_NODE)
                                        {
                                        if (GV->wand_pol_index == i)
                                                pend += drag_or_move_delta;
                                        }
                                else
                                        pend += drag_or_move_delta;
                                }
                        pend = m->map(pend);
			QPoint new_pstart = pend;	//Salvo in new_pstart il valore di pend prima che venga modificato da beautiful_45_degrees()
                        beautiful_45_degrees(pstart, pend);
			painter->drawLine(pstart, pend);
			pstart = new_pstart;		
			}
		//painter->drawPolyline(m->map(polygon));	//Non va bene perchè mi fa i 45° brutti
		}
		break;
	case O_POLYGON:
		{
                if (fill_polygon)
                        {
                        pen_layer.setWidth(0);
                        painter->setPen(pen_layer);
			painter->setBrush(brush_layer);
                        }
		else
                        {
                        int pixel_width = pen_width * m->m11();
                        pen_layer.setWidth(pixel_width);
                        painter->setPen(pen_layer);
                        painter->setBrush(Qt::NoBrush);
                        }
		if (drag_or_move_active)
			{
			QPolygon p_translated = polygon;
                        if (GV->tool == T_EDIT_NODE)
                                p_translated.setPoint(GV->wand_pol_index, p_translated.at(GV->wand_pol_index) + drag_or_move_delta);
                        else
                                p_translated.translate(drag_or_move_delta);
                        painter->drawPolygon(m->map(p_translated), Qt::OddEvenFill);
			}
		else
                        painter->drawPolygon(m->map(polygon), Qt::OddEvenFill);
		}
		break;
	case O_RECT:
		{
		int pixel_width = pen_width * m->m11();	
		pen_layer.setWidth(pixel_width);
		painter->setPen(pen_layer);
		if (fill_polygon)
			painter->setBrush(brush_layer);
		else
			painter->setBrush(Qt::NoBrush);
                s = start;
		if (drag_or_move_active)
			s += drag_or_move_delta;
		s = m->map(s);
                painter->drawRect(s.x(), s.y(), width * m->m11(), height * m->m11());
		}
		break;
	case O_ELLIPSE:
		{
		painter->setPen(Qt::NoPen);
		int pixel_width = pen_width * m->m11();	
		pen_layer.setWidth(pixel_width);
		painter->setPen(pen_layer);
		if (fill_polygon)
			painter->setBrush(brush_layer);
		else
			painter->setBrush(Qt::NoBrush);
		QPoint s = start;
		if (drag_or_move_active)
			s += drag_or_move_delta;
		s = m->map(s);
                painter->drawEllipse(s.x(), s.y(), width * m->m11(), height * m->m11());
		}
		break;
	case O_TEXT:
        case O_IDENT:
        case O_VALUE:
                text_draw(painter, m, &text, info_start_text);
		break;
        case O_PIN:
		{
                s = node_start->point;
		if (drag_or_move_active)
			s += drag_or_move_delta;
                int x = s.x();
                int y = s.y();
		if (node_start->connected_wires.count() == 0)
			{
			pen_layer.setWidth(0);
			painter->setPen(pen_layer);					
                        painter->drawLine(m->map(QPoint(x - 1000, y - 1000)), m->map(QPoint(x + 1000, y + 1000)));
                        painter->drawLine(m->map(QPoint(x - 1000, y + 1000)), m->map(QPoint(x + 1000, y - 1000)));
			}
                int pixel_width = SCH_PEN_NORMAL * m->m11();
		pen_layer.setWidth(pixel_width);
		painter->setPen(pen_layer);
		painter->setBrush(Qt::NoBrush);
                switch (shape)
		{
		case PIN_SHAPE_LINE:
			if (text_position == 'R')
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x + 20000, y)));
			else if (text_position == 'L')
                                painter->drawLine(m->map(QPoint(x - 20000, y)), m->map(QPoint(x, y)));
			else if (text_position == 'T')
                                painter->drawLine(m->map(QPoint(x, y  - 20000)), m->map(QPoint(x, y)));
			else if (text_position == 'B')
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y + 20000)));
			break;
		case PIN_SHAPE_DOT:

			if (text_position == 'R')
				{
                                painter->drawLine( m->map(QPoint(x, y)), m->map(QPoint(x + 15000, y)) );
                                painter->drawEllipse( m->map(QPoint(x + 17500, y)), (int)(2500 * m->m11()), (int)(2500 * m->m11()) );
				}
			else if (text_position == 'L')
				{
                                painter->drawLine( m->map(QPoint(x, y)), m->map(QPoint(x - 15000, y)));
                                painter->drawEllipse( m->map(QPoint(x - 17500, y)), (int)(2500 * m->m11()), (int)(2500 * m->m11()) );
				}
			else if (text_position == 'T')
				{
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y - 15000)));
                                painter->drawEllipse( m->map(QPoint(x, y  - 17500)), (int)(2500 * m->m11()), (int)(2500 * m->m11()) );
				}
			else if (text_position == 'B')
				{
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y + 15000)));
                                painter->drawEllipse( m->map(QPoint(x, y + 17500)), (int)(2500 * m->m11()), (int)(2500 * m->m11()) );
				}
			break;
		case PIN_SHAPE_CLOCK:
			if (text_position == 'R')
				{
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x + 20000, y)));
                                painter->drawLine(m->map(QPoint(x + 20000, y + 4000)), m->map(QPoint(x + 24000, y)));
                                painter->drawLine(m->map(QPoint(x + 20000, y - 4000)), m->map(QPoint(x + 24000, y)));
				}
			else if (text_position == 'L')
				{
                                painter->drawLine(m->map(QPoint(x - 20000, y)), m->map(QPoint(x, y)));
                                painter->drawLine(m->map(QPoint(x - 20000, y + 4000)), m->map(QPoint(x - 24000, y)));
                                painter->drawLine(m->map(QPoint(x - 20000, y - 4000)), m->map(QPoint(x - 24000, y)));
				}
			else if (text_position == 'T')
				{
                                painter->drawLine(m->map(QPoint(x, y  - 20000)), m->map(QPoint(x, y)));
                                painter->drawLine(m->map(QPoint(x + 4000, y - 20000)), m->map(QPoint(x, y - 24000)));
                                painter->drawLine(m->map(QPoint(x - 4000, y - 20000)), m->map(QPoint(x, y - 24000)));
				}
			else if (text_position == 'B')
				{
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y + 20000)));
                                painter->drawLine(m->map(QPoint(x + 4000, y + 20000)), m->map(QPoint(x, y + 24000)));
                                painter->drawLine(m->map(QPoint(x - 4000, y + 20000)), m->map(QPoint(x, y + 24000)));
				}
			break;
		case PIN_SHAPE_DOT_CLOCK:
			if (text_position == 'R')
				{
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x + 15000, y)));
                                painter->drawEllipse( m->map(QPoint(x + 17500, y)), (int)(2500 * m->m11()), (int)(2500 * m->m11()) );
                                painter->drawLine(m->map(QPoint(x + 20000, y + 4000)), m->map(QPoint(x + 24000, y)));
                                painter->drawLine(m->map(QPoint(x + 20000, y - 4000)), m->map(QPoint(x + 24000, y)));
				}
			else if (text_position == 'L')
				{
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x - 15000, y)));
                                painter->drawEllipse( m->map(QPoint(x - 17500, y)), (int)(2500 * m->m11()), (int)(2500 * m->m11()) );
                                painter->drawLine(m->map(QPoint(x - 20000, y + 4000)), m->map(QPoint(x - 24000, y)));
                                painter->drawLine(m->map(QPoint(x - 20000, y - 4000)), m->map(QPoint(x - 24000, y)));
				}
			else if (text_position == 'T')
				{
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y - 15000)));
                                painter->drawEllipse( m->map(QPoint(x, y  - 17500)), (int)(2500 * m->m11()), (int)(2500 * m->m11()) );
                                painter->drawLine(m->map(QPoint(x + 4000, y - 20000)), m->map(QPoint(x, y - 24000)));
                                painter->drawLine(m->map(QPoint(x - 4000, y - 20000)), m->map(QPoint(x, y - 24000)));
				}
			else if (text_position == 'B')
				{
                                painter->drawLine(m->map(QPoint(x, y)), m->map(QPoint(x, y + 15000)));
                                painter->drawEllipse( m->map(QPoint(x, y + 17500)), (int)(2500 * m->m11()), (int)(2500 * m->m11()) );
                                painter->drawLine(m->map(QPoint(x + 4000, y + 20000)), m->map(QPoint(x, y + 24000)));
                                painter->drawLine(m->map(QPoint(x - 4000, y + 20000)), m->map(QPoint(x, y + 24000)));
				}
			break;
		}
		myComponent *c = (myComponent *) comp;
		if (file_type == FT_SCH_SYMBOL)
			c = GV->schcomp_or_pcbcomp;
		if (c->pin_number_is_visible)
			{
			QString t = QString::number(number);
			text_draw(painter, m, &t, info_start_pin_number_text);
			}
		if (c->pin_name_is_visible)
			if (!text.isEmpty())
				text_draw(painter, m, &text, info_start_text);
		}
		break;	
	case O_PAD:
        case O_VIA:
                {
                //Il foro viene disegnato in seguito in drawForeground()
                QPoint	start;
                if (drag_or_move_active)
                        start = node_start->point + drag_or_move_delta;
                else
                        start = node_start->point;
                if (pcb_pad_via_filled)
                        {
                        painter->setPen(Qt::NoPen);
                        painter->setBrush(brush_layer);
                        }
                else
                        {
                        pen_layer.setWidth(0);
                        painter->setPen(pen_layer);
                        painter->setBrush(Qt::NoBrush);
                        }
                switch (shape)
                {
                case PS_ROUND:
                        start = m->map(start);
                        painter->drawEllipse( start, (int)(width * m->m11() / 2), (int)(height * m->m11() / 2 ) );
                        break;
                case PS_RECTANGLE:
                        start = m->map(start - QPoint(width / 2, height / 2));
                        painter->drawRect(start.x(), start.y(), width * m->m11(), height * m->m11());
                        break;
                case PS_ROUNDED_RECTANGLE:
                        start = m->map(start - QPoint(width / 2, height / 2));
                        qreal radius;
                        if (width > height)
                                radius = height * m->m11() / 2;
                        else
                                radius = width * m->m11() / 2;
                        //Conviene usare radius in Qt::AbsoluteSize perchè il Qt::RelativeSize da degli errori
                        painter->drawRoundedRect(start.x(), start.y(), width * m->m11(), height * m->m11(), radius, radius, Qt::AbsoluteSize);
                        break;
                }
                }
                break;
        case O_PORT:
                {
                QPoint s = node_start->point;
                if (drag_or_move_active)
                        s += drag_or_move_delta;
                int x = s.x();
                int y = s.y();
                if (node_start->connected_wires.count() == 0)
                        {
                        pen_layer.setWidth(0);
                        painter->setPen(pen_layer);
                        painter->drawLine(m->map(QPoint(x - 1000, y - 1000)), m->map(QPoint(x + 1000, y + 1000)));
                        painter->drawLine(m->map(QPoint(x - 1000, y + 1000)), m->map(QPoint(x + 1000, y - 1000)));
                        }
                int pixel_width = SCH_PEN_NORMAL * m->m11();
                pen_layer.setWidth(pixel_width);
                painter->setPen(pen_layer);
                painter->setBrush(Qt::NoBrush);
                port_points_update(s);
                painter->drawLine(m->map(s), m->map(port_l));
                QPolygon p;
                switch (shape)
                {
                case PORT_GENERIC:
                        p << m->map(port_tl) << m->map(port_tr) << m->map(port_br) << m->map(port_bl);
                        break;
                case PORT_INPUT:
                        p << m->map(port_tl) << m->map(port_tr) << m->map(port_br) << m->map(port_bl) << m->map(port_l);
                        break;
                case PORT_OUTPUT:
                        p << m->map(port_tl) << m->map(port_tr) << m->map(port_r) << m->map(port_br) << m->map(port_bl);
                        break;
                case PORT_BIDIRECTIONAL:
                        p << m->map(port_tl) << m->map(port_tr) << m->map(port_r) << m->map(port_br) << m->map(port_bl) << m->map(port_l);
                        break;
                }
                if (fill_polygon)
                        {
                        painter->setBrush(brush_layer);
                        painter->drawPolygon(p, Qt::OddEvenFill);
                        }
                else
                        {
                        p << m->map(port_tl);
                        painter->drawPolyline(p);
                        }
                text_draw(painter, m, &text, info_start_text);
                }
                break;
        case O_SEGMENT:
                {
                s = polygon.at(GV->wand_pol_index);
                if (GV->wand_pol_index)
                        e = polygon.at(GV->wand_pol_index - 1);
                else
                        e = polygon.last();
                int pixel_width = pen_width * m->m11();
                pen_layer.setWidth(pixel_width);
                painter->setPen(pen_layer);
                QPoint pend = m->map(e);
                QPoint pstart = m->map(s);
                painter->drawLine(pstart, pend);
                }
                break;
        }
}

void myGraphicsItem::segment_paint(QPainter *painter, QMatrix *m)
{
        char old_tipo = tipo;
        tipo = O_SEGMENT;
        paint(painter, m);
        tipo = old_tipo;
}

bool myGraphicsItem::segment_hitTest(QPoint point)
{
        char old_tipo = tipo;
        tipo = O_SEGMENT;
        bool answer = hitTest(point);
        tipo = old_tipo;
        return answer;
}

void    myGraphicsItem::polygon_interpolation(QPolygon *pol)
{
        static const double Pi = 3.14159265358979323846264338327950288419717;

        int x;
        int y;
        int steps;
        int semi_w = width / 2;
        int semi_h = height / 2;

        switch (tipo)
        {
        case O_ELLIPSE:
                x = start.x() + semi_w;
                y = start.y() + semi_h;
                break;
        case O_PAD:
        case O_VIA:
                x = node_start->point.x();
                y = node_start->point.y();
                break;
        }
        //Stabilisco il numero di punti del poligono
        steps = 36;
        if ((width > 20000) || (height > 20000))    //Sopra il 0.2"
                steps = 72;
        if ((width > 40000) || (height > 40000))    //Sopra il 0.4"
                steps = 144;

        pol->clear();
        for (int i = 0; i < 360; i += 360 / steps)
                {
                float alpha = i * (Pi / 180.0) ;
                float sinalpha = sin(alpha);
                float cosalpha = cos(alpha);
                pol->append(QPoint(x + semi_w * cosalpha, y + semi_h * sinalpha));
                }
}

void    myGraphicsItem::port_points_update(QPoint s)
{
        int x = s.x();
        int y = s.y();
        int boxlength = text_length(&text) + 12500;
        switch (shape)
        {
        case PORT_GENERIC:
                if (text_position == 'R')
                        {
                        port_l = QPoint(x + 2500, y);
                        port_r = QPoint(boxlength + x, y);
                        port_tl = port_l + QPoint(0, 5000);
                        port_tr = port_r + QPoint(0, 5000);
                        port_bl = port_l - QPoint(0, 5000);
                        port_br = port_r - QPoint(0, 5000);
                        }
                else if (text_position == 'L')
                        {
                        port_l = QPoint(x - 2500, y);
                        port_r = QPoint(- boxlength + x, y);
                        port_tl = port_l + QPoint(0, 5000);
                        port_tr = port_r + QPoint(0, 5000);
                        port_bl = port_l - QPoint(0, 5000);
                        port_br = port_r - QPoint(0, 5000);
                        }
                else if (text_position == 'T')
                        {
                        port_l = QPoint(x, y - 2500);
                        port_r = QPoint(x, y - boxlength);
                        port_tl = port_l + QPoint(5000, 0);
                        port_tr = port_r + QPoint(5000, 0);
                        port_bl = port_l - QPoint(5000, 0);
                        port_br = port_r - QPoint(5000, 0);
                        }
                else if (text_position == 'B')
                        {
                        port_l = QPoint(x, y + 2500);
                        port_r = QPoint(x, y + boxlength);
                        port_tl = port_l + QPoint(5000, 0);
                        port_tr = port_r + QPoint(5000, 0);
                        port_bl = port_l - QPoint(5000, 0);
                        port_br = port_r - QPoint(5000, 0);
                        }
                break;
        case PORT_INPUT:
                if (text_position == 'R')
                        {
                        port_l = QPoint(x + 2500, y);
                        port_r = QPoint(boxlength + x, y);
                        port_tl = port_l + QPoint(5000, 5000);
                        port_tr = port_r + QPoint(0, 5000);
                        port_bl = port_l - QPoint(-5000, 5000);
                        port_br = port_r - QPoint(0, 5000);
                        }
                else if (text_position == 'L')
                        {
                        port_l = QPoint(x - 2500, y);
                        port_r = QPoint(- boxlength + x, y);
                        port_tl = port_l + QPoint(-5000, 5000);
                        port_tr = port_r + QPoint(0, 5000);
                        port_bl = port_l - QPoint(5000, 5000);
                        port_br = port_r - QPoint(0, 5000);
                        }
                else if (text_position == 'T')
                        {
                        port_l = QPoint(x, y - 2500);
                        port_r = QPoint(x, y - boxlength);
                        port_tl = port_l + QPoint(5000, -5000);
                        port_tr = port_r + QPoint(5000, 0);
                        port_bl = port_l - QPoint(5000, 5000);
                        port_br = port_r - QPoint(5000, 0);
                        }
                else if (text_position == 'B')
                        {
                        port_l = QPoint(x, y + 2500);
                        port_r = QPoint(x, y + boxlength);
                        port_tl = port_l + QPoint(5000, 5000);
                        port_tr = port_r + QPoint(5000, 0);
                        port_bl = port_l - QPoint(5000, -5000);
                        port_br = port_r - QPoint(5000, 0);
                        }
                break;
        case PORT_OUTPUT:
                if (text_position == 'R')
                        {
                        port_l = QPoint(x + 2500, y);
                        port_r = QPoint(boxlength + x, y);
                        port_tl = port_l + QPoint(0, 5000);
                        port_tr = port_r + QPoint(-5000, 5000);
                        port_bl = port_l - QPoint(0, 5000);
                        port_br = port_r - QPoint(5000, 5000);
                        }
                else if (text_position == 'L')
                        {
                        port_l = QPoint(x - 2500, y);
                        port_r = QPoint(- boxlength + x, y);
                        port_tl = port_l + QPoint(0, 5000);
                        port_tr = port_r + QPoint(5000, 5000);
                        port_bl = port_l - QPoint(0, 5000);
                        port_br = port_r - QPoint(-5000, 5000);
                        }
                else if (text_position == 'T')
                        {
                        port_l = QPoint(x, y - 2500);
                        port_r = QPoint(x, y - boxlength);
                        port_tl = port_l + QPoint(5000, 0);
                        port_tr = port_r + QPoint(5000, 5000);
                        port_bl = port_l - QPoint(5000, 0);
                        port_br = port_r - QPoint(5000, -5000);
                        }
                else if (text_position == 'B')
                        {
                        port_l = QPoint(x, y + 2500);
                        port_r = QPoint(x, y + boxlength);
                        port_tl = port_l + QPoint(5000, 0);
                        port_tr = port_r + QPoint(5000, -5000);
                        port_bl = port_l - QPoint(5000, 0);
                        port_br = port_r - QPoint(5000, 5000);
                        }
                break;
        case PORT_BIDIRECTIONAL:
                if (text_position == 'R')
                        {
                        port_l = QPoint(x + 2500, y);
                        port_r = QPoint(boxlength + x, y);
                        port_tl = port_l + QPoint(5000, 5000);
                        port_tr = port_r + QPoint(-5000, 5000);
                        port_bl = port_l - QPoint(-5000, 5000);
                        port_br = port_r - QPoint(5000, 5000);
                        }
                else if (text_position == 'L')
                        {
                        port_l = QPoint(x - 2500, y);
                        port_r = QPoint(- boxlength + x, y);
                        port_tl = port_l + QPoint(-5000, 5000);
                        port_tr = port_r + QPoint(5000, 5000);
                        port_bl = port_l - QPoint(5000, 5000);
                        port_br = port_r - QPoint(-5000, 5000);
                        }
                else if (text_position == 'T')
                        {
                        port_l = QPoint(x, y - 2500);
                        port_r = QPoint(x, y - boxlength);
                        port_tl = port_l + QPoint(5000, -5000);
                        port_tr = port_r + QPoint(5000, 5000);
                        port_bl = port_l - QPoint(5000, 5000);
                        port_br = port_r - QPoint(5000, -5000);
                        }
                else if (text_position == 'B')
                        {
                        port_l = QPoint(x, y + 2500);
                        port_r = QPoint(x, y + boxlength);
                        port_tl = port_l + QPoint(5000, 5000);
                        port_tr = port_r + QPoint(5000, -5000);
                        port_bl = port_l - QPoint(5000, -5000);
                        port_br = port_r - QPoint(5000, 5000);
                        }
                break;
        }
}

bool    myGraphicsItem::layer_intersect(char l_from, char l_to)
{
        if (tipo == O_PIN)
                return TRUE;    //Nello schematico c'è sempre l'intersezione

        if (layer_from <= l_from)
                {
                if (layer_to >= l_from)
                        return TRUE;
                }
        else
                {
                if (l_to >= layer_from)
                        return TRUE;
                }
        return FALSE;
}

bool    myGraphicsItem::is_showed()
{
        for (int i = layer_from; i <= layer_to; i++)
                if (GV->layer_settings[i].visible == TRUE)
                        return TRUE;

        return FALSE;
}


