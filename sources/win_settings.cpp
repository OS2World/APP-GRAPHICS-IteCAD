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

#include "win_settings.h"

extern	char	file_type;
extern	win_main	*w_main;
extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

extern	lay_data	layer_data[LAYER_DATA_COUNT];
extern	lay_data	workspace_data[WORKSPACE_DATA_COUNT];

extern	UM_description	UM_descriptions[UM_DESCRIPTION_COUNT];

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
extern  int             style_pcb_pad_layer_from;
extern  int             style_pcb_pad_layer_to;
extern  int             style_pcb_pad_drill;
extern  int             style_pcb_via_width;
extern  int             style_pcb_via_layer_from;
extern  int             style_pcb_via_layer_to;
extern  int             style_pcb_via_drill;

extern  qreal		sch_grid_artistic;

extern  bool		pcb_grid_visible;
extern  qreal		pcb_grid_size;
extern  bool            pcb_grid_thou;
extern  char		pcb_grid_snap;

extern  bool            pcb_pad_via_filled;

extern  factory_value   factory_values[FACTORY_VALUE_COUNT];

extern  myGraphicsView	*doc_pcb;

extern  QRegExp         exp_length;

win_settings::win_settings(QWidget *parent, Qt::WFlags tipo, char tab_index)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

        ti = tab_index;
        if (!ti)
                {
                w_main->settings_generaldefault2temp();
                setWindowTitle(tr("New Document Default Settings"));
                }
        else
                {
                w_main->settings_current2temp();                
                }
	//setSizeGripEnabled(true);
        setModal(true);

	//Imposto la larghezza minima pari a metà dello schermo
	//QRect screen = QApplication::desktop()->screenGeometry();	//Non serve perchè uso tw->setUsesScrollButtons(FALSE); 
        //setMinimumWidth(screen.width() / 2);				//Non serve perchè uso tw->setUsesScrollButtons(FALSE);

        QGridLayout	*layout = new QGridLayout(this);

        if (!ti)
                {
                tw = new QTabWidget();
                tw->setUsesScrollButtons(FALSE);
                tw->addTab(setup_sch_drawing_style(), tr("SCH Drawing Style"));
                tw->addTab(setup_sch_artistic_grid(), tr("SCH Artistic Grid"));
                tw->addTab(setup_sch_colours(), tr("SCH Colours"));
                tw->addTab(setup_sch_workspace(), tr("SCH Workspace"));
                tw->addTab(setup_pcb_drawing_style(), tr("PCB Drawing Style"));
                tw->addTab(setup_pcb_grid(), tr("PCB Grid"));
                tw->addTab(setup_pcb_colours(), tr("PCB Colours"));
                tw->addTab(setup_pcb_workspace(), tr("PCB Workspace"));
                layout->addWidget(tw,1,0);
                }
        else
                switch (ti)
                {
                case 1:
                        layout->addWidget(setup_sch_drawing_style(), 1,0);
                        setWindowTitle(tr("Drawing Style"));
                        break;
                case 2:
                        layout->addWidget(setup_sch_artistic_grid(), 1,0);
                        setWindowTitle(tr("Artistic Grid"));
                        break;
                case 3:
                        layout->addWidget(setup_sch_colours(), 1,0);
                        setWindowTitle(tr("Colours"));
                        break;
                case 4:
                        layout->addWidget(setup_sch_workspace(), 1,0);
                        setWindowTitle(tr("Workspace"));
                        break;
                case 5:
                        layout->addWidget(setup_pcb_drawing_style(), 1,0);
                        setWindowTitle(tr("Drawing Style"));
                        break;
                case 6:
                        layout->addWidget(setup_pcb_grid(), 1,0);
                        setWindowTitle(tr("Grid"));
                        break;
                case 7:
                        layout->addWidget(setup_pcb_colours(), 1,0);
                        setWindowTitle(tr("Colours"));
                        break;
                case 8:
                        layout->addWidget(setup_pcb_workspace(), 1,0);
                        setWindowTitle(tr("Workspace"));
                        break;
                }

        layout->addLayout(setup_buttons(),2,0);
        layout->setSpacing(6);
        layout->setMargin(8);

        load_data();

        if ((ti == 0) || (ti == 3) || (ti == 7))
                {
                // Definisco il Timer	//RICORDARSI DI ELIMINARE IL TIMER CON KILLTIMER
                timer_selection = new QTimer( this ); // create internal timer
                connect( timer_selection, SIGNAL(timeout()), SLOT(timeout()));
                timer_selection->start( 200 );	//Ogni 2 decimi di secondi avvio il timeout
                }
}

void    win_settings::grid_UM_changed(int inutile)
{
        int old_index = grid_size->currentIndex();
        grid_size->clear();
        if (grid_UM->currentIndex() == 0)
                {
                grid_size->addItem(QString::number(100));
                grid_size->addItem(QString::number(50));
                grid_size->addItem(QString::number(25));
                grid_size->addItem(QString::number(12.5));
                grid_size->addItem(QString::number(10));
                grid_size->addItem(QString::number(5));
                grid_size->addItem(QString::number(2.5));
                grid_size->addItem(QString::number(1.25));
                }
        else
                {
                grid_size->addItem(QString::number(5));
                grid_size->addItem(QString::number(2.5));
                grid_size->addItem(QString::number(2));
                grid_size->addItem(QString::number(1));
                grid_size->addItem(QString::number(0.5));
                grid_size->addItem(QString::number(0.25));
                grid_size->addItem(QString::number(0.1));
                grid_size->addItem(QString::number(0.05));
                }
        if (old_index != -1)
                grid_size->setCurrentIndex(old_index);
        else
                grid_size->setCurrentIndex(0);
}

