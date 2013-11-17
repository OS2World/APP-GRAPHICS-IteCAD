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


#ifndef CHAR_EDITOR_H
#define CHAR_EDITOR_H

#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QUndoCommand>
#include "tipi_dato.h"

class char_editor : public QLabel
{	
	Q_OBJECT	//Necessario per gestire Signals e Slots	
public:
	char_editor(QWidget *parent);	
	~char_editor()
	{}

	QUndoStack	ur;

	bool	clicked;
	char	click_x;
	char	click_y;

	char	move_x;
	char	move_y;

	char_symbol			cs;
	char_symbol			bg_cs;
	
	QPoint	from_grid_to_pixel(int gridx, int gridy);
	QPoint	from_pixel_to_grid(int pixelx, int pixely);

	char	mode;
	char	weight;

	int		sel_line;
	int		sel_point;

public slots:	
	
protected:
	void	paintEvent(QPaintEvent *event);
	void	mouseMoveEvent(QMouseEvent *event);
	void	mousePressEvent(QMouseEvent *event);
};

class char_draw_line : public QUndoCommand
{
public:
    char_draw_line(char_line cl_draw, char_editor *father_draw);
	virtual	void undo();
	virtual void redo();

	char_editor	*father;
	char_line	cl;
};

class char_draw_point : public QUndoCommand
{
public:
    char_draw_point(char_point cp_draw, char_editor *father_draw);
	virtual	void undo();
	virtual void redo();

	char_editor	*father;
	char_point	cp;
};

class char_erase_line : public QUndoCommand
{
public:
    char_erase_line(int sel_line_erase, char_editor *father_erase);
	virtual	void undo();
	virtual void redo();

	int			sel_line;
	char_editor	*father;
	char_line	cl;
};

class char_erase_point : public QUndoCommand
{
public:
    char_erase_point(int sel_point_erase, char_editor *father_erase);
	virtual	void undo();
	virtual void redo();

	int			sel_point;
	char_editor	*father;
	char_point	cp;
};

#endif
