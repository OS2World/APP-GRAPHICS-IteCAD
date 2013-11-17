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

#include "utilities.h"
#include "myGraphicsView.h"

extern	unsigned char UM;
extern	UM_description	UM_descriptions[UM_DESCRIPTION_COUNT];

extern	lay_data		layer_data[LAYER_DATA_COUNT];
extern	bool	xml_wrong_attribute;
extern	QString	xml_wrong_attribute_description;

extern	QString directory_prj;
extern	QString directory_ssf;
extern	QString directory_pff;
extern	QString directory_ivf;

extern  factory_value   factory_values[FACTORY_VALUE_COUNT];

QString add_file_suffix(QString name, QString suffix)
{
	if (name.right(suffix.length() + 1) != ("." + suffix))
		{
		if (name.right(1) == ".")
			return name + suffix;
		else
			return name + "." + suffix;
		}
	return name;
}

QString from_001thou_to_string(int value, unsigned char um)
{
	float result = (float) value / UM_descriptions[um].k;

        switch (UM_descriptions[um].decimal_digits)
	{
	case 0:
		result = arrotonda_a_0_decimali(result);
		break;
	case 1:
		result = arrotonda_a_1_decimali(result);
		break;
	case 2:
		result = arrotonda_a_2_decimali(result);
		break;
	case 3:
		result = arrotonda_a_3_decimali(result);
		break;
	case 4:
		result = arrotonda_a_4_decimali(result);
		break;
	case 5:
		result = arrotonda_a_5_decimali(result);
		break;
	case 6:
		result = arrotonda_a_6_decimali(result);
		break;
	}

        return QString("%1").arg(result);//(float) value / UM_descriptions[um].k);
}

int from_string_to_001thou(QString testo, unsigned char um)
{
	float risultato = testo.toFloat() * UM_descriptions[um].k;
	return (int) risultato; //Attenzione: devo usare la variabile temporanea risultato, altrimenti per 1.002 non approssima bene... stranissimo ma vero
}

QRect	myQRect(QPoint p1, QPoint p2)
{
	qreal x, y, width, height; 
	if (p1.x() < p2.x())
		{
		x = p1.x();
		width = p2.x() - p1.x();
		}
	else
		{
		x = p2.x();
		width = p1.x() - p2.x();
		}
	if (p1.y() < p2.y())
		{
		y = p1.y();
		height = p2.y() - p1.y();
		}
	else
		{
		y = p2.y();
		height = p1.y() - p2.y();
		}
	return QRect(x, y, width, height);
}

char UM_find_description(QString stringa)
{
	for (int i = 0; i < UM_DESCRIPTION_COUNT; i++)
		if (stringa == UM_descriptions[i].descrizione)
			return i;
	return 0;
}

QString xml_write_bool(bool flag)
{
	if (flag)
		return "Y";
	else
		return "N";
}

QString xml_write_qreal(qreal value)
{
        QString str = QString::number(value, 'f');
        str.replace(",",".");
        //Tolgo gli zero inutili dopo la virgola
        if (str.contains("."))
                {
                while (str.right(1) == "0")
                        str = str.left(str.length() - 1);
                if (str.right(1) == ".")
                        str = str.left(str.length() - 1);
                }
        return str;
}

/*QString	xml_write_pointsize(int ps)
{
	//600 pointsize equivalgono a 600 micrometri
	ps = ps / 100;
	return QString("%1").arg(ps);
}*/

QString	xml_write_objlayer(int layer)
{
    return	layer_data[layer].description;
}

QString xml_write_point(QPoint &point)
{
        return QString::number(point.x()) + "," + QString::number(point.y());
}

QString xml_write_objtype(char type, bool is_sch)
{
	switch (type)
	{
	case O_POLYLINE:
		return "polyline";
	case O_POLYGON:
		return "polygon";
	case O_CONNECTION:
                return "wire";
        case O_TEXT:
		return "text";
	case O_IDENT:
		return "ident";
	case O_VALUE:
		return "value";
	case O_RECT:
		return "rectangle";
	case O_ELLIPSE:
		return "ellipse";
	case O_PIN:
		return "pin";
	case O_PORT:
		return "port";
	case O_PAD:
		return "pad";
	case O_VIA:
		return "via";
        case O_POWER:
                return "power";
	}
}

