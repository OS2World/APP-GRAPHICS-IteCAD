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

#include <QtGui>
	
#include "win_main.h"
#include "win_gerber.h"
#include "win_find_comp.h"
#include "win_find_text.h"
#include "win_drill.h"
#include "win_print.h"

extern	QString prog_version_string;
extern	int prog_version_int;

extern	QApplication	*application;
extern	QString file_name;
extern	QString	file_path;
extern	char	file_type;	

extern	QString rf_path_1, rf_path_2, rf_path_3, rf_path_4;

extern	QList<myGraphicsView *> doc_sch;
extern	myGraphicsView		*doc_pcb;
extern	QList<myGraphicsView *> doc_sch_and_pcb;
extern	QList<QTextEdit *>	doc_txt;
extern	myGraphicsView          *GV;
extern	myGraphicsScene         *GS;
extern	QTextEdit		*TE;
extern	lay_data	layer_data[LAYER_DATA_COUNT];
extern	lay_data	workspace_data[WORKSPACE_DATA_COUNT];

extern	unsigned char UM;
extern	UM_description	UM_descriptions[UM_DESCRIPTION_COUNT];

extern	QString directory_prj;
extern	QString directory_ssf;
extern	QString directory_pff;
extern	QString directory_ivf;

extern	QString	environment_language;
extern  bool    y_axis_is_up;

extern	QString win_mac_standard_font[12][2];

extern	QFont font_pin;

extern	QPoint	drag_or_move_delta;
extern	bool	mode_drag_active;

extern  bool    edit_node_in_pcb_is_all_layers;

extern	myGraphicsView	*preview_GV;
extern	myGraphicsScene	*preview_GS;

extern	win_style	*modeless_win_style;
extern  win_find_text   *modeless_win_find_text;

extern	int		idstamp_last_used;

extern	QVector<ratsnest_struct>	ratsnest;
extern	QPoint				dynamic_ratsnest_endpoint;
extern	bool				dynamic_ratsnest_active;
extern  QList<node_data *>              dynamic_ratsnest_endpoints;

extern	QVector<net_names_struct> net_names;

extern	bool                    highlight_active;

extern	int			mouse_click_num;
extern	QPoint			mouse_new_press;
extern	node_data		*mouse_new_press_node;
extern	QPoint			mouse_old_press;
extern	node_data		*mouse_old_press_node;
extern	QPoint			mouse_release;
extern	QPoint			mouse_move;
extern	node_data		*mouse_move_node;
extern	QPoint			mouse_right_button;

extern  QList<myGraphicsItem *>     clipboard_obj;
extern  QList<myComponent *>        clipboard_comp;
extern  QList<node_data *>          clipboard_nodes;
extern  char                        clipboard_doc_type;
extern  char                        clipboard_file_type;
extern  char                        cloned_action;

preview	*comp_preview;

extern	bool	drc_active;
extern	bool	drc_check_low_insulation;

extern  QSettings       *program_settings;

extern  bool		style_sch_fill_polygon;
extern  char		style_sch_font_height;
extern  bool		style_sch_font_bold;
extern  bool		style_sch_font_italic;
extern  int		style_sch_pen_width_wire;
extern  int		style_sch_pen_width_normal;
extern  int		style_sch_pen_width_frame;
extern  bool		style_pcb_fill_polygon;
extern  char		style_pcb_font_height;
extern  bool		style_pcb_font_bold;
extern  bool		style_pcb_font_italic;
extern  int		style_pcb_pen_width_copper;
extern  int		style_pcb_pen_width_silk;
extern  int		style_pcb_pen_width_paste;
extern  int		style_pcb_pen_width_mask;
extern  int		style_pcb_pen_width_board;
extern  int		style_pcb_pen_width_help;
extern  char            style_pcb_pad_shape;
extern  int             style_pcb_pad_width;
extern  int             style_pcb_pad_height;
extern  unsigned char   style_pcb_pad_layer_from;
extern  unsigned char   style_pcb_pad_layer_to;
extern  int             style_pcb_pad_drill;
extern  int             style_pcb_via_width;
extern  unsigned char   style_pcb_via_layer_from;
extern  unsigned char   style_pcb_via_layer_to;
extern  int             style_pcb_via_drill;

extern  qreal		sch_grid_artistic;

extern  bool		pcb_grid_visible;
extern  qreal		pcb_grid_size;
extern  bool            pcb_grid_thou;
extern  char		pcb_grid_snap;
extern  bool		pcb_pad_via_filled;

extern  factory_value   factory_values[FACTORY_VALUE_COUNT];

win_main::win_main()
{
	//Controllo che siano presenti i font standard di sistema
	/*QFontDatabase *database = new QFontDatabase;
	QStringList families = database->families(QFontDatabase::Any); 
	for (int i = 0 ; i < 12; i++)
		if (!families.contains(win_mac_standard_font[i][0]))
			{
			QMessageBox::warning(0, QObject::tr("Warning"), QObject::tr("To work properly you must install the font %1.\nIt is a standard Windows font included in Win98/2000/XP/Vista").arg(win_mac_standard_font[i][0]));
			}

	delete database;

	font_pin = QFont("Lucida Sans Unicode", 2000);	//Font proposto al sistema operativo
	font_pin.setPixelSize(2000);					//Questa istruzione è molto importante (per non incorrere in errori conviene impostare i pixelsize pari ai pointsize)
	font_pin.setBold(FALSE);
	font_pin.setItalic(FALSE);*/
	
        program_settings = new QSettings("Iteco", "Itecad");
	QPoint pos;
	QSize size;
        size = program_settings->value("win_size", QSize(0,0)).toSize();
	if (size == QSize(0,0))
		showMaximized();	
	else
		{
		resize(size);
                pos = program_settings->value("win_pos", QPoint(0, 0)).toPoint();
		move(pos);
		}

        setWindowTitle(tr("Itecad"));
        setMinimumSize(400, 400);

        tool_avviato = FALSE;

	//Ricarico l'unità di misura utilizzata
        UM = UM_find_description(program_settings->value("measure_unit", "mm").toString());

	setup_actions();
		
        setup_menuBar();

        setup_toolBar();

        setup_statusBar();

        //Ricarico il layout usato l'ultima volta
        restoreState(program_settings->value("win_state").toByteArray(),prog_version_int);

        //this->setGeometry(10,50,872,622);//Con questi valori catturo un immagine di 880x660 pixel che è una risoluzione adatta per essere visualizzata sui browser internet

        //Ricarico le preferenze delle directory
        //** Prima perdevo non ricordavo correttamente il percorso delle directory
        directory_prj = QDir::toNativeSeparators(program_settings->value("directory_prj", QApplication::applicationDirPath() + "/Projects").toString());

        //Ricarico le preferenze dell'environment
        environment_language = program_settings->value("environment_language", "English").toString();
        y_axis_is_up = program_settings->value("y_axis_is_up", TRUE).toBool();
	
	//Ricarico i recent_files
        rf_path_1 = program_settings->value("recent_file_1", "").toString();
        rf_path_2 = program_settings->value("recent_file_2", "").toString();
        rf_path_3 = program_settings->value("recent_file_3", "").toString();
        rf_path_4 = program_settings->value("recent_file_4", "").toString();
	recent_file_update();

        // SCH Workspace
        workspace_data[W_SCHbackground].description = "Background";
        workspace_data[W_SCHbackground].user_description = QObject::tr("Background");
        workspace_data[W_SCHbackground].is_sch = TRUE;
        workspace_data[W_SCHpage].description = "Page";
        workspace_data[W_SCHpage].user_description = QObject::tr("Page");
        workspace_data[W_SCHpage].is_sch = TRUE;
        workspace_data[W_SCHgrid].description = "Grid";
        workspace_data[W_SCHgrid].user_description = QObject::tr("Grid");
        workspace_data[W_SCHgrid].is_sch = TRUE;
        workspace_data[W_SCHselection].description = "Selection";
        workspace_data[W_SCHselection].user_description = QObject::tr("Selection");
        workspace_data[W_SCHselection].is_sch = TRUE;
        // PCB Workspace
        workspace_data[W_PCBbackground].description = "Background";
        workspace_data[W_PCBbackground].user_description = QObject::tr("Background");
        workspace_data[W_PCBbackground].is_sch = FALSE;
        workspace_data[W_PCBgrid].description = "Grid";
        workspace_data[W_PCBgrid].user_description = QObject::tr("Grid");
        workspace_data[W_PCBgrid].is_sch = FALSE;        
        workspace_data[W_PCBpadthrough].description = "PadThrough";
        workspace_data[W_PCBpadthrough].user_description = QObject::tr("Pad Through");
        workspace_data[W_PCBpadthrough].is_sch = FALSE;
        workspace_data[W_PCBpadtop].description = "PadTop";
        workspace_data[W_PCBpadtop].user_description = QObject::tr("Pad Top");
        workspace_data[W_PCBpadtop].is_sch = FALSE;
        workspace_data[W_PCBpadbottom].description = "PadBottom";
        workspace_data[W_PCBpadbottom].user_description = QObject::tr("Pad Bottom");
        workspace_data[W_PCBpadbottom].is_sch = FALSE;
        workspace_data[W_PCBvia].description = "Via";
        workspace_data[W_PCBvia].user_description = QObject::tr("Via");
        workspace_data[W_PCBvia].is_sch = FALSE;
        workspace_data[W_PCBburiedvia].description = "ViaBuried";
        workspace_data[W_PCBburiedvia].user_description = QObject::tr("Buried Via");
        workspace_data[W_PCBburiedvia].is_sch = FALSE;
        workspace_data[W_PCBratsnest].description = "Ratsnest";
        workspace_data[W_PCBratsnest].user_description = QObject::tr("Ratsnest");
        workspace_data[W_PCBratsnest].is_sch = FALSE;
        workspace_data[W_PCBdynamicratsnest].description = "Dynamic Ratsnest";
        workspace_data[W_PCBdynamicratsnest].user_description = QObject::tr("Dynamic Ratsnest");
        workspace_data[W_PCBdynamicratsnest].is_sch = FALSE;
        workspace_data[W_PCBselection].description = "Selection";
        workspace_data[W_PCBselection].user_description = QObject::tr("Selection");
        workspace_data[W_PCBselection].is_sch = FALSE;
        // SCH layers
        layer_data[L_SCHNormal].description = "Normal";
        layer_data[L_SCHNormal].user_description = QObject::tr("Normal");
        layer_data[L_SCHNormal].is_sch = TRUE;
        layer_data[L_SCHNormal].is_copper = FALSE;
        layer_data[L_SCHFrame].description = "Frame";
        layer_data[L_SCHFrame].user_description = QObject::tr("Frame");
        layer_data[L_SCHFrame].is_sch = TRUE;
        layer_data[L_SCHFrame].is_copper = FALSE;
        layer_data[L_SCHWire].description = "Wire";
        layer_data[L_SCHWire].user_description = QObject::tr("Wire");
        layer_data[L_SCHWire].is_sch = TRUE;
        layer_data[L_SCHWire].is_copper = TRUE;
        // PCB Layers
        layer_data[L_PCBCopper1].description = "CopperTop";
        layer_data[L_PCBCopper1].user_description = QObject::tr("1 Copper (Top)");
        layer_data[L_PCBCopper1].is_sch = FALSE;
        layer_data[L_PCBCopper1].is_copper = TRUE;
        /*
        I layer interni li definirei così
        layer_data[L_PCBCopperInt1].description = "Copper2");
        layer_data[L_PCBCopperInt1].user_description = QObject::tr("2 Copper");
        layer_data[L_PCBCopperInt1].is_sch = FALSE;
        layer_data[L_PCBCopperInt1].is_copper = TRUE;
        */
        layer_data[L_PCBCopper16].description = "CopperBottom";
        layer_data[L_PCBCopper16].user_description = QObject::tr("16 Copper (Bottom)");
        layer_data[L_PCBCopper16].is_sch = FALSE;
        layer_data[L_PCBCopper16].is_copper = TRUE;
        layer_data[L_PCBSilkTop].description = "SilkTop";
        layer_data[L_PCBSilkTop].user_description = QObject::tr("Top Silk");
        layer_data[L_PCBSilkTop].is_sch = FALSE;
        layer_data[L_PCBSilkTop].is_copper = FALSE;
        layer_data[L_PCBSilkBottom].description = "SilkBottom";
        layer_data[L_PCBSilkBottom].user_description = QObject::tr("Bottom Silk");
        layer_data[L_PCBSilkBottom].is_sch = FALSE;
        layer_data[L_PCBSilkBottom].is_copper = FALSE;
        layer_data[L_PCBMaskTop].description = "MaskTop";
        layer_data[L_PCBMaskTop].user_description = QObject::tr("Top Mask");
        layer_data[L_PCBMaskTop].is_sch = FALSE;
        layer_data[L_PCBMaskTop].is_copper = FALSE;
        layer_data[L_PCBMaskBottom].description = "MaskBottom";
        layer_data[L_PCBMaskBottom].user_description = QObject::tr("Bottom Mask");
        layer_data[L_PCBMaskBottom].is_sch = FALSE;
        layer_data[L_PCBMaskBottom].is_copper = FALSE;
        layer_data[L_PCBPasteTop].description = "PasteTop";
        layer_data[L_PCBPasteTop].user_description = QObject::tr("Top Paste");
        layer_data[L_PCBPasteTop].is_sch = FALSE;
        layer_data[L_PCBPasteTop].is_copper = FALSE;
        layer_data[L_PCBPasteBottom].description = "PasteBottom";
        layer_data[L_PCBPasteBottom].user_description = QObject::tr("Bottom Paste");
        layer_data[L_PCBPasteBottom].is_sch = FALSE;
        layer_data[L_PCBPasteBottom].is_copper = FALSE;
        layer_data[L_PCBHelp].description = "Help";
        layer_data[L_PCBHelp].user_description = QObject::tr("Help");
        layer_data[L_PCBHelp].is_sch = FALSE;
        layer_data[L_PCBHelp].is_copper = FALSE;
        layer_data[L_PCBBoard].description = "Board";
        layer_data[L_PCBBoard].user_description = QObject::tr("Board");
        layer_data[L_PCBBoard].is_sch = FALSE;
        layer_data[L_PCBBoard].is_copper = FALSE;

        settings_factory2temp();
        settings_temp2current();

        change_file_type(FT_NULL);

        alphabet_load();
	
	mode_drag_active = TRUE;
	actions_mode_dragAct->setChecked(TRUE);

        edit_node_in_pcb_is_all_layers = TRUE;
        actions_edit_node_all_layersAct->setChecked(TRUE);

        comp_preview = new preview();

        help_user_manual_proc = 0;
}

win_main::~win_main()
{
    //Salvo il layout
    program_settings->setValue("win_pos", pos());
    program_settings->setValue("win_size", size());
    program_settings->setValue("win_state", saveState(prog_version_int));
}

void win_main::actions_mode_drag()
{
	if (GV->tool == T_SELECT)
		if (tool_avviato)
			return;
	
	mode_drag_active = TRUE;
	actions_mode_dragAct->setChecked(TRUE);
        actions_selectAct->setIcon(QIcon(":/Resources/cursor_blue.png"));
} 

void win_main::actions_mode_move()
{
	if (GV->tool == T_SELECT)
		if (tool_avviato)
			return;
	
	mode_drag_active = FALSE;
	actions_mode_moveAct->setChecked(TRUE);
        actions_selectAct->setIcon(QIcon(":/Resources/cursor_red.png"));
} 

void win_main::actions_edit_node_all_layers()
{
        if (GV->tool == T_SELECT)
                if (tool_avviato)
                        return;

        edit_node_in_pcb_is_all_layers = TRUE;
        actions_edit_node_all_layersAct->setChecked(TRUE);
        actions_edit_nodeAct->setIcon(QIcon(":/Resources/chart_line.png"));
}

void win_main::actions_edit_node_active_layer()
{
        if (GV->tool == T_SELECT)
                if (tool_avviato)
                        return;

        edit_node_in_pcb_is_all_layers = FALSE;
        actions_edit_node_active_layerAct->setChecked(TRUE);
        actions_edit_nodeAct->setIcon(QIcon(":/Resources/chart_line2.png"));
}

void win_main::actions_edit_component_rotate_r()
{

}

void win_main::actions_edit_component_rotate_l()
{

}

void win_main::actions_escape()
{
        GV->tool_stop();
} 

void win_main::actions_circle()
{
        GV->tool_change(T_ELLIPSE);
} 

void win_main::actions_hand()
{
	GV->tool_change(T_HAND);
} 

void win_main::actions_connection()
{
	GV->tool_change(T_CONNECTION);
} 

void win_main::mainwindow_panUp()
{
        if (file_type != FT_NULL)
                if (doc_type() != DT_TXT)
                        GV->pan_up();
}

void win_main::mainwindow_panDown()
{
        if (file_type != FT_NULL)
                if (doc_type() != DT_TXT)
                        GV->pan_down();
}

void win_main::mainwindow_panLeft()
{
        if (file_type != FT_NULL)
                if (doc_type() != DT_TXT)
                        GV->pan_left();
}

void win_main::mainwindow_panRight()
{
        if (file_type != FT_NULL)
                if (doc_type() != DT_TXT)
                        GV->pan_right();
}

void win_main::mainwindow_zoom_in()
{
        if (file_type != FT_NULL)
                if (doc_type() != DT_TXT)
                        GV->zoom_in(QGraphicsView::AnchorViewCenter);
}

void win_main::mainwindow_zoom_out()
{
        if (file_type != FT_NULL)
                if (doc_type() != DT_TXT)
                        GV->zoom_out(QGraphicsView::AnchorViewCenter);
}

void win_main::mainwindow_select_tab()
{
        //Trovo l'ultimo oggetto/componente selezionato
        int last_selected = 0;
        if (GV->selected_obj.count())
                last_selected = (int) GV->selected_obj.last();
        if (GV->selected_comp.count())
                last_selected = (int) GV->selected_comp.last();

        GV->setUpdatesEnabled(FALSE);

        GV->select_clear();

        QRectF visible_rect = QRectF(GV->mapToScene(0,0), GV->mapToScene(GV->width(),GV->height()));

        int     new_selected = 0;
        bool    last_selected_found = FALSE;
        bool    exit = FALSE;

        foreach(myComponent *comp, GV->all_comp)
                if (!exit)
                        if (QRectF(comp->bounding_rect()).intersects(visible_rect))
                                {
                                if (new_selected == 0)
                                        new_selected = (int) comp;
                                else if (last_selected_found)
                                        {
                                        new_selected = (int) comp;
                                        exit = TRUE;
                                        }
                                if (last_selected == (int) comp)
                                        last_selected_found = TRUE;
                                }

        foreach(myGraphicsItem	*obj, GS->all_obj)
                if (!exit)
                        if (GV->layer_settings[obj->layer_from].visible)
                                if (obj->comp == 0)
                                        if (QRectF(obj->boundingRect()).intersects(visible_rect))
                                                {
                                                if (new_selected == 0)
                                                        new_selected = (int) obj;
                                                else if (last_selected_found)
                                                        {
                                                        new_selected = (int) obj;
                                                        exit = TRUE;
                                                        }
                                                if (last_selected == (int) obj)
                                                        last_selected_found = TRUE;
                                                }

        //Seleziono il nuovo oggetto/componente
        if (new_selected)
                {
                if (GV->all_comp.contains((myComponent*) new_selected))
                        ((myComponent*) new_selected)->set_selected(TRUE);
                else if (GS->all_obj.contains((myGraphicsItem*) new_selected))
                        ((myGraphicsItem*) new_selected)->set_selected(TRUE);
                }

        GV->setUpdatesEnabled(TRUE);
}

void win_main::mainwindow_select_spacebar()
{
        if (mode_drag_active)
                actions_mode_move();
        else
                actions_mode_drag();
}

void win_main::actions_polyline()
{
	GV->tool_change(T_POLYLINE);
} 

void win_main::actions_polygon()
{
	GV->tool_change(T_POLYGON);
} 

void win_main::actions_rectangle()
{
	GV->tool_change(T_RECT);
} 

void win_main::actions_select()
{
	GV->tool_change(T_SELECT);
} 

void win_main::actions_edit_node()
{
        GV->tool_change(T_EDIT_NODE);
} 

void win_main::actions_text()
{
        GV->tool_change(T_TEXT);
}

void win_main::actions_pin()
{
	GV->tool_change(T_PIN);
}

void win_main::actions_padvia()
{
        GV->tool_change(T_PADVIA);
}

void win_main::actions_port()
{
	GV->tool_change(T_PORT);
}

void win_main::actions_set_origin()
{
        GV->tool_change(T_ORIGIN);
}

void win_main::actions_ruler()
{
        GV->tool_change(T_RULER);
}

void win_main::actions_add_power()
{
        GV->tool_change(T_ADD_POWER);
}

void win_main::actions_edit_component()
{
        GV->tool_change(T_EDIT_COMPONENT);
}

void win_main::actions_add()
{
	char old_tool_sch = tool_sch;
	char old_tool_pcb = tool_pcb;
	
	GV->tool_change(T_ADD);

	tool_sch = old_tool_sch;
	tool_pcb = old_tool_pcb;

	win_add_component dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, FALSE);
	if (dialogo.exec())
		{

		}	
}