QWidget *win_settings::setup_pcb_grid()
{
	QWidget *w = new QWidget();

	QGroupBox *gb_grid = new QGroupBox(tr("Grid"));
	QGridLayout *layout_grid = new QGridLayout();		
	visible = new QCheckBox(tr("Visible"));
        visible->setChecked(pcb_grid_visible);
        grid_size = new QComboBox();
        grid_size->setEditable(TRUE);
        grid_size->setValidator(new QRegExpValidator(exp_length, this));  //Uso new perchè ogni volta bisogna ricreare QRegExpValidator (non si può utilizzare l'oggetto altrove...)
        grid_size->setEditText(QString::number(pcb_grid_size));
        grid_UM = new QComboBox();
        grid_UM->addItem("thou");
        grid_UM->addItem("mm");

        //Prima bisogna inserire il valore di grid_UM e dopo quello grid_size
        if (pcb_grid_thou)
                grid_UM->setCurrentIndex(0);
        else
                grid_UM->setCurrentIndex(1);
        grid_UM_changed(0);
        for (int i = 0; i < grid_size->count(); i++)
                {
                if (grid_size->itemText(i).toFloat() == pcb_grid_size)
                        grid_size->setCurrentIndex(i);
                }
        connect(grid_UM, SIGNAL(currentIndexChanged(int)), this, SLOT(grid_UM_changed(int)));

	layout_grid->addWidget(visible, 0, 0);
        layout_grid->addWidget(grid_size, 1, 0);
        layout_grid->addWidget(grid_UM, 1, 1);
	layout_grid->setRowStretch(2, TRUE);
        layout_grid->setColumnStretch(3, TRUE);
	gb_grid->setLayout(layout_grid);

        QGroupBox *gb_snap = new QGroupBox(tr("Snap"));
	QGridLayout *layout_snap = new QGridLayout();		
	snap_none = new QRadioButton(tr("None"));
	snap_grid = new QRadioButton(tr("Grid"));
        snap_nodes_and_grid = new QRadioButton(tr("Objects or Grid"));
        /*switch (pcb_grid_snap)
	{
	case 'N':
		snap_none->setChecked(TRUE);
		break;
	case 'G':
		snap_grid->setChecked(TRUE);
		break;
	case 'I':
                snap_nodes_and_grid->setChecked(TRUE);
		break;
        }*/
	layout_snap->addWidget(snap_none, 0, 0);
	layout_snap->addWidget(snap_grid, 1, 0);
        layout_snap->addWidget(snap_nodes_and_grid, 2, 0);
	gb_snap->setLayout(layout_snap);

        QGridLayout *layout = new QGridLayout();
	layout->addWidget(gb_grid, 0, 0);
        layout->addWidget(gb_snap, 1, 0);
        if (ti)
                layout->setMargin(0);
        w->setLayout(layout);
	
	return w;
}

QWidget *win_settings::setup_sch_artistic_grid()
{
        QWidget *w = new QWidget();

        QGroupBox *gb_grid = new QGroupBox(tr("Artistic Grid"));
        QGridLayout *layout_grid = new QGridLayout();
        grid_artistic = new QComboBox();
        grid_artistic->setEditable(FALSE);
        grid_artistic->addItem("1/1   pin grid (100 thou)");
        grid_artistic->addItem("1/2   pin grid (50 thou) ");
        grid_artistic->addItem("1/4   pin grid (25 thou)");
        grid_artistic->addItem("1/8   pin grid (12.5 thou)");
        grid_artistic->addItem("1/16  pin grid (6.25 thou)");
        QLabel *grid_artistic_label = new QLabel(tr("Pin grid is fixed at 100 thou (0.1\")."));

        layout_grid->addWidget(grid_artistic, 0, 0);
        layout_grid->addWidget(grid_artistic_label, 1, 0);
        layout_grid->setRowStretch(2, TRUE);
        layout_grid->setColumnStretch(1, TRUE);
        gb_grid->setLayout(layout_grid);

        QGridLayout *layout = new QGridLayout();
        layout->addWidget(gb_grid, 0, 0);
        if (ti)
                layout->setMargin(0);
        w->setLayout(layout);

        return w;
}

QWidget *win_settings::setup_sch_workspace()
{
        QWidget *w = new QWidget();

        QGroupBox *gb_lines = new QGroupBox(tr("General"));

        QLabel *pen_width_wire = new QLabel(this);
        pen_width_wire->setText(tr("Wire Pen Width:"));
        pen_width_wire->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        Pen_width_wire = new layout_length("", 0, 0, 100000, TRUE);	//La larghezza 0 deve essere consentita (linee infinitesimali e visibili, anche nel PCB)
        QLabel *pen_width_frame = new QLabel(this);
        pen_width_frame->setText(tr("Frame Pen Width:"));
        pen_width_frame->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        Pen_width_frame = new layout_length("", 0, 0, 100000, TRUE);	//La larghezza 0 deve essere consentita (linee infinitesimali e visibili, anche nel PCB)
        QGridLayout *gbox_lines = new QGridLayout();
        gbox_lines->addWidget(pen_width_wire,0,0);
        gbox_lines->addLayout(Pen_width_wire,0,1);
        gbox_lines->addWidget(pen_width_frame,1,0);
        gbox_lines->addLayout(Pen_width_frame,1,1);
        gbox_lines->setRowStretch(3,TRUE);		//Lo stretch è come se fosse una molla
        gb_lines->setLayout(gbox_lines);

        QGridLayout *style = new QGridLayout();
        style->addWidget(gb_lines, 0, 0, TRUE, FALSE);
        if (ti)
                style->setMargin(0);
        w->setLayout(style);

        return w;
}

