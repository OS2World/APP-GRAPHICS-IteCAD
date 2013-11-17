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

#include "drc.h"

/*********************************************************************************************

A prima vista potrebbe sembrare che l'utilizzo di QPainterPath sia valido.
In realtà no perchè per calcolarmi le insulation dovrei avere dei QPainterPath con lo spessore
Purtroppo non esistono...

**********************************************************************************************/


extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;
extern	win_main	*w_main;

extern	bool		highlight_active;

extern	int		drc_v2v;
extern	int		drc_v2l;
extern	int		drc_l2l;

int drc_insulation;

int p1_x;
int p1_y;
int p1_w;
int p1_h;

int p2_x;
int p2_y;
int p2_w;
int p2_h;

int line1_x1;
int line1_y1;
int line1_x2;
int line1_y2;

int line2_x1;
int line2_y1;
int line2_x2;
int line2_y2;

QPolygon    polygon1;
QPolygon    polygon2;
QPolygon    *pol1;
QPolygon    *pol2;
QPolygon    text_points1;
QPolygon    text_lines1;
QPolygon    text_points2;
QPolygon    text_lines2;
bool        is_pol1_a_polygon;
bool        is_pol2_a_polygon;
bool        is_pol1_filled;
bool        is_pol2_filled;

qint64  insulation;       //Bisogna usare qint64 e non quint64 (i 64 bit sono necessari)

qint64	dx;		//Bisogna usare qint64 e non quint64 (i 64 bit sono necessari)
qint64	dy;		//Bisogna usare qint64 e non quint64 (i 64 bit sono necessari)

bool    basic_p1_p2()
{
        dx = p2_x - p1_x;
        dy = p2_y - p1_y;

        return (insulation < dx*dx + dy*dy);
}

bool    basic_p1_l2()
{
        //Calcolo distance e p2 che è il punto più vicino a p1 dentro a line2
        qreal	t;

        dx = line2_x2 - line2_x1;
        dy = line2_y2 - line2_y1;

        if ((dx == 0) && (dy == 0))
                {
                //Il segmento è in realtà un punto
                p2_x = line2_x1;
                p2_y = line2_y1;
                }
        else
                {
                //Calcolo t
                t = (qreal) ((p1_x - line2_x1) * dx + (p1_y - line2_y1) * dy) / (qreal) (dx * dx + dy * dy);

                if (t < 0)
                        {
                        //Il punto più vicino è line2_x1, line2_y1
                        p2_x = line2_x1;
                        p2_y = line2_y1;
                        }
                else if (t > 1)
                        {
                        //Il punto più vicino è line2_x2, line2_y2
                        p2_x = line2_x2;
                        p2_y = line2_y2;
                        }
                else
                        {
                        //Il punto più vicino è all'interno del segmento
                        p2_x = line2_x1 + t * dx;
                        p2_y = line2_y1 + t * dy;
                        }
                }
        dx = p1_x - p2_x;
        dy = p1_y - p2_y;
        return (insulation < dx*dx + dy*dy);
}

bool    basic_l1_p2()
{
        //Calcolo distance e p2 che è il punto più vicino a p1 dentro a line2
        qreal	t;

        dx = line1_x2 - line1_x1;
        dy = line1_y2 - line1_y1;
        if ((dx == 0) && (dy == 0))
                {
                //Il segmento è in realtà un punto
                p1_x = line1_x1;
                p1_y = line1_y1;
                }
        else
                {
                //Calcolo t
                t = (qreal) ((p2_x - line1_x1) * dx + (p2_y - line1_y1) * dy) / (qreal) (dx * dx + dy * dy);

                if (t < 0)
                        {
                        //Il punto più vicino è line2_x1, line2_y1
                        p1_x = line1_x1;
                        p1_y = line1_y1;
                        }
                else if (t > 1)
                        {
                        //Il punto più vicino è line2_x2, line2_y2
                        p1_x = line1_x2;
                        p1_y = line1_y2;
                        }
                else
                        {
                        //Il punto più vicino è all'interno del segmento
                        p1_x = line1_x1 + t * dx;
                        p1_y = line1_y1 + t * dy;
                        }
                }
        dx = p1_x - p2_x;
        dy = p1_y - p2_y;
        return (insulation < dx*dx + dy*dy);
}

