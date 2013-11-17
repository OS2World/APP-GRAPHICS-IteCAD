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

#include "win_report.h"

extern	win_main		*w_main;
extern	myGraphicsView          *GV;
extern	QVector<ratsnest_struct>	ratsnest;

win_report::win_report(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

	setWindowTitle(tr("Report"));	
	//setSizeGripEnabled(true);
	setModal(true);	

	QGridLayout	*layout = new QGridLayout(this);
	
	
	info.setSelectionMode(QAbstractItemView::SingleSelection);
	info.setIconSize(QSize(24,22));
	info.setFocusPolicy(Qt::NoFocus);	//Se non metto questa riga quando premo Enter la finestra non si chiude (questo perchè il fuoco non passa al comando OK)
        info.setFixedHeight(300);

        layout->addWidget(&info,0,0);
        layout->addWidget(&info_pads_vias_nets,2,0);
        layout->addWidget(&info_ratsnests,3,0);
        layout->addLayout(setup_buttons(),4,0);
	layout->setSpacing(6);
        layout->setMargin(8);

	num_error = 0;
	num_warning = 0;
}

QHBoxLayout	*win_report::setup_buttons()
{
	QHBoxLayout	*L_comandi = new QHBoxLayout();
	L_comandi->setSpacing(6);
	L_comandi->setMargin(0);
        QSpacerItem	*L_comandi_molla = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	L_comandi->addItem(L_comandi_molla);
	QPushButton	*L_comandi_OK = new QPushButton(this);
	L_comandi_OK->setText(tr("Ok"));
	L_comandi->addWidget(L_comandi_OK);
	
	QObject::connect(L_comandi_OK, SIGNAL(clicked()), this, SLOT(accept()));

	L_comandi_OK->setAutoDefault(TRUE);
	
	return L_comandi;
}

void	win_report::insert_row(QString text, char mode)
{
	QListWidgetItem *item;
	switch (mode)
	{
	case 'W':	//Warning
		item = new QListWidgetItem(QIcon(":/Resources/warning.png"), text);
		num_warning++;
                //info.insertItem(num_warning + num_error, item);   //Inserisco al fondo
		break;
	case 'E':	//Error
		item = new QListWidgetItem(QIcon(":/Resources/error.png"), text);
		num_error++;
                //info.insertItem(0, item);   //Inserisco in prima posizione
		break;
	}
        info.addItem(item); //Inserisco tutto di seguito
}

void	win_report::finalize()
{
        info_pads = 0;
        foreach(myGraphicsItem *obj, GV->scene->all_obj)
                if (obj->tipo == O_PAD)
                        info_pads++;
        info_vias = 0;
        foreach(myGraphicsItem *obj, GV->scene->all_obj)
                if (obj->tipo == O_VIA)
                        info_vias++;

        int net_max = 0;
        foreach (node_data *node, GV->all_nodes)
                if (node->connected_pin_or_pad_or_via)
                        {
                        myGraphicsItem *obj = (myGraphicsItem *) node->connected_pin_or_pad_or_via;
                        if (obj->tipo == O_PAD)
                                {
                                if (node->net_id > net_max)
                                        net_max = node->net_id;
                                }
                        }

        int info_all_ratsnests = 0;
        int info_nets = 0;
        for (int i = 1; i <= net_max; i++)
                {
                int net_num = 0;
                foreach (node_data *node, GV->all_nodes)
                        if (node->connected_pin_or_pad_or_via)
                                {
                                myGraphicsItem *obj = (myGraphicsItem *) node->connected_pin_or_pad_or_via;
                                if (obj->tipo == O_PAD)
                                        if (node->net_id == i)
                                                net_num++;
                                }
                if (net_num > 1)
                        {
                        info_nets++;
                        info_all_ratsnests += net_num - 1;
                        }
                }

        info_active_ratsnests = ratsnest.count();
        info_wired_ratsnests = info_all_ratsnests - info_active_ratsnests;

        info_pads_vias_nets.setText(tr("%n Pad(s)", "", info_pads) + ", " + tr("%n Via(s)", "", info_vias) + ", " + tr("%n Net(s)", "", info_nets));

        info_ratsnests.setText(tr("%n Wired Ratsnests(s)", "", info_wired_ratsnests) + ", " + tr("%n Active Ratsnests(s)", "", info_active_ratsnests));

        if (info.count() == 0)
                {
                QListWidgetItem *item;
                item = new QListWidgetItem(QIcon(":/Resources/information.png"), tr("Netlist updated succesfully"));
                info.insertItem(0, item);
                }
        info.addItem("-------------------------");
        info.addItem((tr("%n Error(s)", "", num_error) + ", " + tr("%n Warning(s)", "", num_warning)));
        info.scrollToBottom();   //Sposto la scroll bar al fondo
}


