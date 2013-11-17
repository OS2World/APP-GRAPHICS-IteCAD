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

#include "win_ungated_pins.h"

extern	win_main		*w_main;
extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

extern  char	file_type;

extern  QRegExp exp_pin_number;
extern  QRegExp exp_exclude_separator;

win_ungated_pins_edit::win_ungated_pins_edit(QWidget *parent, Qt::WFlags tipo, int cr, QString pin_text, QString net_text)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help			//Non serve perchè uso tw->setUsesScrollButtons(FALSE); 
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni		//Non serve perchè uso tw->setUsesScrollButtons(FALSE); 

        setModal(true);

	//Imposto la larghezza minima pari a metà dello schermo
	//QRect screen = QApplication::desktop()->screenGeometry();
	//setMinimumWidth(screen.width() / 2);

	QGridLayout	*layout = new QGridLayout(this);
	layout->addLayout(setup_general(),0,0);
	layout->addLayout(setup_buttons(),1,0);
	layout->setSpacing(6);
        layout->setMargin(8);

	if (cr >= 0)
		{
                pin->setText(pin_text);
                net->setText(net_text);
		}
	else
		{
		pin->setText("");
		net->setText("");
		}
}


QGridLayout *win_ungated_pins_edit::setup_general()
{
	QGridLayout *layout_general = new QGridLayout();		
	
	QLabel	*Pin = new QLabel(tr("Pin"));
	pin = new QLineEdit(this);	
        pin->setValidator(new QRegExpValidator(exp_pin_number, this));  //Uso new perchè ogni volta bisogna ricreare QRegExpValidator (non si può utilizzare l'oggetto altrove...)
	QLabel	*Net = new QLabel(tr("Net"));
	net = new QLineEdit(this);	
        net->setValidator(new QRegExpValidator(exp_exclude_separator, this));  //Uso new perchè ogni volta bisogna ricreare QRegExpValidator (non si può utilizzare l'oggetto altrove...)

        if (file_type == FT_PROJECT)
                pin->setEnabled(FALSE);

	layout_general->addWidget(Pin, 0, 0);
	layout_general->addWidget(pin, 1, 0);
	layout_general->addWidget(Net, 0, 1);
	layout_general->addWidget(net, 1, 1);
	layout_general->setRowStretch(2, TRUE);
	
	return layout_general;
}

QHBoxLayout	*win_ungated_pins_edit::setup_buttons()
{
	QHBoxLayout	*L_comandi = new QHBoxLayout();
	L_comandi->setSpacing(6);
	L_comandi->setMargin(0);
	QSpacerItem	*L_comandi_molla = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	L_comandi->addItem(L_comandi_molla);
	QPushButton	*L_comandi_OK = new QPushButton(this);
	L_comandi_OK->setText(tr("Ok"));
	L_comandi->addWidget(L_comandi_OK);
	QPushButton	*L_comandi_Cancel = new QPushButton(this);
	L_comandi_Cancel->setText(tr("Cancel"));
	L_comandi->addWidget(L_comandi_Cancel);

	QObject::connect(L_comandi_OK, SIGNAL(clicked()), this, SLOT(accept()));
	QObject::connect(L_comandi_Cancel, SIGNAL(clicked()), this, SLOT(reject()));

	return L_comandi;
}

bool	win_ungated_pins_edit::is_pin_ok(int pin_number)
{
        if (file_type == FT_PROJECT)
                foreach(myGraphicsItem *obj, GV->selected_comp.first()->all_obj)
                        {
                        if (obj->tipo == O_PIN)
                                if (obj->number == pin_number)
                                        return FALSE;
                        }
        else
                foreach(myGraphicsItem *obj, GS->all_obj)
                        {
                        if (obj->tipo == O_PIN)
                                if (obj->number == pin_number)
                                        return FALSE;
                        }

        return TRUE;
}

void	win_ungated_pins_edit::accept()
{
	int pin_number = pin->text().toInt();
	QString net_string = net->text().trimmed();

	if (pin_number <= 0)
		{
		QMessageBox::warning(w_main, "Itecad", QObject::tr("Invalid Pin. Change it."));
		return;
		}

	if (net_string.isEmpty())
		{
		QMessageBox::warning(w_main, "Itecad", QObject::tr("Invalid Net. Change it."));
		return;
		}

	if (!is_pin_ok(pin_number))
		{
		QMessageBox::warning(w_main, "Itecad", QObject::tr("Duplicated Pin. Change it."));
		return;
		}

	QDialog::accept();
}
