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

#include "win_unicode_input.h"

#define		E_COUNT 2
#define		O_COUNT 4

CharacterWidget::CharacterWidget(QWidget *parent)
     : QWidget(parent)
{
	squareSize = 24;
	group = 'E';
	group_count = E_COUNT;
    columns = 10;
    setMouseTracking(true);
}

int		CharacterWidget::group_value(int position)
{
	///////////////////////////////////////////////
	//http://www.atm.ox.ac.uk/user/iwi/charmap.html
	///////////////////////////////////////////////
	static	int electronic[E_COUNT] = {	0x03A9,		//Omega
										0x03BC};	//Micro

	static	int others[O_COUNT] = {	0x00A9,		//Copyright
										0x00AE,		//Registered
										0x00B2,		//Quadrato
										0x00B3};	//Cubo
	
	switch (group)
	{
	case 'E':
		return electronic[position];
	case 'A':
		return others[position];
	}
}

QSize CharacterWidget::sizeHint() const
{
        return QSize(columns*squareSize, (group_count/columns + 1)*squareSize);
}

int	CharacterWidget::position(int x, int y)
{
	return (y/squareSize)*columns + x/squareSize;
}

void CharacterWidget::mouseMoveEvent(QMouseEvent *event)
{
     QPoint widgetPosition = mapFromGlobal(event->globalPos());
	 int pos = position(widgetPosition.x(), widgetPosition.y());
	 if (pos < group_count)
		 {
		 uint key = group_value(pos);

		 QString text = QString::fromLatin1("<p>Character: <span style=\"font-size: 24pt; font-family: %1\">").arg(displayFont.family())
					   + QChar(key)
					   + QString::fromLatin1("</span><p>Unicode: 0x")
					   + QString::number(key, 16);
		 QToolTip::showText(event->globalPos(), text, this);
		}
	 else
		 QToolTip::hideText();
}

void	CharacterWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		{
		int pos = position(event->x(), event->y());
		if (pos < group_count)
                        //LE->setText(LE->text() + QChar(group_value(pos)));
                        LE->insert(QChar(group_value(pos)));
		}	
}


void CharacterWidget::paintEvent(QPaintEvent *event)
{
     QPainter painter(this);
     painter.fillRect(event->rect(), QBrush(Qt::white));
     painter.setFont(displayFont);

     QRect redrawRect = event->rect();
     int beginRow = redrawRect.top()/squareSize;
     int endRow = redrawRect.bottom()/squareSize;
     int beginColumn = redrawRect.left()/squareSize;
     int endColumn = redrawRect.right()/squareSize;

     painter.setPen(QPen(Qt::gray));
     for (int row = beginRow; row <= endRow; ++row) {
         for (int column = beginColumn; column <= endColumn; ++column) {
             painter.drawRect(column*squareSize, row*squareSize, squareSize, squareSize);
         }
     }

     QFontMetrics fontMetrics(displayFont);
     painter.setPen(QPen(Qt::black));
     for (int row = beginRow; row <= endRow; ++row)
         {
         for (int column = beginColumn; column <= endColumn; ++column)
             {
             int position = row*columns + column;
             if (position < group_count)
                {
                int key = group_value(position);
                painter.setClipRect(column*squareSize, row*squareSize, squareSize, squareSize);
                painter.drawText(column*squareSize + (squareSize / 2) - fontMetrics.width(QChar(key))/2, row*squareSize + 4 + fontMetrics.ascent(), QString(QChar(key)));
                }
             }
         }
} 

win_unicode_input::win_unicode_input(QWidget *parent, Qt::WFlags tipo, QPoint winpos)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

	setWindowTitle(tr("Insert Character"));	
	//setSizeGripEnabled(true);
	setModal(true);
        move(winpos);

        cw = new CharacterWidget(this);
	QGridLayout	*layout = new QGridLayout(this);
	
	layout->addWidget(setup_group(),0,0);
	layout->addWidget(setup_list(),0,1);
	//layout->addLayout(setup_buttons(),1,0, TRUE, FALSE);
	layout->setSpacing(6);
    layout->setMargin(8);
}

QGroupBox *win_unicode_input::setup_group()
{
	QGroupBox *groupBox = new QGroupBox(tr("Group"));

	QVBoxLayout	*layout_group = new QVBoxLayout();
	electronic = new QRadioButton(tr("Electronic"));
	accented = new QRadioButton(tr("Others"));
	QObject::connect(electronic, SIGNAL(toggled(bool)), this, SLOT(set_electronic(bool)));
	QObject::connect(accented, SIGNAL(toggled(bool)), this, SLOT(set_accented(bool)));
	layout_group->addWidget(electronic);
	layout_group->addWidget(accented);
	layout_group->addStretch(1);
	electronic->setChecked(TRUE);

	groupBox->setLayout(layout_group);

	return groupBox;
}

QGroupBox *win_unicode_input::setup_list()
{
	QGroupBox *groupBox = new QGroupBox(tr("List"));

	QVBoxLayout	*layout_list = new QVBoxLayout();
        QScrollArea	*scrollArea = new QScrollArea;
        scrollArea->setWidget(cw);
	layout_list->addWidget(scrollArea);

	groupBox->setLayout(layout_list);

	return groupBox;
}

QHBoxLayout	*win_unicode_input::setup_buttons()
{
	QHBoxLayout	*L_comandi = new QHBoxLayout();
	L_comandi->setSpacing(6);
	L_comandi->setMargin(0);
	QPushButton	*L_comandi_Cancel = new QPushButton(this);
	L_comandi_Cancel->setText(tr("Cancel"));
	L_comandi->addWidget(L_comandi_Cancel);
	QSpacerItem	*L_comandi_molla = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	L_comandi->addItem(L_comandi_molla);
	QPushButton	*L_comandi_OK = new QPushButton(this);
	L_comandi_OK->setText(tr("Insert"));
	L_comandi->addWidget(L_comandi_OK);
	
	QObject::connect(L_comandi_Cancel, SIGNAL(clicked()), this, SLOT(reject()));
	QObject::connect(L_comandi_OK, SIGNAL(clicked()), this, SLOT(accept()));

	L_comandi_Cancel->setAutoDefault(FALSE);
	L_comandi_OK->setAutoDefault(TRUE);

	return L_comandi;
}

void	win_unicode_input::set_electronic(bool inutile)
{
        cw->group = 'E';
        cw->group_count = E_COUNT;
        cw->update();
}

void	win_unicode_input::set_accented(bool inutile)
{
        cw->group = 'A';
        cw->group_count = O_COUNT;
        cw->update();
}

void    win_unicode_input::set_lineedit(QLineEdit *le)
{
        cw->LE = le;

        palette = le->palette();
        QColor old_color = palette.color(QPalette::Base);
        palette.setColor(QPalette::Active, QPalette::Base, QColor(255,255,0));
        palette.setColor(QPalette::Inactive, QPalette::Base, QColor(255,255,0));
        palette.setColor(QPalette::Disabled, QPalette::Base, QColor(255,255,0));
        le->setPalette(palette);
        le->setFocus(Qt::MouseFocusReason);

        //Rimetto la palette di prima di modo da trovarmi pronto per quando chiudo la finestra
        palette.setColor(QPalette::Active, QPalette::Base, old_color);
        palette.setColor(QPalette::Inactive, QPalette::Base, old_color);
        palette.setColor(QPalette::Disabled, QPalette::Base, old_color);
}

win_unicode_input::~win_unicode_input()
{
        cw->LE->setPalette(palette);
}
