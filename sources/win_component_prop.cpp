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

#include "win_component_prop.h"

extern	win_main		*w_main;
extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

extern	char file_type;	

extern	preview	*comp_preview;

extern	myGraphicsView	*preview_GV;
extern	myGraphicsScene	*preview_GS;

extern  QList<myGraphicsView *> doc_sch_and_pcb;

extern	QString	directory_pff;

win_component_prop::win_component_prop(QWidget *parent, Qt::WFlags tipo, myComponent *comp_input, bool id_is_visible_input)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

        setWindowTitle(tr("Properties"));
	//setSizeGripEnabled(true);
	setModal(true);

	//Imposto la larghezza minima pari a metà dello schermo
	//QRect screen = QApplication::desktop()->screenGeometry();		//Non serve perchè uso tw->setUsesScrollButtons(FALSE); 
	//setMinimumWidth(screen.width() / 3);							//Non serve perchè uso tw->setUsesScrollButtons(FALSE); 

	comp = comp_input;
        alias_is_visible = id_is_visible_input;

	QGridLayout	*layout = new QGridLayout(this);
	tw = new QTabWidget();
	tw->setUsesScrollButtons(FALSE);
	tw->addTab(setup_general(), tr("General"));
	if (GV->is_sch)
                tw->addTab(setup_ungated_pins(), tr("Ungated pins"));
        layout->addWidget(tw,1,0);
	layout->addLayout(setup_buttons(),2,0);
	layout->setSpacing(6);
        layout->setMargin(8);
}