QString xml_write_textposition(char tp)
{
	switch (tp)
	{
	case 'T':
		return "T";
	case 'L':
		return "L";
	case 'R':
		return "R";
	case 'B':
		return "B";
	}
}

QString xml_write_pinshape(char ps)
{
	switch (ps)
	{
	case PIN_SHAPE_NONE:
		return "N";
	case PIN_SHAPE_LINE:
		return "L";
	case PIN_SHAPE_CLOCK:
		return "C";
	case PIN_SHAPE_DOT:
		return "D";
	case PIN_SHAPE_DOT_CLOCK:
		return "DC";
	}
}

QString xml_write_portshape(char ps)
{
        switch (ps)
        {
        case PORT_GENERIC:
                return "G";
        case PORT_INPUT:
                return "I";
        case PORT_OUTPUT:
                return "O";
        case PORT_BIDIRECTIONAL:
                return "B";
        }
}

QString xml_write_powershape(char ps)
{
        switch (ps)
        {
        case SYMBOL_GROUND_POWER:
                return "P";
        case SYMBOL_GROUND_SIGNAL:
                return "S";
        case SYMBOL_GROUND_EARTH:
                return "E";
        case SYMBOL_POWER_CIRCLE:
                return "C";
        case SYMBOL_POWER_BAR:
                return "B";
        }
}

QString xml_write_padshape(char ps)
{
	switch (ps)
	{
        case PS_ROUND:
                return "E";
        case PS_RECTANGLE:
                return "R";
	case PS_ROUNDED_RECTANGLE:
                return "RR";
	}
}

QString xml_write_polygon(QPolygon *p)
{
        QString w = QString::number(p->at(0).x()) + "," + QString::number(p->at(0).y());
	for (int i = 1; i < p->count(); i++)
                w += "," + QString::number(p->at(i).x()) + "," + QString::number(p->at(i).y());
	return w;
}

QString xml_write_up(QList<ungated_pins_struct> &up)
{
	QString s = "";
	for (int i = 0; i < up.count(); i++)
		{
		if (!s.isEmpty())
			s += ",";
		s += QString::number(up.at(i).pin) + "," + up.at(i).netname;
		}
	return s;
}

QString xml_write_char(char gs)
{
	return QString(QChar(gs));
}

QString xml_write_penwidth(int w, bool is_sch)
{
        if (is_sch)
                {
                if (w == SCH_PEN_NORMAL)
                        return QObject::tr("Normal");
                else if (w == SCH_PEN_SEMIBOLD)
                        return QObject::tr("SemiBold");
                else
                        return QObject::tr("Bold");
                }
        else
                {
                return QString::number(w);
                }
}

bool xml_read_bool(QString flag)
{
	if (flag == "Y")
		return TRUE;
	else if (flag == "N")
		return FALSE;
	else
		{
		//Attributo sbagliato
		xml_wrong_attribute = TRUE;
		xml_wrong_attribute_description = flag;
		return TRUE;
		}
}

qreal   xml_read_qreal(QString str)
{
        str.replace(",",".");
        return str.toDouble();
}

/*int	xml_read_pointsize(QString ps)
{
	return ps.toInt() * 100;
}*/

int	xml_read_objlayer(QString str)
{
        for (int i = 0; i < LAYER_DATA_COUNT; i++)
		if (str == layer_data[i].description)
                        return i;

        //Attributo sbagliato
	xml_wrong_attribute = TRUE;
	xml_wrong_attribute_description = "layer=" + str;
        return 0;
}

QPoint	xml_read_point(QString str)
{
        QStringList sl = str.split(",");
	return	QPoint(sl.at(0).toInt(), sl.at(1).toInt());
}

char	xml_read_obj_type(QString str)
{
        if (str == "polyline")
		return O_POLYLINE;
        else if (str == "polygon")
		return O_POLYGON;
        else if (str == "wire")
		return O_CONNECTION;
        else if (str == "text")
		return O_TEXT;
        else if (str == "rectangle")
		return O_RECT;
        else if (str == "ellipse")
		return O_ELLIPSE;
        else if (str == "ident")
		return O_IDENT;
        else if (str == "value")
		return O_VALUE;
        else if (str == "pin")
		return O_PIN;
        else if (str == "port")
		return O_PORT;
        else if (str == "pad")
		return O_PAD;
        else if (str == "via")
		return O_VIA;
        else if (str == "power")
                return O_POWER;
	else
		{
		//Attributo sbagliato
		xml_wrong_attribute = TRUE;
                xml_wrong_attribute_description = "type=" + str;
		return O_CONNECTION;								
		}
}

