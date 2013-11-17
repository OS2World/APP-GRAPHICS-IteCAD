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
**
**
**
**
** NOTE SULL'XML
**
** 1) Tutti i tag devono essere minuscoli (non si può scrivere Name, ma si deve scrivere name
** 2) I tag devono essere sempre chiusi e si può sostituire <br></br> con <br/>
** 3) E' possibile avere un tag vuoto senza sottoelementi chiudendolo con / Es: <immagine file="immagine1.jpg" />
**    E' molto più semplice che scrivere <immagine file="immagine1.jpg"></immagine>
** 4) Utilizzare l'attributo id al posto di name per identificare gli elementi di un documento
** 5) I valori degli attributi devono essere specificati tra doppi apici o singoli apici (<table width="30%">)
** 6) I commenti sono racchiusi dentro i caratteri <!-- e -->
**
** NOTE SU ZLIB
** ZLib è una libreria facilmente inseribile dentro a QT
** Questa libreria è usata da QT stesso, da GoogleEarth, da Office e da una miriade di programmi famosi
** Il formato con cui scrivo i dati tramite la libreria Zlib è un formato di cui non conosco l'estensione (.z ???)
** Il formato .gz è il formato gzip che è lo standard dei progetti GNU (gzip sta per GNU Zip) e non è quello che uso io
****************************************************************************/



#include <QtGui>

#include "xml_sax_generator.h"
#include "utilities.h"

extern	QString prog_version_string;

extern	win_main		*w_main;
extern	char file_type;
extern	lay_data	layer_data[LAYER_DATA_COUNT];

extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

myGraphicsView	*xmlg_GV;
myGraphicsScene	*xmlg_GS;
myGraphicsItem	*xmlg_obj;
myComponent		*xmlg_group;

extern	QPoint		xml_points_translation;

extern	int		drc_v2v;
extern	int		drc_v2l;
extern	int		drc_l2l;

extern  factory_value   factory_values[FACTORY_VALUE_COUNT];

extern  int	style_sch_pen_width_wire;

xml_sax_generator::xml_sax_generator()
{

}

bool xml_sax_generator::handle_file()
{
	if (!file.open(QFile::WriteOnly)) 
		{
		QMessageBox::warning(w_main, "Itecad", QObject::tr("Cannot write file %1:\n%2.").arg(file.fileName()).arg(file.errorString()));
		return FALSE;
		}	
	
	out.setDevice(&file);
	out.setCodec("UTF-8");	//Attenzione: UTF-8 usa 8bit per carattere ma riesce a scrivere tutto l'unicode usando sequenze multibyte quando necessario
	
	return TRUE;
}

bool xml_sax_generator::textedit_to_file(QString f_path, QTextEdit *xml)
{
	file.setFileName(f_path);

	if (!handle_file())
		return FALSE;

	datablock.append(xml->toPlainText().toUtf8());

        datablock_compressed = qCompress(datablock, -1);	//Salvo con livello di compressione di default di ZLib, ossia 6 che è un buon compromesso tra compressione e velocità
	out << datablock_compressed;
	datablock.clear();
	datablock_compressed.clear();

	file.close();

	return TRUE;
}

bool xml_sax_generator::save_file(QString f_path, char f_type, QString f_version)
{
	file.setFileName(f_path);

	if (!handle_file())
		return FALSE;

	version = f_version;    
	
	indent = 0;
		   
        line = "?xml version=\"1.0\" encoding=\"UTF-8\"?";	//Uso la versione 1.0 dell'XML
	write_xml_line();
	switch (f_type)
	{
	case FT_PROJECT:
		line = "!DOCTYPE itecadproject";	//Stranissimo, se tolgo lo spazio prima di !DOCTYPE il file non riesce ad essere letto da win_xml_editor ma secondo me è salvato giusto (provare con livello di compressione 0 e vedere) 
		write_xml_line();
		write_prj();
		break;
	case FT_SCH_SYMBOL:
		line = "!DOCTYPE itecadschsymbol";
		write_xml_line();
		write_ssf();
		break;
	case FT_PCB_SYMBOL:
		line = "!DOCTYPE itecadpcbsymbol";
		write_xml_line();
		write_pff();
		break;
	}

	datablock_compressed = qCompress(datablock, -1);	//Salvo con livello di compressione di default di ZLib, ossia 6 che è un buon compromesso tra compressione e velocità
	out << datablock_compressed;
	datablock.clear();
	datablock_compressed.clear();
	
	file.close();
	return TRUE;
}

