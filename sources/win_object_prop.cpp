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

#include "win_object_prop.h"

extern	win_main		*w_main;
extern	myGraphicsView	*GV;
extern	myGraphicsScene	*GS;

extern	char	file_type;	

extern  int	style_pcb_pad_drill;

extern  QRegExp exp_pin_number;

win_object_prop::win_object_prop(QWidget *parent, Qt::WFlags tipo, myGraphicsItem *obj_input)
        : QDialog(parent, tipo)
{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

	//setSizeGripEnabled(true);
	setModal(true);

	QGridLayout	*layout = new QGridLayout(this);
	
	obj = obj_input;
	
	switch (obj->tipo)
	{
	case O_CONNECTION:
                setWindowTitle(tr("Wire Properties"));
                layout->addWidget(setup_connection(), 0, 0);
		layout->addLayout(setup_buttons(), 1, 0, TRUE, FALSE);
		break;
	case O_POLYLINE:
		setWindowTitle(tr("Polyline Properties"));	
		layout->addWidget(setup_polyline(), 0, 0);
		layout->addLayout(setup_buttons(), 1, 0, TRUE, FALSE);
		break;
	case O_POLYGON:
		setWindowTitle(tr("Polygon Properties"));	
		layout->addWidget(setup_polygon(), 0, 0);
		layout->addLayout(setup_buttons(), 1, 0, TRUE, FALSE);
		break;
	case O_RECT:
		setWindowTitle(tr("Rectangle Properties"));	
		layout->addWidget(setup_polygon(), 0, 0);
		layout->addLayout(setup_buttons(), 1, 0, TRUE, FALSE);
		break;
	case O_ELLIPSE:
		setWindowTitle(tr("Ellipse Properties"));	
		layout->addWidget(setup_polygon(), 0, 0);
		layout->addLayout(setup_buttons(), 1, 0, TRUE, FALSE);
		break;
	case O_TEXT:	
		setWindowTitle(tr("Text Properties"));
		layout->addWidget(setup_font_properties(), 0, 0);
		layout->addWidget(setup_text(), 1, 0);
		TextText->setFocus();
		layout->addLayout(setup_buttons(), 2, 0, TRUE, FALSE);
		break;
	case O_PIN:	
		setWindowTitle(tr("Pin Properties"));	
		layout->addWidget(setup_pin_general(), 0, 0);
		layout->addWidget(setup_pin_orientation(), 2, 0);		
		layout->addLayout(setup_buttons(), 3, 0, TRUE, FALSE);
		break;
        case O_POWER:
                setWindowTitle(tr("Power Port Properties"));
                layout->addWidget(setup_power_general(), 0, 0);
                layout->addLayout(setup_buttons(), 3, 0, TRUE, FALSE);
		break;
        case O_PORT:
                setWindowTitle(tr("Module Port Properties"));
                layout->addWidget(setup_port_general(), 0, 0);
                layout->addWidget(setup_port_orientation(), 2, 0);
                layout->addLayout(setup_buttons(), 3, 0, TRUE, FALSE);
                break;
	case O_PAD:
		{
		int row = 0;
		setWindowTitle(tr("Pad Properties"));	
                layout->addLayout(setup_pad(), row++, 0);
                layout->addLayout(setup_buttons(), row++, 0, TRUE, FALSE);
		}
		break;
	case O_VIA:
		{
		int row = 0;
		setWindowTitle(tr("Via Properties"));			
                layout->addLayout(setup_via(), row++, 0);
                layout->addLayout(setup_buttons(), row++, 0, TRUE, FALSE);
		}
		break;
	}
	
	layout->setSpacing(6);
    layout->setMargin(8);
}

QGroupBox *win_object_prop::setup_font_properties()
{
	QGroupBox *groupBox = new QGroupBox(tr("Font Properties"));
	
	font_layout = new layout_font(obj->font_height, obj->font_bold, obj->font_italic);
		
	QGridLayout *gbox_text = new QGridLayout();
	gbox_text->addLayout(font_layout, 0, 0, TRUE, FALSE);
	groupBox->setLayout(gbox_text);

	return groupBox;
}