char	xml_read_textposition(QString str)
{
        if (str == "T")
		return 'T';
        else if (str == "L")
		return 'L';
        else if (str == "R")
		return 'R';
        else if (str == "B")
		return 'B';
	else
		{
		//Attributo sbagliato
		xml_wrong_attribute = TRUE;
                xml_wrong_attribute_description = "position=" + str;
		return O_CONNECTION;								
		}
}

char	xml_read_pinshape(QString str)
{
	if (str == "N")
		return PIN_SHAPE_NONE;
	else if (str == "L")
		return PIN_SHAPE_LINE;
	else if (str == "D")
		return PIN_SHAPE_DOT;
	else if (str == "C")
		return PIN_SHAPE_CLOCK;
	else if (str == "DC")
		return PIN_SHAPE_DOT_CLOCK;
	else
		{
		//Attributo sbagliato
		xml_wrong_attribute = TRUE;
		xml_wrong_attribute_description = "shape=" + str;	
		return O_CONNECTION;								
		}
}

char	xml_read_portshape(QString str)
{
        if (str == "G")
                return PORT_GENERIC;
        else if (str == "I")
                return PORT_INPUT;
        else if (str == "O")
                return PORT_OUTPUT;
        else if (str == "B")
                return PORT_BIDIRECTIONAL;
        else
                {
                //Attributo sbagliato
                xml_wrong_attribute = TRUE;
                xml_wrong_attribute_description = "shape=" + str;
                return O_CONNECTION;
                }
}

char	xml_read_powershape(QString str)
{
        if (str == "P")
                return SYMBOL_GROUND_POWER;
        else if (str == "S")
                return SYMBOL_GROUND_SIGNAL;
        else if (str == "E")
                return SYMBOL_GROUND_EARTH;
        else if (str == "C")
                return SYMBOL_POWER_CIRCLE;
        else if (str == "B")
                return SYMBOL_POWER_BAR;
        else
                {
                //Attributo sbagliato
                xml_wrong_attribute = TRUE;
                xml_wrong_attribute_description = "shape=" + str;
                return O_CONNECTION;
                }
}

char	xml_read_padshape(QString str)
{
        if (str == "E")
                return PS_ROUND;
        else if (str == "R")
		return PS_RECTANGLE;
        else if (str == "RR")
		return PS_ROUNDED_RECTANGLE;
	else
		{
		//Attributo sbagliato
		xml_wrong_attribute = TRUE;
		xml_wrong_attribute_description = "shape=" + str;	
		return O_CONNECTION;								
		}
}

void	xml_read_polygon(QString str, QPolygon *p)
{
        QStringList sl = str.split(",");
	for (int i = 0; i < (sl.count() - 1); i += 2)
                p->append(QPoint(sl.at(i).toInt(), sl.at(i+1).toInt()));
}

void	xml_read_up(QString str, QList<ungated_pins_struct> up)
{
	up.clear();
	QStringList sl = str.split(",");
	for (int i = 0; i < (sl.count() - 1); i += 2)
		{
		ungated_pins_struct up_value;
		up_value.pin = sl.at(i).toInt();
		up_value.netname = sl.at(i + 1);
		up.append(up_value);
		}
}

char	xml_read_char(QString str)
{
	return str.at(0).toAscii();
}

int	xml_read_penwidth(QString str, bool is_sch)
{
        if (is_sch)
                {
                if (str == QObject::tr("Normal"))
                        return SCH_PEN_NORMAL;
                else if (str == QObject::tr("SemiBold"))
                        return SCH_PEN_SEMIBOLD;
                else
                        return SCH_PEN_BOLD;
                }
        else
                return str.toInt();
}

QString	open_prj(QWidget *parent)
{
	//METODO 1
        /*QFileDialog *dialog = new QFileDialog(parent, QObject::tr("Open Project"),  directory_prj, "Itecad Project (*.ite)");
	dialog->setAcceptMode(QFileDialog::AcceptOpen);
	dialog->setFileMode(QFileDialog::AnyFile);
	if (dialog->exec())
		{
		QStringList fileNames = dialog->selectedFiles();
		if (!fileNames.isEmpty())
			{
			delete dialog;
			return QDir::toNativeSeparators(fileNames.first());
			}
		}
	delete dialog;
        return "";*/

        //METODO 2 (E' importante mettere  | QFileDialog::DontUseNativeDialog perchè così la finestra viene posizionata correttamente)
        return QDir::toNativeSeparators(QFileDialog::getOpenFileName(parent, QObject::tr("Open Project"), directory_prj, "Itecad Project (*.ite)", 0, QFileDialog::DontResolveSymlinks  | QFileDialog::DontUseNativeDialog));
}

