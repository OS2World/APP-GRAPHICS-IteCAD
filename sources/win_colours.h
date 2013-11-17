#ifndef WIN_COLOURS_H
	#define WIN_COLOURS_H

	#include "tipi_dato.h"
	#include "win_main.h"
	#include "QTimer"	
	#include "QVariant"
	#include "QAction"
	#include "QApplication"
	#include "QDialog"
	#include "QGroupBox"
	#include "QScrollBar"
	#include "QButtonGroup"
	#include "QGridLayout"
	#include "QHBoxLayout"
	#include "QVBoxLayout"
	#include "QTableWidget"
	#include "QSpinBox"
	#include "QLabel"
	#include "QPushButton"
	#include "QSpacerItem"
	#include "QWidget"
	#include "QDesktopWidget"
	#include "QColorDialog"
	
	

	class win_colours : public QDialog
	{	
		Q_OBJECT	//Necessario per gestire Signals e Slots	
	public:
		win_colours(QWidget *parent, Qt::WFlags tipo);
		~win_colours();
		
		QTableWidget	*table_sch_workspace;
		QTableWidget	*table_sch_layers;
		QTableWidget	*table_pcb_workspace;
		QTableWidget	*table_pcb_layers;
		QRect			rect_sch_workspace;
		
		QGroupBox		*setup_workspace(bool is_sch);
		QGroupBox		*setup_layers(bool is_sch);

		QHBoxLayout		*setup_buttons();

		QTimer	*timer_selection;

	public slots:
		void		change_color(QTableWidgetItem *item);
		void		timeout();

	protected:
		void		accept();
	};

#endif
