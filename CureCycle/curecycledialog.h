#ifndef CURECYCLEDIALOG_H
#define CURECYCLEDIALOG_H

#include <QDialog>
#include "curecycle.h"

namespace Ui {
class CureCycleDialog;
}

class CureCycleDialog : public QDialog
{
    Q_OBJECT

public:
    ///
    /// \brief CureCycleDialog::CureCycleDialog
    /// \param parent
    /// Constructor.
    ///
    explicit CureCycleDialog( QWidget *parent = nullptr );

    ///
    /// \brief CureCycleDialog::~CureCycleDialog
    /// Destructor.
    ///
    ~CureCycleDialog();

private slots:
    ///
    /// \brief CureCycleDialog::on_doneButton_clicked
    /// Slot that performs action when done button is clicked.
    void on_doneButton_clicked();

    ///
    /// \brief CureCycleDialog::on_cancelButton_clicked
    /// Slot that performs action when cancel button is clicked.
    ///
    void on_cancelButton_clicked();

private:
    ///
    /// \brief ui
    /// Instance of the CureCycleDialog
    ///
    Ui::CureCycleDialog *ui;

    ///
    /// \brief cure_cycle_
    /// CureCycle object.
    CureCycle cure_cycle_;
signals:
    ///
    /// \brief send_name_temp_rate
    /// \param data - QStringList holding name, temperature, and rate.
    /// Signal that sends QStringList to receiving slot.
    ///
    void send_name_temp_rate( QStringList data );
};

#endif // CURECYCLEDIALOG_H