QString save_pdf(QWidget *parent, QString file)
{
        if (file == "")
                file = directory_prj;
        //METODO 1
        /*if (file.isEmpty())
                file = directory_prj;
        QFileDialog *dialog = new QFileDialog(parent, QObject::tr("Print to PDF"),  file, "Acrobat Reader (*.pdf)");
        dialog->setAcceptMode(QFileDialog::AcceptSave);
        dialog->setFileMode(QFileDialog::AnyFile);
        dialog->setDefaultSuffix("pdf");
        if (dialog->exec())
                {
                QStringList fileNames = dialog->selectedFiles();
                if (!fileNames.isEmpty())
                        {
                        delete dialog;
                        return set_another_suffix(QDir::toNativeSeparators(fileNames.first()), "pdf");
                        }
                }
        delete dialog;
        return "";*/

        //METODO 2 (E' importante mettere  | QFileDialog::DontUseNativeDialog perchè così la finestra viene posizionata correttamente)
        return set_another_suffix(QDir::toNativeSeparators(QFileDialog::getSaveFileName(parent, QObject::tr("Print to PDF"), file, "Acrobat Reader (*.pdf)", 0, QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog)), "pdf");
}

QString save_prj(QWidget *parent, QString file)
{
        if (file.isEmpty())
                file = directory_prj;
        //METODO 1
        /*QFileDialog *dialog = new QFileDialog(parent, QObject::tr("Save Project"),  file, "Itecad Project (*.ite)");
	dialog->setAcceptMode(QFileDialog::AcceptSave);
	dialog->setFileMode(QFileDialog::AnyFile);
	dialog->setDefaultSuffix("ite");
	if (dialog->exec())
		{
		QStringList fileNames = dialog->selectedFiles();
		if (!fileNames.isEmpty())
			{
			delete dialog;
			return set_another_suffix(QDir::toNativeSeparators(fileNames.first()), "ite");
			}
		}
	delete dialog;
        return "";*/

        //METODO 2 (E' importante mettere  | QFileDialog::DontUseNativeDialog perchè così la finestra viene posizionata correttamente)
        return set_another_suffix(QDir::toNativeSeparators(QFileDialog::getSaveFileName(parent, QObject::tr("Save Project"), file, "Itecad Project (*.ite)", 0, QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog)), "ite");
}

QString	open_ssf(QWidget *parent)
{
	//METODO 1
        /*QFileDialog *dialog = new QFileDialog(parent, QObject::tr("Open Schematic Symbol File"),  directory_ssf, "Itecad Schematic Symbol File (*.ssf)");
	dialog->setAcceptMode(QFileDialog::AcceptOpen);
	dialog->setFileMode(QFileDialog::AnyFile);
	if (dialog->exec())
		{
		QStringList fileNames = dialog->selectedFiles();
		if (!fileNames.isEmpty())
			{
			delete dialog;
			return QDir::toNativeSeparators(fileNames.first());
			}
		}
	delete dialog;
        return "";*/

        //METODO 2 (E' importante mettere  | QFileDialog::DontUseNativeDialog perchè così la finestra viene posizionata correttamente)
        return QDir::toNativeSeparators(QFileDialog::getOpenFileName(parent, QObject::tr("Open Schematic Symbol File"), directory_ssf, "Itecad Schematic Symbol File (*.ssf)", 0, QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog));
}

