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

#include "win_xml_editor.h"

extern	win_main		*w_main;
extern	myGraphicsView	*GV;

extern	QString directory_prj;
extern	QString directory_ssf;
extern	QString directory_pff;

extern  win_find_text   *modeless_win_find_text;

win_xml_editor::win_xml_editor(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

	setWindowTitle(tr("XML Editor"));	
	QRect screen = QApplication::desktop()->screenGeometry();
        this->setMinimumWidth(screen.width() * 4 / 5);
        this->setMinimumHeight(screen.height() * 4 / 5);
	setSizeGripEnabled(true);
	setModal(true);	

	QGridLayout	*layout = new QGridLayout(this);
	
	layout->addWidget(setup_menuBar(),0,0);
	xml.setEnabled(FALSE);
	xml.setLineWrapMode(QTextEdit::NoWrap);
	QFont f = xml.document()->defaultFont();
	f.setPointSize(12);
	xml.document()->setDefaultFont(f);
	highlighter = new Highlighter(xml.document());
	connect(&xml, SIGNAL(cursorPositionChanged()), this, SLOT(changed_cursor_pos()));
	connect(xml.document(), SIGNAL(modificationChanged(bool)), file_saveAct, SLOT(setEnabled(bool)));
    connect(xml.document(), SIGNAL(modificationChanged(bool)), this, SLOT(setWindowModified(bool)));
    connect(xml.document(), SIGNAL(undoAvailable(bool)), edit_undoAct, SLOT(setEnabled(bool)));
    connect(xml.document(), SIGNAL(redoAvailable(bool)), edit_redoAct, SLOT(setEnabled(bool)));
	connect(&xml, SIGNAL(copyAvailable(bool)), edit_cutAct, SLOT(setEnabled(bool)));
	connect(&xml, SIGNAL(copyAvailable(bool)), edit_copyAct, SLOT(setEnabled(bool)));
	layout->addWidget(&xml,1,0);
	
	layout->addWidget(setup_statusBar(),2,0);
	layout->setSpacing(0);
    layout->setMargin(0);
}

win_xml_editor::~win_xml_editor()
{
        if (modeless_win_find_text)
                {
                delete modeless_win_find_text;
                modeless_win_find_text = 0;
                }
}

QMenuBar *win_xml_editor::setup_menuBar()
{
	file_open_prjAct = new QAction(tr("Open Project"), this); 
        file_open_prjAct->setIcon(QIcon(":/Resources/folder_page_white.png"));
    connect(file_open_prjAct, SIGNAL(triggered()), this, SLOT(file_open_prj()));

	file_open_ssfAct = new QAction(tr("Open Schematic Symbol"), this); 
    connect(file_open_ssfAct, SIGNAL(triggered()), this, SLOT(file_open_ssf()));

	file_open_pffAct = new QAction(tr("Open PCB Footprint"), this); 
    connect(file_open_pffAct, SIGNAL(triggered()), this, SLOT(file_open_pff()));

	file_saveAct = new QAction(tr("Save"), this); 
	file_saveAct->setEnabled(FALSE);
        file_saveAct->setIcon(QIcon(":/Resources/disk.png"));
    connect(file_saveAct, SIGNAL(triggered()), this, SLOT(file_save()));

	file_save_asAct = new QAction(tr("Save &As..."), this); 
	file_save_asAct->setEnabled(FALSE);
    connect(file_save_asAct, SIGNAL(triggered()), this, SLOT(file_save_as()));

	file_exitAct = new QAction(tr("Exit"), this); 
    connect(file_exitAct, SIGNAL(triggered()), this, SLOT(close()));

	edit_undoAct = new QAction(tr("Undo"), this); 
	edit_undoAct->setEnabled(FALSE);
        edit_undoAct->setIcon(QIcon(":/Resources/arrow_undo.png"));
    connect(edit_undoAct, SIGNAL(triggered()), &xml, SLOT(undo()));

	edit_redoAct = new QAction(tr("Redo"), this); 
	edit_redoAct->setEnabled(FALSE);
        edit_redoAct->setIcon(QIcon(":/Resources/arrow_redo.png"));
    connect(edit_redoAct, SIGNAL(triggered()), &xml, SLOT(redo()));

	edit_cutAct = new QAction(tr("Cut"), this); 
	edit_cutAct->setEnabled(FALSE);
        edit_cutAct->setIcon(QIcon(":/Resources/cut.png"));
    connect(edit_cutAct, SIGNAL(triggered()), &xml, SLOT(cut()));

	edit_copyAct = new QAction(tr("Copy"), this); 
	edit_copyAct->setEnabled(FALSE);
        edit_copyAct->setIcon(QIcon(":/Resources/page_copy.png"));
    connect(edit_copyAct, SIGNAL(triggered()), &xml, SLOT(copy()));

	edit_pasteAct = new QAction(tr("Paste"), this); 
	edit_pasteAct->setEnabled(TRUE);
        edit_pasteAct->setIcon(QIcon(":/Resources/page_paste.png"));
    connect(edit_pasteAct, SIGNAL(triggered()), &xml, SLOT(paste()));

        edit_findAct = new QAction(tr("Find"), this);
        edit_findAct->setEnabled(TRUE);
        edit_findAct->setIcon(QIcon(":/Resources/find.png"));
    connect(edit_findAct, SIGNAL(triggered()), this, SLOT(edit_find()));

	fileMenu = new QMenu(tr("&File"));
	fileMenu->addAction(file_open_prjAct);
	fileMenu->addAction(file_open_ssfAct);
	fileMenu->addAction(file_open_pffAct);
	fileMenu->addSeparator();
	fileMenu->addAction(file_saveAct);
	fileMenu->addAction(file_save_asAct);
	fileMenu->addSeparator();
	fileMenu->addAction(file_exitAct);
	editMenu = new QMenu(tr("&Edit"));
	editMenu->addAction(edit_undoAct);
	editMenu->addAction(edit_redoAct);
	editMenu->addSeparator();
	editMenu->addAction(edit_cutAct);
	editMenu->addAction(edit_copyAct);
	editMenu->addAction(edit_pasteAct);
        editMenu->addSeparator();
        editMenu->addAction(edit_findAct);
	connect(editMenu,SIGNAL(aboutToShow()),this,SLOT(menu_edit_open()));

	mb = new QMenuBar(this);
	mb->clear();
	mb->addMenu(fileMenu);
	mb->addMenu(editMenu);
	return mb;
}