QGroupBox *win_object_prop::setup_text()
{
	QGroupBox *groupBox = new QGroupBox(tr("Text"));
	
	QGridLayout *gbox_text = new QGridLayout();
	TextText = new QLineEdit(this);
	TextText->setText(obj->text);
	unicode_input = new QToolButton(this);
	unicode_input->setIcon(QIcon(":/Resources/keyboard.png"));
	unicode_input->setAutoRaise(TRUE);
	unicode_input->setCheckable(FALSE);
	connect(unicode_input,SIGNAL(clicked()),this,SLOT(unicode_input_pressed()));
        gbox_text->addWidget(TextText, 0, 0);
	gbox_text->addWidget(unicode_input, 0, 1);
        if (!GV->is_sch)
                {
                TextMirror = new QCheckBox(tr("Mirror?"));
                TextMirror->setChecked(obj->mirror_text_on);
                gbox_text->addWidget(TextMirror, 1, 0);
                }
	groupBox->setLayout(gbox_text);

	return groupBox;
}


void	win_object_prop::unicode_input_pressed()
{
        win_unicode_input dialogo(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint, TextText->mapToGlobal(QPoint(0,TextText->height())));
	dialogo.raise();
        dialogo.activateWindow();
        dialogo.set_lineedit(TextText);
	if (dialogo.exec())
		{
		
		}
}

QGroupBox *win_object_prop::setup_connection()
{
	QGroupBox *groupBox;
        groupBox = new QGroupBox(tr("Track"));

	QLabel *pen_width = new QLabel(this);
	pen_width->setText(tr("Pen Width:"));
	pen_width->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        Pen_width = new layout_length("", obj->pen_width, 0, 1000000, FALSE);	//La larghezza 0 deve essere consentita (linee infinitesimali e visibili, anche nel PCB)
	QGridLayout *gbox = new QGridLayout();
	gbox->addWidget(pen_width,0,0);
	gbox->addLayout(Pen_width,0,1);
	gbox->setRowStretch(0,TRUE);		//Lo stretch è come se fosse una molla
	groupBox->setLayout(gbox);	
	
	return groupBox;
}

QGroupBox *win_object_prop::setup_polyline()
{
	QGroupBox *groupBox = new QGroupBox(tr("Polyline"));

	QLabel *pen_width = new QLabel(this);
	pen_width->setText(tr("Pen Width:"));
	pen_width->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        Pen_width = new layout_length("", obj->pen_width, 0, 1000000, GV->is_sch);	//La larghezza 0 deve essere consentita (linee infinitesimali e visibili, anche nel PCB)
	QGridLayout *gbox = new QGridLayout();
	gbox->addWidget(pen_width,0,0);
	gbox->addLayout(Pen_width,0,1);
	gbox->setRowStretch(0,TRUE);		//Lo stretch è come se fosse una molla
	groupBox->setLayout(gbox);	
	
	return groupBox;
}

QGroupBox *win_object_prop::setup_polygon()
{
	QGroupBox *groupBox;
	switch (obj->tipo)
	{
	case O_POLYGON:
		setWindowTitle(tr("Polygon Properties"));	
		groupBox = new QGroupBox(tr("Polygon"));
		break;
	case O_ELLIPSE:
		setWindowTitle(tr("Circle Properties"));	
		groupBox = new QGroupBox(tr("Circle"));
		break;
	case O_RECT:
		setWindowTitle(tr("Rectangle Properties"));	
		groupBox = new QGroupBox(tr("Rect"));
		break;
	}

	QLabel *pen_width = new QLabel(this);
	pen_width->setText(tr("Pen Width:"));
	pen_width->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        Pen_width = new layout_length("", obj->pen_width, 0, 1000000, GV->is_sch);	//La larghezza 0 deve essere consentita (linee infinitesimali e visibili, anche nel PCB)
        Fill_polygon = new QCheckBox(tr("Fill Polygon?"));
	Fill_polygon->setChecked(obj->fill_polygon);
        QObject::connect(Fill_polygon, SIGNAL(stateChanged(int)), this, SLOT(change_fill_polygon(int)));
        change_fill_polygon(0);
        QGridLayout *gbox = new QGridLayout();
	gbox->addWidget(pen_width,0,0);
	gbox->addLayout(Pen_width,0,1);
	gbox->addWidget(Fill_polygon,1,0, TRUE, FALSE);
	gbox->setRowStretch(0,TRUE);		//Lo stretch è come se fosse una molla
	groupBox->setLayout(gbox);	
	
	return groupBox;
}

