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

#include <QMouseEvent>
#include <QInputEvent>
#include "win_main.h"
#include "widget_layers.h"

extern	win_main		*w_main;
extern	myGraphicsView          *GV;
extern	lay_data		layer_data[LAYER_DATA_COUNT];

My_Layers::My_Layers()
        : QTableWidget()
{
        setShowGrid(TRUE);
        setFocusPolicy(Qt::NoFocus);
	setSelectionMode(QAbstractItemView::NoSelection);	//Seleziono io dal codice, che è meglio
        verticalHeader()->hide();
	horizontalHeader()->hide();		
	setEditTriggers(QAbstractItemView::NoEditTriggers);	//Le caselle non sono editabili

	setIconSize(QSize(20,20));	//Imposto tutte le icone sulla dimensione 20x20 pixel
}

void	My_Layers::load_data()
{
	clear();	
        setRowCount(LAYER_DATA_PCB_COUNT);

	switch(tipo_tabella)
		{
                case 'V':   //Visible
			{			
                        setColumnCount(3);
			int riga = 0;
			for(int i = 0; i < LAYER_DATA_COUNT; i++)
                                if 	(!layer_data[i].is_sch)
					{
                                        QTableWidgetItem *descr = new QTableWidgetItem(GV->layer_square(i, 20), layer_data[i].description);
                                        setItem(riga, 0, descr);

                                        QTableWidgetItem *active = new QTableWidgetItem("");
                                        if (GV->layer_selected == i)
                                                active->setIcon(QIcon(":/Resources/pen.png"));
                                        setItem(riga, 1, active);

                                        QTableWidgetItem *show = new QTableWidgetItem("");
					if (GV->layer_settings[i].visible)
						show->setIcon(QIcon(":/Resources/eye.png"));
                                        setItem(riga, 2, show);

					row_conversion[riga] = i;
					riga++;
					}

                        setColumnWidth(1,30);
                        setColumnWidth(2,30);

			QHeaderView *hh = horizontalHeader();
                        hh->resizeSection(0, viewport()->width() - 60);

			w_main->sb_refresh(); 
			}
			break;
                case 'P':   //Print
			{
                        setColumnCount(2);
			int riga = 0;
			for(int i = 0; i < LAYER_DATA_COUNT; i++)
                                if 	(!layer_data[i].is_sch)
					{
                                        QTableWidgetItem *descr = new QTableWidgetItem(GV->layer_square(i, 20), layer_data[i].description);
                                        setItem(riga, 0, descr);


					QTableWidgetItem *print = new QTableWidgetItem("");
					if (GV->layer_settings[i].print)
                                                print->setIcon(QIcon(":/Resources/printer.png"));
                                        else
                                                print->setIcon(QIcon());
                                        setItem(riga, 1, print);

					row_conversion[riga] = i;
					riga++;
					}

                        setColumnWidth(1,30);

			QHeaderView *hh = horizontalHeader();
                        hh->resizeSection(0, viewport()->width() - 30);

			w_main->sb_refresh(); 
			}
			break;
                  case 'G': //Gerber Drill
                          {
                          setColumnCount(2);
                          int riga = 0;
                          for(int i = 0; i < LAYER_DATA_COUNT; i++)
                                  if 	(!layer_data[i].is_sch)
                                          {
                                          QTableWidgetItem *descr = new QTableWidgetItem(GV->layer_square(i, 20), layer_data[i].description);
                                          setItem(riga, 0, descr);

                                          QTableWidgetItem *gerber = new QTableWidgetItem("");
                                          if (GV->layer_settings[i].gerber)
                                                  gerber->setIcon(QIcon(":/Resources/cog.png"));
                                          else
                                                  gerber->setIcon(QIcon());
                                          setItem(riga, 1, gerber);

                                          row_conversion[riga] = i;
                                          riga++;
                                          }

                          setColumnWidth(1,30);

                          QHeaderView *hh = horizontalHeader();
                          hh->resizeSection(0, viewport()->width() - 30);

                          w_main->sb_refresh();
                          }
                          break;
		}
	
}

QSize	My_Layers::right_size()
{
        QRect r = visualItemRect(item(LAYER_DATA_PCB_COUNT - 1, 0));
        return QSize(r.left(), r.bottom() + 4);     //Di solito lo spessore della griglia è di 4 pixel di margine
}

void	My_Layers::resizeEvent(QResizeEvent *e) 
    {
		switch(tipo_tabella)
		{
		case 'V':
			{
                        setColumnWidth(1,30);
                        setColumnWidth(2,30);

			QHeaderView *hh = horizontalHeader();
                        hh->resizeSection(0,viewport()->width() - 60);
			}
			break;
		case 'P':
                case 'G':
			{
                        setColumnWidth(1,30);
					
			QHeaderView *hh = horizontalHeader();
                        hh->resizeSection(0,viewport()->width() - 30);
			}
			break;
                 }
    }

