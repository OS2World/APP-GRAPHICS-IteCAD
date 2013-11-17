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

#include "win_char_editor.h"
#include "math.h"

extern	win_main		*w_main;

extern	QString directory_ivf;

win_char_editor::win_char_editor(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

	///////////////////////////////////////////////
	//http://www.atm.ox.ac.uk/user/iwi/charmap.html
	///////////////////////////////////////////////

	setModal(true);	

	QGridLayout	*layout = new QGridLayout(this);
	
	layout->addWidget(setup_menuBar(),0,0);
	layout->addLayout(setup_editor(),1,0);
	layout->setSpacing(0);
    layout->setMargin(0);

	never_saved = TRUE;
	file = "New";
	setWindowTitle(QFileInfo(file).completeBaseName() + "[*] - Char Editor");

	actions_escapeAct = new QAction("", this);
	actions_escapeAct->setShortcut(Qt::Key_Escape);
    connect(actions_escapeAct, SIGNAL(triggered()), this, SLOT(actions_escape()));
	addAction(actions_escapeAct);	//Collego questa azione direttamente a win_main senza che essa abbia bottoni o menu associati
}

QMenuBar *win_char_editor::setup_menuBar()
{
	file_new_Act = new QAction(tr("New"), this); 
    connect(file_new_Act, SIGNAL(triggered()), this, SLOT(file_new()));

	file_open_Act = new QAction(tr("Open"), this); 
    connect(file_open_Act, SIGNAL(triggered()), this, SLOT(file_open()));

	file_saveAct = new QAction(tr("Save"), this); 
	file_saveAct->setEnabled(FALSE);
    connect(file_saveAct, SIGNAL(triggered()), this, SLOT(file_save()));

	file_save_asAct = new QAction(tr("Save &As..."), this); 
	file_save_asAct->setEnabled(TRUE);
    connect(file_save_asAct, SIGNAL(triggered()), this, SLOT(file_save_as()));

	file_exitAct = new QAction(tr("Exit"), this); 
    connect(file_exitAct, SIGNAL(triggered()), this, SLOT(close()));

	edit_undoAct = new QAction(tr("Undo"), this); 
	edit_undoAct->setShortcut(tr("Ctrl+Z"));
	connect(edit_undoAct, SIGNAL(triggered()), this, SLOT(edit_undo()));
	edit_undoAct->setEnabled(FALSE);
    
	edit_redoAct = new QAction(tr("Redo"), this); 
	edit_redoAct->setShortcut(tr("Ctrl+Y"));
	connect(edit_redoAct, SIGNAL(triggered()), this, SLOT(edit_redo()));
	edit_redoAct->setEnabled(FALSE);

	fileMenu = new QMenu(tr("&File"));
	fileMenu->addAction(file_new_Act);
	fileMenu->addSeparator();
	fileMenu->addAction(file_open_Act);
	fileMenu->addSeparator();
	fileMenu->addAction(file_saveAct);
	fileMenu->addAction(file_save_asAct);
	fileMenu->addSeparator();
	fileMenu->addAction(file_exitAct);
	connect(fileMenu,SIGNAL(aboutToShow()),this,SLOT(menu_file_open()));
	editMenu = new QMenu(tr("&Edit"));
	editMenu->addAction(edit_undoAct);
	editMenu->addAction(edit_redoAct);
	
	mb = new QMenuBar(this);
	mb->clear();
	mb->addMenu(fileMenu);
	mb->addMenu(editMenu);
	return mb;
}