QGroupBox *win_object_prop::setup_pin_general()
{
	QGroupBox *groupBox = new QGroupBox(tr("General"));
	QGridLayout	*layout_general = new QGridLayout();

	QLabel *number = new QLabel(this);
	number->setText(tr("Number:"));
	number->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	TextNumber = new QLineEdit(QString::number(obj->number));
        TextNumber->setValidator(new QRegExpValidator(exp_pin_number, this));   //Uso new perchè ogni volta bisogna ricreare QRegExpValidator (non si può utilizzare l'oggetto altrove...)
	QLabel *name = new QLabel(this);
	name->setText(tr("Name:"));
	name->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	TextName = new QLineEdit(obj->text);

	QLabel *shape = new QLabel(this);
	shape->setText(tr("Shape:"));
	shape->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	Pin_shape = new QComboBox(this);
	Pin_shape->addItem(tr("None"));
	Pin_shape->addItem(tr("Line"));
	Pin_shape->addItem(tr("Clock"));
	Pin_shape->addItem(tr("Dot"));
	Pin_shape->addItem(tr("Dot Clock"));
        Pin_shape->setCurrentIndex(obj->shape);
        QSpacerItem	*molla_rx = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

	layout_general->addWidget(number, 0, 0);
	layout_general->addWidget(TextNumber, 0, 1);
	layout_general->addWidget(name, 1, 0);
	layout_general->addWidget(TextName, 1, 1);
	layout_general->addWidget(shape, 2, 0);
	QHBoxLayout *riga3 = new QHBoxLayout(0);
	riga3->addWidget(Pin_shape);
	riga3->addItem(molla_rx);
	layout_general->addItem(riga3, 2, 1);
	
	groupBox->setLayout(layout_general);

	return groupBox;
}

QGroupBox *win_object_prop::setup_pin_orientation()
{
	QGroupBox *groupBox = new QGroupBox(tr("Orientation"));
	QGridLayout	*layout_alignment = new QGridLayout();

	orientation_top = new QRadioButton(this);
	if (obj->text_position == 'T')
		orientation_top->setChecked(TRUE);
	QObject::connect(orientation_top, SIGNAL(toggled(bool)), this, SLOT(change_orientation(bool)));
	QHBoxLayout *layout_top = new QHBoxLayout();
	layout_top->addStretch(0);
	layout_top->addWidget(orientation_top);
	layout_top->addStretch(0);
	
	orientation_left = new QRadioButton(this);
	if (obj->text_position == 'L')
		orientation_left->setChecked(TRUE);
	QObject::connect(orientation_left, SIGNAL(toggled(bool)), this, SLOT(change_orientation(bool)));
	
	orientation_right = new QRadioButton(this);
	if (obj->text_position == 'R')
		orientation_right->setChecked(TRUE);
	QObject::connect(orientation_right, SIGNAL(toggled(bool)), this, SLOT(change_orientation(bool)));
	
	orientation_bottom = new QRadioButton(this);
	if (obj->text_position == 'B')
		orientation_bottom->setChecked(TRUE);
	QObject::connect(orientation_bottom, SIGNAL(toggled(bool)), this, SLOT(change_orientation(bool)));
	QHBoxLayout *layout_bottom = new QHBoxLayout();
	layout_bottom->addStretch(10);
	layout_bottom->addWidget(orientation_bottom);
	layout_bottom->addStretch(10);

	QSpacerItem	*molla_sx = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	QSpacerItem	*molla_rx = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

	orientation_image = new QLabel();
	layout_alignment->addItem(molla_sx,0,0, FALSE, TRUE);
	layout_alignment->addLayout(layout_top, 0, 2);
	layout_alignment->addWidget(orientation_left, 1, 1);
	layout_alignment->addWidget(orientation_right, 1, 3);
	layout_alignment->addLayout(layout_bottom, 2, 2);
	layout_alignment->addWidget(orientation_image, 1, 2);
	layout_alignment->addItem(molla_rx,0,4, FALSE, TRUE);
	
	groupBox->setLayout(layout_alignment);

	change_orientation(TRUE);	//Mi serve per caricare l'immagine pin_text.png corretta

	return groupBox;
}

