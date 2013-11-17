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

#include <QPainter>
#include "win_help_about.h"
#include "win_main.h"

extern	win_main	*w_main;
extern	QString         prog_version_string;

win_help_about::win_help_about(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
	{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Vedere Qt::WindowContextHelpButtonHint per avere maggiori informazioni
	
        frame = 0;
        first_scroll = TRUE;

        pixmap_itecad = new QPixmap(":/Resources/itecad.png");
        pixmap_logo = new QPixmap(":/Resources/itecad_logo.png");
        pixmap_iteco = new QPixmap(":/Resources/iteco.png");
        pixmap_scroll = new QPixmap(400, 470);

        //Trovo il colore di sfondo
        colore_sfondo = QImage(":/Resources/iteco.png").pixel(1,1);
        colore_sfondo_attenuato = colore_sfondo;

        font_release.setFamily("Arial");
        font_release.setPixelSize(20);
        font_release.setBold(TRUE);
        font_title.setFamily("Arial");
        font_title.setBold(TRUE);
        font_title.setPixelSize(16);
        font_body.setFamily("Arial");
        font_body.setPixelSize(14);

        release = "Itecad " + prog_version_string;

        setup();

        //Imposto il colore di sfondo della finestra
        QPalette new_palette(palette());
        new_palette.setColor(QPalette::Background, colore_sfondo.dark(150));
        setPalette(new_palette);

        timer_frame.start(50, this);	//Il timer viene eliminato quando si elimina la finestra
        //////////////////////////////////////////////////////////////////////////////
        // Lo scrolling difficilmente sarà fluido perchè per esserlo deve essere sincrono
        // con la frequenza di refresh del monitor che è normalmente
        // Un valore molto usato è 60H = 16,6666ms che è un sottomultiplo di 50ms
        // Tenere presente che i frame delle animazioni possono avvenire un po' prima o un po' dopo
        // rispetto al tempo impostato sul timer per via dei ritardi del sistema operativo
        //////////////////////////////////////////////////////////////////////////////////////////
        }

void	win_help_about::setup()
	{
	setWindowTitle(tr("About"));	
        setModal(true);

	QVBoxLayout	*layout = new QVBoxLayout(this);
        label_scroll = new QLabel(this);
        label_scroll->setFixedWidth(400);
        label_scroll->setFixedHeight(470);
        layout->addWidget(label_scroll);
        if (w_main)
                layout->addLayout(setup_button());
	layout->setSpacing(6);
        layout->setMargin(8);
        }

QHBoxLayout	*win_help_about::setup_button()
	{
	QHBoxLayout	*L_comandi = new QHBoxLayout();
	L_comandi->setSpacing(6);
	L_comandi->setMargin(0);
	QSpacerItem	*L_comandi_molla1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	L_comandi->addItem(L_comandi_molla1);
	QPushButton	*L_comandi_OK = new QPushButton(this);
	L_comandi_OK->setText(tr("Ok"));
	L_comandi->addWidget(L_comandi_OK);
	QSpacerItem	*L_comandi_molla2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	L_comandi->addItem(L_comandi_molla2);
	
	QObject::connect(L_comandi_OK, SIGNAL(clicked()), this, SLOT(accept()));
	
	return L_comandi;
	}

void win_help_about::timerEvent(QTimerEvent *event)
{
        if (event->timerId() == timer_frame.timerId())
                {
                frame++;
                {
                QPainter painter(pixmap_scroll);
                painter.setRenderHint(QPainter::Antialiasing, TRUE);
                painter.setRenderHint(QPainter::HighQualityAntialiasing, TRUE);
                painter.setRenderHint(QPainter::TextAntialiasing, TRUE);
                painter.setRenderHint(QPainter::SmoothPixmapTransform, TRUE);
                painter.fillRect(0, 0, 400, 470, QColor(255,255,255));
                if (w_main)
                        {
                        int     y_pos = 164 - frame;
                        QString page_title;
                        QString page_body;

                        //Disegno il Logo Itecad
                        if ((frame < 20) && first_scroll)
                                {
                                painter.drawPixmap(50, 144, *pixmap_logo);
                                }
                        else
                                {
                                first_scroll = FALSE;
                                painter.drawPixmap(50, y_pos, *pixmap_logo);
                                }

                        y_pos += 220;
                        painter.setFont(font_title);
                        page_title = release;
                        painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_title);
                        painter.setFont(font_body);
                        page_body = tr("\"Italian Electronic CAD\"");
                        y_pos += 30;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);
                                page_body = tr("aka");
                                y_pos += 20;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);
                                page_body = tr("\"Iteco CAD\"");
                                y_pos += 20;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);

                                painter.setFont(font_title);
                                page_title = tr("Released under the GNU/LGPL 2.1 licence");
                                y_pos += 100;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_title);
                                painter.setFont(font_body);
                                page_body = tr("The program is provided AS IS");
                                y_pos += 30;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);
                                page_body = tr("with NO WARRANTY OF ANY KIND,");
                                y_pos += 20;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);
                                page_body = tr("INCLUDING THE WARRANTY OF DESIGN,");
                                y_pos += 20;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);
                                page_body = tr("MERCHANTABILITY AND FITNESS");
                                y_pos += 20;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);
                                page_body = tr("FOR A PARTICULAR PURPOSE.");
                                y_pos += 20;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);
                      if (frame > 200)
                                {
                                painter.setFont(font_title);
                                page_title = tr("Engineering Lead:");
                                y_pos += 100;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_title);
                                painter.setFont(font_body);
                                page_body = "Alberto Plano";
                                y_pos += 30;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);

                                painter.setFont(font_title);
                                page_title = tr("Programming:");
                                y_pos += 100;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_title);
                                painter.setFont(font_body);
                                page_body = "Giuseppe Pallard";
                                y_pos += 30;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);
                                page_body = "Maurilio Pizzurro";
                                y_pos += 20;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);
                                page_body = "Alberto Plano";
                                y_pos += 20;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);

                                painter.setFont(font_title);
                                page_title = tr("Testing and support:");
                                y_pos += 100;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_title);
                                painter.setFont(font_body);
                                page_body = "Luca Boldrini";
                                y_pos += 30;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);
                                page_body = "Andrea Michiardi";
                                y_pos += 20;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);
                                page_body = "Enrico Migliore";
                                y_pos += 20;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);
                                page_body = "Pierfrancesco Tolomelli";
                                y_pos += 20;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);

                                //Aggiungere i traduttori
                                //title.append(tr("Translations:"));
                                //body.append(tr("????"));
                                //else if (frame < 1200)

                                painter.setFont(font_title);
                                page_title = tr("Special Thanks:");
                                y_pos += 100;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_title);
                                painter.setFont(font_body);
                                page_body = tr("www.famfamfam.com (for the icons)");
                                y_pos += 30;
                                painter.drawText(QRect(0,y_pos,400,470), Qt::AlignHCenter, page_body);
                                }
                        if (frame > 1100)
                                {
                                painter.drawPixmap(50, 1450 - frame, *pixmap_logo);
                                }
                        if (frame >= 1286)
                                frame = 0;
                        }
                else
                        {
                        if (frame >= 30)
                                accept();   //Chiudo la finestra
                        //Disegno il Logo Itecad che si ingrandisce
                        QRect target(200, 232, 0, 0);
                        target.adjust(-150*(frame + 70)/100, -87*(frame + 70)/100, 150*(frame + 70)/100, 87*(frame + 70)/100);

                        QRect source(0, 0, 300, 175);
                        painter.drawPixmap(target, *pixmap_logo, source);
                        }
                painter.setBrush(QBrush(colore_sfondo));
                painter.setPen(Qt::NoPen);
                static const QPoint top[4] = {
                            QPoint(0, 0),
                            QPoint(0, 195),
                            QPoint(400, 95),
                            QPoint(400, 0)
                            };
                painter.drawConvexPolygon(top, 4);
                static const QPoint bottom[4] = {
                            QPoint(400, 470),
                            QPoint(400, 304),
                            QPoint(0, 404),
                            QPoint(0, 470)
                            };
                painter.drawConvexPolygon(bottom, 4);

                //Disegno il Logo Itecad
                painter.drawPixmap(0, 5, *pixmap_itecad);

                //Disegno il Logo Iteco
                painter.drawPixmap(400 - 219, 470 - 111, *pixmap_iteco);

                //Scrivo la release
                painter.setPen(QPen(QColor(255,255,255)));
                painter.setFont(font_release);
                painter.drawText(QRect(15, 100, 400, 60), Qt::AlignLeft, release);

                //Scrivo powered by
                painter.setPen(QPen(QColor(255,255,255)));
                painter.setFont(font_title);
                painter.drawText(QRect(15, 470 - 50, 400, 60), Qt::AlignLeft, "Powered by:");
                }
                label_scroll->setPixmap(*pixmap_scroll);
                }
        else
                {
                QWidget::timerEvent(event);
                }
}
