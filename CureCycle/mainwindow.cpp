#include "mainwindow.h"
#include "ui_mainwindow.h"

//Qt Library Files
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
#include <QLabel>
#include <QSerialPortInfo>
#include <QList>

//Custom Files
#include "curecycle.h"
#include "curecyclestage.h"
#include "utilities.h"

///
/// \brief MainWindow::MainWindow
/// \param parent - Parent widget.
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

///
/// \brief MainWindow::~MainWindow
///
MainWindow::~MainWindow()
{
    delete ui;
}

///
/// \brief MainWindow::closeTab
/// \param tab - Number of the tab to close of TabWidget.
/// Remove the specified tab from the CureCycleTabWidget.
///
void MainWindow::closeTab( int tab )
{
   cureCycleTabWidget->removeTab( tab );
   cycle_names_.removeAt( tab );
}
///
/// \brief MainWindow::on_actionExit_triggered
/// Quit the application.
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
    QTableWidget * table = GetTableWidget();
    if( table )
    {
        QStringList cure_cycle_data = Utilities::GetData( GetTableWidget() );
        QString file_path = QFileDialog::getSaveFileName( this, "Save Cure Cycle", QDir::homePath() );
        QString cycle_name = cycle_names_[cureCycleTabWidget->currentIndex()];
        Utilities::SaveData( cure_cycle_data, cycle_name, file_path );
    }
}

///
/// \brief MainWindow::on_actionLoad_Cure_Cycle_triggered
/// - Triggers file dialog for the user to select cure cycle file.
/// - Call ReadFile() passing the specified file path.
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
/// - Triggers an instance of the CureCycleDialog class.
/// - Connect send_name_temp_rate signal to create_new_cycle slot.
///
void MainWindow::on_actionCreate_New_Cure_Cycle_triggered()
{
    cure_cycle_dialog_ = new CureCycleDialog( this );
    cure_cycle_dialog_->show();
    connect( cure_cycle_dialog_, SIGNAL( send_name_temp_rate( QStringList ) ), this, SLOT( create_new_cycle( QStringList ) ) );
}

///
/// \brief MainWindow::on_actionSend_Data_triggered
/// - Uses Utilities::SendData() to send cure cycle data on current
/// QTabWidet to composite oven.
///
void MainWindow::on_actionSend_Data_triggered()
{
    //Verify that there is an exisiting cure cycle
    if( CheckTabWidget() )
    {
        QMessageBox::StandardButton reply = QMessageBox::question( this, "", "Is Oven Ready?", QMessageBox::Yes|QMessageBox::No );
        if( reply == QMessageBox::Yes )
        {
            Utilities::SendData( cycle_names_[cureCycleTabWidget->currentIndex()], Utilities::GetData( GetTableWidget() ) );
        }
    }
}

///
/// \brief MainWindow::create_new_cycle
/// \param data - QStringList containing cycle name, temperature, and rate
/// - Retrieve corresponding data from parameter.
/// - Call InitializeTable() passing that data.
/// - Connect cellChanged signal to updateCells() slot with the returned QTableWidget.
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
/// - Trigget QFileDialog to get the file path.
/// - Create instance of GraphDialog passing that retrieved file path.
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
/// \brief MainWindow::ReadFile
/// \param file_path - QString of file path to file to read.
/// \return - Boolean. False if unsuccesful retrieving data from file. True if successful.
/// - Open file.
/// - Read data from file.
/// - Call LoadData() passing read text stream.
///
bool MainWindow::ReadFile( const QString &file_path )
{
    bool status = false;
    QFile file( file_path );
    if( !file.open( QFile::ReadOnly | QFile::Text ) )
    {
        QMessageBox::warning( this, "Open File Failed", "Failed to open file" );
        status = false;
    }

    QTextStream input( &file );
    QString cure_cycle_data = input.readAll();
    file.close();

    QStringList data = cure_cycle_data.split( "\n", QString::SkipEmptyParts );
    QString header = data[0];
    if( header != "CURECYCLEFILE" )
    {
        QMessageBox::warning( this, "Open File Failed", "Incorrect File Type" );
        status = false;
    }
    else
    {
        LoadData( data );
        status = true;
    }
    return status;
}

///
/// \brief MainWindow::LoadData
/// \param data - Data read from cure cycle file.
/// - Call InitializeTable() passing initial ramp and cycle name.
/// - Parse remaining data and load QTableWidget with data.
/// - Connect QTableWidget cell signals and combobox signals.
///
void MainWindow::LoadData( QStringList &data )
{
    QString cycle_name = data[1];

    //Used for comboboxes
    QStringList types;
    types.append( "Ramp" );
    types.append( "Hold" );
    types.append( "Deramp" );

    //Parse data and populate table
    //Remove header and cycle name
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
            label->setText( "Minutes" );
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

        curecycleTableWidget->setCellWidget( new_row, kStageTypeColumn, stage_type_combo );
        curecycleTableWidget->setItem( new_row, kTemperatureColumn, temperature_item );
        curecycleTableWidget->setItem( new_row, kRateTimeColumn, rate_item );
        curecycleTableWidget->setCellWidget( new_row, kLabelColumn, label );
    }
    connect( curecycleTableWidget, SIGNAL( cellChanged( int, int ) ), SLOT( updateCells( int, int ) ) );
}