QWidget *win_settings::setup_pcb_workspace()
{
        QWidget *w = new QWidget();

        QGroupBox *gb_lines = new QGroupBox(tr("Draw Mode"));

        draw_mode_gerber = new QRadioButton(tr("Gerber Mode:\n- Like Gerber View"));
        draw_mode_draft = new QRadioButton(tr("Draft Mode:\n- Transparency in copper layers\n- Pads and Vias are sketched\n- Requires more graphics resources"));
        QGridLayout *gbox_lines = new QGridLayout();
        gbox_lines->addWidget(draw_mode_gerber,0,0);
        gbox_lines->addWidget(draw_mode_draft,1,0);
        gbox_lines->setRowStretch(2,TRUE);		//Lo stretch è come se fosse una molla
        gb_lines->setLayout(gbox_lines);

        QGridLayout *style = new QGridLayout();
        style->addWidget(gb_lines, 0, 0, TRUE, FALSE);
        if (ti)
                style->setMargin(0);
        w->setLayout(style);

        return w;
}


QWidget *win_settings::setup_sch_drawing_style()
{
	QWidget *w = new QWidget();

        QGroupBox *gb_lines = new QGroupBox(tr("Pen"));

        QLabel *pen_width_normal = new QLabel(this);
        pen_width_normal->setText(tr("Width:"));
        pen_width_normal->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        Pen_width_normal = new layout_length("", 0, 0, 100000, TRUE);//La larghezza 0 deve essere consentita (linee infinitesimali e visibili, anche nel PCB)
        QGridLayout *gbox_lines = new QGridLayout();
        gbox_lines->addWidget(pen_width_normal,0,0);
        gbox_lines->addLayout(Pen_width_normal,0,1);
        gbox_lines->setRowStretch(3,TRUE);		//Lo stretch è come se fosse una molla
        gb_lines->setLayout(gbox_lines);

	QGroupBox *gb_polygon = new QGroupBox(tr("Polygon"));
        sch_Fill_polygon = new QCheckBox(tr("Fill polygon?"));
        QGridLayout *gbox_polygon = new QGridLayout();
        gbox_polygon->addWidget(sch_Fill_polygon,0,0);
	gb_polygon->setLayout(gbox_polygon);

	QGroupBox *gb_text = new QGroupBox(tr("Text"));	
	
        sch_font_layout = new layout_font(10, FALSE, FALSE);

	QGridLayout *gbox_text = new QGridLayout();
        gbox_text->addLayout(sch_font_layout, 0, 0);
	gb_text->setLayout(gbox_text);

	QGridLayout *style = new QGridLayout();
        style->addWidget(gb_lines, 0, 0, TRUE, FALSE);
        style->addWidget(gb_text, 1, 0);
        style->addWidget(gb_polygon, 1, 1);
        if (ti)
                style->setMargin(0);
        w->setLayout(style);

	return w;
}

QWidget *win_settings::setup_pcb_drawing_style()
{
        QWidget *w = new QWidget();

        QGroupBox *gb_lines = new QGroupBox(tr("Pen Width"));

        QLabel *pen_width_copper = new QLabel(this);
        pen_width_copper->setText(tr("Copper Pen Width:"));
        pen_width_copper->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        Pen_width_copper = new layout_length("", style_pcb_pen_width_copper, 0, 100000, FALSE);         //La larghezza 0 deve essere consentita (linee infinitesimali e visibili, anche nel PCB)
        QLabel *pen_width_silk = new QLabel(this);
        pen_width_silk->setText(tr("Silk Pen Width:"));
        pen_width_silk->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        Pen_width_silk = new layout_length("", style_pcb_pen_width_silk, 0, 100000, FALSE);		//La larghezza 0 deve essere consentita (linee infinitesimali e visibili, anche nel PCB)
        QLabel *pen_width_paste = new QLabel(this);
        pen_width_paste->setText(tr("Paste Pen Width:"));
        pen_width_paste->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        Pen_width_paste = new layout_length("", style_pcb_pen_width_paste, 0, 100000, FALSE);		//La larghezza 0 deve essere consentita (linee infinitesimali e visibili, anche nel PCB)
        QLabel *pen_width_mask = new QLabel(this);
        pen_width_mask->setText(tr("Mask Pen Width:"));
        pen_width_mask->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        Pen_width_mask = new layout_length("", style_pcb_pen_width_mask, 0, 100000, FALSE);		//La larghezza 0 deve essere consentita (linee infinitesimali e visibili, anche nel PCB)
        QLabel *pen_width_board = new QLabel(this);
        pen_width_board->setText(tr("Board Pen Width:"));
        pen_width_board->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        Pen_width_board = new layout_length("", style_pcb_pen_width_board, 0, 100000, FALSE);		//La larghezza 0 deve essere consentita (linee infinitesimali e visibili, anche nel PCB)
        QLabel *pen_width_help = new QLabel(this);
        pen_width_help->setText(tr("Help Pen Width:"));
        pen_width_help->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        Pen_width_help = new layout_length("", style_pcb_pen_width_help, 0, 100000, FALSE);		//La larghezza 0 deve essere consentita (linee infinitesimali e visibili, anche nel PCB)
        QGridLayout *gbox_lines = new QGridLayout();
        gbox_lines->addWidget(pen_width_copper,0,0);
        gbox_lines->addLayout(Pen_width_copper,0,1);
        gbox_lines->addWidget(pen_width_silk,1,0);
        gbox_lines->addLayout(Pen_width_silk,1,1);
        gbox_lines->addWidget(pen_width_paste,2,0);
        gbox_lines->addLayout(Pen_width_paste,2,1);
        gbox_lines->addWidget(pen_width_mask,3,0);
        gbox_lines->addLayout(Pen_width_mask,3,1);
        gbox_lines->addWidget(pen_width_board,4,0);
        gbox_lines->addLayout(Pen_width_board,4,1);
        gbox_lines->addWidget(pen_width_help,5,0);
        gbox_lines->addLayout(Pen_width_help,5,1);
        gbox_lines->setRowStretch(6,TRUE);		//Lo stretch è come se fosse una molla
        gb_lines->setLayout(gbox_lines);

        QGridLayout *pad_and_via = new QGridLayout();

        QGroupBox *gb_pad = new QGroupBox(tr("Pad"));
        pad = new layout_pad_via(TRUE, style_pcb_pad_shape, style_pcb_pad_width, style_pcb_pad_height, style_pcb_pad_layer_from, style_pcb_pad_layer_to, style_pcb_pad_drill);
        gb_pad->setLayout(pad);
        pad_and_via->addWidget(gb_pad);

        QGroupBox *gb_via = new QGroupBox(tr("Via"));
        via = new layout_pad_via(FALSE, 0, style_pcb_via_width, 0, style_pcb_via_layer_from, style_pcb_via_layer_to, style_pcb_via_drill);
        gb_via->setLayout(via);
        pad_and_via->addWidget(gb_via);

        QGroupBox *gb_polygon = new QGroupBox(tr("Polygon"));
        pcb_Fill_polygon = new QCheckBox(tr("Fill polygon?"));
        pcb_Fill_polygon->setChecked(style_pcb_fill_polygon);
        QGridLayout *gbox_polygon = new QGridLayout();
        gbox_polygon->addWidget(pcb_Fill_polygon,0,0);
        gb_polygon->setLayout(gbox_polygon);

        QGroupBox *gb_text = new QGroupBox(tr("Text"));

        pcb_font_layout = new layout_font(style_sch_font_height, style_sch_font_bold, style_sch_font_italic);

        QGridLayout *gbox_text = new QGridLayout();
        gbox_text->addLayout(pcb_font_layout, 0, 0);
        gb_text->setLayout(gbox_text);

        QGridLayout *style = new QGridLayout();
        style->addWidget(gb_lines, 0, 0);
        style->addLayout(pad_and_via, 0, 1);
        style->addWidget(gb_text, 1, 0);
        style->addWidget(gb_polygon, 1, 1);
        if (ti)
                style->setMargin(0);
        w->setLayout(style);

        return w;
}

