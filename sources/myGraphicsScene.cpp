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

#include "myGraphicsScene.h"
#include "myGraphicsView.h"

extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

extern	win_main    *w_main;
extern	char        file_type;

extern	QPoint	drag_or_move_delta;

extern	lay_data	layer_data[LAYER_DATA_COUNT];
extern	lay_data	workspace_data[WORKSPACE_DATA_COUNT];
extern	QPen	pen_layer;
extern	QBrush	brush_layer;

extern  QList<myGraphicsItem *>     cloned_obj;
extern  QList<myComponent *>        cloned_comp;
extern  QList<node_data *>          cloned_nodes;

extern	QColor		sel_color;

extern	bool		highlight_active;

extern  bool		pcb_pad_via_filled;

extern  QImage          composition_mode_image;

myGraphicsScene::myGraphicsScene()
        : QGraphicsScene()
{
        setItemIndexMethod(QGraphicsScene::NoIndex);	//Ho sperimentato che questo algoritmo è più conveniente
}

void myGraphicsScene::clear()
{
        //Elimino tutti gli oggetti contenuti nella scena
	foreach(myGraphicsItem *obj, all_obj)
                delete obj;
        all_obj.clear();
}

void myGraphicsScene::add_item(myGraphicsItem *obj)
{
	//Inserisco l'oggetto nella giusta posizione
	for (int i = 0; i < all_obj.count(); i++)
		{
		myGraphicsItem * list_obj = all_obj.at(i);
                if (list_obj->layer_from < obj->layer_from)
			{
			all_obj.insert(i, obj);
			return;
			}
		}
	all_obj.append(obj);
}

void myGraphicsScene::remove_item(myGraphicsItem *obj)
{
	all_obj.removeAll(obj);
}

