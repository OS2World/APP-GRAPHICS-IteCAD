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

#include "myGraphicsView.h"
#include "math.h"

extern	QApplication            *application;
extern	win_main		*w_main;
extern	lay_data		layer_data[LAYER_DATA_COUNT];
extern	lay_data		workspace_data[WORKSPACE_DATA_COUNT];

extern	QPen			pen_layer;
extern	QBrush                  brush_layer;

extern	char	file_type;	
extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

extern  bool    y_axis_is_up;

extern	unsigned char    UM;
extern	QFont	font_pin;

extern	QPoint	drag_or_move_delta;
extern	bool	mode_drag_active;

extern  bool    edit_node_in_pcb_is_all_layers;

extern	QColor		sel_color;
extern	QColor		sel_color_inverted;
extern	QColor		sel_color_transparent;

extern	QVector<ratsnest_struct>	ratsnest;
extern	QPoint						dynamic_ratsnest_endpoint;
extern	bool						dynamic_ratsnest_active;

extern	bool		highlight_active;

extern	int			mouse_click_num;
extern	QPoint			mouse_new_press;
extern	node_data		*mouse_new_press_node;
extern	QPoint			mouse_old_press;
extern	node_data		*mouse_old_press_node;
extern	QPoint			mouse_release;
extern	node_data		*mouse_release_node;
extern	QPoint			mouse_move;
extern	node_data		*mouse_move_node;
extern	QPoint			mouse_right_button;
extern  QPoint                  mouse_position;
extern  char                    mouse_direction;

extern	stampa_formato stampa_formati[];

extern  QList<myGraphicsItem *>     cloned_obj;
extern  QList<myComponent *>        cloned_comp;
extern  QList<node_data *>          cloned_nodes;
extern  char                        cloned_action;
extern  QList<myGraphicsItem *>     clipboard_obj;
extern  QList<myComponent *>        clipboard_comp;
extern  QList<node_data *>          clipboard_nodes;

extern	int	idstamp_last_used;

extern	bool	drc_active;
extern	bool	drc_check_low_insulation;
extern	int	drc_v2v;
extern	int	drc_v2l;
extern	int	drc_l2l;
extern	int	check_v2v;
extern	int	check_v2l;
extern	int	check_l2l;

extern  bool	style_sch_fill_polygon;
extern  char	style_sch_font_height;
extern  bool	style_sch_font_bold;
extern  bool	style_sch_font_italic;
extern  int	style_sch_pen_width_wire;
extern  int	style_sch_pen_width_normal;
extern  int	style_sch_pen_width_frame;
extern  bool	style_pcb_fill_polygon;
extern  char	style_pcb_font_height;
extern  bool	style_pcb_font_bold;
extern  bool	style_pcb_font_italic;
extern  int	style_pcb_pen_width_copper;
extern  int	style_pcb_pen_width_silk;
extern  int	style_pcb_pen_width_paste;
extern  int	style_pcb_pen_width_mask;
extern  int	style_pcb_pen_width_board;
extern  int	style_pcb_pen_width_help;
extern  char	style_pcb_pad_shape;
extern  int	style_pcb_pad_width;
extern  int	style_pcb_pad_height;
extern  unsigned char style_pcb_pad_layer_from;
extern  unsigned char style_pcb_pad_layer_to;
extern  int	style_pcb_pad_drill;
extern  int     style_pcb_via_width;
extern  int     style_pcb_via_layer_from;
extern  int     style_pcb_via_layer_to;
extern  int     style_pcb_via_drill;

extern  qreal	sch_grid_artistic;

extern  bool	pcb_grid_visible;
extern  qreal	pcb_grid_size;
extern  bool    pcb_grid_thou;
extern  char	pcb_grid_snap;

extern  bool    pcb_pad_via_filled;

extern  QPoint  ruler_start;

myGraphicsView::myGraphicsView(QGraphicsScene * scene_input, QWidget *parent, bool is_sch_input, QString name_input)
        : QGraphicsView(scene_input, parent)
{
	scene = (myGraphicsScene *) scene_input;
	
        if (w_main)
                {
                connect(&ur, SIGNAL(canUndoChanged(bool)), w_main, SLOT(edit_can_undo_changed(bool)));
                connect(&ur, SIGNAL(canRedoChanged(bool)), w_main, SLOT(edit_can_redo_changed(bool)));
                }

        midbutton_pan_on = FALSE;

	setRenderHint(QPainter::Antialiasing, FALSE);
	setRenderHint(QPainter::HighQualityAntialiasing, FALSE);
	setRenderHint(QPainter::TextAntialiasing, FALSE);
	setRenderHint(QPainter::SmoothPixmapTransform, FALSE);
	setSceneRect(-2000000, -2000000, 4000000, 4000000);		//La masssima area su cui posso lavorare è un quadrato di poco superiore a 1mt x 1mt

        cursor_setup_Z();

	name = name_input;
	is_sch = is_sch_input;

	if (is_sch)
                layer_selected = L_SCHNormal;
	else
                layer_selected = L_PCBCopper1;
	
	is_pixmapoutput = FALSE;

        // Carico i factory default
        //** Di default addesso la pagina è landscape
        page_width = 1169289;
        page_height = 826770;
	if (!is_sch_input)
		{
		//Carico i factory default
		drc_v2v = 5000;	//0.05" di default
		drc_v2l = 5000;	//0.05" di default
		drc_l2l = 5000;	//0.05" di default
		check_v2v = drc_v2v;
		check_v2l = drc_v2l;
		check_l2l = drc_l2l;
		drc_active = TRUE;
		drc_check_low_insulation = TRUE;
		}

	if (file_type != FT_PROJECT)
		{
		schcomp_or_pcbcomp = new myComponent();
                schcomp_or_pcbcomp->alias = "U";
                schcomp_or_pcbcomp->ident = "U***";
                schcomp_or_pcbcomp->value = "Value";
		}
        for (int i = 0; i < LAYER_DATA_COUNT; i++)
		{
		layer_settings[i].visible = TRUE;
		layer_settings[i].print = TRUE;
		}

	drag_or_move_delta = QPoint(0, 0);

	dynamic_ratsnest_active = FALSE;

	do_ratsnest_recalculate = TRUE;

	inserting_item_is_visible_anyway = FALSE;

        origin = QPoint(0,0);

        setMouseTracking(TRUE);

        if (is_sch)
                {
                frame_organization = new myGraphicsItem(O_TEXT);
                frame_organization->layer_from = L_SCHFrame;
                frame_organization->layer_to = L_SCHFrame;
                frame_organization->start = QPoint(0,0);
                frame_organization->font_height = 10;
                frame_organization->font_bold = FALSE;
                frame_organization->font_italic = FALSE;
                frame_organization->text_position = 'R';
                frame_organization->text = tr("Organization");

                frame_title = new myGraphicsItem(O_TEXT);
                frame_title->layer_from = L_SCHFrame;
                frame_title->layer_to = L_SCHFrame;
                frame_title->start = QPoint(0,0);
                frame_title->font_height = 10;
                frame_title->font_bold = FALSE;
                frame_title->font_italic = FALSE;
                frame_title->text_position = 'R';
                frame_title->text = tr("Title");

                frame_doc_num = new myGraphicsItem(O_TEXT);
                frame_doc_num->layer_from = L_SCHFrame;
                frame_doc_num->layer_to = L_SCHFrame;
                frame_doc_num->start = QPoint(0,0);
                frame_doc_num->font_height = 10;
                frame_doc_num->font_bold = FALSE;
                frame_doc_num->font_italic = FALSE;
                frame_doc_num->text_position = 'R';
                frame_doc_num->text = tr("Doc. Num.");

                frame_rev = new myGraphicsItem(O_TEXT);
                frame_rev->layer_from = L_SCHFrame;
                frame_rev->layer_to = L_SCHFrame;
                frame_rev->start = QPoint(0,0);
                frame_rev->font_height = 10;
                frame_rev->font_bold = FALSE;
                frame_rev->font_italic = FALSE;
                frame_rev->text_position = 'R';
                frame_rev->text = tr("Rev.");

                frame_page = new myGraphicsItem(O_TEXT);
                frame_page->layer_from = L_SCHFrame;
                frame_page->layer_to = L_SCHFrame;
                frame_page->start = QPoint(0,0);
                frame_page->font_height = 10;
                frame_page->font_bold = FALSE;
                frame_page->font_italic = FALSE;
                frame_page->text_position = 'R';

                frame_date = new myGraphicsItem(O_TEXT);
                frame_date->layer_from = L_SCHFrame;
                frame_date->layer_to = L_SCHFrame;
                frame_date->start = QPoint(0,0);
                frame_date->font_height = 10;
                frame_date->font_bold = FALSE;
                frame_date->font_italic = FALSE;
                frame_date->text_position = 'R';

                frame_page = new myGraphicsItem(O_TEXT);
                frame_page->layer_from = L_SCHFrame;
                frame_page->layer_to = L_SCHFrame;
                frame_page->start = QPoint(0,0);
                frame_page->font_height = 10;
                frame_page->font_bold = FALSE;
                frame_page->font_italic = FALSE;
                frame_page->text_position = 'R';
                }

        info_label = new QLabel(this);
        info_label->setHidden(TRUE);
        info_label->setMargin(5);
        info_label->setAutoFillBackground(TRUE);
        info_label->setBackgroundRole(QPalette::ToolTipBase);
        info_label->setFrameStyle(QFrame::Box);
}

void	myGraphicsView::clear()
{
	selected_obj.clear();
	selected_comp.clear();
	selected_nodes.clear();
	foreach(myComponent *comp, all_comp)
		delete comp;
	all_comp.clear();
	all_nodes.clear();
}

void    myGraphicsView::check_data_integrity()
{
        // 1 Cerco i nodi inutili e li elimino
        selected_nodes.clear();
        foreach(node_data *node, all_nodes)
                {
                if (node_is_useless(node))
                        selected_nodes.append(node);
                }
        foreach(node_data *node, selected_nodes)
                all_nodes.removeAll(node);
        selected_nodes.clear();
}