void win_main::documents_update_netlist()
{
        win_report report(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
	
	if (tool_avviato) 
		GV->tool_stop();

        GV->ur.beginMacro(documents_update_netlistAct->text());
	GV->setUpdatesEnabled(FALSE);
	GV->select_clear();
	GV->drag_or_move_setup('R');

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1 - Trovo nuovamente tutti gli idstamp_pointer
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	idstamp_refresh_pointer();

	//////////////////////////////////////////////////////////////////////////////
	// 2 - Elimino i footprint orfani e quelli per cui il path relativo è cambiato	
	//////////////////////////////////////////////////////////////////////////////
	foreach(myComponent *pcb_comp, GV->all_comp)
		if (pcb_comp->idstamp > 0)
                        {
                        if (pcb_comp->idstamp_pointer == 0)
				pcb_comp->set_selected(TRUE);
                        else if (pcb_comp->footprint_link != ((myComponent *) pcb_comp->idstamp_pointer)->footprint_link)
                                {
                                pcb_comp->set_selected(TRUE);
                                ((myComponent *) pcb_comp->idstamp_pointer)->idstamp_pointer = 0;   //Azzero perchè dopo lo cancello
                                }
                        }
	
	if (GV->selected_comp.count() > 0)
		{
		foreach(myComponent *comp, GV->selected_comp) 
                        report.insert_row(QObject::tr("Removed %1.").arg(comp->ident), 'W');
                GV->selection_nodes();
                disconnect_pcb_selection();
		GV->ur.push(new delete_selection());		
		}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 3 - aggiorno nei fooprint il name ed il value presi dallo schematico (potrebbero essere stati modificati)
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	foreach(myComponent *pcb_comp, GV->all_comp)
		if (pcb_comp->idstamp > 0)
			{
			myComponent *sch_comp = (myComponent *) pcb_comp->idstamp_pointer;		
                        if ((pcb_comp->ident != sch_comp->ident) || (pcb_comp->value != sch_comp->value))
                                GV->ur.push(new change_component_ident_and_value(pcb_comp, sch_comp->ident, sch_comp->value));
                        }
	
	/////////////////////////////////////////////////////////////////////
	// 4 - Trovo il punto di piazzamento prima di piazzare qualsiasi cosa
	/////////////////////////////////////////////////////////////////////
	QRect all_rect = QRect(0,0,0,0);
	foreach(myGraphicsItem *obj, GV->scene->all_obj)
		all_rect = all_rect.unite(obj->boundingRect());
        QPoint place_point = GV->grid_point(all_rect.topLeft());
	
	//////////////////////////////////
	// 5 - Inserisco i nuovi footprint
	//////////////////////////////////
	QList<myComponent *>	footprint_to_place;
	for (int i = 0; i < doc_sch.count(); i++)
		{
		myGraphicsView *GVi = (myGraphicsView *) doc_sch.at(i);
		foreach(myComponent *comp, GVi->all_comp)
			{
			if (comp->idstamp_pointer == 0)
                                {
                                if (comp->footprint_link.isEmpty())
                                        report.insert_row(QObject::tr("Component %1 has no footprint.").arg(comp->ident), 'W');
                                else
                                        {
                                        QFileInfo fi(directory_pff + "/" + comp->footprint_link);
                                        if (fi.isFile())
                                                {
                                                if (comp_preview->load(directory_pff + "/" + comp->footprint_link, file_type, FALSE))
                                                        {
                                                        //Controllo che il footprint sia compatibile
                                                        if (comp->footprint_compatibility(preview_GV->all_comp.first()))
                                                                {
                                                                footprint_to_place.append(preview_GV->all_comp.first());
                                                                GV->ur.push(new add_footprint(comp));
                                                                report.insert_row(QObject::tr("Added %1.").arg(comp->ident), 'W');
                                                                }
                                                        else
                                                                report.insert_row(QObject::tr("Component %1 has a wrong footprint: pad numbers incompatible.").arg(comp->ident), 'E');
                                                        }
                                                }
                                        else
                                                report.insert_row(QObject::tr("Footprint of component %1 not found.").arg(comp->ident), 'E');
                                        }
                                }
			}	
		}

	///////////////////////////////
	// 6 - Piazzo i nuovi footprint
	///////////////////////////////
	// Ordino innanzitutto i componenti nella lista in base alla loro altezza (dal più piccolo al più grande) con il Bubble Sort
	int h = footprint_to_place.count();
	while (h > 0)
		{
		for (int i = 0; i < (h - 1); i++) 	
			{
			if (footprint_to_place.at(i)->bounding_rect().height() < footprint_to_place.at(i + 1)->bounding_rect().height())
				footprint_to_place.swap(i, i+1);
			}
		h--;
		}
	//Dispongo i componenti in righe come quelle di un editor di testo larghe al massimo 4000 thou ossia all'incirca 10cm
	int row_length = 0;
	int row_height = 0;
	int column = 0;
	while (footprint_to_place.count() > 0)
		{
		myComponent *footprint = footprint_to_place.takeFirst();
		QRect br = footprint->bounding_rect();
		if (row_length > 400000)
			{
			row_length = 0;
			column -= row_height + 5000;	//Tengo un margine di 50 thou ossia la griglia di default
			row_height = br.bottom();
			}
		
		
                QPoint	move = place_point + GV->grid_point(QPoint(row_length - br.left(),column - br.bottom()));
                move = move / pcb_grid_size;
                move = move * pcb_grid_size;
		//Muovo il footprint
		footprint->anchor_point += move;
		foreach(myGraphicsItem *obj, footprint->all_obj)
			{
			obj->start += move;
			obj->end += move;
			if (obj->node_start)
				obj->node_start->point += move;
			if (obj->node_end)
				obj->node_end->point += move;
			for (int i = 0; i < obj->polygon.count(); i++)
				obj->polygon[i] += move;		
			obj->info_update();
			}

		row_length += br.width() + 5000;	//Tengo un margine di 50 thou ossia la griglia di default
		if (row_height <  br.height())
			row_height = br.height();
		}

	///////////////////////////////
	// 7 - Ricalcolo i net di tutti gli schematici
	///////////////////////////////
        net_id_update_all_sch();
        net_id_update_pads_from_pins();
        net_id_update_pcb();

        ///////////////////////////////
        // 8 - Controllo la collisione tra tutti gli oggetti
	///////////////////////////////
        QList<myGraphicsItem *> *ao = &GS->all_obj;
        int ao_size = ao->size();
	for (int i = 0; i < ao_size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
		{
		myGraphicsItem	*obj = ao->at(i);
                if (obj->net_id < 0)
                        {
                        if (!obj->comp)
                              obj->set_selected(TRUE);
                        else
                              {
                              myComponent *comp = (myComponent *) obj->comp;
                              comp->set_selected(TRUE);
                              }
                        }
		}

        if (GV->selected_obj.count())
                report.insert_row(QObject::tr("DRC error. Removed %1 object(s).").arg(GV->selected_obj.count()), 'W');
        if (GV->selected_comp.count())
                {
                //I componenti non li cancello, ma li segnalo solamente
                foreach(myComponent *comp, GV->selected_comp)
                    report.insert_row(QObject::tr("DRC error. %1 collision.").arg(comp->ident), 'W');
                foreach(myComponent *comp, GV->all_comp)
                    comp->set_selected(FALSE);;
                }
        GV->selection_nodes();
        disconnect_pcb_selection();
        GV->ur.push(new delete_selection());
	
        ///////////////////////////////
        // 9 - Ricalcolo tutta la ratsnest
	///////////////////////////////
        ratsnest_update();

        GV->zoom_to_all();
	GV->setUpdatesEnabled(TRUE);
	GV->ur.endMacro();
        report.finalize();
        report.exec();
}

void win_main::actions_zoom()
{
	GV->tool_change(T_ZOOM);
}

void win_main::actions_zoom_to_all()
{
	GV->zoom_to_all();
}  

bool win_main::ask_if_save()
{
	if (file_type != FT_NULL)
		GV->tool_stop();
	
	if (!isWindowModified())
		return TRUE;

	QString question;
	if (file_type == FT_PROJECT)
		question = tr("The project %1 has been modified. Do you want to save it now?").arg(file_name);
	else 
		question = tr("The symbol %1 has been modified. Do you want to save it now?").arg(file_name);
	switch (QMessageBox::warning(this, "Itecad", question, tr("&Yes"), tr("&No"), tr("&Cancel"), 0, 1))
	{
	case 0:		// 0 = YES
		if (never_saved)
			{
			//Copia adattata della routine file_save_as()
			GV->tool_stop();

			QString file;
			switch (file_type)
			{
			case FT_PROJECT:
                                file = save_prj(this, directory_prj);
				break;
			case FT_SCH_SYMBOL:
				file = save_ssf(this, file_path);
				break;
			case FT_PCB_SYMBOL:
				file = save_pff(this, file_path);
				break;		
			}

			if (!file.isEmpty())
				{
				file_s(file);
				return TRUE;
				}
			return FALSE;
			}
		else
			{
			file_save();
			return TRUE;
			}
	case 1:		// 1 = NO
		return TRUE;
	case 2:		// 2 = CANCEL
		return FALSE;
	}		
}

void	win_main::alphabet_load()
{
        QDir dir(directory_ivf);
        dir.setFilter(QDir::Files);
        QFileInfoList list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i)
                {
                QFileInfo fileInfo = list.at(i);
                char_symbol *cs = new char_symbol();
                bool ok;
                cs->unicode = fileInfo.completeBaseName().toInt(&ok, 16);
                if (char_open(fileInfo.absoluteFilePath(), cs, FALSE))
                        alphabet.insert(cs->unicode, cs);
                else
                        delete cs;
                }
}

bool	win_main::char_open(QString file, char_symbol *cs, bool warning)
{
	QFile	f(file);
	if (!f.open(QFile::ReadOnly)) 
		{
		if (warning)
			QMessageBox::warning(this, "Itecad", QObject::tr("Cannot open file %1:\n%2.").arg(f.fileName()).arg(f.errorString()));
		return FALSE;
		}	
	cs->char_lines.clear();
	cs->char_points.clear();
	QTextStream out;
	out.setDevice(&f);
	out.setCodec("UTF-8");	
	
	QString riga = out.readLine();
		
	cs->width = riga.left(2).toInt();
	riga = out.readLine();
	
	while (!riga.isNull())
		{
		int x1, x2, y1, y2; //Definisco variabili int perchè cl.x1 cl.x2 cl.y1 cl.y2 sono di tipo char e non vanno bene
		x1 = riga.section(';',0,0).toInt();
		y1 = -riga.section(';',1,1).toInt();
		x2 = riga.section(';',2,2).toInt();
		y2 = -riga.section(';',3,3).toInt();
		
		if ((x1 == x2) && (y1 == y2))
			{
			char_point cp;
			cp.x = x1;
			cp.y = y1;
			cs->char_points.append(cp);
			}
		else
			{
			char_line cl;
			cl.x1 = x1;
			cl.x2 = x2;
			cl.y1 = y1;
			cl.y2 = y2;
			cs->char_lines.append(cl);
			}
		riga = out.readLine();
		}

	return TRUE;
}

bool	win_main::char_save(QString file, char_symbol *cs)
{
	QFile	f(file);
	if (!f.open(QFile::WriteOnly)) 
		{
		QMessageBox::warning(this, "Itecad", QObject::tr("Cannot write file %1:\n%2.").arg(f.fileName()).arg(f.errorString()));
		return FALSE;
		}	
	QTextStream out;

	//Salvo il carattere
	out.setDevice(&f);
	out.setCodec("UTF-8");	
	out << (int) cs->width << '\n';	//Salvo la larghezza del carattere
	foreach (char_point cp, cs->char_points)
		out << (int) (cp.x) << ';' << (int) (-cp.y) << ';' << (int) (cp.x) << ';' << (int) (-cp.y) << '\n';
	foreach (char_line cl, cs->char_lines)
		out << (int) (cl.x1) << ';' << (int) (-cl.y1) << ';' << (int) (cl.x2) << ';' << (int) (-cl.y2) << '\n';
	return TRUE;
}

void win_main::closeEvent(QCloseEvent *e)
{
    if (ask_if_save())
        e->accept();
    else
        e->ignore();
}

QString	win_main::documents_new_name(char type)
{
	QString name, stringtype;
	switch (type)
	{
	case DT_SCH:
		stringtype = tr("Schematic");
		break;
	case DT_PCB:
		stringtype = tr("Board");
		break;
	case DT_TXT:
		stringtype = tr("Info");
		break;
	}
	if (tw == 0)
		return stringtype + " 1";
	
	bool	valid_name;
	int		num = 0;
	do
		{
		num++;
		valid_name = TRUE;
		name = stringtype + " " + QString::number(num);
		for (int i = 0; i < doc_sch_and_pcb.count(); i++)
			if (doc_sch_and_pcb.at(i)->name == name)
				valid_name = FALSE;
		for (int i = 0; i < doc_txt.count(); i++)
			if (doc_txt.at(i)->objectName() == name)
				valid_name = FALSE;
		}
	while (!valid_name);
	return name;
}

bool	win_main::documents_valid_name(QString name, int rename_row)
{
	//Controllo se il nome e sia già presente
	for (int i = 0; i < doc_sch_and_pcb.count(); i++)
		if (i != rename_row)
			if (doc_sch_and_pcb.at(i)->name == name)
				{
				QMessageBox::warning(this, tr("Error"), tr("This name is already used in another document"));
				return FALSE;
				}
	for (int i = 0; i < doc_txt.count(); i++)
		if (i != rename_row)
			if (doc_txt.at(i)->objectName() == name)
				{
				QMessageBox::warning(this, tr("Error"), tr("This name is already used in another document"));
				return FALSE;
				}
	return TRUE;
}

void	win_main::documents_add_new(char type, QString name)
{
	if (file_type == FT_PROJECT)
		{
		if (tw == 0)
			{
			//Definisco innanzitutto l'oggetto centrale
			tw = new QTabWidget(this);
			tw->setIconSize(QSize(24,22));
                        connect(tw,SIGNAL(currentChanged(int)),this,SLOT(change_document(int)));
			tw->setUpdatesEnabled(FALSE);
			setCentralWidget(tw);
			}
		switch (type)
		{
		case DT_SCH:
			{
			tool_sch = T_SELECT;
			GS = new myGraphicsScene();
			GV = new myGraphicsView(GS, this, TRUE, name);		
			tw->addTab(GV, QIcon(":/Resources/schematic.png"), name);	
			tw->setVisible(TRUE);
			doc_sch.append(GV);
			doc_sch_and_pcb.append(GV);
			tw->setCurrentIndex(tw->count() - 1);
			GV->zoom_to_default();
			}
			break;
		case DT_PCB:
			{
			tool_pcb = T_SELECT;
			GS = new myGraphicsScene();
			GV = new myGraphicsView(GS, this, FALSE, name);		
			tw->addTab(GV, QIcon(":/Resources/board.png"), name);	
			tw->setVisible(TRUE);
			doc_pcb = GV;
			doc_sch_and_pcb.append(GV);
			tw->setCurrentIndex(tw->count() - 1);
			GV->zoom_to_default();
			}
			break;
		case DT_TXT:
			{
			TE = new QTextEdit(this);	
			QFont font("Arial", 12, QFont::Normal);	//Uso questo font di default
			TE->setCurrentFont(font);	//servono tutti e due
			TE->setFont(font);			//Servono tutti e due
		//	TE->setAcceptRichText(FALSE);
			TE->setContextMenuPolicy(Qt::CustomContextMenu);
			connect(TE, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(TE_popup_menu(const QPoint &)));
                        connect(TE, SIGNAL(undoAvailable(bool)), this, SLOT(edit_can_undo_changed(bool)));
                        connect(TE, SIGNAL(redoAvailable(bool)), this, SLOT(edit_can_redo_changed(bool)));
			TE->setObjectName(name);
			tw->addTab(TE, QIcon(":/Resources/script.png"), name);	
			tw->setVisible(TRUE);
			doc_txt.append(TE);
			tw->setCurrentIndex(tw->count() - 1);
			}
			break;
		}		
		}
	else
		{
		switch (type)
		{
		case DT_SCH:
			{
			tool_sch = T_SELECT;
			GS = new myGraphicsScene();
			GV = new myGraphicsView(GS, this, TRUE, name);	
			doc_sch.append(GV);
			doc_sch_and_pcb.append(GV);
			}
			break;
		case DT_PCB:
			{
			tool_pcb = T_SELECT;
			GS = new myGraphicsScene();
			GV = new myGraphicsView(GS, this, FALSE, name);	
			doc_pcb = GV;
			doc_sch_and_pcb.append(GV);
			}
			break;
		}
		GV->setUpdatesEnabled(FALSE);			
		setCentralWidget(GV);
		GV->setVisible(TRUE);
		GV->zoom_to_default();
		}
}

void win_main::documents_remove(int index)
{
	myGraphicsView	*old_GV;
	myGraphicsScene *old_GS;
	QTextEdit		*old_TE;

	//tw->setUpdatesEnabled(FALSE);	
	switch (doc_type(tw->widget(index)))
	{
	case DT_SCH:
	case DT_PCB:
		old_GV = GV;
		old_GS = GS;
                tw->removeTab(index);		//Quando eseguo questo QT esegue automaticamente change_document()
                //change_document(tw->currentIndex());
		delete old_GS;	//Conviene prima eliminare GS e poi GV
		delete old_GV;
		doc_sch.removeAll(GV);
		doc_sch_and_pcb.removeAll(GV);
		if (doc_pcb == GV)
			doc_pcb = 0;
		break;
	case DT_TXT: 
		old_TE = TE;
                tw->removeTab(index);		//Quando eseguo questo QT esegue automaticamente change_document()
                //change_document(tw->currentIndex()); è inutile
		delete old_TE;
		doc_txt.removeAll(TE);
		break;
	}	
	//tw->setUpdatesEnabled(TRUE);	
}

void win_main::documents_clean_up()
{
	GV->setUpdatesEnabled(FALSE);
        clean_up_pcb_all();
	GV->setUpdatesEnabled(TRUE);
}

void win_main::documents_component_properties()
{
	win_component_prop dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, GV->schcomp_or_pcbcomp, TRUE);
	switch (dialogo.exec())
	{
	case QDialog::Accepted:
		if (dialogo.is_modified())
			{
			GV->setUpdatesEnabled(FALSE);
			GV->ur.push(new change_component_prop((int) &dialogo));
			GV->setUpdatesEnabled(TRUE);
			}
		break;
	case QDialog::Rejected:
		break;
        }


}

void win_main::documents_colours()
{
        GV->tool_stop();

        char tab_index;

        if (GV->is_sch)
                tab_index = 3;
        else
                tab_index = 7;
        win_settings dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, tab_index);
        if (dialogo.exec())
                {
                }
}

void win_main::documents_workspace()
{
        GV->tool_stop();

        char tab_index;

        if (GV->is_sch)
                tab_index = 4;
        else
                tab_index = 8;
        win_settings dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, tab_index);
        if (dialogo.exec())
                {
                }
}

void win_main::documents_layers()
{
        GV->tool_stop();

        win_edit_layers dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
        if (dialogo.exec())
                {
                }
}

void win_main::documents_artistic_grid()
{
        GV->tool_stop();

        win_settings dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, 2);
        if (dialogo.exec())
                {
                }
}

void win_main::documents_grid()
{
	GV->tool_stop();
	
        win_settings dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, 6);
        if (dialogo.exec())
		{
		}	
}

void win_main::documents_style()
{
	GV->tool_stop();
	
        char tab_index;

        if (GV->is_sch)
                tab_index = 1;
        else
                tab_index = 5;
        win_settings dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, tab_index);
        if (dialogo.exec())
		{
		}	
}

void win_main::documents_page()
{
	GV->tool_stop();
	
        win_page dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);

        if (dialogo.exec())
		{
		}	
}

char	win_main::doc_type()
{
	switch (file_type)
	{
	case FT_PROJECT:
		return doc_type(tw->currentWidget());
		break;
	case FT_SCH_SYMBOL:
		return DT_SCH;
		break;
	case FT_PCB_SYMBOL:
		return DT_PCB;
		break;
	}
}

char	win_main::doc_type(QWidget *doc)
{
	if (doc_sch.contains((myGraphicsView *) doc))
		return DT_SCH;
	if (doc_pcb == (myGraphicsView *) doc)
		return DT_PCB;
	if (doc_txt.contains((QTextEdit *)doc))
		return DT_TXT;
}

void win_main::edit_copy()
{
	if (doc_type() == DT_TXT)
		TE->copy();
        else
                {
                if (!no_ident_or_value())
                        {
                        QMessageBox::warning(this, QObject::tr("Error"), QObject::tr("Can't copy Ident or Value"));
                        return;
                        }

                GV->tool_stop();
                actions_selectAct->setChecked(TRUE);
                GV->tool_change(T_SELECT);

                GV->clone_from_selection();
                GV->clone_to_clipboard();
                clipboard_doc_type = doc_type();
                clipboard_file_type = file_type;
                cloned_action = 'P';
                change_selection(); //Serve per abilitarmi subito l'azione "Paste" nei menu
                }
}

void win_main::edit_cut()
{
        if (doc_type() == DT_TXT)
                TE->cut();
        else
                {
                if (!no_ident_or_value())
                        {
                        QMessageBox::warning(this, QObject::tr("Error"), QObject::tr("Can't cut Ident or Value"));
                        return;
                        }

                GV->tool_stop();
                actions_selectAct->setChecked(TRUE);
                GV->tool_change(T_SELECT);

                GV->clone_from_selection();
                GV->clone_to_clipboard();
                clipboard_doc_type = doc_type();
                clipboard_file_type = file_type;
                cloned_action = 'P';

                GV->setUpdatesEnabled(FALSE);
                GV->ur.beginMacro(tr("Cut"));
                if (GV->is_sch)
                        disconnect_sch_selection();
                else
                        disconnect_pcb_selection();
                GV->ur.push(new delete_selection());
                GV->ur.push(new clipboard_clear());
                GV->ur.endMacro();
                GV->setUpdatesEnabled(TRUE);
                }
}

void win_main::edit_paste()
{
        if (doc_type() == DT_TXT)
                TE->paste();
        else
                {
                GV->tool_stop();

                GV->tool_change(T_SELECT);
                GV->tool = T_ADD;
                tool_avviato = TRUE;

                GV->setUpdatesEnabled(FALSE);
                GV->select_clear();
                GV->drag_or_move_setup('R');
                GV->clone_from_clipboard();
                GV->clone_ident_pin_pad_recalculate();
                mouse_move = GV->mapToScene(GV->mapFromGlobal(QCursor::pos())).toPoint();
                GV->clone_move(GV->grid_delta(-GV->clone_center()), TRUE);
                GV->clone_move(GV->grid_delta(mouse_move), TRUE);
                GV->setUpdatesEnabled(TRUE);
                }
}

void win_main::edit_delete()
{
        if (!no_ident_or_value())
                {
                QMessageBox::warning(this, QObject::tr("Error"), QObject::tr("Can't delete Ident or Value"));
                return;
                }

        GV->tool_stop();

	GV->setUpdatesEnabled(FALSE);
	GV->ur.beginMacro(tr("Delete"));
        if (GV->is_sch)
                disconnect_sch_selection();
        else
                disconnect_pcb_selection();
        GV->ur.push(new delete_selection());
	GV->ur.endMacro();
	highlight_active = FALSE;	//Serve se si cancella un wire mentre lo si sta immettendo
	GV->setUpdatesEnabled(TRUE);
} 

void win_main::edit_duplicate()
{
        if (!no_ident_or_value())
                {
                QMessageBox::warning(this, QObject::tr("Error"), QObject::tr("Can't duplicate Ident or Value"));
                return;
                }

        GV->tool_stop();
        actions_selectAct->setChecked(TRUE);
        GV->tool_change(T_SELECT);
        GV->tool = T_ADD;
        tool_avviato = TRUE;

        GV->setUpdatesEnabled(FALSE);
        GV->clone_from_selection();
        GV->clone_ident_pin_pad_recalculate();
        GV->select_clear();
        GV->drag_or_move_setup('R');
        mouse_move = GV->mapToScene(GV->mapFromGlobal(QCursor::pos())).toPoint();
        GV->clone_move(GV->grid_delta(-GV->clone_center()), TRUE);
        GV->clone_move(GV->grid_delta(mouse_move), TRUE);
        cloned_action = 'D';
        GV->setUpdatesEnabled(TRUE);
}

void win_main::edit_select_all()
{
        GV->setUpdatesEnabled(FALSE);
        GV->select_all();
        GV->setUpdatesEnabled(TRUE);
}

void win_main::editor_find()
{
        if (!modeless_win_find_text)
                modeless_win_find_text = new win_find_text(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint, TE);
        modeless_win_find_text->show();
        modeless_win_find_text->raise();
        modeless_win_find_text->activateWindow();
}

void win_main::edit_find()
{
        GV->tool_stop();

        win_find_comp dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
        if (dialogo.exec())
                {
                }
        GV->setFocus();
} 

void win_main::edit_properties()
{
        if ((GV->selected_comp.count() == 0) && (GV->selected_obj.count() == 1))
		{
                myGraphicsItem *obj = GV->selected_obj.first();

                win_object_prop dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, obj);
		switch (dialogo.exec())
		{
		case QDialog::Accepted:
			if (dialogo.is_modified())
				{
                                GV->setUpdatesEnabled(FALSE);
                                GV->ur.beginMacro(tr("Change Object Properties"));
                                GV->ur.push(new change_obj_prop(obj, (int) &dialogo));
                                GV->ur.endMacro();
                                GV->setUpdatesEnabled(TRUE);
				}
			break;
		case QDialog::Rejected:
			break;
		}
		}
        else if ((GV->selected_comp.count() == 1) && (GV->selected_obj.count() == 0))
		{
		myComponent *comp = GV->selected_comp.first();
		
		win_component_prop dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, comp, FALSE);
                switch (dialogo.exec())
		{
		case QDialog::Accepted:
                        if (dialogo.is_modified())
				{
				GV->ur.push(new change_component_prop((int) &dialogo));
				
				//Problema: quando cambio il footprint_link il componente mi sparisce.
				//Aggiungendo qui il comando GV->select_all(FALSE); il tutto sembra funzionare. Why??
				
				GV->setUpdatesEnabled(TRUE);
				}
			break;
		case QDialog::Rejected:
                        break;
		}
                }
	else
		return;	//Nel caso in cui mi attivo tramite una shortkey e non c'è la giusta selezione allora non faccio niente
}

void win_main::edit_undo()
{
	if (doc_type() == DT_TXT)
		TE->undo();
	else
		{	
		GV->tool_stop();

		GV->setUpdatesEnabled(FALSE);
                GV->select_clear();
		GV->ur.undo();	
		GV->setUpdatesEnabled(TRUE);
		}
}

void win_main::edit_redo()
{
	if (doc_type() == DT_TXT)
		TE->redo();
	else
		{	
		GV->tool_stop();

		GV->setUpdatesEnabled(FALSE);
                GV->select_clear();
		GV->ur.redo();
		GV->setUpdatesEnabled(TRUE);
		}
}

void win_main::edit_undo_redo_history()
{
	GV->tool_stop();
	
	win_undo_redo_history dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
	if (dialogo.exec())
		{

		}	
}

void win_main::editor_style()
{
	if (!modeless_win_style)
		modeless_win_style = new win_style(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);
	modeless_win_style->show();
    modeless_win_style->raise();
    modeless_win_style->activateWindow();	
}

void win_main::edit_can_undo_changed(bool new_value)
{
        QString name;
	switch (doc_type())
	{
	case DT_SCH:
	case DT_PCB:
                edit_undoAct->setEnabled(new_value);
		name = GV->name;
                edit_undoAct->setText(tr("Undo") + " " + GV->ur.undoText());
		break;
	case DT_TXT:
                edit_undoAct->setEnabled(TE->document()->isUndoAvailable());
		name = TE->objectName();
		break;
	}
	
        if (file_type == FT_PROJECT)
		{
                if (new_value)
			{
			tw->setTabText(tw->currentIndex(), name + "*");
			setWindowModified(TRUE);
                        }
		else
			{
			tw->setTabText(tw->currentIndex(), name);
			//Controllo tutte le tab page 
			for (int i = 0; i < tw->count(); i++)
				{
				if (tw->tabText(i).right(1) == "*")
					{
					setWindowModified(TRUE);
					return;
					}
				setWindowModified(FALSE);
				}
                        }
		}
	else
		{
		if (new_value)
			setWindowModified(TRUE);
		else
			setWindowModified(FALSE);
                }
}

void win_main::edit_can_redo_changed(bool new_value)
{
	switch (doc_type())
	{
	case DT_SCH:
	case DT_PCB:
		edit_redoAct->setEnabled(GV->ur.canRedo());
                edit_redoAct->setText(tr("Redo") + " " + GV->ur.redoText());
		break;
	case DT_TXT:
		edit_redoAct->setEnabled(TE->document()->isRedoAvailable());
		break;
	}
}