QWidget *win_settings::setup_sch_colours()
{
        table_sch_workspace = 0;
        table_sch_layers = 0;

        QWidget *w = new QWidget();
        QGridLayout *w_layout = new QGridLayout();

        w_layout->addWidget(setup_workspace(TRUE),0,0);
        w_layout->addWidget(setup_layers(TRUE),0,1);
        if (ti)
                w_layout->setMargin(0);
        w->setLayout(w_layout);

        return w;
}

QWidget *win_settings::setup_pcb_colours()
{
        table_pcb_workspace = 0;
        table_pcb_layers = 0;

        QWidget *w = new QWidget();
        QGridLayout *w_layout = new QGridLayout();

        w_layout->addWidget(setup_workspace(FALSE),0,0);
        w_layout->addWidget(setup_layers(FALSE),0,1);
        if (ti)
                w_layout->setMargin(0);
        w->setLayout(w_layout);

        return w;
}

QGroupBox *win_settings::setup_workspace(bool is_sch)
        {
                QGroupBox *groupBox;
                QTableWidget *tw;

                QTableWidgetItem *descr;
                QTableWidgetItem *colore;
                QHeaderView *vh;
                QHeaderView *hh;

                if (is_sch)
                        {
                        groupBox = new QGroupBox(tr("SCH workspace"));
                        table_sch_workspace = new QTableWidget(WORKSPACE_DATA_SCH_COUNT, 2, this);
                        tw = table_sch_workspace;
                        }
                else
                        {
                        groupBox = new QGroupBox(tr("PCB workspace"));
                        table_pcb_workspace = new QTableWidget(WORKSPACE_DATA_PCB_COUNT, 2, this);
                        tw = table_pcb_workspace;
                        }

                //tw->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                tw->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                tw->setFocusPolicy(Qt::NoFocus);
                tw->setSelectionMode(QAbstractItemView::NoSelection);	//Seleziono io dal codice, che è meglio
                vh = tw->verticalHeader();
                vh->setHidden(TRUE);
                hh = tw->horizontalHeader();
                hh->setHidden(TRUE);
                tw->setEditTriggers(QAbstractItemView::NoEditTriggers);	//Le caselle non sono editabili
                int	riga = 0;
                for (int i = 0; i < WORKSPACE_DATA_COUNT; ++i)
                        if (workspace_data[i].is_sch == is_sch)
                                {
                                descr = new QTableWidgetItem(workspace_data[i].user_description);
                                tw->setItem(riga,1, descr);
                                colore = new QTableWidgetItem("  ");
                                tw->setItem(riga,0, colore);
                                tw->resizeRowToContents(riga);
                                riga++;
                                }
                tw->resizeColumnToContents(1);
                //Purtroppo table_pcb_layers->resizeColumnToContents(0); non funziona...
                hh->resizeSection(0,24);
                hh->setStretchLastSection(TRUE);
                connect(tw, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(change_color(QTableWidgetItem *)));

                QGridLayout *gbox = new QGridLayout;
                gbox->addWidget(tw,0,0);//,TRUE,FALSE);

                groupBox->setLayout(gbox);

                return groupBox;
        }