QWidget *win_component_prop::setup_general()
{
	QWidget *w = new QWidget();	
	QGridLayout *gbox_group = new QGridLayout();

	QGroupBox	*group_component = new QGroupBox(tr("Component"));
	QGridLayout	*layout_component = new QGridLayout();
	if (alias_is_visible)
		{
		QLabel *Alias = new QLabel(this);
		Alias->setText(tr("Alias"));
		alias = new QComboBox(this);
		alias->setEditable(TRUE);
                //E' meglio proporre poche lettere, ma sicure
                //Inoltre conviene proporre solo una lettera così l'elenco è più bello
		alias->addItem(tr("C   (Capacitor)"));
                alias->addItem(tr("D   (Diode)"));      //Il Led appartiene ai diodi
		alias->addItem(tr("F   (Fuse)"));
                alias->addItem(tr("H   (Header)"));     //Negli Header ci metto i connettori
		alias->addItem(tr("L   (Inductor)"));
                alias->addItem(tr("Q   (Fet)"));
                alias->addItem(tr("R   (Resistor)"));
                alias->addItem(tr("S   (Switch)"));
                //alias->addItem(tr("SSR (Solid State Relay)"));
                alias->addItem(tr("T   (Transistor)"));
                //alias->addItem(tr("TR  (Transformer)"));
		alias->addItem(tr("U   (IC)"));
		alias->addItem(tr("V   (Battery)"));
		alias->addItem(tr("X   (Crystal)"));
                alias->addItem(tr("K   (Relay)"));
		alias->setEditText(comp->alias);
		connect(alias,SIGNAL(currentIndexChanged(int)),this,SLOT(alias_index_changed(int)));
		layout_component->addWidget(Alias, 0, 0);	
		layout_component->addWidget(alias, 0, 1);	
		}
	else
		{
                QLabel	*Ident = new QLabel(this);
                Ident->setText(tr("Ident"));
                ident = new QLineEdit(this);
                ident->setText(comp->ident);
                unicode_input_ident = new QToolButton(this);
                unicode_input_ident->setIcon(QIcon(":/Resources/keyboard.png"));
                unicode_input_ident->setAutoRaise(TRUE);
                unicode_input_ident->setCheckable(FALSE);
                connect(unicode_input_ident,SIGNAL(clicked()),this,SLOT(unicode_input_ident_pressed()));
                layout_component->addWidget(Ident, 0, 0);
                layout_component->addWidget(ident, 0, 1);
                layout_component->addWidget(unicode_input_ident, 0, 2);
		}

	if (file_type == FT_PROJECT)
		{
		QLabel	*Value = new QLabel(this);
		Value->setText(tr("Value"));
		value = new QLineEdit(this);
		value->setText(comp->value);
		unicode_input_value = new QToolButton(this);
		unicode_input_value->setIcon(QIcon(":/Resources/keyboard.png"));
		unicode_input_value->setAutoRaise(TRUE);
		unicode_input_value->setCheckable(FALSE);
		connect(unicode_input_value,SIGNAL(clicked()),this,SLOT(unicode_input_value_pressed()));
		layout_component->addWidget(Value, 1, 0);	
		layout_component->addWidget(value, 1, 1);
		layout_component->addWidget(unicode_input_value, 1, 2);
		}

	group_component->setLayout(layout_component);
	gbox_group->addWidget(group_component, 0, 0);	

	if (GV->is_sch)
		{
		QGroupBox	*group_show = new QGroupBox(tr("Show"));
		QGridLayout	*layout_show = new QGridLayout();
		number_is_visible = new QCheckBox(tr("Pin numbers are visible"));
		number_is_visible->setChecked(comp->pin_number_is_visible);
		name_is_visible = new QCheckBox(tr("Pin names are visible (IC style)"));
		name_is_visible->setChecked(comp->pin_name_is_visible);
		layout_show->addWidget(number_is_visible, 0, 0);
		layout_show->addWidget(name_is_visible, 1, 0);
		group_show->setLayout(layout_show);
		gbox_group->addWidget(group_show, 1, 0);	

                if (file_type == FT_PROJECT)
                        {
                        QGroupBox	*group_footprint = new QGroupBox(tr("Footprint"));
                        QGridLayout	*layout_footprint = new QGridLayout();
                        fp_name = new QLineEdit(comp->footprint_link);
                        QToolButton	*footprint_change = new QToolButton(this);
                        footprint_change->setIcon(QIcon(":/Resources/folder.png"));
                        footprint_change->setAutoRaise(TRUE);
                        footprint_change->setCheckable(FALSE);
                        connect(footprint_change, SIGNAL(clicked()), this, SLOT(browse()));
                        layout_footprint->addWidget(fp_name, 0, 0);
                        layout_footprint->addWidget(footprint_change, 0, 1);
                        layout_footprint->setColumnStretch(0,1);
                        layout_footprint->setColumnStretch(1,0);
                        group_footprint->setLayout(layout_footprint);

                        //gbox_group->setRowStretch(1,TRUE);
                        gbox_group->addWidget(group_footprint, 2, 0);//, FALSE, TRUE);
                        }
                else
                        {
                        QGroupBox   *group_d = new QGroupBox(tr("Devices"));
                        QGridLayout *layout_d = new QGridLayout();

                        devices_table = new QTableWidget(0, 2, this);
                        devices_table_load();
                        QStringList		sl;
                        sl << QObject::tr("Device");
                        sl << QObject::tr("Footprint") ;
                        devices_table->setHorizontalHeaderLabels(sl);
                        devices_table->setSelectionBehavior(QAbstractItemView::SelectRows);
                        devices_table->setSelectionMode(QAbstractItemView::SingleSelection);
                        devices_table->setContextMenuPolicy(Qt::CustomContextMenu);
                        devices_table->setEditTriggers(QAbstractItemView::NoEditTriggers);	//Le caselle non sono editabili
                        devices_table->setSortingEnabled(FALSE);
                        QHeaderView *vh;
                        QHeaderView *hh;
                        vh = devices_table->verticalHeader();
                        vh->setHidden(TRUE);
                        hh = devices_table->horizontalHeader();
                        hh->setStretchLastSection(TRUE);
                        hh->setResizeMode(QHeaderView::Fixed);
                        vh->setResizeMode(QHeaderView::Fixed);

                        QPushButton	*devices_add = new QPushButton(this);
                        devices_add->setText(tr("Add"));
                        devices_add->setIcon(QIcon(":/Resources/add.png"));
                        QObject::connect(devices_add, SIGNAL(clicked()), this, SLOT(devices_add()));
                        QPushButton	*devices_remove = new QPushButton(this);
                        devices_remove->setText(tr("Remove"));
                        devices_remove->setIcon(QIcon(":/Resources/delete.png"));
                        QObject::connect(devices_remove, SIGNAL(clicked()), this, SLOT(devices_remove()));
                        QPushButton	*devices_edit = new QPushButton(this);
                        devices_edit->setText(tr("Edit"));
                        devices_edit->setIcon(QIcon(":/Resources/textfield_rename.png"));
                        QObject::connect(devices_edit, SIGNAL(clicked()), this, SLOT(devices_edit()));
                        QSpacerItem	*devices_molla = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

                        layout_d->addWidget(devices_table, 0, 0, FALSE, TRUE);
                        layout_d->addWidget(devices_add, 0, 1);
                        layout_d->addWidget(devices_remove, 1, 1);
                        layout_d->addWidget(devices_edit, 2, 1);
                        layout_d->addItem(devices_molla, 3, 1);
                        group_d->setLayout(layout_d);

                        gbox_group->addWidget(group_d, 2, 0);//, FALSE, TRUE);
                        }
                }
	else
		if (comp->idstamp > 0)
			{
			//Il componente non è modificabile. Può essere solo modificato da schematico eseguendo dopo update_netlist()
			w->setEnabled(FALSE);
			}	
	w->setLayout(gbox_group);
	return w;
}