void win_main::edit_change_side()
{
        GV->setUpdatesEnabled(FALSE);
	GV->ur.beginMacro(edit_change_sideAct->text());
        disconnect_pcb_selection();
        foreach (myComponent *comp, GV->selected_comp)
		GV->ur.push(new change_footprint_side(comp));
	GV->ur.endMacro();
	GV->setUpdatesEnabled(TRUE);
}

void win_main::actions_mirror_h()
{
        QPoint center = GV->rotation_point();
	GV->selection_nodes();

	GV->setUpdatesEnabled(FALSE);
	GV->ur.beginMacro(actions_mirror_hAct->text());
        if (GV->is_sch)
                disconnect_sch_selection();
        else
                disconnect_pcb_selection();
        foreach (node_data *node, GV->selected_nodes)
		GV->ur.push(new mirror_node(center, node, TRUE));
	foreach (myGraphicsItem *obj, GV->selected_obj)
		GV->ur.push(new mirror_obj(center, obj, TRUE));
	foreach (myComponent *comp, GV->selected_comp) 
		GV->ur.push(new mirror_comp(center, comp, TRUE));
        if (GV->is_sch)
                connect_sch_selection();
	GV->ur.endMacro();
	GV->setUpdatesEnabled(TRUE);
} 

void win_main::actions_mirror_v()
{
        QPoint center = GV->rotation_point();
	GV->selection_nodes();

	GV->setUpdatesEnabled(FALSE);
	GV->ur.beginMacro(actions_mirror_vAct->text());
        if (GV->is_sch)
                disconnect_sch_selection();
        else
                disconnect_pcb_selection();
        foreach (node_data *node, GV->selected_nodes)
		GV->ur.push(new mirror_node(center, node, FALSE));
	foreach (myGraphicsItem *obj, GV->selected_obj)
		GV->ur.push(new mirror_obj(center, obj, FALSE));
	foreach (myComponent *comp, GV->selected_comp) 
		GV->ur.push(new mirror_comp(center, comp, FALSE));
        if (GV->is_sch)
                connect_sch_selection();
	GV->ur.endMacro();
	GV->setUpdatesEnabled(TRUE);
} 

void win_main::actions_wire_width()
{
        myGraphicsItem *obj = GV->selected_obj.first();

        win_object_prop dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, obj);
        switch (dialogo.exec())
        {
        case QDialog::Accepted:
                if (dialogo.is_modified() || (GV->selected_obj.count() > 1))
                        {
                        GV->setUpdatesEnabled(FALSE);
                        GV->ur.beginMacro(actions_wire_widthAct->text());
                        qreal new_pen_width = dialogo.new_pen_width;
                        foreach (myGraphicsItem *obj, GV->selected_obj)
                                GV->ur.push(new change_wire_width(obj, new_pen_width));
                        GV->ur.endMacro();
                        GV->setUpdatesEnabled(TRUE);
                        }
                break;
        case QDialog::Rejected:
                break;
        }

}

void win_main::edit_select_t()
{
        //Seleziono tutta la track
        GV->setUpdatesEnabled(FALSE);
        track_select(GV->selected_obj.first(), FALSE);
        GV->do_selection_rect_recalculate = TRUE;
        GV->setUpdatesEnabled(TRUE);
}

void win_main::edit_select_c()
{
        //Seleziono tutta la track
        GV->setUpdatesEnabled(FALSE);
        track_select(GV->selected_obj.first(), TRUE);
        GV->do_selection_rect_recalculate = TRUE;
        GV->setUpdatesEnabled(TRUE);
}

void win_main::edit_select_n()
{
        net_id_update_pcb();

        if (GV->selected_obj.first()->node_start->net_id > 0)
                {
                GV->setUpdatesEnabled(FALSE);
                foreach (myGraphicsItem *obj, GV->scene->all_obj)
                        if (obj->tipo == O_CONNECTION)
                                if (obj->net_id == GV->selected_obj.first()->net_id)
                                        obj->set_selected(TRUE);
                GV->do_selection_rect_recalculate = TRUE;
                GV->setUpdatesEnabled(TRUE);
                }
}

void win_main::actions_move_to()
{
	bool	show_absolute = FALSE;
	int		absolute_x = 0;
	int		absolute_y = 0;
	
        if (GV->selected_comp.count() == 1)
		if (GV->selected_obj.count() == 0)
			{	
			show_absolute = TRUE;
			absolute_x = GV->selected_comp.first()->anchor_point.x();
			absolute_y = GV->selected_comp.first()->anchor_point.y();
                        if (y_axis_is_up)
                                absolute_y = -absolute_y;
                        }

	win_move_to dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, show_absolute, absolute_x, absolute_y);
	if (dialogo.exec())
                {
                bool    relative;
                if (show_absolute)
                        relative = dialogo.is_relative->isChecked();
                else
                        relative = TRUE;

                int x_move = dialogo.pos_x->length;
                int y_move = dialogo.pos_y->length;
                if (y_axis_is_up)
                        {
                        absolute_y = -absolute_y;
                        y_move = -y_move;
                        }

                if (relative)
			{
                        drag_or_move_delta.setX(x_move);
                        drag_or_move_delta.setY(y_move);
                        }
		else
			{
                        drag_or_move_delta.setX(x_move - absolute_x);
                        drag_or_move_delta.setY(y_move - absolute_y);
			}
		if (!drag_or_move_delta.isNull())
			{
			if (mode_drag_active)
				{
				GV->drag_or_move_setup('D');	
				GV->drag_or_move_execute('D');	
				}
			else
				{		
				GV->drag_or_move_setup('M');	
				GV->drag_or_move_execute('M');	
				}
			GV->drag_or_move_setup('R');
			}
		}
} 

void win_main::actions_rotate_r()
{
        QPoint center = GV->rotation_point();
	GV->selection_nodes();

	GV->setUpdatesEnabled(FALSE);
	GV->ur.beginMacro(actions_rotate_rAct->text());

        foreach (myGraphicsItem *obj, GV->selected_obj)
                if (obj->tipo == O_PIN)
                        qDebug() << "PIN";

        if (GV->is_sch)
                disconnect_sch_selection();
        else
                disconnect_pcb_selection();

        foreach (myGraphicsItem *obj, GV->selected_obj)
                if (obj->tipo == O_PIN)
                        qDebug() << "PIN2";

        foreach (node_data *node, GV->selected_nodes)
		GV->ur.push(new rotate_node(center, node, TRUE));
	foreach (myGraphicsItem *obj, GV->selected_obj) 
		GV->ur.push(new rotate_obj(center, obj, TRUE));
	foreach (myComponent *comp, GV->selected_comp) 
		GV->ur.push(new rotate_comp(center, comp, TRUE));
        if (GV->is_sch)
                connect_sch_selection();
	GV->ur.endMacro();
	GV->setUpdatesEnabled(TRUE);
} 

void win_main::actions_rotate_l()
{
        QPoint center = GV->rotation_point();
	GV->selection_nodes();

	GV->setUpdatesEnabled(FALSE);
	GV->ur.beginMacro(actions_rotate_lAct->text());
        if (GV->is_sch)
                disconnect_sch_selection();
        else
                disconnect_pcb_selection();
        foreach (node_data *node, GV->selected_nodes)
		GV->ur.push(new rotate_node(center, node, FALSE));
	foreach (myGraphicsItem *obj, GV->selected_obj)
		GV->ur.push(new rotate_obj(center, obj, FALSE));
	foreach (myComponent *comp, GV->selected_comp) 
		GV->ur.push(new rotate_comp(center, comp, FALSE));
        if (GV->is_sch)
                connect_sch_selection();
	GV->ur.endMacro();
	GV->setUpdatesEnabled(TRUE);
}

void win_main::file_c()
{
        switch (file_type)	//Conviene usare switch perchè devo prevedere FT_NULL e non fare niente
	{
	case FT_PROJECT:
		tw->setVisible(FALSE);	//Velocizza e serve
		for (int i = tw->count() - 1; i >= 0; i--)
			{
			switch (doc_type(tw->widget(i)))
			{
			case DT_SCH:
                        case DT_PCB:
				//Elimino dalla memoria gli oggetti memorizzati dentro al tab
				GV = (myGraphicsView *) tw->widget(i);
				GS = GV->scene;
                                delete GS;	//Conviene prima eliminare GS e poi GV
                                delete GV;
                                break;
			case DT_TXT:
				TE = (QTextEdit *) tw->widget(i);
				delete TE;
                                break;
			}
                        tw->removeTab(i);
			}
		delete tw;
		break;
	case FT_SCH_SYMBOL:
	case FT_PCB_SYMBOL:
		//Elimino dalla memoria GV e GS
		GV->setVisible(FALSE);	//Velocizza e serve
		delete GS;	//Conviene prima eliminare GS e poi GV
		delete GV;	
		break;
	case FT_NULL:
		//Conviene usare switch perchè devo prevedere FT_NULL e non fare niente
		break;
	}

        doc_sch.clear();
	doc_sch_and_pcb.clear();
	doc_pcb = 0;
        doc_txt.clear();

        change_file_type(FT_NULL);
}

void win_main::file_close()
{
	if (!ask_if_save())
		return;
    
	file_c();
}

void win_main::file_new_prj()
{
        if (!ask_if_save())
		return;

	file_c();

	file_name = tr("New Project") + ".ite";
	file_path = "";
        never_saved = TRUE;
        file_saveAct->setEnabled(!never_saved); //Riga che va eseguita ogni volta che si cambia never_saved
	setWindowTitle(file_name + "[*] - Itecad");
	
	change_file_type(FT_PROJECT);
	documents_add_new(DT_SCH, documents_new_name(DT_SCH));
        settings_generaldefault2temp();
        settings_temp2current();
	documents_add_new(DT_PCB, documents_new_name(DT_PCB));
        settings_generaldefault2temp();
        settings_temp2current();
	documents_add_new(DT_TXT, documents_new_name(DT_TXT));
        change_document(0);

	ratsnest.clear();
	
	tw->setUpdatesEnabled(TRUE);

	idstamp_last_used = 0;
}

void win_main::file_new_ssf()
{
	if (!ask_if_save())
		return;

	file_c();

	file_name = tr("New SCH Symbol") + ".ssf";
	file_path = "";
	never_saved = TRUE;
        file_saveAct->setEnabled(!never_saved); //Riga che va eseguita ogni volta che si cambia never_saved
	setWindowTitle(file_name + "[*] - Itecad");
	
	change_file_type(FT_SCH_SYMBOL);
	documents_add_new(DT_SCH, tr("Schematic Symbol"));
        settings_generaldefault2temp();
        settings_temp2current();
        change_document(0);

	//Aggiungo Ident e Value
        myGraphicsItem *ident = new myGraphicsItem(O_IDENT);
        ident->layer_from = L_SCHNormal;
        ident->layer_to = L_SCHNormal;
	ident->start = QPoint(0,0);
        ident->font_height = internal_font_height;
        ident->font_bold = internal_font_bold;
        ident->font_italic = internal_font_italic;
	ident->text_position = 'R';
        ident->info_update();
	GS->add_item(ident);	
        myGraphicsItem *value = new myGraphicsItem(O_VALUE);
        value->layer_from = L_SCHNormal;
        value->layer_to = L_SCHNormal;
	value->start = QPoint(0,10000);
        value->font_height = internal_font_height;
        value->font_bold = internal_font_bold;
        value->font_italic = internal_font_italic;
	value->text_position = 'R';
        value->info_update();
	GS->add_item(value);	

	GV->setUpdatesEnabled(TRUE);
}

void win_main::file_new_pff()
{
	if (!ask_if_save())
		return;

	file_c();

	file_name = tr("New PCB Footprint") + ".pff";
	file_path = "";
	never_saved = TRUE;
        file_saveAct->setEnabled(!never_saved); //Riga che va eseguita ogni volta che si cambia never_saved
	setWindowTitle(file_name + "[*] - Itecad");
	
	change_file_type(FT_PCB_SYMBOL);
	documents_add_new(DT_PCB, tr("PCB Footprint"));
        settings_generaldefault2temp();
        settings_temp2current();
        change_document(0);

	//Aggiungo Ident
        myGraphicsItem *ident = new myGraphicsItem(O_IDENT);
        ident->layer_from = L_PCBSilkTop;
        ident->layer_to = L_PCBSilkTop;
	ident->start = QPoint(0,0);
        ident->font_height = internal_font_height;
        ident->font_bold = internal_font_bold;
        ident->font_italic = internal_font_italic;
	ident->text_position = 'R';
        ident->info_update();
	GS->add_item(ident);	
	
	GV->setUpdatesEnabled(TRUE);
}

void win_main::file_o(QString &f_path)
{
	file_c();	//Chiudo prima tutto

	xml_sax_handler handler;
	if (handler.open_file(f_path, FALSE))
		{
                check_data_integrity();

		file_path = f_path;		
		file_name = QFileInfo(f_path).fileName();
		setWindowTitle(file_name + "[*] - Itecad");
		never_saved = FALSE;
                file_saveAct->setEnabled(!never_saved); //Riga che va eseguita ogni volta che si cambia never_saved
		
		recent_file_insert(file_path);

		if (file_type == FT_PROJECT)
			{
			tw->setUpdatesEnabled(TRUE);
			//Trovo idstamp_last_used
			idstamp_last_used = 0;	
			for (int i = 0; i < doc_sch_and_pcb.count(); i++)
				{
				myGraphicsView *GVi = (myGraphicsView *) doc_sch_and_pcb.at(i);
				foreach(myComponent *comp, GVi->all_comp)
					if (comp->idstamp > idstamp_last_used)
						idstamp_last_used = comp->idstamp;
				idstamp_refresh_pointer();
				}	
                        net_id_update_pcb();       //Serve perchè all'inizio non ho nessun net_id definito
                        }
		else
			{
			ratsnest.clear();
			GV->setUpdatesEnabled(TRUE);
			}
                }
}

void win_main::file_open_prj()
{
	if (!ask_if_save())
		return;

	QString file = open_prj(this);
	if (!file.isEmpty())
		file_o(file);
}

void win_main::file_open_ssf()
{
	if (!ask_if_save())
		return;

	QString file = open_ssf(this);
	if (!file.isEmpty())
		file_o(file);
}

void win_main::file_open_pff()
{
	if (!ask_if_save())
		return;

	QString file = open_pff(this);
	if (!file.isEmpty())
		file_o(file);
}

void win_main::file_print()
{
	if (doc_type() == DT_TXT)
		{
		QPrinter printer(QPrinter::HighResolution);
		printer.setFullPage(true);
		QPrintDialog *dlg = new QPrintDialog(&printer, this);
		if (dlg->exec() == QDialog::Accepted) 
			{
			TE->document()->print(&printer);
			}
		delete dlg;
		return;
		}
	
	GV->tool_stop();
	
	win_print dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, FALSE);
	if (dialogo.exec())
		{
		}
}

void win_main::file_print_to_pdf()
{
	if (doc_type() == DT_TXT)
		{
                QString fileName = save_pdf(this, "");
                if (fileName.isEmpty())
			return;
		QPrinter printer(QPrinter::HighResolution);
		printer.setOutputFormat(QPrinter::PdfFormat);
		printer.setOutputFileName(fileName);
		TE->document()->print(&printer);
		return;
		}
	
	GV->tool_stop();
	
	win_print dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, TRUE);
	if (dialogo.exec())
		{
		}
}

void win_main::file_recent_1()
{
	if (!ask_if_save())
		return;

	file_o(rf_path_1);	
}

void win_main::file_recent_2()
{
	if (!ask_if_save())
		return;
			
	file_o(rf_path_2);	
}

void win_main::file_recent_3()
{
	if (!ask_if_save())
		return;

	file_o(rf_path_3);	
}

void win_main::file_recent_4()
{
	if (!ask_if_save())
		return;
			
	file_o(rf_path_4);	
}

void win_main::file_s(QString &f_path)
{
        check_data_integrity();

        xml_sax_generator generator;
        if (generator.save_file(f_path, file_type, prog_version_string))
		{
		file_path = f_path;						//Solo adesso aggiorno il percorso
		file_name = QFileInfo(f_path).fileName();	//Solo adesso aggiorno il nome
		never_saved = FALSE;
                file_saveAct->setEnabled(!never_saved); //Riga che va eseguita ogni volta che si cambia never_saved
		setWindowTitle(file_name + "[*] - Itecad");
		recent_file_insert(file_path);

		//Azzero i vari Undo/Redo...
		if (file_type == FT_PROJECT)
			for (int i = 0; i < tw->count(); i++)
				switch (doc_type(tw->widget(i)))
				{
				case DT_SCH:
				case DT_PCB:
					{
					myGraphicsView *iGV = (myGraphicsView *) tw->widget(i);
					iGV->ur.clear();
					tw->setTabText(i, iGV->name);
					}
					break;
				case DT_TXT:
					{
					QTextEdit *iTE = (QTextEdit *) tw->widget(i);
					iTE->setUndoRedoEnabled(FALSE);
					iTE->setUndoRedoEnabled(TRUE);
					}
					break;
				}
		else
			GV->ur.clear();
		setWindowModified(FALSE);
                }
}

void win_main::file_save()
{
	if (never_saved)	//Se eseguo questo comando con una shortkey non faccio niente 
		return;

	GV->tool_stop();

	if (file_type != FT_PROJECT)
		if (!is_anchor_point_correct())
			return;

	file_s(file_path);	
}

void win_main::file_save_as()
{
	GV->tool_stop();

	if (file_type != FT_PROJECT)
		if (!is_anchor_point_correct())
			return;

	QString file;
	switch (file_type)
	{
	case FT_PROJECT:
		file = save_prj(this, file_path);
		break;
	case FT_SCH_SYMBOL:
		file = save_ssf(this, file_path);
		break;
	case FT_PCB_SYMBOL:
		file = save_pff(this, file_path);
		break;		
	}

	if (!file.isEmpty())
		file_s(file);
}

void win_main::help_user_manual()
{
	if (file_type != FT_NULL)
		GV->tool_stop();

        if (!help_user_manual_proc)
                help_user_manual_proc = new QProcess();

        if (help_user_manual_proc->state() != QProcess::Running)
                {
                QString app = QLibraryInfo::location(QLibraryInfo::BinariesPath) + QDir::separator();
                app += "assistant";

                QStringList args;
                args << "-collectionFile";
                args << QApplication::applicationDirPath() + QDir::separator() + "doc.qhc";

                help_user_manual_proc->start(app, args);

                if (!help_user_manual_proc->waitForStarted())
                        QMessageBox::critical(this, "Itecad", QObject::tr("Unable to launch User Manual (%1)").arg(app));
                }
}

void win_main::help_web()
{
	if (file_type != FT_NULL)
		GV->tool_stop();
	
        QDesktopServices::openUrl(QUrl("http://www.itecad.it"));
}

void win_main::help_email()
{
	if (file_type != FT_NULL)
		GV->tool_stop();
	
        QDesktopServices::openUrl(QUrl("mailto:info@itecad.it"));
}

void win_main::help_about()
{
        if (file_type != FT_NULL)
                GV->tool_stop();

        win_help_about dialogo(this, Qt::SplashScreen);
	// Le seguenti istruzioni servono a centrare una finestra di tipo Qt::Tool in mezzo all'applicazione che la lancia
	// QRect screen = QApplication::desktop()->screenGeometry();
	// dialogo->move(screen.center() - dialogo->frameGeometry().center());
	// dialogo->show();

        dialogo.exec();	//Qui si può usare if(dialogo->exec()) oppure show();
}

bool win_main::is_anchor_point_correct()
{
	QRect all_rect = QRect(0,0,0,0);
	foreach(myGraphicsItem *obj, GS->all_obj)
		all_rect = all_rect.unite(obj->boundingRect());

	if (all_rect.isNull())
		return TRUE;

	if (all_rect.contains(QPoint(0,0)))
		return TRUE;

	switch (QMessageBox::warning(this, "Itecad", "Anchor point is outside of the bounding rectangle including all objects. Proceed anyway?", tr("&Yes"), tr("&No"), tr("&Cancel"), 0, 1))
	{
	case 0:		// 0 = YES
		return TRUE;
	case 1:		// 1 = NO
		return FALSE;
	case 2:		// 2 = CANCEL
		return FALSE;
	}		
}

void win_main::idstamp_refresh_pointer()
{
	for (int i = 0; i < doc_sch.count(); i++)
		{
		myGraphicsView *GVi = (myGraphicsView *) doc_sch.at(i);
		foreach(myComponent *sch_comp, GVi->all_comp)
			sch_comp->idstamp_pointer = 0;
		}

	foreach(myComponent *pcb_comp, doc_pcb->all_comp)
		{
		pcb_comp->idstamp_pointer = 0;
		for (int i = 0; i < doc_sch.count(); i++)
			{
			myGraphicsView *GVi = (myGraphicsView *) doc_sch.at(i);
			foreach(myComponent *sch_comp, GVi->all_comp)
				if (pcb_comp->idstamp == sch_comp->idstamp)
					{
					pcb_comp->idstamp_pointer = (int) sch_comp;
					sch_comp->idstamp_pointer = (int) pcb_comp;
					}
			}
		}
}

void win_main::connection_exit()
{
	if (highlight_active)
                highlight_active = FALSE;
        dynamic_ratsnest_active = FALSE;
        GV->tool_stop();
}

void win_main::connection_add_via()
{

}

bool    win_main::connection_check_junction(node_data *node)
{
        bool    make_a_junction = FALSE;
        int     px = node->point.x();
        int     py = node->point.y();

        foreach(myGraphicsItem *obj, GS->all_obj)
                {
                if ((obj->tipo == O_CONNECTION) && (obj != GV->new_connection))
                        if (obj->layer_intersect(GV->layer_selected, GV->layer_selected))
                                {
                                if (obj->node_start->point.y() == py)
                                        if (obj->node_end->point.y() == py)
                                                {
                                                if (obj->node_start->point.x() < px)
                                                        if (obj->node_end->point.x() > px)
                                                                make_a_junction = TRUE;
                                                if (obj->node_end->point.x() < px)
                                                        if (obj->node_start->point.x() > px)
                                                                make_a_junction = TRUE;
                                                }
                                if (obj->node_start->point.x() == px)
                                        if (obj->node_end->point.x() == px)
                                                {
                                                if (obj->node_start->point.y() < py)
                                                        if (obj->node_end->point.y() > py)
                                                                make_a_junction = TRUE;
                                                if (obj->node_end->point.y() < py)
                                                        if (obj->node_start->point.y() > py)
                                                                make_a_junction = TRUE;
                                                }
                                }
                if (make_a_junction)
                        {
                        GV->ur.push(new connection_insert_node(obj, node));
                        break;
                        }
                }

        //*** Correzione errata creazione dei nuovi collegamenti
        return make_a_junction;
}