QString save_ssf(QWidget *parent, QString file)
{
        if (file.isEmpty())
                file = directory_ssf;
        //METODO 1
        /*QFileDialog *dialog = new QFileDialog(parent, QObject::tr("Save Schematic Symbol File"),  file, "Itecad Schematic Symbol File (*.ssf)");
	dialog->setAcceptMode(QFileDialog::AcceptSave);
	dialog->setFileMode(QFileDialog::AnyFile);
	dialog->setDefaultSuffix("ssf");
	if (dialog->exec())
		{
		QStringList fileNames = dialog->selectedFiles();
		if (!fileNames.isEmpty())
			{
			delete dialog;
			return set_another_suffix(QDir::toNativeSeparators(fileNames.first()), "ssf");
			}
		}
	delete dialog;
        return "";*/

        //METODO 2 (E' importante mettere  | QFileDialog::DontUseNativeDialog perchè così la finestra viene posizionata correttamente)
        return set_another_suffix(QDir::toNativeSeparators(QFileDialog::getSaveFileName(parent, QObject::tr("Save Schematic Symbol File"), file, "Itecad Schematic Symbol File (*.ssf)", 0, QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog)), "ssf");
}

QString	open_pff(QWidget *parent)
{
	//METODO 1
        /*QFileDialog *dialog = new QFileDialog(parent, QObject::tr("Open PCB Footprint File"),  directory_pff, "Itecad PCB Footprint File (*.pff)");
	dialog->setAcceptMode(QFileDialog::AcceptOpen);
	dialog->setFileMode(QFileDialog::AnyFile);
	if (dialog->exec())
		{
		QStringList fileNames = dialog->selectedFiles();
		if (!fileNames.isEmpty())
			{
			delete dialog;
			return set_another_suffix(QDir::toNativeSeparators(fileNames.first()), "pff");
			}
		}
	delete dialog;
        return "";*/

        //METODO 2 (E' importante mettere  | QFileDialog::DontUseNativeDialog perchè così la finestra viene posizionata correttamente)
        return QDir::toNativeSeparators(QFileDialog::getOpenFileName(parent, QObject::tr("Open PCB footprint File"), directory_pff, "Itecad PCB Footprint File (*.pff)", 0, QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog));
}

QString save_pff(QWidget *parent, QString file)
{
        if (file.isEmpty())
                file = directory_pff;
	//METODO 1
        /*QFileDialog *dialog = new QFileDialog(parent, QObject::tr("Save PCB Footprint File"),  file, "Itecad PCB Footprint File (*.pff)");
	dialog->setAcceptMode(QFileDialog::AcceptSave);
	dialog->setFileMode(QFileDialog::AnyFile);
	dialog->setDefaultSuffix("pff");
	if (dialog->exec())
		{
		QStringList fileNames = dialog->selectedFiles();
		if (!fileNames.isEmpty())
			{
			delete dialog;
			return set_another_suffix(QDir::toNativeSeparators(fileNames.first()), "pff");
			}
		}
	delete dialog;
        return "";*/

        //METODO 2 (E' importante mettere  | QFileDialog::DontUseNativeDialog perchè così la finestra viene posizionata correttamente)
        return set_another_suffix(QDir::toNativeSeparators(QFileDialog::getSaveFileName(parent, QObject::tr("Save PCB Footprint File"), file, "Itecad PCB Footprint File (*.pff)", 0, QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog)), "pff");
}

QString open_dir(QWidget *parent, QString description, QString dir)
{
        //METODO 1
        /*QFileDialog *dialog = new QFileDialog(parent, QObject::tr("Choose a directory"), dir);
        //dialog->setAcceptMode(QFileDialog::AcceptSave);
        //dialog->setFileMode(QFileDialog::Directory);
        dialog->setOptions(QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly);
        if (dialog->exec())
                {
                QStringList fileNames = dialog->selectedFiles();
                if (!fileNames.isEmpty())
                        {
                        delete dialog;
                        return QDir::toNativeSeparators(fileNames.first());
                        }
                }
        delete dialog;
        return "";*/

        //METODO 2 (E' importante mettere  | QFileDialog::DontUseNativeDialog perchè così la finestra viene posizionata correttamente)
        return QDir::toNativeSeparators(QFileDialog::getExistingDirectory(parent, description, dir, QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly | QFileDialog::DontUseNativeDialog));
}