QGroupBox *win_object_prop::setup_power_general()
{
        QGroupBox *groupBox = new QGroupBox(tr("General"));
        QGridLayout	*layout_general = new QGridLayout();

        QLabel *name = new QLabel(this);
        name->setText(tr("Name:"));
        name->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        TextName = new QLineEdit(obj->text);

        QLabel *shape = new QLabel(this);
        shape->setText(tr("Shape:"));
        shape->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        Power_shape = new QComboBox(this);
        Power_shape->addItem(tr("Power Ground"));
        Power_shape->addItem(tr("Signal Ground"));
        Power_shape->addItem(tr("Earth Ground"));
        Power_shape->addItem(tr("Circle Power"));
        Power_shape->addItem(tr("Bar Power"));
        Power_shape->setCurrentIndex(obj->shape);
        Show_name = new QCheckBox(tr("Show Name?"));
        Show_name->setChecked(obj->fill_polygon);
        QSpacerItem	*molla_rx = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        layout_general->addWidget(name, 0, 0);
        layout_general->addWidget(TextName, 0, 1);
        layout_general->addWidget(shape, 1, 0);
        QHBoxLayout *riga2 = new QHBoxLayout(0);
        riga2->addWidget(Power_shape);
        riga2->addItem(molla_rx);
        layout_general->addItem(riga2, 1, 1);
        layout_general->addWidget(Show_name, 2, 1);

        groupBox->setLayout(layout_general);

        return groupBox;
}

QGroupBox *win_object_prop::setup_port_general()
{
	QGroupBox *groupBox = new QGroupBox(tr("General"));
	QGridLayout	*layout_general = new QGridLayout();

	QLabel *name = new QLabel(this);
	name->setText(tr("Name:"));
	name->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	TextName = new QLineEdit(obj->text);

	QLabel *shape = new QLabel(this);
	shape->setText(tr("Shape:"));
	shape->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	Port_shape = new QComboBox(this);
	Port_shape->addItem(tr("Generic"));
	Port_shape->addItem(tr("Input"));
	Port_shape->addItem(tr("Output"));
	Port_shape->addItem(tr("Bidirectional"));
        Port_shape->setCurrentIndex(obj->shape);
	QSpacerItem	*molla_rx = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

	layout_general->addWidget(name, 0, 0);
	layout_general->addWidget(TextName, 0, 1);
	layout_general->addWidget(shape, 1, 0);
	QHBoxLayout *riga2 = new QHBoxLayout(0);
	riga2->addWidget(Port_shape);
	riga2->addItem(molla_rx);
	layout_general->addItem(riga2, 1, 1);
	
	groupBox->setLayout(layout_general);

	return groupBox;
}

QGroupBox *win_object_prop::setup_port_orientation()
{
	QGroupBox *groupBox = new QGroupBox(tr("Orientation"));
	QGridLayout	*layout_alignment = new QGridLayout();

	orientation_top = new QRadioButton(this);
	if (obj->text_position == 'T')
		orientation_top->setChecked(TRUE);
	QObject::connect(orientation_top, SIGNAL(toggled(bool)), this, SLOT(change_orientation(bool)));
	QHBoxLayout *layout_top = new QHBoxLayout();
	layout_top->addStretch(0);
	layout_top->addWidget(orientation_top);
	layout_top->addStretch(0);
	
	orientation_left = new QRadioButton(this);
	if (obj->text_position == 'L')
		orientation_left->setChecked(TRUE);
	QObject::connect(orientation_left, SIGNAL(toggled(bool)), this, SLOT(change_orientation(bool)));
	
	orientation_right = new QRadioButton(this);
	if (obj->text_position == 'R')
		orientation_right->setChecked(TRUE);
	QObject::connect(orientation_right, SIGNAL(toggled(bool)), this, SLOT(change_orientation(bool)));
	
	orientation_bottom = new QRadioButton(this);
	if (obj->text_position == 'B')
		orientation_bottom->setChecked(TRUE);
	QObject::connect(orientation_bottom, SIGNAL(toggled(bool)), this, SLOT(change_orientation(bool)));
	QHBoxLayout *layout_bottom = new QHBoxLayout();
	layout_bottom->addStretch(10);
	layout_bottom->addWidget(orientation_bottom);
	layout_bottom->addStretch(10);

	QSpacerItem	*molla_sx = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	QSpacerItem	*molla_rx = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

	orientation_image = new QLabel();
	layout_alignment->addItem(molla_sx,0,0, FALSE, TRUE);
	layout_alignment->addLayout(layout_top, 0, 2);
	layout_alignment->addWidget(orientation_left, 1, 1);
	layout_alignment->addWidget(orientation_right, 1, 3);
	layout_alignment->addLayout(layout_bottom, 2, 2);
	layout_alignment->addWidget(orientation_image, 1, 2);
	layout_alignment->addItem(molla_rx,0,4, FALSE, TRUE);
	
	groupBox->setLayout(layout_alignment);

	change_orientation(TRUE);	//Mi serve per caricare l'immagine pin_text.png corretta

	return groupBox;
}

