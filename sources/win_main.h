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

#ifndef WINDOW_MAIN_H
    #define WINDOW_MAIN_H

        #include <QDebug>
        #include <QProcess>
        #include <QMainWindow>
        #include <QToolBar>
        #include <QToolButton>
        #include <QStatusBar>
        #include <QLabel>
        #include <QComboBox>
        #include <QTabWidget>
        #include <QDesktopServices>
        #include <QUrl>
        #include <QHash>
        #include "tipi_dato.h"
        #include "utilities.h"
        #include "win_help_about.h"
        #include "win_edit_layers.h"
        #include "myGraphicsView.h"
        #include "myGraphicsScene.h"
        #include "myComponent.h"
        #include "widget_zoom.h"
        #include "win_drc.h"
        #include "win_converter.h"
        #include "win_add_component.h"
        #include "win_options.h"
        #include "win_settings.h"
        #include "win_bill_of_materials.h"
        #include "win_page.h"
        #include "win_undo_redo_history.h"
        #include "win_doc_manager.h"
        #include "win_xml_editor.h"
        #include "win_char_editor.h"
        #include "xml_sax_generator.h"
        #include "xml_sax_handler.h"
        #include "preview.h"
        #include "win_style.h"
        #include "drc.h"
        #include "win_move_to.h"
        #include "win_report.h"

	class win_main : public QMainWindow
    {
		 Q_OBJECT	//Necessario per gestire Signals e Slots
	public:
                win_main();
		~win_main();

                void	setup_actions();
                void	setup_menuBar();
		void	setup_toolBar();
                void	setup_statusBar();

		void	recent_file_insert(QString f_path);
		void	recent_file_update();

                bool	ask_if_save();
                void	file_o(QString &f_path);
		void	file_s(QString &f_path);
		void	file_c();

                void    check_data_integrity();

                void	idstamp_refresh_pointer();

                int     	find_comp_doc(QString ident, bool is_sch);
                bool		is_available_ident(QString ident, bool check_cloned);
                QString		get_first_available_ident(QString alias, bool check_cloned);
                int		get_first_available_pin(bool check_cloned);
                int		get_first_available_pad(bool check_cloned);

                void		net_id_update_all_sch();
                void		net_id_update_pads_from_pins();
                void		net_id_update_pcb();
		
                void		ratsnest_update();
                void		ratsnest_add(int net_id);
                void		ratsnest_remove(int net_id);

                void		dynamic_ratsnest_setup(node_data *node);
                void		dynamic_ratsnest_update(node_data *node);
		
                void		group_command_add_connected_nodes_sch(QList<node_data *> &ng, node_data *node);
                void		group_command_set_net_id(node_data *nd, int net_id);
                void		group_command_junction_highlight(myGraphicsItem *wire, QRect *rect);

                void		track_select(myGraphicsItem *wire, bool connected_tracks);

                void		clean_up_pcb_all();

                void		disconnect_pcb_selection(); //E' in pratica un clean_up
                void            disconnect_sch_selection();
                bool		disconnect_selection_executed;

                void		connect_sch_selection();
                                //Il connect_pcb_selection() è meglio non farlo perchè è quasi sempre inutile, a volte dannoso ed è molto difficile da realizzarsi

                QString	documents_new_name(char type);
		bool	documents_valid_name(QString name, int rename_row);
		void	documents_add_new(char type, QString name);
                void	documents_remove(int index);

                void	layer_set_current();

                char			doc_type(QWidget *doc);
                char			doc_type();
                QTabWidget		*tw;

		bool	char_open(QString file, char_symbol *cs, bool warning);
		bool	char_save(QString file, char_symbol *cs);
		QHash<int, char_symbol *> alphabet;
		void	alphabet_load();

                QList<myGraphicsItem *>	proposed_selection_obj;
		QList<myComponent *>	proposed_selection_comp;
                void			proposed_selection_clear();
                void			proposed_selection_insert_obj(myGraphicsItem *obj);
                int			proposed_selection_count();
                bool			proposed_selection_one_is_selected();
                void                    proposed_selection_choose_cyclic();
                void                    proposed_selection_choose_first();

                bool                    no_ident_or_value();

                void                    settings_factory2temp();
                void                    settings_generaldefault2temp();
                void                    settings_temp2generaldefault();
                void                    settings_current2temp();
                void                    settings_temp2current();

                void                    page_numbers_update();

                void                    workspace_set_wire_width();

                QMenu *popupSCHEditMenu;
                QMenu *popupPCBEditMenu;
		QMenu *popupTransformMenu;
		QMenu *popupSCHwireMenu;
		QMenu *popupPCBwireMenu;
                QMenu *popupEditNodeMenu;
                QMenu *popupEditComponentMenu;
                QMenu *transformMenu;
                QMenu *toolBarEditNodeMenu;
                QMenu *toolBarDrag_or_moveMenu;
                QMenu *fileMenu;
		QMenu *editMenu;
                QMenu *editorMenu;
		QMenu *projectMenu;
		QMenu *documentMenu;
                QMenu *outputMenu;
		QMenu *toolsMenu;
                QMenu *helpMenu;

		QAction *file_newAct;
		QAction *file_new_prjAct;
		QAction *file_new_ssfAct;
		QAction *file_new_pffAct;
		QAction *file_openAct;
                QAction *file_open_prjAct;
		QAction *file_open_ssfAct;
		QAction *file_open_pffAct;
		QAction *file_closeAct;
		QAction *file_saveAct;	
                QAction *file_save_asAct;
		QAction *file_printAct;
		QAction *file_print_to_pdfAct;
		QAction *file_exitAct;
		QAction *file_separatorAct;
		QAction *file_recent_1_Act;
		QAction *file_recent_2_Act;
		QAction *file_recent_3_Act;
		QAction *file_recent_4_Act;                
                QAction *edit_undoAct;
                QAction *edit_redoAct;
		QAction *edit_undo_redo_historyAct;
                QAction *edit_cutAct;
                QAction *edit_copyAct;
                QAction *edit_pasteAct;
                QAction *edit_selectTAct;
                QAction *edit_selectCAct;
                QAction *edit_selectNAct;
                QAction *edit_select_allAct;
		QAction *edit_propertiesAct;
		QAction *edit_deleteAct;
		QAction *edit_duplicateAct;		
                QAction *edit_findAct;
		QAction *edit_change_sideAct;
                QAction *editor_findAct;
		QAction *editor_styleAct;
		QAction *project_documents_managerAct;
                QAction *documents_layersAct;
                QAction *documents_artistic_gridAct;
                QAction *documents_gridAct;
		QAction *documents_styleAct;
                QAction *documents_coloursAct;
                QAction *documents_workspaceAct;
		QAction *documents_pageAct;
		QAction *documents_realtime_drcAct;
		QAction *documents_insulation_checkAct;
		QAction *documents_clean_upAct;
		QAction *documents_update_netlistAct;
		QAction *documents_component_propertiesAct;
		QAction *documents_reportAct;
		QAction *layers_editAct;	
		QAction *actions_mode_dragAct;	
		QAction *actions_mode_moveAct;	
                QAction *actions_edit_node_all_layersAct;
                QAction *actions_edit_node_active_layerAct;
		QAction *actions_escapeAct;
		QAction *actions_polylineAct;
		QAction *actions_polygonAct;
		QAction *actions_circleAct;
		QAction *actions_handAct;
		QAction *actions_selectAct;
                QAction *actions_edit_nodeAct;
		QAction *actions_addAct;
		QAction *actions_connectionAct;
                QAction *actions_rectangleAct;
		QAction *actions_textAct;
		QAction *actions_pinAct;
                QAction *actions_padviaAct;
		QAction *actions_portAct;
                QAction *actions_setOriginAct;
                QAction *actions_rulerAct;
                QAction *actions_add_powerAct;
                QAction *actions_edit_componentAct;
		QAction *actions_zoomAct;
		QAction *actions_zoom_to_allAct;
                QAction *actions_wire_widthAct;
                QAction	*actions_moveToAct;
		QAction *actions_rotate_rAct;
		QAction *actions_rotate_lAct;
		QAction *actions_mirror_hAct;
		QAction *actions_mirror_vAct;
                QAction *output_spiceAct;
		QAction *output_bomAct;
                QAction *output_gerberAct;
                QAction *output_drillAct;
                QAction *output_board_and_drillsAct;
                QAction *tools_optionsAct;
                QAction *tools_general_default_settingsAct;
		QAction *tools_xml_editorAct;
		QAction *tools_char_editorAct;
		QAction *tools_length_unit_converterAct;
                QAction *help_user_manualAct;
                QAction *help_webAct;
		QAction *help_emailAct;
		QAction *help_aboutAct;

		QAction *connection_add_viaAct;
		QAction *connection_finish_hereAct;
		QAction *connection_finish_on_viaAct;
		QAction *connection_exitAct;

                QAction *edit_node_deleteAct;
                QAction *edit_node_insertAct;

                QAction *edit_component_rotate_rAct;
                QAction *edit_component_rotate_lAct;
                QAction *edit_component_padAct;

                QAction *mainwindow_select_tabAct;
                QAction *mainwindow_select_spacebarAct;
                QAction *mainwindow_panUpAct;
                QAction *mainwindow_panDownAct;
                QAction *mainwindow_panLeftAct;
                QAction *mainwindow_panRightAct;
                QAction *mainwindow_zoomInAct;
                QAction *mainwindow_zoomOutAct;

                QToolBar *toolbar_standard;
		QToolBar *toolbar_drawing;
		QToolBar *toolbar_zoom;

                QStatusBar	*sb;
                QComboBox       *sb_layer;
                QToolButton	*sb_grid;
		QToolButton	*sb_style;
                QLabel          *sb_x;
                QLabel          *sb_y;
                QLabel          *sb_deltax;
                QLabel          *sb_deltay;
                QComboBox       *sb_um;

                Zoom	*Z;

                bool	menu_documents_need_an_update;

                char	tool_sch;
		char	tool_pcb;
		char	tool_avviato;

		bool	never_saved;

                bool	is_anchor_point_correct();

                QProcess        *help_user_manual_proc;

        public slots:
                void file_close();
                void file_new_prj();
		void file_new_ssf();
		void file_new_pff();
                void file_open_prj();
		void file_open_ssf();
		void file_open_pff();
                void file_print();
                void file_print_to_pdf();
                void file_save();
                void file_save_as();
                void file_recent_1();
		void file_recent_2();
		void file_recent_3();
		void file_recent_4();
                void edit_undo();
                void edit_redo();
                void edit_undo_redo_history();
                void edit_can_undo_changed(bool new_value);
		void edit_can_redo_changed(bool new_value);
                void edit_cut();
                void edit_copy();
                void edit_paste();
                void edit_properties();
		void edit_delete();
		void edit_duplicate();
                void edit_select_t();
                void edit_select_c();
                void edit_select_n();
		void edit_select_all();
                void edit_find();
		void edit_change_side();
                void editor_find();
                void editor_style();
                void project_documents_manager();
                void documents_layers();
                void documents_artistic_grid();
                void documents_grid();
		void documents_style();	
                void documents_colours();
                void documents_workspace();
		void documents_page();	
                void documents_realtime_drc();
		void documents_insulation_check();
		void documents_clean_up();	
		void documents_update_netlist();
                void documents_component_properties();
                void documents_report();
                void actions_mode_drag();
		void actions_mode_move();
                void actions_edit_node_all_layers();
                void actions_edit_node_active_layer();
                void actions_edit_component_rotate_r();
                void actions_edit_component_rotate_l();
                void actions_escape();
		void actions_polyline();
		void actions_circle();
		void actions_rectangle();
		void actions_polygon();
		void actions_hand();
		void actions_add();
		void actions_connection();
                void actions_select();
                void actions_edit_node();
		void actions_text();
		void actions_pin();
                void actions_padvia();
		void actions_port();
                void actions_set_origin();
                void actions_ruler();
                void actions_add_power();
                void actions_edit_component();
		void actions_zoom();
		void actions_zoom_to_all();
                void actions_move_to();
		void actions_rotate_r();
		void actions_rotate_l();
		void actions_mirror_h();
		void actions_mirror_v();
                void actions_wire_width();
                void output_spice();
		void output_bom();
                void output_gerber();
                void output_drill();
                void output_board_and_drills();
                void tools_options();
                void tools_general_default_settings();
		void tools_length_unit_converter();
		void tools_xml_editor();
                void tools_char_editor();
                void help_user_manual();
                void help_web();
                void help_email();
		void help_about();
                void edit_node_delete();
                void edit_node_insert();
                void edit_component_rotate_r();
                void edit_component_rotate_l();
                void edit_component_pad();

                void mainwindow_select_tab();
                void mainwindow_select_spacebar();
                void mainwindow_panUp();
                void mainwindow_panDown();
                void mainwindow_panLeft();
                void mainwindow_panRight();
                void mainwindow_zoom_in();
                void mainwindow_zoom_out();
		
		void sb_refresh();
                void sb_layer_changed(int index);
		void sb_grid_pressed();
		void sb_grid_tooltip();
		void sb_style_pressed();
		void sb_style_tooltip();
		void sb_um_change(int indice);

                void menu_file_open();
                void menu_project_open();
		void menu_project_esegui_voce();
		void menu_document_open();		
                void menu_edit_node_open();
                void menu_edit_component_open();

                void TE_popup_menu(const QPoint &pos);

                bool	connection_add_segment();
                bool    connection_check_junction(node_data *node);
		void	connection_add_via();
		void	connection_finish_here();
		void	connection_finish_on_via();
		void	connection_exit();

                void	line_exit();

                void	change_file_type(char new_file_type);
                void    change_document(int doc);
                void    change_selection();
		
	protected:
                virtual void	closeEvent(QCloseEvent *e);

    private:

	private slots:

    };

#endif
