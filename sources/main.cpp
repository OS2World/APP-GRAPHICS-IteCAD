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

#include <QApplication>
#include <QFontDatabase>
#include <QTextEdit>
#include <QClipboard>
#include "tipi_dato.h"
#include "myGraphicsView.h"
#include "myGraphicsScene.h"
#include "win_main.h"
#include "win_help_about.h"

//////////////////////////////////////////////
//	Variabili Globali
//////////////////////////////////////////////
QString			prog_version_string;
int			prog_version_int;

QSettings               *program_settings;

QString file_name;
QString file_path;
char	file_type = FT_NULL;

QString rf_path_1, rf_path_2, rf_path_3, rf_path_4;

QApplication	*application;
win_main	*w_main;
myGraphicsView	*GV;
myGraphicsScene	*GS;
QTextEdit	*TE;
lay_data	layer_data[LAYER_DATA_COUNT];
lay_data	workspace_data[WORKSPACE_DATA_COUNT];

unsigned char UM;				//Tutto l'input-output utilizza questa unità di misura: solo la griglia ha una sua unità di misura specifica.
UM_description	UM_descriptions[UM_DESCRIPTION_COUNT] = 
{ 
	//Uso come unità base il 1/100 di thou (10^-5 inch) che corrisponde a 0,254micrometri
	//Sotto i 0,254 micrometri è inutile scendere perchè i costruttori di PCB non riescono ad essere più precisi
	//Conviene avere come unità base un sottomultiplo del pollice perchè sul PCB sono più esatti i calcoli e le griglie
        {"mm",		3937.0,		3},	//1000.0,		//0
        {"inch",	100000.0,	5},	//25400.0,		//1
        {"thou",	100.0,		2},	//25.4,			//2
        {"metre",	3937000.0,	6},	//1000000.0,	//3
        {"cm",		39370.0,	4},	//10000.0,		//4
        {"micron",	3.937,		0}	//1.0,			//5
};

//Preferenze delle directory
QString directory_prj;
QString directory_ssf;
QString directory_pff;
QString directory_ivf;

//Preferenze dell'Environment
QString	environment_language;
bool    y_axis_is_up;   //In inglese si usa sempre il plurale (axis oppure axes) e mai il singolare axe

bool	xml_wrong_attribute;
QString	xml_wrong_attribute_description;

