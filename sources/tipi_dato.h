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

#ifndef TIPI_DATO_H
#define TIPI_DATO_H

#include <QColor>
#include <QString>
#include <QPrinter>
#include <QVariant>

#define FT_NULL 0
#define FT_PROJECT 1
#define FT_SCH_SYMBOL 2
#define FT_PCB_SYMBOL 3

#define DT_SCH 0
#define DT_PCB 1
#define DT_TXT 2

#define	W_SCHbackground 0
#define	W_SCHpage 1
#define	W_SCHgrid 2     //ATTENZIONE: serve quando si disegna il simbolo e forse in futuro servirà anche nei progetti
#define	W_SCHselection 3

#define	W_PCBbackground 4
#define	W_PCBgrid 5
#define	W_PCBpadthrough 6
#define	W_PCBpadtop 7
#define	W_PCBpadbottom 8
#define	W_PCBvia 9
#define	W_PCBburiedvia 10
#define	W_PCBratsnest 11
#define	W_PCBdynamicratsnest 12
#define	W_PCBselection 13

//Attenzione l'elenco sotto deve essere definito in ordine di visualizzazione
#define L_SCHNormal 0
#define L_SCHFrame 1
#define L_SCHWire 2

//Attenzione l'elenco sotto deve essere definito in ordine di visualizzazione
#define L_PCBBoard 3
#define L_PCBPasteTop 4
#define L_PCBSilkTop 5
#define L_PCBMaskTop 6
#define L_PCBCopper1 7
/*************************************************************************************
//Uso 16 signal layers. Ha senso contare i signal layers e non i layers fisici del PCB
//Si tenga presente che si possono fare PCB di 1,2,3,4,5,6,7,8 etc signal layers
//I più comuni multistrato sono con 4 signal layers dove 2 layers sono le alimentazioni
//A volte si hanno più alimentazioni per cui si hanno ancora più layers
//Gli spessori tra i vari signal layers non è detto che siano uguali tra tutti i layer
//Tutti i programmi normalmente arrivano fino a 16 signal layers
//Attenzione che quando si gestirà il multistrato bisognera far scegliere all'utente il layer da cui
//si prosegue quando si inserisce una via
//Inoltre bisognerà prevedere le buried vias, ossia i metallizzati che sono dei fori ciechi
//e che collegano tra loro solo alcuni layers e non tutti
//Le via e le buried vias dovrebbero essere tutte con piazzola a forma di cerchio, ma dato che gli
//altri programmi prevedono via delle forme come i pad mi conviene gestirle anch'io così
//Non mi sembra molto complicata la gestione  del multilayer, ma in prima battuta lavoro solo a doppia faccia.
#define L_PCBCopper2 8	//Presente se multistrato formato da 3 signal layers
#define L_PCBCopper3 9	//Presente se multistrato formato da 4 signal layers
#define L_PCBCopper4 10	//Presente se multistrato formato da 5 signal layers
#define L_PCBCopper5 11	//Presente se multistrato formato da 6 signal layers
#define L_PCBCopper6 12	//Presente se multistrato formato da 7 signal layers
#define L_PCBCopper7 13	//Presente se multistrato formato da 8 signal layers
  ... etc etc
**************************************************************************************/
#define L_PCBCopper16 8
#define L_PCBMaskBottom 9
#define L_PCBSilkBottom 10
#define L_PCBPasteBottom 11
#define L_PCBHelp 12

#define LAYER_DATA_COUNT 13
#define LAYER_DATA_SCH_COUNT 3 
#define LAYER_DATA_PCB_COUNT 10
#define WORKSPACE_DATA_COUNT 14
#define WORKSPACE_DATA_SCH_COUNT 4
#define WORKSPACE_DATA_PCB_COUNT 10

#define T_SELECT 0
#define T_EDIT_NODE 1
#define T_ZOOM 2
#define T_HAND 3
#define T_CONNECTION 4
#define T_POLYLINE 5
#define T_POLYGON 6
#define T_ELLIPSE 7
#define T_RECT 8
#define T_TEXT 9
#define T_PIN 10
#define T_PADVIA 11
#define T_PORT 12
#define T_ADD 13
#define T_ORIGIN 14
#define T_RULER 15
#define T_ADD_POWER 16
#define T_EDIT_COMPONENT 17