void myGraphicsScene::layer_index_update()
{
        for (int i = 0; i < LAYER_DATA_COUNT; ++i)
                {
                layer_index_start[i] = -1;
                layer_index_end[i] = -2;
                }

        int size = all_obj.size();
        myGraphicsItem *obj;
        for (int i = 0; i < size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
                {
                obj = all_obj.at(i);		//Nelle QList l'operatore at() è il più veloce
                if ((obj->tipo != O_PAD) && (obj->tipo != O_VIA))
                        {
                        if (layer_index_start[obj->layer_from] < 0)
                                layer_index_start[obj->layer_from] = i;
                        layer_index_end[obj->layer_from] = i;
                        }
                }
}

void myGraphicsScene::draw_layer(QPainter *painter, QMatrix *m, QRect visible_rect, bool for_screen, unsigned char layer)
{
        if (pcb_pad_via_filled)
                GV->draw_pad_and_via_gerber_view(painter, m, for_screen, layer);

        QColor color = layer_data[layer].color;

        pen_layer.setColor(color);
        brush_layer.setColor(color);

        myGraphicsItem *obj;
        for (int i = layer_index_start[layer]; i <= layer_index_end[layer]; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
                {
                obj = all_obj.at(i);		//Nelle QList l'operatore at() è il più veloce
                if ((obj->tipo != O_PAD) && (obj->tipo != O_VIA))
                        if (layer == obj->layer_from)
                                {
                                if (!for_screen || !obj->is_selected)
                                    {
                                    if (visible_rect.intersects(obj->boundingRect()))
                                            {
                                            if (GV->is_sch)
                                                    obj->paint(painter, m);
                                            else
                                                    {
                                                    if (for_screen && highlight_active && obj->highlight_active)
                                                        {
                                                        pen_layer.setColor(color.lighter(150));
                                                        brush_layer.setColor(color.lighter(150));
                                                        obj->paint(painter, m);
                                                        pen_layer.setColor(color);
                                                        brush_layer.setColor(color);
                                                        }
                                                    else
                                                        {
                                                        obj->paint(painter, m);
                                                        }
                                                    }
                                            }
                                    }
                                }
                }
}

void myGraphicsScene::draw_for_screen(QPainter *painter, QMatrix *m)
{
        layer_index_update();

        painter->setRenderHint(QPainter::Antialiasing, FALSE);				//Come in myGraphicsView
        painter->setRenderHint(QPainter::HighQualityAntialiasing, FALSE);		//Come in myGraphicsView
        painter->setRenderHint(QPainter::TextAntialiasing, FALSE);			//Come in myGraphicsView
        painter->setRenderHint(QPainter::SmoothPixmapTransform, FALSE);			//Come in myGraphicsView

        if (GV->is_sch && (file_type == FT_PROJECT))
                GV->draw_frame(painter, m);

        QMatrix minverted = m->inverted();
        QRect visible_rect = minverted.mapRect(painter->viewport());
        painter->setMatrixEnabled(FALSE);

        if (!pcb_pad_via_filled && !GV->is_sch)
                {
                if (painter->viewport() != composition_mode_image.rect())
                        //Rigenero l'immagine con il giusto numero di pixel
                        composition_mode_image = QImage(painter->viewport().width(), painter->viewport().height(), QImage::Format_ARGB32_Premultiplied);

                for (int i = L_PCBHelp; i >= L_PCBBoard; i--)
                        if (GV->layer_settings[i].visible)
                                {
                                if (layer_data[i].is_copper)
                                        {
                                        QPainter *cm_painter = new QPainter();

                                        composition_mode_image.fill(0);
                                        cm_painter->begin(&composition_mode_image);
                                        draw_layer(cm_painter, m, visible_rect, TRUE, i);
                                        cm_painter->end();

                                        painter->setCompositionMode(QPainter::CompositionMode_Exclusion);
                                        painter->drawImage(0,0, composition_mode_image);
                                        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
                                        }
                                else
                                        draw_layer(painter, m, visible_rect, TRUE, i);
                                }
                }
        else
                {
                if (GV->is_sch)
                        {
                        for (int i = L_SCHWire; i >= L_SCHNormal; i--)
                                if (GV->layer_settings[i].visible)
                                        draw_layer(painter, m, visible_rect, TRUE, i);
                        }
                else
                        {
                        for (int i = L_PCBHelp; i >= L_PCBBoard; i--)
                                if (GV->layer_settings[i].visible)
                                        draw_layer(painter, m, visible_rect, TRUE, i);
                        }
                }

        //Disegno tutte le giunzioni dello schematico e i pad/vie del pcb
	if (GV->is_sch)
                GV->draw_junction(painter, m);
        else
                {
                if (!pcb_pad_via_filled)
                        GV->draw_pad_and_via_draft_view(painter, m, TRUE);
                GV->draw_hole(painter, m);
                }
			
	pen_layer.setColor(sel_color);	
	brush_layer.setColor(sel_color);
        myGraphicsItem *obj;
        int size = all_obj.size();
        for (int i = 0; i < size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
		{
                obj = all_obj.at(i);		//Nelle QList l'operatore at() è il più veloce
		if (obj->is_selected)
			if (visible_rect.intersects(obj->boundingRect()))
                                obj->paint(painter, m);
		}

        if ((GV->tool == T_ADD) && w_main->tool_avviato)
                {
                //Questo blocco di codice è usato anche quando si fa il paste e il duplicate
                foreach(myComponent *comp, cloned_comp)
                        {
                        size = comp->all_obj.count();
                        for (int i = 0; i < size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
                                {
                                obj = comp->all_obj.at(i);		//Nelle QList l'operatore at() è il più veloce
                                        obj->paint(painter, m);
                                }
                        }
                foreach(myGraphicsItem *obj, cloned_obj)
                        obj->paint(painter, m);
                }

        painter->setMatrixEnabled(TRUE);
}

void    myGraphicsScene::draw_for_printer(QPainter *painter, QMatrix *m)
{
        ////////////////////////////////////////////////////////////////////////////
        // Purtroppo la sovrapposizione dei colori nel PCB si può fare solo a schermo e non su stampante
        // Per realizzarla su stampante dovrei utilizzare un immagine di dimensioni enormi
        ////////////////////////////////////////////////////////////////////////////

        layer_index_update();

        bool pcb_pad_via_filled_temp = pcb_pad_via_filled;
        pcb_pad_via_filled = TRUE;

        painter->setRenderHint(QPainter::Antialiasing, FALSE);				//Come in myGraphicsView
        painter->setRenderHint(QPainter::HighQualityAntialiasing, FALSE);		//Come in myGraphicsView
        painter->setRenderHint(QPainter::TextAntialiasing, FALSE);			//Come in myGraphicsView
        painter->setRenderHint(QPainter::SmoothPixmapTransform, FALSE);			//Come in myGraphicsView

        if (GV->is_sch && (file_type == FT_PROJECT))
                GV->draw_frame(painter, m);

        QMatrix minverted = m->inverted();
        QRect visible_rect = minverted.mapRect(painter->viewport());
        painter->setMatrixEnabled(FALSE);

        if (GV->is_sch)
                {
                for (int i = L_SCHWire; i >= L_SCHNormal; i--)
                        if (GV->layer_settings[i].print)
                                draw_layer(painter, m, visible_rect, FALSE, i);
                }
        else
                {
                for (int i = L_PCBHelp; i >= L_PCBBoard; i--)
                        if (GV->layer_settings[i].print)
                                draw_layer(painter, m, visible_rect, FALSE, i);
                }

        //Disegno tutte le giunzioni dello schematico e i pad/vie del pcb
        if (GV->is_sch)
                GV->draw_junction(painter, m);
        else
                {
                if (!pcb_pad_via_filled)
                        GV->draw_pad_and_via_draft_view(painter, m, FALSE);
                QColor old_background_color = workspace_data[W_PCBbackground].color;
                workspace_data[W_PCBbackground].color = QColor(255,255,255);
                GV->draw_hole(painter, m);
                workspace_data[W_PCBbackground].color = old_background_color;
                }

        pcb_pad_via_filled = pcb_pad_via_filled_temp;
}
