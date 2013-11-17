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

#ifndef LAYOUT_LENGTH_H
#define LAYOUT_LENGTH_H

#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QToolButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QFontDatabase>
#include <QApplication>
#include <QAction>
#include <QColorDialog>
#include <QMessageBox>

#include "utilities.h"

class layout_length : public QHBoxLayout
{	
	Q_OBJECT	//Necessario per gestire Signals e Slots	
public:
        layout_length(QString description, int length_input, int min, int max, bool	use_combobox_input);
	~layout_length()
	{}

        bool                    use_combobox;
	bool			is_valid;
        int			length;
        int			min_length;
        int			max_length;
        unsigned char		um;
	
        QLabel			*D;
        QLineEdit		*T;
        QComboBox		*C;
        QLabel			*U;

        QPalette		*normal_palette;
        QPalette		*red_palette;

	void			setDisabled(bool flag);
	void			setEnabled(bool flag);
	void			setLength(int new_length);
        void			setC(int new_length);
	void			setUM(char new_um);

	bool			warning_not_valid_parameter(QString parameter);

public slots:	
	void			T_changed(const QString &stringa);
	void			C_changed(int);
};

#endif
