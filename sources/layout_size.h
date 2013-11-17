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

#ifndef LAYOUT_SIZE_H
#define LAYOUT_SIZE_H

#include <QComboBox>
#include <QLabel>
#include <QRadioButton>
#include "layout_length.h"

class layout_size : public QGridLayout
{	
	Q_OBJECT	//Necessario per gestire Signals e Slots	
public:
    layout_size(int page_size_input, int page_width_input, int page_height_input, bool	show_orientation);	
	~layout_size()
	{}

	int		old_page_size;
	int		old_page_width;
	int		old_page_height;

	QComboBox		*size_format;
	layout_length	*size_Width;
	layout_length	*size_Height;
	
	QRadioButton *portrait;
	QRadioButton *landscape;
	QLabel *immagine_pagina;

	int page_size;
	bool page_landscape;

	void	setup();
	void	swap();

public slots:
	void	change_page_orientation(bool checked);
	void	change_page_size(int index);
};

#endif
