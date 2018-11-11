#include "graphdialog.h"
#include "ui_graphdialog.h"

///
/// \brief GraphDialog::GraphDialog
/// \param parent - Parent widget.
/// \param file_path - File path to the graph data.
///
GraphDialog::GraphDialog( QWidget *parent, const QString &file_path ) :
    QDialog( parent ),
    ui( new Ui::GraphDialog )
{
    ui->setupUi( this );
}


///
/// \brief GraphDialog::~GraphDialog
///
GraphDialog::~GraphDialog()
{
    delete ui;
}