QStatusBar *win_xml_editor::setup_statusBar()
{
	QStatusBar	*sb = new QStatusBar(this);
	sb_row = new QLabel("");		
	sb_row->setFixedWidth(sb->height() * 4);			
	sb_row->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	sb_row->setMargin(0);
	sb->addPermanentWidget(sb_row);	//I permanentWidget sono inseriti a destra

	sb_column = new QLabel("");
	sb_column->setFixedWidth(sb->height() * 4);
	sb_column->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	sb_column->setMargin(0);
	sb->addPermanentWidget(sb_column);	//I permanentWidget sono inseriti a destra

	return sb;
}

void win_xml_editor::edit_find()
{
        /*win_find_text dialogo(this, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint, &xml);
        if (dialogo.exec())
                {
                }
        xml.setFocus();*/

        if (!modeless_win_find_text)
                modeless_win_find_text = new win_find_text(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint, &xml);
        modeless_win_find_text->show();
        modeless_win_find_text->raise();
        modeless_win_find_text->activateWindow();
}

void win_xml_editor::menu_edit_open()
{
	if (xml.isEnabled())
                {
		edit_pasteAct->setEnabled(xml.canPaste());
                edit_findAct->setEnabled(TRUE);
                }
	else
                {
		edit_pasteAct->setEnabled(FALSE);
                edit_findAct->setEnabled(FALSE);
                }
}

bool win_xml_editor::ask_if_save()
{
	QString question = tr("The file %1 has been modified. Do you want to save it now?").arg(QFileInfo(file_path).fileName());
	switch (QMessageBox::warning(this, "Itecad", question, tr("&Yes"), tr("&No"), tr("&Cancel"), 0, 1))
	{
	case 0:		// 0 = YES
		file_save();
		return TRUE;
	case 1:		// 1 = NO
		return TRUE;
	case 2:		// 2 = CANCEL
		return FALSE;
	}		
}

void	win_xml_editor::file_open_prj()
{
	if (isWindowModified())
		if (!ask_if_save())
			return;

	file_type_choosed = FT_PROJECT;

	QString file = open_prj(this);
	if (!file.isEmpty())
		{		
		xml_sax_handler handler;
		if (handler.file_to_textedit(file, &xml))
			{
			file_path = file;
			xml.setEnabled(TRUE);
			setWindowTitle(QFileInfo(file).fileName() + "[*] - XML Editor");
			file_save_asAct->setEnabled(TRUE);
			}
		}
}

