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

#ifndef LAYOUT_PAD_VIA_H
        #define LAYOUT_PAD_VIA_H

        #include <QGridLayout>
        #include <QComboBox>
        #include <QLabel>
        #include <QToolButton>
        #include "utilities.h"
        #include "layout_length.h"

class layout_pad_via : public QGridLayout
{	
	Q_OBJECT	//Necessario per gestire Signals e Slots	
public:
    layout_pad_via(bool is_pad_input, char shape_input, int width_input, int height_input, int layer_from_input, int layer_to_input, int drill_input);
        ~layout_pad_via()
	{}

        bool            pv_is_pad;

        char            pv_shape;
        int             pv_width;
        int             pv_height;
        unsigned char   pv_layer_from;
        unsigned char   pv_layer_to;
        int             pv_drill;

        QLabel          *shape;
        QComboBox	*Shape;
        QLabel          *width;
        layout_length   *Width;
        QLabel          *height;
        layout_length   *Height;
        QLabel          *type;
        QComboBox	*Type;
        QLabel          *drill;
        layout_length   *Drill;

        void            update_new_values();

        void		setShape(char shape);
        void		setWidth(int width);
        void		setHeight(int height);
        void		setLayerFromTo(int layer_from, int layer_to);
        void		setDrill(int drill);

public slots:	
        void            change_type(int new_index);
};

#endif
