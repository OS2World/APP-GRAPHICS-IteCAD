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

#include "layout_font.h"

layout_font::layout_font(char f_height, bool f_bold, bool f_italic)
        : QHBoxLayout()
{
	////////////////////////////////////////////////////////////////////////////////
        //Uso un font interno al programma, ma se dovessi usare i font del s.o allora
        //proporrei solamente i font multipiattaforma (standard windows fonts)
	//ossia quelli presenti in tutte le versioni di windows, nei mac e in linux
        //Userei di default il font Lucida Sans Unicode che è presente in tutti i windows a partire da Win98
	//Sul Mac il font Lucida Sans Unicode si chiama Lucida Grande, è identico ed è il font di sitema utilizzato da Mac OS X!
	//Conviene usare questo font come default perchè è unicode (moltissimi caratteri) ed è praticamente identico al  DejaVu Sans Mono che è free ed è un progetto in continuo aggiornamento
	//Potrei usare anche il font Courier New, ma mi piace di meno ed ha un brutto carattere di underscore
        //ATTENZIONE: PER VEDERE BENE I FONT BISOGNA ATTIVARE CLEARTYPE DENTRO A WINDOWS
        //POSSO INGLOBARE I FONT NEL PROGRAMMA USANDO QFontDatabase::addApplicationFontFromData() TOGO!!!!
	//////////////////////////////////////////////////////////////////////////////////////////////////
	
	size.setText(tr("Size"));
	
	font_height = f_height;
	font_bold = f_bold;
	font_italic = f_italic;
	
	for (int h = 8; h <= 100; h++)
		height.addItem(QString::number(h));
        setHeight(font_height);
        connect(&height,SIGNAL(currentIndexChanged(int)),this,SLOT(height_changed(int)));
	
	bold.setIcon(QIcon(":/Resources/textbold.png"));
	bold.setCheckable(TRUE);
	bold.setChecked(font_bold);
	bold.setAutoRaise(TRUE);
	connect(&bold,SIGNAL(clicked(bool)),this,SLOT(bold_changed(bool)));
	italic.setIcon(QIcon(":/Resources/textitalic.png"));
	italic.setCheckable(TRUE);
	italic.setChecked(font_italic);
	italic.setAutoRaise(TRUE);
	connect(&italic,SIGNAL(clicked(bool)),this,SLOT(italic_changed(bool)));

	addWidget(&size);
	addWidget(&height);
	addWidget(&bold);
	addWidget(&italic);
	addStretch(0);
}

void	layout_font::height_changed(int index)
{
	font_height = index + 8;

}

void	layout_font::bold_changed(bool checked)
{
	font_bold = bold.isChecked();
}

void	layout_font::italic_changed(bool checked)
{
	font_italic = italic.isChecked();
}

void	layout_font::setHeight(int new_height)
{
        font_height = new_height;
        height.setCurrentIndex(font_height - 8);
}

void	layout_font::setBold(bool new_bold)
{
        font_bold = new_bold;
        bold.setChecked(font_bold);
}

void	layout_font::setItalic(bool new_italic)
{
        font_italic = new_italic;
        italic.setChecked(font_italic);
}

