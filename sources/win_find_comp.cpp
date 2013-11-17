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

#include "win_find_comp.h"

extern	win_main	*w_main;
extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

extern	QList<myGraphicsView *> doc_sch;
extern  QList<myGraphicsView *> doc_sch_and_pcb;
extern	myGraphicsView		*doc_pcb;

win_find_comp::win_find_comp(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

        setWindowTitle(tr("Find Component"));
        setModal(true);

        QGridLayout	*layout = new QGridLayout(this);

        layout->addWidget(setup_options(), 0, 0);
        layout->addLayout(setup_buttons(),1,0,TRUE,FALSE);

        layout->setSpacing(6);
        layout->setMargin(8);
}

QGroupBox *win_find_comp::setup_options()
{
        QGroupBox *groupBox = new QGroupBox(tr("Find into"));
        QGridLayout	*layout_general = new QGridLayout();

        QLabel *sch_comp_image = new QLabel();
        sch_comp_image->setPixmap(QPixmap(":/Resources/schematic.png"));
        QLabel *sch_comp_label = new QLabel(this);
        sch_comp_label->setText(tr("Schematic documents:"));
        sch_comp = new QComboBox();
        for (int i = 0; i < doc_sch.count(); i++)
               foreach (myComponent *comp, doc_sch.at(i)->all_comp)
                        insert_alphabetically(sch_comp, comp->ident);
        sch_comp->setCurrentIndex(0);
        sch_comp->setEditable(FALSE);
        QToolButton	*sch_comp_find = new QToolButton(this);
        sch_comp_find->setIcon(QIcon(":/Resources/find.png"));
        sch_comp_find->setAutoRaise(TRUE);
        sch_comp_find->setCheckable(FALSE);
        connect(sch_comp_find,SIGNAL(clicked()),this,SLOT(find_sch_comp()));
        layout_general->addWidget(sch_comp_image, 0, 0);
        layout_general->addWidget(sch_comp_label, 0, 1);
        layout_general->addWidget(sch_comp, 0, 2);
        layout_general->addWidget(sch_comp_find, 0, 3);

        QSpacerItem *spazio = new QSpacerItem(20,20);
        layout_general->addItem(spazio,1,0);

        QLabel *pcb_comp_image = new QLabel();
        pcb_comp_image->setPixmap(QPixmap(":/Resources/board.png"));
        QLabel *pcb_comp_label = new QLabel(this);
        pcb_comp_label->setText(tr("Board document:"));
        pcb_comp = new QComboBox();
        foreach (myComponent *comp, doc_pcb->all_comp)
                 insert_alphabetically(pcb_comp, comp->ident);
        pcb_comp->setCurrentIndex(0);
        pcb_comp->setEditable(FALSE);
        QToolButton	*pcb_comp_find = new QToolButton(this);
        pcb_comp_find->setIcon(QIcon(":/Resources/find.png"));
        pcb_comp_find->setAutoRaise(TRUE);
        pcb_comp_find->setCheckable(FALSE);
        connect(pcb_comp_find,SIGNAL(clicked()),this,SLOT(find_pcb_comp()));
        layout_general->addWidget(pcb_comp_image, 2, 0);
        layout_general->addWidget(pcb_comp_label, 2, 1);
        layout_general->addWidget(pcb_comp, 2, 2);
        layout_general->addWidget(pcb_comp_find, 2, 3);

        layout_general->setColumnMinimumWidth(2,100);

        groupBox->setLayout(layout_general);

        return groupBox;
}

QHBoxLayout	*win_find_comp::setup_buttons()
{
	QHBoxLayout	*L_comandi = new QHBoxLayout();
	L_comandi->setSpacing(6);
	L_comandi->setMargin(0);
        QSpacerItem	*L_comandi_molla = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	L_comandi->addItem(L_comandi_molla);
        QPushButton	*L_comandi_Cancel = new QPushButton(this);
        L_comandi_Cancel->setText(tr("Cancel"));
        L_comandi->addWidget(L_comandi_Cancel);
	
        QObject::connect(L_comandi_Cancel, SIGNAL(clicked()), this, SLOT(reject()));

        L_comandi_Cancel->setAutoDefault(TRUE);
	
	return L_comandi;
}

void win_find_comp::reject()
{
        QDialog::reject();
}

void win_find_comp::find_sch_comp()
{
        if (sch_comp->currentText().isNull())
                return;

        for (int i = 0; i < doc_sch_and_pcb.count(); i++)
                if (doc_sch_and_pcb.at(i)->is_sch)
                        foreach (myComponent *comp, doc_sch_and_pcb.at(i)->all_comp)
                                if (comp->ident == sch_comp->currentText())
                                        {
                                        w_main->tw->setCurrentIndex(i);
                                        QRect r = comp->bounding_rect();
                                        QPoint margine = QPoint(r.width() * 0.5, r.height() * 0.5);
                                        r.setTopLeft(r.topLeft() - margine);
                                        r.setBottomRight(r.bottomRight() + margine);
                                        GV->zoom_rect(r);
                                        QDialog::reject();
                                        }
}

void win_find_comp::find_pcb_comp()
{
        if (pcb_comp->currentText().isNull())
                return;

        for (int i = 0; i < doc_sch_and_pcb.count(); i++)
                if (!doc_sch_and_pcb.at(i)->is_sch)
                        foreach (myComponent *comp, doc_sch_and_pcb.at(i)->all_comp)
                                if (comp->ident == pcb_comp->currentText())
                                        {
                                        w_main->tw->setCurrentIndex(i);
                                        QRect r = comp->bounding_rect();
                                        QPoint margine = QPoint(r.width() * 0.5, r.height() * 0.5);
                                        r.setTopLeft(r.topLeft() - margine);
                                        r.setBottomRight(r.bottomRight() + margine);
                                        GV->zoom_rect(r);
                                        QDialog::reject();
                                        }
}

void win_find_comp::insert_alphabetically(QComboBox * cb, QString text)
{
        for (int i = 0; i < cb->count(); i++)
                {
                bool    last_number_sort = FALSE;
                int     ln = last_number_position(text);
                if (ln == last_number_position(cb->itemText(i)))
                        if (text.left(ln) == cb->itemText(i).left(ln))
                                last_number_sort = TRUE;


                if (last_number_sort)
                        {
                        if (last_number(text, ln) < last_number(cb->itemText(i),ln))
                                {
                                cb->insertItem(i, text);
                                return;
                                }
                        }
                else if (text < cb->itemText(i))
                        {
                        cb->insertItem(i, text);
                        return;
                        }
                }
        cb->insertItem(cb->count(), text);
}

int win_find_comp::last_number(QString text, int last_number_position)
{
        int result = 0;
        for (int i = last_number_position; i < text.length(); i++)
                result = result * 10 + text.at(i).digitValue();
        return result;
}

int win_find_comp::last_number_position(QString text)
{
        for (int i = text.length() - 1; i >= 0; i--)
                if (!text.at(i).isNumber())
                        {
                        i++;
                        return i;
                        }
        return 0;
}






