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

#include <QComboBox>
#include "widget_zoom.h"
#include "win_main.h"

extern	char file_type;

extern	myGraphicsView	*GV;

extern  QRegExp exp_zoom;

Zoom::Zoom( QWidget *parent) : QComboBox( parent)	 
{
        setEditable(TRUE);
//	setSizeAdjustPolicy(QComboBox::AdjustToContents);
	setInsertPolicy(QComboBox::InsertAtBottom);	//La stringa inserita manualmente è messa in ultima posizione 
	setMaxVisibleItems(20);	//Imposto a 20 le righe prima di avere la barra di scorrimento verticale
	setAutoCompletion(FALSE);
	clearFocus();
	
	//Definisco le stringhe che dipendono dal translator
	zoom_Fit_to_All_Objects = tr("All Objects");
	zoom_Fit_to_Selected_Objects = tr("Selected Objects");
	zoom_Fit_to_Page = tr("Page");
	zoom_Fit_to_Page_Width = tr("Page Width");
	zoom_Fit_to_Page_Height = tr("Page Height");
	zoom_Fit_to_Board = tr("Board");
	zoom_Fit_to_Board_Width = tr("Board Width");
	zoom_Fit_to_Board_Height = tr("Board Height");
	//Calcolo il valore di riferimento 
	zoom_100_per_100 = ((qreal) physicalDpiX()) / 25400.0;

	connect(this,SIGNAL(activated(int)),this,SLOT(zoom_change(int)));
	//connect(this,SIGNAL(customContextMenuRequested( const QPoint & )),this,SLOT(zoom_magari( const QPoint & )));

	//Inserisco dei dati per avere la giusta larghezza della ComboBox (questi dati verranno poi cancellati)
	clear();
	insertItem(0,"");
	insertItem(1,zoom_Fit_to_All_Objects);
	insertItem(2,zoom_Fit_to_Selected_Objects);
	insertItem(3,zoom_Fit_to_Page);
	insertItem(4,zoom_Fit_to_Page_Width);
	insertItem(5,zoom_Fit_to_Page_Height);
	insertItem(6,zoom_Fit_to_Board);
	insertItem(7,zoom_Fit_to_Board_Width);
	insertItem(8,zoom_Fit_to_Board_Height);

        setValidator(new QRegExpValidator(exp_zoom, this)); //Uso new perchè ogni volta bisogna ricreare QRegExpValidator (non si può utilizzare l'oggetto altrove...)
}

Zoom::~Zoom()
{
//    Qui dovrei eliminare le eventuali classi private create nell'istanza dell'oggetto Layer	
}

void Zoom::zoom_show_scale()
{
        QString new_string = QString::number(100.0 * GV->matrix().m11() / zoom_100_per_100, 'f', 0)+"%";
	if (new_string != currentText())
		setEditText(new_string);    
        GV->setFocus(Qt::OtherFocusReason);	//Riporto il fuoco su GV
}

void Zoom::zoom_change(int indice)
{
        switch (itemData(indice).toInt())
	{
	case 0:			//stringa inserita manualmente 
		{
		QString testo;
		testo = currentText();		
		
		//Controllo che il testo inserito sia un intero con un eventuale "%" di seguito.
		if (testo.right(1) == "%")
			testo = testo.left(testo.length() - 1);
		int percentuale = testo.toInt();

		if (testo == QString::number(percentuale))
			{
                        qreal scale = ((qreal) percentuale / 100.0) * zoom_100_per_100 / GV->matrix().m11();
			GV->zoom_scale(scale);
			}
		zoom_show_scale();
		}
		break;
	case 100:		//Fit to All Objects
		GV->zoom_to_all();
		break;
	case 101:		//Fit to Selected Objects
		GV->zoom_to_selected();
		break;
	case 102:		//Fit to Page 
		GV->zoom_to_page();
		break;
	case 103:		//Fit to Page Width
		GV->zoom_to_page_width();
		break;
	case 104:		//Fit to Page Height
		GV->zoom_to_page_height();
		break;
	case 105:		//Fit to Board
		GV->zoom_to_page();
		break;
	case 106:		//Fit to Board Width
		GV->zoom_to_page_width();
		break;
	case 107:		//Fit to Board Height
		GV->zoom_to_page_height();
		break;
	case 108:		//10%
		{
		qreal scale = 0.1 * zoom_100_per_100 / GV->matrix().m11();
		GV->zoom_scale(scale);
		}
		break;
	case 109:		//25%
		{
		qreal scale = 0.25 * zoom_100_per_100 / GV->matrix().m11();
		GV->zoom_scale(scale);
		}
		break;
	case 110:		//50%
		{
		qreal scale = 0.5 * zoom_100_per_100 / GV->matrix().m11();
		GV->zoom_scale(scale);
		}
		break;
	case 111:		//75%
		{
		qreal scale = 0.75 * zoom_100_per_100 / GV->matrix().m11();
		GV->zoom_scale(scale);
		}
		break;
	case 112:		//100%
		{
		qreal scale = zoom_100_per_100 / GV->matrix().m11();
		GV->zoom_scale(scale);
		}
		break;
	case 113:		//200%
		{
		qreal scale = 2.0 * zoom_100_per_100 / GV->matrix().m11();
		GV->zoom_scale(scale);
		}
		break;
	case 114:		//400%
		{
		qreal scale = 4.0 * zoom_100_per_100 / GV->matrix().m11();
		GV->zoom_scale(scale);
		}
		break;
        }
}

void	Zoom::mousePressEvent(QMouseEvent *event)
{
        QString old_string = currentText();
	
	//Ricreo il menu prima di visualizzarlo
	//E' meglio farlo prima di visualizzare, perchè solo in questo momento so se 
	//bisogna inserire o meno l'item zoom_Fit_to_Selected_Objects
	clear();
	insertItem(0,zoom_Fit_to_All_Objects, 100);
	insertItem(1,zoom_Fit_to_Selected_Objects, 101);
	int riga = 2;
	if (file_type == FT_PROJECT)
		{
		if (GV->is_sch)
			{
			insertItem(riga++,zoom_Fit_to_Page, 102);
			insertItem(riga++,zoom_Fit_to_Page_Width, 103);
			insertItem(riga++,zoom_Fit_to_Page_Height, 104);
			}
		else
			{
			insertItem(riga++,zoom_Fit_to_Board, 105);
			insertItem(riga++,zoom_Fit_to_Board_Width, 106);
			insertItem(riga++,zoom_Fit_to_Board_Height, 107);
			}
		}
	insertItem(riga++,"10%", 108);
	insertItem(riga++,"25%", 109);
	insertItem(riga++,"50%", 110);
	insertItem(riga++,"75%", 111);
	insertItem(riga++,"100%", 112);
	insertItem(riga++,"200%", 113);
	insertItem(riga++,"400%", 114);
	
//	setCurrentIndex(0);	

	setEditText(old_string);

        QComboBox::mousePressEvent(event);
}
