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

#include "win_devices.h"

extern	win_main		*w_main;

extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

extern	preview	*comp_preview;

extern	myGraphicsView	*preview_GV;
extern	myGraphicsScene	*preview_GS;

extern	QString	directory_pff;

extern  QRegExp exp_exclude_separator;

win_devices_edit::win_devices_edit(QWidget *parent, Qt::WFlags tipo, int cr, QString device_text, QString footprint_text)
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
                device->setText(device_text);
                footprint->setText(footprint_text);
		}
	else
		{
                device->setText("");
                footprint->setText("");
		}
}


QGridLayout *win_devices_edit::setup_general()
{
	QGridLayout *layout_general = new QGridLayout();		
	
        QLabel	*Device = new QLabel(tr("Device"));
        device = new QLineEdit(this);
        device->setValidator(new QRegExpValidator(exp_exclude_separator, this));        //Uso new perchè ogni volta bisogna ricreare QRegExpValidator (non si può utilizzare l'oggetto altrove...)
        QLabel	*Footprint = new QLabel(tr("Footprint"));
        footprint = new QLineEdit(this);
        footprint->setValidator(new QRegExpValidator(exp_exclude_separator, this));     //Uso new perchè ogni volta bisogna ricreare QRegExpValidator (non si può utilizzare l'oggetto altrove...)
        QToolButton	*footprint_change = new QToolButton(this);
        footprint_change->setIcon(QIcon(":/Resources/folder.png"));
        footprint_change->setAutoRaise(TRUE);
        footprint_change->setCheckable(FALSE);
        connect(footprint_change,SIGNAL(clicked()),this,SLOT(change_footprint()));

        layout_general->addWidget(Device, 0, 0);
        layout_general->addWidget(device, 1, 0);
        layout_general->addWidget(Footprint, 0, 1);
        layout_general->addWidget(footprint, 1, 1);
        layout_general->addWidget(footprint_change, 1, 2);
	layout_general->setRowStretch(2, TRUE);
	
	return layout_general;
}

QHBoxLayout	*win_devices_edit::setup_buttons()
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

void	win_devices_edit::change_footprint()
{
        win_add_component dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, TRUE);
        dialogo.filepath_write(footprint->text());
        if (dialogo.exec())
                {
                if (comp_preview->load(directory_pff + "/" + dialogo.filepath_read(), FT_PROJECT, FALSE))
                        if (!GV->schcomp_or_pcbcomp->footprint_compatibility(preview_GV->all_comp.first()))
                                {
                                QMessageBox::warning(0,tr("Warning"),tr("Wrong footprint: pad numbers incompatible."));
                                return;
                                }
                footprint->setText(dialogo.filepath_read());
                }
}


void	win_devices_edit::accept()
{
        //Forse qui prima di chiudere dovrei verificare che il footprint esista...

        QDialog::accept();
}