QString	open_chr(QWidget *parent)
{
	//METODO 1
        /*QFileDialog *dialog = new QFileDialog(parent, QObject::tr("Open Char Symbol File"),  QDir::currentPath() + "/Internal_Vector_Font", "Itecad Char Symbol File (*.chr)");
	dialog->setAcceptMode(QFileDialog::AcceptOpen);
	dialog->setFileMode(QFileDialog::AnyFile);
	if (dialog->exec())
		{
		QStringList fileNames = dialog->selectedFiles();
		if (!fileNames.isEmpty())
			{
			delete dialog;
			return QDir::toNativeSeparators(fileNames.first());
			}
		}
	delete dialog;
        return "";*/

        //METODO 2 (E' importante mettere  | QFileDialog::DontUseNativeDialog perchè così la finestra viene posizionata correttamente)
        return QDir::toNativeSeparators(QFileDialog::getOpenFileName(parent, QObject::tr("Open Char Symbol File"), directory_ivf, "Itecad Char Symbol File (*.chr)", 0, QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog));
}

QString save_chr(QWidget *parent, QString file)
{
        if (file.isEmpty())
                file = directory_ivf;
        //METODO 1
        /*QFileDialog *dialog = new QFileDialog(parent, QObject::tr("Save Char Symbol File"), file, "Itecad Char Symbol File (*.chr)");
	dialog->setAcceptMode(QFileDialog::AcceptSave);
	dialog->setFileMode(QFileDialog::AnyFile);
	dialog->setDefaultSuffix("chr");
	if (dialog->exec())
		{
		QStringList fileNames = dialog->selectedFiles();
		if (!fileNames.isEmpty())
			{
			delete dialog;
			return set_another_suffix(QDir::toNativeSeparators(fileNames.first()), "chr");
			}
		}
	delete dialog;
        return "";*/

        //METODO 2 (E' importante mettere  | QFileDialog::DontUseNativeDialog perchè così la finestra viene posizionata correttamente)
        return set_another_suffix(QDir::toNativeSeparators(QFileDialog::getSaveFileName(parent, QObject::tr("Save Char Symbol File"), file, "Itecad Char Symbol File (*.chr)", 0, QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog)), "chr");
}

QString	set_another_suffix(QString filename, QString suffix)
{
	//Se il file viene salvato con un suffisso diverso da quello desiderato io ci aggiungo il suffisso giusto
	//Ad esempio cap_pol5.08D10  viene salvato come cap_pol5.08D10.pff
        if (filename.isNull())
                return filename;    //Se il filename è nullo non faccio niente. Probabilmente in precedenza è avvenuto un Cancel
        if (filename.right(4) != ("." + suffix))
		return filename += ("." + suffix);
	else
		return filename;
}

QString	footprint_name(QString footprint)
{
	int pos1 = footprint.lastIndexOf('\\');
	int pos2 = footprint.lastIndexOf('/');
	if (pos1 > pos2)
		return footprint.mid(++pos1);
	else
		return footprint.mid(++pos2);	
}

QString	footprint_subpath(QString footprint)
{
        QString result = footprint.left(footprint.length() - footprint_name(footprint).length());
	
	if (result.endsWith('\\'))
		result = result.left(result.length() - 1);
	if (result.endsWith('/'))
		result = result.left(result.length() - 1);

	return result;
}

QPoint	rotate_point(QPoint p, QPoint center, bool clockwise)
{
	if (clockwise)
		return QPoint(center.x() - p.y() + center.y(), center.y() + p.x() - center.x());
	else
		return QPoint(center.x() + p.y() - center.y(), center.y() - p.x() + center.x());
}

QPoint	mirror_point(QPoint p, QPoint center, bool horizontal)
{
	if (horizontal)
		{
		p.setX(2*center.x() - p.x());
		return p;
		}
	else
		{
		p.setY(2*center.y() - p.y());
		return p;
		}
}

int		arrotonda_a_intero(qreal x)
{
	//Purtroppo la funzione round non è definita dentro math.h e devo riscriverla io...
	/*if (x > 0)
		return (int) (x + 0.5);
	else
		{
		x = -x;
		return	-((int) (x + 0.5));
		}*/
	bool is_negative = FALSE;
	if (x < 0)
		{
		x = -x;
		is_negative = TRUE;
		}
	int  temp = (int) (x + 0.05);
	if (is_negative)
		return - temp;
	else
		return temp;
}

float	arrotonda_a_0_decimali(qreal x)
{
	//Purtroppo la funzione round non è definita dentro math.h e devo riscriverla io...
	bool is_negative = FALSE;
	if (x < 0)
		{
		x = -x;
		is_negative = TRUE;
		}
	int  temp = (int) (x + 0.05);
	if (is_negative)
		return -(float) temp;
	else
		return (float) temp;
}