QGridLayout *win_object_prop::setup_pad()
{
        QGridLayout *layout_pad = new QGridLayout();
        QGroupBox *pad_general = new QGroupBox(tr("General"));
        QGridLayout *layout_general = new QGridLayout();
        QLabel *number = new QLabel(this);
	number->setText(tr("Number:"));
	number->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	TextNumber = new QLineEdit(QString::number(obj->number));
        TextNumber->setValidator(new QRegExpValidator(exp_pin_number, this));   //Uso new perchè ogni volta bisogna ricreare QRegExpValidator (non si può utilizzare l'oggetto altrove...)
	layout_general->addWidget(number, 0, 0);
	layout_general->addWidget(TextNumber, 0, 1);
        if ((file_type == FT_PROJECT) && obj->comp)
                TextNumber->setEnabled(FALSE);
        pad_general->setLayout(layout_general);

        pad = new layout_pad_via(TRUE, obj->shape, obj->width, obj->height, obj->layer_from, obj->layer_to, obj->drill);
        QGroupBox *pad_shape_and_drill = new QGroupBox(tr("Shape and drill options"));
        pad_shape_and_drill->setLayout(pad);

        layout_pad->addWidget(pad_general, 0, 0);
        layout_pad->addWidget(pad_shape_and_drill, 1, 0);

        return layout_pad;
}

QGridLayout *win_object_prop::setup_via()
{
        QGridLayout	*layout_via = new QGridLayout();

        via = new layout_pad_via(FALSE, obj->shape, obj->width, obj->height, obj->layer_from, obj->layer_to, obj->drill);
        QGroupBox *via_shape_and_drill = new QGroupBox(tr("Shape and drill options"));
        via_shape_and_drill->setLayout(via);

        layout_via->addWidget(via_shape_and_drill, 0, 0);

        return layout_via;
}

QHBoxLayout	*win_object_prop::setup_buttons()
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

void	win_object_prop::change_orientation(bool checked)
{
	if (obj->tipo == O_PIN)
		{
		if (orientation_top->isChecked())
			orientation_image->setPixmap(QPixmap(":/Resources/pin_t.png"));
		else if (orientation_bottom->isChecked())
			orientation_image->setPixmap(QPixmap(":/Resources/pin_b.png"));
		else if (orientation_left->isChecked())
			orientation_image->setPixmap(QPixmap(":/Resources/pin_l.png"));
		else if (orientation_right->isChecked())
			orientation_image->setPixmap(QPixmap(":/Resources/pin_r.png"));
		}
	else if (obj->tipo == O_PORT)
		{
		if (orientation_top->isChecked())
			orientation_image->setPixmap(QPixmap(":/Resources/port_t.png"));
		else if (orientation_bottom->isChecked())
			orientation_image->setPixmap(QPixmap(":/Resources/port_b.png"));
		else if (orientation_left->isChecked())
			orientation_image->setPixmap(QPixmap(":/Resources/port_l.png"));
		else if (orientation_right->isChecked())
			orientation_image->setPixmap(QPixmap(":/Resources/port_r.png"));
		}
}

void	win_object_prop::change_fill_polygon(int state)
{
        if (Fill_polygon->isChecked())
                Pen_width->setEnabled(FALSE);
        else
                Pen_width->setEnabled(TRUE);
}

