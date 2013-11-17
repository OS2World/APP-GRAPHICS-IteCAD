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

#include "undo_redo.h"

extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

extern	win_main *w_main;
extern	char	file_type;	

extern	myGraphicsView	*preview_GV;
extern	myGraphicsScene	*preview_GS;

extern  QList<myGraphicsItem *>     clipboard_obj;
extern  QList<myComponent *>        clipboard_comp;
extern  QList<node_data *>          clipboard_nodes;

add_node::add_node(node_data *new_node) 
{
	node = new_node;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	add_node::redo() 
{
	GV->all_nodes.append(node);	
	GV->selected_nodes.append(node);	
	GV->do_selection_rect_recalculate = TRUE;
}

void	add_node::undo() 
{
	GV->all_nodes.removeAll(node);	
	GV->selected_nodes.removeAll(node);	
	GV->do_selection_rect_recalculate = TRUE;
}

add_object::add_object(myGraphicsItem *add_obj)
{
        obj = add_obj;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	add_object::redo() 
{
	if (obj->tipo == O_CONNECTION)
		{
		if (!obj->node_start->connected_wires.contains((int) obj))
			obj->node_start->connected_wires.append((int) obj);
		if (!obj->node_end->connected_wires.contains((int) obj))
			obj->node_end->connected_wires.append((int) obj);
		}
	obj->set_selected(TRUE);
	obj->info_update();
	GS->add_item(obj);	
	GV->do_selection_rect_recalculate = TRUE;
	GV->do_ratsnest_recalculate = TRUE;
}

void	add_object::undo() 
{
	if (obj->tipo == O_CONNECTION)
		{
		obj->node_start->connected_wires.removeAll((int) obj);
		obj->node_end->connected_wires.removeAll((int) obj);
		}
	GS->remove_item(obj);
	GV->do_selection_rect_recalculate = TRUE;
	GV->do_ratsnest_recalculate = TRUE;
}

add_component::add_component(myComponent *add_comp)
{
        comp = add_comp;
        //Subito dopo la creazione di questa classe viene eseguito redo()
}

void	add_component::redo()
{
        GV->all_comp.append(comp);
	foreach(myGraphicsItem *obj, comp->all_obj)
                GS->add_item(obj);
        //comp->set_selected(TRUE);
        //comp->info_update();
        GV->do_selection_rect_recalculate = TRUE;
        GV->do_ratsnest_recalculate = TRUE;
}

void	add_component::undo()
{
        GV->all_comp.removeAll(comp);
	foreach(myGraphicsItem *obj, comp->all_obj)
		GS->remove_item(obj);	
        GV->do_selection_rect_recalculate = TRUE;
        GV->do_ratsnest_recalculate = TRUE;
}

add_footprint::add_footprint(myComponent *footprint)
{
        sch_comp = footprint;
        pcb_comp = preview_GV->all_comp.first();
        pcb_comp->ident = sch_comp->ident;
        pcb_comp->value = sch_comp->value;
        pcb_comp->footprint_link = sch_comp->footprint_link;
        pcb_comp->idstamp = sch_comp->idstamp;
        pcb_comp->idstamp_pointer = (int) sch_comp;
        sch_comp->idstamp_pointer = (int) pcb_comp;
        foreach(node_data *node, preview_GV->all_nodes)
                pcb_nodes.append(node);
        //Elimino tutti i riferimenti al componente dentro preview_GV di modo che il suo clear() non mi crei problemi
        preview_GV->all_comp.clear();
        preview_GV->all_nodes.clear();
        preview_GS->all_obj.clear();
        //Subito dopo la creazione di questa classe viene eseguito redo()
}

void	add_footprint::redo()
{
        GV->all_comp.append(pcb_comp);
        foreach(myGraphicsItem *obj, pcb_comp->all_obj)
                GS->add_item(obj);
        foreach(node_data *node, pcb_nodes)
                GV->all_nodes.append(node);
}

void	add_footprint::undo()
{
        GV->all_comp.removeAll(pcb_comp);
        foreach(myGraphicsItem *obj, pcb_comp->all_obj)
                GS->remove_item(obj);
        foreach(node_data *node, pcb_nodes)
                GV->all_nodes.removeAll(node);
}

rotate_node::rotate_node(QPoint rot_center, node_data *rot_node, bool rot_clockwise)
{
	center = rot_center;
	node = rot_node;
	clockwise = rot_clockwise;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	rotate_node::redo() 
{
	node->point = rotate_point(node->point, center, clockwise);
	foreach (int obj, node->connected_wires)
		{
		myGraphicsItem *wire = (myGraphicsItem *) obj;
		wire->info_update();
		}
	if (node->connected_pin_or_pad_or_via)
		{
		myGraphicsItem *pin_or_pad = (myGraphicsItem *) node->connected_pin_or_pad_or_via;
		pin_or_pad->info_update();
		}
}

void	rotate_node::undo() 
{
	clockwise = !clockwise;
	redo();
	clockwise = !clockwise;
}

rotate_obj::rotate_obj(QPoint rot_center, myGraphicsItem *rot_obj, bool rot_clockwise)
{
	center = rot_center;
	obj = rot_obj;
	clockwise = rot_clockwise;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	rotate_obj::redo() 
{
	obj->rotate(center, clockwise);
	obj->set_selected(TRUE);
	obj->info_update();
	GV->do_selection_rect_recalculate = TRUE;
	GV->do_ratsnest_recalculate = TRUE;
}

void	rotate_obj::undo() 
{
	clockwise = !clockwise;
	redo();
	clockwise = !clockwise;
}

rotate_comp::rotate_comp(QPoint rot_center, myComponent *rot_comp, bool rot_clockwise)
{
	center = rot_center;
	comp = rot_comp;
	clockwise = rot_clockwise;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	rotate_comp::redo() 
{
	comp->anchor_point = rotate_point(comp->anchor_point, center, clockwise);
	foreach (myGraphicsItem *obj, comp->all_obj)
		{
		obj->rotate(center, clockwise);
		obj->set_selected(TRUE);
		obj->info_update();
		}
	comp->set_selected(TRUE);
	GV->do_selection_rect_recalculate = TRUE;
	GV->do_ratsnest_recalculate = TRUE;
}

void	rotate_comp::undo() 
{
	clockwise = !clockwise;
	redo();
	clockwise = !clockwise;
}

mirror_node::mirror_node(QPoint mir_center, node_data *mir_node, bool mir_horizontal)
{
	center = mir_center;
	node = mir_node;
	horizontal = mir_horizontal;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	mirror_node::redo() 
{
	node->point = mirror_point(node->point, center, horizontal);
	foreach (int obj, node->connected_wires)
		{
		myGraphicsItem *wire = (myGraphicsItem *) obj;
		wire->info_update();
		}
	if (node->connected_pin_or_pad_or_via)
		{
		myGraphicsItem *pin_or_pad = (myGraphicsItem *) node->connected_pin_or_pad_or_via;
		pin_or_pad->info_update();
		}
}

void	mirror_node::undo() 
{
	redo();
}

mirror_obj::mirror_obj(QPoint mir_center, myGraphicsItem *mir_obj, bool mir_horizontal)
{
	center = mir_center;
	obj = mir_obj;
	horizontal = mir_horizontal;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	mirror_obj::redo() 
{
	obj->mirror(center, horizontal);
	obj->set_selected(TRUE);
	obj->info_update();
	GV->do_selection_rect_recalculate = TRUE;
	GV->do_ratsnest_recalculate = TRUE;
}

void	mirror_obj::undo() 
{
	redo();
}

mirror_comp::mirror_comp(QPoint mir_center, myComponent *mir_comp, bool mir_horizontal)
{
	center = mir_center;
	comp = mir_comp;
	horizontal = mir_horizontal;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	mirror_comp::redo()  
{
	comp->anchor_point = mirror_point(comp->anchor_point, center, horizontal);
	foreach (myGraphicsItem *obj, comp->all_obj)
		{
		obj->mirror(center, horizontal);
		obj->set_selected(TRUE);
		obj->info_update();
		}
	comp->set_selected(TRUE);
	GV->do_selection_rect_recalculate = TRUE;
	GV->do_ratsnest_recalculate = TRUE;
}

void	mirror_comp::undo() 
{
	redo();
}

clipboard_clear::clipboard_clear()
{
        //Subito dopo la creazione di questa classe viene eseguito redo()
}

void	clipboard_clear::redo()
{

}

void	clipboard_clear::undo()
{
        clipboard_obj.clear();
        clipboard_comp.clear();
        clipboard_nodes.clear();
}

delete_selection::delete_selection() 
{
	foreach(myGraphicsItem *obj, GV->selected_obj)
		deleted_obj.append(obj);
	foreach(myComponent *comp, GV->selected_comp)
		deleted_comp.append(comp);
	foreach(node_data *node, GV->selected_nodes)
		deleted_nodes.append(node);
}

void	delete_selection::redo() 
{
	foreach(myGraphicsItem *obj, deleted_obj)
		{
		GS->remove_item(obj);
		obj->set_selected(FALSE);
		}
	foreach(myComponent *comp, deleted_comp)
		{
		GV->all_comp.removeAll(comp);
		foreach(myGraphicsItem *obj, comp->all_obj)
			{
			GS->remove_item(obj);
			}
		comp->set_selected(FALSE);
		}
	foreach(node_data *node, deleted_nodes)
		{
                GV->node_recalculate(node);
		if (GV->node_is_useless(node))
			{
			GV->all_nodes.removeAll(node);
			}
		}
	GV->do_selection_rect_recalculate = TRUE;
	GV->do_ratsnest_recalculate = TRUE;
}

void	delete_selection::undo() 
{
	foreach(myGraphicsItem *obj, deleted_obj)
		{
		GS->add_item(obj);
		obj->set_selected(TRUE);
		}
	foreach(myComponent *comp, deleted_comp)
		{
		GV->all_comp.append(comp);
		foreach(myGraphicsItem *obj, comp->all_obj)
			{
			GS->add_item(obj);
			}
		comp->set_selected(TRUE);
		}
	foreach(node_data *node, deleted_nodes)
		{
		GV->node_recalculate(node);
		if (!GV->all_nodes.contains(node))
			GV->all_nodes.append(node);
		}
	GV->do_selection_nodes = TRUE;
	GV->do_selection_rect_recalculate = TRUE;
	GV->do_ratsnest_recalculate = TRUE;
}

delete_clean_up::delete_clean_up()
{
	foreach(myGraphicsItem *obj, GS->all_obj)
		if (obj->highlight_active)
                        if (obj->tipo == O_CONNECTION)
                                deleted_connection.append(obj);
}

void	delete_clean_up::redo()
{
        foreach(myGraphicsItem *obj, deleted_connection)
                {
                GS->remove_item(obj);
                obj->set_selected(FALSE);
                node_data *start = obj->node_start;
                GV->node_recalculate(start);
                if (GV->node_is_useless(start))
                        {
                        GV->all_nodes.removeAll(start);
                        }
                node_data *end = obj->node_end;
                GV->node_recalculate(end);
                if (GV->node_is_useless(end))
                        {
                        GV->all_nodes.removeAll(end);
                        }
                }
        GV->do_selection_rect_recalculate = TRUE;
        GV->do_ratsnest_recalculate = TRUE;
}

void	delete_clean_up::undo()
{
        foreach(myGraphicsItem *obj, deleted_connection)
                {
                GS->add_item(obj);
                obj->set_selected(TRUE);
                node_data *start = obj->node_start;
                if (!GV->all_nodes.contains(start))
                        GV->all_nodes.append(start);
                GV->node_recalculate(start);
                node_data *end = obj->node_end;
                if (!GV->all_nodes.contains(end))
                        GV->all_nodes.append(end);
                GV->node_recalculate(end);
                }
        GV->do_selection_rect_recalculate = TRUE;
        GV->do_ratsnest_recalculate = TRUE;
}

disconnect_pin::disconnect_pin(myGraphicsItem *dis_pin)
{
        //Subito dopo la creazione di questa classe viene eseguito redo()
        pin = dis_pin;

        new_node = new node_data();
        new_node->point = pin->node_start->point;
        new_node->connected_pin_or_pad_or_via = 0;
        new_node->net_id = 0;
        new_node->drag_or_move_active = FALSE;
        foreach(int obj, pin->node_start->connected_wires)
                {
                myGraphicsItem *wire = (myGraphicsItem *) obj;
                new_node->connected_wires.append((int) wire);
                }
}

void	disconnect_pin::redo()
{
        GV->all_nodes.append(new_node);
        foreach(int obj, new_node->connected_wires)
                {
                myGraphicsItem *wire = (myGraphicsItem *) obj;
                if (wire->node_start == pin->node_start)
                        wire->node_start = new_node;
                if (wire->node_end == pin->node_start)
                        wire->node_end = new_node;
                pin->node_start->connected_wires.removeAll((int) wire);
                }
}

void	disconnect_pin::undo()
{
        GV->all_nodes.removeAll(new_node);
        foreach(int obj, new_node->connected_wires)
                {
                myGraphicsItem *wire = (myGraphicsItem *) obj;
                if (wire->node_start == new_node)
                        wire->node_start = pin->node_start;
                if (wire->node_end == new_node)
                        wire->node_end = pin->node_start;
                pin->node_start->connected_wires.append((int) wire);
                }
}

move_obj::move_obj(QPoint mov_delta, myGraphicsItem *mov_obj)
{
	obj = mov_obj;
	delta = mov_delta;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	move_obj::redo() 
{
	obj->start += delta;
	obj->end += delta;
	for (int i = 0; i < obj->polygon.count(); i++)
		obj->polygon[i] += delta;
	obj->set_selected(TRUE);
	obj->info_update();
	myComponent *comp = (myComponent *) obj->comp;
	if (comp != 0)
		if (!GV->selected_comp.contains(comp))
			{
			comp->is_selected = TRUE;
			GV->selected_comp.append(comp);
			}

	GV->do_selection_rect_recalculate = TRUE;
}

void	move_obj::undo() 
{
	delta = -delta;
	redo();
	delta = -delta;
}

move_comp::move_comp(QPoint mov_delta, myComponent *mov_comp)
{
	comp = mov_comp;
	delta = mov_delta;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	move_comp::redo() 
{
	comp->anchor_point += delta;
	foreach (myGraphicsItem *obj, comp->all_obj)
		{
		obj->start += delta;
		obj->end += delta;
		for (int i = 0; i < obj->polygon.count(); i++)
			obj->polygon[i] += delta;
		obj->set_selected(TRUE);
		obj->info_update();
                }
	comp->set_selected(TRUE);
	GV->do_selection_rect_recalculate = TRUE;
}

void	move_comp::undo() 
{
	delta = -delta;
	redo();
	delta = -delta;
}

connection_move_node::connection_move_node(QPoint mov_delta, node_data *mov_node)
{
	node = mov_node;
	delta = mov_delta;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	connection_move_node::redo()
{
	node->point += delta;
	if (node->connected_pin_or_pad_or_via)
		{
		myGraphicsItem *obj =  (myGraphicsItem *) node->connected_pin_or_pad_or_via;
		obj->info_update();
		}
	foreach (int item, node->connected_wires)
		{
		myGraphicsItem *obj = (myGraphicsItem *) item;
		obj->info_update();
		}
	GV->do_selection_rect_recalculate = TRUE;
}

void	connection_move_node::undo()
{
        delta = -delta;
        redo();
        delta = -delta;
}

pol_move_node::pol_move_node(QPoint mov_delta, myGraphicsItem *mov_pol, int mov_index)
{
        pol = mov_pol;
        index = mov_index;
        delta = mov_delta;
        //Subito dopo la creazione di questa classe viene eseguito redo()
}

void	pol_move_node::redo()
{
        pol->polygon.setPoint(index, pol->polygon.at(index) + delta);
        pol->info_update();
}

void	pol_move_node::undo()
{
        delta = -delta;
        redo();
        delta = -delta;
}

pol_del_node::pol_del_node(myGraphicsItem *del_pol, int del_index)
{
        pol = del_pol;
        index = del_index;
        point = pol->polygon.at(index);
        //Subito dopo la creazione di questa classe viene eseguito redo()
}

void	pol_del_node::redo()
{
        pol->polygon.remove(index);
        pol->info_update();
}

void	pol_del_node::undo()
{
        pol->polygon.insert(index, point);
        pol->info_update();
}

pol_insert_node::pol_insert_node(myGraphicsItem *ins_pol, int ins_index, QPoint ins_point)
{
        pol = ins_pol;
        index = ins_index;
        point = ins_point;
        //Subito dopo la creazione di questa classe viene eseguito redo()
}

void	pol_insert_node::redo()
{
        pol->polygon.insert(index, point);
        pol->info_update();
}

void	pol_insert_node::undo()
{
        pol->polygon.remove(index);
        pol->info_update();
}

connection_insert_node::connection_insert_node(myGraphicsItem *ins_con, node_data *ins_node)
{
        con = ins_con;
        node = ins_node;

        con1 = new myGraphicsItem(O_CONNECTION);
        con1->layer_from = con->layer_from;
        con1->layer_to = con->layer_to;
        con1->node_start = con->node_start;
        con1->node_end = node;
        con1->pen_width = con->pen_width;
        node->connected_wires.append((int) con1);
        con1->info_update();

        con2 = new myGraphicsItem(O_CONNECTION);
        con2->layer_from = con->layer_from;
        con2->layer_to = con->layer_to;
        con2->node_start = node;
        con2->node_end = con->node_end;
        con2->pen_width = con->pen_width;
        node->connected_wires.append((int) con2);
        con2->info_update();
        //Subito dopo la creazione di questa classe viene eseguito redo()
}

void	connection_insert_node::redo()
{
        GS->add_item(con1);
        con1->set_selected(FALSE);
        GS->add_item(con2);
        con2->set_selected(FALSE);
        GV->all_nodes.append(node);
        GS->remove_item(con);
        con1->node_start->connected_wires.removeAll((int) con);
        con1->node_start->connected_wires.append((int) con1);
        con2->node_end->connected_wires.removeAll((int) con);
        con2->node_end->connected_wires.append((int) con2);

        GV->wand_connection = 0;
        GV->do_selection_rect_recalculate = TRUE;
}

void	connection_insert_node::undo()
{
        GS->add_item(con);
        con->set_selected(FALSE);
        GS->remove_item(con1);
        GS->remove_item(con2);
        GV->all_nodes.removeAll(node);
        con1->node_start->connected_wires.append((int) con);
        con1->node_start->connected_wires.removeAll((int) con1);
        con2->node_end->connected_wires.append((int) con);
        con2->node_end->connected_wires.removeAll((int) con2);

        GV->wand_connection = 0;
        GV->do_selection_rect_recalculate = TRUE;
}

change_obj_prop::change_obj_prop(myGraphicsItem *sel_obj, int dialogo)
{
        win_object_prop	*d = (win_object_prop	*) dialogo;
	
        obj = sel_obj;

        pen_width = d->new_pen_width;
	fill_polygon = d->new_fill_polygon;
	text = d->new_text;
	font_height = d->new_font_height;
	font_bold = d->new_font_bold;
	font_italic = d->new_font_italic;
	text_position = d->new_text_position;
        number = d->new_number;
        shape = d->new_shape;
        width = d->new_width;
        height = d->new_height;
        drill = d->new_drill;
        layer_from = d->new_layer_from;
        layer_to = d->new_layer_to;
        mirror_text_on = d->new_mirror_text_on;
        //Subito dopo la creazione di questa classe viene eseguito redo()
}

void	change_obj_prop::redo() 
{
	swap_qreal(obj->pen_width, pen_width);
	swap_bool(obj->fill_polygon, fill_polygon);
	swap_qstring(obj->text, text);
	swap_char(obj->text_position, text_position);
	swap_char(obj->font_height, font_height);
	swap_bool(obj->font_bold, font_bold);
	swap_bool(obj->font_italic, font_italic);
        swap_int(obj->number, number);
        swap_char(obj->shape, shape);
        swap_int(obj->width, width);
        swap_int(obj->height, height);
        swap_int(obj->drill, drill);
        swap_char(obj->layer_from, layer_from);
        swap_char(obj->layer_to, layer_to);
        swap_bool(obj->mirror_text_on, mirror_text_on);
        obj->set_selected(TRUE);
	obj->info_update();
	GV->do_selection_rect_recalculate = TRUE;
	GV->do_ratsnest_recalculate = TRUE;
}

void	change_obj_prop::undo() 
{
	redo();
}

change_pad_net_id::change_pad_net_id(int change_net_id, node_data *change_node)
{
	node = change_node;
	net_id = change_net_id;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	change_pad_net_id::undo()
{
	swap_int(node->net_id, net_id);
}

void	change_pad_net_id::redo()
{
	swap_int(node->net_id, net_id);
}

change_component_ident_and_value::change_component_ident_and_value(myComponent *sel_comp, QString new_ident, QString new_value)
{
        comp = sel_comp;
        ident = new_ident;
        value = new_value;
        //Subito dopo la creazione di questa classe viene eseguito redo()
}

void    change_component_ident_and_value::redo()
{
        swap_qstring(comp->ident, ident);
        swap_qstring(comp->value, value);
        foreach(myGraphicsItem *obj, comp->all_obj)
                if ((obj->tipo == O_IDENT) || (obj->tipo == O_VALUE))
                        obj->info_update();
}

void    change_component_ident_and_value::undo()
{
        redo();
}

change_component_prop::change_component_prop(int dialogo)
{
        win_component_prop	*d = (win_component_prop	*) dialogo;
	
	setText(QObject::tr("Change Component Properties"));	
	comp = d->comp;
	alias = d->new_alias;
        ident = d->new_ident;
	value = d->new_value;
	pin_number_is_visible = d->new_pin_number_is_visible;
	pin_name_is_visible = d->new_pin_name_is_visible;
	up = d->new_up;
        devices = d->new_devices;
	footprint_link = d->new_footprint_link;
        //Subito dopo la creazione di questa classe viene eseguito redo()
}

void	change_component_prop::redo() 
{
	swap_qstring(comp->alias, alias);
        swap_qstring(comp->value, value);
        if (file_type == FT_PROJECT)
                {
                swap_qstring(comp->ident, ident);
                }
        else
                comp->ident = comp->alias + "***";
	swap_bool(comp->pin_number_is_visible, pin_number_is_visible);
	swap_bool(comp->pin_name_is_visible, pin_name_is_visible);
	if (file_type == FT_PROJECT)
		foreach(myGraphicsItem *obj, comp->all_obj)
			obj->info_update();
	else
                foreach(myGraphicsItem *obj, GS->all_obj)
			obj->info_update();
        swap_qstring(comp->up,up);
        swap_qstring(comp->devices,devices);
	swap_qstring(footprint_link, comp->footprint_link);
	comp->set_selected(TRUE);
	comp->info_update();		
	GV->do_selection_rect_recalculate = TRUE;
}

void	change_component_prop::undo() 
{
	redo();
}

change_page_and_labelling::change_page_and_labelling(int dialogo)
{
        win_page	*d = (win_page *) dialogo;

        setText(QObject::tr("Change Page and Labelling Properties"));
        schematic = (int) GV;
        page_width = d->page->size_Width->length;
        page_height = d->page->size_Height->length;
        organization = d->organization->text();
        title = d->title->text();
        doc_num = d->doc_num->text();
        rev = d->rev->text();
        //Subito dopo la creazione di questa classe viene eseguito redo()*/
}

void	change_page_and_labelling::redo()
{
        myGraphicsView *sch = (myGraphicsView *) schematic;

        swap_int(page_width, sch->page_width);
        swap_int(page_height, sch->page_height);
        swap_qstring(organization, sch->frame_organization->text);
        sch->frame_organization->info_update();
        swap_qstring(title, sch->frame_title->text);
        sch->frame_title->info_update();
        swap_qstring(doc_num, sch->frame_doc_num->text);
        sch->frame_doc_num->info_update();
        swap_qstring(rev, sch->frame_rev->text);
        sch->frame_rev->info_update();
}

void	change_page_and_labelling::undo()
{
        redo();
}

change_wire_width::change_wire_width(myGraphicsItem *sel_obj, qreal new_pen_width)
{
        //Più efficiente di change_obj_prop() ed ha il vantaggio di modificare solo lo spessore senza cambiarmi il layer, cosa pericolosa se ho selezionate piste di diversi layer
        obj = sel_obj;
        pen_width = new_pen_width;
        //Subito dopo la creazione di questa classe viene eseguito redo()
}

void	change_wire_width::redo()
{
        swap_qreal(pen_width, obj->pen_width);
        obj->set_selected(TRUE);
        obj->info_update();
        GV->do_selection_rect_recalculate = TRUE;
        GV->do_ratsnest_recalculate = TRUE;
}

void	change_wire_width::undo()
{
        redo();
}

select_obj::select_obj(myGraphicsItem *sel_obj)
{
	obj = sel_obj;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

void	select_obj::redo() 
{
        obj->set_selected(TRUE);
	GV->do_selection_nodes = TRUE;
}

void	select_obj::undo() 
{
	obj->set_selected(TRUE);
	GV->do_selection_nodes = TRUE;
}

split_node::split_node(node_data *node)
{
	node_mov = node;
	node_fix = new node_data;
	node_fix->point = node_mov->point;
	node_fix->connected_pin_or_pad_or_via = 0; //Importante

	foreach(int obj, node_mov->connected_wires)
		{
		myGraphicsItem *wire = (myGraphicsItem *) obj;
		if (!wire->drag_or_move_active)
				fix_wires.append(obj);
                }

	if (node_mov->connected_pin_or_pad_or_via)
		{
		if (((myGraphicsItem *) node_mov->connected_pin_or_pad_or_via)->drag_or_move_active)
			fix_pin_or_pad = FALSE;
		else
			fix_pin_or_pad = TRUE;
		}
	else
		fix_pin_or_pad = FALSE;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}
	
void	split_node::redo()
{	
	foreach(int obj, fix_wires)
		{
		node_mov->connected_wires.removeAll(obj);
		node_fix->connected_wires.append(obj);
		myGraphicsItem *wire = (myGraphicsItem *) obj;
		if (wire->node_start == node_mov)
			wire->node_start = node_fix;
		if (wire->node_end == node_mov)
			wire->node_end = node_fix;
		}
	
	if (fix_pin_or_pad)
		{
		node_fix->connected_pin_or_pad_or_via = node_mov->connected_pin_or_pad_or_via;
		node_mov->connected_pin_or_pad_or_via = 0;
		((myGraphicsItem *) node_fix->connected_pin_or_pad_or_via)->node_start = node_fix;
		}
		
	GV->all_nodes.append(node_fix);	
	GV->do_selection_rect_recalculate = TRUE;
}

void	split_node::undo()
{
	foreach(int obj, fix_wires)
		{
		node_fix->connected_wires.removeAll(obj);
		node_mov->connected_wires.append(obj);
		myGraphicsItem *wire = (myGraphicsItem *) obj;
		if (wire->node_start == node_fix)
			wire->node_start = node_mov;
		if (wire->node_end == node_fix)
			wire->node_end = node_mov;
		}
	
	if (fix_pin_or_pad)
		{
		node_mov->connected_pin_or_pad_or_via = node_fix->connected_pin_or_pad_or_via;
		node_fix->connected_pin_or_pad_or_via = 0;
		((myGraphicsItem *) node_mov->connected_pin_or_pad_or_via)->node_start = node_mov;
		}

	GV->all_nodes.removeAll(node_fix);	
	GV->do_selection_rect_recalculate = TRUE;
}

join_node::join_node(node_data *nd1, node_data *nd2)
{
        if (nd1->connected_pin_or_pad_or_via)
                {
                node_fixed = nd1;
                node_removed = nd2;
                }
        else
                {
                node_fixed = nd2;
                node_removed = nd1;
                }

        null_wire = 0;
        foreach(int obj, node_removed->connected_wires)
                {
                myGraphicsItem *wire = (myGraphicsItem *) obj;
                if ((wire->node_start == node_fixed) && (wire->node_end == node_removed))
                        null_wire = wire;   //Di wire_null ce ne è sempre e solo uno
                if ((wire->node_start == node_removed) && (wire->node_end == node_fixed))
                        null_wire = wire;   //Di wire_null ce ne è sempre e solo uno
                }
}

void	join_node::redo()
{
        if (null_wire)
                {
                GS->remove_item(null_wire);
                node_fixed->connected_wires.removeAll((int) null_wire);
                node_removed->connected_wires.removeAll((int) null_wire);
                }

        //Sposto tutti i wires del node_removed sul node_fixed
        foreach(int obj, node_removed->connected_wires)
                {
                myGraphicsItem *wire = (myGraphicsItem *) obj;
                node_fixed->connected_wires.append(obj);
                if (wire->node_start == node_removed)
                        wire->node_start = node_fixed;
                //Sotto bisogna usare else if altrimenti i null_wire sono gestiti malamente
                else if (wire->node_end == node_removed)
                        wire->node_end = node_fixed;
                wire->info_update();
                }

        GV->all_nodes.removeAll(node_removed);
	GV->do_selection_rect_recalculate = TRUE;
}

void	join_node::undo()
{
        GV->all_nodes.append(node_removed);

        foreach(int obj, node_removed->connected_wires)
                {
                myGraphicsItem *wire = (myGraphicsItem *) obj;
                node_fixed->connected_wires.removeAll(obj);
                if (wire->node_start == node_fixed)
                        wire->node_start = node_removed;
                //Sotto bisogna usare else if altrimenti i null_wire sono gestiti malamente
                else if (wire->node_end == node_fixed)
                        wire->node_end = node_removed;
                wire->info_update();
                }

        if (null_wire)
                {
                GS->add_item(null_wire);
                node_fixed->connected_wires.append((int) null_wire);
                node_removed->connected_wires.append((int) null_wire);
                null_wire->info_update();
                }

        GV->do_selection_rect_recalculate = TRUE;
}

change_footprint_side::change_footprint_side(myComponent *comp) 
{
	pcb_comp = comp;
	//Subito dopo la creazione di questa classe viene eseguito redo()
}

int		change_footprint_side::specular_layer(int layer)
{
	switch (layer)
	{
	case L_PCBPasteTop:
		return L_PCBPasteBottom;
	case L_PCBSilkTop:
		return L_PCBSilkBottom;
	case L_PCBMaskTop:
		return L_PCBMaskBottom;
        case L_PCBCopper1:
                return L_PCBCopper16;
        case L_PCBCopper16:
                return L_PCBCopper1;
	case L_PCBMaskBottom:
		return L_PCBMaskTop;
	case L_PCBSilkBottom:
		return L_PCBSilkTop;
	case L_PCBPasteBottom:
                return L_PCBPasteTop;
	}
}

void	change_footprint_side::redo() 
{
        foreach(myGraphicsItem *obj, pcb_comp->all_obj)
                    {
                    GS->remove_item(obj);
                    switch (obj->tipo)
                    {
                    case O_PAD:
                    case O_VIA:
                            if (obj->layer_from == obj->layer_to)
                                    {
                                    //Il pad/via è di tipo smd (superficiale)
                                    obj->layer_from = specular_layer(obj->layer_from);
                                    obj->layer_to = specular_layer(obj->layer_to);
                                    }
                            break;
                    default:
                            obj->layer_from = specular_layer(obj->layer_from);
                            obj->layer_to = specular_layer(obj->layer_to);
                            break;
                    }
                    obj->mirror(pcb_comp->anchor_point, TRUE);
                    if (obj->node_start)
                            obj->node_start->point = mirror_point(obj->node_start->point, pcb_comp->anchor_point, TRUE);
                    if (obj->node_end)
                            obj->node_end->point = mirror_point(obj->node_end->point, pcb_comp->anchor_point, TRUE);
                    obj->info_update();
                    GS->add_item(obj);
                    }
            pcb_comp->info_update();
}

void	change_footprint_side::undo() 
{
	redo();
}
