#include "graphdialog.h"
#include "ui_graphdialog.h"

GraphDialog::GraphDialog( QWidget *parent, const QString &file_path ) :
    QDialog( parent ),
    ui( new Ui::GraphDialog )
{
    ui->setupUi( this );
}

GraphDialog::~GraphDialog()
{
    delete ui;
}
