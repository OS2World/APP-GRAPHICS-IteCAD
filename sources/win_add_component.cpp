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

#include "win_add_component.h"

extern	win_main	*w_main;
extern	char		file_type;
extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;
extern	QString		directory_ssf;
extern	QString		directory_pff;

extern	myGraphicsView	*preview_GV;
extern	myGraphicsScene	*preview_GS;

extern	preview	*comp_preview;

extern	QPoint	drag_or_move_delta;

extern	QPoint	mouse_move;

extern  QList<myGraphicsItem *>     cloned_obj;
extern  QList<myComponent *>        cloned_comp;
extern  QList<node_data *>          cloned_nodes;
extern  char                        cloned_action;

win_add_component::win_add_component(QWidget *parent, Qt::WFlags tipo, bool footprint_change)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

	fp_change = footprint_change;	//Il fottprint change si ha quando si cambia il footprint di un componente
	
	if (fp_change)
		setWindowTitle(tr("Choose a footprint"));	
	else
		setWindowTitle(w_main->actions_addAct->text());	
	//setSizeGripEnabled(true);
	setModal(true);

        sch_directory = directory_ssf;
        pcb_directory = directory_pff;
        if (GV->is_sch && !fp_change)
                {
                is_sch = TRUE;
                root_directory = &sch_directory;
                }
        else
		{
		is_sch = FALSE;
                root_directory = &pcb_directory;
                }

	QGridLayout	*layout = new QGridLayout(this);

	layout->addLayout(setup_buttons(), 1, 0, TRUE, FALSE);
	layout->addWidget(setup_part(), 0, 0);
	layout->addWidget(setup_preview(), 0, 1);
	

	layout->setSpacing(6);
        layout->setMargin(8);
}

QGroupBox *win_add_component::setup_part()
{
	QGroupBox *group_part = new QGroupBox(tr("Part"));
	QGridLayout	*layout_part = new QGridLayout();

        components = new QTreeWidget();
	components->setSelectionMode(QAbstractItemView::SingleSelection);	
	components->setEditTriggers(QAbstractItemView::NoEditTriggers);	//Le caselle non sono editabili
        connect(components,SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),this,SLOT(component_changed(QTreeWidgetItem *, QTreeWidgetItem *)));

        QHBoxLayout *layout_filter = new QHBoxLayout();
	filter = new QCheckBox(tr("Filter components whose names contain"));
	filter_text = new QLineEdit();
	filter_text->setText("*");
	filter_text->setEnabled(FALSE);
	connect(filter_text,SIGNAL(editingFinished()),this,SLOT(apply_filter()));
	connect(filter,SIGNAL(stateChanged(int)),this,SLOT(apply_filter(int)));
	layout_filter->addWidget(filter);
	layout_filter->addWidget(filter_text);

        //QSpacerItem	*L_comandi_molla = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Maximum);

        layout_part->addWidget(components, 0, 0, TRUE, FALSE);
        layout_part->addLayout(layout_filter, 1, 0, TRUE, FALSE);
        //layout_part->addItem(L_comandi_molla, 3, 0);
        group_part->setLayout(layout_part);

        components->setIconSize(comp_preview->size());

	QRect screen = QApplication::desktop()->screenGeometry();
	group_part->setMinimumHeight(screen.height()/2);
		
        all_row.clear();
        QStringList		sl;
        if (is_sch)
                {
                components->setColumnCount(2);
                sl << QObject::tr("Symbol");
                }
        else
                {
                components->setColumnCount(1);
                }
        sl << QObject::tr("Footprint") ;
        components->setHeaderLabels(sl);
        components->setFixedWidth(500);
        if (is_sch)
                {
                components->header()->resizeSection(0, 300);
                components->header()->setStretchLastSection(TRUE);
                }
        load_components(0, root_directory);
        components->sortByColumn(0, Qt::AscendingOrder);

	return group_part;
}