QWidget *win_component_prop::setup_ungated_pins()
{
	QWidget *w = new QWidget();	
	QGridLayout *gbox_group = new QGridLayout();

	up_table = new QTableWidget(0, 2, this);
	up_table_load();
	QStringList		sl;
	sl << QObject::tr("Pin");
	sl << QObject::tr("Net") ;	
	up_table->setHorizontalHeaderLabels(sl);
	up_table->setSelectionBehavior(QAbstractItemView::SelectRows);
	up_table->setSelectionMode(QAbstractItemView::SingleSelection);
	up_table->setContextMenuPolicy(Qt::CustomContextMenu);
	up_table->setEditTriggers(QAbstractItemView::NoEditTriggers);	//Le caselle non sono editabili	
	up_table->setSortingEnabled(FALSE);
	QHeaderView *vh;
	QHeaderView *hh;   	
	vh = up_table->verticalHeader();
	vh->setHidden(TRUE);
	hh = up_table->horizontalHeader();
	hh->setStretchLastSection(TRUE);
	hh->setResizeMode(QHeaderView::Fixed);
	vh->setResizeMode(QHeaderView::Fixed);

	QPushButton	*up_add = new QPushButton(this);
	up_add->setText(tr("Add"));
	up_add->setIcon(QIcon(":/Resources/add.png"));
	QObject::connect(up_add, SIGNAL(clicked()), this, SLOT(ungated_pins_add()));
	QPushButton	*up_remove = new QPushButton(this);
	up_remove->setText(tr("Remove"));
	up_remove->setIcon(QIcon(":/Resources/delete.png"));
	QObject::connect(up_remove, SIGNAL(clicked()), this, SLOT(ungated_pins_remove()));
	QPushButton	*up_edit = new QPushButton(this);
	up_edit->setText(tr("Edit"));
	up_edit->setIcon(QIcon(":/Resources/textfield_rename.png"));
	QObject::connect(up_edit, SIGNAL(clicked()), this, SLOT(ungated_pins_edit()));
	QSpacerItem	*up_molla = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
	
        if (file_type == FT_PROJECT)
                {
                up_add->setEnabled(FALSE);
                up_remove->setEnabled(FALSE);
                }

	gbox_group->addWidget(up_table, 0, 0, FALSE, TRUE);
	gbox_group->addWidget(up_add, 0, 1);
	gbox_group->addWidget(up_remove, 1, 1);
	gbox_group->addWidget(up_edit, 2, 1);
	gbox_group->addItem(up_molla, 3, 1);

	w->setLayout(gbox_group);
	return w;
}

