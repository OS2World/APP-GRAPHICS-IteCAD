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

#ifndef UTILITIES_H
#define UTILITIES_H

#include <QRectF>
#include <QVector>
#include <QFileDialog>
#include "tipi_dato.h"

QString add_file_suffix(QString name, QString suffix);

QString xml_write_bool(bool flag);
QString xml_write_qreal(qreal value);
QString	xml_write_objlayer(int layer);
QString xml_write_point(QPoint &point);
QString xml_write_objtype(char type, bool is_sch);
QString xml_write_textposition(char tp);
QString xml_write_pinshape(char ps);
QString xml_write_portshape(char ps);
QString xml_write_powershape(char ps);
QString xml_write_padshape(char ps);
QString xml_write_padlayer(char ps);
QString xml_write_polygon(QPolygon *p);
QString xml_write_char(char c);
QString xml_write_penwidth(int w, bool is_sch);

bool	xml_read_bool(QString flag);
qreal   xml_read_qreal(QString str);
int	xml_read_objlayer(QString str);
QPoint	xml_read_point(QString str);
char	xml_read_obj_type(QString str);
char	xml_read_textposition(QString str);
char	xml_read_pinshape(QString str);
char	xml_read_portshape(QString str);
char	xml_read_powershape(QString str);
char	xml_read_padshape(QString str);
char	xml_read_padlayer(QString str);
void	xml_read_polygon(QString str, QPolygon *p);
char	xml_read_char(QString str);
int	xml_read_penwidth(QString str, bool is_sch);

QString from_001thou_to_string(int value, unsigned char um);
int	from_string_to_001thou(QString testo, unsigned char um);

QPoint  coordinate_transform_to_user(QPoint p);
QPoint  coordinate_transform_to_internal(QPoint p);

QRect	myQRect(QPoint p1, QPoint p2);
char	UM_find_description(QString);

unsigned char    node_layer_from(node_data *node);
unsigned char    node_layer_to(node_data *node);
bool    node_layer_intersect(node_data *node, char l_from, char l_to);

QString	open_prj(QWidget *parent);
QString save_prj(QWidget *parent, QString file);
QString	open_ssf(QWidget *parent);
QString save_ssf(QWidget *parent, QString file);
QString	open_pff(QWidget *parent);
QString save_pff(QWidget *parent, QString file);
QString open_dir(QWidget *parent, QString description, QString dir);
QString	open_chr(QWidget *parent);
QString save_chr(QWidget *parent, QString file);
QString save_pdf(QWidget *parent, QString file);

QString	set_another_suffix(QString filename, QString suffix);

QString	footprint_name(QString footprint);
QString	footprint_subpath(QString footprint);

QPoint	rotate_point(QPoint p, QPoint center, bool clockwise);
QPoint	mirror_point(QPoint p, QPoint center, bool horizontal);

#define swap_bool(a,b) { \
        bool c = (a);    \
        (a) = (b);		 \
        (b) = c;         \
		}

#define swap_char(a,b) { \
        char c = (a);    \
        (a) = (b);		 \
        (b) = c;         \
		}

#define swap_int(a,b) { \
        int c = (a);    \
        (a) = (b);      \
        (b) = c;        \
		}

#define swap_qpoint(a,b) { \
        QPoint c = (a);    \
        (a) = (b);			\
        (b) = c;			\
		}

#define swap_qreal(a,b) { \
        qreal c = (a);    \
        (a) = (b);			\
        (b) = c;			\
		}

#define swap_qstring(a,b) { \
        QString c = (a);    \
        (a) = (b);			\
        (b) = c;			\
		}

#define swap_qfont(a,b) { \
        QFont c = (a);    \
        (a) = (b);			\
        (b) = c;			\
		}

#define swap_qcolor(a,b) { \
        QColor c = (a);    \
        (a) = (b);			\
        (b) = c;			\
                }

int		arrotonda_a_intero(qreal x);
float	arrotonda_a_0_decimali(qreal x);
float	arrotonda_a_1_decimali(qreal x);
float	arrotonda_a_2_decimali(qreal x);
float	arrotonda_a_3_decimali(qreal x);
float	arrotonda_a_4_decimali(qreal x);
float	arrotonda_a_5_decimali(qreal x);
float	arrotonda_a_6_decimali(qreal x);

#endif
