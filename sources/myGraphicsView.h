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

#ifndef MY_GRAPHICSVIEW_H
#define MY_GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QTimer>
#include <QBitmap>
#include <QScrollBar>
#include <QIcon>
#include <QPainter>
#include <QUndoStack>
#include <QFont>
#include <QMessageBox>
#include <QMenuBar>
#include <QDate>
#include "tipi_dato.h"
#include "win_main.h"
#include "utilities.h"
#include "myGraphicsScene.h"
#include "myGraphicsItem.h"
#include "myComponent.h"
#include "undo_redo.h"
#include "win_object_prop.h"
#include "win_component_prop.h"

class myGraphicsView : public QGraphicsView
{	
	Q_OBJECT	//Necessario per gestire Signals e Slots
public:
    myGraphicsView(QGraphicsScene * scene_input, QWidget *parent, bool is_sch_input, QString name_input);
	~myGraphicsView()
	{clear();}

        QString			name;
	
        bool			is_sch;
        bool			is_pixmapoutput;

        QPoint                  origin;

        int			page_width;
        int			page_height;

        int			page_size_index();

	lay_settings		layer_settings[LAYER_DATA_COUNT];	//Ho delle righe inutili, ma la gestione è semplice e veloce
        unsigned char			layer_selected;

	int		style_pen_width();
        bool            style_fill_polygon();
        char		style_font_height();
        bool		style_font_bold();
        bool		style_font_italic();

        qreal           grid_size();
        char            grid_snap();

        QLabel          *info_label;


	QIcon	layer_square(int row, int lato);

	void	highlight_clear();
        void	highlight_active_net_id();

	bool	zoom_scale(qreal scale_factor);		
	void	zoom_to_default();
	void	zoom_to_page();
	void	zoom_to_page_width();
	void	zoom_to_page_height();
	void	zoom_to_all();
	void	zoom_to_selected();
	void	zoom_in(QGraphicsView::ViewportAnchor mode);
	void	zoom_out(QGraphicsView::ViewportAnchor mode);
	void	zoom_rect(QRect rect);
	void	zoom_rect_with_margin(QRect rect);

        QPoint	grid_point(QPoint pixel);
	QPoint	grid_delta(QPoint punto);
        QPoint	constraint_45_90(QPoint prec_point, QPoint new_point);
        QPoint	constraint_90(QPoint prec_point, QPoint new_point);
        QPoint	constraint_45(QPoint prec_point, QPoint new_point);

	void	pan_up();
	void	pan_down();
	void	pan_left();
	void	pan_right();

	QPoint	rotation_point();

	QCursor	cursor_Z;
	void	cursor_change();
	void	cursor_setup_Z();

	char	tool;
	void	tool_change(char new_tool);
	void	tool_change_to_default();
	void	tool_stop();

	bool	inserting_item_is_visible;
	bool	inserting_item_is_visible_anyway;

	bool	do_ratsnest_recalculate;	//Conviene averlo qui e non dentro win_main perchè così posso settarlo FALSE dopo ogni operazione di delete o move senza preoccuparmi se GV è un PCB oppure no
	bool	do_selection_rect_recalculate;
	bool	do_selection_nodes;

	void	selection_from_point(QPoint point);
	void	selection_from_rect(QRect rect);
	void	selection_add();
	QRect	selection_bounding_rect(bool recalculate);
	QRect	selection_bounding_rect_in_pixel(QRect sel_rect);	//Tiene conto anche della distanza minima tra gli 8 quadratini di selezione
	void	selection_nodes();

        int     active_net_id_from_point(QPoint point);

	bool	key_shift;
	bool	key_ctrl;
        bool	key_alt;

        bool    midbutton_pan_on;
        QPoint  midbutton_pan_start_point;
        int     midbutton_pan_start_vsb;
        int     midbutton_pan_start_hsb;

	QUndoStack	ur;

        myComponent		*schcomp_or_pcbcomp;
        void                    schcomp_or_pcbcomp_update();
	QList<myComponent *>	all_comp;
        QList<node_data *>	all_nodes;
	QList<myGraphicsItem *> selected_obj;
	QList<myComponent *>	selected_comp;
        QList<node_data *>	selected_nodes;

        void	select_all();
        void	select_clear();

	void	clear();

        void    check_data_integrity();

        myComponent	*find_comp(QString ident);
        myGraphicsItem	*find_pin(int pin_number);
        myGraphicsItem	*find_pad(int pad_number);
        myComponent	*clone_find_comp(QString ident);
        myGraphicsItem	*clone_find_pin(int pin_number);
        myGraphicsItem	*clone_find_pad(int pad_number);

	void		drag_or_move_setup(char mode);
	void		drag_or_move_execute(char mode);
	
        void            draw_frame(QPainter *painter, QMatrix *m);
        void            draw_pad_and_via_gerber_view(QPainter *painter, QMatrix *m, bool for_screen, unsigned char layer);
        void            draw_pad_and_via_draft_view(QPainter *painter, QMatrix *m, bool for_screen);
        void            draw_hole(QPainter *painter, QMatrix *m);
        void            draw_junction(QPainter *painter, QMatrix *m);
	
	myGraphicsScene	*scene;

        node_data       *clone_node(node_data *node);
        myGraphicsItem  *clone_obj(myGraphicsItem *obj, int comp);
        myComponent     *clone_comp(myComponent *comp);
        void            clone_from_selection();    //Prendo dalla selezione
        void            clone_to_selection();      //Porto nella selezione
        void            clone_move(QPoint delta, bool clear_prev_delta);
        void            clone_to_clipboard();
        void            clone_from_clipboard();
        QPoint          clone_center();
        void            clone_ident_pin_pad_recalculate();

	node_data	*node_new();
	void		node_recalculate(node_data *nd);
	bool		node_is_useless(node_data *nd);
        void		node_join(node_data *nd);
        void		node_split(node_data *nd);

        bool            edit_node_is_all_layers();

        myGraphicsItem	*wand_connection;
        node_data	*wand_connection_node;
        myGraphicsItem	*wand_ident;
        myGraphicsItem	*wand_value;
        myGraphicsItem	*wand_pad;
        myGraphicsItem	*wand_pol;
        bool            wand_pol_is_vertex;
        int             wand_pol_index;
        QPoint          wand_cursor_pos;

        int             active_net_id;
	
	myGraphicsItem	*new_connection;	
	myGraphicsItem	*new_polyline;	
	myGraphicsItem	*new_polygon;	
	myGraphicsItem	*new_rect_or_ellipse;

        myGraphicsItem	*frame_organization;
        myGraphicsItem	*frame_title;
        myGraphicsItem	*frame_doc_num;
        myGraphicsItem	*frame_rev;
        myGraphicsItem	*frame_date;
        myGraphicsItem	*frame_page;

        QPoint  coordinate_transform_to_user(QPoint p);
        QPoint  coordinate_transform_to_internal(QPoint p);

        void    mouse_press_info(QPoint point);
        void    mouse_move_info(QPoint point);
        void    mouse_release_info(QPoint point);
        void    mouse_snap(QPoint point, QPoint *snap_point, node_data **snap_node);
	
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
        void drawBackground(QPainter *painter, const QRectF &rect);
	void drawForeground(QPainter *painter, const QRectF &rect);
	void wheelEvent(QWheelEvent *event);
};

#endif
