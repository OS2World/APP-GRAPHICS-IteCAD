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

#ifndef DRC_H
#define DRC_H

#include "tipi_dato.h"
#include "myGraphicsItem.h"
#include "myGraphicsView.h"
#include "win_main.h"

void	drc_insulation_on_all_objects();
bool	drc_insulation_on_single_object(myGraphicsItem *obj);
bool	drc_insulation_between_2_objects(myGraphicsItem *obj1, myGraphicsItem *obj2);
bool	drc_insulation_null_on_single_object(myGraphicsItem *obj);

void	drc_net_id_on_all_objects();
void	drc_net_id_recursive(myGraphicsItem *obj);

void	drc_connect_id_on_all_objects();
void	drc_connect_id_recursive(myGraphicsItem *obj);
void	drc_connect_id_recursive_for_clean_up(QList<myGraphicsItem *> *ao, myGraphicsItem *obj);

bool    basic_p1_p2();     //Su queste semplici funzioni si basa tutto il DRC
bool    basic_p1_l2();     //Su queste semplici funzioni si basa tutto il DRC
bool    basic_l1_p2();     //Su queste semplici funzioni si basa tutto il DRC
bool	basic_intersect_l1_l2();     //Su queste semplici funzioni si basa tutto il DRC
bool    basic_l1_l2();     //Su queste semplici funzioni si basa tutto il DRC

bool    basic_pol1_pol2();
bool    basic_p1_pol2();
bool    basic_pol1_p2();
bool    basic_l1_pol2();
bool    basic_pol1_l2();

///////////////////////////////////////////////////////////////////////////////////////////////////
// Si potrebbero forse evitare le duplicazioni delle funzioni sotto,
// ma ritengo che la ridondanza velocizza molto, inoltre non è così semplice ridurre il codice e le funzioni
///////////////////////////////////////////////////////////////////////////////////////////////////

bool    basic_p1_text2();
bool    basic_text1_p2();

bool    basic_text1_l2();
bool    basic_l1_text2();
bool    basic_text1_pol2();
bool    basic_pol1_text2();
bool    basic_text1_text2();

char    convert_obj1(myGraphicsItem *obj);
char    convert_obj2(myGraphicsItem *obj);
#endif