stampa_formato	stampa_formati[STAMPA_FORMATI_COUNT] = 
{ 
        {3311017, 2338578, QPrinter::A1, QT_TR_NOOP("A1"), 0},	//841000, 594000, QPrinter::A1, QT_TR_NOOP("A1"), 0,
        {2338578, 1653540, QPrinter::A2, QT_TR_NOOP("A2"), 0},	//594000, 420000, QPrinter::A2, QT_TR_NOOP("A2"), 0,
        {1653540, 1169289, QPrinter::A3, QT_TR_NOOP("A3"), 0}, 	//420000, 297000, QPrinter::A3, QT_TR_NOOP("A3"), 0,
        {1169289, 826770, QPrinter::A4, QT_TR_NOOP("A4"), 0}, 	//297000, 210000, QPrinter::A4, QT_TR_NOOP("A4"), 0,
        {826770, 582676, QPrinter::A5, QT_TR_NOOP("A5"), 0},		//210000, 148000, QPrinter::A5, QT_TR_NOOP("A5"), 0,
        {582676, 413385, QPrinter::A6, QT_TR_NOOP("A6"), 0},		//148000, 105000, QPrinter::A6, QT_TR_NOOP("A6"), 0,
        {413385, 291338, QPrinter::A7, QT_TR_NOOP("A7"), 0},		//105000, 74000, QPrinter::A7, QT_TR_NOOP("A7"), 0,
        {2027555, 1433068, QPrinter::B3, QT_TR_NOOP("B3"), 0},	//515000, 364000, QPrinter::B3, QT_TR_NOOP("B3"), 0,
        {1433068, 1011809, QPrinter::B4, QT_TR_NOOP("B4"), 0},	//364000, 257000, QPrinter::B4, QT_TR_NOOP("B4"), 0,
        {1011809, 716534, QPrinter::B5, QT_TR_NOOP("B5"), 0},		//257000, 182000, QPrinter::B5, QT_TR_NOOP("B5"), 0,
        {716534, 503936, QPrinter::B6, QT_TR_NOOP("B6"), 0},		//182000, 128000, QPrinter::B6, QT_TR_NOOP("B6"), 0,
        {503936, 358267, QPrinter::B7, QT_TR_NOOP("B7"), 0},		//128000, 91000, QPrinter::B7, QT_TR_NOOP("B7"), 0,
        {358267, 251968, QPrinter::B8, QT_TR_NOOP("B8"), 0},		//91000, 64000, QPrinter::B8, QT_TR_NOOP("B8"), 0,
        {901573, 641731, QPrinter::C5E, QT_TR_NOOP("C5E"), 0},	//229000, 163000, QPrinter::C5E, QT_TR_NOOP("C5E"), 0,
        {948817, 413385, QPrinter::Comm10E, QT_TR_NOOP("Comm10E"), 0},		//241000, 105000, QPrinter::Comm10E, QT_TR_NOOP("Comm10E"), 0,
        {866140, 433070, QPrinter::DLE, QT_TR_NOOP("DLE"), 0},				//220000, 110000, QPrinter::DLE, QT_TR_NOOP("DLE"), 0,
        {1000000, 750000, QPrinter::Executive, QT_TR_NOOP("Executive"), 1},	//254000, 190500, QPrinter::Executive, QT_TR_NOOP("Executive"), 1,
        {1299210, 826770, QPrinter::Folio, QT_TR_NOOP("Folio"), 0},			//330000, 210000, QPrinter::Folio, QT_TR_NOOP("Folio"), 0,
        {1700784, 1098423, QPrinter::Ledger, QT_TR_NOOP("Ledger"), 0},		//432000, 279000, QPrinter::Ledger, QT_TR_NOOP("Ledger"), 0,
        {1400000, 850000, QPrinter::Legal, QT_TR_NOOP("Legal"), 1},			//355600, 215900, QPrinter::Legal, QT_TR_NOOP("Legal"), 1,
        {1100000, 850000, QPrinter::Letter, QT_TR_NOOP("Letter"), 1},			//279400, 215900, QPrinter::Letter, QT_TR_NOOP("Letter"), 1,
        {1700784, 1098423, QPrinter::Tabloid, QT_TR_NOOP("Tabloid"), 0},		//432000, 279000, QPrinter::Tabloid, QT_TR_NOOP("Tabloid"), 0,
        {1169289, 826770, QPrinter::Custom, QT_TR_NOOP("Custom (mm)"), 0},	//297000, 210000, QPrinter::Custom, QT_TR_NOOP("Custom (mm)"), 0,
        {1100000, 850000, QPrinter::Custom, QT_TR_NOOP("Custom (inch)"), 1}	//279400, 215900, QPrinter::Custom, QT_TR_NOOP("Custom (inch)"), 1
};

/*QString win_mac_standard_font[12][2] =		//Colonna0 Win(98/200/XP/Vista), Colonna1 Mac
{
	"Lucida Sans Unicode", "Lucida Grande",	//Lucida Grande è il font di sistema di Mac OS X
	"Arial", "Arial",				
	"Arial Black", "Arial Black",			//Arial Black o Gadget sul Mac
	"Courier New", "Courier New",			
	"Comic Sans MS", "Comic Sans MS",
	"Lucida Console", "Monaco",
	"MS Sans Serif", "Geneva",
	"Symbol", "Symbol",
	"Tahoma", "Geneva",
	"Times New Roman", "Times",
	"Verdana", "Verdana",
	"Webdings", "Webdings"
};*/
QFont		font_pin;
char		font_pin_height = 10;

QPrinter	*default_printer = 0;
QPrinter	*pdf_printer = 0;

QPoint		drag_or_move_delta;
bool		mode_drag_active;
bool            edit_node_in_pcb_is_all_layers;