QGroupBox *win_add_component::setup_preview()
{
	QGroupBox *groupBox = new QGroupBox(tr("Preview"));
	QGridLayout	*layout_preview = new QGridLayout();

        preview_label = new QLabel(this);
        preview_label->setFixedSize(comp_preview->size());
        preview_label2 = new QLabel(this);
        preview_label2->setFixedSize(comp_preview->size());
        layout_preview->addWidget(preview_label, 0, 0);
        layout_preview->addWidget(preview_label2, 1, 0);

        groupBox->setLayout(layout_preview);
	load_preview();
	
	return groupBox;
}

QHBoxLayout	*win_add_component::setup_buttons()
{
	QHBoxLayout	*L_comandi = new QHBoxLayout();
	L_comandi->setSpacing(6);
	L_comandi->setMargin(0);
        QSpacerItem	*L_comandi_molla = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	L_comandi->addItem(L_comandi_molla);
	L_comandi_Add = new QPushButton(this);
	if (fp_change)
		L_comandi_Add->setText(tr("Ok"));
	else
		L_comandi_Add->setText(tr("Add"));
	L_comandi->addWidget(L_comandi_Add);	
	QPushButton	*L_comandi_Cancel = new QPushButton(this);
	L_comandi_Cancel->setText(tr("Cancel"));
	L_comandi->addWidget(L_comandi_Cancel);
	
	QObject::connect(L_comandi_Add, SIGNAL(clicked()), this, SLOT(accept()));
	QObject::connect(L_comandi_Cancel, SIGNAL(clicked()), this, SLOT(reject()));

	L_comandi_Add->setAutoDefault(FALSE);
	L_comandi_Cancel->setAutoDefault(FALSE);

	return L_comandi;
}

void	win_add_component::accept()
{
	if (fp_change)
		{
		
		}
	else
		{
		w_main->tool_avviato = TRUE;

                GV->setUpdatesEnabled(FALSE);
                GV->select_clear();
		GV->drag_or_move_setup('R');
                preview_GV->select_all();
                preview_GV->clone_from_selection();
                if (!components->currentItem()->text(1).isNull())
                        {
                        cloned_comp.first()->value = components->currentItem()->text(0);
                        cloned_comp.first()->footprint_link = components->currentItem()->text(2);
                        }
                mouse_move = GV->mapToScene(GV->mapFromGlobal(QCursor::pos())).toPoint();
                GV->clone_move(GV->grid_delta(mouse_move), TRUE);
                GV->clone_ident_pin_pad_recalculate();
                cloned_action = 'A';
                GV->setUpdatesEnabled(TRUE);
		}
	
	QDialog::accept();
}

void	win_add_component::reject()
{
	if (!fp_change)
		GV->tool_change_to_default();

	QDialog::reject();
}

void	win_add_component::apply_filter(int inutile)
{
        filter_text->setEnabled(filter->isChecked());
        apply_filter();
}

void    win_add_component::show_up(QTreeWidgetItem *item)
{
        item->setHidden(FALSE);
        if (item->parent())
                show_up(item->parent());
}

void    win_add_component::show_down(QTreeWidgetItem *item)
{
        item->setHidden(FALSE);
        for (int i = 0; i < item->childCount(); i++)
                show_down(item->child(i));
}

void	win_add_component::apply_filter()
{
        if (filter->isChecked())
                {
                //Rendo prima tutto hidden
                for (int i = 0; i < all_row.size(); ++i)
                        all_row.at(i)->setHidden(TRUE);

                //Rendo visibile tutto ciò che lo deve essere
                QRegExp exp_filter(filter_text->text(), Qt::CaseInsensitive, QRegExp::Wildcard);
                for (int i = 0; i < all_row.size(); ++i)
                        {
                        if (all_row.at(i)->isHidden())
                                if (exp_filter.exactMatch(all_row.at(i)->text(0)))
                                        {
                                        show_up(all_row.at(i));
                                        show_down(all_row.at(i));
                                        }
                        }
                }
        else
                {
                //Rendo prima tutto hidden
                for (int i = 0; i < all_row.size(); ++i)
                        all_row.at(i)->setHidden(FALSE);
                }

        load_preview();
}

