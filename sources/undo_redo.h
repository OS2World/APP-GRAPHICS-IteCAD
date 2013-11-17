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

#ifndef UNDO_REDO_H
#define UNDO_REDO_H

#include <QMainWindow>
#include <QUndoCommand>
#include "myGraphicsView.h"
#include "win_main.h"
#include "win_object_prop.h"

class add_node : public QUndoCommand
{
public:
    add_node(node_data *add_node);
	virtual	void undo();
	virtual void redo();

	node_data *node;
};

class add_object : public QUndoCommand
{
public:
    add_object(myGraphicsItem *add_obj);
	virtual	void undo();
	virtual void redo();

	myGraphicsItem *obj;
};

class add_component : public QUndoCommand
{
public:
    add_component(myComponent *add_comp);
        virtual	void undo();
        virtual void redo();

        myComponent *comp;
};

class add_footprint : public QUndoCommand
{
public:
    add_footprint(myComponent *footprint);
	virtual	void undo();
	virtual void redo();

	myComponent *pcb_comp;
	myComponent *sch_comp;
	QList<node_data *>	pcb_nodes;
};

class clipboard_clear : public QUndoCommand
{
public:
    clipboard_clear();
	virtual	void undo();
	virtual void redo();
};

class delete_selection : public QUndoCommand
{
public:
    delete_selection();
	virtual	void undo();
	virtual void redo();

	QList<myGraphicsItem *> deleted_obj;
        QList<myComponent *>    deleted_comp;
	QList<node_data *>	deleted_nodes;
};

class delete_clean_up : public QUndoCommand
{
public:
    delete_clean_up();
	virtual	void undo();
	virtual void redo();

	QList<myGraphicsItem *> deleted_connection;
};

class disconnect_pin : public QUndoCommand
{
public:
    disconnect_pin(myGraphicsItem *dis_pin);
        virtual	void undo();
        virtual void redo();

        myGraphicsItem *pin;
        node_data *new_node;
};

class move_obj : public QUndoCommand
{
public:
    move_obj(QPoint mov_delta, myGraphicsItem *mov_obj);
	
	virtual	void undo();
	virtual void redo();

	myGraphicsItem	*obj;
	QPoint			delta;
};

class move_comp : public QUndoCommand
{
public:
    move_comp(QPoint mov_delta, myComponent *mov_comp);
	
	virtual	void undo();
	virtual void redo();

	myComponent		*comp;
	QPoint			delta;
};

class rotate_node : public QUndoCommand
{
public:
    rotate_node(QPoint rot_center, node_data *rot_node, bool rot_clockwise);
	
	virtual	void undo();
	virtual void redo();

	node_data		*node;
	QPoint			center;
	bool			clockwise;
};

class rotate_obj : public QUndoCommand
{
public:
    rotate_obj(QPoint rot_center, myGraphicsItem *rot_obj, bool rot_clockwise);
	
	virtual	void undo();
	virtual void redo();

	myGraphicsItem	*obj;
	QPoint			center;
	bool			clockwise;
};

class rotate_comp : public QUndoCommand
{
public:
    rotate_comp(QPoint rot_center, myComponent *rot_comp, bool rot_clockwise);
	
	virtual	void undo();
	virtual void redo();

	myComponent		*comp;
	QPoint			center;
	bool			clockwise;
};

class mirror_node : public QUndoCommand
{
public:
    mirror_node(QPoint mir_center, node_data *mir_node, bool mirror_horizontal);
	
	virtual	void undo();
	virtual void redo();

	node_data		*node;
	QPoint			center;
	bool			horizontal;
};

class mirror_obj : public QUndoCommand
{
public:
    mirror_obj(QPoint mir_center, myGraphicsItem *mir_obj, bool mirror_horizontal);
	
	virtual	void undo();
	virtual void redo();

	myGraphicsItem	*obj;
	QPoint			center;
	bool			horizontal;
};

class mirror_comp : public QUndoCommand
{
public:
    mirror_comp(QPoint mir_center, myComponent *mir_comp, bool mir_horizontal);
	
	virtual	void undo();
	virtual void redo();

	myComponent		*comp;
	QPoint			center;
	bool			horizontal;
};

class change_pad_net_id : public QUndoCommand
{
public:
    change_pad_net_id(int	change_pad_id, node_data *change_node);
	