void xml_sax_generator::write_prj()
{
	line = "itecadproject version=" + escapedAttribute(version);
        w_main->settings_current2temp();
        for (int i = 0; i < FACTORY_VALUE_COUNT; i++)
                if ((i == 47) || (i == 48) || (i == 51) || (i == 52))
                        line += " " + factory_values[i].description + "=" + escapedAttribute(xml_write_objlayer(factory_values[i].temp.toInt()));
                else
                        switch(factory_values[i].type)
                        {
                        case QVariant::Int:
                                line += " " + factory_values[i].description + "=" + escapedAttribute(factory_values[i].temp.toInt());
                                break;
                        case QVariant::Double:
                                line += " " + factory_values[i].description + "=" + escapedAttribute(xml_write_qreal(factory_values[i].temp.toDouble()));
                                break;
                        case QVariant::Bool:
                                line += " " + factory_values[i].description + "=" + escapedAttribute(xml_write_bool(factory_values[i].temp.toBool()));
                                break;
                        case QVariant::Color:
                                line += " " + factory_values[i].description + "=" + escapedAttribute(factory_values[i].temp.value<QColor>().name());
                                break;
                        }
        write_xml_line();
        indent++;

	for (int i=0; i < w_main->tw->count(); i++)
		write_document(i);

	indent--;
	line = "/itecadproject";
	write_xml_line();
}

void xml_sax_generator::write_ssf()
{
	xmlg_GV = GV;
	xmlg_GS = GS; 

	line = "itecadschsymbol version=" + escapedAttribute(version);
	write_xml_line();
	indent++;

	write_symbol(GV->schcomp_or_pcbcomp);

	indent--;
	line = "/itecadschsymbol";
	write_xml_line();
}

void xml_sax_generator::write_pff()
{
	xmlg_GV = GV;
	xmlg_GS = GS; 

	line = "itecadpcbsymbol version=" + escapedAttribute(version);
	write_xml_line();
	indent++;

	write_symbol(GV->schcomp_or_pcbcomp);

	indent--;
	line = "/itecadpcbsymbol";
	write_xml_line();
}

void xml_sax_generator::write_document(int doc)
{	
	switch (w_main->doc_type(w_main->tw->widget(doc)))
	{
	case DT_SCH:
		xmlg_GV = (myGraphicsView *) w_main->tw->widget(doc);
		xmlg_GS = xmlg_GV->scene; 
		line = "document name=" + escapedAttribute(xmlg_GV->name) + " type=" + escapedAttribute("sch");
		if (file_type == FT_PROJECT)
			{
                        line += " pagewidth=" + escapedAttribute(xmlg_GV->page_width);
                        line += " pageheight=" + escapedAttribute(xmlg_GV->page_height);
                        line += " organization=" + escapedAttribute(xmlg_GV->frame_organization->text);
                        line += " title=" + escapedAttribute(xmlg_GV->frame_title->text);
                        line += " docnum=" + escapedAttribute(xmlg_GV->frame_doc_num->text);
                        line += " rev=" + escapedAttribute(xmlg_GV->frame_rev->text);
                        if (xmlg_GV->ur.canUndo())   //Aggiorno la data se ci sono state delle modifiche
                                {
                                xmlg_GV->frame_date->text = QDate::currentDate().toString("d MMMM yyyy");
                                xmlg_GV->frame_date->info_update();
                                }
                        line += " date=" + escapedAttribute(xmlg_GV->frame_date->text);
			}
		write_xml_line();
		indent++;

		write_nodes();
		
		//Scrivo tutti gli oggetti
		foreach(myGraphicsItem *obj, xmlg_GS->all_obj)
			{
			if (obj->comp == 0)
				write_object(obj);
			}

		//Scrivo tutti i gruppi
		foreach(myComponent *comp, xmlg_GV->all_comp)
			write_symbol(comp);
			
		indent--;
		break;
	case DT_PCB:
		xmlg_GV = (myGraphicsView *) w_main->tw->widget(doc);
		xmlg_GS = xmlg_GV->scene; 
		line = "document name=" + escapedAttribute(xmlg_GV->name) + " type=" + escapedAttribute("pcb");	
                if (file_type == FT_PROJECT)
                        {
                        line += " origin=" + escapedAttribute(xml_write_point(xmlg_GV->origin));
                        }
                write_xml_line();
		indent++;

		write_nodes();
		
		//Scrivo tutti gli oggetti
		foreach(myGraphicsItem *obj, xmlg_GS->all_obj)
			{
			if (obj->comp == 0)
				write_object(obj);
			}

		//Scrivo tutti i gruppi
		foreach(myComponent *comp, xmlg_GV->all_comp)
			write_symbol(comp);
			
		indent--;
		break;
	case DT_TXT:
		QTextEdit	*xml_TE = (QTextEdit *) w_main->tw->widget(doc);
		line = "document name=" + escapedAttribute(xml_TE->objectName()) + " type=";
		line += escapedAttribute("txt");
		line += " html=" + escapedAttribute(xml_TE->toHtml());	//Uso line perchè così posso mettere tra virgolette il tutto
		write_xml_line();
		break;
	}	
	
	line = "/document";
	write_xml_line();	
}