QHBoxLayout	*win_component_prop::setup_buttons()
	{
	QHBoxLayout	*L_comandi = new QHBoxLayout();
	L_comandi->setSpacing(6);
	L_comandi->setMargin(0);
        QSpacerItem	*L_comandi_molla = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	L_comandi->addItem(L_comandi_molla);
	QPushButton	*L_comandi_OK = new QPushButton(this);
	L_comandi_OK->setText(tr("Ok"));
	QObject::connect(L_comandi_OK, SIGNAL(clicked()), this, SLOT(accept()));
	L_comandi->addWidget(L_comandi_OK);
	QPushButton	*L_comandi_Cancel = new QPushButton(this);
	L_comandi_Cancel->setText(tr("Cancel"));
	L_comandi->addWidget(L_comandi_Cancel);
	QObject::connect(L_comandi_Cancel, SIGNAL(clicked()), this, SLOT(reject()));

        L_comandi_OK->setAutoDefault(TRUE);
	L_comandi_Cancel->setAutoDefault(FALSE);

	return L_comandi;
	}

void	win_component_prop::alias_index_changed(int index)
{
	//Eseguito quando si cambia indice sulla ComboBox e quando si da l'ok alla maschera.
	QString text = alias->currentText();
	text = text.trimmed();	//Elimino gli spazi all'inizio e alla fine
	text = text.left(text.indexOf(' '));
	alias->setEditText(text);
}

bool	win_component_prop::is_modified()
{
	if (alias_is_visible)
		{
		if (new_alias != comp->alias)
			return TRUE;
		}
	else
		{
                if (new_ident != comp->ident)
			return TRUE;
		if (new_value != comp->value)
			return TRUE;
		}

	if (GV->is_sch)
		{
		if (new_pin_number_is_visible != comp->pin_number_is_visible)
			return TRUE;
		if (new_pin_name_is_visible != comp->pin_name_is_visible)
			return TRUE;
                if (new_up != comp->up)
                        return TRUE;
                if (file_type == FT_PROJECT)
                        {
                        if (new_footprint_link != comp->footprint_link)
                                return TRUE;
                        }
                else
                        {
                        if (new_devices != comp->devices)
                                return TRUE;
                        }
		}

	return FALSE;
}

void	win_component_prop::accept()
{
	if (GV->is_sch && comp->idstamp < 0)
		{
		//Il componente non è modificabile. Può essere solo modificato da schematico eseguendo dopo update_netlist()
		QDialog::reject();
		}
	else
		{
		if (alias_is_visible)
			{
			alias_index_changed(alias->currentIndex());
			if (alias->currentText().isEmpty()) 
				{
				QMessageBox::warning(0,tr("Warning"),tr("Alias can't be empty"));
				return;
				}
			new_alias = alias->currentText();
                        new_ident = comp->ident;
			new_value = comp->value;
			}
		else
			{
                        if (ident->text().isEmpty())
				{
				QMessageBox::warning(0,tr("Warning"),tr("Name can't be empty"));
				return;
				}	
                        for (int i = 0; i < doc_sch_and_pcb.count(); i++)
                                {
                                myGraphicsView *GVi = (myGraphicsView *) doc_sch_and_pcb.at(i);
                                myComponent* result = GVi->find_comp(ident->text());
                                if (result)
                                        if ((result != comp) && (((myComponent*) result->idstamp_pointer) != comp))
                                                 {
                                                 QMessageBox::warning(0,tr("Warning"),tr("Duplicated Ident. Change it."));
                                                 return;
                                                 }
                                }

                        new_alias = comp->alias;
                        new_ident = ident->text();
			new_value = value->text();
			}

		if (GV->is_sch)
			{
			new_pin_number_is_visible = number_is_visible->isChecked();
			new_pin_name_is_visible = name_is_visible->isChecked();
			new_up = "";
			for (int i = 0; i < up_table->rowCount(); i++)
				{
				if (!new_up.isEmpty())
                                        new_up += ";";
                                new_up += up_table->item(i, 0)->text() + ";" + up_table->item(i, 1)->text();
				}
                        if (file_type == FT_PROJECT)
                                new_footprint_link = fp_name->text();
                        else
                                {
                                new_devices = "";
                                for (int i = 0; i < devices_table->rowCount(); i++)
                                        {
                                        if (!new_devices.isEmpty())
                                                new_devices += ";";
                                        new_devices += devices_table->item(i, 0)->text() + ";" + devices_table->item(i, 1)->text();
                                        }
                                }
			}

		QDialog::accept(); 
		}
}