	virtual	void undo();
	virtual void redo();

	node_data	*node;
	int			net_id;
};

class connection_move_node : public QUndoCommand
{
public:
    connection_move_node(QPoint mov_delta, node_data *mov_node);
	
	virtual	void undo();
	virtual void redo();

	node_data		*node;
	QPoint			delta;
};

class pol_move_node : public QUndoCommand
{
public:
    pol_move_node(QPoint mov_delta, myGraphicsItem *mov_pol, int mov_index);

        virtual	void undo();
        virtual void redo();

        myGraphicsItem		*pol;
        int                     index;
        QPoint			delta;
};

class pol_del_node : public QUndoCommand
{
public:
    pol_del_node(myGraphicsItem *del_pol, int del_index);

        virtual	void undo();
        virtual void redo();

        myGraphicsItem		*pol;
        int                     index;
        QPoint			point;
};

class pol_insert_node : public QUndoCommand
{
public:
    pol_insert_node(myGraphicsItem *ins_pol, int ins_index, QPoint ins_point);

        virtual	void undo();
        virtual void redo();

        myGraphicsItem		*pol;
        int                     index;
        QPoint			point;
};

class connection_insert_node : public QUndoCommand
{
public:
    connection_insert_node(myGraphicsItem *ins_con, node_data *ins_node);

        virtual	void undo();
        virtual void redo();

        myGraphicsItem		*con;
        myGraphicsItem		*con1;
        myGraphicsItem		*con2;
        node_data               *node;
};

class change_obj_prop : public QUndoCommand
{
public:
    change_obj_prop(myGraphicsItem *sel_obj, int dialogo);
	
	virtual	void undo();
	virtual void redo();

	myGraphicsItem *obj;

	qreal	pen_width;	
	bool	fill_polygon;
	QString text;
        char	font_height;
	bool	font_bold;
	bool	font_italic;
	char	text_position;
        char	shape;
        int	width;
        int	height;
        int	drill;
        unsigned char layer_from;
        unsigned char layer_to;
        int	number;
        bool    mirror_text_on;
};

class change_component_ident_and_value : public QUndoCommand
{
public:
    change_component_ident_and_value(myComponent *sel_comp, QString new_ident, QString new_value);
	
	virtual	void undo();
	virtual void redo();

	myComponent		*comp;

        QString	ident;
        QString value;
};

class change_component_prop : public QUndoCommand
{
public:
    change_component_prop(int dialogo);

        virtual	void undo();
        virtual void redo();

        myComponent		*comp;

        QString alias;
        QString	ident;
        QString	value;
        bool	pin_number_is_visible;
        bool	pin_name_is_visible;
        QString	up;
        QString	devices;
        QString	footprint_link;
};

class change_wire_width : public QUndoCommand
{
public:
    change_wire_width(myGraphicsItem *sel_obj, qreal new_pen_width);

        virtual	void undo();
        virtual void redo();

        myGraphicsItem *obj;

        qreal	pen_width;
};

class change_page_and_labelling : public QUndoCommand
{
public:
    change_page_and_labelling(int dialogo);

        virtual	void undo();
        virtual void redo();

        int	schematic;

        int     page_width;
        int     page_height;
        QString organization;
        QString	title;
        QString	doc_num;
        QString	rev;
};

class select_obj : public QUndoCommand
{
public:
    select_obj(myGraphicsItem *sel_obj);
	
	virtual	void undo();
	virtual void redo();

	myGraphicsItem		*obj;
};

class split_node : public QUndoCommand
{
public:
    split_node(node_data *node);
	
	virtual	void undo();
	virtual void redo();

	node_data		*node_fix;	
	node_data		*node_mov;
	QList<int>		fix_wires;
	bool			fix_pin_or_pad;
};

class join_node : public QUndoCommand
{
public:
    join_node(node_data *nd1, node_data *nd2);
	
	virtual	void undo();
	virtual void redo();

        node_data		*node_fixed;
        node_data		*node_removed;

        myGraphicsItem          *null_wire;
};

class change_footprint_side : public QUndoCommand
{
public:
    change_footprint_side(myComponent *comp);

	int		specular_layer(int layer);
	QPoint	horizonatl_mirror (QPoint p);
	
	virtual	void undo();
	virtual void redo();

	myComponent *pcb_comp;
};

#endif