bool	basic_intersect_l1_l2()
{
        qint64	da, db;
        qint64	denominator;
        qreal	s, t;

        dx = line1_x2 - line1_x1;
        dy = line1_y2 - line1_y1;
        da = line2_x2 - line2_x1;
        db = line2_y2 - line2_y1;

        denominator = da * dy - db * dx;
        if (denominator == 0)
                {
                //I segmenti sono paralleli
                return FALSE;
                }

        s = (qreal) (dx * (line2_y1 - line1_y1) + dy * (line1_x1 - line2_x1)) / (qreal) (denominator);
        t = (qreal) (da * (line2_y1 - line1_y1) + db * (line1_x1 - line2_x1)) / (qreal) (denominator);

        return ((s >= 0) && (s <=1) && (t >= 0) && (t <= 1));
        // Attenzione:
        // Se esiste il punto di intersezione è (line1_x1 + t*dx, line1_y1 * t*dy)
}

bool    basic_l1_l2()
{
        if (!basic_intersect_l1_l2())
                {
                p1_x = line1_x1;
                p1_y = line1_y1;
                if (basic_p1_l2())
                        {
                        p1_x = line1_x2;
                        p1_y = line1_y2;
                        if (basic_p1_l2())
                                {
                                p2_x = line2_x1;
                                p2_y = line2_y1;
                                if (basic_l1_p2())
                                        {
                                        p2_x = line2_x2;
                                        p2_y = line2_y2;
                                        if (basic_l1_p2())
                                                return TRUE;
                                }
                                }
                        }
                }
        return FALSE;
}

bool    basic_pol1_pol2()
{
    for (int i = 1; i < pol1->count();i++)
            {
            line1_x1 = pol1->at(i-1).x();
            line1_y1 = pol1->at(i-1).y();
            line1_x2 = pol1->at(i).x();
            line1_y2 = pol1->at(i).y();
            for (int j = 1; j < pol2->count();j++)
                    {
                    line2_x1 = pol2->at(j-1).x();
                    line2_y1 = pol2->at(j-1).y();
                    line2_x2 = pol2->at(j).x();
                    line2_y2 = pol2->at(j).y();
                    if (!basic_l1_l2())
                            return FALSE;
                    }
            if (is_pol2_a_polygon)
                    {
                    line2_x1 = pol2->first().x();
                    line2_y1 = pol2->first().y();
                    line2_x2 = pol2->last().x();
                    line2_y2 = pol2->last().y();
                    if (!basic_l1_l2())
                            return FALSE;
                    if (is_pol2_filled)
                            if (pol2->containsPoint(QPoint(pol1->first().x(), pol1->first().y()), Qt::OddEvenFill))
                                return FALSE;
                    }
            }
    if (is_pol1_a_polygon)
            {
            line1_x1 = pol1->first().x();
            line1_y1 = pol1->first().y();
            line1_x2 = pol1->last().x();
            line1_y2 = pol1->last().y();
            if (!basic_l1_l2())
                    return FALSE;
            if (is_pol1_filled)
                    if (pol1->containsPoint(QPoint(pol2->first().x(), pol2->first().y()), Qt::OddEvenFill))
                        return FALSE;
            }
    return TRUE;
}

bool    basic_p1_pol2()
{
    for (int i = 1; i < pol2->count();i++)
            {
            line2_x1 = pol2->at(i-1).x();
            line2_y1 = pol2->at(i-1).y();
            line2_x2 = pol2->at(i).x();
            line2_y2 = pol2->at(i).y();
            if (!basic_p1_l2())
                    return FALSE;
            }
    if (is_pol2_a_polygon)
            {
            line2_x1 = pol2->first().x();
            line2_y1 = pol2->first().y();
            line2_x2 = pol2->last().x();
            line2_y2 = pol2->last().y();
            if (!basic_p1_l2())
                    return FALSE;
            if (is_pol2_filled)
                    if (pol2->containsPoint(QPoint(p1_x, p1_y), Qt::OddEvenFill))
                           return FALSE;
            }
    return TRUE;
}

