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

#ifndef MY_COMPONENT_H
#define MY_COMPONENT_H

class myComponent
{	
public:
    myComponent();
	~myComponent()
	{}

	QList<myGraphicsItem *>	all_obj;

        QString up;         //Ungated Pins
        QString devices;    //Devices e footprint (Usato solo nei simboli dello schematico)

	bool	is_selected;

	QRect   info_br;
	void	info_update();

	QRect	bounding_rect();
	
	void	set_selected(bool sel);
	bool	hitTest(QPoint point);
	void	append(myGraphicsItem *obj);
	void	move(QPoint delta);

	QString alias;
        QString	ident;
	QString	value;

	QString	footprint_link;
        QStringList component_names;

	bool	pin_number_is_visible;		
        bool	pin_name_is_visible;

        int     pin_count();
        int     pin_max_number();
        int     pad_count();
        int     pad_max_number();
        myGraphicsItem     *pin_with_number(int num);
        myGraphicsItem     *pad_with_number(int num);
        bool    footprint_compatibility(myComponent *pcb_comp);

        signed int	idstamp;

        int		idstamp_pointer;	//Punta al footprint o al simbolo con lo stesso idstamp

        QPoint	anchor_point;			//E' il punto attorno a cui ruota l'oggetto (indispensabile nel PCB)
                                                //Quando si esegue add_component esso viene impostato al punto cliccato

        bool    is_showed();
};

#endif