QGroupBox *win_settings::setup_layers(bool is_sch)
        {
                QGroupBox *groupBox;
                QTableWidget *tw;

                QTableWidgetItem *descr;
                QTableWidgetItem *colore;
                QHeaderView *vh;
                QHeaderView *hh;

                if (is_sch)
                        {
                        groupBox = new QGroupBox(tr("SCH layers"));
                        table_sch_layers = new QTableWidget(LAYER_DATA_SCH_COUNT, 2, this);
                        tw = table_sch_layers;
                        }
                else
                        {
                        groupBox = new QGroupBox(tr("PCB layers"));
                        table_pcb_layers = new QTableWidget(LAYER_DATA_PCB_COUNT, 2, this);
                        tw = table_pcb_layers;
                        }

                //tw->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                tw->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                tw->setFocusPolicy(Qt::NoFocus);
                tw->setSelectionMode(QAbstractItemView::NoSelection);	//Seleziono io dal codice, che è meglio
                vh = tw->verticalHeader();
                vh->setHidden(TRUE);
                hh = tw->horizontalHeader();
                hh->setHidden(TRUE);
                tw->setEditTriggers(QAbstractItemView::NoEditTriggers);	//Le caselle non sono editabili
                int	riga = 0;
                for (int i = 0; i < LAYER_DATA_COUNT; ++i)
                        if (layer_data[i].is_sch == is_sch)
                                {
                                descr = new QTableWidgetItem(layer_data[i].user_description);
                                tw->setItem(riga,1, descr);
                                colore = new QTableWidgetItem("  ");
                                tw->setItem(riga,0, colore);
                                tw->resizeRowToContents(riga);
                                riga++;
                                }
                tw->resizeColumnToContents(1);
                //Purtroppo table_pcb_layers->resizeColumnToContents(0); non funziona...
                hh->resizeSection(0,24);
                hh->setStretchLastSection(TRUE);
                connect(tw, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(change_color(QTableWidgetItem *)));

                QGridLayout *gbox = new QGridLayout;
        gbox->addWidget(tw,0,0);//,TRUE,FALSE);

                groupBox->setLayout(gbox);

                return groupBox;
        }

QHBoxLayout	*win_settings::setup_buttons()
{
        QHBoxLayout	*L_comandi = new QHBoxLayout();
	L_comandi->setSpacing(6);
	L_comandi->setMargin(0);
        if (!ti)
                {
                QPushButton	*L_comandi_restore = new QPushButton(this);
                L_comandi_restore->setText(tr("Load Factory Settings"));
                QObject::connect(L_comandi_restore, SIGNAL(clicked()), this, SLOT(load_factory_settings()));
                L_comandi->addWidget(L_comandi_restore);
                L_comandi_restore->setAutoDefault(FALSE);
                }
        QSpacerItem	*L_comandi_molla = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        L_comandi->addItem(L_comandi_molla);
        QPushButton	*L_comandi_OK = new QPushButton(this);
	L_comandi_OK->setText(tr("Ok"));
        L_comandi->addWidget(L_comandi_OK);
	QPushButton	*L_comandi_Cancel = new QPushButton(this);
	L_comandi_Cancel->setText(tr("Cancel"));
        L_comandi->addWidget(L_comandi_Cancel);

	QObject::connect(L_comandi_OK, SIGNAL(clicked()), this, SLOT(accept()));
	QObject::connect(L_comandi_Cancel, SIGNAL(clicked()), this, SLOT(reject()));

        L_comandi_OK->setAutoDefault(TRUE);
	L_comandi_Cancel->setAutoDefault(FALSE);

	return L_comandi;
}