#define O_CONNECTION 0
#define O_POLYLINE 1
#define O_ELLIPSE 2
#define O_RECT 3
#define O_POLYGON 4
#define O_TEXT 5
#define O_IDENT 6
#define O_VALUE 7
#define O_PIN 8
#define O_PAD 9
#define O_VIA 10
#define O_PORT 11
#define O_POWER 12
#define O_SEGMENT 13    //Utilizzato per disegnare solo un segmento di un poligono o una polilinea

#define PS_ROUND 0
#define PS_RECTANGLE 1
#define PS_ROUNDED_RECTANGLE 2

#define  internal_font_height 8
#define  internal_font_bold FALSE
#define  internal_font_italic FALSE

#define  PIN_SHAPE_NONE 0
#define  PIN_SHAPE_LINE 1
#define  PIN_SHAPE_CLOCK 2
#define  PIN_SHAPE_DOT 3
#define  PIN_SHAPE_DOT_CLOCK 4

#define  PORT_GENERIC 0
#define  PORT_INPUT 1
#define  PORT_OUTPUT 2
#define  PORT_BIDIRECTIONAL 3

#define  SYMBOL_GROUND_POWER 0
#define  SYMBOL_GROUND_SIGNAL 1
#define  SYMBOL_GROUND_EARTH 2
#define  SYMBOL_POWER_CIRCLE 3
#define  SYMBOL_POWER_BAR 4

#define SCH_CONNECTION_GRID	10000.0

#define SCH_PEN_NORMAL 1000
#define SCH_PEN_SEMIBOLD 1600
#define SCH_PEN_BOLD 2200

struct UM_description 
{	
	QString	descrizione;
	float	k;
	int		decimal_digits;
};
#define UM_DESCRIPTION_COUNT 6

struct lay_settings	
{	
	bool	visible;
	bool	print;
        bool	gerber;
};

struct lay_data
{
	QColor	color;
        QString	user_description;
        QString	description;
	bool	is_sch;		// DT_SCH oppure DT_PCB
	bool	is_copper;	// True se vi sono delle connessioni tra PIN e PAD
};

struct stampa_formato 
{	
        int					altezza;
        int					larghezza;
        QPrinter::PaperSize     		formato;
	QString					descrizione;
        int					UM;
};
#define STAMPA_FORMATI_COUNT 24

struct node_data 
{
        QString			net_name;			//Può essere GND, Vcc oppure una stringa vuota
        int			net_id;
        QPoint			point;
        int			connected_pin_or_pad_or_via;	//Si può essere collegati solo a un pin o a un pad o ad una via
        QList<int>		connected_wires;                //Tutti i wires che arrivano al nodo sono sempre tra loro collegati

	bool			highlight_active;
	bool			drag_or_move_active;

	node_data		*help_duplicate;		//Tutte le variabili che iniziano per help servono per elaborazioni temporanee
};

struct char_line
{
	char	x1;
	char	y1;
	char	x2;
	char	y2;
};

struct char_point
{
	char	x;
	char	y;
};

struct char_symbol
{
	int					unicode;
	char				width;
	QList<char_line>	char_lines;
	QList<char_point>	char_points;
};

struct ratsnest_struct
{
	int		net_id;
	int		pcb_node_start;
	int		pcb_node_stop;
};

struct ungated_pins_struct
{
        QString         netname;
	int		pin;
};

struct devices_struct
{
        QString         device;
        QString		footprint;
};

struct net_names_struct
{
        QString         netname;
        int             comp_pointer;
        int             comp_pin_number;
        int		net_id;
        int             pad;
};

struct factory_value
{
        QString     description;
        QVariant    value;     //Factory Value
        QVariant    temp;      //Support
        int         type;      //Tipo di dato contenuto in QVariant
};
#define FACTORY_VALUE_COUNT 60

#endif
