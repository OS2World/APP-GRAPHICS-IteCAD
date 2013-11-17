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

#include "win_find_text.h"

win_find_text::win_find_text(QWidget *parent, Qt::WFlags tipo, QTextEdit *te_input)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

        te = te_input;

        setWindowTitle(tr("Find Text"));
        setModal(false);

        QGridLayout	*layout = new QGridLayout(this);

        layout->addLayout(setup_options(), 0, 0);
        layout->addLayout(setup_buttons(), 1, 0);

        layout->setSpacing(6);
        layout->setMargin(8);
}

QGridLayout *win_find_text::setup_options()
{
        QGridLayout	*layout_general = new QGridLayout();

        QHBoxLayout *text_row = new QHBoxLayout();
        QLabel *label = new QLabel(tr("Text"));
        text = new QLineEdit();
        text_row->addWidget(label);
        text_row->addWidget(text);
        QHBoxLayout *options_row = new QHBoxLayout();
        case_sensitive = new QCheckBox(tr("Case sensitive"));
        QGroupBox *groupBox = new QGroupBox(tr("Direction"));
        QGridLayout	*layout_direction = new QGridLayout();
        up = new QRadioButton(tr("Up"));
        down = new QRadioButton(tr("Down"));
        down->setChecked(TRUE);
        layout_direction->addWidget(up, 0, 0);
        layout_direction->addWidget(down, 0, 1);
        groupBox->setLayout(layout_direction);
        options_row->addWidget(case_sensitive);
        options_row->addWidget(groupBox);

        layout_general->addLayout(text_row, 0, 0);
        layout_general->addLayout(options_row, 1, 0);

        return layout_general;
}

QHBoxLayout	*win_find_text::setup_buttons()
{
        QHBoxLayout	*L_comandi = new QHBoxLayout();
	L_comandi->setSpacing(6);
	L_comandi->setMargin(0);
        QSpacerItem	*L_comandi_molla = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        L_comandi->addItem(L_comandi_molla);
        QPushButton	*L_comandi_Next = new QPushButton(this);
        L_comandi_Next->setText(tr("Find Next"));
        L_comandi->addWidget(L_comandi_Next);
        QObject::connect(L_comandi_Next, SIGNAL(clicked()), this, SLOT(find()));
        QPushButton	*L_comandi_Cancel = new QPushButton(this);
        L_comandi_Cancel->setText(tr("Cancel"));
        L_comandi->addWidget(L_comandi_Cancel);
        QObject::connect(L_comandi_Cancel, SIGNAL(clicked()), this, SLOT(reject()));

        L_comandi_Cancel->setAutoDefault(TRUE);
	
	return L_comandi;
}

void win_find_text::reject()
{
        QDialog::reject();
}

void win_find_text::find()
{
        QTextDocument::FindFlags flags;
        if (case_sensitive->isChecked())
                flags = QTextDocument::FindCaseSensitively;
        if (up->isChecked())
                flags = flags | QTextDocument::FindBackward;
        te->find(text->text(), flags);
}






