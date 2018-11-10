#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include <QDialog>

namespace Ui {
class GraphDialog;
}

class GraphDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GraphDialog( QWidget *parent = nullptr, const QString &file_path = "");
    ~GraphDialog();

private:
    Ui::GraphDialog *ui;
};

#endif // GRAPHDIALOG_H
