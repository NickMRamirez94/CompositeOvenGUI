#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include <QtDebug>
#include <QFileInfo>
#include <QTableWidget>
#include <QScrollArea>
#include <QComboBox>
#include <QPushButton>
#include "curecycle.h"
#include "curecyclestage.h"
#include <QLabel>

enum StageType
{
    kRamp,
    kHold,
    kDeramp
};

///
/// \brief MainWindow::MainWindow
/// \param parent
///
MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );
    cureCycleTabWidget = new QTabWidget();
    cureCycleTabWidget->setTabsClosable( true );
    setCentralWidget( cureCycleTabWidget );

    connect( cureCycleTabWidget, SIGNAL( tabCloseRequested( int ) ), this, SLOT( closeTab( int ) ) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

///
/// \brief MainWindow::closeTab
/// \param tab
///
void MainWindow::closeTab( int tab )
{
   cureCycleTabWidget->removeTab( tab );
}
///
/// \brief MainWindow::on_actionExit_triggered
///
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

///
/// \brief MainWindow::on_actionSave_Cure_Cycle_triggered
///
void MainWindow::on_actionSave_Cure_Cycle_triggered()
{

}

///
/// \brief MainWindow::on_actionLoad_Cure_Cycle_triggered
/// Triggers file dialog for the user to select cure cycle file.
/// This file will be parsed through another class method.
///
void MainWindow::on_actionLoad_Cure_Cycle_triggered()
{
    QString file_path = QFileDialog::getOpenFileName( this, "Load a Cure Cycle", QDir::homePath() );
    if( !file_path.isEmpty() )
    {
        ReadFile( file_path );
    }
}

///
/// \brief MainWindow::on_actionCreate_New_Cure_Cycle_triggered
/// Triggers an instance of the CureCycleDialog class.
void MainWindow::on_actionCreate_New_Cure_Cycle_triggered()
{
    cure_cycle_dialog_ = new CureCycleDialog( this );
    cure_cycle_dialog_->show();
    connect( cure_cycle_dialog_, SIGNAL( send_name_temp_rate( QStringList ) ), this, SLOT( create_new_cycle( QStringList ) ) );
}

///
/// \brief MainWindow::create_new_cycle
/// \param data
///
void MainWindow::create_new_cycle( QStringList data )
{
    QString cycle_name = data[0];
    QString temperature = data[1];
    QString rate = data[2];

    QTableWidget * curecycleTableWidget = InitializeTable( cycle_name, temperature, rate );
    connect( curecycleTableWidget, SIGNAL( cellChanged( int, int ) ), SLOT( updateCells( int, int ) ) );
}

///
/// \brief MainWindow::on_actionGraph_triggered
///
void MainWindow::on_actionGraph_triggered()
{
    QString file_path = QFileDialog::getOpenFileName( this, "Load Graph Health Data", QDir::homePath() );
    if( !file_path.isEmpty() )
    {
        graph_dialog_ = new GraphDialog ( this, file_path );
        graph_dialog_->show();
    }
}

///
/// \brief MainWindow::readFile
///
bool MainWindow::ReadFile( const QString &file_path )
{
    QFile file( file_path );
    if( !file.open( QFile::ReadOnly | QFile::Text ) )
    {
        QMessageBox::warning( this, "Open File Failed", "Failed to open file" );
        return false;
    }

    QTextStream input( &file );
    QString cure_cycle_data = input.readAll();
    file.close();

    LoadData( cure_cycle_data );

    return true;
}

///
/// \brief MainWindow::LoadData
/// \param cure_cycle_data
///
void MainWindow::LoadData( const QString &cure_cycle_data )
{
    QStringList data = cure_cycle_data.split( "\n", QString::SkipEmptyParts );
    QString cycle_name = data[0];

    //Used for comboboxes
    QStringList types;
    types.append( "Ramp" );
    types.append( "Hold" );
    types.append( "Deramp" );

    //Parse data and populate table
    //Remove cycle name and file size
    data.removeFirst();
    data.removeFirst();

    QStringList first_stage = data[0].split( ":" );
    QTableWidget * curecycleTableWidget = InitializeTable( cycle_name, first_stage[1], first_stage[2] );

    //Start at 1 skip first stage
    for( int i = 1; i < data.size(); i++ )
    {
        QStringList stage_text = data[i].split( ":" );
        QString stage_type;
        QString temperature = stage_text[1];
        QString rate = stage_text[2];
        int index = 0;
        QTableWidgetItem * temperature_item = new QTableWidgetItem( temperature );
        QTableWidgetItem * rate_item = new QTableWidgetItem( rate );
        QLabel * label = new QLabel();

        if( stage_text[0] == "R")
        {
            stage_type = "Ramp";
            index = kRamp;
            label->setText( "Degrees/Minute" );
        }
        else if( stage_text[0] == "H")
        {
            stage_type = "Hold";
            index = kHold;
            temperature_item->setFlags( temperature_item->flags() ^ Qt::ItemIsEditable );
            label->setText( "Seconds" );
        }
        else if( stage_text[0] == "D" )
        {
            stage_type = "Deramp";
            index = kDeramp;
            label->setText( "Degrees/Minute" );
        }
        else
        {
            QMessageBox::warning( this, "Read File Failed", "Error in file" );
            return;
        }
        curecycleTableWidget->insertRow( curecycleTableWidget->rowCount() );
        int new_row = curecycleTableWidget->rowCount() - 1;

        QComboBox * stage_type_combo = new QComboBox();
        stage_type_combo->addItems( types );
        stage_type_combo->setCurrentIndex( index );
        stage_type_combo->setProperty( "row", static_cast<int>( new_row ) );
        connect( stage_type_combo, SIGNAL( currentIndexChanged( int ) ), SLOT( updateCombo( int ) ) );

        curecycleTableWidget->setCellWidget( new_row, 0, stage_type_combo );
        curecycleTableWidget->setItem( new_row, 1, temperature_item );
        curecycleTableWidget->setItem( new_row, 2, rate_item );
        curecycleTableWidget->setCellWidget( new_row, 3, label );
    }
    connect( curecycleTableWidget, SIGNAL( cellChanged( int, int ) ), SLOT( updateCells( int, int ) ) );
}

///
/// \brief MainWindow::add_button_pressed
///
void MainWindow::add_button_pressed()
{
    //Add row
    QTableWidget * table = cureCycleTabWidget->widget( cureCycleTabWidget->currentIndex() )->findChild<QTableWidget *>();
    table->insertRow( table->rowCount() );
    const int new_row = table->rowCount() - 1;

    //Create the cells
    QStringList types;
    types.append( "Ramp" );
    types.append( "Hold" );
    types.append( "Deramp" );
    QComboBox * stage_type_combo = new QComboBox();
    stage_type_combo->addItems( types );
    stage_type_combo->setProperty( "row", static_cast<int>( new_row ) );
    QLabel * label = new QLabel( "Degrees/Minute" );

    //Set the cells
    table->setCellWidget( new_row, 0, stage_type_combo );
    table->setItem( new_row, 1, new QTableWidgetItem( "0" ) );
    table->setItem( new_row, 2, new QTableWidgetItem( "0" ) );
    table->setCellWidget( new_row, 3, label );

    connect( stage_type_combo, SIGNAL( currentIndexChanged( int ) ), SLOT( updateCombo( int ) ) );
}

///
/// \brief MainWindow::remove_button_pressed
///
void MainWindow::remove_button_pressed()
{
    //Remove row
    QTableWidget * table = cureCycleTabWidget->widget( cureCycleTabWidget->currentIndex() )->findChild<QTableWidget *>();
    table->removeRow( table->rowCount() - 1 );
}

///
/// \brief MainWindow::InitializeTable
/// \param name
/// \param temperature
/// \param rate
/// \return
///
QTableWidget * MainWindow::InitializeTable( const QString &cycle_name, const QString &temperature, const QString &rate )
{
    //Initialize QScrollArea
    QScrollArea * scrollArea = new QScrollArea();
    QWidget * widget = new QWidget();
    QVBoxLayout * vertical = new QVBoxLayout();
    scrollArea->setWidgetResizable( true );
    widget->setLayout( vertical );
    int new_tab_index = cureCycleTabWidget->addTab( widget, cycle_name );
    cureCycleTabWidget->setCurrentIndex( new_tab_index );
    vertical->addWidget( scrollArea );

    //Initialize QTableWidget
    QTableWidget * curecycleTableWidget = new QTableWidget( 0, 4, scrollArea );
    QStringList header_names;
    header_names.append( "Stage Type" );
    header_names.append( "Temperature (Deg)" );
    header_names.append( "" );
    header_names.append( "" );
    curecycleTableWidget->setHorizontalHeaderLabels( header_names );
    curecycleTableWidget->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
    scrollArea->setWidget( curecycleTableWidget );

    //Create the cells
    QComboBox * stage_type_combo = new QComboBox();
    stage_type_combo->addItem( "Ramp" );
    QLabel * label = new QLabel( "Degrees/Minute" );

    curecycleTableWidget->insertRow( curecycleTableWidget->rowCount() );

    //Set the cells
    curecycleTableWidget->setCellWidget( curecycleTableWidget->rowCount() - 1, 0, stage_type_combo );
    curecycleTableWidget->setItem( curecycleTableWidget->rowCount() - 1, 1, new QTableWidgetItem( temperature ) );
    curecycleTableWidget->setItem( curecycleTableWidget->rowCount() - 1, 2, new QTableWidgetItem( rate ) );
    curecycleTableWidget->setCellWidget( curecycleTableWidget->rowCount() - 1, 3, label );

    //Add buttons
    QWidget * buttons = new QWidget( cureCycleTabWidget );
    QPushButton * add_button = new QPushButton( "Add", buttons );
    QPushButton * remove_button = new QPushButton( "Remove", buttons );

    QHBoxLayout * horizontal = new QHBoxLayout();
    buttons->setLayout( horizontal );
    horizontal->addWidget( add_button );
    horizontal->addWidget( remove_button );
    vertical->addWidget( buttons );

    connect( add_button, SIGNAL( clicked() ), this, SLOT( add_button_pressed() ) );
    connect( remove_button, SIGNAL( clicked() ), this, SLOT( remove_button_pressed() ) );

    return curecycleTableWidget;
}

///
/// \brief MainWindow::updateCells
/// \param row
/// \param column
///
/// If next stage is hold: update its value to that of changed cell
/// If cell was changed it must be ramp/deramp
void MainWindow::updateCells( int row, int column )
{
    //TODO: Causing segfault when editing last row because going out of bounds
    const int stage_column = 0;
    const int temperature_column = 1;
    QTableWidget * table = cureCycleTabWidget->widget( cureCycleTabWidget->currentIndex() )->findChild<QTableWidget *>();
    //Don't update cell was in law row
    if( row < table->rowCount() - 1 )
    {
        QComboBox * stage_type_combo = static_cast<QComboBox *>( table->cellWidget( row + 1, stage_column ) );

        const int next_stage_index = stage_type_combo->currentIndex();
        //Update value
        if( next_stage_index == kHold )
        {
            QTableWidgetItem * temperature_item_edited = table->item( row, temperature_column );
            const QString temperature = temperature_item_edited->text();
            QTableWidgetItem * temperature_item_hold = table->item( row + 1, temperature_column );
            temperature_item_hold->setText( temperature );

        }
    }
}

///
/// \brief MainWindow::updateCombo
/// \param index
///
/// Set temperature editability
/// Set label
/// If hold: update hold temperature to temperature of previous stage
void MainWindow::updateCombo( int index )
{
    QTableWidget * table = cureCycleTabWidget->widget( cureCycleTabWidget->currentIndex() )->findChild<QTableWidget *>();
    int row = sender()->property( "row" ).toInt();
    const int temperature_column = 1;
    const int label_column = 3;
    QTableWidgetItem * temperature_item = table->item( row, temperature_column );
    QLabel * label = static_cast<QLabel*>( table->cellWidget( row, label_column ) );

    //Disable editing and set label
    if( index == kHold )
    {
        temperature_item->setFlags( temperature_item->flags() ^ Qt::ItemIsEditable );
        label->setText( "Seconds" );
        //Update hold temperature to temperature of previous stage
        const QString temperature = table->item( row - 1, temperature_column )->text();
        temperature_item->setText( temperature );
    }
    //Enable editing and set label
    else
    {
        temperature_item->setFlags( temperature_item->flags() | Qt::ItemIsEditable );
        label->setText( "Degrees/Minute" );
    }
}