void	win_xml_editor::file_open_ssf()
{
	if (isWindowModified())
		if (!ask_if_save())
			return;

	file_type_choosed = FT_SCH_SYMBOL;

	QString file = open_ssf(this);
	if (!file.isEmpty())
		{		
		xml_sax_handler handler;
		if (handler.file_to_textedit(file, &xml))
			{
			file_path = file;
			xml.setEnabled(TRUE);
			setWindowTitle(QFileInfo(file).fileName() + "[*] - XML Editor");
			file_save_asAct->setEnabled(TRUE);
			}
		}
}

void	win_xml_editor::file_open_pff()
{
	if (isWindowModified())
		if (!ask_if_save())
			return;

	file_type_choosed = FT_PCB_SYMBOL;

	QString file = open_pff(this);
	if (!file.isEmpty())
		{		
		xml_sax_handler handler;
		if (handler.file_to_textedit(file, &xml))
			{
			file_path = file;
			xml.setEnabled(TRUE);
			setWindowTitle(QFileInfo(file).fileName() + "[*] - XML Editor");
			file_save_asAct->setEnabled(TRUE);
			}
		}
}

void	win_xml_editor::file_save()
{
	xml_sax_generator generator;
	if (generator.textedit_to_file(file_path, &xml))
		{
		xml.setPlainText(xml.toPlainText());	//Questa istruzione azzera i vari Undo/Redo	
		}	
}

void	win_xml_editor::file_save_as()
{
	QString file;
	switch (file_type_choosed)
	{
	case FT_PROJECT:
		file = save_prj(this, file_path);
		break;
	case FT_SCH_SYMBOL:
		file = save_ssf(this, file_path);
		break;
	case FT_PCB_SYMBOL:
		file = save_pff(this, file_path);
		break;		
	}
	if (!file.isEmpty())
		{		
		xml_sax_generator generator;
		if (generator.textedit_to_file(file, &xml))
			{
			file_path = file;
			setWindowTitle(QFileInfo(file).fileName() + "[*] - XML Editor");
			xml.setPlainText(xml.toPlainText());	//Questa istruzione azzera i vari Undo/Redo	
			}	
		}	
}

void	win_xml_editor::closeEvent(QCloseEvent *e)
{
	if (isWindowModified())
		{
		if (ask_if_save())
			e->accept();
		else
			e->ignore();
		}
	else
		e->accept();
}

void	win_xml_editor::changed_cursor_pos()
{
	int row, column;

	column = xml.textCursor().columnNumber() + 1;
	row = xml.textCursor().blockNumber() + 1;

	sb_row->setText(tr("Row") + " " + QString::number(row));
	sb_column->setText(tr("Column") + " " + QString::number(column));
}

void	win_xml_editor::reject()
{
	//Senza questa reimplementazione premendo ESC si chiuderebbe brutalmente la finestra senza chiedere se salvare o no.
}


Highlighter::Highlighter(QTextDocument *parent)
     : QSyntaxHighlighter(parent)
{
	//Qui sotto definisco le regole che saranno usate dentro a highlightBlock()
	HighlightingRule rule;

	keywordFormat.setForeground(Qt::darkBlue);
	keywordFormat.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
    keywordPatterns		<< "\\bitecadproject\\b" 
						<< "\\bitecadschsymbol\\b"
						<< "\\bitecadpcbsymbol\\b"
						<< "\\bobject\\b"
						<< "\\bsymbol\\b"
						<< "\\bfootprint\\b"
						<< "\\bnode\\b"
						<< "\\bpin\\b"
						<< "\\bport\\b"
						<< "\\bpad\\b"
						<< "\\bvia\\b"
						<< "\\bwire\\b"
                                                << "\\bpolyline\\b"
						<< "\\bpolygon\\b"
						<< "\\btext\\b"
						<< "\\brectangle\\b"
						<< "\\bellipse\\b"
						<< "\\bident\\b"
						<< "\\bvalue\\b"
                                                << "\\bdocument\\b"
                                                << "\\bpower\\b";
	foreach (QString pattern, keywordPatterns) 
		{
		rule.pattern = QRegExp(pattern);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
		}

     quotationFormat.setForeground(Qt::darkGreen);
     rule.pattern = QRegExp("\"[a-zA-Z0-9 .-,]*\"");	//Dato che il C++ per inserire caratteri speciali deve farli precedere da '\' questa stringa equivale a "[a-zA-Z0-9 .-]*"
     rule.format = quotationFormat;
     highlightingRules.append(rule);
}

 void Highlighter::highlightBlock(const QString &text)
 {
	//Questa funzione deve essere reimplementata ed è lei che si occupa di gestire i colori
	foreach (HighlightingRule rule, highlightingRules)
		{
		QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);
        while (index >= 0) 
			{
			int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = text.indexOf(expression, index + length);
			}
		}
 }
