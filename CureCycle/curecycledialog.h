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
    explicit CureCycleDialog( QWidget *parent = nullptr );
    ~CureCycleDialog();

private slots:
    void on_doneButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::CureCycleDialog *ui;
    CureCycle cure_cycle_;
signals:
    void send_name_temp_rate( QStringList data );
};

#endif // CURECYCLEDIALOG_H