bool win_main::connection_add_segment()
{
	bool	track_stop = FALSE;

        if (mouse_new_press_node)
		{
                if (mouse_new_press_node == mouse_old_press_node)
			return FALSE;
		}
	else
                if (mouse_new_press == mouse_old_press)
                        return FALSE;

        if (!GV->is_sch)
		{
                //Controllo che non ci siano errori di DRC
                GV->new_connection->start = mouse_old_press;
                GV->new_connection->end = mouse_new_press;
		GV->new_connection->pen_width = GV->style_pen_width();
		if (drc_active)
			{
                        //Devo calcolare il bounding rect (info_br) prima di usare drc_check_insulation_on_single_object()
                        //Purtroppo non posso usare info_update() perchè
                        int xleft, xright;
                        int ytop, ybottom;
                        xleft = GV->new_connection->start.x();
                        xright = GV->new_connection->end.x();
                        if (xleft > xright)
                                swap_int(xleft,xright);
                        ytop = GV->new_connection->start.y();
                        ybottom = GV->new_connection->end.y();
                        if (ytop < ybottom)
                                swap_int(ytop,ybottom);
                        GV->new_connection->info_br = QRect(xleft - GV->new_connection->pen_width/2, ybottom - GV->new_connection->pen_width/2, xright - xleft + GV->new_connection->pen_width, ytop - ybottom + GV->new_connection->pen_width);
                        GV->new_connection->net_id = GV->active_net_id;


                        if (!drc_insulation_null_on_single_object(GV->new_connection))
				{
				GV->inserting_item_is_visible_anyway = TRUE;
				switch (QMessageBox::critical(this, "Itecad", tr("DRC error: net crossing\nAccept (Enter) or Step Backward (Esc)"), tr("&Accept"), tr("&Step Backward."), "", 0, 1))
				{
				case 0:		// 0 = YES
					break;	
				case 1:		// 1 = NO
				case 2:		// 2 = CANCEL
					return FALSE;
				}
				GV->inserting_item_is_visible_anyway = FALSE;
				}
                        else
                                {
                                if (drc_check_low_insulation)
                                        if (!drc_insulation_on_single_object(GV->new_connection))
                                                {
                                                GV->inserting_item_is_visible_anyway = TRUE;
                                                switch (QMessageBox::warning(this, "Itecad", tr("DRC error: low insulation\nAccept (Enter) or Step Backward (Esc)"), tr("&Accept"), tr("&Step Backward."), "", 0, 1))
                                                {
                                                case 0:		// 0 = YES
                                                        break;
                                                case 1:		// 1 = NO
                                                case 2:		// 2 = CANCEL
                                                        return FALSE;
                                                }
                                                GV->inserting_item_is_visible_anyway = FALSE;
                                                }
                                }
			}
		}

	GV->setUpdatesEnabled(FALSE);

        if (mouse_click_num == 2)
                GV->ur.beginMacro(actions_connectionAct->text());
        if (!mouse_old_press_node)
		{
                mouse_old_press_node = GV->node_new();
                mouse_old_press_node->point = mouse_old_press;
                mouse_old_press_node->highlight_active = TRUE;
                GV->ur.push(new add_node(mouse_old_press_node));
                //Controllo se devo inserire una giunzione sul nodo iniziale
                if (mouse_click_num == 2)
                        connection_check_junction(mouse_old_press_node);
                }
        GV->new_connection->node_start = mouse_old_press_node;
        mouse_old_press_node->connected_wires.append((int) GV->new_connection);
		
        if (!mouse_new_press_node)
		{
                mouse_new_press_node = GV->node_new();
                mouse_new_press_node->point = mouse_new_press;
                mouse_new_press_node->highlight_active = TRUE;
                GV->ur.push(new add_node(mouse_new_press_node));
                //Controllo se devo inserire una giunzione sul nodo finale
                if (connection_check_junction(mouse_new_press_node))
                        track_stop = TRUE;
                }
	else
		track_stop = TRUE;
        GV->new_connection->node_end = mouse_new_press_node;
        GV->new_connection->node_end->net_id = GV->new_connection->node_start->net_id;
        mouse_new_press_node->connected_wires.append((int) GV->new_connection);
        GV->new_connection->highlight_active = TRUE;
	GV->new_connection->info_update();
        GV->ur.push(new add_object(GV->new_connection));

        //Se ho cliccato su un nodo già esistente mi fermo
	if (track_stop)
		{
                GV->ur.endMacro();
		GV->tool_stop();
		highlight_active = FALSE;
		}
	else
		{
                if (!GV->is_sch)
                        dynamic_ratsnest_update(mouse_new_press_node);
                //GV->do_ratsnest_recalculate = FALSE;	//In questo modo evito il fastidioso ricalcolo del ratsnest
		
                GV->new_connection = new myGraphicsItem(O_CONNECTION);
                GV->new_connection->layer_from = GV->layer_selected;
                GV->new_connection->layer_to = GV->layer_selected;
                GV->new_connection->start = mouse_new_press;
		GV->new_connection->pen_width = GV->style_pen_width();
                }
	
        GV->select_clear();
	GV->setUpdatesEnabled(TRUE);

	return TRUE;
}

void win_main::connection_finish_here()
{
        if (mouse_new_press == mouse_right_button)
                if (mouse_click_num > 1)
			connection_exit();
	
        GV->mouse_move_info(mouse_right_button);
        GV->mouse_press_info(mouse_right_button);

        if (connection_add_segment())
		{
		if (highlight_active)
                        highlight_active = FALSE;
                dynamic_ratsnest_active = FALSE;
                GV->ur.endMacro();
		GV->tool_stop();
		}
}

void win_main::connection_finish_on_via()
{
	//Aggiungo la via
        myGraphicsItem *obj = new myGraphicsItem(O_VIA);
        obj->layer_from = style_pcb_via_layer_from;
        obj->layer_to = style_pcb_via_layer_to;
        obj->start = mouse_move;
	obj->number = 0;
	obj->text = QString::number(obj->number);
        obj->shape = style_pcb_pad_shape;
        obj->width = style_pcb_pad_width;
        obj->height = style_pcb_pad_height;
        obj->drill = style_pcb_pad_drill;
        if (!mouse_move_node)
		{
                mouse_move_node = GV->node_new();
                mouse_move_node->point = mouse_move;
                GV->ur.push(new add_node(mouse_move_node));
		}
        mouse_move_node->connected_pin_or_pad_or_via = (int) obj;
        obj->node_start = mouse_move_node;
        GV->ur.push(new add_node(mouse_move_node));
	GV->ur.push(new add_object(obj));
	
	connection_finish_here();
}

void win_main::menu_edit_node_open()
{
        popupEditNodeMenu->clear();

        if (GV->wand_connection)
                {
                if (GV->wand_connection_node)
                        {
                        popupEditNodeMenu->addAction(edit_node_deleteAct);
                        //Aggiungere "Split Node"
                        }
                else
                        {
                        popupEditNodeMenu->addAction(edit_node_insertAct);
                        //Aggiungere "Change Width"
                        }
                }

        if (GV->wand_pol)
                {
                if (GV->wand_pol_is_vertex)
                        {
                        if ((GV->wand_pol->tipo == O_POLYGON) && (GV->wand_pol->polygon.count() > 3))
                                popupEditNodeMenu->addAction(edit_node_deleteAct);
                        else if ((GV->wand_pol->tipo == O_POLYLINE) && (GV->wand_pol->polygon.count() > 2))
                                popupEditNodeMenu->addAction(edit_node_deleteAct);
                        }
                else
                        popupEditNodeMenu->addAction(edit_node_insertAct);
                }
}

void win_main::menu_edit_component_open()
{
        popupEditComponentMenu->clear();

        if (GV->wand_ident || GV->wand_value)
                {
                popupEditComponentMenu->addAction(edit_component_rotate_rAct);
                popupEditComponentMenu->addAction(edit_component_rotate_lAct);
                }
        if (GV->wand_pad)
                {
                popupEditComponentMenu->addAction(edit_component_padAct);
                }
}

void win_main::menu_file_open()
{
        file_recent_1_Act->setVisible(!rf_path_1.isEmpty());
	file_recent_2_Act->setVisible(!rf_path_2.isEmpty());
	file_recent_3_Act->setVisible(!rf_path_3.isEmpty());
	file_recent_4_Act->setVisible(!rf_path_4.isEmpty());
}

void win_main::menu_project_esegui_voce()
{
	GV->tool_stop();
	
	QAction *action = qobject_cast<QAction *>(sender());
	int doc = action->data().toInt();
	
	tw->setCurrentIndex(doc);

	menu_documents_need_an_update = TRUE;
}

void win_main::menu_project_open()
{
	if (menu_documents_need_an_update)
		{
		menu_documents_need_an_update = FALSE;
		projectMenu->clear();
		if (file_type == FT_PROJECT)
			{
			for(int i = 0; i < tw->count(); i++)
				{
				QAction *newAct = new QAction(tw->tabText(i), this);
				newAct->setData(i);
				if (i == tw->currentIndex())
					{
					newAct->setCheckable(TRUE);
					newAct->setChecked(TRUE);
					}
				else
					connect(newAct, SIGNAL(triggered()), this, SLOT(menu_project_esegui_voce()));
				projectMenu->addAction(newAct);
				}
			projectMenu->addSeparator();
			projectMenu->addAction(project_documents_managerAct);
			}
		}
}

void win_main::menu_document_open()
{
	documentMenu->clear();
        if (GV->is_sch)
                documentMenu->addAction(documents_artistic_gridAct);
        else
                {
                documentMenu->addAction(documents_layersAct);
                documentMenu->addSeparator();
		documentMenu->addAction(documents_gridAct);
                }
	documentMenu->addAction(documents_styleAct);
        documentMenu->addSeparator();
        documentMenu->addAction(documents_coloursAct);
        documentMenu->addAction(documents_workspaceAct);

	switch (file_type)
	{
	case FT_PROJECT:
		if (GV->is_sch)
			{
			documentMenu->addAction(documents_pageAct);
			documentMenu->addSeparator();
			documentMenu->addAction(documents_reportAct);
			}
		else
			{
			documentMenu->addSeparator();
			documentMenu->addAction(documents_realtime_drcAct);
			documentMenu->addAction(documents_insulation_checkAct);
			documentMenu->addSeparator();
			documentMenu->addAction(documents_clean_upAct);
			documentMenu->addAction(documents_update_netlistAct);
                        }
		break;
	case FT_SCH_SYMBOL:
	case FT_PCB_SYMBOL:
		documentMenu->addSeparator();
		documentMenu->addAction(documents_component_propertiesAct);
		break;
	}	
}

bool win_main::no_ident_or_value()
{
    switch (file_type)
    {
    case FT_SCH_SYMBOL:
    case FT_PCB_SYMBOL:
        foreach(myGraphicsItem *obj, GV->selected_obj)
            switch (obj->tipo)
            {
            case O_IDENT:
            case O_VALUE:
                return FALSE;
            }
        return TRUE;
        break;
    default:
        return TRUE;
    }
}

int win_main::find_comp_doc(QString ident, bool is_sch)
{
        for (int i = 0; i < doc_sch_and_pcb.count(); i++)
		{
		myGraphicsView *GVi = (myGraphicsView *) doc_sch_and_pcb.at(i);
		if (GVi->is_sch == is_sch)
			{
                        if (GVi->find_comp(ident))
                                return (int) GVi;
			}
		}	
	return 0;
}

bool	win_main::is_available_ident(QString ident, bool check_cloned)
{
	for (int i = 0; i < doc_sch_and_pcb.count(); i++)
		{
		myGraphicsView *GVi = (myGraphicsView *) doc_sch_and_pcb.at(i);
                if (GVi->find_comp(ident))
			return FALSE;
		}	
        if (check_cloned)
                if (GV->clone_find_comp(ident))
                        return FALSE;
        return TRUE;
}

QString	win_main::get_first_available_ident(QString alias, bool check_cloned)
{
	int		first = 1;
	
        while (!is_available_ident(alias + QString::number(first), check_cloned))
		first++;

	return alias + QString::number(first);
}

int	win_main::get_first_available_pin(bool check_cloned)
{
	int first = 1;
	forever
                if (check_cloned)
                        {
                        if (GV->find_pin(first) || GV->clone_find_pin(first))
                                first++;
                        else
                                return first;
                        }
                else
                        {
                        if (GV->find_pin(first))
                                first++;
                        else
                                return first;
                        }
}

int	win_main::get_first_available_pad(bool check_cloned)
{
	int first = 1;
	forever
                if (check_cloned)
                        {
                        if (GV->find_pad(first) || GV->clone_find_pad(first))
                                first++;
                        else
                                return first;
                        }
                else
                        {
                        if (GV->find_pad(first))
                                first++;
                        else
                                return first;
                        }
}

void	win_main::layer_set_current()
{
        if (GV->is_sch)
		{
                if (file_type == FT_PROJECT)
                        {
                        actions_connectionAct->setEnabled(TRUE);
                        }
                else
                        {
                        actions_connectionAct->setEnabled(FALSE);
                        }
                }
	else
		{
                if (layer_data[GV->layer_selected].is_copper && (file_type == FT_PROJECT))
                        {
                        actions_connectionAct->setEnabled(TRUE);
                        actions_polylineAct->setEnabled(FALSE);
                        if (GV->tool == T_POLYLINE)
                                {
                                GV->tool_stop();
                                GV->tool = T_CONNECTION;
                                actions_connectionAct->setChecked(TRUE);
                                }
                        }
                else
                        {
                        actions_connectionAct->setEnabled(FALSE);	//Quando creo un footprint non posso disegnare piste. E' vietato.
                        actions_polylineAct->setEnabled(TRUE);
                        if (GV->tool == T_CONNECTION)
                                {
                                GV->tool_stop();
                                GV->tool = T_POLYLINE;
                                actions_polylineAct->setChecked(TRUE);
                                }
                        }
                }
        sb_style_tooltip();
}

void	win_main::line_exit()
{
	GV->tool_stop();
	switch (GV->tool)
		{
		case T_POLYGON:
			//Devono esserci perlomeno 3 punti per poter inserire
			if (GV->new_polygon->polygon.count() > 3)
				{
				GV->new_polygon->polygon.pop_back();
				GV->new_polygon->info_update();
				GV->ur.beginMacro(actions_polygonAct->text());
				GV->ur.push(new add_object(GV->new_polygon));
				GV->ur.endMacro();
				}
			break;
		case T_POLYLINE:
			//Devono esserci perlomeno 2 punti per poter inserire
			if (GV->new_polyline->polygon.count() > 2)
				{
				GV->new_polyline->polygon.pop_back();
				GV->new_polyline->info_update();	
				GV->ur.beginMacro(actions_polylineAct->text());
				GV->ur.push(new add_object(GV->new_polyline));
				GV->ur.endMacro();
				}
			break;		
		}
}

void win_main::documents_report()
{
        QString message;
        QStringList row_list;
        QString row;

        net_id_update_all_sch();

        foreach(myComponent *comp, GV->all_comp)
                        {
                        row = "";
                        int pin_max = comp->pin_max_number();

                        QStringList sl_up = comp->up.split(";", QString::SkipEmptyParts);

                        for (int pin_number = 0; pin_number <= pin_max; pin_number++)
                                {
                                bool pin_number_exists = FALSE;
                                if (comp->pin_with_number(pin_number))
                                        pin_number_exists = TRUE;

                                for (int i = 0; i < sl_up.count(); i +=2)
                                        if (sl_up.at(i).toInt() == pin_number)
                                                pin_number_exists = TRUE;

                                if (pin_number_exists)
                                        {
                                        //Trovo il valore del net_id
                                        int size = net_names.count();
                                        int net_id;
                                        for (int i = 0; i < size; i++)
                                                if (net_names.at(i).comp_pin_number == pin_number)
                                                        if (net_names.at(i).comp_pointer == (int) comp)
                                                                net_id = net_names.at(i).net_id;

                                        //Controllo se un'altro pin ha lo stesso net_id
                                        bool is_connected = FALSE;
                                        for (int i = 0; i < size; i++)
                                                if (net_names.at(i).net_id == net_id)
                                                        if (net_names.at(i).comp_pointer)
                                                                {
                                                                if (net_names.at(i).comp_pointer != (int) comp)
                                                                        is_connected = TRUE;
                                                                if (net_names.at(i).comp_pin_number != pin_number)
                                                                        is_connected = TRUE;
                                                                }

                                        if (!is_connected)
                                                {
                                                if (!row.isEmpty())
                                                        row += ",";
                                                row += QString::number(pin_number);
                                                }
                                        }
                                }
                        if (!row.isEmpty())
                                {
                                row = comp->ident + " (" + row + ")\n";
                                row_list << row;
                                }
                        }

        if (row_list.isEmpty())
                message = tr("All pins connected.");
        else
                {
                message = tr("Not connected pins:") + "\n";
                row_list.sort();
                for (int i = 0; i < row_list.size(); ++i)
                        message += row_list.at(i);
                }

        QMessageBox::warning(this, QObject::tr("Report NC"), GV->name + "\n\n" + message);
}

void win_main::output_spice()
{
	//Qui conviene uscire con un file .net che possa essere usato da SPICE3, programma creato all'università di Berkeley
        //Ritengo che questo formato di uscita sia lo standard de facto della simulazione
	//Spice è un programma open source per unix e bisogna trovare un porting per windows
        //Ad esempio Spice Opus che è freeware (http://www.spiceopus.si/) oppure Spice+ (http://spicep.sourceforge.net/)
}

void win_main::output_bom()
{
        GV->tool_stop();

        win_bill_of_materials dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
        if (dialogo.exec())
                {
                }
}

void win_main::output_gerber()
{
        GV->tool_stop();

        win_gerber dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, FALSE);
        if (dialogo.exec())
                {
                }
        GV->setFocus();
}

void win_main::output_drill()
{
        GV->tool_stop();

        win_drill dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
        if (dialogo.exec())
                {
                }
        GV->setFocus();
}

void win_main::output_board_and_drills()
{
        GV->tool_stop();

        win_gerber dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, TRUE);
        if (dialogo.exec())
                {
                }
        GV->setFocus();
}

void win_main::page_numbers_update()
{
        for (int i = 0; i < doc_sch.count(); i++)
                {
                if (doc_sch.at(i)->frame_date->text.isEmpty())
                        {
                        doc_sch.at(i)->frame_date->text = QDate::currentDate().toString("d MMMM yyyy");
                        doc_sch.at(i)->frame_date->info_update();
                        }
                doc_sch.at(i)->frame_page->text = QString("Page %1/%2").arg(i+1).arg(doc_sch.count());
                doc_sch.at(i)->frame_page->info_update();
                }
}

void win_main::project_documents_manager()
{
	GV->tool_stop();
	
	win_doc_manager dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
	if (dialogo.exec())
		{
		}	
}

void	win_main::proposed_selection_clear()
{
        proposed_selection_obj.clear();
	proposed_selection_comp.clear();
}

int	win_main::proposed_selection_count()
{
        return proposed_selection_obj.count() + proposed_selection_comp.count();
}

void	win_main::proposed_selection_insert_obj(myGraphicsItem *obj)
{
        if (obj->comp == 0)
		{
		if (obj->tipo == O_CONNECTION)
			{
			//Devo evitare di inserire due wire appartenenti allo stesso collegamento
			if (!obj->highlight_active)
				{
				QRect r;
                                group_command_junction_highlight(obj, &r);
				proposed_selection_obj.append(obj);
				}
			}
		else
			proposed_selection_obj.append(obj);
		}
	else
		{
		myComponent *comp = (myComponent *) obj->comp;
		if (!proposed_selection_comp.contains(comp))
			proposed_selection_comp.append(comp);
		}
}

bool	win_main::proposed_selection_one_is_selected()
{
	foreach (myGraphicsItem *obj, proposed_selection_obj)
		if (obj->is_selected)
			return TRUE;

	foreach (myComponent *comp, proposed_selection_comp)
		if (comp->is_selected)
			return TRUE;

	return FALSE;
}

void win_main::proposed_selection_choose_first()
{
        if (proposed_selection_count() >= 1)
                {
                if (proposed_selection_obj.count())
                        proposed_selection_obj.first()->set_selected(!proposed_selection_obj.first()->is_selected);
                else
                        proposed_selection_comp.first()->set_selected(!proposed_selection_comp.first()->is_selected);
                }
}

void win_main::proposed_selection_choose_cyclic()
{
        static unsigned char alt_index;

        if (proposed_selection_count() == 1)
                proposed_selection_choose_first();
        else
                {
                if (alt_index >= proposed_selection_count())
                        alt_index = 0;
                if (alt_index < proposed_selection_obj.count())
                        proposed_selection_obj.at(alt_index)->set_selected(!proposed_selection_obj.at(alt_index)->is_selected);
                else
                        {
                        unsigned char alt_comp_index = alt_index - proposed_selection_obj.count();
                        proposed_selection_comp.at(alt_comp_index)->set_selected(!proposed_selection_comp.at(alt_comp_index)->is_selected);
                        }
                alt_index++;
                }
}

void win_main::documents_realtime_drc()
{
	win_drc dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, FALSE);
	if (dialogo.exec())
		{
		}
} 

void win_main::documents_insulation_check()
{
	win_drc dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, TRUE);
	if (dialogo.exec())
		{
		}
}

void	win_main::net_id_update_all_sch()
{
        //Azzero il net_id di tutti i nodi degli schematici
	//Il net 0 sta ad indicare un nodo non connesso a niente
	for (int i = 0; i < doc_sch.count(); i++)
		{
		myGraphicsView *GVi = (myGraphicsView *) doc_sch.at(i);
		foreach(node_data *node, GVi->all_nodes)
			node->net_id = 0;
                }

	//Azzero il vettore net_names
        net_names.clear();

	//Trovo il net_id di tutti i nodi degli schematici
	int id = 1;
	static	QList<node_data *> net_group;
	for (int i = 0; i < doc_sch.count(); i++)
		{
		myGraphicsView *GVi = (myGraphicsView *) doc_sch.at(i);
		foreach (node_data *node, GVi->all_nodes)
			if (node->net_id == 0)
				{
				net_group.clear();
                                group_command_add_connected_nodes_sch(net_group, node);
				foreach (node_data *node_in_net, net_group)
					{
					node_in_net->net_id = id;
					if (node_in_net->connected_pin_or_pad_or_via)
						{
						myGraphicsItem *obj = (myGraphicsItem *) node_in_net->connected_pin_or_pad_or_via;
                                                if ((obj->tipo == O_PORT) || (obj->tipo == O_POWER))
							{
							net_names_struct item;
                                                        item.netname = obj->text;
                                                        item.comp_pointer = 0;
                                                        item.comp_pin_number = 0;
                                                        item.net_id = id;
                                                        item.pad = 0;
                                                        net_names.append(item);
							}
                                                else if (obj->tipo == O_PIN)
                                                        {
                                                        net_names_struct item;
                                                        item.netname = "";
                                                        item.comp_pointer = obj->comp;
                                                        item.comp_pin_number = obj->number;
                                                        item.net_id = id;
                                                        item.pad = 0;
                                                        myComponent *sch_comp = (myComponent *) obj->comp;
                                                        if (sch_comp)
                                                                {
                                                                myComponent *pcb_comp = (myComponent *) sch_comp->idstamp_pointer;
                                                                if (pcb_comp)
                                                                        foreach (myGraphicsItem *pcb_obj, pcb_comp->all_obj)
                                                                                if (pcb_obj->tipo == O_PAD)
                                                                                        if (pcb_obj->number == obj->number)
                                                                                                item.pad = (int) pcb_obj;
                                                                }
                                                        net_names.append(item);
                                                        }
                                                }
					}
				id++;
				}
		}

        //Considero gli Ungated Pins che purtroppo non sono associati a nessun nodo nello schematico
        for (int i = 0; i < doc_sch.count(); i++)
                {
                myGraphicsView *GVi = (myGraphicsView *) doc_sch.at(i);
                foreach (myComponent *comp, GVi->all_comp)
                        if (!comp->up.isEmpty())
                                {
                                QStringList sl_up = comp->up.split(";", QString::SkipEmptyParts);
                                QString up_pin, up_name;
                                for (int i = 0; i < sl_up.count(); i +=2)
                                        {
                                        up_pin = sl_up.at(i);
                                        up_name = sl_up.at(i+1);

                                        net_names_struct item;
                                        item.netname = up_name;
                                        item.comp_pointer = (int) comp;
                                        item.comp_pin_number = up_pin.toInt();
                                        item.net_id = id;
                                        item.pad = 0;
                                        myComponent *pcb_comp = (myComponent *) comp->idstamp_pointer;
                                        if (pcb_comp)
                                                foreach (myGraphicsItem *pcb_obj, pcb_comp->all_obj)
                                                        if (pcb_obj->tipo == O_PAD)
                                                            if (QString::number(pcb_obj->number) == up_pin)
                                                                        item.pad = (int) pcb_obj;
                                        net_names.append(item);
                                        id++;				//Importante
                                        }
                                }
                }

        int size = net_names.count();
        QString i_netname, j_netname;
        int i_net_id, j_net_id;
        //Nel caso in cui ad esempio "Vcc" sia collegato a "+5V" tengo un solo netname
        for (int i = 0; i < size; i++)
                {
                i_netname = net_names.at(i).netname;
                i_net_id = net_names.at(i).net_id;
                if (!i_netname.isEmpty())
                        for (int j = 0; j < size; j++)
                                {
                                j_netname = net_names.at(j).netname;
                                j_net_id = net_names.at(j).net_id;
                                if ((i_netname == j_netname) && (i_net_id != j_net_id))
                                        {
                                        //Tengo un solo net_id
                                        for (int k = 0; k < size; k++)
                                                if (net_names.at(k).net_id == j_net_id)
                                                        net_names[k].net_id = i_net_id;
                                        }
                                if ((i_netname != j_netname) && (i_net_id == j_net_id))
                                        {
                                        //Tengo un solo netname
                                        if (!j_netname.isEmpty())
                                                for (int k = 0; k < size; k++)
                                                        if (net_names.at(k).netname == j_netname)
                                                                net_names[k].netname = i_netname;
                                        }
                                }
                }
}

void	win_main::net_id_update_pads_from_pins()
{
        int size = net_names.count();

        //Azzero il net_id di tutti i nodi del PCB
	//Il valore 0 sta ad indicare un nodo non connesso a niente
	foreach(node_data *node, doc_pcb->all_nodes)
		if (node->net_id != 0)
			{
			if (node->connected_pin_or_pad_or_via)
				{
				myGraphicsItem *obj = (myGraphicsItem *) node->connected_pin_or_pad_or_via;
				if (obj->tipo == O_PAD)
					if (node->net_id)
						GV->ur.push(new change_pad_net_id(0, node));
				}
			node->net_id = 0;
			}
		
	//Trovo il net_id dei PAD appartenenti a footprint presenti nello schematico 
        for (int i = 0; i < size; i++)
                if (net_names.at(i).pad)
                        {
                        myGraphicsItem *pad = (myGraphicsItem *) net_names.at(i).pad;
                        GV->ur.push(new change_pad_net_id(net_names.at(i).net_id, pad->node_start));
                        }
}

