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

#include "xml_sax_handler.h"

extern	win_main		*w_main;
extern	char			file_type;
extern	lay_data		layer_data[LAYER_DATA_COUNT];
extern	bool	xml_wrong_attribute;
extern	QString	xml_wrong_attribute_description;

extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;
extern	QTextEdit		*TE;

myGraphicsView	*xmlh_GV;
myGraphicsScene	*xmlh_GS;
myGraphicsItem	*xmlh_obj;
myComponent	*xmlh_group;

extern	int		drc_v2v;
extern	int		drc_v2l;
extern	int		drc_l2l;
extern	int		check_v2v;
extern	int		check_v2l;
extern	int		check_l2l;

extern  int	style_sch_pen_width_wire;

extern  factory_value   factory_values[FACTORY_VALUE_COUNT];

xml_sax_handler::xml_sax_handler()
{
    
}

bool xml_sax_handler::handle_file()
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
		{
		QMessageBox::warning(w_main, "Error", QObject::tr("The file %1 is not an Itecad file.").arg(file.fileName()));
        return FALSE;
		}
	datablock = qUncompress(datablock_compressed);
        datablock_compressed.clear();

	return TRUE;
}

bool xml_sax_handler::file_to_textedit(QString f_path, QTextEdit *xml)
{
	file.setFileName(f_path);

	if (!handle_file())
		return FALSE;

	xml->clear();
        xml->setPlainText(QString::fromUtf8(datablock, datablock.length()));
		
	file.close();
	return TRUE;	
}