///
/// \brief MainWindow::add_button_pressed
/// - Add new row to the QTableWidget.
/// - Connect combobox signal to updateCombo() slot.
///
void MainWindow::add_button_pressed()
{
    //Add row
    QTableWidget * table = GetTableWidget();
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
    table->setCellWidget( new_row, kStageTypeColumn, stage_type_combo );
    table->setItem( new_row, kTemperatureColumn, new QTableWidgetItem( "0" ) );
    table->setItem( new_row, kRateTimeColumn, new QTableWidgetItem( "0" ) );
    table->setCellWidget( new_row, kLabelColumn, label );

    connect( stage_type_combo, SIGNAL( currentIndexChanged( int ) ), SLOT( updateCombo( int ) ) );
}

///
/// \brief MainWindow::remove_button_pressed
/// Remove last row from QTableWidget.
///
void MainWindow::remove_button_pressed()
{
    //Remove row
    QTableWidget * table = GetTableWidget();
    table->removeRow( table->rowCount() - 1 );
}

///
/// \brief MainWindow::InitializeTable
/// \param name - Cure cycle name.
/// \param temperature - Cure cycle initial ramp temperature.
/// \param rate - Cure cycle initial ramp rate.
/// \return - Pointer to created QTableWidget.
/// - Append header.
/// - Create QComboBox and QLabel.
/// - Append data to new row of QTableWidget
/// - Add add and remove buttons and connect their signals and slots.
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
    cycle_names_.append( cycle_name );
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
    curecycleTableWidget->setCellWidget( curecycleTableWidget->rowCount() - 1, kStageTypeColumn, stage_type_combo );
    curecycleTableWidget->setItem( curecycleTableWidget->rowCount() - 1, kTemperatureColumn, new QTableWidgetItem( temperature ) );
    curecycleTableWidget->setItem( curecycleTableWidget->rowCount() - 1, kRateTimeColumn, new QTableWidgetItem( rate ) );
    curecycleTableWidget->setCellWidget( curecycleTableWidget->rowCount() - 1, kLabelColumn, label );

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
/// \param row - Row number of cell that was modified.
/// \param column - column number of cell that was modified.
/// - Get QTableWidget on current tab.
/// - Check if next stage is Hold stage.
/// - If it is, then update its temperature to edited temperature.
///
void MainWindow::updateCells( int row, int column )
{
    QTableWidget * table = GetTableWidget();

    //Make sure updated cell is a number
    QString edited_text = table->item( row, column )->text();
    if( Utilities::CheckNumber( edited_text ) )
    {
        //Don't update cell was in law row
        if( row < table->rowCount() - 1 )
        {
            QComboBox * stage_type_combo = static_cast<QComboBox *>( table->cellWidget( row + 1, kStageTypeColumn ) );

            const int next_stage_index = stage_type_combo->currentIndex();
            //Update value
            if( next_stage_index == kHold )
            {
                QTableWidgetItem * temperature_item_edited = table->item( row, kTemperatureColumn );
                const QString temperature = temperature_item_edited->text();
                QTableWidgetItem * temperature_item_hold = table->item( row + 1, kTemperatureColumn );
                temperature_item_hold->setText( temperature );

            }
        }
    }
    else
    {
        table->item( row, kTemperatureColumn )->setText( "0" );
        table->item( row, kRateTimeColumn )->setText( "0" );
        QMessageBox::warning( this, "Data Error", "Data must be a positive number" );
    }
}

///
/// \brief MainWindow::updateCombo
/// \param index - Index of QComboBox that combobox was changed to.
/// - If hold stage, then make temperature uneditable, update hold temperature to that
/// of previous stage, and update label.
/// - It not hold stage, then make temperature editable and update label.
///
void MainWindow::updateCombo( int index )
{
    QTableWidget * table = cureCycleTabWidget->widget( cureCycleTabWidget->currentIndex() )->findChild<QTableWidget *>();
    int row = sender()->property( "row" ).toInt();
    QTableWidgetItem * temperature_item = table->item( row, kTemperatureColumn );
    QLabel * label = static_cast<QLabel*>( table->cellWidget( row, kLabelColumn ) );

    //Disable editing and set label
    if( index == kHold )
    {
        temperature_item->setFlags( temperature_item->flags() ^ Qt::ItemIsEditable );
        label->setText( "Minutes" );
        //Update hold temperature to temperature of previous stage
        const QString temperature = table->item( row - 1, kTemperatureColumn )->text();
        temperature_item->setText( temperature );
    }
    //Enable editing and set label
    else
    {
        temperature_item->setFlags( temperature_item->flags() | Qt::ItemIsEditable );
        label->setText( "Degrees/Minute" );
    }
}

///
/// \brief MainWindow::GetTableWidget
/// \return - Pointer to QTableWidget on current tab
/// Searches children for QTableWidget of the active tab on the QTabWidget.
///
inline QTableWidget * MainWindow::GetTableWidget()
{
    return cureCycleTabWidget->widget( cureCycleTabWidget->currentIndex() )->findChild<QTableWidget *>();
}

///
/// \brief MainWindow::CheckTabWidget
/// \return - True if there is at least one tab on QTabWidget false otherwise.
///
inline bool MainWindow::CheckTabWidget()
{
    return cureCycleTabWidget->count();
}
