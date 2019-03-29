#include "graphdialog.h"
#include "ui_graphdialog.h"
#include "utilities.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtCharts>

using namespace QtCharts;

///
/// \brief GraphDialog::GraphDialog
/// \param parent - Parent widget.
/// \param file_path - File path to the graph data.
///
GraphDialog::GraphDialog( QWidget *parent, const QString & file_path ) :
    QDialog( parent ),
    ui( new Ui::GraphDialog )
{
    ui->setupUi( this );
    temperature_data_ = new QByteArray();
    if( file_path.isEmpty() )
    {
        serial_port_name_ = new QString( Utilities::GetPortName() );
        if( serial_port_name_->isEmpty() )
        {
            qDebug() << "Serial Port was not found";
            this->close();
        }
        else
        {
            serial_ = new QSerialPort();
            serial_->setPortName( *serial_port_name_ );

            if( serial_->open( QIODevice::ReadOnly ) )
            {
                serial_->setBaudRate( QSerialPort::Baud38400 );
                serial_->setDataBits( QSerialPort::Data8 );
                serial_->setParity( QSerialPort::NoParity );
                serial_->setStopBits( QSerialPort::OneStop );
                serial_->setFlowControl( QSerialPort::NoFlowControl );

                timer_ = new QTimer( this );

                connect( serial_, SIGNAL( readyRead() ),  SLOT( ReadSerial() ) );
                connect( timer_, SIGNAL( timeout() ), SLOT( DoneReading() ) );
            }
            else
            {
                qDebug() << "Serial Port failed to open";
                this->close();
            }
        }
    }
    else
    {
        if ( ReadTemperatureData( file_path ) )
        {
            CreateGraph();
        }
    }
}

///
/// \brief GraphDialog::~GraphDialog
///
GraphDialog::~GraphDialog()
{
    if( serial_->isOpen() )
    {
        serial_->close();
    }
    delete ui;
}

void GraphDialog::ReadSerial()
{
    timer_->start( 2000 );
    temperature_data_->append( serial_->readAll() );
}

void GraphDialog::DoneReading()
{
    serial_->close();
    Utilities::SaveTemperatureData( *temperature_data_ );
    CreateGraph();
}

void GraphDialog::CreateGraph()
{
    const int max_temperature = 375;
    QLineSeries * ambient_series = new QLineSeries();
    QLineSeries * part_series = new QLineSeries();
    ambient_series->setName( "Ambient Temperature" );
    part_series->setName( "Part Temperature" );

    QByteArray::iterator ptr = temperature_data_->begin();
    int second = 0;

    //Extract the data
    //Skip the name of the cure cycle
    //ptr + 32 - Only plot one point per second instead of four samples per second
    for( std::advance( ptr, 20 ); ptr < temperature_data_->end(); ptr = ptr + 32 )
    {
        //Part Temperature Data
        quint16 part_temp_one = Utilities::PrepNumber( *ptr, *(ptr + 1) );
        quint16 part_temp_two = Utilities::PrepNumber( *(ptr + 2), *(ptr + 3) );
        quint16 part_temp_average = ( part_temp_one + part_temp_two ) / 2;
        part_series->append( second, part_temp_average );

        //Ambient Temperature Data
        quint16 ambient_temp_one = Utilities::PrepNumber( *(ptr + 4), *(ptr + 5) );
        quint16 ambient_temp_two = Utilities::PrepNumber( *(ptr + 6), *(ptr + 7) );
        quint16 ambient_temp_average = ( ambient_temp_one + ambient_temp_two ) / 2;
        ambient_series->append( second, ambient_temp_average );

        second++;
    }

    //Extract name
    QString name = "";
    QByteArray::iterator end = temperature_data_->begin();
    std::advance( end, 20 );
    for( ptr = temperature_data_->begin(); ptr < end; ptr++ )
    {
        name += *ptr;
    }
    name = name.simplified();

    //TEMPERATURE CHART
    QChart * temperature_chart = new QChart();
    temperature_chart->legend()->setAlignment(  Qt::AlignBottom );
//    chart->addSeries( part_series );
    temperature_chart->addSeries( ambient_series );
    temperature_chart->createDefaultAxes();
    temperature_chart->axes(Qt::Horizontal).first()->setRange(0, second);
    temperature_chart->axes(Qt::Horizontal).first()->setTitleText("Time (Seconds)");
    temperature_chart->axes(Qt::Vertical).first()->setRange(0, max_temperature);
    temperature_chart->axes(Qt::Vertical).first()->setTitleText("Temperature (Fahrenheit)");
    temperature_chart->setTitle( name + " Temperature Data" );
    QChartView * temperature_chart_view = new QChartView( temperature_chart );
    temperature_chart_view->setRenderHint(QPainter::Antialiasing);

    //PRESSURE CHART
    QChart * pressure_chart = new QChart();
    pressure_chart->legend()->setAlignment(  Qt::AlignBottom );
    pressure_chart->setTitle( name + " Pressure Data" );
    QChartView * pressure_chart_view = new QChartView( pressure_chart );
    pressure_chart_view->setRenderHint(QPainter::Antialiasing);

    ui->scrollArea_layout->addWidget( temperature_chart_view );
    ui->scrollArea_layout->addWidget( pressure_chart_view );
}

bool GraphDialog::ReadTemperatureData( const QString & file_path )
{
    bool status = false;
    QFile file( file_path );
    if( file.open( QFile::ReadOnly ) )
    {
        status = true;
        temperature_data_->append( file.readAll() );
        file.close();
    }

    return status;
}
