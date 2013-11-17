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

#include "win_bill_of_materials.h"
#include "win_main.h"

extern	char			file_type;
extern	win_main		*w_main;
extern	QClipboard              *clipboard;

extern  QString file_name;
extern  QString file_path;

win_bill_of_materials::win_bill_of_materials(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
{
        // Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
        // Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
        // Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

        setWindowTitle(tr("Bill of Materials"));
        //setSizeGripEnabled(true);
        setModal(true);

        QGridLayout	*layout = new QGridLayout(this);

        QRect screen = QApplication::desktop()->screenGeometry();
        bom.setFixedWidth(screen.width() / 2);
        bom.setFixedHeight(screen.height() / 2);
        bom.setSelectionMode(QAbstractItemView::NoSelection);

        bom.setIconSize(QSize(24,22));
        bom.setFocusPolicy(Qt::NoFocus);	//Se non metto questa riga quando premo Enter la finestra non si chiude (questo perchè il fuoco non passa al comando OK)
        bom.setFixedHeight(600);

        layout->addWidget(&bom,0,0);
        layout->addLayout(setup_buttons(),1,0);
        layout->setSpacing(6);
        layout->setMargin(8);

        load_bom();
}

QHBoxLayout	*win_bill_of_materials::setup_buttons()
{
        QHBoxLayout	*L_comandi = new QHBoxLayout();
        L_comandi->setSpacing(6);
        L_comandi->setMargin(0);
        group_by_value = new QCheckBox(tr("Group by Value"));
        group_by_value->setChecked(TRUE);
        QObject::connect(group_by_value,SIGNAL(stateChanged(int)),this,SLOT(load_bom()));
        L_comandi->addWidget(group_by_value);
        QSpacerItem	*L_comandi_molla1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        L_comandi->addItem(L_comandi_molla1);

        QPushButton	*L_comandi_Save = new QPushButton(this);
        L_comandi_Save->setText(tr("Save"));
        L_comandi->addWidget(L_comandi_Save);
        QObject::connect(L_comandi_Save, SIGNAL(clicked()), this, SLOT(save_as_csv()));
        QPushButton	*L_comandi_Clipboard = new QPushButton(this);
        L_comandi_Clipboard->setText(tr("Copy to Clipboard"));
        L_comandi->addWidget(L_comandi_Clipboard);
        QObject::connect(L_comandi_Clipboard, SIGNAL(clicked()), this, SLOT(copy_to_clipboard()));


        //QSpacerItem	*L_comandi_molla2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        //L_comandi->addItem(L_comandi_molla2);

        QPushButton	*L_comandi_Cancel = new QPushButton(this);
        L_comandi_Cancel->setText(tr("Cancel"));
        L_comandi->addWidget(L_comandi_Cancel);
        QObject::connect(L_comandi_Cancel, SIGNAL(clicked()), this, SLOT(reject()));

        L_comandi_Save->setAutoDefault(TRUE);

        return L_comandi;
}

void    win_bill_of_materials::update_csv()
{
        csv_text = "";
        for (int i= 0; i < bom.rowCount(); i++)
                csv_text += bom.item(i, 0)->text() + "\t" + bom.item(i, 1)->text() + "\t" + bom.item(i, 2)->text() + "\r\n";
}

void    win_bill_of_materials::copy_to_clipboard()
{
        update_csv();
        clipboard->setText(csv_text);
}

void    win_bill_of_materials::save_as_csv()
{
        QFileDialog *dialog = new QFileDialog(this, QObject::tr("Save As CSV file"),  QFileInfo(file_path).path() + "/bom.txt", "CSV file (*.txt)");
        dialog->setAcceptMode(QFileDialog::AcceptSave);
        dialog->setFileMode(QFileDialog::AnyFile);
        //Conviene usare l'estensione .txt perchè .csv è gestito male da Excel
        dialog->setDefaultSuffix("txt");
        if (dialog->exec())
                {
                QStringList fileNames = dialog->selectedFiles();
                if (!fileNames.isEmpty())
                        {
                        QFile file(fileNames.first());
                        if (file.open(QFile::WriteOnly | QFile::Truncate))
                                {
                                QTextStream out(&file);
                                out.setCodec("UTF-8");	//Attenzione: UTF-8 usa 8bit per carattere ma riesce a scrivere tutto l'unicode usando sequenze multibyte quando necessario
                                update_csv();
                                out << csv_text;
                                }
                        else
                                QMessageBox::warning(w_main, "Itecad", QObject::tr("Cannot write file %1:\n%2").arg(file.fileName()).arg(file.errorString()));

                        }
                }
}

void    win_bill_of_materials::load_bom()
{
        bom.setSortingEnabled(FALSE);
        bom.clear();
        //Le colonne sono sempre 3: se ho il raggruppamento aggiungo quantity e non document
        bom.setColumnCount(3);
        bom.setRowCount(0);
        if (group_by_value->isChecked())
                {
                bom.setColumnWidth(0, bom.width()*2/16);
                bom.setColumnWidth(1, bom.width()*4/16);
                }
        else
                {
                bom.setColumnWidth(0, bom.width()*4/16);
                bom.setColumnWidth(1, bom.width()*7/16);
                }
        QStringList	sl;
        if (group_by_value->isChecked())
                sl << QObject::tr("Quantity");
        sl << QObject::tr("Ident");
        sl << QObject::tr("Value");
        if (!group_by_value->isChecked())
                sl << QObject::tr("Document") ;
        bom.setHorizontalHeaderLabels(sl);
        QHeaderView *vh;
        QHeaderView *hh;
        vh = bom.verticalHeader();
        hh = bom.horizontalHeader();
        hh->setStretchLastSection(TRUE);
        vh->setResizeMode(QHeaderView::Fixed);

        if (file_type == FT_PROJECT)
                {
                for(int i = 0; i < w_main->tw->count(); i++)
                        if (w_main->doc_type(w_main->tw->widget(i)) == DT_SCH)
                                {
                                myGraphicsView          *GVi = (myGraphicsView *) w_main->tw->widget(i);
                                QTableWidgetItem *quantity;
                                QTableWidgetItem *ident;
                                QTableWidgetItem *value;
                                QTableWidgetItem *document;
                                foreach(myComponent *comp, GVi->all_comp)
                                        {
                                        if (group_by_value->isChecked())
                                                {
                                                bool new_item = TRUE;
                                                for (int i= 0; i < bom.rowCount(); i++)
                                                        if (bom.item(i, 2)->text() == comp->value)
                                                                {
                                                                quantity = bom.item(i, 0);
                                                                ident = bom.item(i, 1);
                                                                quantity->setText(QString::number(quantity->text().toInt() + 1));
                                                                ident->setText(ident->text() + ", " + comp->ident);
                                                                new_item = FALSE;
                                                                break;
                                                                }

                                                if (new_item)
                                                        {
                                                        bom.insertRow(0);
                                                        quantity = new QTableWidgetItem("");
                                                        ident = new QTableWidgetItem("");
                                                        value = new QTableWidgetItem("");

                                                        quantity->setText("1");
                                                        ident->setText(comp->ident);
                                                        value->setText(comp->value);

                                                        bom.setItem(0, 0, quantity);
                                                        bom.setItem(0, 1, ident);
                                                        bom.setItem(0, 2, value);
                                                        }
                                                }
                                        else
                                                {
                                                bom.insertRow(0);
                                                ident = new QTableWidgetItem("");
                                                value = new QTableWidgetItem("");
                                                document = new QTableWidgetItem("");

                                                ident->setText(comp->ident);
                                                value->setText(comp->value);
                                                document->setText(GVi->name);

                                                bom.setItem(0, 0, ident);
                                                bom.setItem(0, 1, value);
                                                bom.setItem(0, 2, document);
                                                }
                                        }
                                }
                }
        bom.setSortingEnabled(TRUE);
}
