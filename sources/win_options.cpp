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

#include "win_options.h"

extern	win_main	*w_main;

extern	QString directory_prj;
extern	QString directory_ssf;
extern	QString directory_pff;
extern	QString directory_ivf;

extern	QString	environment_language;

extern  QSettings   *program_settings;
extern  bool        y_axis_is_up;

win_options::win_options(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
	{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help		
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni	

	setWindowTitle(tr("Options"));	
	//setSizeGripEnabled(true);
    setModal(true);

	//Imposto la larghezza minima pari a metà dello schermo
	QRect screen = QApplication::desktop()->screenGeometry();	//Non serve perchè uso tw->setUsesScrollButtons(FALSE); 
	setMinimumWidth(screen.width() / 2);						//Non serve perchè uso tw->setUsesScrollButtons(FALSE); 

	QGridLayout	*layout = new QGridLayout(this);
	tw = new QTabWidget();
	tw->setUsesScrollButtons(FALSE);
	tw->addTab(setup_general(), tr("General"));
	tw->addTab(setup_directories(), tr("Directories"));
		
	layout->addWidget(tw,1,0);
	layout->addLayout(setup_buttons(),2,0);
	layout->setSpacing(6);
    layout->setMargin(8);

	}


QWidget *win_options::setup_directories()
    {
		QWidget *w = new QWidget();

		QLabel *dir_prj = new QLabel(this);
		dir_prj->setText(tr("Projects directory:"));
		QHBoxLayout *riga1 = new QHBoxLayout(0);	
		dir_PRJ = new QLineEdit(directory_prj);
                QToolButton	*dir_prj_change = new QToolButton(this);
                //dir_prj_change->setText(tr("Browse"));
                dir_prj_change->setIcon(QIcon(":/Resources/folder.png"));
                dir_prj_change->setAutoRaise(TRUE);
                dir_prj_change->setCheckable(FALSE);
                connect(dir_prj_change,SIGNAL(clicked()),this,SLOT(change_prj_directory()));
		riga1->addSpacing(30);
		riga1->addWidget(dir_PRJ);
		riga1->addWidget(dir_prj_change);
		
		QLabel *dir_ssf = new QLabel(this);
		dir_ssf->setText(tr("Schematic symbols directory:"));
		QHBoxLayout *riga2 = new QHBoxLayout(0);	
                QLineEdit *dir_SSF = new QLineEdit(QDir::toNativeSeparators(directory_ssf));
                dir_SSF->setDisabled(TRUE);
                QToolButton	*dir_ssf_change = new QToolButton(this);
                //dir_ssf_change->setText(tr("Browse"));
                dir_ssf_change->setIcon(QIcon(":/Resources/folder_explore.png"));
                dir_ssf_change->setAutoRaise(TRUE);
                dir_ssf_change->setCheckable(FALSE);
                connect(dir_ssf_change,SIGNAL(clicked()),this,SLOT(change_ssf_directory()));
		riga2->addSpacing(30);
		riga2->addWidget(dir_SSF);
		riga2->addWidget(dir_ssf_change);
		
		QLabel *dir_pff = new QLabel(this);
		dir_pff->setText(tr("PCB footprints directory:"));
                QHBoxLayout *riga3 = new QHBoxLayout(0);
                QLineEdit *dir_PFF = new QLineEdit(QDir::toNativeSeparators(directory_pff));
                dir_PFF->setDisabled(TRUE);
                QToolButton	*dir_pff_change = new QToolButton(this);
                //dir_pff_change->setText(tr("Browse"));
                dir_pff_change->setIcon(QIcon(":/Resources/folder_explore.png"));
                dir_pff_change->setAutoRaise(TRUE);
                dir_pff_change->setCheckable(FALSE);
                connect(dir_pff_change,SIGNAL(clicked()),this,SLOT(change_pff_directory()));
		riga3->addSpacing(30);
		riga3->addWidget(dir_PFF);
		riga3->addWidget(dir_pff_change);

                QLabel *dir_ivf = new QLabel(this);
                dir_ivf->setText(tr("Internal Vector Font directory:"));
                QHBoxLayout *riga4 = new QHBoxLayout(0);
                QLineEdit *dir_IVF = new QLineEdit(QDir::toNativeSeparators(directory_ivf));
                dir_IVF->setDisabled(TRUE);
                QToolButton	*dir_ivf_change = new QToolButton(this);
                //dir_pff_change->setText(tr("Browse"));
                dir_ivf_change->setIcon(QIcon(":/Resources/folder_explore.png"));
                dir_ivf_change->setAutoRaise(TRUE);
                dir_ivf_change->setCheckable(FALSE);
                connect(dir_ivf_change,SIGNAL(clicked()),this,SLOT(change_ivf_directory()));
                riga4->addSpacing(30);
                riga4->addWidget(dir_IVF);
                riga4->addWidget(dir_ivf_change);
		
		QGridLayout *gbox = new QGridLayout;
                gbox->addWidget(dir_prj,0,0);
                gbox->addLayout(riga1,1,0,TRUE,FALSE);
		QSpacerItem *spazio1 = new QSpacerItem(20,20);
		gbox->addItem(spazio1,2,0);
		gbox->addWidget(dir_ssf,3,0);
                gbox->addLayout(riga2,4,0,TRUE,FALSE);
		QSpacerItem *spazio2 = new QSpacerItem(20,20);
		gbox->addItem(spazio2,5,0);
		gbox->addWidget(dir_pff,6,0);
                gbox->addLayout(riga3,7,0,TRUE,FALSE);
                QSpacerItem *spazio3 = new QSpacerItem(20,20);
                gbox->addItem(spazio3,8,0);
                gbox->addWidget(dir_ivf,9,0);
                gbox->addLayout(riga4,10,0,TRUE,FALSE);
		gbox->setRowStretch(8,TRUE);
		
		w->setLayout(gbox);
		
		return w;
    }

QWidget *win_options::setup_general()
    {
		QWidget *w = new QWidget();

		QGroupBox *language = new QGroupBox(tr("Language"));
		QGridLayout	*layout_language = new QGridLayout();
		QLabel *language_text = new QLabel(tr("Change language:"));
		language_cb = new QComboBox();
                language_cb->addItem("English");
		language_cb->setCurrentIndex(language_cb->findText(environment_language));
		layout_language->addWidget(language_text, 0, 0);
		layout_language->addWidget(language_cb, 0, 1);
		layout_language->setColumnStretch(2, TRUE);
		language->setLayout(layout_language);

                QGroupBox *y_axis = new QGroupBox(tr("Y axis direction"));
                QGridLayout	*layout_y_axis = new QGridLayout();
                y_axis_up = new QRadioButton(tr("Up"), this);
                y_axis_down = new QRadioButton(tr("Down"), this);
                layout_y_axis->addWidget(y_axis_up, 0, 0);
                layout_y_axis->addWidget(y_axis_down, 1, 0);
                if (y_axis_is_up)
                        y_axis_up->setChecked(TRUE);
                else
                        y_axis_down->setChecked(TRUE);
                layout_language->setColumnStretch(2, TRUE);
                y_axis->setLayout(layout_y_axis);

		QGridLayout *gbox = new QGridLayout;
		gbox->addWidget(language,0,0);
                gbox->addWidget(y_axis,1,0);
                gbox->setRowStretch(2,TRUE);
		w->setLayout(gbox);
		
		return w;
    }

QHBoxLayout	*win_options::setup_buttons()
	{
		QHBoxLayout	*L_comandi = new QHBoxLayout();
		L_comandi->setSpacing(6);
		L_comandi->setMargin(0);
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

void	win_options::change_prj_directory()
{
	QString directory = open_dir(this, tr("Choose Project Directory"), dir_PRJ->text());
    if (!directory.isEmpty())
         dir_PRJ->setText(directory);
}

void	win_options::change_ssf_directory()
{
        QString path = QDir::toNativeSeparators(QApplication::applicationDirPath());
        QDesktopServices::openUrl(QUrl("file:///" + directory_ssf));
}

void	win_options::change_pff_directory()
{
        QString path = QDir::toNativeSeparators(QApplication::applicationDirPath());
        QDesktopServices::openUrl(QUrl("file:///" + directory_pff));
}

void	win_options::change_ivf_directory()
{
        QString path = QDir::toNativeSeparators(QApplication::applicationDirPath());
        QDesktopServices::openUrl(QUrl("file:///" + directory_ivf));
}

void	win_options::accept()
{
	directory_prj = QDir::toNativeSeparators(dir_PRJ->text()); 

	environment_language = language_cb->currentText();
        y_axis_is_up = y_axis_up->isChecked();

        program_settings->setValue("directory_prj", directory_prj);

        program_settings->setValue("environment_language", environment_language);

        program_settings->setValue("y_axis_is_up", y_axis_is_up);

	QDialog::accept();
}
