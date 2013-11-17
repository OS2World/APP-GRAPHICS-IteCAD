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

#include "myUndoView.h"

extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

myUndoView::myUndoView(QUndoStack *stack, QWidget *parent)
        : QUndoView(stack, parent)
{
}

void myUndoView::currentChanged (const QModelIndex &current, const QModelIndex &previous)
{
	GV->setUpdatesEnabled(FALSE);
	GV->setUpdatesEnabled(TRUE);
}
