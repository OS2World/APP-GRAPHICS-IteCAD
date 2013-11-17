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

#include "char_editor.h"

char_editor::char_editor(QWidget *parent)
        : QLabel(parent)
{	
	setFixedWidth(601);
	setFixedHeight(601);
	setFrameShape(QFrame::NoFrame);
	setMouseTracking(TRUE);

	mode = 'D';
	weight = 42;	//passo *1.4 è non grasseto, mentre passo * 1.8 è grassetto
	clicked = FALSE;
	sel_line = -1;
	sel_point = -1;
}

void char_editor::paintEvent(QPaintEvent *event)
{
    QColor grigio = QColor(200,200,200);
	QColor grigioscuro = QColor(140,140,140);
	
	QPainter painter(this);
	painter.setBrush(QColor(255,255,255));
    painter.drawRect(0,0,641,641);
	painter.setPen(grigio);
	for (int x = 0; x <= 20; x++)
		painter.drawLine(x*30, 0, x*30, 600);
	for (int y = 0; y <= 20; y++)
		painter.drawLine(0, y*30, 600, y*30);
	painter.setPen(grigioscuro);
	painter.drawLine(0, 450, 600, 450);

	painter.setPen(QPen(grigio, weight, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	foreach (char_line cl, bg_cs.char_lines)
		painter.drawLine(from_grid_to_pixel(cl.x1, cl.y1), from_grid_to_pixel(cl.x2, cl.y2));
	foreach (char_point cp, bg_cs.char_points)
		painter.drawPoint(from_grid_to_pixel(cp.x, cp.y));
	
	painter.setPen(QPen(QColor(0,0,0), weight, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	foreach (char_line cl, cs.char_lines)
		painter.drawLine(from_grid_to_pixel(cl.x1, cl.y1), from_grid_to_pixel(cl.x2, cl.y2));
	foreach (char_point cp, cs.char_points)
		painter.drawPoint(from_grid_to_pixel(cp.x, cp.y));

	switch (mode)
	{
	case 'D':
		if (clicked)
			{
			//Traccio la linea in procinto di essere disegnata
			painter.setPen(QPen(QColor(255,0,0), weight, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			if ((move_x == click_x) && (move_y == click_y))
				painter.drawPoint(from_grid_to_pixel(click_x, click_y));
			else
				painter.drawLine(from_grid_to_pixel(click_x, click_y), from_grid_to_pixel(move_x, move_y));
			}
		break;
	case 'E':
		if (sel_point >= 0)
			{
			painter.setPen(QPen(QColor(255,0,0), weight, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			char_point cp = cs.char_points.at(sel_point);
			painter.drawPoint(from_grid_to_pixel(cp.x, cp.y));
			}
		if (sel_line >= 0)
			{
			painter.setPen(QPen(QColor(255,0,0), weight, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			char_line cl = cs.char_lines.at(sel_line);
			painter.drawLine(from_grid_to_pixel(cl.x1, cl.y1), from_grid_to_pixel(cl.x2, cl.y2));
			}
		break;
	}

	painter.setBrush(grigioscuro);
	painter.setPen(grigioscuro);
	painter.drawRect(30*cs.width,0,601,601);
}

void char_editor::mouseMoveEvent(QMouseEvent *event)
{
	int	last_sel_point = sel_point;
	int	last_sel_line = sel_line;
	
	QPoint mouse = event->pos();
	int x = mouse.x();
	int y = mouse.y();
	switch (mode)
	{
	case 'D':
		{
		QPoint move = from_pixel_to_grid(mouse.x(), mouse.y());
		move_x = move.x();
		move_y = move.y();
		setUpdatesEnabled(FALSE);
		setUpdatesEnabled(TRUE);
		}
		break;
	case 'E':
		static QImage image = QImage(1, 1, QImage::Format_RGB32);	
		QColor rosso = QColor(255,0,0);
		
		QPainter painter(&image);
		painter.begin(&image);
		painter.fillRect(0,0,1,1,QColor(255,255,255));
		painter.end();
		sel_point = 0;
		sel_line = -1;
		foreach (char_point cp, cs.char_points)
			{
			QPainter painter(&image);
			painter.setPen(QPen(rosso, weight, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			painter.begin(&image);
			painter.drawPoint(from_grid_to_pixel(cp.x, cp.y) - QPoint(x,y));
			painter.end();

			QColor pixel_color = image.pixel(0,0);
			if (pixel_color == rosso)
				{
				if (last_sel_point != sel_point)
					{
					setUpdatesEnabled(FALSE);
					setUpdatesEnabled(TRUE);
					}
				return;
				}
			sel_point++;
			}
		sel_point = -1;
		sel_line = 0;
		foreach (char_line cl, cs.char_lines)
			{
			QPainter painter(&image);
			painter.setPen(QPen(rosso, weight, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			painter.begin(&image);
			painter.drawLine(from_grid_to_pixel(cl.x1, cl.y1) - QPoint(x,y), from_grid_to_pixel(cl.x2, cl.y2) - QPoint(x,y));
			painter.end();

			QColor pixel_color = image.pixel(0,0);
			if (pixel_color == rosso)
				{
				if (last_sel_line != sel_line)
					{
					setUpdatesEnabled(FALSE);
					setUpdatesEnabled(TRUE);
					}
				return;
				}
			sel_line++;
			}
		sel_line = -1;
		if (last_sel_line != sel_line)
			{
			setUpdatesEnabled(FALSE);
			setUpdatesEnabled(TRUE);
			}	
		break;
	}
}

void char_editor::mousePressEvent(QMouseEvent *event)
{
	QPoint mouse = event->pos();
	
	switch (mode)
	{
	case 'D':
		if (clicked == FALSE)
			{
			QPoint click = from_pixel_to_grid(mouse.x(), mouse.y());
			click_x = click.x();
			click_y = click.y();
			clicked = TRUE;
			}
		else
			{
			//Aggiungo la linea nella lista delle linee
			char_line	cl;
			cl.x1 = click_x;
			cl.y1 = click_y;
			QPoint help = from_pixel_to_grid(mouse.x(), mouse.y());
			cl.x2 = help.x();
			cl.y2 = help.y();
			if ((cl.x1 == cl.x2) && (cl.y1 == cl.y2))
				{
				char_point cp;
				cp.x = cl.x1;
				cp.y = cl.y1;
				ur.push(new char_draw_point(cp, this));
				}
			else
				ur.push(new char_draw_line(cl, this));
			clicked = FALSE;
			}
		break;
	case 'E':
		if (sel_line >= 0)
			{
			ur.push(new char_erase_line(sel_line, this));
			sel_line = -1;
			}
		if (sel_point >= 0)
			{
			ur.push(new char_erase_point(sel_point, this));
			sel_point = -1;
			}
		break;
	}
	setUpdatesEnabled(FALSE);
	setUpdatesEnabled(TRUE);
}

QPoint	char_editor::from_grid_to_pixel(int gridx, int gridy)
{
	return	QPoint(gridx*30 + 15, (gridy+14)*30 + 15);
}

QPoint	char_editor::from_pixel_to_grid(int pixelx, int pixely)
{
	int gridx = pixelx/30;
	if (gridx == 20)
		gridx = 15;
	int gridy = pixely/30-14;
	if (gridy == 20)
		gridy = 15;

	return QPoint(gridx, gridy);
}

char_draw_line::char_draw_line(char_line cl_draw, char_editor *father_draw)
{
	father = father_draw;
	cl = cl_draw;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	char_draw_line::redo()
{
	father->cs.char_lines.append(cl);
	father->setUpdatesEnabled(FALSE);
	father->setUpdatesEnabled(TRUE);
}

void	char_draw_line::undo()
{
	father->cs.char_lines.removeLast();
	father->setUpdatesEnabled(FALSE);
	father->setUpdatesEnabled(TRUE);
}

char_draw_point::char_draw_point(char_point cp_draw, char_editor *father_draw)
{
	father = father_draw;
	cp = cp_draw;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	char_draw_point::redo()
{
	father->cs.char_points.append(cp);
	father->setUpdatesEnabled(FALSE);
	father->setUpdatesEnabled(TRUE);
}

void	char_draw_point::undo()
{
	father->cs.char_points.removeLast();
	father->setUpdatesEnabled(FALSE);
	father->setUpdatesEnabled(TRUE);
}

char_erase_line::char_erase_line(int sel_line_erase, char_editor *father_erase)
{
	father = father_erase;
	sel_line = sel_line_erase;
	cl = father->cs.char_lines.at(sel_line);
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	char_erase_line::redo()
{
	father->cs.char_lines.removeAt(sel_line);
	father->setUpdatesEnabled(FALSE);
	father->setUpdatesEnabled(TRUE);
}

void	char_erase_line::undo()
{
	father->cs.char_lines.insert(sel_line, cl);
	father->setUpdatesEnabled(FALSE);
	father->setUpdatesEnabled(TRUE);
}

char_erase_point::char_erase_point(int sel_point_erase, char_editor *father_erase)
{
	father = father_erase;
	sel_point = sel_point_erase;
	cp = father->cs.char_points.at(sel_point);
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	char_erase_point::redo()
{
	father->cs.char_points.removeAt(sel_point);
	father->setUpdatesEnabled(FALSE);
	father->setUpdatesEnabled(TRUE);
}

void	char_erase_point::undo()
{
	father->cs.char_points.insert(sel_point, cp);
	father->setUpdatesEnabled(FALSE);
	father->setUpdatesEnabled(TRUE);
}