bool	win_object_prop::is_modified()
{
	switch (obj->tipo)
	{
	case O_POLYLINE:
	case O_CONNECTION:
		if (new_pen_width != obj->pen_width)
			return TRUE;
		break;
	case O_POLYGON:
	case O_RECT:
	case O_ELLIPSE:
		if (new_pen_width != obj->pen_width)
			return TRUE;
                if (new_width != obj->width)
			return TRUE;
                if (new_height != obj->height)
			return TRUE;
		if (new_fill_polygon != obj->fill_polygon)
			return TRUE;
		break;
	case O_TEXT:
		if (new_font_height != obj->font_height)
			return TRUE;
		if (new_font_bold != obj->font_bold)
			return TRUE;
		if (new_font_italic != obj->font_italic)
			return TRUE;
		if (new_text != obj->text)
			return TRUE;
		if (new_text_position != obj->text_position)
			return TRUE;
                if (!GV->is_sch)
                        if (new_mirror_text_on != obj->mirror_text_on)
                                return TRUE;
		break;
	case O_PIN:
		if (new_number != obj->number)
			return TRUE;
		if (new_text != obj->text)
			return TRUE;
		if (new_text_position != obj->text_position)
			return TRUE;
                if (new_shape != obj->shape)
                        return TRUE;
		break;
        case O_POWER:
                if (new_text != obj->text)
                        return TRUE;
                if (new_text_position != obj->text_position)
                        return TRUE;
                if (new_shape != obj->shape)
                        return TRUE;
                if (new_fill_polygon != obj->fill_polygon)
                        return TRUE;
                break;
	case O_PORT:
		if (new_text != obj->text)
			return TRUE;
		if (new_text_position != obj->text_position)
			return TRUE;
                if (new_shape != obj->shape)
			return TRUE;
		break;
	case O_PAD:
                if (new_number != obj->number)
                        return TRUE;
                if (new_shape != obj->shape)
                        return TRUE;
                if (new_width != obj->width)
                        return TRUE;
                if (new_height != obj->height)
                        return TRUE;
                if (new_layer_from != obj->layer_from)
                        return TRUE;
                if (new_layer_to != obj->layer_to)
                        return TRUE;
                if (new_drill != obj->drill)
                        return TRUE;
                break;
	case O_VIA:
                if (new_width != obj->width)
                        return TRUE;
                if (new_layer_from != obj->layer_from)
                        return TRUE;
                if (new_layer_to != obj->layer_to)
                        return TRUE;
                if (new_drill != obj->drill)
                        return TRUE;
                break;
	}

	return FALSE;
}

