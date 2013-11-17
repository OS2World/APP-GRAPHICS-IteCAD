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

#ifndef XML_SAX_HANDLER_H
#define XML_SAX_HANDLER_H

#include <QXmlDefaultHandler>
#include <QFile>
#include <QTextEdit>
#include "win_main.h"

class xml_sax_handler : public QXmlDefaultHandler
{
public:
    xml_sax_handler();

	bool	open_file(QString f_path, bool is_pixmapoutput_input);
	bool	file_to_textedit(QString f_path, QTextEdit *xml);

	bool	is_pixmapoutput;

	QFile	file;
	bool	handle_file();

        bool	startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &attributes);
	bool	endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool	characters(const QString &str);
    bool	fatalError(const QXmlParseException &exception);
    QString errorString() const;
	int		line_num;
	bool	ft_found;

	bool	wrong_attribute_value(QString attribute); 
	bool	wrong_name(QString name); 

private:
	QTextStream			in;
	QXmlSimpleReader	reader;
	QXmlInputSource		xmlInputSource;
	
	QString	line;
	QString currentText;
    QString errorStr;
    bool	metProjectTag;

	QByteArray datablock;
	QByteArray datablock_compressed;
};

#endif