void    win_settings::load_data()
{
        if ((ti == 0) || (ti == 3))
                {
                // SCH Workspace
                table_sch_workspace->item(0,0)->setBackgroundColor(factory_values[0].temp.value<QColor>());
                table_sch_workspace->item(1,0)->setBackgroundColor(factory_values[1].temp.value<QColor>());
                table_sch_workspace->item(2,0)->setBackgroundColor(factory_values[2].temp.value<QColor>());
                table_sch_workspace->item(3,0)->setBackgroundColor(factory_values[3].temp.value<QColor>());
                }

        if ((ti == 0) || (ti == 7))
                {
                // PCB Workspace
                table_pcb_workspace->item(0,0)->setBackgroundColor(factory_values[4].temp.value<QColor>());
                table_pcb_workspace->item(1,0)->setBackgroundColor(factory_values[5].temp.value<QColor>());
                table_pcb_workspace->item(2,0)->setBackgroundColor(factory_values[6].temp.value<QColor>());
                table_pcb_workspace->item(3,0)->setBackgroundColor(factory_values[7].temp.value<QColor>());
                table_pcb_workspace->item(4,0)->setBackgroundColor(factory_values[8].temp.value<QColor>());
                table_pcb_workspace->item(5,0)->setBackgroundColor(factory_values[9].temp.value<QColor>());
                table_pcb_workspace->item(6,0)->setBackgroundColor(factory_values[10].temp.value<QColor>());
                table_pcb_workspace->item(7,0)->setBackgroundColor(factory_values[11].temp.value<QColor>());
                table_pcb_workspace->item(8,0)->setBackgroundColor(factory_values[12].temp.value<QColor>());
                table_pcb_workspace->item(9,0)->setBackgroundColor(factory_values[13].temp.value<QColor>());
                }

        if ((ti == 0) || (ti == 3))
                {
                // SCH Layers
                table_sch_layers->item(0,0)->setBackgroundColor(factory_values[14].temp.value<QColor>());
                table_sch_layers->item(1,0)->setBackgroundColor(factory_values[15].temp.value<QColor>());
                table_sch_layers->item(2,0)->setBackgroundColor(factory_values[16].temp.value<QColor>());
                }

        if ((ti == 0) || (ti == 7))
                {
                // PCB Layers
                table_pcb_layers->item(0,0)->setBackgroundColor(factory_values[17].temp.value<QColor>());
                table_pcb_layers->item(1,0)->setBackgroundColor(factory_values[18].temp.value<QColor>());
                table_pcb_layers->item(2,0)->setBackgroundColor(factory_values[19].temp.value<QColor>());
                table_pcb_layers->item(3,0)->setBackgroundColor(factory_values[20].temp.value<QColor>());
                table_pcb_layers->item(4,0)->setBackgroundColor(factory_values[21].temp.value<QColor>());
                table_pcb_layers->item(5,0)->setBackgroundColor(factory_values[22].temp.value<QColor>());
                table_pcb_layers->item(6,0)->setBackgroundColor(factory_values[23].temp.value<QColor>());
                table_pcb_layers->item(7,0)->setBackgroundColor(factory_values[24].temp.value<QColor>());
                table_pcb_layers->item(8,0)->setBackgroundColor(factory_values[25].temp.value<QColor>());
                table_pcb_layers->item(9,0)->setBackgroundColor(factory_values[26].temp.value<QColor>());
                }

        if ((ti == 0) || (ti == 1))
                {
                // SCH Style
                Pen_width_normal->setC(factory_values[27].temp.toInt());
                sch_Fill_polygon->setChecked(factory_values[30].temp.toBool());
                sch_font_layout->setHeight(factory_values[31].temp.toInt());
                sch_font_layout->setBold(factory_values[32].temp.toBool());
                sch_font_layout->setItalic(factory_values[33].temp.toBool());
                }

        if ((ti == 0) || (ti == 4))
                {
                // SCH Style
                Pen_width_frame->setC(factory_values[28].temp.toInt());
                Pen_width_wire->setC(factory_values[29].temp.toInt());
                }

        if ((ti == 0) || (ti == 5))
                {
                // PCB Style
                Pen_width_copper->setLength(factory_values[34].temp.toInt());
                Pen_width_silk->setLength(factory_values[35].temp.toInt());
                Pen_width_paste->setLength(factory_values[36].temp.toInt());
                Pen_width_mask->setLength(factory_values[37].temp.toInt());
                Pen_width_board->setLength(factory_values[38].temp.toInt());
                Pen_width_help->setLength(factory_values[39].temp.toInt());
                pcb_Fill_polygon->setChecked(factory_values[40].temp.toBool());
                pcb_font_layout->setHeight(factory_values[41].temp.toInt());
                pcb_font_layout->setBold(factory_values[42].temp.toBool());
                pcb_font_layout->setItalic(factory_values[43].temp.toBool());
                pad->setShape(factory_values[44].temp.toInt());
                pad->setWidth(factory_values[45].temp.toInt());
                pad->setHeight(factory_values[46].temp.toInt());
                pad->setLayerFromTo(factory_values[47].temp.toInt(), factory_values[48].temp.toInt());
                pad->setDrill(factory_values[49].temp.toInt());
                pad->update_new_values();
                via->setWidth(factory_values[50].temp.toInt());
                via->setLayerFromTo(factory_values[51].temp.toInt(), factory_values[52].temp.toInt());
                via->setDrill(factory_values[53].temp.toInt());
                via->update_new_values();
                }

        if ((ti == 0) || (ti == 2))
                {
                // SCH Grid
                if (factory_values[54].temp.toReal() == 10000.0)
                        grid_artistic->setCurrentIndex(0);
                if (factory_values[54].temp.toReal() == 5000.0)
                        grid_artistic->setCurrentIndex(1);
                if (factory_values[54].temp.toReal() == 2500.0)
                        grid_artistic->setCurrentIndex(2);
                if (factory_values[54].temp.toReal() == 1250.0)
                        grid_artistic->setCurrentIndex(3);
                if (factory_values[54].temp.toReal() == 625.0)
                        grid_artistic->setCurrentIndex(4);
                }

        if ((ti == 0) || (ti == 6))
                {
                // PCB Grid
                visible->setChecked(factory_values[55].temp.toBool());
                grid_size->setEditText(QString::number(factory_values[56].temp.toFloat()));
                if (factory_values[57].temp.toBool() == TRUE)
                        grid_UM->setCurrentIndex(0);
                if (factory_values[57].temp.toBool() == FALSE)
                        grid_UM->setCurrentIndex(1);
                if (factory_values[58].temp.toInt() == 'N')
                        snap_none->setChecked(TRUE);
                if (factory_values[58].temp.toInt() == 'G')
                        snap_grid->setChecked(TRUE);
                if (factory_values[58].temp.toInt() == 'I')
                        snap_nodes_and_grid->setChecked(TRUE);
                }
        if ((ti == 0) || (ti == 8))
                {
                // PCB Extra
                if (factory_values[59].temp.toBool())
                        draw_mode_gerber->setChecked(TRUE);
                else
                        draw_mode_draft->setChecked(TRUE);
                }
}

