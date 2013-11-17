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

#ifndef WIN_BILL_OF_MATERIALS_H
#define WIN_BILL_OF_MATERIALS_H

#include <QTableView>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QClipboard>
#include "tipi_dato.h"
#include "utilities.h"
#include "myGraphicsView.h"

class win_bill_of_materials : public QDialog
{
        Q_OBJECT	//Necessario per gestire Signals e Slots
public:
        win_bill_of_materials(QWidget *parent, Qt::WFlags tipo);
        ~win_bill_of_materials()
        {}

        QHBoxLayout	*setup_buttons();

        QTableWidget	bom;
        QCheckBox       *group_by_value;

        QString         csv_text;

        void            update_csv();

public slots:
        void            load_bom();
        void            save_as_csv();
        void            copy_to_clipboard();
};

#endif
