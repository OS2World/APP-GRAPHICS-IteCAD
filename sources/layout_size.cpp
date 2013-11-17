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

#include "layout_size.h"

extern	stampa_formato stampa_formati[];
extern	unsigned char UM;

layout_size::layout_size(int page_size_input, int page_width_input, int page_height_input, bool	show_orientation)
        : QGridLayout()
{	
	old_page_size = page_size_input;
	old_page_width = page_width_input;
	old_page_height = page_height_input;
	
	//Formato della pagina
	size_format = new QComboBox();
	for (int i = 0; i< STAMPA_FORMATI_COUNT; i++)
		size_format->addItem(stampa_formati[i].descrizione);
	size_format->setCurrentIndex(old_page_size);
	QLabel *size_width = new QLabel(tr("Width:"));
	size_width->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	size_Width = new layout_length("", 10000, 1000, 10000000, FALSE);
	QLabel *size_height = new QLabel(tr("Height:"));
	size_height->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	size_Height = new layout_length("", 10000, 1000, 10000000, FALSE);
	QGridLayout *layout_size = new QGridLayout();
	layout_size->addWidget(size_format,0,0,TRUE,FALSE);
	layout_size->addWidget(size_width,1,0);
	layout_size->addWidget(size_height,2,0);
	layout_size->addLayout(size_Width,1,1);
	layout_size->addLayout(size_Height,2,1);
	QGroupBox *gb_size = new QGroupBox("Size");	
	gb_size->setLayout(layout_size);
	
	QObject::connect(size_format, SIGNAL(activated(int)), this, SLOT(change_page_size(int)));

	//Orientamento della pagina
	portrait = new QRadioButton(tr("Portrait"));
	landscape = new QRadioButton(tr("Landscape"));
	immagine_pagina = new QLabel();	
		
	QGridLayout *layout_orientation = new QGridLayout();
	layout_orientation->addWidget(immagine_pagina,0,0,FALSE,TRUE);
	layout_orientation->addWidget(portrait,0,1);
	layout_orientation->addWidget(landscape,1,1);
	QGroupBox *gb_orientation = new QGroupBox("Orientation");	
	gb_orientation->setLayout(layout_orientation);
	QObject::connect(portrait, SIGNAL(toggled(bool)), this, SLOT(change_page_orientation(bool)));
	QObject::connect(landscape, SIGNAL(toggled(bool)), this, SLOT(change_page_orientation(bool)));
	
	
	//Carico i dati iniziali
	change_page_size(old_page_size);
	size_Width->setLength(old_page_width);
	size_Height->setLength(old_page_height);
	size_Width->setUM(stampa_formati[old_page_size].UM);
	size_Height->setUM(stampa_formati[old_page_size].UM);
	if (size_Width->length > size_Height->length)
		{
		landscape->setChecked(TRUE);
		immagine_pagina->setPixmap(QPixmap(":/Resources/landscape.png"));
		}
	else
		{
		portrait->setChecked(TRUE);
		immagine_pagina->setPixmap(QPixmap(":/Resources/portrait.png"));
		}
		
	addWidget(gb_size,0,0);
	if (show_orientation)
		addWidget(gb_orientation,1,0);	
}

void layout_size::change_page_orientation(bool checked)
{
	//La variabile checked non viene considerata dentro a questa funzione
	if (landscape->isChecked())
		{
		page_landscape = TRUE;
		immagine_pagina->setPixmap(QPixmap(":/Resources/landscape.png"));
		if (size_Width->length < size_Height->length)
			swap();
		}
	else
		{
		page_landscape = FALSE;
		immagine_pagina->setPixmap(QPixmap(":/Resources/portrait.png"));
		if (size_Width->length > size_Height->length)
			swap();
		}
}

void layout_size::change_page_size(int index) 
{
	page_size = index;
	size_format->setCurrentIndex(index);
	size_Width->setUM(stampa_formati[index].UM);
	size_Height->setUM(stampa_formati[index].UM);
	if (landscape->isChecked())
		{
		size_Width->setLength(stampa_formati[index].altezza);
		size_Height->setLength(stampa_formati[index].larghezza);
		}
	else
		{
		size_Width->setLength(stampa_formati[index].larghezza);
		size_Height->setLength(stampa_formati[index].altezza);
		}	
	if (index >= (STAMPA_FORMATI_COUNT - 2))
		{
		size_Width->setDisabled(FALSE);
		size_Height->setDisabled(FALSE);
		}
	else
		{
		size_Width->setDisabled(TRUE);
		size_Height->setDisabled(TRUE);
		}
}

void layout_size::swap()
{
	int w = size_Width->length;
	size_Width->setLength(size_Height->length);
	size_Height->setLength(w);
}