void	win_add_component::load_components(QTreeWidgetItem *father, QDir *dir)
{
        QDirIterator it(*dir, QDirIterator::NoIteratorFlags);
        QFileInfo fi;
        QTreeWidgetItem *item;
        while (it.hasNext())
                {
                it.next();
                fi = it.fileInfo();
                if (fi.isDir())
                        {
                        if ((fi.fileName() != ".") && (fi.fileName() != ".."))
                                {
                                if (father == 0)
                                        item = new QTreeWidgetItem(components);
                                else
                                        item = new QTreeWidgetItem(father);
                                item->setText(0, it.fileName());
                                load_components(item, &QDir(fi.filePath()));
                                }
                        }
                else
                        {
                        if (father == 0)
                                item = new QTreeWidgetItem(components);
                        else
                                item = new QTreeWidgetItem(father);
                        item->setText(0, fi.completeBaseName());
                        //Nella colonna 2 inserisco il percorso relativo dell'SCH o del PCB
                        item->setText(2, root_directory->relativeFilePath(it.filePath()));
                        if (is_sch)
                                {
                                //Apro il file e leggo device e footprint
                                xml_sax_handler_read_devices handler;
                                        if (handler.open_file(fi.filePath(), item, &all_row))
                                                {
                                                //Qui vengono aggiunte delle righe dentro item...
                                                }
                                }
                        all_row.append(item);
                        }
                }
}

void	win_add_component::component_changed(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	load_preview();
}

void	win_add_component::load_preview()
{
        bool exit = FALSE;

        if (components->currentItem() == 0)
                exit = TRUE;
        else
                {
                if (components->currentItem()->isHidden())
                    exit = TRUE;
                if (components->currentItem()->text(2).isNull())
                    exit = TRUE;
                }
        if (exit)
		{		
                comp_preview->load_pixmap("", preview_label, file_type, is_sch);
		L_comandi_Add->setEnabled(FALSE);
                if (GV->is_sch && !fp_change)
                        comp_preview->load_pixmap("", preview_label2, file_type, FALSE);
                return;
		}

        bool ok;
        if (GV->is_sch && !fp_change)
                {
                if (components->currentItem()->text(1).isNull())
                        {
                        comp_preview->load_pixmap("", preview_label2, file_type, FALSE);
                        ok = comp_preview->load_pixmap(root_directory->path() + "/" + components->currentItem()->text(2), preview_label, file_type, TRUE);
                        }
                else
                        {
                        comp_preview->load_pixmap(pcb_directory.path() + "/" + components->currentItem()->text(2), preview_label2, file_type, FALSE);
                        ok = comp_preview->load_pixmap(root_directory->path() + "/" + components->currentItem()->parent()->text(2), preview_label, file_type, TRUE);
                        }
                }
        else
                {
                ok = comp_preview->load_pixmap(root_directory->path() + "/" + components->currentItem()->text(2), preview_label, file_type, is_sch);
                }
        L_comandi_Add->setEnabled(ok);
}

QString win_add_component::filepath_read()
{
        return components->currentItem()->text(2);
}

void    win_add_component::filepath_write(QString fp)
{
        if (fp.isEmpty())
                return;

        QFileInfo fi(directory_pff + "/" + fp);
        if (fi.isFile())
                for (int i = 0; i < all_row.size(); ++i)
                        {
                        if (all_row.at(i)->text(2) == fp)
                                {
                                components->setCurrentItem(all_row.at(i));
                                break;
                                }
                        }
        else
                QMessageBox::warning(0,tr("Warning"),tr("%1\nnot found in PCB footprints directory.").arg(fp));

}