void	win_main::net_id_update_pcb()
{
        //Questa fumzione è molto impegnativa e va richiamata PRIMA di eseguire operazioni di inserimento/modifica
        //ossia prima di inserire una connection o prima di fare un move o un rotate
        //In questo modo la si può non eseguire quando si fa l'undo/redo
        //Questa funzione va usata poche volte

        //Azzero il net_id di ogni nodo non collegato ad un PAD di un footprint che deriva dallo schematico
	foreach (node_data *pcb_node, doc_pcb->all_nodes)
		if (pcb_node->connected_pin_or_pad_or_via)
			{
			myGraphicsItem *obj = (myGraphicsItem *) pcb_node->connected_pin_or_pad_or_via;
			switch (obj->tipo)
			{
			case O_PAD:
				{
				myComponent *comp = (myComponent *) obj->comp;
				if (comp->idstamp_pointer == 0)
					pcb_node->net_id = 0;
				}
				break;
			case O_VIA:
				pcb_node->net_id = 0;
				break;
			}
			}
		else
			pcb_node->net_id = 0;

        //Scrivo il corretto net_id su tutti i nodi
        QList<myGraphicsItem *> *ao = &doc_pcb->scene->all_obj;
	int ao_size = ao->size();
	for (int i = 0; i < ao_size; ++i)	//Uso questo sistema invece di foreach() perchè così posso usare at() che mi velocizza il tutto.
		{
		myGraphicsItem	*obj = ao->at(i);	
		if (obj->tipo == O_PAD)
			if (obj->node_start->net_id)
				group_command_set_net_id(obj->node_start, obj->node_start->net_id);
		}

        //Scrivo il net_id sugli oggetti utilizzando le funzioni DRC
        drc_net_id_on_all_objects();
}

void	win_main::dynamic_ratsnest_setup(node_data *node)
{
        //Il ratsnest dinamico è meglio se punta sempre a un pad
        //Si potrebbe farlo puntare ad un qualsiasi nodo ma è meglio non farlo
        //Il suggerimento, infatti, potrebbe far commettere un errore al masterista

        if (!node->connected_pin_or_pad_or_via)
                {
                dynamic_ratsnest_active = FALSE;
                return;
                }
        myGraphicsItem *pad = (myGraphicsItem *) node->connected_pin_or_pad_or_via;
        if (pad->tipo != O_PAD)
                {
                dynamic_ratsnest_active = FALSE;
                return;
                }

        drc_connect_id_on_all_objects();

        dynamic_ratsnest_endpoints.clear();
        foreach (myGraphicsItem *obj_i, GS->all_obj)
                if (obj_i->tipo == O_PAD)
                        if (obj_i->net_id == GV->active_net_id)
                                if (obj_i->connect_id != pad->connect_id)
                                        dynamic_ratsnest_endpoints.append(obj_i->node_start);

        if (dynamic_ratsnest_endpoints.count())
                dynamic_ratsnest_active = TRUE;
        else
                {
                dynamic_ratsnest_active = FALSE;
                return;
                }
}

void	win_main::dynamic_ratsnest_update(node_data *node)
{
        qint64	nearest_length, length, delta_x, delta_y;
        nearest_length = 9e18;	//Massimo valore rappresentabile a 64 bit con segno
        foreach (node_data *node_i, dynamic_ratsnest_endpoints)
                {
                delta_x = node_i->point.x() - node->point.x();
                delta_y = node_i->point.y() - node->point.y();
                length = delta_x * delta_x + delta_y * delta_y;
                if (length < nearest_length)
                        {
                        nearest_length = length;
                        dynamic_ratsnest_endpoint = node_i->point;
                        }
                }
}

void	win_main::ratsnest_update()
{
        ratsnest.clear();

        int net_max = 0;
        foreach (node_data *pcb_node, doc_pcb->all_nodes)
                if (pcb_node->connected_pin_or_pad_or_via)
                        {
                        myGraphicsItem *obj = (myGraphicsItem *) pcb_node->connected_pin_or_pad_or_via;
                        if (obj->tipo == O_PAD)
                                {
                                if (pcb_node->net_id > net_max)
                                        net_max = pcb_node->net_id;
                                }
                        }

        drc_connect_id_on_all_objects();

        for (int i = 1; i <= net_max; i++)
                ratsnest_add(i);

}

void	win_main::ratsnest_add(int net_id)
{
        static	QList<myGraphicsItem *> pad_to_insert;
        static	QList<myGraphicsItem *> pad_inserted;

        pad_to_insert.clear();
        foreach (myGraphicsItem *obj, GS->all_obj)
                if (obj->tipo == O_PAD)
                        if (obj->node_start->net_id == net_id)
                                pad_to_insert.append(obj);

        pad_inserted.clear();
        if (pad_to_insert.count())
                {
                int connect_id = pad_to_insert.first()->connect_id;
                foreach (myGraphicsItem *pad, pad_to_insert)
                        if (pad->connect_id == connect_id)
                                pad_inserted.append(pad);
                foreach (myGraphicsItem *pad, pad_inserted)
                        pad_to_insert.removeAll(pad);
                }

        while (pad_to_insert.count())
                {
                        int connect_id;
                        //Trovo il ratsnest più vicino possibile
                        qint64		nearest_length, length, delta_x, delta_y;
                        ratsnest_struct nearest_r;
                        nearest_r.net_id = net_id;
                        nearest_length = 9e18;	//Massimo valore rappresentabile a 64 bit con segno
                        foreach (myGraphicsItem *pad_i, pad_to_insert)
                                foreach (myGraphicsItem *pad_j, pad_inserted)
                                        {
                                        delta_x = pad_i->node_start->point.x() - pad_j->node_start->point.x();
                                        delta_y = pad_i->node_start->point.y() - pad_j->node_start->point.y();
                                        length = delta_x * delta_x + delta_y * delta_y;
                                        if (length < nearest_length)
                                                {
                                                nearest_length = length;
                                                nearest_r.pcb_node_start = (int) pad_i->node_start;
                                                nearest_r.pcb_node_stop = (int) pad_j->node_start;
                                                connect_id = pad_i->connect_id;
                                                }
                                        }
                        //Aggiungo in ratsnest il collegamento più vicino tra i due gruppi
                        ratsnest.append(nearest_r);

                        foreach (myGraphicsItem *pad, pad_to_insert)
                                if (pad->connect_id == connect_id)
                                        pad_inserted.append(pad);
                        foreach (myGraphicsItem *pad, pad_inserted)
                                pad_to_insert.removeAll(pad);
                }
}

void	win_main::ratsnest_remove(int net_id)
{
	//Elimino i ratsnest con net_id
	for (int i = ratsnest.count(); i >= 0; i--)
		if (ratsnest.at(i).net_id == net_id)
			ratsnest.remove(i);
}

void	win_main::group_command_add_connected_nodes_sch(QList<node_data *> &ng, node_data *node)
{
	//Creo una lista con tutti i nodi collegati a node
	ng.append(node);
	foreach (int obj, node->connected_wires)
		{
		myGraphicsItem *wire = (myGraphicsItem *) obj;
		if (node != wire->node_start)
			if (!ng.contains(wire->node_start))
                                group_command_add_connected_nodes_sch(ng, wire->node_start);
		if (node != wire->node_end)
			if (!ng.contains(wire->node_end))
                                group_command_add_connected_nodes_sch(ng, wire->node_end);
		}
}

void	win_main::group_command_junction_highlight(myGraphicsItem *wire, QRect *rect)
{
	//Simile a group_command_select_junction

	*rect = QRect(wire->node_start->point, wire->node_end->point);

	wire->highlight_active = TRUE;
	node_data *next_node;
	myGraphicsItem *next_wire;

	//Procedo in un verso
	next_node = wire->node_start;
	next_wire = wire;
        while ((next_node->connected_wires.count() == 2) && (!next_node->connected_pin_or_pad_or_via))
		{
		foreach (int w, next_node->connected_wires)
                        if (w != (int) next_wire)
				{
				next_wire = (myGraphicsItem *) w;
                                next_wire->highlight_active = TRUE;
                                *rect = rect->unite(QRect(next_wire->node_start->point, next_wire->node_end->point));
				if (next_node != next_wire->node_start)
					next_node = next_wire->node_start;
				else
					next_node = next_wire->node_end;
				break;
                                }
                }

	//Procedo nell'altro verso
	next_node = wire->node_end;
	next_wire = wire;
        while ((next_node->connected_wires.count() == 2) && (!next_node->connected_pin_or_pad_or_via))
		{
		foreach (int w, next_node->connected_wires)
                        if (w != (int) next_wire)
				{
				next_wire = (myGraphicsItem *) w;
                                next_wire->highlight_active = TRUE;
                                *rect = rect->unite(QRect(next_wire->node_start->point, next_wire->node_end->point));
				if (next_node != next_wire->node_start)
					next_node = next_wire->node_start;
				else
					next_node = next_wire->node_end;
				break;
                                }
                }
}

void	win_main::group_command_set_net_id(node_data *nd, int net_id)
{
	nd->net_id = net_id;
	
	foreach (int obj, nd->connected_wires)
		{
		myGraphicsItem *wire = (myGraphicsItem *) obj;
		node_data *new_node;
		if (nd != wire->node_start)
			new_node = wire->node_start;
		else
			new_node = wire->node_end;
		if (new_node->net_id != net_id)
                        {
			if (new_node->connected_pin_or_pad_or_via)
				{
				myGraphicsItem	*obj = (myGraphicsItem	*) new_node->connected_pin_or_pad_or_via;
				if (obj->tipo == O_VIA)
					group_command_set_net_id(new_node, net_id);
				}
			else
				group_command_set_net_id(new_node, net_id);
                        }
		}		
}

void	win_main::track_select(myGraphicsItem *wire, bool connected_tracks)
{
        wire->set_selected(TRUE);
        node_data *ns = wire->node_start;
        bool go;
        if (ns->connected_pin_or_pad_or_via)
                {
                go = FALSE;
                if (((myGraphicsItem *) ns->connected_pin_or_pad_or_via)->tipo == O_VIA)
                        if (connected_tracks || (ns->connected_wires.count() <= 2))
                                    go = TRUE;
                }
        else
                go = (ns->connected_wires.count() <= 2) || connected_tracks;
        if (go)
                foreach (int w, ns->connected_wires)
                        {
                        myGraphicsItem *next_wire = (myGraphicsItem *) w;
                        if (!next_wire->is_selected)
                                track_select(next_wire, connected_tracks);
                        }
        node_data *ne = wire->node_end;
        if (ne->connected_pin_or_pad_or_via)
                {
                go = FALSE;
                if (((myGraphicsItem *) ne->connected_pin_or_pad_or_via)->tipo == O_VIA)
                        if (connected_tracks || (ne->connected_wires.count() <= 2))
                                    go = TRUE;
                }
        else
                go = (ne->connected_wires.count() <= 2) || connected_tracks;
        if (go)
                foreach (int w, ne->connected_wires)
                        {
                        myGraphicsItem *next_wire = (myGraphicsItem *) w;
                        if (!next_wire->is_selected)
                                track_select(next_wire, connected_tracks);
                        }
}

void    win_main::check_data_integrity()
{
        //Questa funzione viene eseguita prima di salvare e dopo aver aperto i file. Serve a controllare eventuali errori
        if (file_type == FT_PROJECT)
                for (int i=0; i < tw->count(); i++)
                        switch (doc_type(tw->widget(i)))
                        {
                        case DT_SCH:
                        case DT_PCB:
                                {
                                myGraphicsView *GVi = (myGraphicsView *) tw->widget(i);
                                GVi->check_data_integrity();
                                }
                                break;
                        case DT_TXT:
                                break;
                        }
        else
                GV->check_data_integrity();
}

void    win_main::disconnect_sch_selection()
{
        foreach (node_data *node, GV->selected_nodes)
                if (node->connected_pin_or_pad_or_via)
                        {
                        //Decido se il pin va scollegato
                        myGraphicsItem *pin = (myGraphicsItem *) node->connected_pin_or_pad_or_via;
                        foreach(int obj, pin->node_start->connected_wires)
                                {
                                bool disconnect = FALSE;
                                myGraphicsItem *wire = (myGraphicsItem *) obj;
                                if (pin->is_selected != wire->is_selected)
                                        disconnect = TRUE;
                                if (disconnect)
                                        if (node->connected_wires.count())
                                                GV->ur.push(new disconnect_pin((myGraphicsItem *) node->connected_pin_or_pad_or_via));
                                }
                        }
        GV->selection_nodes();

        disconnect_selection_executed = TRUE;
}

void	win_main::disconnect_pcb_selection()
{
        //Elimino tutti i collegamenti tra un pad selezionato e qualcosa che non è selezionato
        GV->highlight_clear();
        foreach (myGraphicsItem *obj_i, GS->all_obj)    //Il loop deve essere fatto su tutti gli oggetti e non solo su quelli selezionati
                if (obj_i->is_selected && obj_i->tipo == O_PAD)
                        {
                        foreach (int obj_j, obj_i->node_start->connected_wires)
                                {
                                myGraphicsItem *connection = (myGraphicsItem *) obj_j;
                                if (!connection->is_selected)
                                        {
                                        QRect inutile;
                                        group_command_junction_highlight(connection, &inutile);
                                        }
                                }
                        }

        disconnect_selection_executed = TRUE;

        GV->ur.push(new delete_clean_up());
        GV->selection_nodes();  //Conviene sempre ricalcolare i nodi selezionati
}

void	win_main::connect_sch_selection()
{
        foreach (node_data *nd, GV->selected_nodes)
                GV->node_join(nd);
}

void	win_main::clean_up_pcb_all()
{
        drc_connect_id_on_all_objects();
        GV->select_clear();

        static QList<myGraphicsItem *> clean_up_obj;
        static QList<myGraphicsItem *> connected_obj;
        int connect_id;
        int num_pad;
        myGraphicsItem *one_pad;
        clean_up_obj.clear();
        foreach (myGraphicsItem *obj_i, GS->all_obj)
                if (!obj_i->comp && ((obj_i->tipo == O_CONNECTION) || (obj_i->tipo == O_VIA)))
                        {
                        connect_id = obj_i->connect_id;
                        if (connect_id != -1)
                                {
                                //Aggiorno la lista dei connected_obj
                                connected_obj.clear();
                                num_pad = 0;
                                one_pad = 0;
                                foreach (myGraphicsItem *obj_j, GS->all_obj)
                                        if (!clean_up_obj.contains(obj_j))
                                                if (obj_j->connect_id == connect_id)
                                                        {
                                                        connected_obj.append(obj_j);
                                                        if (obj_j->tipo == O_PAD)
                                                                {
                                                                num_pad++;
                                                                one_pad = obj_j;
                                                                }
                                                        }
                                if (num_pad >= 2)
                                        {
                                        //Rimuovo da connected_obj l'oggetto obj_i
                                        connected_obj.removeAll(obj_i);
                                        //Azzero il connect_id della lista connected_obj
                                        foreach (myGraphicsItem *obj_j, connected_obj)
                                                obj_j->connect_id = -1;
                                        one_pad->connect_id = connect_id;
                                        drc_connect_id_recursive_for_clean_up(&connected_obj, one_pad);

                                        bool useful = FALSE;
                                        foreach (myGraphicsItem *obj_j, connected_obj)
                                                if (obj_j->tipo == O_PAD)
                                                        if (obj_j->connect_id != connect_id)
                                                                useful = TRUE;

                                        if (!useful)
                                                clean_up_obj.append(obj_i);
                                        //Rimetto a posto il connect_id della lista connected_obj
                                        foreach (myGraphicsItem *obj_j, connected_obj)
                                                obj_j->connect_id = connect_id;
                                        }
                                else
                                        clean_up_obj.append(obj_i);
                                }
                        else
                                clean_up_obj.append(obj_i);
                        }

        bool exit = FALSE;
        QRect geometry;
        GV->select_clear();
        while (clean_up_obj.count())
                {
                myGraphicsItem *obj = clean_up_obj.first();
                GV->setUpdatesEnabled(FALSE);
                //Seleziono tutti gli oggetti di clean_up che sono collegati a obj
                foreach (myGraphicsItem *obj_i, clean_up_obj)
                        obj_i->connect_id = -1;
                obj->connect_id = 10;
                drc_connect_id_recursive_for_clean_up(&clean_up_obj, obj);
                foreach (myGraphicsItem *obj_i, clean_up_obj)
                        if (obj_i->connect_id == 10)
                                obj_i->set_selected(TRUE);
                GV->selection_nodes();
                GV->zoom_to_selected();
                GV->zoom_scale(0.1);
                GV->setUpdatesEnabled(TRUE);

                QMessageBox msgBox;
                if (!geometry.isNull())
                        msgBox.setGeometry(geometry);
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText(tr("Clean Up?"));
                msgBox.addButton(tr("&Delete"), QMessageBox::YesRole);
                msgBox.addButton(tr("&Skip"), QMessageBox::NoRole);
                msgBox.addButton(tr("&Exit"), QMessageBox::RejectRole);
                msgBox.setDefaultButton(QMessageBox::Save);
                int ret = msgBox.exec();
                geometry = msgBox.geometry();
                switch(ret)
                {
                case 0:		// 0 = DELETE
                        foreach (myGraphicsItem *obj_i, GS->all_obj)
                                if (obj_i->is_selected)
                                        clean_up_obj.removeAll(obj_i);
                        GV->ur.beginMacro(tr("Clean Up"));
                        GV->ur.push(new delete_selection());
                        GV->ur.endMacro();
                        break;
                case 1:		// 1 = SKIP
                        foreach (myGraphicsItem *obj_i, GS->all_obj)
                                if (obj_i->is_selected)
                                        clean_up_obj.removeAll(obj_i);
                        GV->select_clear();
                        break;
                case 2:		// 2 = EXIT
                        exit = TRUE;
                        break;
                }
                if (exit)
                        break;
                }

        GV->setUpdatesEnabled(FALSE);
        GV->setUpdatesEnabled(TRUE);
}

void win_main::recent_file_insert(QString f_path)
{
	QList<QFileInfo> recent;
	QFileInfo rf1 = QFileInfo(rf_path_1);
	QFileInfo rf2 = QFileInfo(rf_path_2);
	QFileInfo rf3 = QFileInfo(rf_path_3);
	QFileInfo rf4 = QFileInfo(rf_path_4);

	recent.append(QFileInfo(f_path));

	if (rf1.exists())
		if (!recent.contains(rf1))
			recent.append(rf1);

	if (rf2.exists())
		if (!recent.contains(rf2))
			recent.append(rf2);

	if (rf3.exists())
		if (!recent.contains(rf3))
			recent.append(rf3);

	if (rf4.exists())
		if (!recent.contains(rf4))
			recent.append(rf4);

	rf_path_1 = QDir::toNativeSeparators(recent.takeFirst().filePath());

	if (recent.count() > 0)
		rf_path_2 = QDir::toNativeSeparators(recent.takeFirst().filePath());
	else
		rf_path_2 = "";

	if (recent.count() > 0)
		rf_path_3 = QDir::toNativeSeparators(recent.takeFirst().filePath());
	else
		rf_path_3 = "";

	if (recent.count() > 0)
		rf_path_4 = QDir::toNativeSeparators(recent.takeFirst().filePath());
	else
		rf_path_4 = "";

	//Salvo l'elenco dei recent files nel registro di sistema
        program_settings->setValue("recent_file_1", rf_path_1);
        program_settings->setValue("recent_file_2", rf_path_2);
        program_settings->setValue("recent_file_3", rf_path_3);
        program_settings->setValue("recent_file_4", rf_path_4);

	recent_file_update();
}

void win_main::recent_file_update()
{
	//Il testo che visualizzo nei menu potrebbe essere un'abbreviazione del path invece del solo nome del file.
	//QT non prevede nativamente una funzione per abbreviare il nome.
	//In Windows esistono numerose API tipo shortenPath che abbreviano e sono usate da molti
	//Secondo me è sufficiente e forse più elegante scrivere solo il nome
        //Nell esempio Recent Files della documentazione QT si scrive solo il nome
	file_recent_1_Act->setText(QFileInfo(rf_path_1).fileName());
	file_recent_2_Act->setText(QFileInfo(rf_path_2).fileName());
	file_recent_3_Act->setText(QFileInfo(rf_path_3).fileName());
	file_recent_4_Act->setText(QFileInfo(rf_path_4).fileName());
}

void win_main::sb_layer_changed(int index)
{
        GV->tool_stop();

        GV->layer_selected = LAYER_DATA_SCH_COUNT + sb_layer->currentIndex();
        GV->layer_settings[GV->layer_selected].visible = TRUE;	//Rendo visibile il layer selezionato (Il layer selezionato è sempre visibile)

        layer_set_current();

        GV->setUpdatesEnabled(FALSE);
        GV->setUpdatesEnabled(TRUE);
}

void win_main::sb_refresh()
{
        switch (doc_type())
        {
        case DT_SCH:
        case DT_PCB:
                sb_style->setVisible(TRUE);
                if (GV->is_sch)
                        sb_layer->setVisible(FALSE);
                else
                        {
                        sb_layer->setVisible(TRUE);
                        int icon_height = sb_layer->height()*3/4;
                        //Sconnetto i segnali
                        disconnect(sb_layer,SIGNAL(currentIndexChanged(int)),this,SLOT(sb_layer_changed(int)));
                        sb_layer->clear();
                        for(int i = 0; i < LAYER_DATA_COUNT; i++)
                                if (!layer_data[i].is_sch)
                                       sb_layer->addItem(GV->layer_square(i, icon_height), layer_data[i].description);
                        //Riconnetto i segnali
                        connect(sb_layer,SIGNAL(currentIndexChanged(int)),this,SLOT(sb_layer_changed(int)));
                        sb_layer->setCurrentIndex(GV->layer_selected - LAYER_DATA_SCH_COUNT);
                        }
                Z->setVisible(TRUE);
                sb_grid->setVisible(TRUE);
                sb_x->setVisible(TRUE);
                sb_y->setVisible(TRUE);
                sb_deltax->setVisible(TRUE);
                sb_deltay->setVisible(TRUE);
                sb_um->setVisible(TRUE);
                break;
        case DT_TXT:
                sb_layer->setVisible(FALSE);
                sb_grid->setVisible(FALSE);
                sb_style->setVisible(FALSE);
                Z->setVisible(FALSE);
                sb_x->setVisible(FALSE);
                sb_y->setVisible(FALSE);
                sb_deltax->setVisible(FALSE);
                sb_deltay->setVisible(FALSE);
                sb_um->setVisible(FALSE);
                break;
        }
}

void win_main::sb_style_pressed()
{
        documents_style();
	
	sb_style->setDown(FALSE);
	GV->setFocus();
}

void win_main::sb_style_tooltip()
{
	QString tooltip;
	if (GV->is_sch)
		{
                tooltip = tr("Pen Style:\n  Width = ") + xml_write_penwidth(style_sch_pen_width_wire, TRUE) + "\n";
                tooltip += QString(tr("\nText Style:\n  Height = %1").arg((int) style_sch_font_height));
                if (style_sch_font_bold)
                        tooltip += QString(tr(", bold"));
                if (style_sch_font_italic)
                        tooltip += QString(tr(", italic"));
                }
	else
                {
                tooltip = QString(tr("Lines Style:\n  Copper Pen Width = %1 %2").arg(from_001thou_to_string(style_pcb_pen_width_copper, UM), UM_descriptions[UM].descrizione)) + "\n";
                tooltip += QString(tr("  Silk Pen Width = %1 %2").arg(from_001thou_to_string(style_pcb_pen_width_silk, UM), UM_descriptions[UM].descrizione)) + "\n";
                tooltip += QString(tr("  Copper Pen Width = %1 %2").arg(from_001thou_to_string(style_pcb_pen_width_paste, UM), UM_descriptions[UM].descrizione)) + "\n";
                tooltip += QString(tr("  Paste Pen Width = %1 %2").arg(from_001thou_to_string(style_pcb_pen_width_mask, UM), UM_descriptions[UM].descrizione)) + "\n";
                tooltip += QString(tr("  Board Pen Width = %1 %2").arg(from_001thou_to_string(style_pcb_pen_width_board, UM), UM_descriptions[UM].descrizione)) + "\n";
                tooltip += QString(tr("  Help Pen Width = %1 %2").arg(from_001thou_to_string(style_pcb_pen_width_help, UM), UM_descriptions[UM].descrizione));

                tooltip += QString(tr("\n\nText Style:\n  Height = %1").arg((int) style_pcb_font_height));
                if (style_pcb_font_bold)
                        tooltip += QString(tr(", bold"));
                if (style_pcb_font_italic)
                        tooltip += QString(tr(", italic"));
                }

        sb_style->setToolTip(tooltip);
}

void win_main::sb_grid_pressed()
{
        GV->tool_stop();
	
        if (GV->is_sch)
                documents_artistic_grid();
        else
                documents_grid();
	
        sb_grid->setDown(FALSE);
        GV->setFocus();
}

void win_main::sb_grid_tooltip()
{
        QString tooltip;

        if (GV->is_sch)
                {
                tooltip = "Artistic Grid\n";
                tooltip += QString(tr("  %1 thou").arg(sch_grid_artistic / 100));
                }
        else
                {
                tooltip = QString(tr("Grid\n  Visible = %1").arg(xml_write_bool(pcb_grid_visible)));
                tooltip += QString(tr("\n  Size = %1").arg(xml_write_qreal(pcb_grid_size)));
                if (pcb_grid_thou)
                        tooltip += " thou";
                else
                        tooltip += " mm";
                switch (pcb_grid_snap)
                        {
                        case 'N':
                                tooltip += QString(tr("\n  Snap = None"));
                                break;
                        case 'G':
                                tooltip += QString(tr("\n  Snap = Grid"));
                                break;
                        case 'I':
                                tooltip += QString(tr("\n  Snap = Nodes and Grid"));
                                break;
                        }
                }

	sb_grid->setToolTip(tooltip);
}