void	win_settings::accept()
{
        //Controllo prima che tutti i parametri inseriti siano corretti
        if ((ti == 0) || (ti == 1))
                {
                if (Pen_width_normal->warning_not_valid_parameter(tr("Normal Pen width")))
                        return;
                }

        if ((ti == 0) || (ti == 4))
                {
                if (Pen_width_wire->warning_not_valid_parameter(tr("Wire Pen width")))
                        return;
                if (Pen_width_frame->warning_not_valid_parameter(tr("Frame Pen width")))
                        return;
                }

        if ((ti == 0) || (ti == 5))
                {
                if (Pen_width_copper->warning_not_valid_parameter(tr("Copper Pen width")))
                        return;
                if (Pen_width_copper->warning_not_valid_parameter(tr("Silk Pen width")))
                        return;
                if (Pen_width_copper->warning_not_valid_parameter(tr("Paste Pen width")))
                        return;
                if (Pen_width_copper->warning_not_valid_parameter(tr("Mask Pen width")))
                        return;
                if (Pen_width_copper->warning_not_valid_parameter(tr("Board Pen width")))
                        return;
                if (Pen_width_copper->warning_not_valid_parameter(tr("Help Pen width")))
                        return;
                }

        if ((ti == 0) || (ti == 3))
                {
                // SCH Workspace
                factory_values[0].temp      = table_sch_workspace->item(0,0)->backgroundColor();
                factory_values[1].temp      = table_sch_workspace->item(1,0)->backgroundColor();
                factory_values[2].temp      = table_sch_workspace->item(2,0)->backgroundColor();
                factory_values[3].temp      = table_sch_workspace->item(3,0)->backgroundColor();
                }

        if ((ti == 0) || (ti == 7))
                {
                // PCB Workspace
                factory_values[4].temp      = table_pcb_workspace->item(0,0)->backgroundColor();
                factory_values[5].temp      = table_pcb_workspace->item(1,0)->backgroundColor();
                factory_values[6].temp      = table_pcb_workspace->item(2,0)->backgroundColor();
                factory_values[7].temp      = table_pcb_workspace->item(3,0)->backgroundColor();
                factory_values[8].temp      = table_pcb_workspace->item(4,0)->backgroundColor();
                factory_values[9].temp      = table_pcb_workspace->item(5,0)->backgroundColor();
                factory_values[10].temp      = table_pcb_workspace->item(6,0)->backgroundColor();
                factory_values[11].temp      = table_pcb_workspace->item(7,0)->backgroundColor();
                factory_values[12].temp      = table_pcb_workspace->item(8,0)->backgroundColor();
                factory_values[13].temp      = table_pcb_workspace->item(9,0)->backgroundColor();
                }

        if ((ti == 0) || (ti == 3))
                {
                // SCH layers
                factory_values[14].temp      = table_sch_layers->item(0,0)->backgroundColor();
                factory_values[15].temp      = table_sch_layers->item(1,0)->backgroundColor();
                factory_values[16].temp     = table_sch_layers->item(2,0)->backgroundColor();
                }

        if ((ti == 0) || (ti == 7))
                {
                // PCB Layers
                factory_values[17].temp     = table_pcb_layers->item(0,0)->backgroundColor();
                factory_values[18].temp     = table_pcb_layers->item(1,0)->backgroundColor();
                factory_values[19].temp     = table_pcb_layers->item(2,0)->backgroundColor();
                factory_values[20].temp     = table_pcb_layers->item(3,0)->backgroundColor();
                factory_values[21].temp     = table_pcb_layers->item(4,0)->backgroundColor();
                factory_values[22].temp     = table_pcb_layers->item(5,0)->backgroundColor();
                factory_values[23].temp     = table_pcb_layers->item(6,0)->backgroundColor();
                factory_values[24].temp     = table_pcb_layers->item(7,0)->backgroundColor();
                factory_values[25].temp     = table_pcb_layers->item(8,0)->backgroundColor();
                factory_values[26].temp     = table_pcb_layers->item(9,0)->backgroundColor();
                }

        if ((ti == 0) || (ti == 1))
                {
                // SCH Style
                factory_values[27].temp     = Pen_width_normal->length;
                factory_values[30].temp     = sch_Fill_polygon->isChecked();
                factory_values[31].temp     = sch_font_layout->font_height;
                factory_values[32].temp     = sch_font_layout->font_bold;
                factory_values[33].temp     = sch_font_layout->font_italic;
                }

        if ((ti == 0) || (ti == 4))
                {
                // SCH Workspace
                factory_values[28].temp     = Pen_width_frame->length;
                factory_values[29].temp     = Pen_width_wire->length;
                }

        if ((ti == 0) || (ti == 5))
                {
                // PCB Style
                factory_values[34].temp     = Pen_width_copper->length;
                factory_values[35].temp     = Pen_width_silk->length;
                factory_values[36].temp     = Pen_width_paste->length;
                factory_values[37].temp     = Pen_width_mask->length;
                factory_values[38].temp     = Pen_width_board->length;
                factory_values[39].temp     = Pen_width_help->length;
                factory_values[40].temp     = pcb_Fill_polygon->isChecked();
                factory_values[41].temp     = pcb_font_layout->font_height;
                factory_values[42].temp     = pcb_font_layout->font_bold;
                factory_values[43].temp     = pcb_font_layout->font_italic;
                pad->update_new_values();
                factory_values[44].temp     = pad->pv_shape;
                factory_values[45].temp     = pad->pv_width;
                factory_values[46].temp     = pad->pv_height;
                factory_values[47].temp     = pad->pv_layer_from;
                factory_values[48].temp     = pad->pv_layer_to;
                factory_values[49].temp     = pad->pv_drill;
                via->update_new_values();
                factory_values[50].temp     = via->pv_width;
                factory_values[51].temp     = via->pv_layer_from;
                factory_values[52].temp     = via->pv_layer_to;
                factory_values[53].temp     = via->pv_drill;
                }

        if ((ti == 0) || (ti == 2))
                {
                // SCH Artistic Grid
                switch (grid_artistic->currentIndex())
                {
                case 1: // 1/2
                        factory_values[54].temp = 5000.0;
                        break;
                case 2: // 1/4
                        factory_values[54].temp = 2500.0;
                        break;
                case 3: // 1/8
                        factory_values[54].temp = 1250.0;
                        break;
                case 4: // 1/16
                        factory_values[54].temp = 625.0;
                        break;
                default: // 1/1
                        factory_values[54].temp = 10000.0;
                        break;
                }
                }

        if ((ti == 0) || (ti == 6))
                {
                // PCB Grid
                factory_values[55].temp = visible->isChecked();
                factory_values[56].temp = grid_size->currentText().toFloat();
                factory_values[57].temp = (grid_UM->currentIndex() == 0);
                if (snap_none->isChecked())
                        factory_values[58].temp = 'N';
                if (snap_grid->isChecked())
                        factory_values[58].temp = 'G';
                if (snap_nodes_and_grid->isChecked())
                        factory_values[58].temp = 'I';
                }

        if ((ti == 0) || (ti == 8))
                {
                // PCB Workspace
                factory_values[59].temp = draw_mode_gerber->isChecked();
                }

        if (!ti)
                w_main->settings_temp2generaldefault();
        else
                {
                w_main->settings_temp2current();
                w_main->sb_grid_tooltip();
                w_main->sb_style_tooltip();
                }

        if (ti == 4)
                w_main->workspace_set_wire_width();

        QDialog::accept();
}