void	win_component_prop::browse()
{
	win_add_component dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, TRUE);
        dialogo.filepath_write(fp_name->text());
        if (dialogo.exec())
                {
                if (comp_preview->load(directory_pff + "/" + dialogo.filepath_read(), file_type, FALSE))
                        {
                        if (!comp->footprint_compatibility(preview_GV->all_comp.first()))
                            {
                            QMessageBox::warning(0,tr("Warning"),tr("Wrong footprint: pad numbers incompatible."));
                            return;
                            }
                        }
                fp_name->setText(dialogo.filepath_read());
                }


}

void	win_component_prop::unicode_input_ident_pressed()
{
        win_unicode_input dialogo(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint, ident->mapToGlobal(QPoint(0,ident->height())));
	dialogo.raise();
        dialogo.activateWindow();
        dialogo.set_lineedit(ident);
	if (dialogo.exec())
		{
		
		}
}

void	win_component_prop::unicode_input_value_pressed()
{
        win_unicode_input dialogo(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint, value->mapToGlobal(QPoint(0,value->height())));
	dialogo.raise();
        dialogo.activateWindow();
        dialogo.set_lineedit(value);
	if (dialogo.exec())
		{
		
		}
}

void	win_component_prop::ungated_pins_add()
{
        if (devices_table->rowCount())
                for (int i = 0; i < devices_table->rowCount(); i++)
                        if (!devices_table->item(i, 1)->text().isEmpty())
                                {
                                QMessageBox::warning(0,tr("Warning"),tr("Please remove footprint devices.\nIf you add new ungated pins then the pad numbers will be incompatible."));
                                return;
                                }

        win_ungated_pins_edit dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, -1, "", "");
	if (dialogo.exec())
		{
		QTableWidgetItem *new_pin = new QTableWidgetItem(dialogo.pin->text());
		QTableWidgetItem *new_net = new QTableWidgetItem(dialogo.net->text());
		up_table->insertRow(0);
		up_table->setItem(0, 0, new_pin);
		up_table->setItem(0, 1, new_net);
		up_table->resizeRowToContents(0);

		up_table->sortByColumn(0, Qt::AscendingOrder);
		}
}

void	win_component_prop::ungated_pins_edit()
{
	int cr = up_table->currentRow();

	if (cr >= 0)
		{
                win_ungated_pins_edit dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, cr, up_table->item(cr,0)->text(), up_table->item(cr,1)->text());
		if (dialogo.exec())
			{
			QTableWidgetItem *new_pin = new QTableWidgetItem(dialogo.pin->text());
			QTableWidgetItem *new_net = new QTableWidgetItem(dialogo.net->text());
			up_table->setItem(cr, 0, new_pin);
			up_table->setItem(cr, 1, new_net);
			up_table->resizeRowToContents(cr);

			up_table->sortByColumn(0, Qt::AscendingOrder);
			}
		}
}

void	win_component_prop::ungated_pins_remove()
{
        int cr = up_table->currentRow();

	if (cr >= 0)
                {
                if (devices_table->rowCount())
                        for (int i = 0; i < devices_table->rowCount(); i++)
                                if (!devices_table->item(i, 1)->text().isEmpty())
                                        {
                                        QMessageBox::warning(0,tr("Warning"),tr("Please remove footprint devices.\nIf you add new ungated pins then the pad numbers will be incompatible."));
                                        return;
                                        }

                QString question = tr("Do you want to remove pin=%1 net=%2 ?").arg(up_table->item(cr,0)->text()).arg(up_table->item(cr,1)->text());

		switch (QMessageBox::warning(this, "Itecad", question, tr("&Yes"), tr("&No"), 0))
		{
		case 0:		// 0 = YES
			up_table->removeRow(cr);
			break;
		case 1:		// 1 = NO
			break;
		}		
		}
}