void win_main::sb_um_change(int indice)
{
    UM = sb_um->currentIndex();	//Aggiorno l'unità di misura globale
	sb_grid_tooltip();
	sb_style_tooltip();
	GV->setFocus();

	//Salvo nel registro di sistema
        program_settings->setValue("measure_unit", UM_descriptions[UM].descrizione);
}

void win_main::setup_actions()
{
        //Gli "standard accelerator" raccomandati nella documentazione QT sono del tipo Alt + O = Open
	//Conviene usare solamente quelli più comuni
	// Alt + A = About
        // Alt + N = New  Purtroppo non riesco a metterlo perchè devo scegliere tra progetto e simboli
        // Alt + O = Open Purtroppo non riesco a metterlo perchè devo scegliere tra progetto e simboli
	// Alt + S = Save 
	// Alt + A = Save As
	// Alt + C = Close
	// Alt + x = Exit
	// Alt + U = Undo 
	// Alt + R = Redo
	// Alt + t = Cut
	// Alt + C = Copy
	// Alt + D = Delete
	// Alt + P = Paste
	// Alt + P = Print
	// Alt + F = Find
	// Oltre a questi conviene mettere gli standard accelerator di tutti i menu


	file_newAct = new QAction(tr("New Project"), this); 
    file_newAct->setIcon(QIcon(":/Resources/page_white.png"));
	connect(file_newAct, SIGNAL(triggered()), this, SLOT(file_new_prj()));

	file_new_prjAct = new QAction(tr("Project"), this); 
    file_new_prjAct->setShortcut(tr("Ctrl+N"));	
	file_new_prjAct->setIcon(QIcon(":/Resources/page_white.png"));
    connect(file_new_prjAct, SIGNAL(triggered()), this, SLOT(file_new_prj()));
				
	file_new_ssfAct = new QAction(tr("Schematic Symbol"), this); 
    connect(file_new_ssfAct, SIGNAL(triggered()), this, SLOT(file_new_ssf()));

	file_new_pffAct = new QAction(tr("PCB Footprint"), this); 
    connect(file_new_pffAct, SIGNAL(triggered()), this, SLOT(file_new_pff()));

	file_openAct = new QAction(tr("Open Project"), this); 
    file_openAct->setIcon(QIcon(":/Resources/folder_page_white.png"));
	connect(file_openAct, SIGNAL(triggered()), this, SLOT(file_open_prj()));

	file_open_prjAct = new QAction(tr("Project"), this); 
    file_open_prjAct->setShortcut(tr("Ctrl+O"));
	file_open_prjAct->setIcon(QIcon(":/Resources/folder_page_white.png"));
    connect(file_open_prjAct, SIGNAL(triggered()), this, SLOT(file_open_prj()));

	file_open_ssfAct = new QAction(tr("Schematic Symbol"), this); 
    connect(file_open_ssfAct, SIGNAL(triggered()), this, SLOT(file_open_ssf()));

	file_open_pffAct = new QAction(tr("PCB Footprint"), this); 
    connect(file_open_pffAct, SIGNAL(triggered()), this, SLOT(file_open_pff()));

        file_closeAct = new QAction(tr("&Close"), this); //Uno dei pochi standard accelerator che servono (vedi commenti a inizio routine)
    connect(file_closeAct, SIGNAL(triggered()), this, SLOT(file_close()));

    file_saveAct = new QAction(tr("&Save"), this); //Uno dei pochi standard accelerator che servono (vedi commenti a inizio routine)
    file_saveAct->setShortcut(tr("Ctrl+S"));
	file_saveAct->setIcon(QIcon(":/Resources/disk.png"));
    connect(file_saveAct, SIGNAL(triggered()), this, SLOT(file_save()));

        file_save_asAct = new QAction(tr("Save &As..."), this); //Uno dei pochi standard accelerator che servono (vedi commenti a inizio routine)
    file_save_asAct->setShortcut(tr("Ctrl+A"));
    connect(file_save_asAct, SIGNAL(triggered()), this, SLOT(file_save_as()));

        file_printAct = new QAction(tr("&Print..."), this);		//Uno dei pochi standard accelerator che servono (vedi commenti a inizio routine)
    file_printAct->setShortcut(tr("Ctrl+P"));
	file_printAct->setIcon(QIcon(":/Resources/printer.png"));
    connect(file_printAct, SIGNAL(triggered()), this, SLOT(file_print()));

	file_print_to_pdfAct = new QAction(tr("Print to PDF"), this); 
    connect(file_print_to_pdfAct, SIGNAL(triggered()), this, SLOT(file_print_to_pdf()));

	file_recent_1_Act = new QAction("", this);		
    connect(file_recent_1_Act, SIGNAL(triggered()), this, SLOT(file_recent_1()));

	file_recent_2_Act = new QAction("", this);		
    connect(file_recent_2_Act, SIGNAL(triggered()), this, SLOT(file_recent_2()));

	file_recent_3_Act = new QAction("", this);		
    connect(file_recent_3_Act, SIGNAL(triggered()), this, SLOT(file_recent_3()));

	file_recent_4_Act = new QAction("", this);		
    connect(file_recent_4_Act, SIGNAL(triggered()), this, SLOT(file_recent_4()));
	
        file_exitAct = new QAction(tr("E&xit"), this); //Uno dei pochi standard accelerator che servono (vedi commenti a inizio routine)
    file_exitAct->setShortcut(tr("Ctrl+Q"));
    connect(file_exitAct, SIGNAL(triggered()), this, SLOT(close()));

        edit_undoAct = new QAction(tr("&Undo"), this); //Uno dei pochi standard accelerator che servono (vedi commenti a inizio routine)
    edit_undoAct->setShortcut(tr("Ctrl+Z"));
	edit_undoAct->setIcon(QIcon(":/Resources/arrow_undo.png"));
    connect(edit_undoAct, SIGNAL(triggered()), this, SLOT(edit_undo()));

    edit_redoAct = new QAction(tr("&Redo"), this); //Uno dei pochi standard accelerator che servono (vedi commenti a inizio routine)
    edit_redoAct->setShortcut(tr("Ctrl+Y"));
	edit_redoAct->setIcon(QIcon(":/Resources/arrow_redo.png"));
    connect(edit_redoAct, SIGNAL(triggered()), this, SLOT(edit_redo()));

	edit_undo_redo_historyAct = new QAction(tr("&Undo/Redo history"), this); //Una lista degli acceleratori standard si trova nell'Help (cercare standard accelerator)
    connect(edit_undo_redo_historyAct, SIGNAL(triggered()), this, SLOT(edit_undo_redo_history()));

        edit_cutAct = new QAction(tr("Cu&t"), this); //Uno dei pochi standard accelerator che servono (vedi commenti a inizio routine)
    edit_cutAct->setShortcut(tr("Ctrl+X"));
	edit_cutAct->setIcon(QIcon(":/Resources/cut.png"));
    connect(edit_cutAct, SIGNAL(triggered()), this, SLOT(edit_cut()));

    edit_copyAct = new QAction(tr("&Copy"), this); //Uno dei pochi standard accelerator che servono (vedi commenti a inizio routine)
    edit_copyAct->setShortcut(tr("Ctrl+C"));
	edit_copyAct->setIcon(QIcon(":/Resources/page_copy.png"));
    connect(edit_copyAct, SIGNAL(triggered()), this, SLOT(edit_copy()));

    edit_pasteAct = new QAction(tr("&Paste"), this);	//Uno dei pochi standard accelerator che servono (vedi commenti a inizio routine)
    edit_pasteAct->setShortcut(tr("Ctrl+V"));
	edit_pasteAct->setIcon(QIcon(":/Resources/page_paste.png"));
    connect(edit_pasteAct, SIGNAL(triggered()), this, SLOT(edit_paste()));

        edit_deleteAct = new QAction(tr("&Delete"), this);	//Uno dei pochi standard accelerator che servono (vedi commenti a inizio routine)
	edit_deleteAct->setShortcut(Qt::Key_Delete);
    connect(edit_deleteAct, SIGNAL(triggered()), this, SLOT(edit_delete()));	

	edit_duplicateAct = new QAction(tr("Duplicate"), this);
	edit_duplicateAct->setShortcut(tr("Ctrl+D"));
    connect(edit_duplicateAct, SIGNAL(triggered()), this, SLOT(edit_duplicate()));

        edit_selectTAct = new QAction(tr("Select Track"), this);
        connect(edit_selectTAct, SIGNAL(triggered()), this, SLOT(edit_select_t()));

        edit_selectCAct = new QAction(tr("Select Connected Tracks"), this);
        connect(edit_selectCAct, SIGNAL(triggered()), this, SLOT(edit_select_c()));

        edit_selectNAct = new QAction(tr("Select Net"), this);
        connect(edit_selectNAct, SIGNAL(triggered()), this, SLOT(edit_select_n()));

	edit_select_allAct = new QAction(tr("Select All"), this);
        connect(edit_select_allAct, SIGNAL(triggered()), this, SLOT(edit_select_all()));

        edit_propertiesAct = new QAction(tr("Properties"), this);
	connect(edit_propertiesAct, SIGNAL(triggered()), this, SLOT(edit_properties()));	

	edit_change_sideAct = new QAction(tr("Change Footprint Side"), this);
	connect(edit_change_sideAct, SIGNAL(triggered()), this, SLOT(edit_change_side()));	

        edit_findAct = new QAction(tr("&Find Component"), this);	//Uno dei pochi standard accelerator che servono (vedi commenti a inizio routine)
    edit_findAct->setShortcut(tr("Ctrl+F"));
	edit_findAct->setIcon(QIcon(":/Resources/find.png"));
    connect(edit_findAct, SIGNAL(triggered()), this, SLOT(edit_find()));

        editor_findAct = new QAction(tr("&Find"), this);	//Uno dei pochi standard accelerator che servono (vedi commenti a inizio routine)
        editor_findAct->setIcon(QIcon(":/Resources/find.png"));
    connect(editor_findAct, SIGNAL(triggered()), this, SLOT(editor_find()));

	editor_styleAct = new QAction(tr("Style"), this);
	editor_styleAct->setIcon(QIcon(":/Resources/style.png"));
    connect(editor_styleAct, SIGNAL(triggered()), this, SLOT(editor_style()));

	project_documents_managerAct = new QAction(tr("Documents Manager"), this);
    connect(project_documents_managerAct, SIGNAL(triggered()), this, SLOT(project_documents_manager()));

    documents_layersAct = new QAction(tr("Layers"), this);
connect(documents_layersAct, SIGNAL(triggered()), this, SLOT(documents_layers()));

	documents_gridAct = new QAction(tr("Grid"), this);
	documents_gridAct->setIcon(QIcon(":/Resources/grid.png"));
    connect(documents_gridAct, SIGNAL(triggered()), this, SLOT(documents_grid()));

        documents_artistic_gridAct = new QAction(tr("Artistic Grid"), this);
        documents_artistic_gridAct->setIcon(QIcon(":/Resources/grid.png"));
    connect(documents_artistic_gridAct, SIGNAL(triggered()), this, SLOT(documents_artistic_grid()));

	documents_styleAct = new QAction(tr("Style"), this);
	documents_styleAct->setIcon(QIcon(":/Resources/style_edit.png"));
	connect(documents_styleAct, SIGNAL(triggered()), this, SLOT(documents_style()));

        documents_pageAct = new QAction(tr("Page Format and Labelling"), this);
	connect(documents_pageAct, SIGNAL(triggered()), this, SLOT(documents_page()));

	documents_realtime_drcAct = new QAction(tr("Realtime DRC"), this);
	connect(documents_realtime_drcAct, SIGNAL(triggered()), this, SLOT(documents_realtime_drc()));

	documents_insulation_checkAct = new QAction(tr("Insulation Check"), this);
	documents_insulation_checkAct->setIcon(QIcon(":/Resources/drc.png"));
	connect(documents_insulation_checkAct, SIGNAL(triggered()), this, SLOT(documents_insulation_check()));

	documents_clean_upAct = new QAction(tr("Clean Up"), this);
    connect(documents_clean_upAct, SIGNAL(triggered()), this, SLOT(documents_clean_up()));

	documents_update_netlistAct = new QAction(tr("Update Netlist"), this);
    documents_update_netlistAct->setIcon(QIcon(":/Resources/lightning.png"));
    connect(documents_update_netlistAct, SIGNAL(triggered()), this, SLOT(documents_update_netlist()));

	documents_component_propertiesAct = new QAction(tr("Properties"), this);
	connect(documents_component_propertiesAct, SIGNAL(triggered()), this, SLOT(documents_component_properties()));

        documents_reportAct = new QAction(tr("Report NC"), this);
        connect(documents_reportAct, SIGNAL(triggered()), this, SLOT(documents_report()));

        actions_mode_dragAct = new QAction("Drag Mode", this);
	actions_mode_dragAct->setCheckable(TRUE);
	connect(actions_mode_dragAct, SIGNAL(triggered()), this, SLOT(actions_mode_drag()));
	addAction(actions_mode_dragAct);	//Collego questa azione direttamente a win_main senza che essa abbia bottoni o menu associati

	actions_mode_moveAct = new QAction("Move Mode", this);
	actions_mode_moveAct->setCheckable(TRUE);
	connect(actions_mode_moveAct, SIGNAL(triggered()), this, SLOT(actions_mode_move()));
	addAction(actions_mode_moveAct);	//Collego questa azione direttamente a win_main senza che essa abbia bottoni o menu associati

        actions_edit_node_all_layersAct = new QAction("All layers", this);
        actions_edit_node_all_layersAct->setCheckable(TRUE);
        connect(actions_edit_node_all_layersAct, SIGNAL(triggered()), this, SLOT(actions_edit_node_all_layers()));
        addAction(actions_edit_node_all_layersAct);	//Collego questa azione direttamente a win_main senza che essa abbia bottoni o menu associati

        actions_edit_node_active_layerAct = new QAction("Active layer", this);
        actions_edit_node_active_layerAct->setCheckable(TRUE);
        connect(actions_edit_node_active_layerAct, SIGNAL(triggered()), this, SLOT(actions_edit_node_active_layer()));
        addAction(actions_edit_node_active_layerAct);	//Collego questa azione direttamente a win_main senza che essa abbia bottoni o menu associati


	actions_escapeAct = new QAction("", this);
	actions_escapeAct->setShortcut(Qt::Key_Escape);
    connect(actions_escapeAct, SIGNAL(triggered()), this, SLOT(actions_escape()));
	addAction(actions_escapeAct);	//Collego questa azione direttamente a win_main senza che essa abbia bottoni o menu associati

	actions_polylineAct = new QAction(tr("Insert Polyline"), this);
    actions_polylineAct->setCheckable(TRUE);
	actions_polylineAct->setIcon(QIcon(":/Resources/draw_polyline.png"));
    connect(actions_polylineAct, SIGNAL(triggered()), this, SLOT(actions_polyline()));

	actions_polygonAct = new QAction(tr("Insert Polygon"), this);
    actions_polygonAct->setCheckable(TRUE);
	actions_polygonAct->setIcon(QIcon(":/Resources/draw_polygon.png"));
    connect(actions_polygonAct, SIGNAL(triggered()), this, SLOT(actions_polygon()));

	actions_circleAct = new QAction(tr("Insert Circle"), this);
    actions_circleAct->setCheckable(TRUE);
	actions_circleAct->setIcon(QIcon(":/Resources/draw_circle.png"));
    connect(actions_circleAct, SIGNAL(triggered()), this, SLOT(actions_circle()));

	actions_rectangleAct = new QAction(tr("Insert Rectangle"), this);
    actions_rectangleAct->setCheckable(TRUE);
	actions_rectangleAct->setIcon(QIcon(":/Resources/draw_rect.png"));
    connect(actions_rectangleAct, SIGNAL(triggered()), this, SLOT(actions_rectangle()));

	actions_handAct = new QAction(tr("Hand"), this);
    actions_handAct->setCheckable(TRUE);
	actions_handAct->setIcon(QIcon(":/Resources/hand.png"));
    connect(actions_handAct, SIGNAL(triggered()), this, SLOT(actions_hand()));

	actions_addAct = new QAction(tr("Insert Component"), this);
    actions_addAct->setCheckable(TRUE);
	actions_addAct->setIcon(QIcon(":/Resources/chip.png"));
    connect(actions_addAct, SIGNAL(triggered()), this, SLOT(actions_add()));

        actions_connectionAct = new QAction("Insert Wire", this);
    actions_connectionAct->setCheckable(TRUE);
        actions_connectionAct->setIcon(QIcon(":/Resources/wire.png"));
    connect(actions_connectionAct, SIGNAL(triggered()), this, SLOT(actions_connection()));

        mainwindow_select_tabAct = new QAction("", this);
    mainwindow_select_tabAct->setShortcut(tr("Tab"));
    connect(mainwindow_select_tabAct, SIGNAL(triggered()), this, SLOT(mainwindow_select_tab()));

        mainwindow_select_spacebarAct = new QAction("", this);
    connect(mainwindow_select_spacebarAct, SIGNAL(triggered()), this, SLOT(mainwindow_select_spacebar()));
    mainwindow_select_spacebarAct->setShortcut(Qt::Key_Space);
    addAction(mainwindow_select_spacebarAct);	//Collego questa azione direttamente a win_main senza che essa abbia bottoni o menu associati

        mainwindow_panUpAct = new QAction("", this);
    mainwindow_panUpAct->setShortcut(Qt::Key_Up);
    connect(mainwindow_panUpAct, SIGNAL(triggered()), this, SLOT(mainwindow_panUp()));

        mainwindow_panDownAct = new QAction("", this);
        mainwindow_panDownAct->setShortcut(Qt::Key_Down);
    connect(mainwindow_panDownAct, SIGNAL(triggered()), this, SLOT(mainwindow_panDown()));

        mainwindow_panLeftAct = new QAction("", this);
    mainwindow_panLeftAct->setShortcut(Qt::Key_Left);
    connect(mainwindow_panLeftAct, SIGNAL(triggered()), this, SLOT(mainwindow_panLeft()));

        mainwindow_panRightAct = new QAction("", this);
    mainwindow_panRightAct->setShortcut(Qt::Key_Right);
    connect(mainwindow_panRightAct, SIGNAL(triggered()), this, SLOT(mainwindow_panRight()));

        mainwindow_zoomInAct = new QAction("", this);
    mainwindow_zoomInAct->setShortcut(Qt::Key_PageUp);
    connect(mainwindow_zoomInAct, SIGNAL(triggered()), this, SLOT(mainwindow_zoom_in()));

        mainwindow_zoomOutAct = new QAction("", this);
    mainwindow_zoomOutAct->setShortcut(Qt::Key_PageDown);
    connect(mainwindow_zoomOutAct, SIGNAL(triggered()), this, SLOT(mainwindow_zoom_out()));

	actions_selectAct = new QAction(tr("Select"), this);
    actions_selectAct->setCheckable(TRUE);
        actions_selectAct->setIcon(QIcon(":/Resources/cursor_blue.png"));
    connect(actions_selectAct, SIGNAL(triggered()), this, SLOT(actions_select()));

        actions_edit_nodeAct = new QAction(tr("Node Edit"), this);
    actions_edit_nodeAct->setCheckable(TRUE);
        actions_edit_nodeAct->setIcon(QIcon(":/Resources/chart_line.png"));
    connect(actions_edit_nodeAct, SIGNAL(triggered()), this, SLOT(actions_edit_node()));

	actions_textAct = new QAction(tr("Insert Text"), this);
    actions_textAct->setCheckable(TRUE);
	actions_textAct->setIcon(QIcon(":/Resources/font.png"));
    connect(actions_textAct, SIGNAL(triggered()), this, SLOT(actions_text()));

	actions_pinAct = new QAction(tr("Insert Pin"), this);
    actions_pinAct->setCheckable(TRUE);
	actions_pinAct->setIcon(QIcon(":/Resources/flag_blue.png"));
    connect(actions_pinAct, SIGNAL(triggered()), this, SLOT(actions_pin()));

        actions_padviaAct = new QAction(tr("Insert Pad/Via"), this);
    actions_padviaAct->setCheckable(TRUE);
        actions_padviaAct->setIcon(QIcon(":/Resources/pad.png"));
    connect(actions_padviaAct, SIGNAL(triggered()), this, SLOT(actions_padvia()));

	actions_portAct = new QAction(tr("Insert Module Port"), this);
    actions_portAct->setCheckable(TRUE);
        actions_portAct->setIcon(QIcon(":/Resources/tag_blue.png"));
    connect(actions_portAct, SIGNAL(triggered()), this, SLOT(actions_port()));

        actions_setOriginAct = new QAction(tr("Set Origin"), this);
    actions_setOriginAct->setCheckable(TRUE);
        actions_setOriginAct->setIcon(QIcon(":/Resources/set_origin.png"));
    connect(actions_setOriginAct, SIGNAL(triggered()), this, SLOT(actions_set_origin()));

        actions_rulerAct = new QAction(tr("Ruler"), this);
    actions_rulerAct->setCheckable(TRUE);
        actions_rulerAct->setIcon(QIcon(":/Resources/ruler.png"));
    connect(actions_rulerAct, SIGNAL(triggered()), this, SLOT(actions_ruler()));

        actions_add_powerAct = new QAction(tr("Insert Power"), this);
    actions_add_powerAct->setCheckable(TRUE);
        actions_add_powerAct->setIcon(QIcon(":/Resources/power.png"));
    connect(actions_add_powerAct, SIGNAL(triggered()), this, SLOT(actions_add_power()));

        actions_edit_componentAct = new QAction(tr("Component Edit"), this);
    actions_edit_componentAct->setCheckable(TRUE);
        actions_edit_componentAct->setIcon(QIcon(":/Resources/wand.png"));
    connect(actions_edit_componentAct, SIGNAL(triggered()), this, SLOT(actions_edit_component()));

	actions_zoomAct = new QAction(tr("Zoom"), this);
    actions_zoomAct->setShortcut(tr("Ctrl+H"));
    actions_zoomAct->setCheckable(TRUE);
	actions_zoomAct->setIcon(QIcon(":/Resources/magnifier.png"));
    connect(actions_zoomAct, SIGNAL(triggered()), this, SLOT(actions_zoom()));

	actions_zoom_to_allAct = new QAction(tr("Zoom to All"), this);
    //actions_zoom_to_allAct->setShortcut(tr("Ctrl+G"));
	actions_zoom_to_allAct->setIcon(QIcon(":/Resources/magnifier_all.png"));
    connect(actions_zoom_to_allAct, SIGNAL(triggered()), this, SLOT(actions_zoom_to_all()));

	actions_moveToAct = new QAction(tr("Move To"), this);
	connect(actions_moveToAct, SIGNAL(triggered()), this, SLOT(actions_move_to()));

	actions_rotate_rAct = new QAction(tr("Rotate Right"), this);
	actions_rotate_rAct->setShortcut(tr("Ctrl+R"));
	actions_rotate_rAct->setIcon(QIcon(":/Resources/shape_rotate_clockwise.png"));
    connect(actions_rotate_rAct, SIGNAL(triggered()), this, SLOT(actions_rotate_r()));

	actions_rotate_lAct = new QAction(tr("Rotate Left"), this);
	actions_rotate_lAct->setShortcut(tr("Ctrl+L"));
	actions_rotate_lAct->setIcon(QIcon(":/Resources/shape_rotate_anticlockwise.png"));
    connect(actions_rotate_lAct, SIGNAL(triggered()), this, SLOT(actions_rotate_l()));

	actions_mirror_hAct = new QAction(tr("Mirror Horizontal"), this);
	actions_mirror_hAct->setIcon(QIcon(":/Resources/shape_flip_horizontal.png"));
	connect(actions_mirror_hAct, SIGNAL(triggered()), this, SLOT(actions_mirror_h()));

	actions_mirror_vAct = new QAction(tr("Mirror Vertical"), this);
	actions_mirror_vAct->setIcon(QIcon(":/Resources/shape_flip_vertical.png"));
        connect(actions_mirror_vAct, SIGNAL(triggered()), this, SLOT(actions_mirror_v()));

        actions_wire_widthAct = new QAction(tr("Wire Width"), this);
        connect(actions_wire_widthAct, SIGNAL(triggered()), this, SLOT(actions_wire_width()));

        output_spiceAct = new QAction(tr("Spice Netlist File"), this);
	connect(output_spiceAct, SIGNAL(triggered()), this, SLOT(output_spice()));

	output_bomAct = new QAction(tr("Bill of Materials"), this);
	connect(output_bomAct, SIGNAL(triggered()), this, SLOT(output_bom()));

        output_gerberAct = new QAction(tr("Gerber"), this);
        connect(output_gerberAct, SIGNAL(triggered()), this, SLOT(output_gerber()));

        output_drillAct = new QAction(tr("Drill"), this);
        connect(output_drillAct, SIGNAL(triggered()), this, SLOT(output_drill()));

        output_board_and_drillsAct = new QAction(tr("Board and Drills"), this);
        connect(output_board_and_drillsAct, SIGNAL(triggered()), this, SLOT(output_board_and_drills()));

        documents_coloursAct = new QAction(tr("Colours"), this);
        documents_coloursAct->setIcon(QIcon(":/Resources/color_swatch.png"));
    connect(documents_coloursAct, SIGNAL(triggered()), this, SLOT(documents_colours()));

        documents_workspaceAct = new QAction(tr("Workspace"), this);
    connect(documents_workspaceAct, SIGNAL(triggered()), this, SLOT(documents_workspace()));

	tools_optionsAct = new QAction(tr("Options"), this);
	tools_optionsAct->setIcon(QIcon(":/Resources/wrench.png"));
    connect(tools_optionsAct, SIGNAL(triggered()), this, SLOT(tools_options()));

        tools_general_default_settingsAct = new QAction(tr("Default settings"), this);
    connect(tools_general_default_settingsAct, SIGNAL(triggered()), this, SLOT(tools_general_default_settings()));

	tools_xml_editorAct = new QAction(tr("XML editor"), this);
    connect(tools_xml_editorAct, SIGNAL(triggered()), this, SLOT(tools_xml_editor()));

	tools_char_editorAct = new QAction(tr("Char editor"), this);
    connect(tools_char_editorAct, SIGNAL(triggered()), this, SLOT(tools_char_editor()));

	tools_length_unit_converterAct = new QAction(tr("Length unit converter"), this);
    connect(tools_length_unit_converterAct, SIGNAL(triggered()), this, SLOT(tools_length_unit_converter()));
	
        help_user_manualAct = new QAction(tr("User Manual"), this);
        help_user_manualAct->setShortcut(tr("F1"));
        help_user_manualAct->setIcon(QIcon(":/Resources/manual.png"));
        help_user_manualAct->setEnabled(FALSE);
    connect(help_user_manualAct, SIGNAL(triggered()), this, SLOT(help_user_manual()));

	help_webAct = new QAction(tr("Itecad on the web"), this);
	help_webAct->setIcon(QIcon(":/Resources/house.png"));
    connect(help_webAct, SIGNAL(triggered()), this, SLOT(help_web()));

	help_emailAct = new QAction(tr("E-Mail to us"), this);
	help_emailAct->setIcon(QIcon(":/Resources/email.png"));
    connect(help_emailAct, SIGNAL(triggered()), this, SLOT(help_email()));

        help_aboutAct = new QAction(tr("&About"), this);	//Uno dei pochi standard accelerator che servono (vedi commenti a inizio routine)
	help_aboutAct->setIcon(QIcon(":/Resources/information.png"));
    connect(help_aboutAct, SIGNAL(triggered()), this, SLOT(help_about()));

	connection_add_viaAct = new QAction(tr("Add Via"), this);
	connect(connection_add_viaAct, SIGNAL(triggered()), this, SLOT(connection_add_via()));

	connection_finish_hereAct = new QAction(tr("Finish here"), this);
	connect(connection_finish_hereAct, SIGNAL(triggered()), this, SLOT(connection_finish_here()));

	connection_finish_on_viaAct = new QAction(tr("Finish on via"), this);
	connect(connection_finish_on_viaAct, SIGNAL(triggered()), this, SLOT(connection_finish_on_via()));

	connection_exitAct = new QAction(tr("Exit"), this);
	connect(connection_exitAct, SIGNAL(triggered()), this, SLOT(connection_exit()));

        edit_node_deleteAct = new QAction(tr("Delete Node"), this);
        connect(edit_node_deleteAct, SIGNAL(triggered()), this, SLOT(edit_node_delete()));

        edit_component_rotate_rAct = new QAction(tr("Rotate Right"), this);
        connect(edit_component_rotate_rAct, SIGNAL(triggered()), this, SLOT(edit_component_rotate_r()));

        edit_component_rotate_lAct = new QAction(tr("Rotate Left"), this);
        connect(edit_component_rotate_lAct, SIGNAL(triggered()), this, SLOT(edit_component_rotate_l()));

        edit_component_padAct = new QAction(tr("Properties"), this);
        connect(edit_component_padAct, SIGNAL(triggered()), this, SLOT(edit_component_pad()));

        edit_node_insertAct = new QAction(tr("Insert Node"), this);
        connect(edit_node_insertAct, SIGNAL(triggered()), this, SLOT(edit_node_insert()));

        //Creo il QActionGroup
	QActionGroup *actions_group = new QActionGroup(this);
	actions_group->addAction(actions_selectAct);
        actions_group->addAction(actions_edit_nodeAct);
	actions_group->addAction(actions_handAct);
	actions_group->addAction(actions_zoomAct);
	actions_group->addAction(actions_connectionAct);
	actions_group->addAction(actions_polylineAct);
	actions_group->addAction(actions_circleAct);
	actions_group->addAction(actions_rectangleAct);
	actions_group->addAction(actions_polygonAct);
	actions_group->addAction(actions_textAct);
	actions_group->addAction(actions_pinAct);
        actions_group->addAction(actions_padviaAct);
	actions_group->addAction(actions_addAct);
	actions_group->addAction(actions_portAct);
        actions_group->addAction(actions_setOriginAct);
        actions_group->addAction(actions_rulerAct);
        actions_group->addAction(actions_add_powerAct);
        actions_group->addAction(actions_edit_componentAct);

	//Creo il QActionModeGroup
	QActionGroup *actions_mode_group = new QActionGroup(this);
	actions_mode_group->addAction(actions_mode_dragAct);
	actions_mode_group->addAction(actions_mode_moveAct);

        //Creo il QActionEditNodeGroup
        QActionGroup *actions_edit_node_group = new QActionGroup(this);
        actions_edit_node_group->addAction(actions_edit_node_all_layersAct);
        actions_edit_node_group->addAction(actions_edit_node_active_layerAct);
}

