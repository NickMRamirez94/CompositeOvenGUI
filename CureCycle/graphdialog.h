#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class GraphDialog;
}

class GraphDialog : public QDialog
{
    Q_OBJECT

public:
    ///
    /// \brief GraphDialog::GraphDialog
    /// \param parent
    /// \param file_path
    /// Constructor
    ///
    explicit GraphDialog( QWidget *parent = nullptr, const QString & file_path = "" );

    ///
    /// \brief GraphDialog::~GraphDialog
    /// Destructor
    ///
    ~GraphDialog();

private slots:
    ///
    /// \brief ReadSerial
    /// Slot called when data is sent from controller to application.
    /// Reads data and puts in QByteArray
    ///
    void ReadSerial();

    ///
    /// \brief DoneReading
    /// Slot called when QTimer object signals 2 seconds has passed without data.
    /// Creates graph on dialog
    ///
    void DoneReading();


private:

    ///
    /// \brief CreateGraph
    /// Creates the graph using temperature_data_
    ///
    void CreateGraph();

    ///
    /// \brief CreateReceivingDataMessageBox
    /// Lets user know when data has first been received through QMessageBox
    ///
    inline void CreateReceivingDataMessageBox();

    ///
    /// \brief CloseReceivingDataMessageBox
    /// Closes QMessageBox when data is done being recieved
    ///
    inline void CloseReceivingDataMessageBox();

    ///
    /// \brief ui
    /// Instnace of the GraphDialog
    ///
    Ui::GraphDialog *ui;

    ///
    /// \brief temperature_data_
    /// Holds the temperature data recevied from controller
    ///
    QByteArray * temperature_data_;

    ///
    /// \brief serial_port_name_
    /// The serial port name to receive data from
    ///
    QString * serial_port_name_;

    ///
    /// \brief serial_
    /// Serial port object that data is read from
    ///
    QSerialPort * serial_;

    ///
    /// \brief timer_
    /// Timer object used to know when data is done being sent from controller
    ///
    QTimer * timer_;

    ///
    /// \brief ReadTemperatureData
    /// \param file_path the path to the data file
    /// \return true is successful. False otherwise
    /// Reads the temperature data from the specified file and populates temperature_data_
    ///
    bool ReadTemperatureData( const QString & file_path );

    ///
    /// \brief receiving_data_message_box_
    /// QMessageBox used to let user know that data has started to be received
    ///
    QMessageBox * receiving_data_message_box_;

};

#endif // GRAPHDIALOG_H