void xml_sax_generator::write_symbol(myComponent *group)
{
	if (xmlg_GV->is_sch)
		{
		line = "symbol ";
		line += " idstamp=" + escapedAttribute(group->idstamp);
		line += " alias=" + escapedAttribute(group->alias);
                line += " name=" + escapedAttribute(group->ident);
		line += " value=" + escapedAttribute(group->value);
		line += " pinnumbervisible=" + escapedAttribute(xml_write_bool(group->pin_number_is_visible));
		line += " pinnamevisible=" + escapedAttribute(xml_write_bool(group->pin_name_is_visible));
                line += " ungatedpins=" + escapedAttribute(group->up);
                if (file_type == FT_PROJECT)
                        line += " footprintlink=" + escapedAttribute(group->footprint_link);
                else
                        line += " devices=" + escapedAttribute(group->devices);
		}
	else
		{
		line = "footprint ";
		line += " idstamp=" + escapedAttribute(group->idstamp);
		line += " alias=" + escapedAttribute(group->alias);
                line += " name=" + escapedAttribute(group->ident);
                line += " value=" + escapedAttribute(group->value);
                line += " footprintlink=" + escapedAttribute(group->footprint_link);
		}
	if (file_type == FT_PROJECT)
		line += " anchorpoint=" + escapedAttribute(xml_write_point(group->anchor_point));
	
	write_xml_line();
	indent++;
	
	if (file_type != FT_PROJECT)
		{
		write_nodes();
		foreach(myGraphicsItem *obj, xmlg_GS->all_obj)
			write_object(obj);
		}
	else
		foreach(myGraphicsItem *obj, group->all_obj)
			write_object(obj);
	indent--;
	if (xmlg_GV->is_sch)
		line = "/symbol";
	else
		line = "/footprint";
	write_xml_line();	
}

void xml_sax_generator::write_nodes()
{
        //Scrivo tuti i nodi
	for (int i=0; i < xmlg_GV->all_nodes.count(); i++)
                {
		line = "node index=" + escapedAttribute(i);
		line += " point=" + escapedAttribute(xml_write_point(xmlg_GV->all_nodes.at(i)->point));
		line += " /";
		write_xml_line();
                }
}

