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

#include "win_page.h"

extern	char	file_type;
extern	win_main	*w_main;
extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

win_page::win_page(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

        setWindowTitle(tr("Page Format and Labelling"));
        //setSizeGripEnabled(true);
        setModal(true);

	//Imposto la larghezza minima pari a metà dello schermo
	//QRect screen = QApplication::desktop()->screenGeometry();	//Non serve perchè uso tw->setUsesScrollButtons(FALSE); 
	//setMinimumWidth(screen.width() / 2);						//Non serve perchè uso tw->setUsesScrollButtons(FALSE); 

	QGridLayout	*layout = new QGridLayout(this);

        page = new layout_size(GV->page_size_index(), GV->page_width, GV->page_height, TRUE);
        layout->addLayout(page,0,0);
        layout->addWidget(setup_labelling(),0,1);
        layout->addLayout(setup_buttons(),1,0,TRUE,FALSE);
	layout->setSpacing(6);
        layout->setMargin(8);
}

QGroupBox *win_page::setup_labelling()
{
        QGroupBox *gb_labelling = new QGroupBox(tr("Labelling"));
        QGridLayout *layout_labelling = new QGridLayout();

        QLabel	*Organization = new QLabel(this);
        Organization->setText(tr("Organization"));
        organization = new QLineEdit(this);
        organization->setText(GV->frame_organization->text);
        unicode_input_organization = new QToolButton(this);
        unicode_input_organization->setIcon(QIcon(":/Resources/keyboard.png"));
        unicode_input_organization->setAutoRaise(TRUE);
        unicode_input_organization->setCheckable(FALSE);
        connect(unicode_input_organization,SIGNAL(clicked()),this,SLOT(unicode_input_organization_pressed()));
        layout_labelling->addWidget(Organization, 0, 0);
        layout_labelling->addWidget(organization, 0, 1);
        layout_labelling->addWidget(unicode_input_organization, 0, 2);

        QLabel	*Title = new QLabel(this);
        Title->setText(tr("Title"));
        title = new QLineEdit(this);
        title->setText(GV->frame_title->text);
        unicode_input_title = new QToolButton(this);
        unicode_input_title->setIcon(QIcon(":/Resources/keyboard.png"));
        unicode_input_title->setAutoRaise(TRUE);
        unicode_input_title->setCheckable(FALSE);
        connect(unicode_input_title,SIGNAL(clicked()),this,SLOT(unicode_input_title_pressed()));
        layout_labelling->addWidget(Title, 1, 0);
        layout_labelling->addWidget(title, 1, 1);
        layout_labelling->addWidget(unicode_input_title, 1, 2);

        QLabel	*Doc_num = new QLabel(this);
        Doc_num->setText(tr("Document Number"));
        doc_num = new QLineEdit(this);
        doc_num->setText(GV->frame_doc_num->text);
        unicode_input_doc_num = new QToolButton(this);
        unicode_input_doc_num->setIcon(QIcon(":/Resources/keyboard.png"));
        unicode_input_doc_num->setAutoRaise(TRUE);
        unicode_input_doc_num->setCheckable(FALSE);
        connect(unicode_input_doc_num,SIGNAL(clicked()),this,SLOT(unicode_input_doc_num_pressed()));
        layout_labelling->addWidget(Doc_num, 2, 0);
        layout_labelling->addWidget(doc_num, 2, 1);
        layout_labelling->addWidget(unicode_input_doc_num, 2, 2);

        QLabel	*Rev = new QLabel(this);
        Rev->setText(tr("Revision"));
        rev = new QLineEdit(this);
        rev->setText(GV->frame_rev->text);
        unicode_input_rev = new QToolButton(this);
        unicode_input_rev->setIcon(QIcon(":/Resources/keyboard.png"));
        unicode_input_rev->setAutoRaise(TRUE);
        unicode_input_rev->setCheckable(FALSE);
        connect(unicode_input_rev,SIGNAL(clicked()),this,SLOT(unicode_input_rev_pressed()));
        layout_labelling->addWidget(Rev, 3, 0);
        layout_labelling->addWidget(rev, 3, 1);
        layout_labelling->addWidget(unicode_input_rev, 3, 2);

        layout_labelling->setRowStretch(4, TRUE);
        gb_labelling->setLayout(layout_labelling);

        return gb_labelling;
}

QHBoxLayout	*win_page::setup_buttons()
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

        L_comandi_OK->setAutoDefault(TRUE);
	L_comandi_Cancel->setAutoDefault(FALSE);

	return L_comandi;
}

void	win_page::unicode_input_organization_pressed()
{
        win_unicode_input dialogo(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint,  organization->mapToGlobal(QPoint(0,organization->height())));
        dialogo.raise();
        dialogo.activateWindow();
        dialogo.set_lineedit(organization);
        if (dialogo.exec())
                {

                }
}

void	win_page::unicode_input_title_pressed()
{
        win_unicode_input dialogo(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint, title->mapToGlobal(QPoint(0,title->height())));
        dialogo.raise();
        dialogo.activateWindow();
        dialogo.set_lineedit(title);
        if (dialogo.exec())
                {

                }
}

void	win_page::unicode_input_doc_num_pressed()
{
        win_unicode_input dialogo(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint,  doc_num->mapToGlobal(QPoint(0,doc_num->height())));
        dialogo.raise();
        dialogo.activateWindow();
        dialogo.set_lineedit(doc_num);
        if (dialogo.exec())
                {

                }
}

void	win_page::unicode_input_rev_pressed()
{
        win_unicode_input dialogo(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint,  rev->mapToGlobal(QPoint(0,rev->height())));
        dialogo.raise();
        dialogo.activateWindow();
        dialogo.set_lineedit(rev);
        if (dialogo.exec())
                {

                }
}

bool    win_page::is_modified()
{
        if (page->size_Width->length != GV->page_width)
                return TRUE;
        if (page->size_Height->length != GV->page_height)
                return TRUE;
        if (organization->text() != GV->frame_organization->text)
                return TRUE;
        if (title->text() != GV->frame_title->text)
                return TRUE;
        if (doc_num->text() != GV->frame_doc_num->text)
                return TRUE;
        if (rev->text() != GV->frame_rev->text)
                return TRUE;

        return FALSE;
}

void	win_page::accept()
{
        if (is_modified())
                {
                GV->setUpdatesEnabled(FALSE);
                GV->ur.push(new change_page_and_labelling((int) this));
                GV->setUpdatesEnabled(TRUE);
                }

        QDialog::accept();
}