void	win_object_prop::accept()
{
        switch (obj->tipo)
	{
	case O_POLYLINE:
	case O_CONNECTION:
		if (Pen_width->warning_not_valid_parameter(tr("Pen width")))
			return;
		new_pen_width = Pen_width->length;
		new_text = "";
		new_text_position = 0;
                new_layer_from = obj->layer_from;
                new_layer_to = obj->layer_to;
		break;
	case O_POLYGON:
	case O_RECT:
	case O_ELLIPSE:
		if (Pen_width->warning_not_valid_parameter(tr("Pen width")))
			return;
		new_pen_width = Pen_width->length;
                new_width = obj->width;
                new_height = obj->height;
		new_fill_polygon = Fill_polygon->isChecked();
		new_text = "";
		new_text_position = 0;
                new_layer_from = obj->layer_from;
                new_layer_to = obj->layer_to;
		break;
	case O_TEXT:
		if (TextText->text().isEmpty())
			{
			QMessageBox::warning(0,tr("Warning"),tr("Text can't be empty"));
			return;
			}
                new_font_height = font_layout->font_height;
		new_font_bold = font_layout->font_bold;
		new_font_italic = font_layout->font_italic;
		new_text = TextText->text();
                new_text_position = obj->text_position;
                new_layer_from = obj->layer_from;
                new_layer_to = obj->layer_to;
                if (!GV->is_sch)
                        new_mirror_text_on = TextMirror->isChecked();
                else
                        new_mirror_text_on = obj->mirror_text_on;
		break;
	case O_PIN:
		{
		if (TextNumber->text().isEmpty())
			{
			QMessageBox::warning(0,tr("Warning"),tr("Number can't be empty"));
			return;
			}
                myGraphicsItem *p = GV->find_pin(TextNumber->text().toInt());
		if (p)
			if (p != obj)
				{
				QMessageBox::warning(0,tr("Warning"),tr("Duplicated Number. Change it."));
				return;
				}
		new_number = TextNumber->text().toInt();
		new_text = TextName->text();
		if (orientation_top->isChecked())
			new_text_position = 'T';
		else if (orientation_bottom->isChecked())
			new_text_position = 'B';
		else if (orientation_left->isChecked())
			new_text_position = 'L';
		else if (orientation_right->isChecked())
			new_text_position = 'R';	
                new_font_height = internal_font_height;
                new_font_bold = internal_font_bold;
                new_font_italic = internal_font_italic;
                new_shape = Pin_shape->currentIndex();
                new_mirror_text_on = FALSE;
                new_layer_from = obj->layer_from;
                new_layer_to = obj->layer_to;
                }
		break;
        case O_POWER:
                if (TextName->text().trimmed().isEmpty())
                        {
                        QMessageBox::warning(0,tr("Warning"),tr("Text can't be empty"));
                        return;
                        }
                new_text_position = 'L';
                new_text = TextName->text().trimmed();
                new_font_height = internal_font_height;
                new_font_bold = internal_font_bold;
                new_font_italic = internal_font_italic;
                new_shape = Power_shape->currentIndex();
                new_fill_polygon = Show_name->isChecked();
                new_mirror_text_on = FALSE;
                new_layer_from = obj->layer_from;
                new_layer_to = obj->layer_to;
                break;
	case O_PORT:
		if (TextName->text().trimmed().isEmpty())
			{
			QMessageBox::warning(0,tr("Warning"),tr("Text can't be empty"));
			return;
			}
		new_text = TextName->text().trimmed();
		if (orientation_top->isChecked())
			new_text_position = 'T';
		else if (orientation_bottom->isChecked())
			new_text_position = 'B';
		else if (orientation_left->isChecked())
			new_text_position = 'L';
		else if (orientation_right->isChecked())
			new_text_position = 'R';	
                new_font_height = internal_font_height;
                new_font_bold = internal_font_bold;
                new_font_italic = internal_font_italic;
                new_shape = Port_shape->currentIndex();
                new_mirror_text_on = FALSE;
                new_fill_polygon = FALSE;
                new_layer_from = obj->layer_from;
                new_layer_to = obj->layer_to;


                break;
	case O_PAD:
                {
                if (file_type == FT_PCB_SYMBOL)
                        {
                        if (TextNumber->text().isEmpty())
                                {
                                QMessageBox::warning(0,tr("Warning"),tr("Number can't be empty"));
                                return;
                                }
                        myGraphicsItem *p = GV->find_pad(TextNumber->text().toInt());
                        if (p)
                                if (p != obj)
                                        {
                                        QMessageBox::warning(0,tr("Warning"),tr("Duplicated Number. Change it."));
                                        return;
                                        }
                        }
                new_number = TextNumber->text().toInt();
                pad->update_new_values();
                if (pad->Width->warning_not_valid_parameter(tr("Pad width")))
                        return;
                if (pad->Height->warning_not_valid_parameter(tr("Pad height")))
                        return;
                if (pad->Drill->warning_not_valid_parameter(tr("Drill diameter")))
                        return;
                new_shape = pad->pv_shape;
                new_width = pad->pv_width;
                new_height = pad->pv_height;
                new_layer_from = pad->pv_layer_from;
                new_layer_to = pad->pv_layer_to;
                new_drill = pad->pv_drill;
                }
                break;
	case O_VIA:
		{
                via->update_new_values();
                if (via->Width->warning_not_valid_parameter(tr("Via Diameter")))
			return;
                if (via->Drill->warning_not_valid_parameter(tr("Drill")))
                        return;
                new_shape = PS_ROUND;
                new_width = via->pv_width;
                new_height = via->pv_width; //Nelle vie Height coincide con Width
                new_layer_from = via->pv_layer_from;
                new_layer_to = via->pv_layer_to;
                new_drill = via->pv_drill;
                }
		break;
	}	

	QDialog::accept(); 
}
