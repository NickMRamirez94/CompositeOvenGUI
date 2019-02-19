#ifndef UTILITIES_H
#define UTILITIES_H

#include <QObject>
#include <QTableWidget>

class Utilities
{
public:
    Utilities();

    ///
    /// \brief SaveData
    /// \param data - Data to save.
    /// \param name - Name of cure cycle.
    /// \param file_path - Path of save file.
    /// \return - True if successful false otherwise.
    /// Save the specified data to the specified file.
    ///
    static bool SaveCureCycleData( const QStringList &data, const QString &name, const QString &file_path );

    static bool SaveTemperatureData( const QByteArray &temperature_data );

    ///
    /// \brief ReadFile
    /// \param file_path
    /// \return
    ///
    static bool ReadFile( const QString &file_path );

    ///
    /// \brief LoadData
    /// \param data
    ///
    static void LoadData( const QStringList &data );

    ///
    /// \brief SendData
    /// \param data - Data to send.
    /// \return - True if successful false otherwise.
    /// Sends data to the USB to UART module.
    ///
    static bool SendDataToController( const QString &name, const QStringList &data );

    ///
    /// \brief GetData
    /// \param table - Pointer to the QTableWidget data is in.
    /// \return - List of the data from QTableWidget.
    /// Retrieves the data from the QTableWidget.
    ///
    static QStringList GetDataFromTable( const QTableWidget * table );

    ///
    /// \brief CheckNumber
    /// \param number - Number to validate.
    /// \return - True if valid false otherwise.
    /// Checks if the specified number is within range.
    ///
    static bool CheckNumber( const QString &number );

    ///
    /// \brief CheckName
    /// \param name - Name to validate.
    /// \return - True if valid false otherwise.
    /// Checks if the specified name is non empty and less than 20 characters.
    ///
    static bool CheckName( const QString &name );

    static QString GetPortName();

    static quint16 PrepNumber( quint8 msb, quint8 lsb );

private:
    ///
    /// \brief usbtouart_product_identifier
    /// Product identifier for USB to UART module.
    ///
    static const quint16 usbtouart_product_identifier = 60000;

    ///
    /// \brief usbtouart_vendor_identifier
    /// Vendor identifier for USB to UART module.
    ///
    static const quint16 usbtouart_vendor_identifier = 4292;

};

#endif // UTILITIES_H
