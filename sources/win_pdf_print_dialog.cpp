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

#include "win_pdf_print_dialog.h"

extern	stampa_formato stampa_formati[];

win_pdf_print_dialog::win_pdf_print_dialog(QPrinter *printer, QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

	setWindowTitle(tr("PDF Page Setup"));	
	//setSizeGripEnabled(true);
	setModal(true);

	pdf_printer = printer;

	QGridLayout	*layout = new QGridLayout(this);
	
        QPrinter::PaperSize  ps = pdf_printer->paperSize();
	for (int i = 0; i< STAMPA_FORMATI_COUNT; i++)
		{
                if (stampa_formati[i].formato == ps)
                        pdf_page = new layout_size(i, (pdf_printer->widthMM()*100000)/25.4, (pdf_printer->heightMM()*100000)/25.4, TRUE);
		}

	layout->addLayout(pdf_page,0,0);
	layout->addLayout(setup_buttons(),1,0);
	layout->setSpacing(6);
    layout->setMargin(8);
}

QHBoxLayout	*win_pdf_print_dialog::setup_buttons()
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

void	win_pdf_print_dialog::accept()
{
        pdf_printer->setPaperSize(stampa_formati[pdf_page->page_size].formato);
	if (pdf_page->size_Width->length < pdf_page->size_Height->length)
		pdf_printer->setOrientation(QPrinter::Portrait);
	else
		pdf_printer->setOrientation(QPrinter::Landscape);

	QDialog::accept();
}
