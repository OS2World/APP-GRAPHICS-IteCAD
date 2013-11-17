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
#include "myComponent.h"

extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

myComponent::myComponent()
{
        is_selected = FALSE;
	idstamp = 0;			//Serve nel caso inserisco un componente nel PCB senza passare dallo schematico
	idstamp_pointer = 0;	//Serve nel caso inserisco un componente nel PCB senza passare dallo schematico

	pin_number_is_visible = TRUE;		
	pin_name_is_visible = TRUE;
}

void	myComponent::append(myGraphicsItem *obj)
{
	all_obj.append(obj);
	obj->comp = (int) this;
}

QRect	myComponent::bounding_rect()
{
	return info_br;
}

void	myComponent::info_update()
{
	info_br = QRect(0,0,0,0);
	foreach(myGraphicsItem *obj, all_obj)
		info_br = info_br.unite(obj->info_br);
}

void	myComponent::set_selected(bool sel)
{
	if (is_selected == sel)
		return;		//Se il valore è lo stesso di prima allora esco senza fare niente
	
	is_selected = sel;	
	
	if (is_selected)
		{
		//Aggiungo questo oggetto alla lista degli oggetti selezionati
		if (!GV->selected_comp.contains(this))
			GV->selected_comp.append(this);
		}
	else
		GV->selected_comp.removeAll(this);

	foreach(myGraphicsItem *obj, all_obj)
		obj->set_selected(is_selected);
}

bool	myComponent::hitTest(QPoint point)
{
        if (!is_showed())
                return FALSE;

        foreach(myGraphicsItem *obj, all_obj)
		if (obj->hitTest(point))
			return TRUE;
	return FALSE;
}

void	myComponent::move(QPoint delta)
{
	anchor_point += delta;
	foreach(myGraphicsItem *obj, all_obj)
		obj->move(delta);
}

bool	myComponent::is_showed()
{
        foreach(myGraphicsItem *obj, all_obj)
                if (obj->is_showed())
                        return TRUE;
        return FALSE;
}

int	myComponent::pin_count()
{
        //Devo considerare anche gli Ungated Pins
        int num = 0;

        foreach(myGraphicsItem *obj, all_obj)
                if (obj->tipo == O_PIN)
                        num++;

        if (!up.isEmpty())
                {
                QStringList sl_up = up.split(";", QString::SkipEmptyParts);
                num += sl_up.count()/2;
                }

        return num;
}

int	myComponent::pin_max_number()
{
        //Devo considerare anche gli Ungated Pins
        int num = 0;

        foreach(myGraphicsItem *obj, all_obj)
                if (obj->tipo == O_PIN)
                        if (obj->number > num)
                                num = obj->number;

        if (!up.isEmpty())
                {
                QStringList sl_up = up.split(";", QString::SkipEmptyParts);
                for (int i = 0; i < sl_up.count(); i +=2)
                    if (sl_up.at(i).toInt() > num)
                                num = sl_up.at(i).toInt();
                }

        return num;
}

int	myComponent::pad_count()
{
        int num = 0;

        foreach(myGraphicsItem *obj, all_obj)
                if (obj->tipo == O_PAD)
                        num++;
        return num;
}

int	myComponent::pad_max_number()
{
        int num = 0;

        foreach(myGraphicsItem *obj, all_obj)
                if (obj->tipo == O_PAD)
                        if (obj->number > num)
                                num = obj->number;
        return num;
}

myGraphicsItem     *myComponent::pin_with_number(int num)
{
        //ATTENZIONE: Se il mun si riferisce ad un Ungated Pin allora la funzione restituisce 0
        foreach(myGraphicsItem *obj, all_obj)
                if (obj->tipo == O_PIN)
                        if (obj->number == num)
                                return obj;
        return 0;
}

myGraphicsItem     *myComponent::pad_with_number(int num)
{
        foreach(myGraphicsItem *obj, all_obj)
                if (obj->tipo == O_PAD)
                        if (obj->number == num)
                                return obj;
        return 0;
}

bool    myComponent::footprint_compatibility(myComponent *footprint)
{
        int num = pin_count();
        int max = pin_max_number();
        //Verifico che il footprint abbia un numero di piedini uguale
        if (num != footprint->pad_count())
                return FALSE;
        if (max != footprint->pad_max_number())
                return FALSE;

        for (int i = 0; i <= max; i++)
                {
                myGraphicsItem *obj = pin_with_number(i);
                if (obj)
                        if (footprint->pad_with_number(i) == 0)
                                return FALSE;
                }

         if (!up.isEmpty())
                {
                QStringList sl_up = up.split(";", QString::SkipEmptyParts);
                for (int i = 0; i < sl_up.count(); i +=2)
                        if (footprint->pad_with_number(sl_up.at(i).toInt()) == 0)
                                return FALSE;
                }

        return TRUE;
}