void win_main::setup_menuBar()
{
        toolBarDrag_or_moveMenu = new QMenu();
        toolBarDrag_or_moveMenu->addAction(actions_mode_dragAct);
        toolBarDrag_or_moveMenu->addAction(actions_mode_moveAct);

        toolBarEditNodeMenu = new QMenu();
        toolBarEditNodeMenu->addAction(actions_edit_node_all_layersAct);
        toolBarEditNodeMenu->addAction(actions_edit_node_active_layerAct);

        fileMenu = new QMenu(tr("&File"));
	QMenu *newMenu = fileMenu->addMenu(tr("New"));	
	newMenu->addAction(file_new_prjAct);
	newMenu->addAction(file_new_ssfAct);
	newMenu->addAction(file_new_pffAct);
	QMenu *openMenu = fileMenu->addMenu(tr("Open"));	
	openMenu->addAction(file_open_prjAct);
	openMenu->addAction(file_open_ssfAct);
	openMenu->addAction(file_open_pffAct);
	fileMenu->addSeparator();
	fileMenu->addAction(file_closeAct);
	fileMenu->addSeparator();
	fileMenu->addAction(file_saveAct);
	fileMenu->addAction(file_save_asAct);
	fileMenu->addSeparator();
	fileMenu->addAction(file_printAct);
	fileMenu->addAction(file_print_to_pdfAct);
	file_separatorAct = fileMenu->addSeparator();
	fileMenu->addAction(file_recent_1_Act);
	fileMenu->addAction(file_recent_2_Act);
	fileMenu->addAction(file_recent_3_Act);
	fileMenu->addAction(file_recent_4_Act);
	fileMenu->addSeparator();
	fileMenu->addAction(file_exitAct);
	connect(fileMenu,SIGNAL(aboutToShow()),this,SLOT(menu_file_open()));

	editMenu = new QMenu(tr("&Edit"));
	editMenu->addAction(edit_undoAct);
	editMenu->addAction(edit_redoAct);
	editMenu->addAction(edit_undo_redo_historyAct);
	editMenu->addSeparator();
	editMenu->addAction(edit_cutAct);
	editMenu->addAction(edit_copyAct);
	editMenu->addAction(edit_pasteAct);
	editMenu->addSeparator();
        editMenu->addAction(edit_selectTAct);
        editMenu->addAction(edit_selectCAct);
        editMenu->addAction(edit_selectNAct);
        editMenu->addAction(edit_select_allAct);
        editMenu->addSeparator();
	editMenu->addAction(edit_deleteAct);
	editMenu->addAction(edit_duplicateAct);
        editMenu->addSeparator();
	editMenu->addAction(edit_findAct);
        editMenu->addSeparator();
        transformMenu = editMenu->addMenu(tr("Transform"));
        transformMenu->addAction(actions_moveToAct);
        transformMenu->addSeparator();
        transformMenu->addAction(actions_rotate_rAct);
        transformMenu->addAction(actions_rotate_lAct);
        transformMenu->addSeparator();
        transformMenu->addAction(actions_mirror_hAct);
        transformMenu->addAction(actions_mirror_vAct);
        editMenu->addSeparator();
	editMenu->addAction(edit_change_sideAct);	
	editMenu->addSeparator();
	editMenu->addAction(edit_propertiesAct);	

	projectMenu = new QMenu(tr("&Project"));
	connect(projectMenu,SIGNAL(aboutToShow()),this,SLOT(menu_project_open()));	

	documentMenu = new QMenu("");	//Il titolo viene definito dopo e può essere &Documents oppure &Symbol
	connect(documentMenu,SIGNAL(aboutToShow()),this,SLOT(menu_document_open()));

        this->addAction(mainwindow_select_tabAct);
        this->addAction(mainwindow_panUpAct);
        this->addAction(mainwindow_panDownAct);
        this->addAction(mainwindow_panLeftAct);
        this->addAction(mainwindow_panRightAct);
        this->addAction(mainwindow_zoomInAct);
        this->addAction(mainwindow_zoomOutAct);

	outputMenu = new QMenu(tr("&Output"));
        outputMenu->addSeparator();
	outputMenu->addAction(output_bomAct);
	outputMenu->addSeparator();
        outputMenu->addAction(output_gerberAct);
        outputMenu->addAction(output_drillAct);
        outputMenu->addAction(output_board_and_drillsAct);

	toolsMenu = new QMenu(tr("&Tools"));
        toolsMenu->addAction(tools_optionsAct);
	toolsMenu->addSeparator();
        toolsMenu->addAction(tools_general_default_settingsAct);
        toolsMenu->addSeparator();
	toolsMenu->addAction(tools_xml_editorAct);
	toolsMenu->addAction(tools_char_editorAct);
	toolsMenu->addAction(tools_length_unit_converterAct);

	helpMenu = new QMenu(tr("&Help"));
        helpMenu->addAction(help_user_manualAct);
        helpMenu->addSeparator();
	helpMenu->addAction(help_webAct);
	helpMenu->addAction(help_emailAct);
	helpMenu->addSeparator();
	helpMenu->addAction(help_aboutAct);

        //Creo i menu di PopUp
        popupSCHEditMenu = new QMenu(tr("&Edit"));
        popupSCHEditMenu->addAction(edit_selectTAct);
        popupSCHEditMenu->addAction(edit_selectCAct);
        popupSCHEditMenu->addAction(edit_selectNAct);
        popupSCHEditMenu->addAction(edit_select_allAct);
        popupSCHEditMenu->addSeparator();
        popupSCHEditMenu->addAction(edit_deleteAct);
        popupSCHEditMenu->addAction(edit_duplicateAct);
        popupSCHEditMenu->addSeparator();
        popupSCHEditMenu->addMenu(transformMenu);
        popupSCHEditMenu->addSeparator();
        popupSCHEditMenu->addAction(edit_propertiesAct);

        popupPCBEditMenu = new QMenu(tr("&Edit"));
        popupPCBEditMenu->addAction(edit_selectTAct);
        popupPCBEditMenu->addAction(edit_selectCAct);
        popupPCBEditMenu->addAction(edit_selectNAct);
        popupPCBEditMenu->addAction(edit_select_allAct);
        popupPCBEditMenu->addSeparator();
        popupPCBEditMenu->addAction(edit_deleteAct);
        popupPCBEditMenu->addAction(edit_duplicateAct);
        popupPCBEditMenu->addSeparator();
        popupPCBEditMenu->addMenu(transformMenu);
        popupPCBEditMenu->addSeparator();
        popupPCBEditMenu->addAction(actions_wire_widthAct);
        popupPCBEditMenu->addSeparator();
        popupPCBEditMenu->addAction(edit_change_sideAct);
        popupPCBEditMenu->addSeparator();
        popupPCBEditMenu->addAction(edit_propertiesAct);

	editorMenu = new QMenu(tr("&Edit"));
	editorMenu->addAction(edit_undoAct);
	editorMenu->addAction(edit_redoAct);
	editorMenu->addSeparator();
	editorMenu->addAction(edit_cutAct);
	editorMenu->addAction(edit_copyAct);
	editorMenu->addAction(edit_pasteAct);
        editorMenu->addSeparator();
        editorMenu->addAction(editor_findAct);
        editorMenu->addSeparator();
	editorMenu->addAction(editor_styleAct);

	popupSCHwireMenu = new QMenu();
	popupSCHwireMenu->addAction(connection_finish_hereAct);
	popupSCHwireMenu->addSeparator();
	popupSCHwireMenu->addAction(connection_exitAct);

        popupPCBwireMenu = new QMenu();
	popupPCBwireMenu->addAction(connection_add_viaAct);
	popupPCBwireMenu->addSeparator();
	popupPCBwireMenu->addAction(connection_finish_hereAct);
	popupPCBwireMenu->addAction(connection_finish_on_viaAct);
	popupPCBwireMenu->addSeparator();
        popupPCBwireMenu->addAction(actions_wire_widthAct);
        popupPCBwireMenu->addSeparator();
	popupPCBwireMenu->addAction(connection_exitAct);

        popupEditNodeMenu = new QMenu("");	//Il titolo viene definito dopo e può essere &Documents oppure &Symbol
        connect(popupEditNodeMenu,SIGNAL(aboutToShow()),this,SLOT(menu_edit_node_open()));

        popupEditComponentMenu = new QMenu("");	//Il titolo viene definito dopo e può essere &Documents oppure &Symbol
        connect(popupEditComponentMenu,SIGNAL(aboutToShow()),this,SLOT(menu_edit_component_open()));
}

void win_main::setup_statusBar()
{
        sb = statusBar();

	int sb_height = 25;

	sb->setFixedHeight(sb_height);	//Questo parametro è molto importante e si potrebbe inserirlo nelle opzioni 
									//Nelle opzioni si potrebbe scegliere tra h=20, 25, 30 etc...
	
        sb_layer = new QComboBox(this);
        sb_layer->setVisible(FALSE);
        sb_layer->setFocusPolicy(Qt::NoFocus);
        sb->addPermanentWidget(sb_layer);	//I permanentWidget sono inseriti a destra
        connect(sb_layer,SIGNAL(currentIndexChanged(int)),this,SLOT(sb_layer_changed(int)));
	
	sb_grid = new QToolButton(this);
	sb_grid->setToolButtonStyle(Qt::ToolButtonTextOnly);
	sb_grid->setIcon(QIcon(":/Resources/grid.png"));
	sb_grid->setAutoRaise(TRUE);
	sb->addPermanentWidget(sb_grid);	//I permanentWidget sono inseriti a destra
	connect(sb_grid,SIGNAL(pressed()),this,SLOT(sb_grid_pressed()));
	sb_grid->setEnabled(FALSE);

	sb_style = new QToolButton(this);
	sb_style->setToolButtonStyle(Qt::ToolButtonTextOnly);
	sb_style->setIcon(QIcon(":/Resources/style_edit.png"));
	sb_style->setAutoRaise(TRUE);
	sb->addPermanentWidget(sb_style);	//I permanentWidget sono inseriti a destra
	connect(sb_style,SIGNAL(pressed()),this,SLOT(sb_style_pressed()));
	sb_style->setEnabled(FALSE);

	Z = new Zoom(this);
	sb->addPermanentWidget(Z);
	Z->setEnabled(FALSE);
		
	sb_x = new QLabel("");		
	sb_x->setFixedWidth(sb_height * 5);			
	sb_x->setFrameStyle(QFrame::StyledPanel);
	sb_x->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	sb_x->setMargin(0);
	sb->addPermanentWidget(sb_x);	//I permanentWidget sono inseriti a destra

	sb_y = new QLabel("");
	sb_y->setFixedWidth(sb_height * 5);
	sb_y->setFrameStyle(QFrame::StyledPanel);		
	sb_y->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	sb_y->setMargin(0);
	sb->addPermanentWidget(sb_y);	//I permanentWidget sono inseriti a destra

	sb_deltax = new QLabel("");		
	sb_deltax->setFixedWidth(sb_height * 5);			
	sb_deltax->setFrameStyle(QFrame::StyledPanel);
	sb_deltax->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	sb_deltax->setMargin(0);
	sb->addPermanentWidget(sb_deltax);	//I permanentWidget sono inseriti a destra
	
	sb_deltay = new QLabel("");		
	sb_deltay->setFixedWidth(sb_height * 5);			
	sb_deltay->setFrameStyle(QFrame::StyledPanel);
	sb_deltay->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	sb_deltay->setMargin(0);
	sb->addPermanentWidget(sb_deltay);	//I permanentWidget sono inseriti a destra
	
	sb_um = new QComboBox(this);
	for (int i = 0; i < UM_DESCRIPTION_COUNT; i++)
		sb_um->insertItem(i, UM_descriptions[i].descrizione);
	sb_um->setCurrentIndex(UM);	
	sb->addPermanentWidget(sb_um);	//I permanentWidget sono inseriti a destra
	connect(sb_um,SIGNAL(activated(int)),this,SLOT(sb_um_change(int)));
	sb_um->setEnabled(FALSE);	
}

void win_main::setup_toolBar()
{
	toolbar_standard = new QToolBar(tr("Standard"), this);
	toolbar_standard->setObjectName("S");	//E' molto importante che sia un nome unico non duplicato
	toolbar_standard->addAction(file_newAct);
	toolbar_standard->addAction(file_openAct);
	toolbar_standard->addAction(file_saveAct);
	toolbar_standard->addAction(file_printAct);
	addToolBar(toolbar_standard);
	
	toolbar_zoom = new QToolBar(tr("Zoom"), this);	
	toolbar_zoom->setObjectName("Z");	//E' molto importante che sia un nome unico non duplicato
	toolbar_zoom->addAction(actions_zoomAct);
	toolbar_zoom->addAction(actions_zoom_to_allAct);
	toolbar_zoom->addAction(actions_handAct);
        toolbar_zoom->addAction(actions_setOriginAct);
        toolbar_zoom->addAction(actions_rulerAct);
	addToolBar(toolbar_zoom);

	toolbar_drawing = new QToolBar(tr("Drawing"), this);
	toolbar_drawing->setObjectName("D");	//E' molto importante che sia un nome unico non duplicato
	toolbar_drawing->addAction(actions_selectAct);
        toolbar_drawing->addAction(actions_edit_nodeAct);
        toolbar_drawing->addAction(actions_connectionAct);
        toolbar_drawing->addAction(actions_textAct);
        toolbar_drawing->addAction(actions_polylineAct);
        toolbar_drawing->addAction(actions_circleAct);
        toolbar_drawing->addAction(actions_rectangleAct);
        toolbar_drawing->addAction(actions_polygonAct);
        toolbar_drawing->addAction(actions_pinAct);
        toolbar_drawing->addAction(actions_padviaAct);
        toolbar_drawing->addAction(actions_addAct);
        toolbar_drawing->addAction(actions_portAct);
        toolbar_drawing->addAction(actions_add_powerAct);
        toolbar_drawing->addAction(actions_edit_componentAct);
        toolbar_drawing->addAction(documents_update_netlistAct);

        QToolButton *button;
        button = (QToolButton *) toolbar_drawing->widgetForAction(actions_selectAct);
        button->setPopupMode(QToolButton::DelayedPopup);
        button->setMenu(toolBarDrag_or_moveMenu);

        button = (QToolButton *) toolbar_drawing->widgetForAction(actions_edit_nodeAct);
        button->setPopupMode(QToolButton::DelayedPopup);
        button->setMenu(toolBarEditNodeMenu);
        
        addToolBar(toolbar_drawing);
}

void    win_main::settings_factory2temp()
{
        for (int i = 0; i < FACTORY_VALUE_COUNT; i++)
                factory_values[i].temp = factory_values[i].value;

}

void    win_main::settings_generaldefault2temp()
{
    for (int i = 0; i < FACTORY_VALUE_COUNT; i++)
            switch (factory_values[i].type)
            {
            case QVariant::Int:
                    factory_values[i].temp          = program_settings->value(factory_values[i].description, factory_values[i].value).toInt();
                    break;
            case QVariant::Bool:
                    factory_values[i].temp          = program_settings->value(factory_values[i].description, factory_values[i].value).toBool();
                    break;
            case QVariant::Color:
                    factory_values[i].temp          = program_settings->value(factory_values[i].description, factory_values[i].value).value<QColor>();
                    break;
            case QVariant::Double:
                    factory_values[i].temp          = program_settings->value(factory_values[i].description, factory_values[i].value).toReal();
                    break;
            }
}

void    win_main::settings_temp2generaldefault()
{
        for (int i = 0; i < FACTORY_VALUE_COUNT; i++)
                program_settings->setValue(factory_values[i].description, factory_values[i].temp);
}

void    win_main::settings_current2temp()
{
        // SCH Workspace
        factory_values[0].temp      = workspace_data[W_SCHbackground].color;
        factory_values[1].temp      = workspace_data[W_SCHpage].color;
        factory_values[2].temp      = workspace_data[W_SCHgrid].color;
        factory_values[3].temp      = workspace_data[W_SCHselection].color;
        // PCB Workspace
        factory_values[4].temp      = workspace_data[W_PCBbackground].color;
        factory_values[5].temp      = workspace_data[W_PCBgrid].color;
        factory_values[6].temp      = workspace_data[W_PCBpadthrough].color;
        factory_values[7].temp      = workspace_data[W_PCBpadtop].color;
        factory_values[8].temp      = workspace_data[W_PCBpadbottom].color;
        factory_values[9].temp      = workspace_data[W_PCBvia].color;
        factory_values[10].temp      = workspace_data[W_PCBburiedvia].color;
        factory_values[11].temp      = workspace_data[W_PCBratsnest].color;
        factory_values[12].temp      = workspace_data[W_PCBdynamicratsnest].color;
        factory_values[13].temp      = workspace_data[W_PCBselection].color;
        // SCH layers
        factory_values[14].temp      = layer_data[L_SCHNormal].color;
        factory_values[15].temp      = layer_data[L_SCHFrame].color;
        factory_values[16].temp     = layer_data[L_SCHWire].color;
        // PCB Layers
        factory_values[17].temp     = layer_data[L_PCBBoard].color;
        factory_values[18].temp     = layer_data[L_PCBPasteTop].color;
        factory_values[19].temp     = layer_data[L_PCBSilkTop].color;
        factory_values[20].temp     = layer_data[L_PCBMaskTop].color;
        factory_values[21].temp     = layer_data[L_PCBCopper1].color;
        factory_values[22].temp     = layer_data[L_PCBCopper16].color;
        factory_values[23].temp     = layer_data[L_PCBMaskBottom].color;
        factory_values[24].temp     = layer_data[L_PCBSilkBottom].color;
        factory_values[25].temp     = layer_data[L_PCBPasteBottom].color;
        factory_values[26].temp     = layer_data[L_PCBHelp].color;

        // SCH Style
        factory_values[27].temp     = style_sch_pen_width_normal;
        factory_values[28].temp     = style_sch_pen_width_frame;
        factory_values[29].temp     = style_sch_pen_width_wire;
        factory_values[30].temp     = style_sch_fill_polygon;
        factory_values[31].temp     = style_sch_font_height;
        factory_values[32].temp     = style_sch_font_bold;
        factory_values[33].temp     = style_sch_font_italic;

        // PCB Style
        factory_values[34].temp     = style_pcb_pen_width_copper;
        factory_values[35].temp     = style_pcb_pen_width_silk;
        factory_values[36].temp     = style_pcb_pen_width_paste;
        factory_values[37].temp     = style_pcb_pen_width_mask;
        factory_values[38].temp     = style_pcb_pen_width_board;
        factory_values[39].temp     = style_pcb_pen_width_help;
        factory_values[40].temp     = style_pcb_fill_polygon;
        factory_values[41].temp     = style_pcb_font_height;
        factory_values[42].temp     = style_pcb_font_bold;
        factory_values[43].temp     = style_pcb_font_italic;
        factory_values[44].temp     = style_pcb_pad_shape;
        factory_values[45].temp     = style_pcb_pad_width;
        factory_values[46].temp     = style_pcb_pad_height;
        factory_values[47].temp     = style_pcb_pad_layer_from;
        factory_values[48].temp     = style_pcb_pad_layer_to;
        factory_values[49].temp     = style_pcb_pad_drill;
        factory_values[50].temp     = style_pcb_via_width;
        factory_values[51].temp     = style_pcb_via_layer_from;
        factory_values[52].temp     = style_pcb_via_layer_to;
        factory_values[53].temp     = style_pcb_via_drill;

        // SCH Grid
        factory_values[54].temp     = sch_grid_artistic;

        // PCB Grid
        factory_values[55].temp     = pcb_grid_visible;
        factory_values[56].temp     = pcb_grid_size;
        factory_values[57].temp     = pcb_grid_thou;
        factory_values[58].temp     = pcb_grid_snap;

        // PCB Extra
        factory_values[59].temp     = pcb_pad_via_filled;
}