QGridLayout	*win_char_editor::setup_editor()
{
	QGridLayout	*layout = new QGridLayout();
	
	QGroupBox *group_char = new QGroupBox(tr("Character"));
	QGridLayout	*layout_char = new QGridLayout();
	c = new char_editor(this);
        connect(&(c->ur), SIGNAL(canUndoChanged(bool)), this, SLOT(edit_can_undo_changed(bool)));
        connect(&(c->ur), SIGNAL(canRedoChanged(bool)), this, SLOT(edit_can_redo_changed(bool)));
	layout_char->addWidget(c);
	group_char->setLayout(layout_char);

	QGroupBox *group_mode = new QGroupBox(tr("Mode"));
	QGridLayout	*layout_mode = new QGridLayout();
	mode = new QLabel();
	mode->setPixmap(QPixmap(":/Resources/mode_draw.png"));
	tool_draw = new QRadioButton(tr("Draw mode"), this);
	tool_erase = new QRadioButton(tr("Erase mode"), this);
	tool_draw->setChecked(TRUE);
	QObject::connect(tool_erase, SIGNAL(toggled(bool)), this, SLOT(set_tool_erase(bool)));
	QObject::connect(tool_draw, SIGNAL(toggled(bool)), this, SLOT(set_tool_draw(bool)));
	layout_mode->addWidget(mode, 0, 0, FALSE, TRUE);
	layout_mode->addWidget(tool_draw, 0, 1);
	layout_mode->addWidget(tool_erase, 1, 1);
	layout_mode->setColumnStretch(1, TRUE);
	group_mode->setLayout(layout_mode);

	QGroupBox *group_margins = new QGroupBox(tr("Properties"));
	QGridLayout	*layout_margins = new QGridLayout();
	w = new QLabel(tr("Char width:"));	
	char_width = new QComboBox();
	for (int i = 1; i <= 20; i++)
		char_width->addItem(QString::number(i));
	connect(char_width, SIGNAL(currentIndexChanged(int)), this, SLOT(char_width_changed(int)));
	char_width->setCurrentIndex(10);	//Di default imposto la larghezza a 11 pixel
	layout_margins->addWidget(w, 0, 0);
	layout_margins->addWidget(char_width, 0, 1);
        group_margins->setLayout(layout_margins);

	QGroupBox *group_background = new QGroupBox(tr("Background Char"));
	QGridLayout	*layout_background = new QGridLayout();
	background = new QCheckBox(tr("Show this char in the background:"), this);
	connect(background, SIGNAL(stateChanged(int)), this, SLOT(background_on_off(int)));
	bg_list = new QComboBox();
	bg_list_fill();
	connect(bg_list, SIGNAL(currentIndexChanged(int)), this, SLOT(background_changed(int)));
	bg_list->setEnabled(FALSE);
	layout_background->addWidget(background, 0, 0, TRUE, FALSE);
	layout_background->addWidget(bg_list, 1, 1);
	group_background->setLayout(layout_background);
		
	layout->addWidget(group_mode,0,0);
	layout->addWidget(group_margins,1,0);
	layout->addWidget(group_background,2,0);
	layout->addWidget(group_char,0,1, FALSE, TRUE);
	layout->setRowStretch(3, TRUE);

	layout->setSpacing(6);
    layout->setMargin(8);
		
	return layout;
}

void	win_char_editor::background_on_off(int state)
{
	bg_list->setEnabled(background->isChecked());
	if (background->isChecked())
		background_changed(bg_list->currentIndex());
	else
		{
		c->bg_cs.char_lines.clear();
		c->bg_cs.char_points.clear();
		}
	c->setUpdatesEnabled(FALSE);
	c->setUpdatesEnabled(TRUE);
}

void	win_char_editor::background_changed(int index)
{
	QString bg_file = bg_list->itemText(index);
	if (bg_file == "")
		return;
        bg_file = directory_ivf + "/" + bg_file.mid(3) + ".chr";
	if (!bg_file.isEmpty())
		if (w_main->char_open(bg_file, &(c->bg_cs), TRUE))
			{
			c->setUpdatesEnabled(FALSE);
			c->setUpdatesEnabled(TRUE);
			}
}

void	win_char_editor::set_tool_erase(bool flag)
{
	mode->setPixmap(QPixmap(":/Resources/mode_erase.png"));
	c->mode = 'E';
	c->clicked = FALSE;
	c->sel_line = -1;
	c->setUpdatesEnabled(FALSE);
	c->setUpdatesEnabled(TRUE);
}

void	win_char_editor::set_tool_draw(bool flag)
{
	mode->setPixmap(QPixmap(":/Resources/mode_draw.png"));
	c->mode = 'D';
	c->clicked = FALSE;
	c->sel_line = -1;
	c->setUpdatesEnabled(FALSE);
	c->setUpdatesEnabled(TRUE);
}

void	win_char_editor::bg_list_fill()
{
	bg_list->clear();
        QDir dir = QDir(directory_ivf);
	QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) 
		{
		QFileInfo fi = list.at(i);
		
		if (fi.isFile())
			if (fi.completeBaseName().length() == 4)
				{
				bool ok;
				bg_list->addItem(QString(QChar(fi.completeBaseName().toInt(&ok, 16))) + "  " + fi.baseName());
				}
        }
	
}

void	win_char_editor::edit_undo()
{
	c->ur.undo();
}