void	myGraphicsView::cursor_change()
{
	if (is_sch)
		w_main->tool_sch = tool;
	else
		w_main->tool_pcb = tool;
	
	//Scelgo il DragMode
	if (tool == T_HAND)
		setDragMode(QGraphicsView::ScrollHandDrag);
	else
		setDragMode(QGraphicsView::NoDrag);

	//Scelgo il cursore
	switch(tool)
	{
	case T_SELECT:
		viewport()->setCursor(Qt::ArrowCursor);
		break;
	case T_CONNECTION:
	case T_POLYLINE:
	case T_RECT:
	case T_ELLIPSE:
	case T_POLYGON:
        case T_EDIT_NODE:
	case T_ADD:
	case T_TEXT:
	case T_PIN:
        case T_PADVIA:
	case T_PORT:
        case T_ORIGIN:
        case T_RULER:
        case T_ADD_POWER:
        case T_EDIT_COMPONENT:
		viewport()->setCursor(Qt::CrossCursor);
		break;
	case T_HAND:
		{
		// Genero un QMouseEvent perchè:
		// cliccando prima sul botton SELECT e poi sul bottone HAND e poi sul bottone ZOOM il cursore continua a rimanere di tipo HAND
		// quando nei 3 click non si passa col mouse attraverso viewport()
                // QT non aggiorna correttamente il dragMode se non si passa col mouse sopra almeno una volta
		QMouseEvent *event = new QMouseEvent(QEvent::MouseMove, QPoint(100,100), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
		QGraphicsView::mouseMoveEvent(event);
		delete event;
		}
		break;
	case T_ZOOM:
		{
		// Genero un QMouseEvent perchè:
		// cliccando prima sul botton SELECT e poi sul bottone HAND e poi sul bottone ZOOM il cursore continua a rimanere di tipo HAND
		// quando nei 3 click non si passa col mouse attraverso viewport()
                // QT non aggiorna correttamente il dragMode se non si passa col mouse sopra almeno una volta
		QMouseEvent *event = new QMouseEvent(QEvent::MouseMove, QPoint(100,100), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
		QGraphicsView::mouseMoveEvent(event);
		delete event;
		}	
		viewport()->setCursor(cursor_Z);
		break;
	}
}

node_data       *myGraphicsView::clone_node(node_data *node)
{
        node_data *dup_node = node_new();
        dup_node->point = node->point;
        node->help_duplicate = dup_node;	//Metto il riferimento al nodo duplicato
        return dup_node;
}

myGraphicsItem  *myGraphicsView::clone_obj(myGraphicsItem *obj, int comp)
{
        myGraphicsItem *dup_obj = new myGraphicsItem(obj->tipo);
        dup_obj->layer_from = obj->layer_from;
        dup_obj->layer_to = obj->layer_to;
        dup_obj->start = obj->start;
        dup_obj->end = obj->end;
        dup_obj->pen_width = obj->pen_width;
        dup_obj->node_start = obj->node_start;
        dup_obj->number = obj->number;
        if (dup_obj->node_start != 0)
                {
                dup_obj->node_start = dup_obj->node_start->help_duplicate;
                switch (obj->tipo)
                {
                case O_CONNECTION:
                        dup_obj->node_start->connected_wires.append((int) dup_obj);
                        break;
                case O_PIN:
                case O_PAD:
                case O_VIA:
                case O_POWER:
                case O_PORT:
                        dup_obj->node_start->connected_pin_or_pad_or_via = (int) dup_obj;
                        break;
                }
                }
        dup_obj->node_end = obj->node_end;
        if (dup_obj->node_end != 0)
                {
                dup_obj->node_end = dup_obj->node_end->help_duplicate;
                switch (obj->tipo)
                {
                case O_CONNECTION:
                        dup_obj->node_end->connected_wires.append((int) dup_obj);
                        break;
                }
                }
        dup_obj->polygon = obj->polygon;
        dup_obj->fill_polygon = obj->fill_polygon;
        dup_obj->text_position = obj->text_position;
        dup_obj->text = obj->text;
        dup_obj->font_height = obj->font_height;
        dup_obj->font_bold = obj->font_bold;
        dup_obj->font_italic = obj->font_italic;
        dup_obj->mirror_text_on = obj->mirror_text_on;
        dup_obj->comp = comp;
        dup_obj->shape = obj->shape;
        dup_obj->width = obj->width;
        dup_obj->height = obj->height;
        dup_obj->drill = obj->drill;
        dup_obj->info_update();
        return dup_obj;
}

myComponent     *myGraphicsView::clone_comp(myComponent *comp)
{
        myComponent *dup_comp = new myComponent();
        dup_comp->alias = comp->alias;
        dup_comp->ident = comp->ident;
        dup_comp->value = comp->value;
        dup_comp->pin_number_is_visible = comp->pin_number_is_visible;
        dup_comp->pin_name_is_visible = comp->pin_name_is_visible;
        dup_comp->up = comp->up;
        dup_comp->footprint_link = comp->footprint_link;
        dup_comp->info_br = comp->info_br;
        dup_comp->anchor_point = comp->anchor_point;
        foreach(myGraphicsItem *obj, comp->all_obj)
                dup_comp->all_obj.append(clone_obj(obj, (int) dup_comp));
        dup_comp->info_update();
        return dup_comp;
}

void myGraphicsView::clone_from_selection()
{
        cloned_obj.clear();
        cloned_comp.clear();
        cloned_nodes.clear();

	//Clono i nodi selezionati
	foreach(node_data *node, selected_nodes)
                cloned_nodes.append(clone_node(node));
        //Clono gli oggetti selezionati (nessuno di essi appartiene ad un componente)
	foreach(myGraphicsItem *obj, selected_obj)
                cloned_obj.append(clone_obj(obj, 0));

        //Clono i componenti selezionati e gli oggetti ad essi appartenenti
	foreach(myComponent *comp, selected_comp)
                cloned_comp.append(clone_comp(comp));
}

void myGraphicsView::clone_to_selection()
{
        foreach(node_data *node, cloned_nodes)
                ur.push(new add_node(node));
        //Sposto tutti gli oggetti che non appartengono a dei componenti
        foreach(myGraphicsItem *item, cloned_obj)
                {
                if (file_type != FT_PROJECT)
                        switch (item->tipo)
                        {
                        case O_PIN:
                                item->text = QString::number(item->number);
                                break;
                        case O_PAD:
                                item->text = QString::number(item->number);
                                break;
                        }
                item->info_update();
                ur.push(new add_object(item));
                }
        foreach(myComponent *comp, cloned_comp)
                {
                if (GV->is_sch)
                        comp->idstamp = ++idstamp_last_used;
                else
                        {
                        //Per i componenti immessi direttamente nel PCB impost idstamp negativa
                        comp->idstamp = -(++idstamp_last_used);
                        }

                ur.push(new add_component(comp));
                }

        //Seleziono tutti gli oggetti
        GV->select_clear(); //Serve
        foreach(myGraphicsItem *item, cloned_obj)
                item->set_selected(TRUE);
        foreach(myComponent *comp, cloned_comp)
                comp->set_selected(TRUE);
        selection_nodes();
}

void myGraphicsView::clone_move(QPoint delta, bool clear_prev_delta)
{
        static QPoint prev_delta;

        if (clear_prev_delta)
                prev_delta = QPoint(0,0);

        foreach(node_data *node, cloned_nodes)
                node->point += (delta - prev_delta);
        //Muovo gli oggetti selezionati (nessuno di essi appartiene ad un componente)
        foreach(myGraphicsItem *obj, cloned_obj)
                {
                obj->move(delta - prev_delta);
                obj->info_update();
                }
        //Muovo i componenti selezionati e gli oggetti ad essi appartenenti
        foreach(myComponent *comp, cloned_comp)
                {
                comp->move(delta - prev_delta);
                foreach(myGraphicsItem *obj, comp->all_obj)
                        obj->info_update();
                comp->info_update();
                }

        prev_delta = delta;
}

QPoint  myGraphicsView::clone_center()
{
        //Trovo il centro di clone
        QRect cloned_rect = QRect(0,0,0,0);
        foreach(myGraphicsItem *obj, cloned_obj)
                cloned_rect = cloned_rect.unite(obj->boundingRect());
        foreach(myComponent *comp, cloned_comp)
                cloned_rect = cloned_rect.unite(comp->bounding_rect());
        return cloned_rect.center();
}

void    myGraphicsView::clone_to_clipboard()
{
        clipboard_obj = cloned_obj;
        clipboard_comp = cloned_comp;
        clipboard_nodes = cloned_nodes;
}

void    myGraphicsView::clone_from_clipboard()
{
        cloned_obj = clipboard_obj;
        cloned_comp = clipboard_comp;
        cloned_nodes = clipboard_nodes;
}

void    myGraphicsView::clone_ident_pin_pad_recalculate()
{
        if (file_type == FT_PROJECT)
                {
                foreach(myComponent *comp, cloned_comp)
                        comp->ident = "";
                foreach(myComponent *comp, cloned_comp)
                        comp->ident = w_main->get_first_available_ident(comp->alias, TRUE);
                }
        else
                {
                foreach(myGraphicsItem *obj, cloned_obj)
                        {
                        if (obj->tipo == O_PIN)
                                obj->number = 0;
                        if (obj->tipo == O_PAD)
                                obj->number = 0;
                        }
                foreach(myGraphicsItem *obj, cloned_obj)
                        {
                        if (obj->tipo == O_PIN)
                                obj->number = w_main->get_first_available_pin(TRUE);
                        if (obj->tipo == O_PAD)
                                obj->number = w_main->get_first_available_pad(TRUE);
                        }
                }
}

void	myGraphicsView::cursor_setup_Z()
{
	static QBitmap cursor_bitmap(16,16);
	static QBitmap cursor_mask(16,16);

	QPainter paint_bitmap(&cursor_bitmap);
	QPainter paint_mask(&cursor_mask);
	
	int m[16];
	m[0] = (int) &("-----xxxx-------");
	m[1] = (int) &("---xx    xx-----");
	m[2] = (int) &("--x  xxxx  x----");
	m[3] = (int) &("-x xx----xx x---");
	m[4] = (int) &("-x x------x x---");
	m[5] = (int) &("x x--------x x--");
	m[6] = (int) &("x x--------x x--");
	m[7] = (int) &("x x--------x x--");
	m[8] = (int) &("x x--------x x--");
	m[9] = (int) &("-x x------x x---");
	m[10] =(int) &("-x xx----xx x---");
	m[11] =(int) &("--x  xxxx   x---");
	m[12] =(int) &("---xx    xxx x--");
	m[13] =(int) &("-----xxxx---x x-");
	m[14] =(int) &("-------------x x");
	m[15] =(int) &("--------------x-");
	
	for(int i = 0; i < 16; i++)
		{
		const char *k = (const char *) m[i];
		for(int j = 0; j < 16; j++)
		{
			switch(k[j])
			{
			case '-':	//Transparent
				paint_bitmap.setPen(Qt::color0);
				paint_bitmap.drawPoint(j,i);
				paint_mask.setPen(Qt::color0);
				paint_mask.drawPoint(j,i);
				break;
			case 'x':	//Black
				paint_bitmap.setPen(Qt::color1);
				paint_bitmap.drawPoint(j,i);
				paint_mask.setPen(Qt::color1);
				paint_mask.drawPoint(j,i);
				break;
			case ' ':	//White
				paint_bitmap.setPen(Qt::color0);
				paint_bitmap.drawPoint(j,i);
				paint_mask.setPen(Qt::color1);
				paint_mask.drawPoint(j,i);
				break;
			}
		}
	}

	cursor_Z = QCursor(cursor_bitmap, cursor_mask, 8, 8);
}

void	myGraphicsView::drag_or_move_setup(char mode)
{
	//fare drag_or_move_clear()
	//ridefinire drag_or_move_setup()
	
	foreach (node_data *node, all_nodes)
		node->drag_or_move_active = FALSE;
	
	switch(mode)
	{
	case 'D':	//Drag
		foreach (myGraphicsItem *obj, scene->all_obj)
			obj->drag_or_move_active = obj->is_selected;
		foreach (node_data *nd, selected_nodes)
			{
			bool drag;
			if (nd->connected_pin_or_pad_or_via != 0)
				drag = ((myGraphicsItem *) nd->connected_pin_or_pad_or_via)->drag_or_move_active;	
			else
				{
				drag = TRUE;
				foreach (int item, nd->connected_wires)
					{
					myGraphicsItem *wire = (myGraphicsItem *) item;
					if (!wire->is_selected)
						{
						drag = FALSE;
						break;
						}
					}
				}
			nd->drag_or_move_active = drag;
			if (drag)
				foreach (int item, nd->connected_wires)
					{
					myGraphicsItem *wire = (myGraphicsItem *) item;
					wire->drag_or_move_active = TRUE;
					}
			}
		break;
	case 'M':	//Move
	case 'C':	//Copy
		foreach (myGraphicsItem *obj, scene->all_obj)
			obj->drag_or_move_active = obj->is_selected;
		foreach (node_data *node, selected_nodes)
			node->drag_or_move_active = TRUE;
                break;
	case 'R':	//Reset
		drag_or_move_delta = QPoint(0,0);
		foreach (myGraphicsItem *obj, scene->all_obj)
			obj->drag_or_move_active = FALSE;		
		break;
	}
}

void	myGraphicsView::drag_or_move_execute(char mode)
{
        bool drag;
	
	switch(mode)
	{
	case 'D':	//Drag
                ur.beginMacro(tr("Drag"));
		break;
	case 'M':
                if (!w_main->disconnect_selection_executed)
                        ur.beginMacro(tr("Move"));
		break;
	}
	
	foreach (myGraphicsItem *obj, selected_obj)
		switch (obj->tipo)
		{
		case O_CONNECTION:
			if (obj->is_selected)
				ur.push(new select_obj(obj));
			break;
                case O_VIA:
                        foreach (int wire, obj->node_start->connected_wires)
                            ur.push(new select_obj((myGraphicsItem *) wire));
                        break;
		default:
			if (obj->drag_or_move_active)
				ur.push(new move_obj(drag_or_move_delta, obj));
			break;
		}
	foreach (myComponent *comp, selected_comp)
                ur.push(new move_comp(drag_or_move_delta, comp));
        foreach (node_data *nd, selected_nodes)
		{
		if (nd->connected_pin_or_pad_or_via != 0)
			drag = ((myGraphicsItem *) nd->connected_pin_or_pad_or_via)->drag_or_move_active;	
		else
			{
			drag = TRUE;
			foreach (int item, nd->connected_wires)
				{
				myGraphicsItem *wire = (myGraphicsItem *) item;
				if (!wire->is_selected)
					{
					drag = FALSE;
					break;
					}
				}
			}
		if (drag)
                        ur.push(new connection_move_node(drag_or_move_delta, nd));
		}
        if (is_sch)
                w_main->connect_sch_selection();
        ur.endMacro();
}

void myGraphicsView::draw_frame(QPainter *painter, QMatrix *m)
{
        painter->setMatrixEnabled(FALSE);
        int pixel_width = style_sch_pen_width_frame * m->m11();
        pen_layer.setWidth(pixel_width);
        pen_layer.setColor(layer_data[L_SCHFrame].color);
        painter->setPen(pen_layer);
        QPoint top_left = QPoint(-page_width/2 + 39370, -page_height/2 + 39370);
        QPoint bottom_right = QPoint(page_width/2 - 39370,page_height/2 - 39370);
        painter->drawRect(QRect(m->map(top_left),m->map(bottom_right)));

        //Calcolo il numero di pagine e la posizione
        w_main->page_numbers_update();

        //Trovo l'altezza di una riga
        int h_text = frame_organization->font_height * 60 * 20;   //Un carattere è definito da 20 punti (15 punti sopra e 5 sotto)
        int h_row = h_text * 2;
        int h_offset = frame_organization->font_height * 60 * 25;

        //Trovo la larghezza minima dei campi left
        int w_left = 0;
        int w_doc_num = frame_doc_num->boundingRect().width();
        int w_date = frame_date->boundingRect().width();
        if (w_date > w_doc_num)
                w_left = w_date + h_row;
        else
                w_left = w_doc_num + h_row;

        //Trovo la larghezza minima dei campi right
        int w_right = 0;
        int w_rev = frame_rev->boundingRect().width();
        int w_page = frame_page->boundingRect().width();
        if (w_page > w_rev)
                w_right = w_page + h_row;
        else
                w_right = w_rev + h_row;

        //Trovo la larghezza minima
        int w_row = 0;
        int w_organization = frame_organization->boundingRect().width();
        int w_title = frame_title->boundingRect().width();
        if (w_organization > w_title)
                w_row = w_organization + h_row;
        else
                w_row = w_title + h_row;
        if ((w_left + w_right) > w_row)
                w_row = w_left + w_right;

        //Disegno date e page
        QPoint origin = bottom_right - QPoint(w_row, h_row);
        painter->drawLine(m->map(origin), m->map(origin + QPoint(w_row,0)));
        painter->drawLine(m->map(origin), m->map(origin + QPoint(0,h_row)));
        frame_date->info_start_text = origin + QPoint(h_text, h_offset);
        frame_date->paint(painter, m);
        QPoint origin2 = bottom_right - QPoint(w_right, h_row);
        pen_layer.setWidth(pixel_width);
        pen_layer.setColor(layer_data[L_SCHFrame].color);
        painter->setPen(pen_layer);
        painter->drawLine(m->map(origin2), m->map(origin2 + QPoint(0,h_row)));
        frame_page->info_start_text = origin2 + QPoint(h_text, h_offset);
        frame_page->paint(painter, m);

        //Disegno doc_num e rev
        if ((frame_doc_num->text.length() > 0) || (frame_rev->text.length() > 0))
                {
                origin = origin - QPoint(0, h_row);
                pen_layer.setWidth(pixel_width);
                pen_layer.setColor(layer_data[L_SCHFrame].color);
                painter->setPen(pen_layer);
                painter->drawLine(m->map(origin), m->map(origin + QPoint(w_row,0)));
                painter->drawLine(m->map(origin), m->map(origin + QPoint(0,h_row)));
                frame_doc_num->info_start_text = origin + QPoint(h_text, h_offset);
                frame_doc_num->paint(painter, m);
                origin2 = origin2 - QPoint(0, h_row);
                pen_layer.setWidth(pixel_width);
                pen_layer.setColor(layer_data[L_SCHFrame].color);
                painter->setPen(pen_layer);
                painter->drawLine(m->map(origin2), m->map(origin2 + QPoint(0,h_row)));
                frame_rev->info_start_text = origin2 + QPoint(h_text, h_offset);
                frame_rev->paint(painter, m);
                }

        //Disegno title
        if (frame_title->text.length() > 0)
                {
                origin = origin - QPoint(0, h_row);
                pen_layer.setWidth(pixel_width);
                pen_layer.setColor(layer_data[L_SCHFrame].color);
                painter->setPen(pen_layer);
                painter->drawLine(m->map(origin), m->map(origin + QPoint(w_row,0)));
                painter->drawLine(m->map(origin), m->map(origin + QPoint(0,h_row)));
                frame_title->info_start_text = origin + QPoint(h_text, h_offset);
                frame_title->paint(painter, m);
                }

        //Disegno Organization
        if (frame_organization->text.length() > 0)
                {
                origin = origin - QPoint(0, h_row);
                pen_layer.setWidth(pixel_width);
                pen_layer.setColor(layer_data[L_SCHFrame].color);
                painter->setPen(pen_layer);
                painter->drawLine(m->map(origin), m->map(origin + QPoint(w_row,0)));
                painter->drawLine(m->map(origin), m->map(origin + QPoint(0,h_row)));
                frame_organization->info_start_text = origin + QPoint(h_text, h_offset);
                frame_organization->paint(painter, m);
                }

        painter->setMatrixEnabled(TRUE);
}

void myGraphicsView::draw_pad_and_via_gerber_view(QPainter *painter, QMatrix *m, bool for_screen, unsigned char layer)
{
        QColor  color;
        if ((layer >= L_PCBCopper1) && (layer <= L_PCBCopper16))
                color = layer_data[layer].color;
        else
                return;

        //Disegno i PAD e le VIE
        foreach (node_data *nd, all_nodes)
                if (nd->connected_pin_or_pad_or_via)
                        {
                        myGraphicsItem *obj = (myGraphicsItem *) nd->connected_pin_or_pad_or_via;
                        if (!for_screen || !obj->is_selected)
                                if (obj->layer_intersect(layer, layer))
                                        {
                                        if (for_screen && highlight_active && obj->highlight_active)
                                            {
                                            pen_layer.setColor(color.lighter(150));
                                            brush_layer.setColor(color.lighter(150));
                                            }
                                        else
                                            {
                                            pen_layer.setColor(color);
                                            brush_layer.setColor(color);
                                            }
                                        obj->paint(painter, m);
                                        }
                        }
}

void myGraphicsView::draw_pad_and_via_draft_view(QPainter *painter, QMatrix *m, bool for_screen)
{
        QColor color;
        //Disegno i PAD e le VIE
        foreach (node_data *nd, all_nodes)
                if (nd->connected_pin_or_pad_or_via)
                        {
                        myGraphicsItem *obj = (myGraphicsItem *) nd->connected_pin_or_pad_or_via;
                        if (!for_screen || !obj->is_selected)
                                {
                                if (obj->tipo == O_PAD)
                                        {
                                        if (obj->layer_from != obj->layer_to)
                                                color = workspace_data[W_PCBpadthrough].color;
                                        else if (obj->layer_from == L_PCBCopper1)
                                                color = workspace_data[W_PCBpadtop].color;
                                        else if (obj->layer_from == L_PCBCopper16)
                                                color = workspace_data[W_PCBpadbottom].color;
                                        }
                                else
                                        color = workspace_data[W_PCBvia].color;
                                if (for_screen && highlight_active && obj->highlight_active)
                                    {
                                    pen_layer.setColor(color.lighter(150));
                                    brush_layer.setColor(color.lighter(150));
                                    }
                                else
                                    {
                                    pen_layer.setColor(color);
                                    brush_layer.setColor(color);
                                    }
                                obj->paint(painter, m);
                                }
                        }
}

void myGraphicsView::draw_hole(QPainter *painter, QMatrix *m)
{
        QColor color;
        if (pcb_pad_via_filled)
                {
                //Eseguo tutti i fori dei PAD e delle vie
                color = workspace_data[W_PCBbackground].color;
                painter->setPen(Qt::NoPen);
                painter->setBrush(QBrush(color));
                foreach (node_data *nd, all_nodes)
                        if (nd->connected_pin_or_pad_or_via)
                                {
                                myGraphicsItem *obj = (myGraphicsItem *) nd->connected_pin_or_pad_or_via;
                                if (obj->layer_from != obj->layer_to)
                                        {
                                        QPoint	start;
                                        if (obj->drag_or_move_active)
                                                start = m->map(obj->node_start->point + drag_or_move_delta);
                                        else
                                                start = m->map(obj->node_start->point);
                                        int radius = obj->drill * m->m11() / 2;
                                        painter->drawEllipse(start, radius, radius);
                                        }
                                }
                }
}

void myGraphicsView::draw_junction(QPainter *painter, QMatrix *m)
{
        foreach (node_data *node, GV->all_nodes)
            if (!node->drag_or_move_active)
                    {
                    if (node->connected_pin_or_pad_or_via)
                            {
                            int cw = node->connected_wires.count();
                            if (cw >= 2)
                                    {
                                    pen_layer.setWidth(4 * SCH_PEN_NORMAL * m->m11());
                                    painter->setPen(pen_layer);
                                    QPoint s = node->point;
                                    painter->drawPoint(m->map(s));
                                    }
                            }
                    else
                            {
                            int cw = node->connected_wires.count();
                            if (cw == 1)
                                    {
                                    pen_layer.setWidth(0);
                                    painter->setPen(pen_layer);
                                    QPoint s = node->point;
                                    qreal x = m->map(s).x();
                                    qreal y = m->map(s).y();
                                    qreal s500 = 500 * m->m11();
                                    painter->drawLine(x - s500, y - s500, x + s500, y + s500);
                                    painter->drawLine(x + s500, y - s500, x - s500, y + s500);
                                    }
                            else if (cw >= 3)
                                    {
                                    pen_layer.setWidth(4 * SCH_PEN_NORMAL * m->m11());
                                    painter->setPen(pen_layer);
                                    QPoint s = node->point;
                                    painter->drawPoint(m->map(s));
                                    }
                            }
                    }
        //Non ha senso colorare con sel_color le giunzioni
        //Colorandoli poi si avrebbe il brutto effetto in alcuni casi di una linea colorata sel_color sopra un punto che è nero
        //Meglio non perdere tempo con una cosa che oltretutto è molto complicata
}

void myGraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
        //Imposto i colori della selezione (COnviene definirli in drawBackGround anche se non sono utilizzati dentro di essa) In questo modo se cambiuo i colori della selezione essi vengono subito aggiornati senza dover fare un ridisegno inutile
	if (is_sch)
		sel_color = workspace_data[W_SCHselection].color;
	else
		sel_color = workspace_data[W_PCBselection].color;
        sel_color_inverted = QColor(255 - sel_color.red(), 255 - sel_color.green(), 255 - sel_color.blue());
	sel_color_transparent = sel_color;
	sel_color_transparent.setAlpha(128);
	
        QMatrix m = painter->worldMatrix();
	
        if (is_sch)
		{
                //Disegno il foglio bianco
                if (file_type == FT_PROJECT)
                        {
                        if (!is_pixmapoutput)
                                {
                                painter->fillRect(rect, workspace_data[W_SCHbackground].color);
                                painter->setPen(QPen(Qt::NoPen));
                                painter->fillRect(QRect(-page_width/2,-page_height/2,page_width,page_height), workspace_data[W_SCHpage].color);
                                }
                        else
                                painter->fillRect(rect, workspace_data[W_SCHpage].color);
                        }
                else
                        painter->fillRect(rect, workspace_data[W_SCHpage].color);
                }
	else
		painter->fillRect(rect, workspace_data[W_PCBbackground].color);
	
        if (is_pixmapoutput)
                return;

        if (is_sch)
                {
                painter->setPen(QPen(workspace_data[W_SCHgrid].color, 0, Qt::DashLine, Qt::SquareCap, Qt::BevelJoin));//E' molto importante avere lo SquareCap altrimenti mi saltano dei punti
                if (file_type != FT_PROJECT)
                        {
                        painter->drawLine(origin.x(), -2000000, origin.x(), 2000000);
                        painter->drawLine(-2000000, origin.y(), 2000000, origin.y());
                        }
                }
        else
                {
                painter->setPen(QPen(workspace_data[W_PCBgrid].color, 0, Qt::DashLine, Qt::SquareCap, Qt::BevelJoin));//E' molto importante avere lo SquareCap altrimenti mi saltano dei punti
                if ((file_type != FT_PROJECT) || (tool == T_ORIGIN))
                        {
                        painter->drawLine(origin.x(), -2000000, origin.x(), 2000000);
                        painter->drawLine(-2000000, origin.y(), 2000000, origin.y());
                        }
                }

        if ((is_sch) && (file_type == FT_PROJECT))
                return;
        if (!is_sch && !pcb_grid_visible)
                return;

        qreal passo;
        if (is_sch)
                passo = SCH_CONNECTION_GRID;    //Visualizzo sempre la griglia dlle connessioni
        else
                passo = grid_size();
        if (passo <= 0)
                return;

	//Se la distanza tra un punto di griglia e l'altro è superiore ai 10 pixel (griglia non troppo fitta) disegno la griglia
	int pixel10 = 10 / matrix().m11();
	if (passo > pixel10) 
		{	
                QPoint start = mapToScene(0, 0).toPoint();
                QPoint stop = mapToScene(width(), height()).toPoint();
                int y_div_start = (qreal) start.y() / passo;
                y_div_start--;
                int x_stop = stop.x();
                int y_stop = stop.y();
                int x, y, x_div, y_div;

                x_div = (qreal) start.x() / passo;
                x_div--;
                do
                        {
                        x = x_div * passo;
                        y_div = (qreal) start.y() / passo;
                        y_div--;
                        do
                                {
                                y = y_div * passo;
                                painter->drawPoint(x,y);
                                y_div++;
                                }
                        while (y < y_stop);
                        x_div++;
                        }
                while (x < x_stop);
                }

	if (do_selection_nodes)
		{
		selection_nodes();
		do_selection_nodes = FALSE;
		}
}

void myGraphicsView::drawForeground(QPainter *painter, const QRectF &rect)
{
        QMatrix m = painter->matrix();
        GS->draw_for_screen(painter, &m);

        painter->setMatrixEnabled(FALSE);	//Lavoro in scala 1 a 1 senza nessuna matrice di trasformazione

	if (is_pixmapoutput)
		return;

	if (file_type == FT_PROJECT)
		if (!is_sch)
			{
			if (do_ratsnest_recalculate)
				{
				do_ratsnest_recalculate = FALSE;
				w_main->ratsnest_update();
				}
			//Disegno i ratsnest
                        QColor ratsnest_color = workspace_data[W_PCBratsnest].color;
                        painter->setPen(QPen(ratsnest_color , 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			foreach (ratsnest_struct r, ratsnest)
				{
				node_data *s = (node_data *) r.pcb_node_start;
				node_data *e = (node_data *) r.pcb_node_stop;
				QPoint start = s->point;
				if (s->drag_or_move_active)
					start += drag_or_move_delta;
				QPoint end = e->point;
				if (e->drag_or_move_active)
					end += drag_or_move_delta;
				painter->drawLine(m.map(start), m.map(end));	
				}	
			//Disegno il ratsnest dinamico
			if (dynamic_ratsnest_active)
				{
                                QColor dynamic_ratsnest_color = workspace_data[W_PCBdynamicratsnest].color;
                                painter->setPen(QPen(dynamic_ratsnest_color , 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                                painter->setPen(dynamic_ratsnest_color);
                                painter->drawLine(m.map(mouse_move), m.map(dynamic_ratsnest_endpoint));
				}
			}

        switch (tool)
		{	
		case T_SELECT:
			if (w_main->tool_avviato)
				return;
			break;
		case T_CONNECTION:
			if (w_main->tool_avviato)
                                if (inserting_item_is_visible)
					{
                                        pen_layer.setColor(sel_color_transparent);
                                        brush_layer.setColor(sel_color_transparent);
                                        if (mouse_move_node)
                                                new_connection->end = mouse_move_node->point;
                                        else
                                                new_connection->end = mouse_move;
                                        new_connection->paint(painter, &m);
                                        if (is_sch)
                                                {
                                                pen_layer.setWidth(0);
                                                pen_layer.setStyle(Qt::DotLine);
                                                painter->setPen(pen_layer);
                                                pen_layer.setStyle(Qt::SolidLine);
                                                painter->drawLine(m.map(new_connection->end), m.map(mouse_position));
                                                }
                                        }
                        break;
		case T_POLYLINE:
                        if (w_main->tool_avviato)
                                if (inserting_item_is_visible)
					{
                                        pen_layer.setColor(sel_color_transparent);
                                        brush_layer.setColor(sel_color_transparent);
                                        new_polyline->polygon.last() = mouse_move;
					new_polyline->paint(painter, &m);	
					}
                        break;
		case T_POLYGON:
			if (w_main->tool_avviato)
                                if (inserting_item_is_visible)
					{
                                        pen_layer.setColor(sel_color_transparent);
                                        brush_layer.setColor(sel_color_transparent);
                                        new_polygon->polygon.last() = mouse_move;
					new_polygon->paint(painter, &m);	
					}
                        break;
		case T_RECT:
		case T_ELLIPSE:
			if (w_main->tool_avviato)
                                if (inserting_item_is_visible)
					{
                                        pen_layer.setColor(sel_color_transparent);
                                        brush_layer.setColor(sel_color_transparent);
                                        QRect r = QRect(mouse_new_press, mouse_move);
					r = r.normalized();
					new_rect_or_ellipse->start = r.topLeft();
                                        new_rect_or_ellipse->width = r.width();
                                        new_rect_or_ellipse->height = r.height();
                                        new_rect_or_ellipse->paint(painter, &m);
                                        }
                        break;
                case T_EDIT_COMPONENT:
                        if (wand_ident)
                                {
                                pen_layer.setColor(sel_color);
                                brush_layer.setColor(sel_color);
                                wand_ident->paint(painter, &m);
                                }
                        if (wand_value)
                                {
                                pen_layer.setColor(sel_color);
                                brush_layer.setColor(sel_color);
                                wand_value->paint(painter, &m);
                                }
                        if (wand_pad)
                                {
                                pen_layer.setColor(sel_color);
                                brush_layer.setColor(sel_color);
                                wand_pad->paint(painter, &m);
                                }
                        break;
                case T_EDIT_NODE:
                        if (wand_connection)
                                {
                                if (wand_connection_node)
                                        {
                                        pen_layer.setColor(sel_color);
                                        brush_layer.setColor(sel_color);
                                        painter->setPen(QPen(sel_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                                        painter->setBrush(Qt::NoBrush);
                                        QPoint pixel = mapFromScene(wand_connection_node->point + drag_or_move_delta);
                                        painter->drawEllipse(pixel.x() - 5, pixel.y() - 5, 9, 9);
                                        }
                                else
                                        {
                                        pen_layer.setColor(sel_color);
                                        brush_layer.setColor(sel_color);
                                        wand_connection->paint(painter, &m);
                                        qreal levelOfDetail = matrix().m11();
                                        qreal pixel20 = 20 / levelOfDetail;
                                        if (wand_connection->pen_width > pixel20)
                                                {
                                                painter->setPen(QPen(sel_color_inverted, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                                                painter->setBrush(Qt::NoBrush);
                                                QPoint pixel = mapFromScene(wand_connection->node_start->point + drag_or_move_delta);
                                                painter->drawEllipse(pixel.x() - 5, pixel.y() - 5, 9, 9);
                                                pixel = mapFromScene(wand_connection->node_end->point + drag_or_move_delta);
                                                painter->drawEllipse(pixel.x() - 5, pixel.y() - 5, 9, 9);
                                                }
                                        }
                                }
                        else if (wand_pol)
                                {
                                if (wand_pol_is_vertex)
                                        {
                                        pen_layer.setColor(sel_color);
                                        brush_layer.setColor(sel_color);
                                        painter->setPen(QPen(sel_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                                        painter->setBrush(Qt::NoBrush);
                                        QPoint pixel = mapFromScene(wand_pol->polygon.at(wand_pol_index) + drag_or_move_delta);
                                        painter->drawEllipse(pixel.x() - 5, pixel.y() - 5, 9, 9);
                                        }
                                else
                                        {
                                        pen_layer.setColor(sel_color);
                                        brush_layer.setColor(sel_color);
                                        wand_pol->segment_paint(painter, &m);
                                        qreal levelOfDetail = matrix().m11();
                                        qreal pixel20 = 20 / levelOfDetail;
                                        if (wand_pol->pen_width > pixel20)
                                                {
                                                painter->setPen(QPen(sel_color_inverted, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                                                painter->setBrush(Qt::NoBrush);
                                                QPoint pixel = mapFromScene(wand_pol->polygon.at(wand_pol_index) + drag_or_move_delta);
                                                painter->drawEllipse(pixel.x() - 5, pixel.y() - 5, 9, 9);
                                                if (wand_pol_index)
                                                        pixel = mapFromScene(wand_pol->polygon.at(wand_pol_index-1) + drag_or_move_delta);
                                                else
                                                        pixel = mapFromScene(wand_pol->polygon.last() + drag_or_move_delta);
                                                painter->drawEllipse(pixel.x() - 5, pixel.y() - 5, 9, 9);
                                                }
                                        }
                                }
                        break;
		case T_ADD:
			if (w_main->tool_avviato)
				return;
			break;
                case T_RULER:
                        if (w_main->tool_avviato)
                                {
                                pen_layer.setColor(sel_color);
                                brush_layer.setColor(sel_color);
                                painter->setPen(QPen(sel_color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                                painter->drawLine(mapFromScene(ruler_start), mapFromScene(mouse_move));
                                info_label->setHidden(FALSE);
                                }
                        else
                                info_label->setHidden(TRUE);
                        break;
		}

	//Disegno il rettangolo di selezione
	QRect sr = selection_bounding_rect_in_pixel(selection_bounding_rect(do_selection_rect_recalculate));
	if (!sr.isNull())
		{
		int r = sr.right();
		int l = sr.left();
		int t = sr.top();
		int b = sr.bottom();
		int cx = sr.center().x();
		int cy = sr.center().y();
		painter->setPen(QPen(QColor(0, 0, 0) , 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		painter->setBrush(QColor(255, 255, 255));	

		painter->drawRect(r, t, 5, 5);
		painter->drawRect(r, b - 5, 5, 5);
		painter->drawRect(l - 5, t, 5, 5);
		painter->drawRect(l - 5, b - 5, 5, 5);
		painter->drawRect(cx - 3, b - 5, 5, 5);
		painter->drawRect(cx - 3, t, 5, 5);
		painter->drawRect(r, cy - 3, 5, 5);
		painter->drawRect(l - 5, cy - 3, 5, 5);
		}
        painter->setMatrixEnabled(TRUE);
}

void	myGraphicsView::highlight_clear()
{
	QList<myGraphicsItem *> *ao = &GS->all_obj;
	int ao_size = ao->size();
	for (int i = 0; i < ao_size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
		{
		myGraphicsItem	*obj = ao->at(i);	
		obj->highlight_active = FALSE;
		}

	QList<node_data *> *an = &GV->all_nodes;
	int an_size = GV->all_nodes.size();
	for (int i = 0; i < an_size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
		{
		node_data	*node = an->at(i);	
		node->highlight_active = FALSE;
		}
}	

void	myGraphicsView::highlight_active_net_id()
{
	QList<myGraphicsItem *> *ao = &GS->all_obj;
	int ao_size = ao->size();
	for (int i = 0; i < ao_size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
		{
		myGraphicsItem	*obj = ao->at(i);	
                if (obj->net_id == active_net_id)
			obj->highlight_active = TRUE;
		}
}	

qreal    myGraphicsView::grid_size()
{
        if (is_sch)
                switch (tool)
                {
                case T_POLYLINE:
                case T_POLYGON:
                case T_ELLIPSE:
                case T_RECT:
                case T_EDIT_COMPONENT:
                case T_TEXT:
                        return sch_grid_artistic;
                case T_EDIT_NODE:
                        if (wand_connection)
                                return SCH_CONNECTION_GRID;
                        else
                                return sch_grid_artistic;
                case T_SELECT:
                        return SCH_CONNECTION_GRID;
                default:
                        return SCH_CONNECTION_GRID;
                }
        else
                {
                if (pcb_grid_thou)
                        return pcb_grid_size * 100.0;
                else
                        return pcb_grid_size * 3937.007874;    //Ossia 10^5/25.4
                }
}

char   myGraphicsView::grid_snap()
{
        if (is_sch)
                return 'G';
        else
                switch (tool)
                {
                case T_CONNECTION:
                case T_EDIT_NODE:
                case T_PADVIA:
                case T_RULER:
                case T_ORIGIN:
                        return pcb_grid_snap;
                default:
                        if (pcb_grid_snap == 'I')
                                return 'G';     //L'intellisnap (ossia l'agganciarsi a un nodo) è attivo solo quando lavoro sulle piste
                        else
                                return pcb_grid_snap;
                }
}

QPoint myGraphicsView::grid_delta(QPoint point)
{
        point = point / grid_size();
        return point * grid_size();
}

QPoint  myGraphicsView::grid_point(QPoint punto)
{
        QPoint risultato;
	int x = punto.x();
	int y = punto.y();

        qreal passo = grid_size();

        // Becco il punto della griglia più vicino
        int div = (qreal) x / passo;
        int griglia_p1;
        int griglia_p2;
	if (x > 0)
	{
                griglia_p1 = div * passo;
                griglia_p2 = (div+1) * passo;
                if ((x - griglia_p1) < (griglia_p2 - x))
                        risultato.setX(griglia_p1);
		else
                        risultato.setX(griglia_p2);
	}
	else
	{
                griglia_p1 = div * passo;
                griglia_p2 = (div-1) * passo;
                if ((x - griglia_p1) > (griglia_p2 - x))
                        risultato.setX(griglia_p1);
		else
                        risultato.setX(griglia_p2);
	}
        div = (qreal) y / passo;
        if (y > 0)
        {
                griglia_p1 = div * passo;
                griglia_p2 = (div+1) * passo;
                if ((y - griglia_p1) < (griglia_p2 - y))
                        risultato.setY(griglia_p1);
                else
                        risultato.setY(griglia_p2);
        }
        else
        {
                griglia_p1 = div * passo;
                griglia_p2 = (div-1) * passo;
                if ((y - griglia_p1) > (griglia_p2 - y))
                        risultato.setY(griglia_p1);
                else
                        risultato.setY(griglia_p2);
        }
	return risultato;
}

QIcon	myGraphicsView::layer_square(int row, int lato)
{
	lato -= 4;
	QPixmap pixmap(lato,lato);
        //if (layer_settings[row].visible)
		pixmap.fill(layer_data[row].color);
        /*else
		{
		pixmap.fill(QColor(255,255,255));//Qui sarebbe più giusto mettere il background del widget...
		QPainter p(&pixmap);
		p.setPen(QPen(QColor(0,0,0),1,Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		p.drawLine(2,2,lato-3,lato-3);
		p.drawLine(2,lato-3,lato-3,2);
		p.setPen(QPen(layer_data[row].color,3,Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		p.setBrush(Qt::NoBrush);
		p.drawRect(1, 1, lato - 3, lato - 3);
                }*/
	return QIcon(pixmap);	
}

QPoint myGraphicsView::constraint_45_90(QPoint prec_point, QPoint new_point)
{
	QPoint risultato;
	//Calcolo l'angolo 
        int angolo;
	float deltax = new_point.x() - prec_point.x();
	float deltay = new_point.y() - prec_point.y();

	if (deltax == 0)
	{
		if (deltay > 0)
			angolo = 90;
		else
			angolo = 270;
	}
	else if (deltay == 0)
	{
		if (deltax > 0)
			angolo = 0;
		else
			angolo = 180;	
	}
	else
		{
                float x_div_y = fabs(deltax / deltay);
                bool near_y = (x_div_y < 0.4142);
                bool near_x = (x_div_y > 2.4142);
                if (deltax > 0)
			{
			if (deltay > 0)
				{
				if (near_y)
					angolo = 90;
				else if (near_x)
					angolo = 0;
				else
					angolo = 45;
				}
			else
				{
				if (near_y)
					angolo = 270;
				else if (near_x)
					angolo = 0;
				else
					angolo = 315;
				}
			}
		else
			{
			if (deltay > 0)
				{
				if (near_y)
					angolo = 90;
				else if (near_x)
					angolo = 180;
				else
					angolo = 135;
				}
			else
				{
				if (near_y)
					angolo = 270;
				else if (near_x)
					angolo = 180;
				else
					angolo = 225;
				}
			}
		}

        int distance = sqrt(deltax*deltax + deltay*deltay) / 1.4142;
        switch (angolo)
        {
        case 0:
        case 180:
                risultato.setX(new_point.x());
                risultato.setY(prec_point.y());
                break;
        case 45:
                risultato.setX(prec_point.x() + distance);
                risultato.setY(prec_point.y() + distance);
                break;
        case 135:
                risultato.setX(prec_point.x() - distance);
                risultato.setY(prec_point.y() + distance);
                break;
        case 90:
        case 270:
                risultato.setX(prec_point.x());
                risultato.setY(new_point.y());
                break;
        case 225:
                risultato.setX(prec_point.x() - distance);
                risultato.setY(prec_point.y() - distance);
                break;
        case 315:
                risultato.setX(prec_point.x() + distance);
                risultato.setY(prec_point.y() - distance);
                break;
        default:
                break;
        }
        return risultato;
}

QPoint myGraphicsView::constraint_45(QPoint prec_point, QPoint new_point)
{

        QPoint risultato;
	//Calcolo l'angolo 
	int angolo;
	float deltax = new_point.x() - prec_point.x();
	float deltay = new_point.y() - prec_point.y();
	if (deltax == 0)
	{
		if (deltay > 0)
			angolo = 45;
		else
			angolo = 135;
	}
	else if (deltay == 0)
	{
		if (deltax > 0)
			angolo = 45;
		else
			angolo = 135;	
	}
	else
		{
		if (deltax > 0)
			{
			if (deltay > 0)
				angolo = 45;
			else
				angolo = 315;
			}
		else
			{
			if (deltay > 0)
				angolo = 135;
			else
				angolo = 225;
			}
		}

		int distance;
		if (abs(deltax) > abs(deltay))
			distance = abs(deltax);
		else
			distance = abs(deltay);
		switch (angolo)
		{
		case 45:
			risultato.setX(prec_point.x() + distance);
			risultato.setY(prec_point.y() + distance);
			break;
		case 135:
			risultato.setX(prec_point.x() - distance);
			risultato.setY(prec_point.y() + distance);
			break;
		case 225:
			risultato.setX(prec_point.x() - distance);
			risultato.setY(prec_point.y() - distance);		
			break;
		case 315:
			risultato.setX(prec_point.x() + distance);
			risultato.setY(prec_point.y() - distance);			
			break;
		default:
			break;	
		}
        return risultato;
}

QPoint myGraphicsView::constraint_90(QPoint prec_point, QPoint new_point)
{
        QPoint risultato;

        int deltax = GV->grid_point(new_point).x() - prec_point.x();
        int deltay = GV->grid_point(new_point).y() - prec_point.y();

        if ((abs(deltax) <= SCH_CONNECTION_GRID) && (abs(deltay) <= SCH_CONNECTION_GRID))
                mouse_direction = '?';

        switch (mouse_direction)
        {
        case 'L':
                if (deltax >= 0)
                        {
                        if (deltay >= 0)
                                mouse_direction = 'T';
                        else
                                mouse_direction = 'B';
                        }
                break;
        case 'R':
                if (deltax <= 0)
                        {
                        if (deltay >= 0)
                                mouse_direction = 'T';
                        else
                                mouse_direction = 'B';
                        }
                break;
        case 'T':
                if (deltay <= 0)
                        {
                        if (deltax >= 0)
                                mouse_direction = 'R';
                        else
                                mouse_direction = 'L';
                        }
                break;
        case 'B':
                if (deltay >= 0)
                        {
                        if (deltax >= 0)
                                mouse_direction = 'R';
                        else
                                mouse_direction = 'L';
                        }
                break;
        case '?':
                if (abs(deltax) >= abs(deltay))
                        {
                        if (deltax >= 0)
                                mouse_direction = 'R';
                        else
                                mouse_direction = 'L';
                        }
                else
                        {
                        if (deltay >= 0)
                                mouse_direction = 'T';
                        else
                                mouse_direction = 'B';
                        }
                break;
        }

        switch (mouse_direction)
        {
        case 'L':
        case 'R':
                risultato.setX(new_point.x());
                risultato.setY(prec_point.y());
                break;
        case 'T':
        case 'B':
                risultato.setX(prec_point.x());
                risultato.setY(new_point.y());
                break;
        }
        return risultato;
}

void myGraphicsView::mousePressEvent(QMouseEvent *event)
{
        key_shift = (event->modifiers() == Qt::SHIFT);
	key_ctrl = (event->modifiers() == Qt::CTRL);
        key_alt = (event->modifiers() == Qt::ALT);

        w_main->sb->clearMessage();

        if (event->button() == Qt::LeftButton)
                {
                mouse_press_info(mapToScene(event->pos()).toPoint());
		switch (tool)
		{
		case T_SELECT:
                        highlight_active = FALSE;
                        w_main->disconnect_selection_executed = FALSE;
                        setUpdatesEnabled(FALSE);
			w_main->tool_avviato = FALSE;			
                        w_main->proposed_selection_clear();
                        selection_from_point(mouse_new_press);

                        if (key_alt)
                                {
                                select_clear();
                                if (w_main->proposed_selection_count() >= 1)
                                        {
                                        w_main->proposed_selection_choose_cyclic();
                                        w_main->tool_avviato = FALSE;
                                        }
                                }
                        else
                        {
			if (selection_bounding_rect_in_pixel(selection_bounding_rect(FALSE)).contains(event->pos()))
				{
                                if (key_shift)	//E' giusto perchè quando si clicca con shift sul rettangolo di selezione si può solo aggiungere/togliere un oggetto (vietato muovere o fare un rettangolo di selezione)
					{
                                        w_main->proposed_selection_choose_first();
					w_main->tool_avviato = FALSE;
					setUpdatesEnabled(TRUE);
					return;
					}
				else
					w_main->tool_avviato = TRUE;
                                }

			if (!w_main->tool_avviato)
				if (w_main->proposed_selection_count())
					{
                                        if (key_shift)	//E' giusto perchè quando si clicca con shift su uno o più oggetti si può solo aggiungere/togliere un oggetto (vietato muovere o fare un rettangolo di selezione)
						{
                                                w_main->proposed_selection_choose_first();
						w_main->tool_avviato = FALSE;
						setUpdatesEnabled(TRUE);
						return;
						}
					else
						{
                                                select_clear();
						if (w_main->proposed_selection_count() > 1)
							{
                                                        w_main->proposed_selection_choose_first();
							w_main->tool_avviato = FALSE;
							}
						else
							{
							w_main->tool_avviato = TRUE;
							selection_add();
							}
						}	
					}
							
			if (w_main->tool_avviato)
				{
				if (mode_drag_active)
					drag_or_move_setup('D');
				else
					drag_or_move_setup('M');
				drag_or_move_delta = QPoint(0, 0);

                                viewport()->setCursor(Qt::SizeAllCursor);
				}
			else
				{
				if ((!key_shift) && (w_main->proposed_selection_count() == 0))
                                        select_clear();
				setDragMode(QGraphicsView::RubberBandDrag);	
				QGraphicsView::mousePressEvent(event);	//Senza questa riga non si genera il QRubberBand dello zoom e del select
				viewport()->setCursor(Qt::ArrowCursor);
				}	
                        }
			setUpdatesEnabled(TRUE);
			break;
		case T_CONNECTION:
			{
			if (is_sch)
                                {
                                layer_selected = L_SCHWire;	//Solo nell'SCH la scelta del layer è fatta dal tool utilizzato
                                mouse_direction = '?';
                                }
                        if (mouse_click_num == 1)
				{
				highlight_active = FALSE;
                                new_connection = new myGraphicsItem(O_CONNECTION);
                                new_connection->layer_from = layer_selected;
                                new_connection->layer_to = layer_selected;
                                new_connection->start = mouse_new_press;
                                new_connection->pen_width = GV->style_pen_width();
                                select_clear();
				w_main->tool_avviato = TRUE;

				dynamic_ratsnest_active = FALSE;				
				if (!is_sch)
					{
                                        w_main->net_id_update_pcb();

                                        //Imposto l'highlight delle zone che posso toccare
					highlight_active = TRUE;
					highlight_clear();
                                        if (mouse_new_press_node)
						{
                                                if (mouse_new_press_node->net_id)
                                                        active_net_id = mouse_new_press_node->net_id;
                                                else
                                                        active_net_id = -1;
                                                }
                                        else
                                                active_net_id = active_net_id_from_point(mouse_new_press);
                                        if (active_net_id)
                                                highlight_active_net_id();

                                        if (mouse_new_press_node)
                                                {
                                                w_main->dynamic_ratsnest_setup(mouse_new_press_node);
                                                w_main->dynamic_ratsnest_update(mouse_new_press_node);
                                                }
                                        }
				}
			else
                                {
                                w_main->connection_add_segment();
                                mouseMoveEvent(event);
                                }
			}
			break;
                case T_POLYLINE:
			if (is_sch)
                                layer_selected = L_SCHNormal;	//Solo nell'SCH la scelta del layer è fatta dal tool utilizzato
                        if (mouse_click_num == 1)
				{
				highlight_active = FALSE;
                                new_polyline = new myGraphicsItem(O_POLYLINE);
                                new_polyline->layer_from = layer_selected;
                                new_polyline->layer_to = layer_selected;
                                new_polyline->polygon.append(mouse_new_press);
                                new_polyline->polygon.append(mouse_new_press);
                                new_polyline->fill_polygon = GV->style_fill_polygon();
				new_polyline->pen_width = GV->style_pen_width();
                                select_clear();
				w_main->tool_avviato = TRUE;
				break;
				}
			
                        if (mouse_new_press == mouse_old_press)
				return;

			GV->setUpdatesEnabled(FALSE);
                        new_polyline->polygon.append(mouse_move);
			new_polyline->info_update();

                        mouse_new_press = mouse_move;
			GV->do_selection_rect_recalculate = TRUE;
			GV->setUpdatesEnabled(TRUE);
			break;
		case T_POLYGON:
			if (is_sch)
                                layer_selected = L_SCHNormal;	//Solo nell'SCH la scelta del layer è fatta dal tool utilizzato
                        if (mouse_click_num == 1)
				{
				highlight_active = FALSE;
                                new_polygon = new myGraphicsItem(O_POLYGON);
                                new_polygon->layer_from = layer_selected;
                                new_polygon->layer_to = layer_selected;
                                new_polygon->polygon.append(mouse_new_press);
                                new_polygon->polygon.append(mouse_new_press);
                                new_polygon->fill_polygon = GV->style_fill_polygon();
                                if (new_polygon->fill_polygon)
                                        new_polygon->pen_width = 0;
                                else
                                        new_polygon->pen_width = GV->style_pen_width();
                                select_clear();
				w_main->tool_avviato = TRUE;
				break;
				}
			
                        if (mouse_new_press == mouse_old_press)
				return;

			GV->setUpdatesEnabled(FALSE);
                        new_polygon->polygon.append(mouse_move);
			new_polygon->info_update();

                        mouse_new_press = mouse_move;
			GV->do_selection_rect_recalculate = TRUE;
			GV->setUpdatesEnabled(TRUE);
			break;
		case T_RECT:
		case T_ELLIPSE:
			{
			if (is_sch)
                                layer_selected = L_SCHNormal;	//Solo nell'SCH la scelta del layer è fatta dal tool utilizzato
			highlight_active = FALSE;
                        if (tool == T_RECT)
                                new_rect_or_ellipse = new myGraphicsItem(O_RECT);
			else
                                new_rect_or_ellipse = new myGraphicsItem(O_ELLIPSE);
                        new_rect_or_ellipse->layer_from = layer_selected;
                        new_rect_or_ellipse->layer_to = layer_selected;
                        new_rect_or_ellipse->fill_polygon = GV->style_fill_polygon();
                        if (new_rect_or_ellipse->fill_polygon)
				new_rect_or_ellipse->pen_width = 0;
			else
				new_rect_or_ellipse->pen_width = GV->style_pen_width();
                        select_clear();
			w_main->tool_avviato = TRUE;
			break;
			}
			break;
		case T_TEXT:
			{
			if (is_sch)
                                layer_selected = L_SCHNormal;	//Solo nell'SCH la scelta del layer è fatta dal tool utilizzato
			highlight_active = FALSE;
                        myGraphicsItem *obj = new myGraphicsItem(O_TEXT);
                        obj->layer_from = layer_selected;
                        obj->layer_to = layer_selected;
                        obj->start = mouse_new_press;
                        obj->font_height = style_font_height();
                        obj->font_bold = style_font_bold();
                        obj->font_italic = style_font_italic();
			obj->text_position = 'R';
                        if (!is_sch && (layer_selected >= L_PCBCopper16))
                                obj->mirror_text_on = TRUE;
                        else
                                obj->mirror_text_on = FALSE;
			win_object_prop dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, obj);
			dialogo.setWindowTitle(w_main->actions_textAct->text());
			switch (dialogo.exec())	
			{
			case QDialog::Accepted:
				{
				setUpdatesEnabled(FALSE);
                                select_clear();
				ur.beginMacro(w_main->actions_textAct->text());
				obj->font_height = dialogo.new_font_height;
				obj->font_bold = dialogo.new_font_bold;
				obj->font_italic = dialogo.new_font_italic;
                                obj->mirror_text_on = dialogo.new_mirror_text_on;
				obj->text = dialogo.new_text;
				ur.push(new add_object(obj));
				ur.endMacro();
				setUpdatesEnabled(TRUE);
				}
				break;
			case QDialog::Rejected:
				delete obj;
				break;
			}			
			}	
			break;
		case T_PIN:
			{
                        layer_selected = L_SCHNormal;
			highlight_active = FALSE;
                        myGraphicsItem *obj = new myGraphicsItem(O_PIN);
                        obj->layer_from = layer_selected;
                        obj->layer_to = layer_selected;
                        obj->start = mouse_new_press;
                        obj->number = w_main->get_first_available_pin(FALSE);
			obj->text = QString::number(obj->number);
			obj->text_position = 'R';
                        obj->shape = PIN_SHAPE_LINE;
			win_object_prop dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, obj);
			dialogo.setWindowTitle(w_main->actions_pinAct->text());
			switch (dialogo.exec())	
			{
			case QDialog::Accepted:
				{
				setUpdatesEnabled(FALSE);
                                select_clear();
				obj->number = dialogo.new_number;
				obj->text_position = dialogo.new_text_position;
                                obj->text = dialogo.new_text;
				obj->font_height = dialogo.new_font_height;
				obj->font_bold = dialogo.new_font_bold;
				obj->font_italic = dialogo.new_font_italic;
                                obj->shape = dialogo.new_shape;
				node_data *nd = node_new();
                                nd->point = mouse_new_press;
				nd->connected_pin_or_pad_or_via = (int) obj;
				obj->node_start = nd;
				ur.beginMacro(w_main->actions_pinAct->text());
				ur.push(new add_node(nd));
				ur.push(new add_object(obj));
				ur.endMacro();
				setUpdatesEnabled(TRUE);
				}
				break;
			case QDialog::Rejected:
				delete obj;
				break;
			}			
			}	
			break;
                case T_PADVIA:
			{
			highlight_active = FALSE;
                        myGraphicsItem *obj;
			if (file_type == FT_PROJECT)
                                {
                                obj = new myGraphicsItem(O_VIA);
                                obj->width = style_pcb_via_width;
                                obj->drill = style_pcb_via_drill;
                                obj->layer_from = style_pcb_via_layer_from;
                                obj->layer_to = style_pcb_via_layer_to;
                                }
			else
                                {
                                obj = new myGraphicsItem(O_PAD);
                                obj->number = w_main->get_first_available_pad(FALSE);
                                obj->text = QString::number(obj->number);
                                obj->shape = style_pcb_pad_shape;
                                obj->width = style_pcb_pad_width;
                                obj->height = style_pcb_pad_height;
                                obj->drill = style_pcb_pad_drill;
                                obj->layer_from = style_pcb_pad_layer_from;
                                obj->layer_to = style_pcb_pad_layer_to;
                                }
                        obj->start = mouse_new_press;
                        win_object_prop dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, obj);
                        dialogo.setWindowTitle(w_main->actions_padviaAct->text());
			switch (dialogo.exec())	
			{
			case QDialog::Accepted:
				{
                                setUpdatesEnabled(FALSE);
                                select_clear();
                                if (file_type == FT_PROJECT)
                                        {
                                        dialogo.via->update_new_values();
                                        obj->width = dialogo.new_width;
                                        obj->height = dialogo.new_width;
                                        obj->layer_from = dialogo.new_layer_from;
                                        obj->layer_to = dialogo.new_layer_to;
                                        obj->drill = dialogo.new_drill;
                                        }
                                else
                                        {
                                        dialogo.pad->update_new_values();
                                        obj->number = dialogo.new_number;
                                        obj->shape = dialogo.new_shape;
                                        obj->width = dialogo.new_width;
                                        obj->height = dialogo.new_height;
                                        obj->layer_from = dialogo.new_layer_from;
                                        obj->layer_to = dialogo.new_layer_to;
                                        obj->drill = dialogo.new_drill;
                                        }
                                ur.beginMacro(w_main->actions_padviaAct->text());
                                if (!mouse_new_press_node || mouse_new_press_node->connected_pin_or_pad_or_via)
                                        {
                                        node_data *nd = node_new();
                                        nd->point = mouse_new_press;
                                        nd->connected_pin_or_pad_or_via = (int) obj;
                                        obj->node_start = nd;
                                        ur.push(new add_node(nd));
                                        }
                                else
                                        {
                                        mouse_new_press_node->connected_pin_or_pad_or_via = (int) obj;
                                        obj->node_start = mouse_new_press_node;
                                        }
                                ur.push(new add_object(obj));
				ur.endMacro();
                                setUpdatesEnabled(TRUE);
				}
				break;
			case QDialog::Rejected:
				delete obj;
				break;
			}			
			}	
			break;
                case T_ADD_POWER:
                        {
                        layer_selected = L_SCHNormal;
                        highlight_active = FALSE;
                        myGraphicsItem *obj = new myGraphicsItem(O_POWER);
                        obj->layer_from = layer_selected;
                        obj->layer_to = layer_selected;
                        obj->start = mouse_new_press;
                        obj->text_position = 'L';
                        obj->shape = SYMBOL_GROUND_POWER;
                        win_object_prop dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, obj);
                        dialogo.setWindowTitle(w_main->actions_add_powerAct->text());
                        switch (dialogo.exec())
                        {
                        case QDialog::Accepted:
                                {
                                setUpdatesEnabled(FALSE);
                                select_clear();
                                obj->text = dialogo.new_text;
                                obj->text_position = dialogo.new_text_position;
                                obj->font_height = dialogo.new_font_height;
                                obj->font_bold = dialogo.new_font_bold;
                                obj->font_italic = dialogo.new_font_italic;
                                obj->shape = dialogo.new_shape;
                                ur.beginMacro(w_main->actions_portAct->text());
                                if (!mouse_new_press_node || mouse_new_press_node->connected_pin_or_pad_or_via)
                                        {
                                        node_data *nd = node_new();
                                        nd->point = mouse_new_press;
                                        nd->connected_pin_or_pad_or_via = (int) obj;
                                        obj->node_start = nd;
                                        ur.push(new add_node(nd));
                                        }
                                else
                                        {
                                        mouse_new_press_node->connected_pin_or_pad_or_via = (int) obj;
                                        obj->node_start = mouse_new_press_node;
                                        }
                                ur.push(new add_object(obj));
                                ur.endMacro();
                                setUpdatesEnabled(TRUE);
                                }
                                break;
                        case QDialog::Rejected:
                                delete obj;
                                break;
                        }
                        }
                        break;
		case T_PORT:
			{
                        layer_selected = L_SCHNormal;
			highlight_active = FALSE;
                        myGraphicsItem *obj = new myGraphicsItem(O_PORT);
                        obj->layer_from = layer_selected;
                        obj->layer_to = layer_selected;
                        obj->start = mouse_new_press;
			obj->text_position = 'R';
                        obj->shape = PORT_GENERIC;
			win_object_prop dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, obj);
			dialogo.setWindowTitle(w_main->actions_portAct->text());
			switch (dialogo.exec())	
			{
			case QDialog::Accepted:
				{
				setUpdatesEnabled(FALSE);
                                select_clear();
				obj->text = dialogo.new_text;
				obj->text_position = dialogo.new_text_position;
				obj->font_height = dialogo.new_font_height;
				obj->font_bold = dialogo.new_font_bold;
				obj->font_italic = dialogo.new_font_italic;
                                obj->shape = dialogo.new_shape;
				node_data *nd = node_new();
                                nd->point = mouse_new_press;
				nd->connected_pin_or_pad_or_via = (int) obj;
				obj->node_start = nd;
				ur.beginMacro(w_main->actions_portAct->text());
                                if (!mouse_new_press_node || mouse_new_press_node->connected_pin_or_pad_or_via)
                                        {
                                        node_data *nd = node_new();
                                        nd->point = mouse_new_press;
                                        nd->connected_pin_or_pad_or_via = (int) obj;
                                        obj->node_start = nd;
                                        ur.push(new add_node(nd));
                                        }
                                else
                                        {
                                        mouse_new_press_node->connected_pin_or_pad_or_via = (int) obj;
                                        obj->node_start = mouse_new_press_node;
                                        }
                                ur.push(new add_object(obj));
				ur.endMacro();
				setUpdatesEnabled(TRUE);
				}
				break;
			case QDialog::Rejected:
				delete obj;
				break;
			}			
			}	
			break;
                case T_ORIGIN:
                        origin = mouse_new_press;
                        setUpdatesEnabled(FALSE);
                        setUpdatesEnabled(TRUE);
                        break;
                case T_RULER:
                        ruler_start = mouse_new_press;
                        w_main->tool_avviato = TRUE;
                        break;
		case T_ZOOM:
                        setDragMode(QGraphicsView::RubberBandDrag);
			QGraphicsView::mousePressEvent(event);	//Senza questa riga non si genera il QRubberBand dello zoom e del select
			break;
		case T_HAND:
			QGraphicsView::mousePressEvent(event);	//Senza questa riga non si genera il QRubberBand dello zoom e del select
			break;
                case T_EDIT_COMPONENT:
                        highlight_active = FALSE;
                        mouse_new_press = mapToScene(event->pos()).toPoint();
                        select_clear();
                        drag_or_move_setup('R');
                        if (wand_ident)
                                wand_ident->drag_or_move_active = TRUE;
                        else if (wand_value)
                                wand_value->drag_or_move_active = TRUE;
                        w_main->tool_avviato = TRUE;
                        break;
                case T_EDIT_NODE:
			highlight_active = FALSE;
                        select_clear();
                        drag_or_move_setup('R');
                        if (wand_connection && wand_connection_node)
                                {
                                mouse_new_press = wand_connection_node->point;
                                wand_connection->drag_or_move_active = TRUE;
                                wand_connection_node->drag_or_move_active = TRUE;
                                if (!wand_connection_node->connected_pin_or_pad_or_via)
                                        if (wand_connection_node->connected_wires.count() == 2)
                                                foreach (int obj, wand_connection_node->connected_wires)
                                                        {
                                                        myGraphicsItem *wire = (myGraphicsItem *) obj;
                                                        wire->drag_or_move_active = TRUE;
                                                        }
                                w_main->tool_avviato = TRUE;
                                }
                        else if (wand_pol && wand_pol_is_vertex)
                                {
                                mouse_new_press = wand_pol->polygon.at(wand_pol_index);
                                wand_pol->drag_or_move_active = TRUE;
                                w_main->tool_avviato = TRUE;
                                }
                        break;
		case T_ADD:
                        setUpdatesEnabled(FALSE);
                        switch (cloned_action)
                        {
                        case 'P':
                                ur.beginMacro(tr("Paste"));
                                break;
                        case 'D':
                                ur.beginMacro(tr("Duplicate"));
                                break;
                        case 'A':
                                ur.beginMacro(w_main->actions_addAct->text());
                                break;
                        }
                        clone_to_selection();
                        clone_from_selection();
                        clone_ident_pin_pad_recalculate();
                        if (is_sch)
                                w_main->connect_sch_selection();
                        select_clear();
                        ur.endMacro();
                        if (cloned_action != 'A')
                                {
                                clipboard_obj.clear();
                                clipboard_comp.clear();
                                clipboard_nodes.clear();
                                tool_stop();
                                }

                        setUpdatesEnabled(TRUE);
                        break;
		}	
                }

        else if (event->button() == Qt::RightButton)
                {
                mouse_right_button = mapToScene(event->pos()).toPoint();
                switch (tool)
		{
		case T_SELECT:
                        w_main->proposed_selection_clear();
                        selection_from_point(mouse_right_button);
                        if (w_main->proposed_selection_count() || selection_bounding_rect_in_pixel(selection_bounding_rect(FALSE)).contains(event->pos()))
                                {
                                if (is_sch)
                                        w_main->popupSCHEditMenu->popup(QCursor::pos());
                                else
                                        w_main->popupPCBEditMenu->popup(QCursor::pos());
                                }
                        break;
		case T_CONNECTION:
                        if (w_main->tool_avviato)
				{
				if (highlight_active)
					w_main->popupPCBwireMenu->popup(QCursor::pos());
				else
					w_main->popupSCHwireMenu->popup(QCursor::pos());
				}
			break;
		case T_POLYLINE:
		case T_POLYGON:
			if (w_main->tool_avviato)
				tool_stop();
			break;
		case T_ZOOM:
                        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
			zoom_scale(0.8);
			w_main->Z->zoom_show_scale();
			break;
                case T_RULER:
                        setUpdatesEnabled(FALSE);
                        w_main->tool_avviato = FALSE;
                        setUpdatesEnabled(TRUE);
                        break;
                case T_EDIT_NODE:
                        if (wand_connection || wand_pol)
				{
                                if (wand_pol && wand_pol_is_vertex)
                                        {
                                        if ((GV->wand_pol->tipo == O_POLYGON) && (GV->wand_pol->polygon.count() <= 3))
                                                break;
                                        else if ((GV->wand_pol->tipo == O_POLYLINE) && (GV->wand_pol->polygon.count() <= 2))
                                                break;
                                        }
                                wand_cursor_pos = QCursor::pos();
                                w_main->popupEditNodeMenu->popup(QCursor::pos());
				}
			break;
                case T_EDIT_COMPONENT:
                        if (wand_ident || wand_value || wand_pad)
                                {
                                w_main->popupEditComponentMenu->popup(QCursor::pos());
                                }
                        break;
                }
                }

        else if (event->button() == Qt::MidButton)
                {
                // Il setDragMode(QGraphicsView::ScrollHandDrag) purtroppo funziona in Qt solo col il pulsante sinistro...
                midbutton_pan_on = TRUE;
                midbutton_pan_start_point = event->pos();
                QScrollBar *vsb = verticalScrollBar();
                midbutton_pan_start_vsb = vsb->value();
                QScrollBar *hsb = horizontalScrollBar();
                midbutton_pan_start_hsb = hsb->value();
                return;
                }
}

void myGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
        if (event->button() == Qt::LeftButton)
                {
                mouse_release_info(mapToScene(event->pos()).toPoint());
		switch (tool)
		{
		case T_SELECT:
			if (w_main->tool_avviato)
				{
                                setUpdatesEnabled(FALSE);
                                if (!drag_or_move_delta.isNull())
					{
                                        if (mode_drag_active)
						drag_or_move_execute('D');	
					else
                                                drag_or_move_execute('M');
                                        }
				drag_or_move_setup('R');
								
                                setUpdatesEnabled(TRUE);
				w_main->tool_avviato = FALSE;
				}
			else
				{
                                QGraphicsView::mouseReleaseEvent(event);	//Senza questa riga non si elimina il QRubberBand dello zoom e del select
				setDragMode(QGraphicsView::NoDrag);
                                if (mouse_release != mouse_new_press)
					{
					setUpdatesEnabled(FALSE);
                                        selection_from_rect(myQRect(mouse_new_press, mouse_release));
					setUpdatesEnabled(TRUE);
					}
				}
                        if (selection_bounding_rect(FALSE).contains(mouse_new_press))
				viewport()->setCursor(Qt::SizeAllCursor);
			else
				viewport()->setCursor(Qt::ArrowCursor);
			break;
		case T_RECT:
		case T_ELLIPSE:
                        if ((mouse_release.x() == mouse_new_press.x()) || (mouse_release.y() == mouse_new_press.y()))
				{
				QMessageBox::warning(this, QObject::tr("Error"), QObject::tr("Can't finish here"));
				delete new_rect_or_ellipse;
				w_main->tool_avviato = FALSE;
				GV->do_selection_rect_recalculate = TRUE;
				GV->setUpdatesEnabled(TRUE);
				break;
				}

			GV->setUpdatesEnabled(FALSE);
                        if (tool == T_RECT)
                                GV->ur.beginMacro(w_main->actions_rectangleAct->text());
                        else
                                GV->ur.beginMacro(w_main->actions_circleAct->text());
                        GV->ur.push(new add_object(new_rect_or_ellipse));
                        GV->ur.endMacro();
			new_rect_or_ellipse->info_update();
                        tool_stop();
                        GV->do_selection_rect_recalculate = TRUE;
			GV->setUpdatesEnabled(TRUE);
			break;
		case T_ZOOM:
                        QGraphicsView::mouseReleaseEvent(event);	//Senza questa riga non si elimina il QRubberBand dello zoom e del select
			setDragMode(QGraphicsView::NoDrag);
                        if (mouse_release == mouse_new_press)
				{
				setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
				zoom_scale(1.25);
				w_main->Z->zoom_show_scale();
				}
			else
                                zoom_rect(myQRect(mouse_new_press, mouse_release));
			break;
		case T_HAND:
			QGraphicsView::mouseReleaseEvent(event);	//Senza questa riga non si termina il dragmode	
			break;
                case T_EDIT_COMPONENT:
                        if (w_main->tool_avviato)
                                {
                                //mouse_release = mapToScene(event->pos()).toPoint();	//Qui non devo applicare la funzione grid_point()
                                drag_or_move_delta = grid_delta(mouse_release - mouse_new_press);
                                setUpdatesEnabled(FALSE);
                                if (!drag_or_move_delta.isNull())
                                        {
                                        if (wand_ident)
                                                {
                                                ur.beginMacro(tr("Drag Ident"));
                                                ur.push(new move_obj(drag_or_move_delta, wand_ident));
                                                ur.endMacro();
                                                }
                                        else if (wand_value)
                                                {
                                                ur.beginMacro(tr("Drag Value"));
                                                ur.push(new move_obj(drag_or_move_delta, wand_value));
                                                ur.endMacro();
                                                }
                                        }
                                drag_or_move_setup('R');
                                setUpdatesEnabled(TRUE);
                                w_main->tool_avviato = FALSE;
                        }
                        break;
                case T_EDIT_NODE:
			if (w_main->tool_avviato)
				{
                                setUpdatesEnabled(FALSE);
                                if (!drag_or_move_delta.isNull())
					{
                                        if (wand_connection && wand_connection_node)
						{
                                                ur.beginMacro(tr("Drag Node"));
                                                node_split(wand_connection_node);
                                                ur.push(new connection_move_node(drag_or_move_delta, wand_connection_node));
                                                node_join(wand_connection_node);
                                                ur.endMacro();
                                                }
                                        else if (wand_pol && wand_pol_is_vertex)
                                                {
                                                ur.beginMacro(tr("Drag Node"));
                                                ur.push(new pol_move_node(drag_or_move_delta, wand_pol, wand_pol_index));
                                                ur.endMacro();
						}
                                        }
                                drag_or_move_setup('R');
                                setUpdatesEnabled(TRUE);
                                w_main->tool_avviato = FALSE;
			}
			break;
		}	
        }

        else if (event->button() == Qt::MidButton)
                {
                // Il setDragMode(QGraphicsView::ScrollHandDrag) purtroppo funziona in Qt solo col il pulsante sinistro...
                midbutton_pan_on = FALSE;
                return;
                }
}
 
void myGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
        QGraphicsView::mouseMoveEvent(event);	//Questa riga mi serve per gestire correttamente il wheelEvent()

        //Senza questa riga non si genera il QRubberBand dello zoom e del select
	key_ctrl = (event->modifiers() == Qt::CTRL);

        if (midbutton_pan_on)
                {
                QPoint delta = event->pos() - midbutton_pan_start_point;
                QScrollBar *vsb = verticalScrollBar();
                vsb->setValue(midbutton_pan_start_vsb - delta.y());
                QScrollBar *hsb = horizontalScrollBar();
                hsb->setValue(midbutton_pan_start_hsb - delta.x());
                return;
                }

        mouse_move_info(mapToScene(event->pos()).toPoint());

        switch (tool)
	{
	case T_SELECT:
		if (w_main->tool_avviato)
			{
                        setUpdatesEnabled(FALSE);
                        drag_or_move_delta = grid_delta(mouse_move - mouse_new_press);
                        if (!drag_or_move_delta.isNull())
				if (!mode_drag_active)
                                        if (!w_main->disconnect_selection_executed)
						{
						ur.beginMacro(tr("Move"));
                                                if (GV->is_sch)
                                                        w_main->disconnect_sch_selection();
                                                else
                                                        w_main->disconnect_pcb_selection();
                                                }
					
                        setUpdatesEnabled(TRUE);
			viewport()->setCursor(Qt::SizeAllCursor);
			}
		else
			{
                        if (selection_bounding_rect_in_pixel(selection_bounding_rect(FALSE)).contains(event->pos()))	//position))
				viewport()->setCursor(Qt::SizeAllCursor);
			else
				viewport()->setCursor(Qt::ArrowCursor);
                        }
		break;
	case T_CONNECTION:
                if (w_main->tool_avviato)
                        {
                        setUpdatesEnabled(FALSE);
                        inserting_item_is_visible = TRUE;
                        setUpdatesEnabled(TRUE);
                        }
                break;
	case T_POLYLINE:
	case T_POLYGON:
                if (w_main->tool_avviato)
			{
                        setUpdatesEnabled(FALSE);
			inserting_item_is_visible = TRUE;
                        setUpdatesEnabled(TRUE);
                        }
		break;
	case T_RECT:
	case T_ELLIPSE:
		if (w_main->tool_avviato)
			{
                        setUpdatesEnabled(FALSE);
			inserting_item_is_visible = TRUE;
                        setUpdatesEnabled(TRUE);
			}
		break;
        case T_EDIT_COMPONENT:
                if (w_main->tool_avviato)
                        {
                        setUpdatesEnabled(FALSE);
                        drag_or_move_delta = grid_delta(mouse_move - mouse_new_press);
                        setUpdatesEnabled(TRUE);
                        }
                else
                        {
                        QPoint exact_point = mapToScene(event->pos()).toPoint();
                        //Controllo se sono già sopra al nodo/oggetto già selezionato. Se è così non faccio niente
                        if (wand_ident)
                                if (wand_ident->hitTest(exact_point))
                                        // Sono già su ident e non faccio niente
                                        break;
                        if (wand_value)
                                if (wand_value->hitTest(exact_point))
                                        // Sono già su value e non faccio niente
                                        break;
                        if (wand_pad)
                                if (wand_pad->hitTest(exact_point))
                                        // Sono già sul pad e non faccio niente
                                        break;

                        //Scelgo il primo nodo/oggetto
                        wand_connection = 0;
                        wand_connection_node = 0;
                        wand_ident = 0;
                        wand_value = 0;
                        wand_pad = 0;
                        wand_pol = 0;

                        foreach (myGraphicsItem *obj, scene->all_obj)
                                {
                                bool exit = FALSE;  //Uso questa variabile perchè break mi fa uscire solo da switch() e non da foreach()
                                switch (obj->tipo)
                                {
                                case O_IDENT:
                                        if (obj->hitTest(exact_point))
                                                {
                                                wand_ident = obj;
                                                exit = TRUE;
                                                }
                                        break;
                                case O_VALUE:
                                        if (obj->hitTest(exact_point))
                                                {
                                                wand_value = obj;
                                                exit = TRUE;
                                                }
                                        break;
                                case O_PAD:
                                        if (obj->hitTest(exact_point))
                                                {
                                                wand_pad = obj;
                                                exit = TRUE;
                                                }
                                        break;
                                }
                                if (exit)
                                        break;
                                }

                        //Aggiorno lo schermo
                        setUpdatesEnabled(FALSE);
                        select_clear();
                        setUpdatesEnabled(TRUE);
                        }
                break;
        case T_EDIT_NODE:
                if (w_main->tool_avviato)
                        {
                        setUpdatesEnabled(FALSE);
                        if (mouse_move_node)
                                drag_or_move_delta = mouse_move - mouse_new_press;
                        else
                                drag_or_move_delta = grid_delta(mouse_move - mouse_new_press);
                        setUpdatesEnabled(TRUE);
                        }
                else
                        {
                        qreal levelOfDetail = matrix().m11();
                        qreal pixel3 = 3 / levelOfDetail;
                        qreal pixel5 = 5 / levelOfDetail;
                        QRect mouse_move_rect = QRect(mouse_move.x() - pixel3, mouse_move.y() - pixel3, pixel5, pixel5);
                        //Controllo se sono già sopra al nodo/oggetto già selezionato. Se è così non faccio niente
                        if (wand_connection)
                                {
                                if (wand_connection_node)
                                        {
                                        if (mouse_move_rect.contains(wand_connection_node->point))
                                                // Sono già sul nodo e non faccio niente
                                                break;
                                        else if (wand_connection->hitTest(mouse_move))
                                                {
                                                wand_connection_node = 0;
                                                //Aggiorno lo schermo
                                                setUpdatesEnabled(FALSE);
                                                select_clear();
                                                setUpdatesEnabled(TRUE);
                                                break;
                                                }
                                        }
                                else if (wand_connection->hitTest(mouse_move))
                                        {
                                        //Controllo se non sono finito sopra uno dei nodi della connection
                                        if (mouse_move_rect.contains(wand_connection->node_start->point))
                                                {
                                                wand_connection_node = wand_connection->node_start;
                                                //Aggiorno lo schermo
                                                setUpdatesEnabled(FALSE);
                                                select_clear();
                                                setUpdatesEnabled(TRUE);
                                                }
                                        else if (mouse_move_rect.contains(wand_connection->node_end->point))
                                                {
                                                wand_connection_node = wand_connection->node_end;
                                                //Aggiorno lo schermo
                                                setUpdatesEnabled(FALSE);
                                                select_clear();
                                                setUpdatesEnabled(TRUE);
                                                }
                                        break;
                                        }
                                }
                        if (wand_pol)
                                {
                                if (wand_pol_is_vertex)
                                        {
                                        if (mouse_move_rect.contains(wand_pol->polygon.at(wand_pol_index)))
                                                // sono già sul nodo e non faccio niente
                                                break;
                                        }
                                else
                                        {
                                        //Controllo se sono finito su un nodo
                                        for (int i = 0; i < wand_pol->polygon.count();i++)
                                                if (mouse_move_rect.contains(wand_pol->polygon.at(i)))
                                                        {
                                                        wand_pol_is_vertex = TRUE;
                                                        wand_pol_index = i;
                                                        //Aggiorno lo schermo
                                                        setUpdatesEnabled(FALSE);
                                                        select_clear();
                                                        setUpdatesEnabled(TRUE);
                                                        break;
                                                        }
                                        if (wand_pol->segment_hitTest(mouse_move))
                                                // sono già sul segmento e non faccio niente
                                                break;
                                        }
                                        }

                        //Scelgo il primo nodo/oggetto
                        wand_connection = 0;
                        wand_connection_node = 0;
                        wand_ident = 0;
                        wand_pad = 0;
                        wand_value = 0;
                        wand_pol = 0;

                        foreach (myGraphicsItem *obj, scene->all_obj)
                                {
                                bool exit = FALSE;  //Uso questa variabile perchè break mi fa uscire solo da switch() e non da foreach()
                                if (!obj->comp && ((edit_node_is_all_layers()) || ((obj->layer_from <= layer_selected) && (obj->layer_to >= layer_selected))))
                                        if (obj->is_showed())
                                                {
                                                switch (obj->tipo)
                                                {
                                                case O_CONNECTION:
                                                        if (obj->boundingRect().intersects(mouse_move_rect))
                                                                {
                                                                wand_connection = obj;
                                                                wand_connection_node = (node_data *) obj->node_start;
                                                                float deltax = wand_connection_node->point.x() - mouse_move.x();
                                                                float deltay = wand_connection_node->point.y() - mouse_move.y();
                                                                int distance = sqrt(deltax * deltax + deltay * deltay);
                                                                if (distance > pixel5)
                                                                        {
                                                                        wand_connection_node = (node_data *) obj->node_end;
                                                                        deltax = wand_connection_node->point.x() - mouse_move.x();
                                                                        deltay = wand_connection_node->point.y() - mouse_move.y();
                                                                        distance = sqrt(deltax * deltax + deltay * deltay);
                                                                        if (distance > pixel5)
                                                                                wand_connection_node = 0;
                                                                        }
                                                                if (wand_connection_node)
                                                                        exit = TRUE;
                                                                else
                                                                        {
                                                                        if (obj->hitTest(mouse_move))
                                                                                exit = TRUE;
                                                                        else
                                                                                wand_connection = 0;
                                                                        }
                                                                }
                                                        break;
                                                case O_POLYLINE:
                                                case O_POLYGON:
                                                        if (obj->boundingRect().intersects(mouse_move_rect))
                                                                {
                                                                //Controllo prima tutti i nodi
                                                                for (wand_pol_index = 0; wand_pol_index < obj->polygon.count();wand_pol_index++)
                                                                        {
                                                                        float deltax = obj->polygon.at(wand_pol_index).x() - mouse_move.x();
                                                                        float deltay = obj->polygon.at(wand_pol_index).y() - mouse_move.y();
                                                                        int distance = sqrt(deltax * deltax + deltay * deltay);
                                                                        if (distance < pixel5)
                                                                                {
                                                                                wand_pol = obj;
                                                                                wand_pol_is_vertex = TRUE;
                                                                                exit = TRUE;
                                                                                break;
                                                                                }
                                                                        }
                                                                if (wand_pol == 0)
                                                                        {
                                                                        //Controllo tutti i segmenti
                                                                        if (obj->tipo == O_POLYGON)
                                                                                wand_pol_index = 0;   //Solo il poligono ha il segmento che va dal primo all'ultimo punto
                                                                        else
                                                                                wand_pol_index = 1;
                                                                        for (; wand_pol_index < obj->polygon.count();wand_pol_index++)
                                                                                if (obj->segment_hitTest(mouse_move))
                                                                                        {
                                                                                        wand_pol = obj;
                                                                                        wand_pol_is_vertex = FALSE;
                                                                                        exit = TRUE;
                                                                                        break;
                                                                                        }
                                                                        }
                                                                }
                                                        break;
                                                }
                                        if (exit)
                                                break;
                                        }
                                }

                        //Aggiorno lo schermo
                        setUpdatesEnabled(FALSE);
                        select_clear();
                        setUpdatesEnabled(TRUE);
                        }
                break;
        case T_RULER:
                if (w_main->tool_avviato)
                        {
                        setUpdatesEnabled(FALSE);
                        qint64 ruler_x = mouse_move.x() - ruler_start.x();
                        qint64 ruler_y = mouse_move.y() - ruler_start.y();
                        if (y_axis_is_up)
                                ruler_y = -ruler_y;
                        qint64 ruler_length = sqrt(ruler_x*ruler_x + ruler_y*ruler_y);
                        info_label->setText(QString("Length = %1\n\nX = %2 \nY = %3").arg(from_001thou_to_string(ruler_length, UM), from_001thou_to_string(ruler_x, UM), from_001thou_to_string(ruler_y, UM)));
                        info_label->adjustSize();

                        QPoint new_pos = event->pos() + QPoint(10,10);
                        if ((new_pos.y() + info_label->height() + 50) > height())
                                new_pos += QPoint(0,-20-info_label->height());
                        if ((new_pos.x() + info_label->width() + 50) > width())
                                new_pos += QPoint(-20-info_label->width(),0);
                        info_label->move(new_pos);

                        setUpdatesEnabled(TRUE);
                        }
                break;
	case T_ZOOM:
		break;
	case T_HAND:
		break;
	case T_ADD:
		if (w_main->tool_avviato)		
			{
                        setUpdatesEnabled(FALSE);
                        GV->clone_move(grid_delta(mouse_move), FALSE);
			setUpdatesEnabled(TRUE);
			viewport()->setCursor(Qt::SizeAllCursor);
                        }
		break;
	}	

	//Aggiorno la statusbar
        QPoint coordinate = coordinate_transform_to_user(mapToScene(event->pos()).toPoint());
        w_main->sb_x->setText(QString("X = %1").arg(from_001thou_to_string(coordinate.x(), UM)));
        w_main->sb_y->setText(QString("Y = %1").arg(from_001thou_to_string(coordinate.y(), UM)));
	if (w_main->tool_avviato)
		switch (tool)
		{
		case T_CONNECTION:
		case T_POLYLINE:
		case T_POLYGON:
		case T_RECT:
		case T_ELLIPSE:
			{
                        float delta_x = mouse_move.x() - mouse_new_press.x();
                        float delta_y = mouse_move.y() - mouse_new_press.y();
                        if (y_axis_is_up)
                                delta_y = -delta_y;
                        w_main->sb_deltax->setText(QString("DX = %1").arg(from_001thou_to_string(delta_x, UM)));
			w_main->sb_deltay->setText(QString("DY = %1").arg(from_001thou_to_string(delta_y, UM)));
			//////////////////////////////////
			// Esempio di come si calcola la lunghezza e l'angolo
			// Potrebbe essere utile nel tool_righello
			// int length = sqrt((delta_x*delta_x + delta_y*delta_y));	//delta_x e delta_y devono essere float in sqrt
			// int angle = atan2(delta_y, delta_x) * 57.32; //delta_x e delta_y devono essere float in sqrt
			//////////////////////////////////
			}
			break;
		}
	else
		{
		w_main->sb_deltax->setText("");
		w_main->sb_deltay->setText("");
                }
}

myComponent	*myGraphicsView::find_comp(QString ident)
{
	foreach(myComponent *comp, all_comp)
                if (comp->ident == ident)
			return comp;
	
	return 0;
}

myGraphicsItem	*myGraphicsView::find_pin(int pin_number)
{
	foreach(myGraphicsItem *obj, scene->all_obj)
                if (obj->tipo == O_PIN)
                        if (obj->number == pin_number)
                                return obj;

	return 0;
}

myGraphicsItem	*myGraphicsView::find_pad(int pad_number)
{
	foreach(myGraphicsItem *obj, scene->all_obj)
                if (obj->tipo == O_PAD)
                        if (obj->number == pad_number)
                                return obj;

	return 0;
}

myComponent	*myGraphicsView::clone_find_comp(QString ident)
{
        foreach(myComponent *comp, cloned_comp)
                if (comp->ident == ident)
                        return comp;

        return 0;
}

myGraphicsItem	*myGraphicsView::clone_find_pin(int pin_number)
{
        foreach(myGraphicsItem *obj, cloned_obj)
                if (obj->tipo == O_PIN)
                        if (obj->number == pin_number)
                                return obj;

        return 0;
}

myGraphicsItem	*myGraphicsView::clone_find_pad(int pad_number)
{
        foreach(myGraphicsItem *obj, cloned_obj)
                if (obj->tipo == O_PAD)
                        if (obj->number == pad_number)
                                return obj;

        return 0;
}

node_data	*myGraphicsView::node_new()
{
	node_data *nd = new node_data();
	nd->connected_pin_or_pad_or_via = 0;
	nd->net_id = 0;
	nd->drag_or_move_active = FALSE;
	return nd;
}

void	myGraphicsView::node_recalculate(node_data *nd)
{
        nd->connected_pin_or_pad_or_via = 0;
	nd->connected_wires.clear();
	
	foreach(myGraphicsItem *obj, scene->all_obj)
		switch (obj->tipo)
		{
		case O_CONNECTION:
			if (obj->node_start == nd)
				nd->connected_wires.append((int) obj);
			else if (obj->node_end == nd)
				nd->connected_wires.append((int) obj);
			break;
		case O_PIN:
		case O_PAD:
		case O_VIA:
                case O_POWER:
		case O_PORT:
			if (obj->node_start == nd)
				nd->connected_pin_or_pad_or_via = (int) obj;
			break;
		}
}

bool	myGraphicsView::node_is_useless(node_data *nd)
{
	if (nd->connected_pin_or_pad_or_via == 0)
		if (nd->connected_wires.count() == 0)
			return TRUE;
	return FALSE;
}

int		myGraphicsView::page_size_index()
{
	for (int i = 0; i < STAMPA_FORMATI_COUNT;  i++)
		{
		if ((stampa_formati[i].larghezza == page_width) && (stampa_formati[i].altezza == page_height))
			return i;
		if ((stampa_formati[i].larghezza == page_height) && (stampa_formati[i].altezza == page_width))
			return i;
		}
	return 30;	//Di default restituisco QPrinter::Custom in mm
}

void	myGraphicsView::pan_up()
{
	QScrollBar *sb = verticalScrollBar(); 
	
	sb->setValue(sb->value() - sb->pageStep()/10);
	//Non riesco a usare l'istruzione translate(), mi sembra che non funziona

        mouseMoveEvent(&QMouseEvent(QEvent::MouseMove, mapFromGlobal(QCursor::pos()), Qt::NoButton, Qt::NoButton, Qt::NoModifier));	//La documentazione QT dice che per i MouseMoveEvent conviene usare Qt::NoButton
	//L'evento sopra serve per aggiornare quando faccio il move e mi muovo con le frecce o con la rotellina
}

void	myGraphicsView::pan_down()
{
	QScrollBar *sb = verticalScrollBar(); 
	
	sb->setValue(sb->value() + sb->pageStep()/10);
	//Non riesco a usare l'istruzione translate(), mi sembra che non funziona

        mouseMoveEvent(&QMouseEvent(QEvent::MouseMove, mapFromGlobal(QCursor::pos()), Qt::NoButton, Qt::NoButton, Qt::NoModifier));	//La documentazione QT dice che per i MouseMoveEvent conviene usare Qt::NoButton
	//L'evento sopra serve per aggiornare quando faccio il move e mi muovo con le frecce o con la rotellina
}

void	myGraphicsView::pan_left()
{
        QScrollBar *sb = horizontalScrollBar();
	
	sb->setValue(sb->value() - sb->pageStep()/10);
	//Non riesco a usare l'istruzione translate(), mi sembra che non funziona

        mouseMoveEvent(&QMouseEvent(QEvent::MouseMove, mapFromGlobal(QCursor::pos()), Qt::NoButton, Qt::NoButton, Qt::NoModifier));	//La documentazione QT dice che per i MouseMoveEvent conviene usare Qt::NoButton
	//L'evento sopra serve per aggiornare quando faccio il move e mi muovo con le frecce o con la rotellina
}

void	myGraphicsView::pan_right()
{
	QScrollBar *sb = horizontalScrollBar(); 
	
	sb->setValue(sb->value() + sb->pageStep()/10);
	//Non riesco a usare l'istruzione translate(), mi sembra che non funziona

        mouseMoveEvent(&QMouseEvent(QEvent::MouseMove, mapFromGlobal(QCursor::pos()), Qt::NoButton, Qt::NoButton, Qt::NoModifier));	//La documentazione QT dice che per i MouseMoveEvent conviene usare Qt::NoButton
	//L'evento sopra serve per aggiornare quando faccio il move e mi muovo con le frecce o con la rotellina
}

QPoint	myGraphicsView::rotation_point()
{
	//Se è selezionato solo un componente uso il suo anchor_point
	if (selected_obj.count() == 0)
		if (selected_comp.count() == 1)
			return selected_comp.first()->anchor_point;
	
	//Trovo il punto di rotazione del rettangolo di selezione
	QPoint center;
	QRect sel_rect = selection_bounding_rect(FALSE);

	center = sel_rect.center();

	//Trovo il punto più vicino alla griglia
        if (is_sch)
                return grid_point(center);
        else
                switch(pcb_grid_snap)
                {
                case 'N':	//None
                        return center;
                        break;
                case 'G':	//Grid
                case 'I':	//Intellisnap
                        return grid_point(center);
                        break;
                }

	return center;
}

void	myGraphicsView::select_all()
{
	//Commuto temporanemaente GV e GS usati nei vari set_selected
	myGraphicsView *GV_temp = GV;
	myGraphicsScene *GS_temp = GS;
	GV = this;
	GS = scene;
	
        foreach(myGraphicsItem *obj, scene->all_obj)	//Bisogna usare scene() e non GS
            {
            if (obj->comp == 0)
                obj->set_selected(TRUE);
            }
			
        foreach(myComponent *comp, all_comp)
            comp->set_selected(TRUE);

        selected_nodes.clear();
        foreach (node_data *node, all_nodes)
            selected_nodes.append(node);

        do_selection_rect_recalculate = TRUE;

        GV = GV_temp;
        GS = GS_temp;
}

void	myGraphicsView::select_clear()
{
        //Commuto temporanemaente GV e GS usati nei vari set_selected
        myGraphicsView *GV_temp = GV;
        myGraphicsScene *GS_temp = GS;
        GV = this;
        GS = scene;

        foreach(myGraphicsItem *obj, selected_obj)
                obj->set_selected(FALSE);
			
        selected_obj.clear();

        foreach(myComponent *comp, selected_comp)
                comp->set_selected(FALSE);
			
        selected_comp.clear();

        selected_nodes.clear();

	do_selection_rect_recalculate = TRUE;

	GV = GV_temp;
	GS = GS_temp;
}

void	myGraphicsView::selection_add()
{
        foreach (myGraphicsItem *obj, w_main->proposed_selection_obj)
                obj->set_selected(!obj->is_selected);

	foreach (myComponent *comp, w_main->proposed_selection_comp)
		comp->set_selected(!comp->is_selected);	
			
        selection_bounding_rect(TRUE);	//Ricalcolo il boundingRect visto che la selezione è cambiata
}

void	myGraphicsView::selection_from_point(QPoint point)
{
        bool    old_pcb_pad_via_filled = pcb_pad_via_filled;
        pcb_pad_via_filled = TRUE;

        do_selection_rect_recalculate = TRUE;
	qreal pixel4 = 4.0 / matrix().m11();	//Uso 7 pixel di errore per cui il centro è a 4 pixel

	w_main->proposed_selection_obj.clear();
	highlight_clear();	//Serve ...

	foreach(myGraphicsItem *obj, scene->all_obj)
                if (GV->layer_settings[obj->layer_from].visible)
                        if (obj->boundingRect().top() - pixel4 <= point.y())
                                if (obj->boundingRect().bottom() + pixel4 >= point.y())
                                        if (obj->boundingRect().left() - pixel4 <= point.x())
                                                if (obj->boundingRect().right() + pixel4 >= point.x())
                                                        if (obj->hitTest(point))	//Eseguo l'hittest
                                                                w_main->proposed_selection_insert_obj(obj);

        pcb_pad_via_filled = old_pcb_pad_via_filled;
}

int     myGraphicsView::active_net_id_from_point(QPoint point)
{
        foreach(myGraphicsItem *obj, scene->all_obj)
                if ((obj->layer_from <= layer_selected) && (obj->layer_to >= layer_selected))
                        if (obj->boundingRect().contains(point))
                                if (obj->hitTest(point))	//Eseguo l'hittest
                                        return(obj->net_id);

        return 0;
}

void	myGraphicsView::selection_from_rect(QRect rect)
{
        do_selection_rect_recalculate = TRUE;

	myGraphicsItem	*obj;
	QList<myGraphicsItem *> *ao = &scene->all_obj;
	int ao_size = ao->size();

	for (int i = 0; i < ao_size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
		{
		obj = ao->at(i);
		if (obj->comp == 0)
                        if (obj->is_showed())
                                if (myQRect(mouse_new_press, mouse_release).contains(obj->boundingRect()))
                                        obj->set_selected(!obj->is_selected);
		}

	foreach(myComponent *comp, all_comp)
                if (comp->is_showed())
                        if (myQRect(mouse_new_press, mouse_release).contains(comp->bounding_rect()))
                                comp->set_selected(!comp->is_selected);
}

void	myGraphicsView::selection_nodes()
{
	selected_nodes.clear();
	
	foreach(myGraphicsItem *obj, selected_obj)
		switch (obj->tipo)
		{
		case O_CONNECTION:
			if (!selected_nodes.contains(obj->node_start))
				selected_nodes.append(obj->node_start);
			if (!selected_nodes.contains(obj->node_end))
				selected_nodes.append(obj->node_end);
			break;
		case O_PIN:
		case O_PAD:
		case O_VIA:
                case O_POWER:
		case O_PORT:
			if (!selected_nodes.contains(obj->node_start))
				selected_nodes.append(obj->node_start);
			break;
		}
	foreach(myComponent *comp, selected_comp)
		foreach(myGraphicsItem *obj, comp->all_obj)
			switch (obj->tipo)
			{
			case O_CONNECTION:
				if (!selected_nodes.contains(obj->node_start))
					selected_nodes.append(obj->node_start);
				if (!selected_nodes.contains(obj->node_end))
					selected_nodes.append(obj->node_end);
				break;
			case O_PIN:
			case O_PAD:
			case O_VIA:
                        case O_POWER:
			case O_PORT:
				if (!selected_nodes.contains(obj->node_start))
					selected_nodes.append(obj->node_start);
				break;
			}
}

QRect	myGraphicsView::selection_bounding_rect(bool recalculate)
{
	static QRect sel_rect = QRect(0,0,0,0);

	if (recalculate)
		{
		sel_rect = QRect(0,0,0,0);
		foreach(myGraphicsItem *item, selected_obj)
			sel_rect = sel_rect.unite(item->boundingRect());
		foreach(myComponent *comp, selected_comp)
			sel_rect = sel_rect.unite(comp->bounding_rect());
                selection_nodes();
                do_selection_rect_recalculate = FALSE;
                w_main->change_selection();
                return sel_rect;
		}
	else
		return sel_rect;
}	

QRect	myGraphicsView::selection_bounding_rect_in_pixel(QRect sel_rect)
{
	QRect r;
	if (!sel_rect.isNull())
		{
		QPoint center = mapFromScene(sel_rect.center());
		int cx = center.x();
		int cy = center.y();
		int rx = (sel_rect.width()/2) * matrix().m11();
		if (rx < 7)
			rx = 7;
		int ry = (sel_rect.height()/2) * matrix().m11();
		if (ry < 7)
			ry = 7;
		r.setLeft(cx - rx);
		r.setRight(cx + rx);
		r.setTop(cy + ry);
		r.setBottom(cy - ry);
		}
	return r;	
}	

void	myGraphicsView::tool_change(char new_tool)
{
        int old_grid_size = grid_size();

        if (tool == new_tool)
		return;

        //Se è cambiata la griglia allora...
        if (old_grid_size != grid_size())
                {
                setUpdatesEnabled(FALSE);
                setUpdatesEnabled(TRUE);
                }

        if ((tool == T_ORIGIN) || (new_tool == T_ORIGIN))
                {
                setUpdatesEnabled(FALSE);
                setUpdatesEnabled(TRUE);
                }

        if ((new_tool == T_EDIT_NODE) || (tool == T_EDIT_NODE) || (new_tool == T_EDIT_COMPONENT) || (tool == T_EDIT_COMPONENT))
                if (wand_connection || wand_ident || wand_value || wand_pol)
                        {
                        wand_connection = 0;
                        wand_connection_node = 0;
                        wand_ident = 0;
                        wand_pad = 0;
                        wand_value = 0;
                        wand_pol = 0;
                        setUpdatesEnabled(FALSE);
                        select_clear();
                        setUpdatesEnabled(TRUE);
                        }

        tool_stop();
	
	tool = new_tool;		
	
        mouse_click_num = 0;

	cursor_change();
}

void	myGraphicsView::tool_change_to_default()
{
	if (is_sch)
		tool_change(w_main->tool_sch);
	else
		tool_change(w_main->tool_pcb);
	
	switch (tool)
	{
	case T_SELECT:
		w_main->actions_selectAct->setChecked(TRUE);
		break;
        case T_EDIT_NODE:
                w_main->actions_edit_nodeAct->setChecked(TRUE);
		break;
	case T_ZOOM:
		w_main->actions_zoomAct->setChecked(TRUE);
		break;
	case T_HAND:
		w_main->actions_handAct->setChecked(TRUE);
		break;
	case T_CONNECTION:
		w_main->actions_connectionAct->setChecked(TRUE);
		break;
	case T_POLYLINE:
		w_main->actions_polylineAct->setChecked(TRUE);
		break;
	case T_POLYGON:
		w_main->actions_polygonAct->setChecked(TRUE);
		break;
	case T_ELLIPSE:
		w_main->actions_circleAct->setChecked(TRUE);
		break;
	case T_RECT:
		w_main->actions_rectangleAct->setChecked(TRUE);
		break;	
	case T_TEXT:
		w_main->actions_textAct->setChecked(TRUE);
		break;
	case T_PIN:
		w_main->actions_pinAct->setChecked(TRUE);
		break;
        case T_PADVIA:
                w_main->actions_padviaAct->setChecked(TRUE);
		break;
	case T_PORT:
		w_main->actions_portAct->setChecked(TRUE);
		break;
        case T_ORIGIN:
                w_main->actions_setOriginAct->setChecked(TRUE);
                break;
        case T_RULER:
                w_main->actions_rulerAct->setChecked(TRUE);
                break;
        case T_ADD_POWER:
                w_main->actions_add_powerAct->setChecked(TRUE);
                break;
        case T_EDIT_COMPONENT:
                w_main->actions_edit_componentAct->setChecked(TRUE);
                break;
	}

}

void	myGraphicsView::tool_stop()
{
	if (w_main->tool_avviato)
		{
		w_main->tool_avviato = FALSE;
                mouse_click_num = 0;
		setUpdatesEnabled(FALSE);
		switch (tool)
		{
		case T_SELECT:
			//Cancello il move che era in esecuzione
			drag_or_move_setup('R');
			break;
		case T_POLYLINE:
		case T_POLYGON:
                        w_main->line_exit();
			break;
		case T_CONNECTION:
			dynamic_ratsnest_active = FALSE;
			do_ratsnest_recalculate = TRUE;
			GV->ur.endMacro();
                        highlight_active = FALSE;   //
                        break;
		case T_ADD:
			{
                        drag_or_move_setup('R');
			if (is_sch)
				w_main->tool_sch = T_SELECT;
			else
				w_main->tool_pcb = T_SELECT;
			tool_change_to_default();
			}
			break;
                case T_RULER:
                        info_label->setHidden(TRUE);
                        break;
		}
		setUpdatesEnabled(TRUE);
	}
	w_main->sb->clearMessage();
}

bool myGraphicsView::zoom_scale(qreal scale_factor)
{
	//QRectF visible_rect = QRectF(mapToScene(rect().topLeft()), mapToScene(rect().bottomRight()));

	if (is_sch)
		{
		if ((matrix().m11() * scale_factor) > 0.02)
			scale_factor = 0.02 / matrix().m11();	//1 pixel equivale a 0.5 thou
		}
	else
		{
		if ((matrix().m11() * scale_factor) > 1.0)
			scale_factor = 1.0 / matrix().m11();	//1 pixel equivale a 1/100 di thou
		}
	
	QMatrix	old_matrix = matrix();
	
        scale(scale_factor, scale_factor);

	QRect	viewport_rect = viewport()->rect();
	QRectF	new_visible_rect = QRectF(mapToScene(viewport_rect.topLeft()), mapToScene(viewport_rect.bottomRight()));

	bool	new_scale_is_ok = sceneRect().contains(new_visible_rect);

	if (!new_scale_is_ok)
		setMatrix(old_matrix);

	return	new_scale_is_ok;
}

void myGraphicsView::zoom_in(QGraphicsView::ViewportAnchor mode)
{
	// mode può essere QGraphicsView::AnchorViewCenter oppure QGraphicsView::AnchorUnderMouse
	setTransformationAnchor(mode);

        if (zoom_scale(1.25))	//1.25 = 1/0.8
		{
		w_main->Z->zoom_show_scale();
                mouseMoveEvent(&QMouseEvent(QEvent::MouseMove, mapFromGlobal(QCursor::pos()), Qt::NoButton, Qt::NoButton, Qt::NoModifier));	//La documentazione QT dice che per i MouseMoveEvent conviene usare Qt::NoButton
		//L'evento sopra serve per aggiornare quando faccio il move e mi muovo con le frecce o con la rotellina
		}
}

void myGraphicsView::zoom_out(QGraphicsView::ViewportAnchor mode)
{
	// mode può essere QGraphicsView::AnchorViewCenter oppure QGraphicsView::AnchorUnderMouse
	setTransformationAnchor(mode);
	
	if (zoom_scale(0.8))	//0.8 = 1/1.25
		{
		w_main->Z->zoom_show_scale();
                mouseMoveEvent(&QMouseEvent(QEvent::MouseMove, mapFromGlobal(QCursor::pos()), Qt::NoButton, Qt::NoButton, Qt::NoModifier));	//La documentazione QT dice che per i MouseMoveEvent conviene usare Qt::NoButton
		//L'evento sopra serve per aggiornare quando faccio il move e mi muovo con le frecce o con la rotellina
		}
}



void myGraphicsView::zoom_rect(QRect rect)
{
	setUpdatesEnabled(FALSE);	
	fitInView(rect, Qt::KeepAspectRatio);
	setTransformationAnchor(QGraphicsView::AnchorViewCenter);
	zoom_scale(1.0);	//Serve perchè dentro zoom_scale faccio un check sulla validità della nuova scala e nel caso mi riporto a un livello di zoom corretto
	setUpdatesEnabled(TRUE);

	if (!is_pixmapoutput)
		w_main->Z->zoom_show_scale();
}

void myGraphicsView::zoom_rect_with_margin(QRect rect)
{
	//Mi prendo un margine del 5% da tutti i lati
	QPoint margine = QPoint(rect.width() * 0.05, rect.height() * 0.05);
	rect.setTopLeft(rect.topLeft() - margine);
	rect.setBottomRight(rect.bottomRight() + margine);	
	
	setUpdatesEnabled(FALSE);	
	fitInView(rect, Qt::KeepAspectRatio);
	setTransformationAnchor(QGraphicsView::AnchorViewCenter);
	zoom_scale(1.0);	//Serve perchè dentro zoom_scale faccio un check sulla validità della nuova scala e nel caso mi riporto a un livello di zoom corretto
	setUpdatesEnabled(TRUE);

	if (!is_pixmapoutput)
		w_main->Z->zoom_show_scale();
}

void myGraphicsView::zoom_to_page()
{
        QRect all_rect = QRect(-page_width/2, -page_height/2, page_width, page_height);
	
	zoom_rect_with_margin(all_rect);
	/*//Mi prendo un margine del 5% da tutti i lati
	QPoint margine = QPoint(all_rect.width() * 0.05, all_rect.height() * 0.05);
	all_rect.setTopLeft(all_rect.topLeft() - margine);
	all_rect.setBottomRight(all_rect.bottomRight() + margine);	

	fitInView(all_rect, Qt::KeepAspectRatio);

	if (!is_pixmapoutput)
		w_main->Z->zoom_show_scale();*/
}

void myGraphicsView::zoom_to_page_width()
{
        QRect all_rect = QRect(-page_width/2, -10, page_width, 20);
	
	zoom_rect_with_margin(all_rect);
	/*//Mi prendo un margine del 5% da tutti i lati
	QPoint margine = QPoint(all_rect.width() * 0.05, all_rect.height() * 0.05);
	all_rect.setTopLeft(all_rect.topLeft() - margine);
	all_rect.setBottomRight(all_rect.bottomRight() + margine);	
	
	fitInView(all_rect, Qt::KeepAspectRatio);
	w_main->Z->zoom_show_scale();*/
}

void myGraphicsView::zoom_to_page_height()
{
        QRect all_rect = QRect(-10, -page_height/2, 20, page_height);
	
	zoom_rect_with_margin(all_rect);
	/*//Mi prendo un margine del 5% da tutti i lati
	QPoint margine = QPoint(all_rect.width() * 0.05, all_rect.height() * 0.05);
	all_rect.setTopLeft(all_rect.topLeft() - margine);
	all_rect.setBottomRight(all_rect.bottomRight() + margine);	
	
	fitInView(all_rect, Qt::KeepAspectRatio);
	w_main->Z->zoom_show_scale();*/
}

void myGraphicsView::zoom_to_default()
{
	//valori di zoom applicati quando si crea un nuovo progetto o un nuovo simbolo
	if ((file_type == FT_PROJECT) && is_sch)
		zoom_to_page();
	else
		{
		centerOn(0,0);
		float scale_factor = 0.05 / matrix().m11();	
		zoom_scale(scale_factor);			//Imposto una scala per cui 1 pixel = 20 micrometri
											//Su uno schermo largo 1280 pixel questo significa 25,6 mm di larghezza
		w_main->Z->zoom_show_scale();
		}
}

void myGraphicsView::zoom_to_all()
{
	QRect all_rect = QRect(0,0,0,0);
	foreach(myGraphicsItem *obj, GS->all_obj)
		all_rect = all_rect.unite(obj->boundingRect());
	
	if (all_rect.isNull()) 
		zoom_to_default();
	else
		{
		zoom_rect_with_margin(all_rect);

		/*//Mi prendo un margine del 5% da tutti i lati
		QPoint margine = QPoint(all_rect.width() * 0.05, all_rect.height() * 0.05);
		all_rect.setTopLeft(all_rect.topLeft() - margine);
		all_rect.setBottomRight(all_rect.bottomRight() + margine);	
		
		fitInView(all_rect, Qt::KeepAspectRatio);

		if (!is_pixmapoutput)
			w_main->Z->zoom_show_scale();*/
		}
}

void myGraphicsView::zoom_to_selected()
{
	QRect all_rect = selection_bounding_rect(do_selection_rect_recalculate);
	
	if (all_rect.isNull()) 
		zoom_to_all();
	else
		{
		zoom_rect_with_margin(all_rect);

		/*//Mi prendo un margine del 5% da tutti i lati
		QPoint margine = QPoint(all_rect.width() * 0.05, all_rect.height() * 0.05);
		all_rect.setTopLeft(all_rect.topLeft() - margine);
		all_rect.setBottomRight(all_rect.bottomRight() + margine);	
	
		fitInView(all_rect, Qt::KeepAspectRatio);
		w_main->Z->zoom_show_scale();*/
		}
}

void myGraphicsView::wheelEvent(QWheelEvent *event)
{
        if (event->delta() > 0)
		zoom_in(QGraphicsView::AnchorUnderMouse);
	else
		zoom_out(QGraphicsView::AnchorUnderMouse);
}

void	myGraphicsView::mouse_press_info(QPoint point)
{
        mouse_click_num++;

        mouse_old_press = mouse_new_press;
        mouse_old_press_node = mouse_new_press_node;
        if (mouse_click_num == 1)
                mouse_snap(point, &mouse_new_press, &mouse_new_press_node);
        else
                {
                mouse_old_press = mouse_new_press;
                mouse_old_press_node = mouse_new_press_node;
                mouse_new_press = mouse_move;
                mouse_new_press_node = mouse_move_node;
                }
}

void	myGraphicsView::mouse_move_info(QPoint point)
{
        switch (tool)
        {
        case T_SELECT:
        case T_EDIT_NODE:
        case T_ZOOM:
        case T_HAND:
                mouse_move = point;
                mouse_move_node = 0;
                break;
        case T_CONNECTION:
                if (!key_ctrl)
                        {
                        if (is_sch)
                                {
                                mouse_position = grid_point(point);
                                point = constraint_90(mouse_new_press, point);
                                }
                        else
                                point = constraint_45_90(mouse_new_press, point);
                        mouse_snap(point, &mouse_move, &mouse_move_node);
                        }
                else
                        {
                        mouse_snap(point, &mouse_move, &mouse_move_node);
                        if (is_sch)
                                {
                                mouse_position = mouse_move;
                                }
                        }
                break;
        case T_POLYLINE:
        case T_POLYGON:
                if (!key_ctrl)
                        point = constraint_45_90(mouse_new_press, point);
                mouse_snap(point, &mouse_move, &mouse_move_node);
                break;
        case T_RECT:
        case T_ELLIPSE:
                if (!key_ctrl)
                        point = constraint_45(mouse_new_press, point);
                mouse_snap(point, &mouse_move, &mouse_move_node);
                break;
        case T_RULER:
        case T_ORIGIN:
        case T_TEXT:
        case T_EDIT_COMPONENT:
        case T_PIN:
        case T_PADVIA:
        case T_ADD_POWER:
        case T_PORT:
        case T_ADD:
                mouse_snap(point, &mouse_move, &mouse_move_node);
                break;
        }
}

void	myGraphicsView::mouse_release_info(QPoint point)
{
        mouse_release = mouse_move;
        mouse_release_node = mouse_move_node;
}

void    myGraphicsView::mouse_snap(QPoint point, QPoint *snap_point, node_data **snap_node)
{
        char layer_from;
        char layer_to;

        bool is_all_layers;
        switch (tool)
        {
        case T_EDIT_COMPONENT:
        case T_RULER:
        case T_PADVIA:
        case T_ORIGIN:
                is_all_layers = TRUE;
                break;
        case T_EDIT_NODE:
                is_all_layers = edit_node_is_all_layers();
                break;
        default:
                is_all_layers = FALSE;
                break;
        }

        if (is_sch)
                {
                layer_from = L_SCHWire;
                layer_to = L_SCHWire;
                }
        else
                {
                if (is_all_layers)
                        {
                        layer_from = L_PCBCopper1;
                        layer_to = L_PCBCopper16;
                        }
                else
                        {
                        layer_from = layer_selected;
                        layer_to = layer_selected;
                        }
                }


        switch(grid_snap())
        {
        case 'N':	//None
                {
                QPoint p = point;
                foreach(node_data *nd, all_nodes)
                        if (nd->point == p)
                                {
                                *snap_point = p;
                                *snap_node = nd;
                                return;
                                }
                *snap_point = p;
                *snap_node = 0;
                }
                break;
        case 'G':	//Grid
                {
                QPoint p = grid_point(point);
                foreach(node_data *nd, all_nodes)
                        if (nd->point == p)
                                {
                                *snap_point = p;
                                *snap_node = nd;
                                return;
                                }
                *snap_point = p;
                *snap_node = 0;
                }
                break;
        case 'I':	//ATTENZIONE: l'Intellisnap e attivo solo quando uso T_CONNECTION, T_EDIT_NODE, T_PADVIA, T_ORIGIN, T_RULER (vedere funzione grid_snap()) altrimenti non ha senso usarlo
                {
                QPoint p = grid_point(point);
                if (!is_sch)
                        {
                        int px = point.x();
                        int py = point.y();
                        qint64	dx;             			//Bisogna usare qint64 e non quint64 (i 64 bit sono necessari)
                        qint64	dy;             			//Bisogna usare qint64 e non quint64 (i 64 bit sono necessari)
                        qint64  node_radius;                             //Bisogna usare qint64 e non quint64 (i 64 bit sono necessari)
                        foreach(node_data *nd, all_nodes)
                                if (node_layer_intersect(nd, layer_from, layer_to))
                                        {
                                        node_radius = 0;                //Serve...
                                        if (nd->connected_pin_or_pad_or_via)
                                                {
                                                myGraphicsItem* pad_or_via = (myGraphicsItem*) nd->connected_pin_or_pad_or_via;
                                                if (pad_or_via->width < pad_or_via->height)
                                                        node_radius = pad_or_via->width/2;
                                                else
                                                        node_radius = pad_or_via->height/2;
                                                }
                                        foreach (int w, nd->connected_wires)
                                                {
                                                myGraphicsItem* wire = (myGraphicsItem*) w;
                                                if (wire->layer_intersect(layer_from, layer_to))
                                                        if (node_radius < wire->pen_width/2)
                                                                node_radius = wire->pen_width/2;
                                                }
                                        dx = px - nd->point.x();
                                        dy = py - nd->point.y();
                                        if ((dx*dx + dy*dy) <= (node_radius * node_radius))
                                                {
                                                *snap_point = nd->point;
                                                *snap_node = nd;
                                                return;
                                                }
                                        }
                        }
                else
                        foreach(node_data *nd, all_nodes)
                                if (nd->point == p)
                                        {
                                        *snap_point = p;
                                        *snap_node = nd;
                                        return;
                                        }

                *snap_point = p;
                *snap_node = 0;
                }
                break;
        }
}

void	myGraphicsView::node_split(node_data *nd)
{
        if (nd->connected_pin_or_pad_or_via)
                ur.push(new split_node(wand_connection_node));
        else if (nd->connected_wires.count() >= 3)
                ur.push(new split_node(wand_connection_node));
}

void	myGraphicsView::node_join(node_data *nd)
{
        //Cerco un nodo nelle vicinanze
        foreach(node_data *node, all_nodes)
                if (node != nd)
                        if (node->point == nd->point)   //penso conviene fare l'aggancio solo quando i punti coincidono perfettamente
                                {
                                bool join = TRUE;
                                if (nd->connected_pin_or_pad_or_via && node->connected_pin_or_pad_or_via)
                                        join = FALSE;
                                if (!node_layer_intersect(node, node_layer_from(nd), node_layer_to(nd)))
                                        join = FALSE;
                                if (join)
                                        {
                                        ur.push(new join_node(nd, node));
                                        break;
                                        }
                                }
}

int	myGraphicsView::style_pen_width()
{
	if (is_sch)
                switch (layer_selected)
		{
		case L_SCHWire:
                        return style_sch_pen_width_wire;
		case L_SCHNormal:
                        return style_sch_pen_width_normal;
		case L_SCHFrame:
                        return style_sch_pen_width_frame;
		}
	else
                switch (layer_selected)
		{
		case L_PCBBoard:
                        return style_pcb_pen_width_board;
		case L_PCBPasteTop:
		case L_PCBPasteBottom:
                        return style_pcb_pen_width_paste;
		case L_PCBSilkTop:
		case L_PCBSilkBottom:
                        return style_pcb_pen_width_silk;;
		case L_PCBMaskTop:
		case L_PCBMaskBottom:
                        return style_pcb_pen_width_mask;
                case L_PCBCopper1:
                case L_PCBCopper16:
                        return style_pcb_pen_width_copper;
		case L_PCBHelp:
                        return style_pcb_pen_width_help;
		}
}

bool            myGraphicsView::style_fill_polygon()
{
        if (is_sch)
                return style_sch_fill_polygon;
        else
                return style_pcb_fill_polygon;
}

char		myGraphicsView::style_font_height()
{
        if (is_sch)
                return style_sch_font_height;
        else
                return style_pcb_font_height;
}

bool		myGraphicsView::style_font_bold()
{
        if (is_sch)
                return style_sch_font_bold;
        else
                return style_pcb_font_bold;
}

bool		myGraphicsView::style_font_italic()
{
        if (is_sch)
                return style_sch_font_bold;
        else
                return style_pcb_font_bold;
}

QPoint  myGraphicsView::coordinate_transform_to_user(QPoint p)
{
        if (y_axis_is_up)
                return QPoint(p.x() - origin.x(), -p.y() + origin.y());
        else
                return QPoint(p.x() - origin.x(), p.y() - origin.y());
}

QPoint  myGraphicsView::coordinate_transform_to_internal(QPoint p)
{
        if (y_axis_is_up)
                return QPoint(p.x() - GV->origin.x(), -p.y() + - origin.y());
        else
                return QPoint(p.x() - GV->origin.x(), p.y() - origin.y());
}

bool    myGraphicsView::edit_node_is_all_layers()
{
        if (is_sch)
                return TRUE;
        else
                return edit_node_in_pcb_is_all_layers;
}

void    myGraphicsView::schcomp_or_pcbcomp_update()
{
        //Per ora questa funzione non è utilizzata (è usata la sua analoga schcomp_or_pcbcomp_update_with_new_data())
        //ma credo che potrebbe rivelarsi utile
        schcomp_or_pcbcomp->all_obj.clear();
        foreach (myGraphicsItem *obj, GS->all_obj)
                schcomp_or_pcbcomp->all_obj.append(obj);
}