float	arrotonda_a_1_decimali(qreal x)
{
	//Purtroppo la funzione round non è definita dentro math.h e devo riscriverla io...
	bool is_negative = FALSE;
	if (x < 0)
		{
		x = -x;
		is_negative = TRUE;
		}
	int  temp = (int) ((x + 0.05) * 10.0);
	if (is_negative)
		return -(float) temp / 10.0;
	else
		return (float) temp / 10.0;
}

float	arrotonda_a_2_decimali(qreal x)
{
	//Purtroppo la funzione round non è definita dentro math.h e devo riscriverla io...
	bool is_negative = FALSE;
	if (x < 0)
		{
		x = -x;
		is_negative = TRUE;
		}
	int  temp = (int) ((x + 0.005) * 100.0);
	if (is_negative)
		return -(float) temp / 100.0;
	else
		return (float) temp / 100.0;
}

float	arrotonda_a_3_decimali(qreal x)
{
	//Purtroppo la funzione round non è definita dentro math.h e devo riscriverla io...
	bool is_negative = FALSE;
	if (x < 0)
		{
		x = -x;
		is_negative = TRUE;
		}
	int  temp = (int) ((x + 0.0005) * 1000.0);
	if (is_negative)
		return -(float) temp / 1000.0;
	else
		return (float) temp / 1000.0;
}

float	arrotonda_a_4_decimali(qreal x)
{
	//Purtroppo la funzione round non è definita dentro math.h e devo riscriverla io...
	bool is_negative = FALSE;
	if (x < 0)
		{
		x = -x;
		is_negative = TRUE;
		}
	int  temp = (int) ((x + 0.00005) * 10000.0);
	if (is_negative)
		return -(float) temp / 10000.0;
	else
		return (float) temp / 10000.0;
}

float	arrotonda_a_5_decimali(qreal x)
{
	//Purtroppo la funzione round non è definita dentro math.h e devo riscriverla io...
	bool is_negative = FALSE;
	if (x < 0)
		{
		x = -x;
		is_negative = TRUE;
		}
	int  temp = (int) ((x + 0.000005) * 100000.0);
	if (is_negative)
		return -(float) temp / 100000.0;
	else
		return (float) temp / 100000.0;
}

float	arrotonda_a_6_decimali(qreal x)
{
	//Purtroppo la funzione round non è definita dentro math.h e devo riscriverla io...
	bool is_negative = FALSE;
	if (x < 0)
		{
		x = -x;
		is_negative = TRUE;
		}
	int  temp = (int) ((x + 0.0000005) * 1000000.0);
	if (is_negative)
		return -(float) temp / 1000000.0;
	else
		return (float) temp / 1000000.0;
}

unsigned char    node_layer_from(node_data *node)
{
        if (node->connected_pin_or_pad_or_via)
                {
                myGraphicsItem *pin_or_pad_or_via = (myGraphicsItem *) node->connected_pin_or_pad_or_via;
                switch (pin_or_pad_or_via->tipo)
                {
                case O_PIN:
                case O_PORT:
                case O_POWER:
                        return L_SCHWire;
                case O_PAD:
                case O_VIA:
                        return pin_or_pad_or_via->layer_from;
                }
                }
        else
                {
                myGraphicsItem *wire = (myGraphicsItem *) node->connected_wires.first();
                return wire->layer_from;
                }
}

unsigned char    node_layer_to(node_data *node)
{
        if (node->connected_pin_or_pad_or_via)
                {
                myGraphicsItem *pin_or_pad_or_via = (myGraphicsItem *) node->connected_pin_or_pad_or_via;
                switch (pin_or_pad_or_via->tipo)
                {
                case O_PIN:
                case O_PORT:
                case O_POWER:
                        return L_SCHWire;
                case O_PAD:
                case O_VIA:
                        return pin_or_pad_or_via->layer_to;
                }
                }
        else
                {
                myGraphicsItem *wire = (myGraphicsItem *) node->connected_wires.first();
                return wire->layer_from;
                }
}

bool    node_layer_intersect(node_data *node, char l_from, char l_to)
{
        char node_from = node_layer_from(node);
        char node_to = node_layer_to(node);

        if (node_from <= l_from)
                {
                if (node_to >= l_from)
                        return TRUE;
                }
        else
                {
                if (l_to >= node_from)
                        return TRUE;
                }
        return FALSE;
}

