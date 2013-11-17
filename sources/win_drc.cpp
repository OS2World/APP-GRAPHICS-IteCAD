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

#include "win_drc.h"

extern	char file_type;	
extern	win_main	*w_main;

extern	bool	drc_active;
extern	bool	drc_check_low_insulation;
extern	int		drc_v2v;
extern	int		drc_v2l;
extern	int		drc_l2l;
extern	int		check_v2v;
extern	int		check_v2l;
extern	int		check_l2l;

win_drc::win_drc(QWidget *parent, Qt::WFlags tipo, bool	insulation_check_input)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help			//Non serve perchè uso tw->setUsesScrollButtons(FALSE); 
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni		//Non serve perchè uso tw->setUsesScrollButtons(FALSE); 

	insulation_check = insulation_check_input;

	if (insulation_check)
		setWindowTitle(tr("Insulation Check"));	
	else
		setWindowTitle(tr("Realtime DRC"));	
	//setSizeGripEnabled(true);
    setModal(true);

	//Imposto la larghezza minima pari a metà dello schermo
	//QRect screen = QApplication::desktop()->screenGeometry();
	//setMinimumWidth(screen.width() / 2);

	QGridLayout	*layout = new QGridLayout(this);
	if (insulation_check)
		{
		layout->addWidget(setup_insulation(),0,0);
		layout->addLayout(setup_buttons(),1,0);
		}
	else
		{
		layout->addWidget(setup_realtime_drc(),0,0);
		layout->addWidget(setup_insulation(),1,0);
		layout->addLayout(setup_buttons(),2,0);
		}
	layout->setSpacing(6);
    layout->setMargin(8);

	if (!insulation_check)
		enable_disable(0);
}


QGroupBox *win_drc::setup_realtime_drc()
{
	QGroupBox *gb_online_drc = new QGroupBox(tr("General"));
	QGridLayout *layout_online_drc = new QGridLayout();		
	
	active = new QCheckBox(tr("Enable Realtime Design Rule Checking (DRC)"));
	active->setChecked(drc_active);
	connect(active,SIGNAL(stateChanged(int)),this,SLOT(enable_disable(int)));
	
	layout_online_drc->addWidget(active, 0, 0);
	layout_online_drc->setRowStretch(1,TRUE);
	gb_online_drc->setLayout(layout_online_drc);

	return gb_online_drc;
}

QGroupBox *win_drc::setup_insulation()
{
	QGroupBox *gb_insulation = new QGroupBox(tr("Insulation"));
	QGridLayout *layout_insulation = new QGridLayout();		
	
	if (!insulation_check)
		{
		low_insulation = new QCheckBox(tr("Check low insulation"));
		low_insulation->setChecked(drc_check_low_insulation);
		connect(low_insulation,SIGNAL(stateChanged(int)),this,SLOT(enable_disable(int)));
		}

	QLabel *l2l_text = new QLabel(this);
	l2l_text->setText(tr("Line to Line:"));
	l2l_text->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	if (insulation_check)
		l2l = new layout_length("", check_l2l, 1, 100000, FALSE);
	else
		l2l = new layout_length("", drc_l2l, 1, 100000, FALSE);
	QLabel *v2l_text = new QLabel(this);
	v2l_text->setText(tr("Line to Via:"));
	v2l_text->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	if (insulation_check)
		v2l = new layout_length("", check_v2l, 1, 100000, FALSE);
	else
		v2l = new layout_length("", drc_v2l, 1, 100000, FALSE);
	QLabel *v2v_text = new QLabel(this);
	v2v_text->setText(tr("Via to Via:"));
	v2v_text->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	if (insulation_check)
		v2v = new layout_length("", check_v2v, 1, 100000, FALSE);
	else
		v2v = new layout_length("", drc_v2v, 1, 100000, FALSE);

	if (!insulation_check)
		layout_insulation->addWidget(low_insulation, 0, 0, TRUE, FALSE);
	QSpacerItem *spazio1 = new QSpacerItem(20,20);
	layout_insulation->addItem(spazio1,1,0);
	layout_insulation->addWidget(l2l_text, 1, 1);
	layout_insulation->addLayout(l2l, 1, 2);
	QSpacerItem *spazio2 = new QSpacerItem(20,20);
	layout_insulation->addItem(spazio2,2,0);
	layout_insulation->addWidget(v2l_text, 2, 1);
	layout_insulation->addLayout(v2l, 2, 2);
	QSpacerItem *spazio3 = new QSpacerItem(20,20);
	layout_insulation->addItem(spazio3,3,0);
	layout_insulation->addWidget(v2v_text, 3, 1);
	layout_insulation->addLayout(v2v, 3, 2);
	layout_insulation->setRowStretch(4,TRUE);
	gb_insulation->setLayout(layout_insulation);

	return gb_insulation;
}

QHBoxLayout	*win_drc::setup_buttons()
{
	QHBoxLayout	*L_comandi = new QHBoxLayout();
	L_comandi->setSpacing(6);
	L_comandi->setMargin(0);
        QSpacerItem	*L_comandi_molla = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	L_comandi->addItem(L_comandi_molla);
	QPushButton	*L_comandi_OK = new QPushButton(this);
	if (insulation_check)
		L_comandi_OK->setText(tr("Check"));
	else
		L_comandi_OK->setText(tr("Ok"));
	L_comandi->addWidget(L_comandi_OK);
	QPushButton	*L_comandi_Cancel = new QPushButton(this);
	L_comandi_Cancel->setText(tr("Cancel"));
	L_comandi->addWidget(L_comandi_Cancel);

	QObject::connect(L_comandi_OK, SIGNAL(clicked()), this, SLOT(accept()));
	QObject::connect(L_comandi_Cancel, SIGNAL(clicked()), this, SLOT(reject()));

	return L_comandi;
}

void	win_drc::accept()
{
	if (insulation_check)
		{
		check_l2l = l2l->length;
		check_v2l = v2l->length;
		check_v2v = v2v->length;

		swap_int(check_l2l, drc_l2l);
		swap_int(check_v2l, drc_v2l);
		swap_int(check_v2v, drc_v2v);

                w_main->net_id_update_pcb();
                drc_insulation_on_all_objects();

		swap_int(check_l2l, drc_l2l);
		swap_int(check_v2l, drc_v2l);
		swap_int(check_v2v, drc_v2v);
		}
	else
		{
		drc_active = active->isChecked();
		drc_check_low_insulation = low_insulation->isChecked();
		drc_l2l = l2l->length;
		drc_v2l = v2l->length;
		drc_v2v = v2v->length;
		//Aggiorno anche i valori di check
		check_v2v = drc_v2v;
		check_v2l = drc_v2l;
		check_l2l = drc_l2l;
		}
	
	QDialog::accept();
}

void	win_drc::enable_disable(int inutile)
{
	bool flag;
	
	if (!active->isChecked())
		{
		low_insulation->setEnabled(FALSE);
		flag = FALSE;
		}
	else
		{
		low_insulation->setEnabled(TRUE);
		flag = active->isChecked() && low_insulation->isChecked();
		}

	v2v->setEnabled(flag);
	v2l->setEnabled(flag);
	l2l->setEnabled(flag);
}
