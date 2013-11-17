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

/****************************************************************************
**
** Per Scrivere/leggere i dati dei file utilizzo le specifiche XML
** Inoltre accedo ai dati scritti in XML attraverso l'iterfaccia SAX (Simple Api for XML)
**
****************************************************************************/

#include <QtGui>

#include "xml_sax_handler_read_devices.h"

extern	win_main		*w_main;
extern	char			file_type;
extern	lay_data		layer_data[LAYER_DATA_COUNT];
extern	bool	xml_wrong_attribute;
extern	QString	xml_wrong_attribute_description;

xml_sax_handler_read_devices::xml_sax_handler_read_devices()
{
    
}

bool xml_sax_handler_read_devices::handle_file()
{
	if (!file.open(QFile::ReadOnly)) 
		{
        QMessageBox::warning(w_main, "Error", QObject::tr("Cannot read file %1:\n%2.").arg(file.fileName()).arg(file.errorString()));
        return FALSE;
		}

	in.setDevice(&file);
	in.setCodec("UTF-8");	//Attenzione: UTF-8 usa 8bit per carattere ma riesce a scrivere tutto l'unicode usando sequenze multibyte quando necessario
	
	//Il file è stato compresso per cui lo carico tutto in memoria per scompattarlo
	datablock_compressed.clear();
	datablock_compressed.append(in.readAll());
	//I dati compressi con ZLib iniziano sempre con 4 numeri che indicano la dimensione di memoria seguiti da 120 e -100 
	if ((datablock_compressed.at(4) != 120) || (datablock_compressed.at(5) != -100))	
                return FALSE;
        datablock = qUncompress(datablock_compressed);
        datablock_compressed.clear();

	return TRUE;
}

bool xml_sax_handler_read_devices::open_file(QString file_input, QTreeWidgetItem *item_input, QList<QTreeWidgetItem *>    *all_row_input)
{
        file.setFileName(file_input);

	if (!handle_file())
		return FALSE;

        item = item_input;
        all_row = all_row_input;

        reader.setContentHandler(this);
        reader.setErrorHandler(this);
	xmlInputSource.setData(QString(""));	//All'inizio faccio il parse su una stringa vuota per poi fare parseContinue
	reader.parse(&xmlInputSource,TRUE);
	int datablock_lunghezza = datablock.size();
	int pos_start = 0;
	int pos_return = datablock.indexOf(QString("\n"));
        xml_wrong_attribute = FALSE;
	while (pos_return < datablock_lunghezza)
		{
                line = QString::fromUtf8(datablock.mid(pos_start, pos_return - pos_start));
                xmlInputSource.setData(line);
		reader.parseContinue();
		pos_start = pos_return + 1;
		pos_return = datablock.indexOf(QString("\n"), pos_start);
                if ((pos_return == -1))
			{
			//Leggo fino al fondo ed esco da ciclo while
			line = QString(datablock.mid(pos_start, datablock_lunghezza - pos_start));	
                        if (line.length() > 0)
                            {
                            xmlInputSource.setData(line);
                            reader.parseContinue();
                            }
			pos_return = datablock_lunghezza; //Mi serve per uscire dal ciclo while
                        }
		}	
		
	file.close();
	if (errorString() == "")
		return TRUE;
	else
                return FALSE;
}

bool xml_sax_handler_read_devices::startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &attributes)
{
        if (qName == "symbol")
                {
                QString devices = attributes.value("devices");
                QString fn;
                if (devices.length() > 0)
                        {
                        QStringList sl_devices = devices.split(";", QString::SkipEmptyParts);
                        for (int i = 0; i < sl_devices.count(); i +=2)
                                {
                                QTreeWidgetItem *child = new QTreeWidgetItem(item);
                                child->setText(0, sl_devices.at(i));
                                //Nella colonna 1 metto solo il filename senza il percorso
                                fn = sl_devices.at(i+1).section('/',-1);
                                fn = fn.left(fn.length()-4);
                                child->setText(1, fn);
                                //Nella colonna 2 metto tutto, anche il percorso
                                child->setText(2, sl_devices.at(i+1));
                                all_row->append(child);
                                }
                        }
                }
        return TRUE;
}

bool xml_sax_handler_read_devices::endElement(const QString &, const QString &, const QString &qName)
{
        return TRUE;
}

bool xml_sax_handler_read_devices::characters(const QString &str)
{
        return TRUE;
}

bool xml_sax_handler_read_devices::fatalError(const QXmlParseException &exception)
{
        return FALSE;
}

QString xml_sax_handler_read_devices::errorString() const
{
        return errorStr;
}

bool	xml_sax_handler_read_devices::wrong_attribute_value(QString attribute)
{
        errorStr = "";
	return FALSE;	
}

bool	xml_sax_handler_read_devices::wrong_name(QString name)
{
        errorStr = "";
	return FALSE;
}
