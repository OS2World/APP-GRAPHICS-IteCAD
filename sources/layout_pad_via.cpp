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

#include <QDebug>
#include "layout_pad_via.h"

layout_pad_via::layout_pad_via(bool is_pad_input, char shape_input, int width_input, int height_input, int layer_from_input, int layer_to_input, int drill_input)
        : QGridLayout()
{
        pv_is_pad = is_pad_input;

        pv_shape = shape_input;
        pv_width = width_input;
        pv_height = height_input;
        pv_layer_from = layer_from_input;
        pv_layer_to = layer_to_input;
        pv_drill = drill_input;

        //Definisco gli oggetti
        if (pv_is_pad)
                {
                shape = new QLabel();
                shape->setText(tr("shape:"));
                shape->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                Shape = new QComboBox();
                Shape->addItem(tr("Round"));
                Shape->addItem(tr("Rectangle"));
                Shape->addItem(tr("Rounded Rectangle"));
                Shape->setCurrentIndex(pv_shape);
                width = new QLabel(QObject::tr("width:"));
                width->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                }
        else
                {
                width = new QLabel(QObject::tr("diameter:"));
                width->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                }
        Width = new layout_length("", pv_width, 1, 100000, FALSE);
        if (pv_is_pad)
                {
                height = new QLabel(QObject::tr("height:"));
                height->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
                Height = new layout_length("", pv_height, 1, 100000, FALSE);
                }
        type = new QLabel();
        type->setText(tr("type:"));
        type->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        Type = new QComboBox();
        Type->addItem(tr("Through"));
        Type->addItem(tr("Top Copper"));
        Type->addItem(tr("Bottom Copper"));
        drill = new QLabel(QObject::tr("drill:"));
        drill->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        Drill = new layout_length("", pv_drill, 1, 100000, FALSE);
        QObject::connect(Type, SIGNAL(currentIndexChanged(int)), this, SLOT(change_type(int)));
        setLayerFromTo(pv_layer_from, pv_layer_to);

        //Piazzo gli oggetti
        if (pv_is_pad)
                {
                this->addWidget(shape, 0, 0);
                this->addWidget(Shape, 0, 1);
                this->addWidget(width, 0, 2);
                this->addLayout(Width, 0, 3);
                this->addWidget(height, 1, 2);
                this->addLayout(Height, 1, 3);
                this->addWidget(type, 2, 0);
                this->addWidget(Type, 2, 1);
                this->addWidget(drill, 2, 2);
                this->addLayout(Drill, 2, 3);
                }
        else
                {
                this->addWidget(width, 0, 0);
                this->addLayout(Width, 0, 1);
                this->addWidget(drill, 1, 0);
                this->addLayout(Drill, 1, 1);
                }
}

void	layout_pad_via::change_type(int new_index)
{
        Drill->setEnabled(new_index == 0);
}

void	layout_pad_via::update_new_values()
{
        if (pv_is_pad)
                {
                pv_shape = Shape->currentIndex();
                pv_width = Width->length;
                pv_height = Height->length;
                switch (Type->currentIndex())
                {
                case 0:
                        pv_layer_from = L_PCBCopper1;
                        pv_layer_to = L_PCBCopper16;
                        pv_drill = Drill->length;
                        Drill->setEnabled(TRUE);
                        break;
                case 1:
                        pv_layer_from = L_PCBCopper1;
                        pv_layer_to = L_PCBCopper1;
                        Drill->setEnabled(FALSE);
                        break;
                case 2:
                        pv_layer_from = L_PCBCopper16;
                        pv_layer_to = L_PCBCopper16;
                        Drill->setEnabled(FALSE);
                        break;
                }
                }
        else
                {
                pv_shape = PS_ROUND;
                pv_width = Width->length;
                pv_height = Width->length;  //Nelle vie la Height coincide con Width
                pv_drill = Drill->length;
                }        
}

void	layout_pad_via::setShape(char shape)
{
        Shape->setCurrentIndex(shape);
}

void	layout_pad_via::setWidth(int width)
{
        Width->setLength(width);
}

void	layout_pad_via::setHeight(int height)
{
        Height->setLength(height);
}

void	layout_pad_via::setLayerFromTo(int layer_from, int layer_to)
{
        if (layer_from != layer_to)
                Type->setCurrentIndex(0);
        else if (pv_layer_from == L_PCBCopper1)
                Type->setCurrentIndex(1);
        else
                Type->setCurrentIndex(2);
}

void	layout_pad_via::setDrill(int drill)
{
        Drill->setLength(drill);
}

