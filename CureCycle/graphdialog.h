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
    void ReadSerial();

    void DoneReading();


private:    
    void CreateGraph();

    inline void CreateReceivingDataMessageBox();

    inline void CloseReceivingDataMessageBox();

    ///
    /// \brief ui
    /// Instnace of the GraphDialog
    ///
    Ui::GraphDialog *ui;

    QByteArray * temperature_data_;

    QString * serial_port_name_;

    QSerialPort * serial_;

    QTimer * timer_;

    bool ReadTemperatureData( const QString & file_path );

    QMessageBox * receiving_data_message_box_;

};

#endif // GRAPHDIALOG_H
