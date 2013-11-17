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

#include "win_converter.h"

extern	unsigned char UM;
extern	UM_description	UM_descriptions[UM_DESCRIPTION_COUNT];

extern  QRegExp exp_converter;

struct Unit_Conversion 
{	
	QString	descrizione;
	float	k;
};

Unit_Conversion	UC[9] = 
{ 
        {"cm",					10000.0},	//0
        {"dm",					100000.0},	//1
        {"feet",				304800.0},	//2
        {"inch",				25400.0},	//3
        {"metre",				1000000.0},	//4
        {"micron",				1.0},		//5
        {"mm",					1000.0},		//6
        {"thou",				25.4},		//7
        {"yards",				914400.0}	//8
};

win_converter::win_converter(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

	setWindowTitle(tr("Length Unit Converter"));	
	//setSizeGripEnabled(true);
	setModal(true);

	QGridLayout	*layout = new QGridLayout(this);
	
	T = new QLineEdit(this);
        T->setValidator(new QRegExpValidator(exp_converter, this)); //Uso new perchè ogni volta bisogna ricreare QRegExpValidator (non si può utilizzare l'oggetto altrove...)
	T->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	T->setText("1");
	connect(T, SIGNAL(textEdited(const QString &)), this, SLOT(T_edited(const QString &)));
	
	U = new QComboBox(this);
	for (int i = 0; i < 9; i++)
		{
		U->insertItem(i, UC[i].descrizione);
		if (UM_descriptions[UM].descrizione == UC[i].descrizione)
			U->setCurrentIndex(i);
		}
	connect(U,SIGNAL(activated(int)),this,SLOT(U_change(int)));

	layout->addWidget(T,0,0);
	layout->addWidget(U,0,1);
	layout->addWidget(setup_metric_group(),2,0,TRUE,FALSE);
	layout->addWidget(setup_imperial_group(),3,0,TRUE,FALSE);
	layout->addLayout(setup_buttons(),5,0, TRUE, FALSE);
	layout->setSpacing(6);
    layout->setMargin(8);

	T_edited("");
}

QGroupBox *win_converter::setup_imperial_group()
{
	QGroupBox *groupBox = new QGroupBox(tr("Imperial System"));

	QGridLayout	*layout_imperial = new QGridLayout();

	feet1 = new QLabel(this);
	feet1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	feet2 = new QLabel(this);
	feet2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	feet2->setText(UC[2].descrizione);
	inch1 = new QLabel(this);
	inch1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	inch2 = new QLabel(this);
	inch2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	inch2->setText(UC[3].descrizione);
	thou1 = new QLabel(this);
	thou1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	thou2 = new QLabel(this);
	thou2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	thou2->setText(UC[7].descrizione);
	yards1 = new QLabel(this);
	yards1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	yards2 = new QLabel(this);
	yards2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	yards2->setText(UC[8].descrizione);

	layout_imperial->addWidget(feet1,0,0);
	layout_imperial->addWidget(feet2,0,1);
	layout_imperial->addWidget(inch1,1,0);
	layout_imperial->addWidget(inch2,1,1);
	layout_imperial->addWidget(thou1,2,0);
	layout_imperial->addWidget(thou2,2,1);
	layout_imperial->addWidget(yards1,3,0);
	layout_imperial->addWidget(yards2,3,1);
	groupBox->setLayout(layout_imperial);

	return groupBox;
}

QGroupBox *win_converter::setup_metric_group()
{
	QGroupBox *groupBox = new QGroupBox(tr("Metric System"));

	QGridLayout	*layout_metric = new QGridLayout();

	cm1 = new QLabel(this);
	cm1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	cm2 = new QLabel(this);
	cm2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	cm2->setText(UC[0].descrizione);
	dm1 = new QLabel(this);
	dm1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	dm2 = new QLabel(this);
	dm2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	dm2->setText(UC[1].descrizione);
	metre1 = new QLabel(this);
	metre1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	metre2 = new QLabel(this);
	metre2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	metre2->setText(UC[4].descrizione);
	micron1 = new QLabel(this);
	micron1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	micron2 = new QLabel(this);
	micron2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	micron2->setText(UC[5].descrizione);
	mm1 = new QLabel(this);
	mm1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	mm2 = new QLabel(this);
	mm2->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	mm2->setText(UC[6].descrizione);

	layout_metric->addWidget(cm1,0,0);
	layout_metric->addWidget(cm2,0,1);	
	layout_metric->addWidget(dm1,1,0);
	layout_metric->addWidget(dm2,1,1);
	layout_metric->addWidget(metre1,2,0);
	layout_metric->addWidget(metre2,2,1);
	layout_metric->addWidget(micron1,3,0);
	layout_metric->addWidget(micron2,3,1);
	layout_metric->addWidget(mm1,4,0);
	layout_metric->addWidget(mm2,4,1);	
	groupBox->setLayout(layout_metric);

	return groupBox;
}

QHBoxLayout	*win_converter::setup_buttons()
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

void win_converter::T_edited(const QString &stringa)
{
	//Qui ho riscritto e leggermente riadattato le funzioni:
	//	from_string_to_001thou()
	//	from_001thou_to_string()
	
	float length_mm = T->text().toFloat() * UC[U->currentIndex()].k;
	
	//Metric System
	cm1->setText(QString("%1").arg(length_mm / UC[0].k));
	dm1->setText(QString("%1").arg(length_mm / UC[1].k));
	metre1->setText(QString("%1").arg(length_mm / UC[4].k));
	micron1->setText(QString("%1").arg(length_mm));
	mm1->setText(QString("%1").arg(length_mm / UC[6].k));

	//Imperial System
	feet1->setText(QString("%1").arg(length_mm / UC[2].k));
	inch1->setText(QString("%1").arg(length_mm / UC[3].k));
	thou1->setText(QString("%1").arg(length_mm / UC[7].k));
	yards1->setText(QString("%1").arg(length_mm / UC[8].k));
}

void win_converter::U_change(int inutile)
{
	T_edited("");
}