bool    basic_pol1_p2()
{
    for (int i = 1; i < pol1->count();i++)
            {
            line1_x1 = pol1->at(i-1).x();
            line1_y1 = pol1->at(i-1).y();
            line1_x2 = pol1->at(i).x();
            line1_y2 = pol1->at(i).y();
            if (!basic_l1_p2())
                    return FALSE;
            }
    if (is_pol1_a_polygon)
            {
            line1_x1 = pol1->first().x();
            line1_y1 = pol1->first().y();
            line1_x2 = pol1->last().x();
            line1_y2 = pol1->last().y();
            if (!basic_l1_p2())
                    return FALSE;
            if (is_pol1_filled)
                    if (pol1->containsPoint(QPoint(p2_x, p2_y), Qt::OddEvenFill))
                           return FALSE;
            }
    return TRUE;
}

bool    basic_l1_pol2()
{
        for (int i = 1; i < pol2->count();i++)
                {
                line2_x1 = pol2->at(i-1).x();
                line2_y1 = pol2->at(i-1).y();
                line2_x2 = pol2->at(i).x();
                line2_y2 = pol2->at(i).y();
                if (!basic_l1_l2())
                        return FALSE;
                }
        if (is_pol2_a_polygon)
                {
                line2_x1 = pol2->first().x();
                line2_y1 = pol2->first().y();
                line2_x2 = pol2->last().x();
                line2_y2 = pol2->last().y();
                if (!basic_l1_l2())
                        return FALSE;
                if (is_pol2_filled)
                        if (pol2->containsPoint(QPoint(line1_x1, line1_y1), Qt::OddEvenFill))
                               return FALSE;
                }
        return TRUE;
}

bool    basic_pol1_l2()
{
        for (int i = 1; i < pol1->count();i++)
                {
                line1_x1 = pol1->at(i-1).x();
                line1_y1 = pol1->at(i-1).y();
                line1_x2 = pol1->at(i).x();
                line1_y2 = pol1->at(i).y();
                if (!basic_l1_l2())
                        return FALSE;
                }
        if (is_pol1_a_polygon)
                {
                line1_x1 = pol1->first().x();
                line1_y1 = pol1->first().y();
                line1_x2 = pol1->last().x();
                line1_y2 = pol1->last().y();
                if (!basic_l1_l2())
                        return FALSE;
                if (is_pol1_filled)
                        if (pol1->containsPoint(QPoint(line2_x1, line2_y1), Qt::OddEvenFill))
                               return FALSE;
                }
        return TRUE;
}

bool	drc_insulation_between_2_objects(myGraphicsItem *obj1, myGraphicsItem *obj2)
{
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 1 - non considero gli oggetti che si trovano su piani diversi
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (!obj1->layer_intersect(obj2->layer_from, obj2->layer_to))
                return TRUE;

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 2 - ricavo la drc_insulation
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if ((obj1->tipo == O_PAD) || (obj1->tipo == O_VIA))
                {
                if ((obj2->tipo == O_PAD) || (obj2->tipo == O_VIA))
                        drc_insulation = drc_v2v;
                else
                        drc_insulation = drc_v2l;
                }
        else
                {
                if ((obj2->tipo == O_PAD) || (obj2->tipo == O_VIA))
                        drc_insulation = drc_v2l;
                else
                        drc_insulation = drc_l2l;
                }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 3 - guardo la distanza approssimativa tra i bounding rect per scartare i casi in cui gli oggetti sono troppo distanti
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        int br_bottom1 = obj1->info_br.bottom();
        int br_top1 = obj1->info_br.top();
        int br_left1 = obj1->info_br.left();
        int br_right1 = obj1->info_br.right();
        int br_bottom2 = obj2->info_br.bottom();
        int br_top2 = obj2->info_br.top();
        int br_left2 = obj2->info_br.left();
        int br_right2 = obj2->info_br.right();

        if (((br_top1 - br_bottom2) > drc_insulation) || ((br_top2 - br_bottom1) > drc_insulation))
            return TRUE;
        if (((br_left1 - br_right2) > drc_insulation) || ((br_left2 - br_right1) > drc_insulation))
            return TRUE;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // 4 - analizzo gli oggetti tra di loro
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        insulation = drc_insulation;
        char basic_shape1 = convert_obj1(obj1);
        char basic_shape2 = convert_obj2(obj2);
        insulation = insulation * insulation;

        switch (basic_shape1)
        {
        case 'C':   //Circle
                switch (basic_shape2)
                {
                case 'C':   //Circle
                        return basic_p1_p2();
                case 'L':   //Line
                        return basic_p1_l2();
                case 'P':   //Poly
                        return basic_p1_pol2();
                case 'T':   //Text;
                        return basic_p1_text2();
                }
        case 'L':   //Line
                switch (basic_shape2)
                {
                case 'C':   //Circle
                        return basic_l1_p2();
                case 'L':   //Line
                        return basic_l1_l2();
                case 'P':   //Poly
                        return basic_l1_pol2();
                case 'T':   //Text;
                        return basic_l1_text2();
                }
        case 'P':   //Poly
                switch (basic_shape2)
                {
                case 'C':   //Circle
                        return basic_pol1_p2();
                case 'L':   //Line
                        return basic_pol1_l2();
                case 'P':   //Poly
                        return basic_pol1_pol2();
                case 'T':   //Text;
                        return basic_pol1_text2();
                }
        case 'T':   //Text;
                switch (basic_shape2)
                {
                case 'C':   //Circle
                        return basic_text1_p2();
                case 'L':   //Line
                        return basic_text1_l2();
                case 'P':   //Poly
                        return basic_text1_pol2();
                case 'T':   //Text;
                        return basic_text1_text2();
                }
        }

        return TRUE;
}