void	win_component_prop::devices_add()
{
        schcomp_or_pcbcomp_update_with_new_data();

        win_devices_edit dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, -1, "", "");
        if (dialogo.exec())
                {
                QTableWidgetItem *new_device = new QTableWidgetItem(dialogo.device->text());
                QTableWidgetItem *new_footprint = new QTableWidgetItem(dialogo.footprint->text());
                devices_table->insertRow(0);
                devices_table->setItem(0, 0, new_device);
                devices_table->setItem(0, 1, new_footprint);
                devices_table->resizeRowToContents(0);

                devices_table->sortByColumn(0, Qt::AscendingOrder);
                }
}

void	win_component_prop::devices_edit()
{
        int cr = devices_table->currentRow();

        if (cr >= 0)
                {
                schcomp_or_pcbcomp_update_with_new_data();

                win_devices_edit dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, cr, devices_table->item(cr,0)->text(), devices_table->item(cr,1)->text());
                if (dialogo.exec())
                        {
                        QTableWidgetItem *new_device = new QTableWidgetItem(dialogo.device->text());
                        QTableWidgetItem *new_footprint = new QTableWidgetItem(dialogo.footprint->text());
                        devices_table->setItem(cr, 0, new_device);
                        devices_table->setItem(cr, 1, new_footprint);
                        devices_table->resizeRowToContents(cr);

                        devices_table->sortByColumn(0, Qt::AscendingOrder);
                        }
                }
}

void	win_component_prop::devices_remove()
{
        int cr = devices_table->currentRow();

        if (cr >= 0)
                {
                QString question = tr("Do you want to remove device=%1 footprint=%2 ?").arg(devices_table->item(cr,0)->text()).arg(devices_table->item(cr,1)->text());

                switch (QMessageBox::warning(this, "Itecad", question, tr("&Yes"), tr("&No"), 0))
                {
                case 0:		// 0 = YES
                        devices_table->removeRow(cr);
                        break;
                case 1:		// 1 = NO
                        break;
                }
                }
}

void	win_component_prop::up_table_load()
{
	up_table->clear();

        QStringList sl_up = comp->up.split(";", QString::SkipEmptyParts);
        for (int i = 0; i < sl_up.count(); i +=2)
		{
                QTableWidgetItem *new_pin = new QTableWidgetItem(sl_up.at(i));
                QTableWidgetItem *new_net = new QTableWidgetItem(sl_up.at(i+1));
		up_table->insertRow(0);
		up_table->setItem(0, 0, new_pin);
		up_table->setItem(0, 1, new_net);
		up_table->resizeRowToContents(0);
		}

	up_table->sortByColumn(0, Qt::AscendingOrder);
}

void	win_component_prop::devices_table_load()
{
        devices_table->clear();

        QStringList sl_devices = comp->devices.split(";", QString::SkipEmptyParts);
        for (int i = 0; i < sl_devices.count(); i +=2)
                {
                QTableWidgetItem *new_device = new QTableWidgetItem(sl_devices.at(i));
                QTableWidgetItem *new_footprint = new QTableWidgetItem(sl_devices.at(i+1));
                devices_table->insertRow(0);
                devices_table->setItem(0, 0, new_device);
                devices_table->setItem(0, 1, new_footprint);
                devices_table->resizeRowToContents(0);
                }

        devices_table->sortByColumn(0, Qt::AscendingOrder);
}

void    win_component_prop::schcomp_or_pcbcomp_update_with_new_data()
{
        //Aggiorno schcomp_or_pcbcomp con i nuovi dati che si vogliono inserire
        GV->schcomp_or_pcbcomp->all_obj.clear();
        foreach (myGraphicsItem *obj, GS->all_obj)
                GV->schcomp_or_pcbcomp->all_obj.append(obj);
        QString new_up = "";
        for (int i = 0; i < up_table->rowCount(); i++)
                {
                if (!new_up.isEmpty())
                        new_up += ";";
                new_up += up_table->item(i, 0)->text() + ";" + up_table->item(i, 1)->text();
                }
        GV->schcomp_or_pcbcomp->up = new_up;
}

