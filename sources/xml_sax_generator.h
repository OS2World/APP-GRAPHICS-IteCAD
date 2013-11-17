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

#ifndef XML_SAX_GENERATOR_H
#define XML_SAX_GENERATOR_H

#include <QTextStream>
#include <QByteArray>
#include <QDataStream>
#include <QImage>
#include <QTextEdit>
#include "win_main.h"
#include "myGraphicsItem.h"
#include "myGraphicsView.h"
#include "myComponent.h"

class xml_sax_generator
{
public:
    xml_sax_generator();

	bool		save_file(QString f_path, char f_type, QString f_version);
	bool		textedit_to_file(QString f_path, QTextEdit *xml);
	void		write_prj();
	void		write_ssf();
	void		write_pff();
	void		write_nodes();
	void		write_document(int doc);
	void		write_symbol(myComponent *group);
	void		write_object(myGraphicsItem *obj);

	QString		version;
	
	char		indent;
	QString		line;
	QTextStream out;	

	QFile		file;
	bool		handle_file();

private:
	QString		escapedAttribute(const QString &str);
	QString		escapedAttribute(const int &i);
	void		write_xml_line();

	QByteArray datablock;
	QByteArray datablock_compressed;
};

#endif
