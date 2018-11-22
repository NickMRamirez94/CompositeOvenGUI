#include "utilities.h"

//Qt Library Files
#include <QTextStream>
#include <QDir>
#include <QFileDialog>
#include <QComboBox>
#include <QtSerialPort/QSerialPort>

Utilities::Utilities()
{

}

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

bool Utilities::SendData( const QString &name, const QStringList &data )
{
    QSerialPort serial;
    serial.setPortName( "com4" );
    serial.setBaudRate( QSerialPort::Baud115200 );
    serial.setDataBits( QSerialPort::Data8 );
    serial.setParity( QSerialPort::NoParity );
    serial.setStopBits( QSerialPort::OneStop );
    serial.setFlowControl( QSerialPort::NoFlowControl );
    serial.open( QIODevice::WriteOnly );

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

    //Send data
    const int stage_count = data.count();
    const QByteArray hold_filler( 2, 0 );
    for( int i = 0; i < stage_count; i++ )
    {
        QStringList stage_text = data[i].split( ":" );
        QString stage_type = stage_text[0];
        QString temperature = stage_text[1];
        QString rate_time = stage_text[2];

        QByteArray type_byte = stage_type.toLocal8Bit();
        char * type_serial = type_byte.data();

        uint16_t temperature_int = temperature.toUShort();
        uint16_t rate_time_int = rate_time.toUShort();
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
}

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

bool Utilities::CheckNumber( const QString &number )
{
    QRegExp re("\\d*");
    return re.exactMatch( number );

}

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
