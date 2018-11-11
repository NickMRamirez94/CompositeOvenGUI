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
    ///
    /// \brief GraphDialog::GraphDialog
    /// \param parent
    /// \param file_path
    /// Constructor
    ///
    explicit GraphDialog( QWidget *parent = nullptr, const QString &file_path = "");

    ///
    /// \brief GraphDialog::~GraphDialog
    /// Destructor
    ///
    ~GraphDialog();

private:
    ///
    /// \brief ui
    /// Instnace of the GraphDialog
    ///
    Ui::GraphDialog *ui;
};

#endif // GRAPHDIALOG_H