void	drc_insulation_on_all_objects()
{
        GV->select_clear();
	highlight_active = TRUE;
	GV->highlight_clear();

        QList<myGraphicsItem *> *ao = &GS->all_obj;
	int ao_size = ao->size();
	for (int i = 0; i < ao_size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
		{
		myGraphicsItem	*obj_i = ao->at(i);
                if (obj_i->layer_intersect(L_PCBCopper1, L_PCBCopper16))
                        for (int j = i + 1; j < ao_size; ++j)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
                                {
                                myGraphicsItem	*obj_j = ao->at(j);
                                if (obj_j->layer_intersect(L_PCBCopper1, L_PCBCopper16))
                                            if (obj_i->net_id != obj_j->net_id)
                                                    if (!drc_insulation_between_2_objects(obj_i, obj_j))	//Controllo che i due oggetti non appartengano allo stesso net_id
                                                            {
                                                            obj_i->highlight_active = TRUE;
                                                            obj_j->highlight_active = TRUE;
                                                            }
                                }
			}
}

bool	drc_insulation_on_single_object(myGraphicsItem *obj)
{
        //Questa funzione è utilizzata da win_main quando si inseriscono nuovi oggetti
        QList<myGraphicsItem *> *ao = &GS->all_obj;
	int ao_size = ao->size();
	for (int i = 0; i < ao_size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
		{
		myGraphicsItem	*obj_i = ao->at(i);
                if (obj_i->layer_intersect(L_PCBCopper1, L_PCBCopper16))
                        if (obj->net_id != obj_i->net_id)
                                if (!drc_insulation_between_2_objects(obj, obj_i))
                                        return FALSE;
		}
	return TRUE;
}

bool	drc_insulation_null_on_single_object(myGraphicsItem *obj)
{
        //Questa funzione è utilizzata da win_main quando si inseriscono nuovi oggetti
        int old_drc_v2v = drc_v2v;
        int old_drc_v2l = drc_v2l;
        int old_drc_l2l = drc_l2l;
        drc_v2v = 0;
        drc_v2l = 0;
        drc_l2l = 0;

        bool	result = drc_insulation_on_single_object(obj);

        drc_v2v = old_drc_v2v;
        drc_v2l = old_drc_v2l;
        drc_l2l = old_drc_l2l;

        return result;
}

void	drc_net_id_on_all_objects()
{
        //Questa funzione viene sempre e solo richiamata dentro w_main->net_id_from_pad()

        int old_drc_v2v = drc_v2v;
        int old_drc_v2l = drc_v2l;
        int old_drc_l2l = drc_l2l;
        drc_v2v = 0;
        drc_v2l = 0;
        drc_l2l = 0;

        //Azzero il net_id di ogni oggetto tranne i PAD le VIE e le CONNECTION che sono facili da stabilire
        QList<myGraphicsItem *> *ao = &GS->all_obj;
        int ao_size = ao->size();
        for (int i = 0; i < ao_size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
               {
               myGraphicsItem	*obj = ao->at(i);
               switch (obj->tipo)
               {
               case O_PAD:
               case O_VIA:
                      obj->net_id = obj->node_start->net_id;
                      break;
               case O_CONNECTION:
                      obj->net_id = obj->node_start->net_id;
                      if (obj->net_id != obj->node_end->net_id)
                            obj->net_id = -1;
                      break;
               default:
                      obj->net_id = 0;
                      break;
               }
               }

        //Controllo gli incroci tra tutti gli oggetti
        for (int i = 0; i < ao_size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
               {
               myGraphicsItem	*obj = ao->at(i);
               if (obj->layer_intersect(L_PCBCopper1, L_PCBCopper16))
                    if (obj->net_id > 0)
                          drc_net_id_recursive(obj);
               }

        drc_v2v = old_drc_v2v;
        drc_v2l = old_drc_v2l;
        drc_l2l = old_drc_l2l;
}

void	drc_net_id_recursive(myGraphicsItem *obj)
{
        if (obj->net_id <= 0)
            return;

        QList<myGraphicsItem *> *ao = &GS->all_obj;
        int ao_size = ao->size();
        for (int i = 0; i < ao_size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
               {
               myGraphicsItem	*obj_i = ao->at(i);
               if (obj_i->layer_intersect(L_PCBCopper1, L_PCBCopper16))
                    if (obj->net_id != obj_i->net_id)
                            if (!drc_insulation_between_2_objects(obj, obj_i))
                                    {
                                    if (obj_i->net_id == 0)
                                            {
                                            obj_i->net_id = obj->net_id;
                                            drc_net_id_recursive(obj_i);
                                            }
                                    else if (obj_i->net_id > 0)
                                            {
                                            if (!obj_i->comp)
                                                obj_i->net_id = -1;     //Solo uno non ha superato il DRC
                                            else if (!obj->comp)
                                                    {
                                                    obj->net_id = -1;   //Solo uno non ha superato il DRC
                                                    return;
                                                    }
                                            else
                                                    {
                                                    obj->net_id = -1;       //Ambedue non hanno superato il DRC
                                                    obj_i->net_id = -1;     //Ambedue non hanno superato il DRC
                                                    return;
                                                    }
                                            }
                                    }
               }
}

void	drc_connect_id_on_all_objects()
{
        //Questa funzione viene sempre e solo richiamata dentro w_main->net_id_from_pad()

        int old_drc_v2v = drc_v2v;
        int old_drc_v2l = drc_v2l;
        int old_drc_l2l = drc_l2l;
        drc_v2v = 0;
        drc_v2l = 0;
        drc_l2l = 0;

        //Azzero il connect_id di ogni oggetto
        QList<myGraphicsItem *> *ao = &GS->all_obj;
        int ao_size = ao->size();
        for (int i = 0; i < ao_size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
               {
               myGraphicsItem	*obj = ao->at(i);
               obj->connect_id = -1;
               }

        //Controllo gli incroci tra tutti gli oggetti
        int connect_id = 1;
        for (int i = 0; i < ao_size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
               {
               myGraphicsItem	*obj = ao->at(i);
               if (obj->layer_intersect(L_PCBCopper1, L_PCBCopper16))
                    if (obj->connect_id <= 0)
                            {
                            obj->connect_id = connect_id;
                            connect_id++;
                            drc_connect_id_recursive(obj);
                            }
               }

        drc_v2v = old_drc_v2v;
        drc_v2l = old_drc_v2l;
        drc_l2l = old_drc_l2l;
}

void	drc_connect_id_recursive(myGraphicsItem *obj)
{
        QList<myGraphicsItem *> *ao = &GS->all_obj;
        int ao_size = ao->size();
        for (int i = 0; i < ao_size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
                {
                myGraphicsItem	*obj_i = ao->at(i);
                if (obj_i->layer_intersect(L_PCBCopper1, L_PCBCopper16))
                        if (obj_i->connect_id <= 0)
                                if (!drc_insulation_between_2_objects(obj, obj_i))
                                        {
                                        obj_i->connect_id = obj->connect_id;
                                        drc_connect_id_recursive(obj_i);
                                        }
               }
}

void	drc_connect_id_recursive_for_clean_up(QList<myGraphicsItem *> *ao, myGraphicsItem *obj)
{
        int old_drc_v2v = drc_v2v;
        int old_drc_v2l = drc_v2l;
        int old_drc_l2l = drc_l2l;
        drc_v2v = 0;
        drc_v2l = 0;
        drc_l2l = 0;

        int ao_size = ao->size();
        for (int i = 0; i < ao_size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
                {
                myGraphicsItem	*obj_i = ao->at(i);
                if (obj_i->layer_intersect(L_PCBCopper1, L_PCBCopper16))
                        if (obj_i->connect_id <= 0)
                                if (!drc_insulation_between_2_objects(obj, obj_i))
                                        {
                                        obj_i->connect_id = obj->connect_id;
                                        drc_connect_id_recursive_for_clean_up(ao, obj_i);
                                        }
               }

        drc_v2v = old_drc_v2v;
        drc_v2l = old_drc_v2l;
        drc_l2l = old_drc_l2l;
}

char    convert_obj1(myGraphicsItem *obj)
{
        switch (obj->tipo)
        {
        case O_CONNECTION:
                line1_x1 = obj->start.x();
                line1_y1 = obj->start.y();
                line1_x2 = obj->end.x();
                line1_y2 = obj->end.y();
                insulation += obj->pen_width/2;
                return 'L';
        case O_VIA:
        case O_PAD:
                switch (obj->shape)
                {
                case PS_ROUND:
                        if (obj->width == obj->height)
                                {
                                p1_x = obj->node_start->point.x();
                                p1_y = obj->node_start->point.y();
                                insulation += obj->width/2;
                                return 'C';
                                }
                        else
                                {
                                obj->polygon_interpolation(&polygon1);
                                pol1 = &polygon1;
                                is_pol1_a_polygon = TRUE;
                                is_pol1_filled = TRUE;
                                return 'P';
                                }
                case PS_ROUNDED_RECTANGLE:
                        {
                        int delta_div_2 = (obj->width - obj->height)/2;
                        if (delta_div_2)
                                {
                                line1_x1 = obj->node_start->point.x() + delta_div_2;
                                line1_x2 = obj->node_start->point.x() - delta_div_2;
                                line1_y1 = obj->node_start->point.y();
                                line1_y2 = obj->node_start->point.y();

                                insulation += obj->height/2;
                                }
                        else
                                {
                                line1_x1 = obj->node_start->point.x();
                                line1_x2 = obj->node_start->point.x();
                                line1_y1 = obj->node_start->point.y() + delta_div_2;
                                line1_y2 = obj->node_start->point.y() - delta_div_2;

                                insulation += obj->width/2;
                                }
                        return  'L';
                        }
                case PS_RECTANGLE:
                        {
                        int w_div_2 = obj->width / 2;
                        int h_div_2 = obj->height / 2;
                        int x1 = obj->node_start->point.x() - w_div_2;
                        int x2 = obj->node_start->point.x() + w_div_2;
                        int y1 = obj->node_start->point.y() - h_div_2;
                        int y2 = obj->node_start->point.y() + h_div_2;

                        polygon1.clear();
                        polygon1.append(QPoint(x1, y1));
                        polygon1.append(QPoint(x1, y2));
                        polygon1.append(QPoint(x2, y2));
                        polygon1.append(QPoint(x2, y1));

                        pol1 = &polygon1;
                        is_pol1_a_polygon = TRUE;
                        return  'P';
                        }
                }
        case O_POLYLINE:
        case O_POLYGON:
                {
                pol1 = &obj->polygon;
                is_pol1_a_polygon = (obj->tipo == O_POLYGON);
                is_pol1_filled = obj->fill_polygon;
                insulation += obj->pen_width/2;
                return  'P';
                }
        case O_RECT:
                {
                int x1 = obj->start.x();
                int x2 = obj->start.x() + obj->width;
                int y1 = obj->start.y();
                int y2 = obj->start.y() + obj->height;

                polygon1.clear();
                polygon1.append(QPoint(x1, y1));
                polygon1.append(QPoint(x1, y2));
                polygon1.append(QPoint(x2, y2));
                polygon1.append(QPoint(x2, y1));

                pol1 = &polygon1;
                is_pol1_a_polygon = obj->fill_polygon;
                insulation += obj->pen_width/2;
                return  'P';
                }
        case O_ELLIPSE:
                {
                obj->polygon_interpolation(&polygon1);
                pol1 = &polygon1;
                is_pol1_a_polygon = TRUE;
                is_pol1_filled = TRUE;
                return 'P';
                }
        case O_TEXT:
                obj->text_to_polygon(&obj->text, obj->info_start_text, &text_points1, &text_lines1);
                //text1 = obj;
                //Il pezzo sotto è riadattato da myGraphicsItem::text_br()
                qreal	passo = obj->font_height * 60;
                if (obj->font_bold)
                        insulation += (0.9 * passo);
                else
                        insulation += (0.7 * passo);
                return 'T';
        }
}

char    convert_obj2(myGraphicsItem *obj)
{
        switch (obj->tipo)
        {
        case O_CONNECTION:
                line2_x1 = obj->start.x();
                line2_y1 = obj->start.y();
                line2_x2 = obj->end.x();
                line2_y2 = obj->end.y();
                insulation += obj->pen_width/2;
                return 'L';
        case O_VIA:
        case O_PAD:
                switch (obj->shape)
                {
                case PS_ROUND:
                        if (obj->width == obj->height)
                                {
                                p2_x = obj->node_start->point.x();
                                p2_y = obj->node_start->point.y();
                                insulation += obj->width/2;
                                return 'C';
                                }
                        else
                                {
                                obj->polygon_interpolation(&polygon2);
                                pol2 = &polygon2;
                                is_pol2_a_polygon = TRUE;
                                is_pol2_filled = TRUE;
                                return 'P';
                                }
                case PS_ROUNDED_RECTANGLE:
                        {
                        int delta_div_2 = (obj->width - obj->height)/2;
                        if (delta_div_2)
                                {
                                line2_x1 = obj->node_start->point.x() + delta_div_2;
                                line2_x2 = obj->node_start->point.x() - delta_div_2;
                                line2_y1 = obj->node_start->point.y();
                                line2_y2 = obj->node_start->point.y();

                                insulation += obj->height/2;
                                }
                        else
                                {
                                line2_x1 = obj->node_start->point.x();
                                line2_x2 = obj->node_start->point.x();
                                line2_y1 = obj->node_start->point.y() + delta_div_2;
                                line2_y2 = obj->node_start->point.y() - delta_div_2;

                                insulation += obj->width/2;
                                }
                        return  'L';
                        }
                case PS_RECTANGLE:
                        {
                        int w_div_2 = obj->width / 2;
                        int h_div_2 = obj->height / 2;
                        int x1 = obj->node_start->point.x() - w_div_2;
                        int x2 = obj->node_start->point.x() + w_div_2;
                        int y1 = obj->node_start->point.y() - h_div_2;
                        int y2 = obj->node_start->point.y() + h_div_2;

                        polygon2.clear();
                        polygon2.append(QPoint(x1, y1));
                        polygon2.append(QPoint(x1, y2));
                        polygon2.append(QPoint(x2, y2));
                        polygon2.append(QPoint(x2, y1));

                        pol2 = &polygon2;
                        is_pol2_a_polygon = TRUE;
                        return  'P';
                        }
                }
        case O_POLYLINE:
        case O_POLYGON:
                {
                pol2 = &obj->polygon;
                is_pol2_a_polygon = (obj->tipo == O_POLYGON);
                is_pol2_filled = obj->fill_polygon;
                insulation += obj->pen_width/2;
                return  'P';
                }
        case O_RECT:
                {
                int x1 = obj->start.x();
                int x2 = obj->start.x() + obj->width;
                int y1 = obj->start.y();
                int y2 = obj->start.y() + obj->height;

                polygon2.clear();
                polygon2.append(QPoint(x1, y1));
                polygon2.append(QPoint(x1, y2));
                polygon2.append(QPoint(x2, y2));
                polygon2.append(QPoint(x2, y1));

                pol2 = &polygon2;
                is_pol2_a_polygon = obj->fill_polygon;
                insulation += obj->pen_width/2;
                return  'P';
                }
        case O_ELLIPSE:
                {
                obj->polygon_interpolation(&polygon2);
                pol2 = &polygon2;
                is_pol2_a_polygon = TRUE;
                is_pol2_filled = TRUE;
                return 'P';
                }
        case O_TEXT:
                obj->text_to_polygon(&obj->text, obj->info_start_text, &text_points2, &text_lines2);
                //text2 = obj;
                //Il pezzo sotto è riadattato da myGraphicsItem::text_br()
                qreal	passo = obj->font_height * 60;
                if (obj->font_bold)
                        insulation += (0.9 * passo);
                else
                        insulation += (0.7 * passo);
                return 'T';
        }
}

bool    basic_p1_text2()
{
        for(int i = 0; i < text_points2.count(); i++)
                {
                p2_x = text_points2.at(i).x();
                p2_y = text_points2.at(i).y();
                if (!basic_p1_p2())
                        return FALSE;
                }
        for(int i = 0; i < text_lines2.count(); i++)
                {
                line2_x1 = text_lines2.at(i).x();
                line2_y1 = text_lines2.at(i++).y();
                line2_x2 = text_lines2.at(i).x();
                line2_y2 = text_lines2.at(i).y();
                if (!basic_p1_l2())
                        return FALSE;
                }
        return TRUE;
}

bool    basic_text1_p2()
{
        for(int i = 0; i < text_points1.count(); i++)
                {
                p1_x = text_points1.at(i).x();
                p1_y = text_points1.at(i).y();
                if (!basic_p1_p2())
                        return FALSE;
                }
        for(int i = 0; i < text_lines1.count(); i++)
                {
                line1_x1 = text_lines1.at(i).x();
                line1_y1 = text_lines1.at(i++).y();
                line1_x2 = text_lines1.at(i).x();
                line1_y2 = text_lines1.at(i).y();
                if (!basic_l1_p2())
                        return FALSE;
                }
        return TRUE;
}

bool    basic_text1_l2()
{
        for(int i = 0; i < text_points1.count(); i++)
                {
                p1_x = text_points1.at(i).x();
                p1_y = text_points1.at(i).y();
                if (!basic_p1_l2())
                        return FALSE;
                }
        for(int i = 0; i < text_lines1.count(); i++)
                {
                line1_x1 = text_lines1.at(i).x();
                line1_y1 = text_lines1.at(i++).y();
                line1_x2 = text_lines1.at(i).x();
                line1_y2 = text_lines1.at(i).y();
                if (!basic_l1_l2())
                        return FALSE;
                }
        return TRUE;
}

bool    basic_l1_text2()
{
        for(int i = 0; i < text_points2.count(); i++)
                {
                p2_x = text_points2.at(i).x();
                p2_y = text_points2.at(i).y();
                if (!basic_l1_p2())
                        return FALSE;
                }
        for(int i = 0; i < text_lines2.count(); i++)
                {
                line2_x1 = text_lines2.at(i).x();
                line2_y1 = text_lines2.at(i++).y();
                line2_x2 = text_lines2.at(i).x();
                line2_y2 = text_lines2.at(i).y();
                if (!basic_l1_l2())
                        return FALSE;
                }
        return TRUE;
}

bool    basic_text1_pol2()
{
        for(int i = 0; i < text_points1.count(); i++)
                {
                p1_x = text_points1.at(i).x();
                p1_y = text_points1.at(i).y();
                if (!basic_p1_pol2())
                        return FALSE;
                }
        for(int i = 0; i < text_lines1.count(); i++)
                {
                line1_x1 = text_lines1.at(i).x();
                line1_y1 = text_lines1.at(i++).y();
                line1_x2 = text_lines1.at(i).x();
                line1_y2 = text_lines1.at(i).y();
                if (!basic_l1_pol2())
                        return FALSE;
                }
        return TRUE;
}

bool    basic_pol1_text2()
{
        for(int i = 0; i < text_points2.count(); i++)
                {
                p2_x = text_points2.at(i).x();
                p2_y = text_points2.at(i).y();
                if (!basic_pol1_p2())
                        return FALSE;
                }
        for(int i = 0; i < text_lines2.count(); i++)
                {
                line2_x1 = text_lines2.at(i).x();
                line2_y1 = text_lines2.at(i++).y();
                line2_x2 = text_lines2.at(i).x();
                line2_y2 = text_lines2.at(i).y();
                if (!basic_pol1_l2())
                        return FALSE;
                }
        return TRUE;
}

bool    basic_text1_text2()
{
        for(int i = 0; i < text_points1.count(); i++)
                {
                p1_x = text_points1.at(i).x();
                p1_y = text_points1.at(i).y();
                if (!basic_p1_text2())
                        return FALSE;
                }
        for(int i = 0; i < text_lines1.count(); i++)
                {
                line1_x1 = text_lines1.at(i).x();
                line1_y1 = text_lines1.at(i++).y();
                line1_x2 = text_lines1.at(i).x();
                line1_y2 = text_lines1.at(i).y();
                if (!basic_l1_text2())
                        return FALSE;
                }
        return TRUE;
}