void xml_sax_generator::write_object(myGraphicsItem *obj)
{
        line = xml_write_objtype(obj->tipo, xmlg_GV->is_sch);

        switch (obj->tipo)
	{
        case O_CONNECTION:
                line += " layer=" + escapedAttribute(xml_write_objlayer(obj->layer_from));
                line += " node1=" + escapedAttribute(xmlg_GV->all_nodes.indexOf(obj->node_start));
                line += " node2=" + escapedAttribute(xmlg_GV->all_nodes.indexOf(obj->node_end));
                if (!xmlg_GV->is_sch)
                        line += " penwidth=" + escapedAttribute(xml_write_penwidth(obj->pen_width, FALSE));
		break;	
	case O_POLYLINE:
                line += " layer=" + escapedAttribute(xml_write_objlayer(obj->layer_from));
		line += " points=" + escapedAttribute(xml_write_polygon(&obj->polygon));
                line += " penwidth=" + escapedAttribute(xml_write_penwidth(obj->pen_width, xmlg_GV->is_sch));
		break;		
        case O_POLYGON:
                line += " layer=" + escapedAttribute(xml_write_objlayer(obj->layer_from));
		line += " points=" + escapedAttribute(xml_write_polygon(&obj->polygon));
                line += " penwidth=" + escapedAttribute(xml_write_penwidth(obj->pen_width, xmlg_GV->is_sch));
		line += " fillpolygon=" + escapedAttribute(xml_write_bool(obj->fill_polygon));
		break;	
	case O_TEXT:
                line += " layer=" + escapedAttribute(xml_write_objlayer(obj->layer_from));
                line += " point1=" + escapedAttribute(xml_write_point(obj->start));
		line += " text=" + escapedAttribute(obj->text);
		line += " height=" + escapedAttribute(obj->font_height);
		line += " bold=" + escapedAttribute(xml_write_bool(obj->font_bold));
		line += " italic=" + escapedAttribute(xml_write_bool(obj->font_italic));
		line += " textposition=" + escapedAttribute(xml_write_textposition(obj->text_position));
                if (!xmlg_GV->is_sch)
                        line += " textmirror=" + escapedAttribute(xml_write_bool(obj->mirror_text_on));
                break;
	case O_RECT:
	case O_ELLIPSE:
                line += " layer=" + escapedAttribute(xml_write_objlayer(obj->layer_from));
                line += " point1=" + escapedAttribute(xml_write_point(obj->start));
                line += " width=" + escapedAttribute(obj->width);
                line += " height=" + escapedAttribute(obj->height);
		line += " fillpolygon=" + escapedAttribute(xml_write_bool(obj->fill_polygon));
                line += " penwidth=" + escapedAttribute(xml_write_penwidth(obj->pen_width, xmlg_GV->is_sch));
		break;
	case O_IDENT:
	case O_VALUE:
                line += " layer=" + escapedAttribute(xml_write_objlayer(obj->layer_from));
                line += " point1=" + escapedAttribute(xml_write_point(obj->start));
                if (!xmlg_GV->is_sch && (obj->tipo == O_IDENT))  //E' editabile solo l'ident del PCB
                        {
                        line += " height=" + escapedAttribute(obj->font_height);
                        line += " bold=" + escapedAttribute(xml_write_bool(obj->font_bold));
                        line += " italic=" + escapedAttribute(xml_write_bool(obj->font_italic));
                        }
                line += " textposition=" + escapedAttribute(xml_write_textposition(obj->text_position));
		break;
	case O_PIN:
                line += " layer=" + escapedAttribute(xml_write_objlayer(obj->layer_from));
                line += " node1=" + escapedAttribute(xmlg_GV->all_nodes.indexOf(obj->node_start));
		line += " number=" + escapedAttribute(obj->number);
		line += " name=" + escapedAttribute(obj->text);
		line += " textposition=" + escapedAttribute(xml_write_textposition(obj->text_position));
                line += " shape=" + escapedAttribute(xml_write_pinshape(obj->shape));
		break;
	case O_PORT:
                line += " layer=" + escapedAttribute(xml_write_objlayer(obj->layer_from));
                line += " node1=" + escapedAttribute(xmlg_GV->all_nodes.indexOf(obj->node_start));
                line += " name=" + escapedAttribute(obj->text);
                line += " textposition=" + escapedAttribute(xml_write_textposition(obj->text_position));
                line += " shape=" + escapedAttribute(xml_write_portshape(obj->shape));
                break;
        case O_POWER:
                line += " layer=" + escapedAttribute(xml_write_objlayer(obj->layer_from));
                line += " node1=" + escapedAttribute(xmlg_GV->all_nodes.indexOf(obj->node_start));
		line += " name=" + escapedAttribute(obj->text);
		line += " textposition=" + escapedAttribute(xml_write_textposition(obj->text_position));
                line += " shape=" + escapedAttribute(xml_write_powershape(obj->shape));
                line += " showname=" + escapedAttribute(xml_write_bool(obj->fill_polygon));
		break;
	case O_PAD:
                line += " layerfrom=" + escapedAttribute(xml_write_objlayer(obj->layer_from));
                line += " layerto=" + escapedAttribute(xml_write_objlayer(obj->layer_to));
                line += " node1=" + escapedAttribute(xmlg_GV->all_nodes.indexOf(obj->node_start));
                line += " number=" + escapedAttribute(obj->number);
                if (file_type == FT_PROJECT)
                        line += " net=" + escapedAttribute(obj->node_start->net_id);
                line += " shape=" + escapedAttribute(xml_write_padshape(obj->shape));
                line += " width=" + escapedAttribute(obj->width);
                line += " height=" + escapedAttribute(obj->height);
                line += " drill=" + escapedAttribute(obj->drill);
                break;
	case O_VIA:
                line += " layerfrom=" + escapedAttribute(xml_write_objlayer(obj->layer_from));
                line += " layerto=" + escapedAttribute(xml_write_objlayer(obj->layer_to));
                line += " node1=" + escapedAttribute(xmlg_GV->all_nodes.indexOf(obj->node_start));
                if (file_type == FT_PROJECT)
                        line += " net=" + escapedAttribute(obj->node_start->net_id);
                line += " width=" + escapedAttribute(obj->width);
                line += " drill=" + escapedAttribute(obj->drill);
		break;
	}
	line += " /";
	write_xml_line();	
}

QString xml_sax_generator::escapedAttribute(const QString &str)
{
	QString result = str;
	result.replace("&", "&amp;");	
    result.replace("<", "&lt;");	
    result.replace(">", "&gt;");	
    result.replace("\"", "&quot;");	
    result.prepend("\"");			
    result.append("\"");
    return result;
}

QString xml_sax_generator::escapedAttribute(const int &i)
{
    return "\"" + QString("%1").arg(i) + "\"";
}

void xml_sax_generator::write_xml_line()
{
	line = QString(indent * 2, ' ') + "<" + line + ">\n";
	//Devo salvare i caratteri unicode oltre 0xFF (come l'Ohm)

	QByteArray ba = line.toUtf8();

	datablock.append(ba);	
}

