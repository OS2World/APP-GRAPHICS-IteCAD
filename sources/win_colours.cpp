#include "win_colours.h"
#include "tipi_dato.h"

extern	char file_type;	

extern	lay_data	layer_data[LAYER_DATA_COUNT];
extern	lay_data	workspace_data[WORKSPACE_DATA_COUNT];

extern	win_main	*w_main;

win_colours::win_colours(QWidget *parent, Qt::WFlags tipo)
        : QDialog(parent, tipo)
	{
	// Se questo QDialog viene creato con tipo == Qt::Dialog allora sono costretto ad avere il ? in alto a destra
	// Ciò forse è un bene perchè mi costringerà ad avere un minimo di help
	// Veder Qt::WindowContextHelpButtonHint per avere maggiori informazioni

	setWindowTitle(tr("Colours"));	
	//setSizeGripEnabled(true);
    setModal(true);

	//Imposto la larghezza minima pari a metà dello schermo
	//QRect screen = QApplication::desktop()->screenGeometry();
	//setMinimumWidth(screen.width() / 2);
	
	QGridLayout	*layout = new QGridLayout(this);
	layout->addWidget(setup_workspace(TRUE),1,0);
	layout->addWidget(setup_workspace(FALSE),1,1);
	layout->addWidget(setup_layers(TRUE),2,0);
	layout->addWidget(setup_layers(FALSE),2,1);
	layout->addLayout(setup_buttons(),3,0,TRUE,FALSE);
	layout->setSpacing(6);
    layout->setMargin(8);

	// Definisco il Timer	//RICORDARSI DI ELIMINARE IL TIMER CON KILLTIMER
	timer_selection = new QTimer( this ); // create internal timer
	connect( timer_selection, SIGNAL(timeout()), SLOT(timeout()));
	timer_selection->start( 200 );	//Ogni 2 decimi di secondi avvio il timeout
	}

win_colours::~win_colours()
    {
	QObject::killTimer(timer_selection->timerId());
	}

void win_colours::timeout()
    {
	static int riga;
	static QTableWidget *tabella = 0;
	int new_riga = 0;
	QTableWidget *new_tabella = 0;
	
	if (table_sch_workspace->underMouse())
		{
		QTableWidgetItem *item = table_sch_workspace->itemAt(table_sch_workspace->mapFromGlobal(QCursor::pos()));
		if (item != 0)
			{
			//Sono su una cella su cui è possibile cliccare
			new_riga = table_sch_workspace->row(item);
			new_tabella = table_sch_workspace;
			}
		}
	else if (table_pcb_workspace->underMouse())
		{
		QTableWidgetItem *item = table_pcb_workspace->itemAt(table_pcb_workspace->mapFromGlobal(QCursor::pos()));
		if (item != 0)
			{
			//Sono su una cella su cui è possibile cliccare
			new_riga = table_pcb_workspace->row(item);
			new_tabella = table_pcb_workspace;
			}
		}
	else if (table_sch_layers->underMouse())
		{
		QTableWidgetItem *item = table_sch_layers->itemAt(table_sch_layers->mapFromGlobal(QCursor::pos()));
		if (item != 0)
			{
			//Sono su una cella su cui è possibile cliccare
			new_riga = table_sch_layers->row(item);
			new_tabella = table_sch_layers;
			}
		}
	else if (table_pcb_layers->underMouse())
		{
		QTableWidgetItem *item = table_pcb_layers->itemAt(table_pcb_layers->mapFromGlobal(QCursor::pos()));
		if (item != 0)
			{
			//Sono su una cella su cui è possibile cliccare
			new_riga = table_pcb_layers->row(item);
			new_tabella = table_pcb_layers;
			}
		}

	if ((new_riga!= riga) || (new_tabella != tabella))
		{
		//Deseleziono prima la vecchia riga
		if (tabella != 0)
			tabella->setItemSelected(tabella->item(riga,1),FALSE);
		//Seleziono la nuova riga
		if (new_tabella != 0)
			new_tabella->setItemSelected(new_tabella->item(new_riga,1),TRUE);
		//Memorizzo i nuovi dati
		riga = new_riga;
		tabella = new_tabella;
		}
	}

QGroupBox *win_colours::setup_workspace(bool is_sch)
	{
		QGroupBox *groupBox;
		QTableWidget *tw;
		
		QTableWidgetItem *descr;
		QTableWidgetItem *colore;
		QHeaderView *vh;
		QHeaderView *hh;        
		
		if (is_sch)
			{
			groupBox = new QGroupBox(tr("SCH workspace"));
			table_sch_workspace = new QTableWidget(WORKSPACE_DATA_SCH_COUNT, 2, this);
			tw = table_sch_workspace;
			}
		else
			{
			groupBox = new QGroupBox(tr("PCB workspace"));
			table_pcb_workspace = new QTableWidget(WORKSPACE_DATA_PCB_COUNT, 2, this);
			tw = table_pcb_workspace;
			}
		
		//tw->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
		tw->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		tw->setFocusPolicy(Qt::NoFocus);
		tw->setSelectionMode(QAbstractItemView::NoSelection);	//Seleziono io dal codice, che è meglio
		vh = tw->verticalHeader();
		vh->setHidden(TRUE);
		hh = tw->horizontalHeader();
		hh->setHidden(TRUE);
		tw->setEditTriggers(QAbstractItemView::NoEditTriggers);	//Le caselle non sono editabili		
		int	riga = 0;
		for (int i = 0; i < WORKSPACE_DATA_COUNT; ++i)
			if (workspace_data[i].is_sch == is_sch)
				{
				descr = new QTableWidgetItem(workspace_data[i].description);
				tw->setItem(riga,1, descr);
				colore = new QTableWidgetItem("  ");
				colore->setBackgroundColor(workspace_data[i].color);
				tw->setItem(riga,0, colore);
				tw->resizeRowToContents(riga);
				riga++;
				}		
		tw->resizeColumnToContents(1);
		//Purtroppo table_pcb_layers->resizeColumnToContents(0); non funziona...
		hh->resizeSection(0,24); 
		hh->setStretchLastSection(TRUE);
		connect(tw, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(change_color(QTableWidgetItem *)));

		QGridLayout *gbox = new QGridLayout;
        gbox->addWidget(tw,0,0);//,TRUE,FALSE);
		
		groupBox->setLayout(gbox);

		return groupBox;	
	}