bool xml_sax_handler::open_file(QString f_path, bool is_pixmapoutput_input)
{
	is_pixmapoutput = is_pixmapoutput_input;

	file.setFileName(f_path);

	if (!handle_file())
		return FALSE;

	ft_found = FALSE;
	reader.setContentHandler(this);
        reader.setErrorHandler(this);
	xmlInputSource.setData(QString(""));	//All'inizio faccio il parse su una stringa vuota per poi fare parseContinue
	reader.parse(&xmlInputSource,TRUE);
	int datablock_lunghezza = datablock.size();
	int pos_start = 0;
	int pos_return = datablock.indexOf(QString("\n"));
	line_num = 0;
	xml_wrong_attribute = FALSE;
	while (pos_return < datablock_lunghezza)
		{
                line_num++;
                qDebug() << "LINEA: " << line_num;
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
		{
		if (!GV->is_pixmapoutput)
			w_main->file_c();
		return FALSE;
		}	
}

bool xml_sax_handler::startElement(const QString &, const QString &, const QString &qName, const QXmlAttributes &attributes)
{
	//Identifico prima il tipo di file
	if (!ft_found)
		{
		xmlh_GV = 0;		//Per sicurezza e per rilevare meglio gli errori
		xmlh_GS = 0;		//Per sicurezza e per rilevare meglio gli errori
		xmlh_obj = 0;	//Per sicurezza e per rilevare meglio gli errori
		xmlh_group = 0;	//Per sicurezza e per rilevare meglio gli errori
		if ((qName == "itecadproject") || (qName == "itecadschsymbol") || (qName == "itecadpcbsymbol"))
			{
			QString version = attributes.value("version");
                        /*if (!version.isEmpty() && version != "1.0")
				{
				errorStr = QObject::tr("Version %1 is not supported.").arg(version);
				return false;
                                }*/
			ft_found = TRUE;
			if (!is_pixmapoutput)
				{
				if (qName == "itecadproject")
                                        {
					file_type = FT_PROJECT;
                                        for (int i = 0; i < FACTORY_VALUE_COUNT; i++)
                                                if (attributes.value(factory_values[i].description).isEmpty())
                                                        factory_values[i].temp = factory_values[i].value;
                                                else
                                                        {
                                                        if ((i == 47) || (i == 48) || (i == 51) || (i == 52))
                                                                factory_values[i].temp = xml_read_objlayer(attributes.value(factory_values[i].description));
                                                        else
                                                                switch(factory_values[i].type)
                                                                {
                                                                case QVariant::Int:
                                                                        factory_values[i].temp = attributes.value(factory_values[i].description);
                                                                        break;
                                                                case QVariant::Bool:
                                                                        factory_values[i].temp = xml_read_bool(attributes.value(factory_values[i].description));
                                                                        break;
                                                                case QVariant::Color:
                                                                        factory_values[i].temp = QColor(attributes.value(factory_values[i].description));
                                                                        break;
                                                                case QVariant::Double:
                                                                        factory_values[i].temp = xml_read_qreal(attributes.value(factory_values[i].description));
                                                                        break;
                                                                }
                                                        }
                                        w_main->settings_temp2current();
                                        }
				else if (qName == "itecadschsymbol")
                                        {
                                        file_type = FT_SCH_SYMBOL;
                                        w_main->settings_generaldefault2temp();
                                        w_main->settings_temp2current();
                                        }
				else if (qName == "itecadpcbsymbol")
                                        {
					file_type = FT_PCB_SYMBOL;
                                        w_main->settings_generaldefault2temp();
                                        w_main->settings_temp2current();
                                        }
				}
                        return TRUE;
			}
		else
			return wrong_name(qName);
		}
	if (qName == "node")
		{
		node_data *nd = xmlh_GV->node_new();
		nd->point = xml_read_point(attributes.value("point"));
		xmlh_GV->all_nodes.append(nd);
                }
	else if (qName == "document")
		{
		QString type = attributes.value("type");
		QString name = attributes.value("name");
		if (type == "sch")
			{
			w_main->documents_add_new(DT_SCH, name);
			xmlh_GV = GV; 
			xmlh_GS = GS; 
			if (file_type == FT_PROJECT)
				{
				xmlh_GV->page_width = attributes.value("pagewidth").toInt();
				xmlh_GV->page_height = attributes.value("pageheight").toInt();

                                xmlh_GV->frame_organization->text = attributes.value("organization");
                                xmlh_GV->frame_organization->info_update();
                                xmlh_GV->frame_title->text = attributes.value("title");
                                xmlh_GV->frame_title->info_update();
                                xmlh_GV->frame_doc_num->text = attributes.value("docnum");
                                xmlh_GV->frame_doc_num->info_update();
                                xmlh_GV->frame_rev->text = attributes.value("rev");
                                xmlh_GV->frame_rev->info_update();
                                xmlh_GV->frame_date->text = attributes.value("date");
                                xmlh_GV->frame_date->info_update();
                                }
			}
		else if (type == "pcb")
			{
			w_main->documents_add_new(DT_PCB, name);
			xmlh_GV = GV; 
                        xmlh_GS = GS;
                        if (file_type == FT_PROJECT)
                                {
                                xmlh_GV->origin = xml_read_point(attributes.value("origin"));
                                }
                        }
		else if (type == "txt")
			{
			w_main->documents_add_new(DT_TXT, name);
			QTextEdit	*xml_TE = TE; 
			xml_TE->setHtml(attributes.value("html"));
			}
		else
			return wrong_attribute_value("type=" + type);
                }
	else if ((qName == "symbol") || (qName == "footprint"))
		{
                if (is_pixmapoutput)
			{
			xmlh_GV = GV;
			xmlh_GS = GS;
			xmlh_group = new myComponent();	
                        xmlh_group->ident = attributes.value("alias") + "***";
			xmlh_group->value = QFileInfo(file).completeBaseName();
			xmlh_GV->all_comp.append(xmlh_group);
			}
		else if (file_type == FT_PROJECT)
			{
			xmlh_group = new myComponent();			
			xmlh_GV->all_comp.append(xmlh_group);
                        xmlh_group->ident = attributes.value("name");
                        xmlh_group->value = attributes.value("value");
                        xmlh_group->anchor_point = xml_read_point(attributes.value("anchorpoint"));
			}
		else
			{
			QString name = attributes.value("name");
			if (file_type == FT_SCH_SYMBOL)
				w_main->documents_add_new(DT_SCH, name);
			else if (file_type == FT_PCB_SYMBOL)
				w_main->documents_add_new(DT_PCB, name);
			xmlh_GV = GV;			
			xmlh_GS = GS;
			xmlh_group = xmlh_GV->schcomp_or_pcbcomp;
                        xmlh_group->ident = attributes.value("alias") + "***";
			if (xmlh_GV->is_sch)
				xmlh_group->value = "Value";
			}
		xmlh_group->idstamp = attributes.value("idstamp").toInt();
		xmlh_group->alias = attributes.value("alias");		
		if (xmlh_GV->is_sch)
			{			
			xmlh_group->pin_number_is_visible = xml_read_bool(attributes.value("pinnumbervisible"));
			xmlh_group->pin_name_is_visible = xml_read_bool(attributes.value("pinnamevisible"));
                        if (file_type == FT_PROJECT)
                                xmlh_group->footprint_link = attributes.value("footprintlink");
                        else
                                xmlh_group->devices = attributes.value("devices");
                        xmlh_group->up = attributes.value("ungatedpins");
			}
                else
                        xmlh_group->footprint_link = attributes.value("footprintlink");
                }
        else if ((qName == "wire") || (qName == "polyline") || (qName == "polygon") || (qName == "text") || (qName == "rectangle") || (qName == "ellipse") || (qName == "pin") || (qName == "port") || (qName == "pad")  || (qName == "via")  || (qName == "ident")  || (qName == "value") || (qName == "power"))
		{
                xmlh_obj = new myGraphicsItem(xml_read_obj_type((QString) qName));
                switch (xmlh_obj->tipo)
		{
		case O_CONNECTION:
                        xmlh_obj->layer_from = xml_read_objlayer(attributes.value("layer"));
                        xmlh_obj->layer_to = xmlh_obj->layer_from;
                        xmlh_obj->node_start = xmlh_GV->all_nodes.at(attributes.value("node1").toInt());
                        xmlh_obj->node_end = xmlh_GV->all_nodes.at(attributes.value("node2").toInt());
                        if (xmlh_GV->is_sch)
                                xmlh_obj->pen_width = style_sch_pen_width_wire;
                        else
                                xmlh_obj->pen_width = xml_read_penwidth(attributes.value("penwidth"), xmlh_GV->is_sch);
			xmlh_obj->node_start->connected_wires.append((int) xmlh_obj);
			xmlh_obj->node_end->connected_wires.append((int) xmlh_obj);
			break;
		case O_POLYLINE:
                        xmlh_obj->layer_from = xml_read_objlayer(attributes.value("layer"));
                        xmlh_obj->layer_to = xmlh_obj->layer_from;
                        xml_read_polygon(attributes.value("points"), &xmlh_obj->polygon);
                        xmlh_obj->pen_width = xml_read_penwidth(attributes.value("penwidth"), xmlh_GV->is_sch);
			break;
		case O_POLYGON:
                        xmlh_obj->layer_from = xml_read_objlayer(attributes.value("layer"));
                        xmlh_obj->layer_to = xmlh_obj->layer_from;
                        xml_read_polygon(attributes.value("points"), &xmlh_obj->polygon);
                        xmlh_obj->pen_width = xml_read_penwidth(attributes.value("penwidth"), xmlh_GV->is_sch);
			xmlh_obj->fill_polygon = xml_read_bool(attributes.value("fillpolygon"));
			break;
		case O_TEXT:
                        xmlh_obj->layer_from = xml_read_objlayer(attributes.value("layer"));
                        xmlh_obj->layer_to = xmlh_obj->layer_from;
                        xmlh_obj->start = xml_read_point(attributes.value("point1"));
			xmlh_obj->text = attributes.value("text");
			xmlh_obj->font_height = attributes.value("height").toInt();
			xmlh_obj->font_bold = xml_read_bool(attributes.value("bold"));
			xmlh_obj->font_italic = xml_read_bool(attributes.value("italic"));
			xmlh_obj->text_position = xml_read_textposition(attributes.value("textposition"));
                        if (!xmlh_GV->is_sch)
                                xmlh_obj->mirror_text_on = xml_read_bool(attributes.value("textmirror"));
                        break;
		case O_RECT:
		case O_ELLIPSE:
                        xmlh_obj->layer_from = xml_read_objlayer(attributes.value("layer"));
                        xmlh_obj->layer_to = xmlh_obj->layer_from;
                        xmlh_obj->start = xml_read_point(attributes.value("point1"));
                        xmlh_obj->pen_width = xml_read_penwidth(attributes.value("penwidth"), xmlh_GV->is_sch);
                        xmlh_obj->width = attributes.value("width").toInt();
                        xmlh_obj->height = attributes.value("height").toInt();
			xmlh_obj->fill_polygon = xml_read_bool(attributes.value("fillpolygon"));
			break;
		case O_IDENT:
		case O_VALUE:
                        xmlh_obj->layer_from = xml_read_objlayer(attributes.value("layer"));
                        xmlh_obj->layer_to = xmlh_obj->layer_from;
                        xmlh_obj->start = xml_read_point(attributes.value("point1"));
                        if (!xmlh_GV->is_sch && (xmlh_obj->tipo == O_IDENT))  //E' editabile solo l'ident del PCB
                                {
                                xmlh_obj->font_height = attributes.value("height").toInt();
                                xmlh_obj->font_bold = xml_read_bool(attributes.value("bold"));
                                xmlh_obj->font_italic = xml_read_bool(attributes.value("italic"));
                                }
                        else
                                {
                                xmlh_obj->font_height = internal_font_height;
                                xmlh_obj->font_bold = internal_font_bold;
                                xmlh_obj->font_italic = internal_font_italic;
                                }
			xmlh_obj->text_position = xml_read_textposition(attributes.value("textposition"));
			break;	
		case O_PIN:
                        xmlh_obj->layer_from = xml_read_objlayer(attributes.value("layer"));
                        xmlh_obj->layer_to = xmlh_obj->layer_from;
                        xmlh_obj->node_start = xmlh_GV->all_nodes.at(attributes.value("node1").toInt());
			xmlh_obj->node_start->connected_pin_or_pad_or_via = (int) xmlh_obj;
			xmlh_obj->text_position = xml_read_textposition(attributes.value("textposition"));
			xmlh_obj->number = attributes.value("number").toInt();
			xmlh_obj->text = attributes.value("name");
                        xmlh_obj->shape = xml_read_pinshape(attributes.value("shape"));
                        xmlh_obj->font_height = internal_font_height;
                        xmlh_obj->font_bold = internal_font_bold;
                        xmlh_obj->font_italic = internal_font_italic;
			break;	
		case O_PORT:
                        xmlh_obj->layer_from = xml_read_objlayer(attributes.value("layer"));
                        xmlh_obj->layer_to = xmlh_obj->layer_from;
                        xmlh_obj->node_start = xmlh_GV->all_nodes.at(attributes.value("node1").toInt());
			xmlh_obj->node_start->connected_pin_or_pad_or_via = (int) xmlh_obj;
			xmlh_obj->text_position = xml_read_textposition(attributes.value("textposition"));
			xmlh_obj->text = attributes.value("name");
                        xmlh_obj->shape = xml_read_portshape(attributes.value("shape"));
                        xmlh_obj->font_height = internal_font_height;
                        xmlh_obj->font_bold = internal_font_bold;
                        xmlh_obj->font_italic = internal_font_italic;
			break;	
                case O_POWER:
                        xmlh_obj->layer_from = xml_read_objlayer(attributes.value("layer"));
                        xmlh_obj->layer_to = xmlh_obj->layer_from;
                        xmlh_obj->node_start = xmlh_GV->all_nodes.at(attributes.value("node1").toInt());
                        xmlh_obj->node_start->connected_pin_or_pad_or_via = (int) xmlh_obj;
                        xmlh_obj->text_position = xml_read_textposition(attributes.value("textposition"));
                        xmlh_obj->text = attributes.value("name");
                        xmlh_obj->shape = xml_read_powershape(attributes.value("shape"));
                        xmlh_obj->font_height = internal_font_height;
                        xmlh_obj->font_bold = internal_font_bold;
                        xmlh_obj->font_italic = internal_font_italic;
                        xmlh_obj->fill_polygon = xml_read_bool(attributes.value("showname"));
                        break;
		case O_PAD:
                        xmlh_obj->layer_from = xml_read_objlayer(attributes.value("layerfrom"));
                        xmlh_obj->layer_to = xml_read_objlayer(attributes.value("layerto"));
                        xmlh_obj->node_start = xmlh_GV->all_nodes.at(attributes.value("node1").toInt());
                        xmlh_obj->node_start->connected_pin_or_pad_or_via = (int) xmlh_obj;
                        if (file_type == FT_PROJECT)
                                xmlh_obj->node_start->net_id = attributes.value("net").toInt();
                        xmlh_obj->number = attributes.value("number").toInt();
                        xmlh_obj->shape = xml_read_padshape(attributes.value("shape"));
                        xmlh_obj->width = attributes.value("width").toInt();
                        xmlh_obj->height = attributes.value("height").toInt();
                        xmlh_obj->drill = attributes.value("drill").toInt();
                        break;
		case O_VIA:
                        xmlh_obj->layer_from = xml_read_objlayer(attributes.value("layerfrom"));
                        xmlh_obj->layer_to = xml_read_objlayer(attributes.value("layerto"));
                        xmlh_obj->node_start = xmlh_GV->all_nodes.at(attributes.value("node1").toInt());
			xmlh_obj->node_start->connected_pin_or_pad_or_via = (int) xmlh_obj;
                        xmlh_obj->shape = PS_ROUND;
			if (file_type == FT_PROJECT)
                                xmlh_obj->node_start->net_id = attributes.value("net").toInt();
                        xmlh_obj->width = attributes.value("width").toInt();
                        xmlh_obj->height = attributes.value("width").toInt();
                        xmlh_obj->drill = attributes.value("drill").toInt();
			break;	
		}
		xmlh_GS->add_item(xmlh_obj);
		if ((file_type == FT_PROJECT) && (xmlh_group != 0))
			{
			xmlh_obj->comp = (int) xmlh_group;
                        xmlh_group->append(xmlh_obj);
			}
		else
			xmlh_obj->comp = 0;
		xmlh_obj->info_update();
                }
	else
		return wrong_name(qName);

        if (xml_wrong_attribute)
                return wrong_attribute_value(xml_wrong_attribute_description);
	
	return TRUE;
}

bool xml_sax_handler::endElement(const QString &, const QString &, const QString &qName)
{
    if (qName == "document")
		{
		if (w_main->doc_type(w_main->tw->currentWidget()) != DT_TXT)
			GV->zoom_to_all();
		}
	else if ((qName == "symbol") || (qName == "footprint"))
		{
		xmlh_group = 0;
		if (file_type != FT_PROJECT)
			GV->zoom_to_all();
		else
			{
			if (is_pixmapoutput)
				GV->zoom_to_all();
			}
		}		
	else if (qName == "itecadproject") 
		{
		if (!is_pixmapoutput)
			{
			w_main->change_file_type(FT_PROJECT);
                        w_main->change_document(0);
			}
                }
	else if (qName == "itecadschsymbol") 
		{
		if (!is_pixmapoutput)
			{
			w_main->change_file_type(FT_SCH_SYMBOL);
                        w_main->change_document(0);
			}
		}
	else if (qName == "itecadpcbsymbol")
		{
		if (!is_pixmapoutput)
			{
			w_main->change_file_type(FT_PCB_SYMBOL);
                        w_main->change_document(0);
			}
		}
	return TRUE;
}

bool xml_sax_handler::characters(const QString &str)
{
    return TRUE;
}

bool xml_sax_handler::fatalError(const QXmlParseException &exception)
{
	QMessageBox::warning(w_main, QObject::tr("Error"), QObject::tr("File %1\nParse error at line %2 in XML data:\n%3")
							 .arg(file.fileName())
                             .arg(line_num)//exception.lineNumber())
                             .arg(exception.message()));
    return FALSE;
}

QString xml_sax_handler::errorString() const
{
    return errorStr;
}

bool	xml_sax_handler::wrong_attribute_value(QString attribute)
{
	errorStr = QObject::tr("Wrong attribute value (%1).").arg(attribute);
	return FALSE;	
}

bool	xml_sax_handler::wrong_name(QString name)
{
	errorStr = QObject::tr("Unexpected %1").arg(name);
	return FALSE;
}