win_settings::~win_settings()
{
        if ((ti == 0) || (ti == 3) || (ti == 7))
                QObject::killTimer(timer_selection->timerId());
}

void win_settings::timeout()
    {
        static int riga;
        static QTableWidget *tabella = 0;
        int new_riga = 0;
        QTableWidget *new_tabella = 0;

        if ((ti == 0) || (ti == 2))
                if (table_sch_workspace)
                        if (table_sch_workspace->underMouse())
                                {
                                QTableWidgetItem *item = table_sch_workspace->itemAt(table_sch_workspace->mapFromGlobal(QCursor::pos()));
                                if (item != 0)
                                        {
                                        //Sono su una cella su cui è possibile cliccare
                                        new_riga = table_sch_workspace->row(item);
                                        new_tabella = table_sch_workspace;
                                        }
                                }

        if ((ti == 0) || (ti == 7))
                if (table_pcb_workspace)
                        if (table_pcb_workspace->underMouse())
                                {
                                QTableWidgetItem *item = table_pcb_workspace->itemAt(table_pcb_workspace->mapFromGlobal(QCursor::pos()));
                                if (item != 0)
                                        {
                                        //Sono su una cella su cui è possibile cliccare
                                        new_riga = table_pcb_workspace->row(item);
                                        new_tabella = table_pcb_workspace;
                                        }
                                }

        if ((ti == 0) || (ti == 2))
                if (table_sch_layers)
                        if (table_sch_layers->underMouse())
                                {
                                QTableWidgetItem *item = table_sch_layers->itemAt(table_sch_layers->mapFromGlobal(QCursor::pos()));
                                if (item != 0)
                                        {
                                        //Sono su una cella su cui è possibile cliccare
                                        new_riga = table_sch_layers->row(item);
                                        new_tabella = table_sch_layers;
                                        }
                                }

        if ((ti == 0) || (ti == 7))
                if (table_pcb_layers)
                        if (table_pcb_layers->underMouse())
                                {
                                QTableWidgetItem *item = table_pcb_layers->itemAt(table_pcb_layers->mapFromGlobal(QCursor::pos()));
                                if (item != 0)
                                        {
                                        //Sono su una cella su cui è possibile cliccare
                                        new_riga = table_pcb_layers->row(item);
                                        new_tabella = table_pcb_layers;
                                        }
                                }

        if ((new_riga!= riga) || (new_tabella != tabella))
                {
                //Deseleziono prima la vecchia riga
                if (tabella != 0)
                        tabella->setItemSelected(tabella->item(riga,1),FALSE);
                //Seleziono la nuova riga
                if (new_tabella != 0)
                        new_tabella->setItemSelected(new_tabella->item(new_riga,1),TRUE);
                //Memorizzo i nuovi dati
                riga = new_riga;
                tabella = new_tabella;
                }
        }

void	win_settings::change_color(QTableWidgetItem *item)
{
        if (item == 0)	//Serve se clicco sulla tabella al di fuori delle QTableWidgetItem
                return;

        timer_selection->stop();
        timeout();


                int riga;
                if (item->tableWidget() == table_sch_workspace)
                        {
                        riga = table_sch_workspace->row(item);
                        QColor new_color = QColorDialog::getColor(table_sch_workspace->item(riga,0)->backgroundColor());
                        if (new_color.isValid())
                                {
//				style_change_color(&new_style_sch_w, riga, new_color);
                                table_sch_workspace->item(riga,0)->setBackgroundColor(new_color);
                                }
                        }
                else if (item->tableWidget() == table_sch_layers)
                        {
                        riga = table_sch_layers->row(item);
                        QColor new_color = QColorDialog::getColor(table_sch_layers->item(riga,0)->backgroundColor());
                        if (new_color.isValid())
                                {
//				style_change_color(&new_style_sch_l, table_sch_layers->row(item), new_color);
                                table_sch_layers->item(riga,0)->setBackgroundColor(new_color);
                                }
                        }
                else if (item->tableWidget() == table_pcb_workspace)
                        {
                        riga = table_pcb_workspace->row(item);
                        QColor new_color = QColorDialog::getColor(table_pcb_workspace->item(riga,0)->backgroundColor());
                        if (new_color.isValid())
                                {
//				style_change_color(&new_style_pcb_w, riga, new_color);
                                table_pcb_workspace->item(riga,0)->setBackgroundColor(new_color);
                                }
                        }
                else if (item->tableWidget() == table_pcb_layers)
                        {
                        riga = table_pcb_layers->row(item);
                        QColor new_color = QColorDialog::getColor(table_pcb_layers->item(riga,0)->backgroundColor());
                        if (new_color.isValid())
                                {
//				style_change_color(&new_style_pcb_l, table_pcb_layers->row(item), new_color);
                                table_pcb_layers->item(riga,0)->setBackgroundColor(new_color);
                                }
                        }

        timer_selection->start();
}

void    win_settings::load_factory_settings()
{
    switch (QMessageBox::warning(this, "Itecad", tr("Attention: this operation will affect all tabs. Proceed?"), tr("&Ok"), tr("&Cancel."), "", 0, 1))
        {
        case 0:		// 0 = YES
                w_main->settings_factory2temp();
                load_data();
                break;
        case 1:		// 1 = NO
        case 2:		// 2 = CANCEL
                break;
        }
}

void    win_settings::load_general_default_settings()
{
        switch (QMessageBox::warning(this, "Itecad", tr("Attention: this operation will affect all tabs. Proceed?"), tr("&Ok"), tr("&Cancel."), "", 0, 1))
        {
        case 0:		// 0 = YES
                w_main->settings_generaldefault2temp();
                load_data();
                break;
        case 1:		// 1 = NO
        case 2:		// 2 = CANCEL
                break;
        }
}