void	win_char_editor::edit_redo()
{
	c->ur.redo();
}

void	win_char_editor::menu_file_open()
{
	file_saveAct->setEnabled(c->ur.canUndo() && !never_saved);
}

void	win_char_editor::edit_can_undo_changed(bool new_value)
{
        if (c->ur.canUndo())
                {
                setWindowModified(TRUE);
                edit_undoAct->setEnabled(TRUE);
                }
        else
                {
                edit_undoAct->setEnabled(FALSE);
                setWindowModified(FALSE);
                }
}

void	win_char_editor::edit_can_redo_changed(bool new_value)
{
        if (c->ur.canRedo())
                edit_redoAct->setEnabled(TRUE);
        else
                edit_redoAct->setEnabled(FALSE);
}

bool	win_char_editor::ask_if_save()
{
	if (!isWindowModified())
		return TRUE;
	
	switch (QMessageBox::warning(this, "Itecad", tr("The char has been modified. Do you want to save it now?"), tr("&Yes"), tr("&No"), tr("&Cancel"), 0, 1))
	{
	case 0:		// 0 = YES
		if (never_saved)
			{
			file_save_as();
			return TRUE;
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

void	win_char_editor::file_new()
{
	c->cs.char_points.clear();
	c->cs.char_lines.clear();
	c->ur.clear();
	file = "New";
	never_saved = TRUE;
	setWindowModified(FALSE);
	setWindowTitle(QFileInfo(file).completeBaseName() + "[*] - Char Editor");
	char_width->setCurrentIndex(10);	//Di default imposto la larghezza a 11 pixel
	file_saveAct->setEnabled(FALSE);
	c->setUpdatesEnabled(FALSE);
	c->setUpdatesEnabled(TRUE);
}

void	win_char_editor::file_open()
{
	if (!ask_if_save())
		return;

	QString new_file = open_chr(this);
	if (!new_file.isEmpty())
		if (w_main->char_open(new_file, &(c->cs), TRUE))
			{
			file = new_file;
			never_saved = FALSE;
			setWindowModified(FALSE);
			setWindowTitle(QFileInfo(file).completeBaseName() + "[*] - Char Editor");
			char_width->setCurrentIndex(c->cs.width - 1);
			c->ur.clear();
			c->setUpdatesEnabled(FALSE);
			c->setUpdatesEnabled(TRUE);
			}
}

void	win_char_editor::file_save()
{
	if (never_saved)	//Se eseguo questo comando con una shortkey non faccio niente 
		return;

	file_s(file);
}

void	win_char_editor::file_save_as()
{
	QString new_file;
	if (never_saved)
		new_file = save_chr(this, "");
	else
		new_file = save_chr(this, file);
	if (!new_file.isEmpty())
		{
		file_s(new_file);
		}
}

void	win_char_editor::file_s(QString filename)
{
	if (w_main->char_save(filename, &(c->cs)))
		{
		file = filename;
		never_saved = FALSE;
		setWindowTitle(QFileInfo(file).completeBaseName() + "[*] - Char Editor");
		c->ur.clear();
		setWindowModified(FALSE);
		
		QString current_bg = bg_list->currentText();
		bg_list_fill();
		bg_list->setCurrentIndex(bg_list->findText(current_bg));
		background_on_off(background->isChecked());

		//Aggiungo il simbolo in alphabet di modo da averlo immediatamente disponibile
		char_symbol *cs = new char_symbol();
		*cs = c->cs;
		bool ok;
		cs->unicode = QFileInfo(file).completeBaseName().toInt(&ok, 16);
		w_main->alphabet.insert(cs->unicode, cs);
		//Ad essere pignoli dovrei fare info_update() di tutti gli oggetti di tutti i documenti del progetto
		//Questo perchè una stringa potrebbe contenere un carattere che ho appena creato...
		}
}

void	win_char_editor::reject()
{
	//Senza questa reimplementazione premendo ESC si chiuderebbe brutalmente la finestra senza chiedere se salvare o no.
}

void	win_char_editor::actions_escape()
{
	c->clicked = FALSE;
	c->setUpdatesEnabled(FALSE);
	c->setUpdatesEnabled(TRUE);
}

void	win_char_editor::closeEvent(QCloseEvent *e)
{
    if (ask_if_save())
		e->accept();
	else
		e->ignore();
}

void	win_char_editor::char_width_changed(int index)
{
	c->cs.width = index + 1;
	c->repaint();
}
