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

#ifndef WIDGET_ZOOM_H
#define WIDGET_ZOOM_H

#include <QMainWindow>
#include <QComboBox>
#include <QObject>

class Zoom : public QComboBox
{
	 Q_OBJECT	 //Obbligatorio per implementare slot e signals
public:
	Zoom( QWidget *parent = 0);
	~Zoom();
	
	QString		zoom_Fit_to_All_Objects;
	QString		zoom_Fit_to_Selected_Objects;
	QString		zoom_Fit_to_Page;
	QString		zoom_Fit_to_Page_Width;
	QString		zoom_Fit_to_Page_Height;
	QString		zoom_Fit_to_Board;
	QString		zoom_Fit_to_Board_Width;
	QString		zoom_Fit_to_Board_Height;
	qreal		zoom_100_per_100;

	void		zoom_show_scale();

protected:
	void		mousePressEvent(QMouseEvent *event);

public slots:
	void		zoom_change(int indice);
};

#endif