QPen		pen_layer = QPen(QColor(0,0,0), 0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
QBrush		brush_layer = QBrush(QColor(0,0,0), Qt::SolidPattern);

QColor		sel_color;
QColor		sel_color_inverted;
QColor		sel_color_transparent;

int			idstamp_last_used;

myGraphicsView		*doc_pcb;
QList<myGraphicsView *> doc_sch;
QList<myGraphicsView *> doc_sch_and_pcb;
QList<QTextEdit *>	doc_txt;

QVector<ratsnest_struct> ratsnest;	//Qui conviene usare QVector invece di QList perchè:
                                        // - il tipo di oggetti contenuti occupa più spazio in memoria di un puntatore
                                        // - non devo creare nuovi oggetti ratsnest_struct che a furia di essere creati e distrutti mi comprometto la memoria
                                        //Conviene usare invece le QList per memorizzare nodi, oggetti, componenti, footprint perchè voglio che essi rimangano in memoria e non vengano cancellati per poterli usare con Undo/Redo
QPoint					dynamic_ratsnest_endpoint;
bool					dynamic_ratsnest_active;
QList<node_data *>                      dynamic_ratsnest_endpoints;

QVector<net_names_struct> net_names;	//Qui conviene usare QVector invece di QList perchè:
                                        // - il tipo di oggetti contenuti occupa più spazio in memoria di un puntatore
                                        // - non devo creare nuovi oggetti ratsnest_struct che a furia di essere creati e distrutti mi comprometto la memoria
                                        //Conviene usare invece le QList per memorizzare nodi, oggetti, componenti, footprint perchè voglio che essi rimangano in memoria e non vengano cancellati per poterli usare con Undo/Redo

QList<myGraphicsItem *>     cloned_obj;
QList<myComponent *>        cloned_comp;
QList<node_data *>          cloned_nodes;
char                        cloned_action;
QList<myGraphicsItem *>     clipboard_obj;
QList<myComponent *>        clipboard_comp;
QList<node_data *>          clipboard_nodes;
char                        clipboard_doc_type;
char                        clipboard_file_type;

bool		highlight_active;

win_style	*modeless_win_style;
win_find_text   *modeless_win_find_text;

int			mouse_click_num;
QPoint			mouse_new_press;
node_data		*mouse_new_press_node;
QPoint			mouse_old_press;
node_data		*mouse_old_press_node;
QPoint			mouse_release;
node_data		*mouse_release_node;
QPoint			mouse_move;
node_data		*mouse_move_node;
QPoint			mouse_right_button;
QPoint                  mouse_position;
char                    mouse_direction;

bool		drc_active;
bool		drc_check_low_insulation;
int		drc_v2v;
int		drc_v2l;
int		drc_l2l;
int		check_v2v;
int		check_v2l;
int		check_l2l;

bool		style_sch_fill_polygon;
char		style_sch_font_height;
bool		style_sch_font_bold;
bool		style_sch_font_italic;
int		style_sch_pen_width_wire;
int		style_sch_pen_width_normal;
int		style_sch_pen_width_frame;
bool		style_pcb_fill_polygon;
char		style_pcb_font_height;
bool		style_pcb_font_bold;
bool		style_pcb_font_italic;
int		style_pcb_pen_width_copper;
int		style_pcb_pen_width_silk;
int		style_pcb_pen_width_paste;
int		style_pcb_pen_width_mask;
int		style_pcb_pen_width_board;
int		style_pcb_pen_width_help;
char		style_pcb_pad_shape;
int		style_pcb_pad_width;
int		style_pcb_pad_height;
unsigned char	style_pcb_pad_layer_from;
unsigned char	style_pcb_pad_layer_to;
int		style_pcb_pad_drill;
int             style_pcb_via_width;
unsigned char	style_pcb_via_layer_from;
unsigned char	style_pcb_via_layer_to;
int             style_pcb_via_drill;

qreal           sch_grid_artistic;

bool		pcb_grid_visible;
qreal		pcb_grid_size;
bool            pcb_grid_thou;
char		pcb_grid_snap;

bool            pcb_pad_via_filled;

char            sch_artistic_grid;


factory_value   factory_values[FACTORY_VALUE_COUNT]   =
{
        // SCH Workspace
        {"SCH_colour_background", QColor(100,100,100), QColor(100,100,100), QVariant::Color},
        {"SCH_colour_page", QColor(255,255,255), QColor(255,255,255), QVariant::Color},
        {"SCH_colour_grid", QColor(0,0,0), QColor(0,0,0), QVariant::Color},
        {"SCH_colour_selection", QColor(255,0,0), QColor(255,0,0), QVariant::Color},

        // PCB Workspace
        {"PCB_colour_background", QColor(0,0,0), QColor(0,0,0), QVariant::Color},
        {"PCB_colour_grid", QColor(255,255,255), QColor(255,255,255), QVariant::Color},
        {"PCB_colour_pad_through", QColor(0,255,0), QColor(0,255,0), QVariant::Color},
        {"PCB_colour_pad_top", QColor(0,255,0), QColor(0,255,0), QVariant::Color},
        {"PCB_colour_pad_bottom", QColor(255,0,0), QColor(0,255,0), QVariant::Color},
        {"PCB_colour_via", QColor(0,0,255), QColor(0,255,0), QVariant::Color},
        {"PCB_colour_buried_via", QColor(0,0,255), QColor(0,255,0), QVariant::Color},
        {"PCB_colour_ratsnest", QColor(255,255,255), QColor(255,255,255), QVariant::Color},
        {"PCB_colour_dynamic_ratsnest", QColor(255,0,0), QColor(255,0,0), QVariant::Color},
        {"PCB_colour_selection", QColor(255,255,255), QColor(255,255,255), QVariant::Color},

        // SCH layers
        {"SCH_colour_normal", QColor(0,0,0), QColor(0,0,0), QVariant::Color},
        {"SCH_colour_frame", QColor(0,0,0), QColor(0,0,0), QVariant::Color},
        {"SCH_colour_wire", QColor(0,0,255), QColor(0,0,255), QVariant::Color},

        // PCB Layers
        {"PCB_colour_board", QColor(0,0,255), QColor(0,0,255), QVariant::Color},
        {"PCB_colour_pastetop", QColor(255,0,0), QColor(255,0,0), QVariant::Color},
        {"PCB_colour_silktop", QColor(255,255,0), QColor(255,255,0), QVariant::Color},
        {"PCB_colour_masktop", QColor(255,0,0), QColor(255,0,0), QVariant::Color},
        {"PCB_colour_copper1", QColor(0,255,0), QColor(0,255,0), QVariant::Color},
        {"PCB_colour_copper16", QColor(255,0,0), QColor(255,0,0), QVariant::Color},
        {"PCB_colour_maskbottom", QColor(255,0,0), QColor(255,0,0), QVariant::Color},
        {"PCB_colour_silkbottom", QColor(255,0,0), QColor(255,0,0), QVariant::Color},
        {"PCB_colour_pastebottom", QColor(255,0,0), QColor(255,0,0), QVariant::Color},
        {"PCB_colour_help", QColor(0,0,0), QColor(0,0,0), QVariant::Color},

        // SCH style
        {"SCH_pen_width_normal", SCH_PEN_NORMAL, SCH_PEN_NORMAL, QVariant::Int},
        {"SCH_pen_width_frame", SCH_PEN_NORMAL, SCH_PEN_NORMAL, QVariant::Int},
        {"SCH_pen_width_wire", SCH_PEN_NORMAL, SCH_PEN_NORMAL, QVariant::Int},
        {"SCH_fill_polygon", FALSE, FALSE, QVariant::Bool},
        {"SCH_font_height", 10, 10, QVariant::Int},
        {"SCH_font_bold", FALSE, FALSE, QVariant::Bool},
        {"SCH_font_italic", FALSE, FALSE, QVariant::Bool},

        // PCB style
        {"PCB_pen_width_copper", 1500,1500, QVariant::Int},	//Equivalente a 0.381mm
        {"PCB_pen_width_silk", 500, 500, QVariant::Int},		//Equivalente a 0,127mm
        {"PCB_pen_width_paste", 500, 500, QVariant::Int},	//Equivalente a 0,127mm
        {"PCB_pen_width_mask", 500, 500, QVariant::Int}, 	//Equivalente a 0,127mm
        {"PCB_pen_width_board", 500, 500, QVariant::Int},	//Equivalente a 0,127mm
        {"PCB_pen_width_help", 500, 500, QVariant::Int}, 	//Equivalente a 0,127mm
        {"PCB_fill_polygon", FALSE, FALSE, QVariant::Bool},
        {"PCB_font_height", 10, 10, QVariant::Int},
        {"PCB_font_bold", FALSE, FALSE, QVariant::Bool},
        {"PCB_font_italic", FALSE, FALSE, QVariant::Bool},
        {"PCB_pad_shape", PS_ROUND, PS_ROUND, QVariant::Int},
        {"PCB_pad_width", 5511, 5511, QVariant::Int},		//Equivalente a 1.4mm	(essendo un programma per un target europeo conviene fare così)
        {"PCB_pad_height", 5511, 5511, QVariant::Int},		//Equivalente a 1.4mm	(essendo un programma per un target europeo conviene fare così)
        {"PCB_pad_layer_from", L_PCBCopper1, L_PCBCopper1, QVariant::Int},
        {"PCB_pad_layer_to", L_PCBCopper16, L_PCBCopper16, QVariant::Int},
        {"PCB_pad_drill", 3149, 3149, QVariant::Int},		//Equivalente a 0.8mm	(essendo un programma per un target europeo conviene fare così)
        {"PCB_via_width", 5511, 5511, QVariant::Int},		//Equivalente a 1.4mm	(essendo un programma per un target europeo conviene fare così)
        {"PCB_via_layer_from", L_PCBCopper1, L_PCBCopper1, QVariant::Int},
        {"PCB_via_layer_to", L_PCBCopper16, L_PCBCopper16, QVariant::Int},
        {"PCB_via_drill", 3149, 3149, QVariant::Int},		//Equivalente a 0.8mm	(essendo un programma per un target europeo conviene fare così)

        // SCH Grid
        {"SCH_artistic_grid", 2500.0, 2500.0, QVariant::Double},

        // PCB Grid
        {"PCB_grid_visible", TRUE, TRUE, QVariant::Bool},
        {"PCB_grid_size", 50.0, 50.0, QVariant::Double},                 //0.05" di default per il PCB (la griglia del SCH viene definita in modo diverso)
        {"PCB_grid_thou", TRUE, TRUE, QVariant::Bool},                   //thou
        {"PCB_grid_snap", 'I', 'I', QVariant::Int},

        //PCB General
        {"PCB_pad_via_filled", FALSE, FALSE, QVariant::Bool}

        //Suggerisco: Axis x from left to right
        //Suggerisco: Axis y from top to bottom
        //Suggerisco: "PCB_origin_x", 0, 0,
        //Suggerisco: "PCB_origin_y", 0, 0,

        //SCH General
        //Suggerisco: "SCH_show_ungated_pins", 10, 10
};

QRegExp exp_length("(-?\\d{0,6})[.,](\\d{0,6})");
QRegExp exp_positive_length("(\\d{0,6})[.,](\\d{0,6})");
QRegExp exp_zoom("(\\d{1,7})");
QRegExp exp_converter("(\\d{0,10})[.,](\\d{0,10})");
QRegExp exp_pin_number("[1-9]\\d{0,3}");
//QRegExp exp_alphabet("\"[a-zA-Z0-9 .-,]*\"");
QRegExp exp_exclude_separator("[^;]+");

QPoint  ruler_start;

QPolygon    text_points;
QPolygon    text_lines;

QClipboard  *clipboard;

QPoint port_l, port_r, port_tl, port_tr, port_bl, port_br;

QImage  composition_mode_image = QImage(1,1, QImage::Format_ARGB32_Premultiplied);

int main(int argc, char *argv[])
{
        prog_version_string = "1.2.0";
        prog_version_int = 120;

        QApplication a(argc, argv);

        directory_ssf = QDir::currentPath() + "/Library/SDT";
        directory_pff = QDir::currentPath() + "/Library/PCB";
        directory_ivf = QDir::currentPath() + "/Internal_Vector_Font";

        //Controllo che sia presente la sottodirectory Internal_Vector_Font
        QDir dir(directory_ivf);
        if (!dir.exists())
                {
                //Questo errore può capitare nel QtCreator quando è abilitato "Shadow Build" in Build Settings
                //oppure quando è impostata male "Working directory" in Run Settings
                QMessageBox::critical(0, "Itecad", QObject::tr("Can't load internal vector font in:\n%1").arg(directory_ivf));
                return 0;
                }

        //Metto queste istruzioni in un blocco in modo da eliminare la finestra (e le sue animazioni) al termine del blocco
        {
        w_main = 0; //Segnalo allo splashscreen seguente che la finestra principale non è ancora stata creata
        win_help_about dialogo(0, Qt::SplashScreen);
        dialogo.exec();
        }

        application = &a;
        win_main w;
	w_main = &w;
        w.show();
        a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));

        clipboard = QApplication::clipboard();

	//Assegno l'icona del programma (Attenzione che il nome del file deve essere per forza Itecad.ico
	//Per assegnare un icona al programma si usa setWindowIcon, ma questo assegna solo l'icona alla mainwindow
	//e non al file del programma. Per assegnare un icona al programma il metodo più semplice è
        //modificare Itecad.ico (file contenente 3 formati 48x48, 32x32, 16x16)
        a.setWindowIcon(QIcon(":/Itecad.ico"));

        return a.exec();
}


