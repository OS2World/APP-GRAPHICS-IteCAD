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

#include "win_style.h"

extern	win_main		*w_main;
extern	QTextEdit		*TE;

win_style::win_style(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
{
	setModal(false);
	setWindowTitle(tr("Style"));

	QGridLayout	*layout = new QGridLayout(this);
	
	font = new QFontComboBox(this);
	size = new QComboBox(this);
	QFontDatabase db;
	foreach(int i, db.standardSizes())
		size->addItem(QString::number(i));
	bold = new QToolButton(this);
	bold->setIcon(QIcon(":/Resources/textbold.png"));
	bold->setCheckable(TRUE);
	bold->setAutoRaise(TRUE);
	connect(bold, SIGNAL(triggered()), this, SLOT(text_bold()));
	italic = new QToolButton(this);
	italic->setIcon(QIcon(":/Resources/textitalic.png"));
	italic->setCheckable(TRUE);
	italic->setAutoRaise(TRUE);
	underline = new QToolButton(this);
	underline->setIcon(QIcon(":/Resources/textunder.png"));
	underline->setCheckable(TRUE);
	underline->setAutoRaise(TRUE);
	left = new QToolButton(this);
	left->setIcon(QIcon(":/Resources/textleft.png"));
	left->setCheckable(TRUE);
	left->setAutoRaise(TRUE);
	center = new QToolButton(this);
	center->setIcon(QIcon(":/Resources/textcenter.png"));
	center->setCheckable(TRUE);
	center->setAutoRaise(TRUE);
	right = new QToolButton(this);
	right->setIcon(QIcon(":/Resources/textright.png"));
	right->setCheckable(TRUE);
	right->setAutoRaise(TRUE);
	justify = new QToolButton(this);
	justify->setIcon(QIcon(":/Resources/textjustify.png"));
	justify->setCheckable(TRUE);
	justify->setAutoRaise(TRUE);

	layout->addWidget(font, 0, 0);
	layout->addWidget(size, 0, 1);
	layout->addWidget(bold, 0, 2);
	layout->addWidget(italic, 0, 3);
	layout->addWidget(underline, 0, 4);
	layout->addWidget(left, 0, 5);
	layout->addWidget(center, 0, 6);
	layout->addWidget(right, 0, 7);
	layout->addWidget(justify, 0, 8);

	connect(TE, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)), this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
	
	connect(font, SIGNAL(activated(const QString &)), this, SLOT(change_family(const QString &)));
	connect(size, SIGNAL(activated(const QString &)), this, SLOT(change_size(const QString &)));
	connect(bold, SIGNAL(clicked()), this, SLOT(change_bold()));
	connect(italic, SIGNAL(clicked()), this, SLOT(change_italic()));
	connect(underline, SIGNAL(clicked()), this, SLOT(change_underline()));

	connect(TE, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
	connect(left, SIGNAL(clicked()), this, SLOT(change_left()));
	connect(center, SIGNAL(clicked()), this, SLOT(change_center()));
	connect(right, SIGNAL(clicked()), this, SLOT(change_right()));
	connect(justify, SIGNAL(clicked()), this, SLOT(change_justify()));

	//Carico i valori iniziali
	currentCharFormatChanged(TE->currentCharFormat());
	cursorPositionChanged();

	//layout->setSpacing(6);
    //layout->setMargin(8);
}

void	win_style::currentCharFormatChanged(const QTextCharFormat &format)
{
	QFont *f = &format.font();
	font->setCurrentIndex(font->findText(QFontInfo(*f).family()));
	size->setCurrentIndex(size->findText(QString::number(f->pointSize())));
	bold->setChecked(f->bold());
	italic->setChecked(f->italic());
	underline->setChecked(f->underline());
}

void	win_style::change_family(const QString &f)
 {
     QTextCharFormat new_char_format;
     new_char_format.setFontFamily(f);
     mergeFormatOnWordOrSelection(new_char_format);
 }

void	win_style::change_size(const QString &p)
{
	QTextCharFormat new_char_format;
	new_char_format.setFontPointSize(p.toFloat());
	mergeFormatOnWordOrSelection(new_char_format);
}

void	win_style::change_bold()
{
	QTextCharFormat new_char_format;
	if (bold->isChecked())
		new_char_format.setFontWeight(QFont::Bold);
	else
		new_char_format.setFontWeight(QFont::Normal);
	mergeFormatOnWordOrSelection(new_char_format);
}

void	win_style::change_italic()
{
	QTextCharFormat new_char_format;
	new_char_format.setFontItalic(italic->isChecked());
	mergeFormatOnWordOrSelection(new_char_format);
}

void	win_style::change_underline()
{
	QTextCharFormat new_char_format;
	new_char_format.setFontUnderline(underline->isChecked());
	mergeFormatOnWordOrSelection(new_char_format);
}

void	win_style::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
 {
     QTextCursor cursor = TE->textCursor();
     if (!cursor.hasSelection())
         cursor.select(QTextCursor::WordUnderCursor);
     cursor.mergeCharFormat(format);
     TE->mergeCurrentCharFormat(format);
 }

void	win_style::change_left()
{
	 TE->setAlignment(Qt::AlignLeft);
	 left->setChecked(TRUE);
	 center->setChecked(FALSE);
	 right->setChecked(FALSE);
	 justify->setChecked(FALSE);
}

void	win_style::change_center()
{
	 TE->setAlignment(Qt::AlignCenter);
	 left->setChecked(FALSE);
	 center->setChecked(TRUE);
	 right->setChecked(FALSE);
	 justify->setChecked(FALSE);
}

void	win_style::change_right()
{
	 TE->setAlignment(Qt::AlignRight);
	 left->setChecked(FALSE);
	 center->setChecked(FALSE);
	 right->setChecked(TRUE);
	 justify->setChecked(FALSE);
}

void	win_style::change_justify()
{
	 TE->setAlignment(Qt::AlignJustify);
	 left->setChecked(FALSE);
	 center->setChecked(FALSE);
	 right->setChecked(FALSE);
	 justify->setChecked(TRUE);
}

void	win_style::cursorPositionChanged()
{
	Qt::Alignment	a = TE->alignment();
	if (a & Qt::AlignLeft) 
		{
        left->setChecked(TRUE);
		center->setChecked(FALSE);
		right->setChecked(FALSE);
		justify->setChecked(FALSE);
		} 
	 else if (a & Qt::AlignHCenter)
		{
        left->setChecked(FALSE);
		center->setChecked(TRUE);
		right->setChecked(FALSE);
		justify->setChecked(FALSE);
		} 
     else if (a & Qt::AlignRight)
		{
        left->setChecked(FALSE);
		center->setChecked(FALSE);
		right->setChecked(TRUE);
		justify->setChecked(FALSE);
		} 
     else if (a & Qt::AlignJustify)			
		{
        left->setChecked(FALSE);
		center->setChecked(FALSE);
		right->setChecked(FALSE);
		justify->setChecked(TRUE);
		} 
}