void    win_main::settings_temp2current()
{
        // SCH Workspace
        workspace_data[W_SCHbackground].color   = factory_values[0].temp.value<QColor>();
        workspace_data[W_SCHpage].color         = factory_values[1].temp.value<QColor>();
        workspace_data[W_SCHgrid].color         = factory_values[2].temp.value<QColor>();
        workspace_data[W_SCHselection].color    = factory_values[3].temp.value<QColor>();
        // PCB Workspace
        workspace_data[W_PCBbackground].color   = factory_values[4].temp.value<QColor>();
        workspace_data[W_PCBgrid].color         = factory_values[5].temp.value<QColor>();        
        workspace_data[W_PCBpadthrough].color   = factory_values[6].temp.value<QColor>();
        workspace_data[W_PCBpadtop].color       = factory_values[7].temp.value<QColor>();
        workspace_data[W_PCBpadbottom].color    = factory_values[8].temp.value<QColor>();
        workspace_data[W_PCBvia].color          = factory_values[9].temp.value<QColor>();
        workspace_data[W_PCBburiedvia].color    = factory_values[10].temp.value<QColor>();
        workspace_data[W_PCBratsnest].color     = factory_values[11].temp.value<QColor>();
        workspace_data[W_PCBdynamicratsnest].color = factory_values[12].temp.value<QColor>();
        workspace_data[W_PCBselection].color    = factory_values[13].temp.value<QColor>();
        // SCH layers
        layer_data[L_SCHNormal].color           = factory_values[14].temp.value<QColor>();
        layer_data[L_SCHFrame].color            = factory_values[15].temp.value<QColor>();
        layer_data[L_SCHWire].color             = factory_values[16].temp.value<QColor>();
        // PCB Layers
        layer_data[L_PCBBoard].color            = factory_values[17].temp.value<QColor>();
        layer_data[L_PCBPasteTop].color         = factory_values[18].temp.value<QColor>();
        layer_data[L_PCBSilkTop].color          = factory_values[19].temp.value<QColor>();
        layer_data[L_PCBMaskTop].color          = factory_values[20].temp.value<QColor>();
        layer_data[L_PCBCopper1].color          = factory_values[21].temp.value<QColor>();
        layer_data[L_PCBCopper16].color         = factory_values[22].temp.value<QColor>();
        layer_data[L_PCBMaskBottom].color       = factory_values[23].temp.value<QColor>();
        layer_data[L_PCBSilkBottom].color       = factory_values[24].temp.value<QColor>();
        layer_data[L_PCBPasteBottom].color      = factory_values[25].temp.value<QColor>();
        layer_data[L_PCBHelp].color             = factory_values[26].temp.value<QColor>();


        // SCH Style
        style_sch_pen_width_normal              = factory_values[27].temp.toInt();
        style_sch_pen_width_frame               = factory_values[28].temp.toInt();
        style_sch_pen_width_wire                = factory_values[29].temp.toInt();
        style_sch_fill_polygon                  = factory_values[30].temp.toBool();
        style_sch_font_height                   = factory_values[31].temp.toInt();
        style_sch_font_bold                     = factory_values[32].temp.toBool();
        style_sch_font_italic                   = factory_values[33].temp.toBool();

        // PCB Style
        style_pcb_pen_width_copper              = factory_values[34].temp.toInt();
        style_pcb_pen_width_silk                = factory_values[35].temp.toInt();
        style_pcb_pen_width_paste               = factory_values[36].temp.toInt();
        style_pcb_pen_width_mask                = factory_values[37].temp.toInt();
        style_pcb_pen_width_board               = factory_values[38].temp.toInt();
        style_pcb_pen_width_help                = factory_values[39].temp.toInt();
        style_pcb_fill_polygon                  = factory_values[40].temp.toBool();
        style_pcb_font_height                   = factory_values[41].temp.toInt();
        style_pcb_font_bold                     = factory_values[42].temp.toBool();
        style_pcb_font_italic                   = factory_values[43].temp.toBool();
        style_pcb_pad_shape                     = factory_values[44].temp.toInt();
        style_pcb_pad_width                     = factory_values[45].temp.toInt();
        style_pcb_pad_height                    = factory_values[46].temp.toInt();
        style_pcb_pad_layer_from                = factory_values[47].temp.toInt();
        style_pcb_pad_layer_to                  = factory_values[48].temp.toInt();
        style_pcb_pad_drill                     = factory_values[49].temp.toInt();
        style_pcb_via_width                     = factory_values[50].temp.toInt();
        style_pcb_via_layer_from                = factory_values[51].temp.toInt();
        style_pcb_via_layer_to                  = factory_values[52].temp.toInt();
        style_pcb_via_drill                     = factory_values[53].temp.toInt();

        // SCH Grid
        sch_grid_artistic                       = factory_values[54].temp.toReal();

        // PCB Grid
        pcb_grid_visible                        = factory_values[55].temp.toBool();
        pcb_grid_size                           = factory_values[56].temp.toReal();
        pcb_grid_thou                           = factory_values[57].temp.toBool();
        pcb_grid_snap                           = factory_values[58].temp.toInt();

        // PCB Extra
        pcb_pad_via_filled                      = factory_values[59].temp.toBool();
}

void win_main::tools_options()
{
	win_options dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
	if (dialogo.exec())
		{
		}
} 

void win_main::tools_general_default_settings()
{
        GV->tool_stop();

        win_settings dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, 0);
        if (dialogo.exec())
                {
                }
}

void win_main::tools_length_unit_converter()
{
	win_converter dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
	if (dialogo.exec())
		{
		}
} 

void win_main::tools_xml_editor()
{
	win_xml_editor dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
	if (dialogo.exec())
		{
		}
} 

void win_main::tools_char_editor()
{
	win_char_editor dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
	if (dialogo.exec())
		{
		}
}

void win_main::change_document(int doc)
{
        highlight_active = FALSE;

        if (tool_avviato)
                GV->tool_stop();

        if (modeless_win_style)
                {
                delete modeless_win_style;
                modeless_win_style = 0;
                }

        if (modeless_win_find_text)
                {
                delete modeless_win_find_text;
                modeless_win_find_text = 0;
                }

        menuBar()->clear();
        menuBar()->addMenu(fileMenu);

        char tipo;
        switch (file_type)
        {
        case FT_PROJECT:
                if (tw->count() == 0)
                    return;
                tipo = doc_type(tw->widget(doc));
                break;
        case FT_SCH_SYMBOL:
                tipo = DT_SCH;
                break;
        case FT_PCB_SYMBOL:
                tipo = DT_PCB;
                break;
        }

        switch (tipo)
        {
        case DT_SCH:
        case DT_PCB:
                toolbar_zoom->setEnabled(TRUE);
                toolbar_drawing->setEnabled(TRUE);
                toolbar_zoom->setVisible(TRUE);
                toolbar_drawing->setVisible(TRUE);
                actions_rulerAct->setEnabled(TRUE);
                if (file_type == FT_PROJECT)
                        {
                        tw->setCurrentIndex(doc);
                        //Definisco i puntatori
                        GV = (myGraphicsView *) tw->widget(doc);
                        GS = GV->scene;
                        }
                if (GV->is_sch)
                        {
                        if (file_type == FT_SCH_SYMBOL)
                                {
                                actions_pinAct->setEnabled(TRUE);
                                actions_portAct->setEnabled(FALSE);
                                actions_add_powerAct->setEnabled(FALSE);
                                actions_edit_componentAct->setEnabled(FALSE);
                                }
                        else
                                {
                                actions_pinAct->setEnabled(FALSE);
                                actions_portAct->setEnabled(TRUE);
                                actions_add_powerAct->setEnabled(TRUE);
                                actions_edit_componentAct->setEnabled(TRUE);
                                }
                        actions_setOriginAct->setEnabled(FALSE);    //Devo disabilitare altrimenti potrei cambiare l'anchor-point di un simbolo e nei progetti non ha alcun senso...
                        actions_padviaAct->setEnabled(FALSE);
                        documents_clean_upAct->setEnabled(FALSE);
                        documents_update_netlistAct->setEnabled(FALSE);
                        documents_realtime_drcAct->setEnabled(FALSE);
                        documents_insulation_checkAct->setEnabled(FALSE);
                        output_bomAct->setEnabled(TRUE);
                        output_gerberAct->setEnabled(FALSE);
                        output_drillAct->setEnabled(FALSE);
                        output_board_and_drillsAct->setEnabled(FALSE);
                        //Metto a posto la wand
                        QToolButton *button = (QToolButton *) toolbar_drawing->widgetForAction(actions_edit_nodeAct);
                        button->setMenu(0);
                        actions_edit_nodeAct->setIcon(QIcon(":/Resources/chart_line.png"));
                        }
                else
                        {
                        if (file_type == FT_PCB_SYMBOL)
                                {
                                actions_edit_componentAct->setEnabled(FALSE);
                                }
                        else
                                {
                                actions_edit_componentAct->setEnabled(TRUE);
                                }
                        actions_setOriginAct->setEnabled(TRUE);
                        actions_pinAct->setEnabled(FALSE);
                        actions_padviaAct->setEnabled(TRUE);
                        actions_portAct->setEnabled(FALSE);
                        actions_add_powerAct->setEnabled(FALSE);
                        documents_clean_upAct->setEnabled(file_type == FT_PROJECT);
                        documents_update_netlistAct->setEnabled(file_type == FT_PROJECT);
                        documents_realtime_drcAct->setEnabled(file_type == FT_PROJECT);
                        documents_insulation_checkAct->setEnabled(file_type == FT_PROJECT);
                        output_bomAct->setEnabled(FALSE);
                        output_gerberAct->setEnabled(TRUE);
                        output_drillAct->setEnabled(TRUE);
                        output_board_and_drillsAct->setEnabled(TRUE);
                        //Metto a posto la wand
                        QToolButton *button = (QToolButton *) toolbar_drawing->widgetForAction(actions_edit_nodeAct);
                        button->setMenu(toolBarEditNodeMenu);
                        if (edit_node_in_pcb_is_all_layers)
                                actions_edit_nodeAct->setIcon(QIcon(":/Resources/chart_line.png"));
                        else
                                actions_edit_nodeAct->setIcon(QIcon(":/Resources/chart_line2.png"));
                        }

                menu_documents_need_an_update = TRUE;

                GV->do_selection_rect_recalculate = TRUE;

                //Aggiorno la StatusBar e lo Zoom
                sb_refresh();
                sb_grid_tooltip();
                sb_style_tooltip();
                Z->zoom_show_scale();

                GV->tool_change_to_default();

                //Aggiorno le abilitazioni di Undo/Redo
                edit_undoAct->setEnabled(GV->ur.canUndo());
                edit_redoAct->setEnabled(GV->ur.canRedo());

                //Aggiorno i menu e la statusBar
                menuBar()->addMenu(editMenu);
                switch (file_type)
                {
                case FT_PROJECT:
                       menuBar()->addMenu(projectMenu);
                       menuBar()->addMenu(documentMenu);
                       menuBar()->addMenu(outputMenu);
                       menuBar()->addMenu(toolsMenu);
                       break;
                case FT_SCH_SYMBOL:
                case FT_PCB_SYMBOL:
                       menuBar()->addMenu(documentMenu);
                       menuBar()->addMenu(toolsMenu);
                       break;
                }
                edit_can_undo_changed(GV->ur.canUndo());
                edit_can_redo_changed(GV->ur.canRedo());
                break;
        case DT_TXT:
                toolbar_zoom->setEnabled(FALSE);
                toolbar_drawing->setEnabled(FALSE);
                toolbar_zoom->setVisible(FALSE);
                toolbar_drawing->setVisible(FALSE);
                documents_realtime_drcAct->setEnabled(FALSE);
                documents_insulation_checkAct->setEnabled(FALSE);
                menu_documents_need_an_update = TRUE;
                menuBar()->addMenu(editorMenu);
                menuBar()->addMenu(projectMenu);
                menuBar()->addMenu(toolsMenu);
                sb_refresh();
                edit_undoAct->setText(tr("Undo"));
                edit_undoAct->setEnabled(TE->document()->isUndoAvailable());
                edit_redoAct->setText(tr("Redo"));
                edit_redoAct->setEnabled(TE->document()->isRedoAvailable());
                break;
        }

        menuBar()->addMenu(helpMenu);
        layer_set_current();
}

void win_main::change_selection()
{
        int num_comp = GV->selected_comp.count();
        int num_obj = GV->selected_obj.count();
        int selected_num = num_comp + num_obj;
        bool only_one = (selected_num == 1);
        bool only_one_connection = FALSE;
        if ((num_comp == 0))
                if (num_obj == 1)
                        if (GV->selected_obj.first()->tipo == O_CONNECTION)
                                only_one_connection = TRUE;
        bool only_pcb_connections = FALSE;
        if (!GV->is_sch && (num_comp == 0))
                {
                only_pcb_connections = TRUE;
                if (num_obj)
                        foreach (myGraphicsItem *obj, GV->selected_obj)
                                if (obj->tipo != O_CONNECTION)
                                        only_pcb_connections = FALSE;
                }
        bool can_paste = (clipboard_obj.count() > 0) || (clipboard_comp.count());
        if ((clipboard_doc_type != doc_type()) || (clipboard_file_type != file_type))
                can_paste = FALSE;
        edit_select_allAct->setEnabled(GS->all_obj.count() > 0);
        if (only_one && no_ident_or_value())
                {
                bool no_sch_connection = TRUE;
                if (GV->is_sch)
                       if (GV->selected_obj.count())
                               if (GV->selected_obj.first()->tipo == O_CONNECTION)
                                       no_sch_connection = FALSE;
                edit_propertiesAct->setEnabled(no_sch_connection);
                }
        else
                edit_propertiesAct->setEnabled(FALSE);
        edit_deleteAct->setEnabled(selected_num);
        edit_duplicateAct->setEnabled(selected_num);
        edit_copyAct->setEnabled(selected_num);
        edit_cutAct->setEnabled(selected_num);
        edit_pasteAct->setEnabled(can_paste);
        edit_change_sideAct->setEnabled(!GV->is_sch && (num_obj == 0) && (num_comp >= 1));
        edit_selectTAct->setEnabled(only_one_connection);
        edit_selectCAct->setEnabled(!GV->is_sch && only_one_connection); //Nello schematico è pericoloso consentire la selezione delle connected tracks e della net
        edit_selectNAct->setEnabled(!GV->is_sch && only_one_connection); //Nello schematico è pericoloso consentire la selezione delle connected tracks e della net
        actions_wire_widthAct->setEnabled(only_pcb_connections);
        actions_mirror_hAct->setEnabled(GV->is_sch && (selected_num > 0));
        actions_mirror_vAct->setEnabled(GV->is_sch && (selected_num > 0));
        actions_rotate_rAct->setEnabled(selected_num > 0);
        actions_rotate_lAct->setEnabled(selected_num > 0);
        actions_moveToAct->setEnabled(!GV->is_sch && (selected_num > 0));
        transformMenu->setEnabled(selected_num > 0);
}

void win_main::change_file_type(char new_file_type)
{
        file_type = new_file_type;

        switch (file_type)
        {
        case FT_NULL:
                setWindowTitle("Itecad");
                setWindowModified(FALSE);

                //Disabilito le azioni di modo che nelle toolbar non siano attivabili
                file_closeAct->setEnabled(FALSE);
                file_saveAct->setEnabled(FALSE);
                file_save_asAct->setEnabled(FALSE);
                file_printAct->setEnabled(FALSE);
                file_print_to_pdfAct->setEnabled(FALSE);
                documents_update_netlistAct->setEnabled(FALSE);
                documents_realtime_drcAct->setEnabled(FALSE);
                documents_insulation_checkAct->setEnabled(FALSE);
                actions_zoomAct->setEnabled(FALSE);
                actions_zoom_to_allAct->setEnabled(FALSE);
                actions_selectAct->setEnabled(FALSE);
                actions_handAct->setEnabled(FALSE);
                actions_connectionAct->setEnabled(FALSE);
                actions_edit_nodeAct->setEnabled(FALSE);
                actions_polylineAct->setEnabled(FALSE);
                actions_circleAct->setEnabled(FALSE);
                actions_rectangleAct->setEnabled(FALSE);
                actions_polygonAct->setEnabled(FALSE);
                actions_textAct->setEnabled(FALSE);
                actions_pinAct->setEnabled(FALSE);
                actions_padviaAct->setEnabled(FALSE);
                actions_addAct->setEnabled(FALSE);
                actions_portAct->setEnabled(FALSE);
                actions_edit_componentAct->setEnabled(FALSE);
                actions_add_powerAct->setEnabled(FALSE);
                actions_setOriginAct->setEnabled(FALSE);
                actions_rulerAct->setEnabled(FALSE);

                //Disabilito la StatusBar e lo Zoom
                sb_grid->setEnabled(FALSE);
                sb_style->setEnabled(FALSE);
                sb_um->setEnabled(FALSE);
                Z->setEnabled(FALSE);

                menuBar()->clear();
                menuBar()->addMenu(fileMenu);
                menuBar()->addMenu(toolsMenu);
                menuBar()->addMenu(helpMenu);

                tw = 0;
                GV = 0;
                GS = 0;
                break;
        case FT_PROJECT:
        case FT_SCH_SYMBOL:
        case FT_PCB_SYMBOL:
                //Abilito le azioni di modo che nelle toolbar siano attivabili
                file_closeAct->setEnabled(TRUE);
                file_save_asAct->setEnabled(TRUE);
                file_printAct->setEnabled(TRUE);
                file_print_to_pdfAct->setEnabled(TRUE);
                actions_zoomAct->setEnabled(TRUE);
                actions_zoom_to_allAct->setEnabled(TRUE);
                actions_selectAct->setEnabled(TRUE);
                actions_handAct->setEnabled(TRUE);
                actions_edit_nodeAct->setEnabled(TRUE);
                actions_polylineAct->setEnabled(TRUE);
                actions_circleAct->setEnabled(TRUE);
                actions_rectangleAct->setEnabled(TRUE);
                actions_polygonAct->setEnabled(TRUE);
                actions_textAct->setEnabled(TRUE);
                if (file_type == FT_PROJECT)
                        {
                        actions_addAct->setEnabled(TRUE);
                        edit_findAct->setEnabled(TRUE);
                        documentMenu->setTitle(tr("&Document"));
                        }
                else if (file_type == FT_SCH_SYMBOL)
                        {
                        actions_addAct->setEnabled(FALSE);
                        edit_findAct->setEnabled(FALSE);
                        documentMenu->setTitle(tr("&Symbol"));
                        }
                else if (file_type == FT_PCB_SYMBOL)
                        {
                        actions_addAct->setEnabled(FALSE);
                        edit_findAct->setEnabled(FALSE);
                        documentMenu->setTitle(tr("&Footprint"));
                        }

                //Abilito la StatusBar e lo Zoom
                sb_grid->setEnabled(TRUE);
                sb_style->setEnabled(TRUE);
                sb_um->setEnabled(TRUE);
                Z->setEnabled(TRUE);
                break;
        }
}

void win_main::TE_popup_menu(const QPoint &pos)
{
	editorMenu->popup(QCursor::pos());	
}

void win_main::edit_node_delete()
{
	GV->setUpdatesEnabled(FALSE);
        GV->select_clear();
	
        GV->ur.beginMacro(edit_node_deleteAct->text());
        if (GV->wand_connection)
                {
                if ((GV->wand_connection_node->connected_wires.count() == 2) && (!GV->wand_connection_node->connected_pin_or_pad_or_via))
                        {
                        //Se ci sono solo due linee che vanno a questo nodo allora ...
                        myGraphicsItem *wire1 = (myGraphicsItem *) GV->wand_connection_node->connected_wires.first();
                        myGraphicsItem *wire2 = (myGraphicsItem *) GV->wand_connection_node->connected_wires.last();
                        myGraphicsItem *new_wire = new myGraphicsItem(O_CONNECTION);
                        new_wire->layer_from = wire1->layer_from;
                        new_wire->layer_to = wire1->layer_to;
                        new_wire->pen_width = wire1->pen_width;
                        if (wire1->node_start != GV->wand_connection_node)
                                new_wire->node_start = wire1->node_start;
                        else
                                new_wire->node_start = wire1->node_end;
                        if (wire2->node_start != GV->wand_connection_node)
                                new_wire->node_end = wire2->node_start;
                        else
                                new_wire->node_end = wire2->node_end;
                        new_wire->info_update();
                        GV->ur.push(new add_object(new_wire));
                        GV->select_clear();
                        }

                foreach (int obj, GV->wand_connection_node->connected_wires)
                        {
                        myGraphicsItem *wire = (myGraphicsItem *) obj;
                        wire->set_selected(TRUE);
                        }
                GV->selection_nodes();
                GV->ur.push(new delete_selection());
                GV->wand_connection = 0;
                GV->wand_connection_node = 0;
                }
        if (GV->wand_pol)
                {
                //Ci sono sempre più di due nodi altrimenti il menu non sarebbe stato abilitato
                GV->ur.push(new pol_del_node(GV->wand_pol, GV->wand_pol_index));
                GV->wand_pol = 0;
                }
	GV->ur.endMacro();
        GV->setUpdatesEnabled(TRUE);
}

void win_main::edit_node_insert()
{
        GV->setUpdatesEnabled(FALSE);
        GV->ur.beginMacro(edit_node_insertAct->text());
        char pcb_grid_snap_temp = pcb_grid_snap;
        pcb_grid_snap = 'G';
        QPoint p = GV->mapToScene(GV->mapFromGlobal(GV->wand_cursor_pos)).toPoint();
        GV->mouse_snap(p, &mouse_new_press, &mouse_new_press_node);
        pcb_grid_snap = pcb_grid_snap_temp;
        if (GV->wand_connection)
                {
                int radius = 1;
                while (mouse_new_press_node)
                        {
                        //Il punto scelto è già occupato da un nodo. Conviene cercarne un altro
                        char pcb_grid_snap_temp = pcb_grid_snap;
                        pcb_grid_snap = 'G';
                        QPoint r = QPoint(-radius/2, -radius/2) + QPoint(rand() % radius, rand() % radius);
                        p = GV->mapToScene(GV->mapFromGlobal(GV->wand_cursor_pos + r)).toPoint();
                        GV->mouse_snap(p, &mouse_new_press, &mouse_new_press_node);
                        pcb_grid_snap = pcb_grid_snap_temp;
                        radius++;
                        }
                node_data *node = GV->node_new();
                node->point = mouse_new_press;
                GV->ur.push(new add_node(node));
                GV->ur.push(new connection_insert_node(GV->wand_connection, node));
                }
        if (GV->wand_pol)
                GV->ur.push(new pol_insert_node(GV->wand_pol, GV->wand_pol_index, mouse_new_press));
        GV->ur.endMacro();
        GV->setUpdatesEnabled(TRUE);
}

void win_main::edit_component_rotate_r()
{
        myGraphicsItem *obj;
        if (GV->wand_ident)
                obj = GV->wand_ident;
        else
                obj = GV->wand_value;

        GV->setUpdatesEnabled(FALSE);
        GV->ur.beginMacro(edit_component_rotate_rAct->text());
        GV->ur.push(new rotate_obj(obj->start, obj, TRUE));
        GV->ur.endMacro();
        GV->setUpdatesEnabled(TRUE);
}

void win_main::edit_component_rotate_l()
{
        myGraphicsItem *obj;
        if (GV->wand_ident)
                obj = GV->wand_ident;
        else
                obj = GV->wand_value;

        GV->setUpdatesEnabled(FALSE);
        GV->ur.beginMacro(edit_component_rotate_lAct->text());
        GV->ur.push(new rotate_obj(obj->start, obj, FALSE));
        GV->ur.endMacro();
        GV->setUpdatesEnabled(TRUE);
}

void win_main::edit_component_pad()
{
        myGraphicsItem *obj = GV->wand_pad;

        win_object_prop dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, obj);
        switch (dialogo.exec())
        {
        case QDialog::Accepted:
                if (dialogo.is_modified())
                        {
                        GV->setUpdatesEnabled(FALSE);
                        GV->ur.beginMacro(tr("Change Pad Properties"));
                        GV->ur.push(new change_obj_prop(obj, (int) &dialogo));
                        GV->ur.endMacro();
                        GV->setUpdatesEnabled(TRUE);
                        }
                break;
        case QDialog::Rejected:
                break;
        }
}

void win_main::workspace_set_wire_width()
{
        for (int i = 0; i < doc_sch.count(); i++)
                foreach (myGraphicsItem *obj, doc_sch.at(i)->scene->all_obj)
                        if (obj->tipo == O_CONNECTION)
                                {
                                obj->pen_width = style_sch_pen_width_wire;
                                obj->info_update();
                                }
}