QGroupBox *win_colours::setup_layers(bool is_sch)
	{
		QGroupBox *groupBox;
		QTableWidget *tw;
		
		QTableWidgetItem *descr;
		QTableWidgetItem *colore;
		QHeaderView *vh;
		QHeaderView *hh;        
		
		if (is_sch)
			{
			groupBox = new QGroupBox(tr("SCH layers"));
			table_sch_layers = new QTableWidget(LAYER_DATA_SCH_COUNT, 2, this);
			tw = table_sch_layers;
			}
		else
			{
			groupBox = new QGroupBox(tr("PCB layers"));
			table_pcb_layers = new QTableWidget(LAYER_DATA_PCB_COUNT, 2, this);
			tw = table_pcb_layers;
			}
		
		//tw->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
		tw->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		tw->setFocusPolicy(Qt::NoFocus);
		tw->setSelectionMode(QAbstractItemView::NoSelection);	//Seleziono io dal codice, che è meglio
		vh = tw->verticalHeader();
		vh->setHidden(TRUE);
		hh = tw->horizontalHeader();
		hh->setHidden(TRUE);
		tw->setEditTriggers(QAbstractItemView::NoEditTriggers);	//Le caselle non sono editabili		
		int	riga = 0;
		for (int i = 0; i < LAYER_DATA_COUNT; ++i)
			if (layer_data[i].is_sch == is_sch)
				{
				descr = new QTableWidgetItem(layer_data[i].description);
				tw->setItem(riga,1, descr);
				colore = new QTableWidgetItem("  ");
				colore->setBackgroundColor(layer_data[i].color);
				tw->setItem(riga,0, colore);
				tw->resizeRowToContents(riga);
				riga++;
				}		
		tw->resizeColumnToContents(1);
		//Purtroppo table_pcb_layers->resizeColumnToContents(0); non funziona...
		hh->resizeSection(0,24); 
		hh->setStretchLastSection(TRUE);
		connect(tw, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(change_color(QTableWidgetItem *)));

		QGridLayout *gbox = new QGridLayout;
        gbox->addWidget(tw,0,0);//,TRUE,FALSE);
		
		groupBox->setLayout(gbox);

		return groupBox;	
	}

QHBoxLayout	*win_colours::setup_buttons()
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

void	win_colours::change_color(QTableWidgetItem *item)
{
	if (item == 0)	//Serve se clicco sulla tabella al di fuori delle QTableWidgetItem
		return;
	
	timer_selection->stop();
	timeout();	

	
		int riga;
		if (item->tableWidget() == table_sch_workspace)
			{
			riga = table_sch_workspace->row(item);
			QColor new_color = QColorDialog::getColor(table_sch_workspace->item(riga,0)->backgroundColor());
			if (new_color.isValid())
				{			
//				style_change_color(&new_style_sch_w, riga, new_color);
				table_sch_workspace->item(riga,0)->setBackgroundColor(new_color);
				}
			}
		else if (item->tableWidget() == table_sch_layers)
			{
			riga = table_sch_layers->row(item);
			QColor new_color = QColorDialog::getColor(table_sch_layers->item(riga,0)->backgroundColor());
			if (new_color.isValid())
				{	
//				style_change_color(&new_style_sch_l, table_sch_layers->row(item), new_color);
				table_sch_layers->item(riga,0)->setBackgroundColor(new_color);
				}
			}
		else if (item->tableWidget() == table_pcb_workspace)
			{
			riga = table_pcb_workspace->row(item);
			QColor new_color = QColorDialog::getColor(table_pcb_workspace->item(riga,0)->backgroundColor());
			if (new_color.isValid())
				{	
//				style_change_color(&new_style_pcb_w, riga, new_color);
				table_pcb_workspace->item(riga,0)->setBackgroundColor(new_color);
				}
			}
		else if (item->tableWidget() == table_pcb_layers)
			{
			riga = table_pcb_layers->row(item);
			QColor new_color = QColorDialog::getColor(table_pcb_layers->item(riga,0)->backgroundColor());
			if (new_color.isValid())
				{	
//				style_change_color(&new_style_pcb_l, table_pcb_layers->row(item), new_color);
				table_pcb_layers->item(riga,0)->setBackgroundColor(new_color);
				}	
			}		

	timer_selection->start();
}

void	win_colours::accept()
{
	int sch_row = 0;
	int pcb_row = 0;
	for (int i = 0; i < LAYER_DATA_COUNT; ++i)
		if (layer_data[i].is_sch)
			layer_data[i].color = table_sch_layers->item(sch_row++,0)->backgroundColor();
		else
			layer_data[i].color = table_pcb_layers->item(pcb_row++,0)->backgroundColor();
	sch_row = 0;
	pcb_row = 0;
	for (int i = 0; i < WORKSPACE_DATA_COUNT; ++i)
		if (workspace_data[i].is_sch)
			workspace_data[i].color = table_sch_workspace->item(sch_row++,0)->backgroundColor();
		else
			workspace_data[i].color = table_pcb_workspace->item(pcb_row++,0)->backgroundColor();
		
	w_main->sb_refresh();
	QDialog::accept();
}