void	My_Layers::toogle_select(int tab_row)
        {
                int lay_row = row_conversion[tab_row];

                //Deseleziono prima tutto
                for (int i = 0; i < rowCount(); i++)
                        {
                        QTableWidgetItem *active = item(i,1);
                        active->setIcon(QPixmap());
                        }

                GV->layer_settings[lay_row].visible = TRUE;	//Rendo visibile il layer selezionato (Il layer selezionato è sempre visibile)
                QTableWidgetItem *eye = item(tab_row,2);
                eye->setIcon(QIcon(":/Resources/eye.png"));
                QTableWidgetItem *active = item(tab_row,1);
                active->setIcon(QIcon(":/Resources/pen.png"));

                GV->layer_selected = lay_row;

                reset();	//Questa istruzione è molto importante

                GV->setUpdatesEnabled(FALSE);
                GV->setUpdatesEnabled(TRUE);

                w_main->sb_refresh(); //layer->setCurrentIndex(tab_row);
        }

void	My_Layers::toogle_show(int tab_row)
	{
		int lay_row = row_conversion[tab_row];

                if (lay_row == GV->layer_selected)
                        return;	//Un layer selezionato non può essere invisibile
                //Incredibile!!! La riga sotto non funziona per cui devo fare diversamente
                //GV->layer_settings[lay_row].visible = !(GV->layer_settings[lay_row].visible);
                if (GV->layer_settings[lay_row].visible)
                        GV->layer_settings[lay_row].visible = FALSE;
                else
                        GV->layer_settings[lay_row].visible = TRUE;

                QTableWidgetItem *eye = item(tab_row,2);
		if (GV->layer_settings[lay_row].visible)
			eye->setIcon(QIcon(":/Resources/eye.png"));
		else
			eye->setIcon(QPixmap());

		reset();	//Questa istruzione è molto importante

                GV->setUpdatesEnabled(FALSE);
                GV->setUpdatesEnabled(TRUE);
	}

void	My_Layers::toogle_print(int tab_row)
	{
		int lay_row = row_conversion[tab_row];

                //Incredibile!!! La riga sotto non funziona per cui devo fare diversamente
                //GV->layer_settings[lay_row].print = !(GV->layer_settings[lay_row].print);
                if (GV->layer_settings[lay_row].print)
                        GV->layer_settings[lay_row].print = FALSE;
                else
                        GV->layer_settings[lay_row].print = TRUE;
		
                QTableWidgetItem *print = item(tab_row,1);
                if (GV->layer_settings[lay_row].print)
			{
                        print->setIcon(QIcon(":/Resources/printer.png"));
                        }
		else
			{
                        print->setIcon(QIcon());
                        }
		
		reset();	//Questa istruzione è molto importante
	}

void	My_Layers::toogle_gerber(int tab_row)
        {
                int lay_row = row_conversion[tab_row];

                //Incredibile!!! La riga sotto non funziona per cui devo fare diversamente
                //GV->layer_settings[lay_row].gerber = !(GV->layer_settings[lay_row].gerber);
                if (GV->layer_settings[lay_row].gerber)
                        GV->layer_settings[lay_row].gerber = FALSE;
                else
                        GV->layer_settings[lay_row].gerber = TRUE;

                QTableWidgetItem *gerber = item(tab_row,1);
                if (GV->layer_settings[lay_row].gerber)
                        {
                        gerber->setIcon(QIcon(":/Resources/cog.png"));
                        }
                else
                        {
                        gerber->setIcon(QIcon());
                        }

                reset();	//Questa istruzione è molto importante
        }

void	My_Layers::mousePressEvent(QMouseEvent *e)
	{
	QPoint point = e->pos();
	//Verifico che point sia dentro una casella della tabella
        QRect visible_area = visualItemRect(item(rowCount()-1,0));
        if (point.y() > visible_area.bottom())
		return;
	if (e->button() == Qt::LeftButton)
		{
			int col = columnAt(point.x());
			
			switch(tipo_tabella)
			{
			case 'V':
				switch(col)
				{
				case 0:
                                        break;
                                case 1:
                                        toogle_select(row(itemAt(point)));
                                        break;
                                case 2:
					toogle_show(row(itemAt(point)));
					break;
				}
				break;
			case 'P':
                                if(col == 1)
                                        toogle_print(row(itemAt(point)));
				break;
                        case 'G':
                                if(col == 1)
                                        toogle_gerber(row(itemAt(point)));
                                break;
                        }
		}
        }
