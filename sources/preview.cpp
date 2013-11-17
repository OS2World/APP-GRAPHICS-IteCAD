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

#include "preview.h"

extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;
extern	win_main *w_main;

extern	char			file_type;

myGraphicsView	*preview_GV;
myGraphicsScene	*preview_GS;

preview::preview()
{	
        preview_GS = new myGraphicsScene();
        preview_GV = new myGraphicsView(preview_GS, 0, TRUE, "");
	preview_GV->setFrameStyle(QFrame::StyledPanel);	
	preview_GV->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);	//Purtroppo le scroolBar sono indispensabili, altrimenti lo zoom è errato...
	preview_GV->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);		//Purtroppo le scroolBar sono indispensabili, altrimenti lo zoom è errato...
	preview_GV->is_pixmapoutput = TRUE;
	preview_GV->setEnabled(FALSE);
	preview_GV->setFixedWidth(300);
	preview_GV->setFixedHeight(300);
}

bool	preview::load(QString file, char f_type, bool is_sch)
{
        myGraphicsView *old_GV = GV;
	myGraphicsScene *old_GS = GS;
	GV = preview_GV;
	GS = preview_GS;
        char old_file_type = file_type;
        file_type = f_type;
	
	GV->is_sch = is_sch;

	bool	loaded;
	
	GV->setUpdatesEnabled(FALSE);
	GS->clear();
	GV->clear();

	if (file == "")
		loaded = FALSE;
	else
		{
		xml_sax_handler handler;
		if (handler.open_file(file, TRUE))
			loaded = TRUE; 
		else
			loaded = FALSE;
		}	

	GV = old_GV;
	GS = old_GS;
        file_type = old_file_type;

        return loaded;
}

bool	preview::load_pixmap(QString file, QLabel *preview_label, char f_type, bool is_sch)
{
        myGraphicsView *old_GV = GV;
	myGraphicsScene *old_GS = GS;
        GV = preview_GV;
	GS = preview_GS;
        char old_file_type = file_type;
        file_type = f_type;

        GV->is_sch = is_sch;

	bool	loaded;
	
	GV->setUpdatesEnabled(FALSE);
	GS->clear();
	GV->clear();

	if (file == "")
		{		
		GV->setUpdatesEnabled(TRUE);
		preview_label->setPixmap(0);
		loaded = FALSE;
		}
	else
		{
		xml_sax_handler handler;
		if (handler.open_file(file, TRUE))
			{
			GV->setUpdatesEnabled(TRUE);	
			preview_label->setPixmap(QPixmap::grabWidget(GV->viewport(), GV->viewport()->rect()));
			loaded = TRUE;
			}
		else
			{
			GV->setUpdatesEnabled(TRUE);
			preview_label->setPixmap(0);
			loaded = FALSE;
			}
		}	

	GV = old_GV;
        GS = old_GS;
        file_type = old_file_type;

        return loaded;
}

QSize	preview::size()
{
        return QSize(300, 300);
}
