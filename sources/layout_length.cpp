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

#include "layout_length.h"

extern	unsigned char UM;
extern	UM_description	UM_descriptions[UM_DESCRIPTION_COUNT];

extern  QRegExp exp_length;
extern  QRegExp exp_positive_length;

layout_length::layout_length(QString description, int length_input, int min, int max, bool use_combobox_input)
        : QHBoxLayout()
{	
        use_combobox = use_combobox_input;
        is_valid = TRUE;
	length = length_input;
	min_length = min;
	max_length = max;
	um = UM;
	
        if (use_combobox)
		{
		C = new QComboBox();
		C->addItem(tr("Normal"));
		C->addItem(tr("SemiBold"));
		C->addItem(tr("Bold"));
                setC(length);
                connect(C, SIGNAL(currentIndexChanged(int)), this, SLOT(C_changed(int)));
		}
	else
		{
		T = new QLineEdit();
		T->setText(from_001thou_to_string(length, um));
		connect(T, SIGNAL(textChanged(const QString &)), this, SLOT(T_changed(const QString &)));
		normal_palette = new QPalette(T->palette());
		red_palette = new QPalette(T->palette());
		red_palette->setColor(QPalette::Text, QColor(255,0,0));
		if (min_length < 0)
                        T->setValidator(new QRegExpValidator(exp_length, this));            //Uso new perchè ogni volta bisogna ricreare QRegExpValidator (non si può utilizzare l'oggetto altrove...)
		else
                        T->setValidator(new QRegExpValidator(exp_positive_length, this));   //Uso new perchè ogni volta bisogna ricreare QRegExpValidator (non si può utilizzare l'oggetto altrove...)
		T->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		}
	
	if (description != "")
		D = new QLabel(description);
	else
		D = 0;	//Serve per sicurezza nel setDisabled()

	U = new QLabel();
	U->setText(UM_descriptions[um].descrizione);
	
	QSpacerItem	*molla = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

	if (description != "")
		this->addWidget(D);
	if (use_combobox)
		{
		this->addWidget(C);
		}
	else
		{
		this->addWidget(T);
		this->addWidget(U);
		}

	this->addItem(molla);
}

void layout_length::T_changed(const QString &stringa)
{
        int temp_int = 0;
        QString temp_string = T->text();
        switch (T->validator()->validate(temp_string, temp_int))
	{
	case QValidator::Acceptable:
	case QValidator::Intermediate:
		length = from_string_to_001thou(T->text(), um);
		if (length > max_length)
			{
			is_valid = FALSE;
			length = max_length;
			T->setPalette(*red_palette);
			}
		else if (length < min_length)
			{
			is_valid = FALSE;
			length = min_length;
			T->setPalette(*red_palette);
			}
		else
			{
			is_valid = TRUE;
			T->setPalette(*normal_palette);
			}
		break;
	case QValidator::Invalid:
		is_valid = FALSE;
		length = min_length;
		break;
	}
}

void layout_length::C_changed(int)
{
	is_valid = TRUE;
        switch (C->currentIndex())
	{
	case 0:
		length = SCH_PEN_NORMAL;
		break;
	case 1:
		length = SCH_PEN_SEMIBOLD;
		break;
	case 2:
		length = SCH_PEN_BOLD;
		break;
	}
}

bool	layout_length::warning_not_valid_parameter(QString parameter)
{
	if (!is_valid)
		{
		QMessageBox::warning(0,tr("Warning"),tr("%1 parameter is not valid.").arg(parameter));
		return TRUE;
		}
	else
		return FALSE;
}

void layout_length::setDisabled(bool flag)
{
	if (D)
		D->setDisabled(flag);
        if (use_combobox)
                C->setDisabled(flag);
        else
                {
                T->setDisabled(flag);
                U->setDisabled(flag);
                }
}

void layout_length::setEnabled(bool flag)
{
	setDisabled(!flag);	
}

void layout_length::setLength(int new_length)
{
        length = new_length;
        T->setText(from_001thou_to_string(length, um));
}

void layout_length::setC(int new_length)
{
        length = new_length;
        if (length <= SCH_PEN_NORMAL)
                C->setCurrentIndex(0);
        else if (length <= SCH_PEN_SEMIBOLD)
                C->setCurrentIndex(1);
        else
                C->setCurrentIndex(2);
}


void layout_length::setUM(char new_um)
{
	um = new_um;
	T->setText(from_001thou_to_string(length, um));
	U->setText(UM_descriptions[um].descrizione);	
}
