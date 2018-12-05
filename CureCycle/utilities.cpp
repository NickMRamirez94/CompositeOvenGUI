#include "utilities.h"

//Qt Library Files
#include <QTextStream>
#include <QDir>
#include <QFileDialog>
#include <QComboBox>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

Utilities::Utilities()
{

}

///
/// \brief Utilities::SaveData
/// \param data - Data to save.
/// \param name - Name of cure cycle.
/// \param file_path - Path of save file.
/// \return - True if successful false otherwise.
/// - Validate file_path and data.
/// - Open file.
/// - Output header and cure cycle name.
/// - For each stage in data output data to file.
///
bool Utilities::SaveData( const QStringList &data, const QString &name, const QString &file_path )
{
    if( file_path.isEmpty() || data.isEmpty() )
    {
        return false;
    }
    else
    {
        QFile file( file_path );
        if( !file.open( QFile::WriteOnly ) )
        {
            return false;
        }

        const int stage_count = data.count();

        QTextStream stream( &file );
        stream << "CURECYCLEFILE\n";
        stream << name << "\n";
        for(int i = 0; i < stage_count; i++)
        {
            stream << data[i];
        }
        file.close();
    }
    return true;
}

///
/// \brief Utilities::ReadFile
/// \param file_path
/// \return
///
bool Utilities::ReadFile( const QString &file_path )
{
    bool status = false;
    QFile file( file_path );
    if( file.open( QFile::ReadOnly | QFile::Text ) )
    {
        status = true;
        QTextStream input( &file );
        QString cure_cycle_data = input.readAll();
        file.close();

        QStringList data = cure_cycle_data.split( "\n", QString::SkipEmptyParts );
        QString header = data[0];
        if( header != "CURECYCLEFILE" )
        {
            status = false;
        }
        else
        {
            LoadData( data );
        }
    }
    else
    {
        status = false;
    }
    return status;
}

void Utilities::LoadData( const QStringList &data )
{

}

///
/// \brief Utilities::SendData
/// \param name
/// \param data
/// \return - True if successfully sent data false otherwise.
/// - Get port name based on known vendor and product identifier.
/// - Open port: Format for UART is 8-N-1 115200.
/// - Parse data.
/// - Format data to following: Opcode (Char) 1 byte Rate 2 bytes (limited to max of 10) for HOLD TIME Temperature 2 bytes (max 450) for HOLD 0
/// - Send data through serial port.
///
bool Utilities::SendData( const QString &name, const QStringList &data )
{
    bool port_ready = false;
    QString port_name;
    for (QSerialPortInfo port : QSerialPortInfo::availablePorts())
    {

        if( port.vendorIdentifier() == usbtouart_vendor_identifier && port.productIdentifier() == usbtouart_product_identifier )
        {
            port_ready = true;
            port_name = port.portName();
        }
    }

    if( port_ready )
    {
        QSerialPort serial;
        serial.setPortName( port_name );

        if( serial.open( QIODevice::WriteOnly ) )
        {
            serial.setBaudRate( QSerialPort::Baud115200 );
            serial.setDataBits( QSerialPort::Data8 );
            serial.setParity( QSerialPort::NoParity );
            serial.setStopBits( QSerialPort::OneStop );
            serial.setFlowControl( QSerialPort::NoFlowControl );

            //Send name with space filler if needed
            const int max_length = 20;
            QByteArray name_byte = name.toLocal8Bit();
            const char * name_serial = name_byte.data();
            serial.write( name_serial, max_length );

            int fill_bytes = 0;
            if( name.length() < max_length )
            {
                fill_bytes = max_length - name.length();
            }
            for( int i = 0; i < fill_bytes; i++ )
            {
                serial.write( " " );
            }

            //Send cycle data
            const int stage_count = data.count();
            //Third piece of data written if hold stage
            const QByteArray hold_filler( 2, 0 );
            for( int i = 0; i < stage_count; i++ )
            {
                //Data format-> Type:Temperature:Rate/Time
                QStringList stage_text = data[i].split( ":" );
                QString stage_type = stage_text[0];
                QString temperature = stage_text[1];
                QString rate_time = stage_text[2];

                //Format the data
                QByteArray type_byte = stage_type.toLocal8Bit();
                char * type_serial = type_byte.data();

                quint16 temperature_int = temperature.toUShort();
                quint16 rate_time_int = rate_time.toUShort();
                QByteArray temperature_byte;
                QByteArray rate_time_byte;
                temperature_byte.setNum( temperature_int );
                rate_time_byte.setNum( rate_time_int );

                if( stage_type == "H" )
                {
                    serial.write( type_serial, 1 );
                    serial.write( rate_time_byte );
                    serial.write( hold_filler );
                }
                else
                {
                    serial.write( type_serial, 1 );
                    serial.write( rate_time_byte );
                    serial.write( temperature_byte );
                }
            }
            serial.close();
        }
        else
        {
            port_ready = false;
        }
    }
    else
    {
        qDebug() << "Serial Port failed to open";
    }
    return port_ready;
}

///
/// \brief Utilities::GetData
/// \param table - Pointer to the QTableWidget with the data.
/// \return - List of the data from the QTableWidget.
/// - For every row in the QTableWidget.
/// - Get stage type from QComboBox.
/// - Get temperature from QTableWidgetItem.
/// - Get rate/time from QTableWidgetItem.
/// - Append to list.
///
QStringList Utilities::GetData( const QTableWidget * table )
{
    QStringList cure_cycle_data;
    QString cycle_stage;
    QString type;
    QString temperature;
    QString rate_time;

    const int row_count = table->rowCount();

    for( int i = 0; i < row_count; i++ )
    {
        switch( static_cast<QComboBox *>( table->cellWidget( i, 0 ) )->currentIndex() )
        {
            case 0:
                type = "R";
                break;
            case 1:
                type = "H";
                break;
            case 2:
                type = "D";
                break;
        }

        temperature = table->item( i, 1 )->text();
        rate_time = table->item( i, 2 )->text();

        cycle_stage.clear();
        cycle_stage.append( type );
        cycle_stage.append( ":" );
        cycle_stage.append( temperature );
        cycle_stage.append( ":" );
        cycle_stage.append( rate_time );
        cycle_stage.append( "\n" );
        cure_cycle_data.append( cycle_stage );
    }
    return cure_cycle_data;
}

///
/// \brief Utilities::CheckNumber
/// \param number - Number to verify.
/// \return - True if valid false otherwise.
///
bool Utilities::CheckNumber( const QString &number )
{
    QRegExp re("\\d*");
    return re.exactMatch( number );

}

///
/// \brief Utilities::CheckName
/// \param name - Name to verify.
/// \return - True if valid false otherwise.
/// Valid if less than 20 characters and not empty.
///
bool Utilities::CheckName( const QString &name )
{
    bool status = true;
    const int max_length = 20;
    QString name_simplified = name.simplified();
    if( name_simplified.isEmpty() )
    {
        status = false;
    }
    if( name_simplified.length() > max_length )
    {
        status = false;
    }

    return status;
}
