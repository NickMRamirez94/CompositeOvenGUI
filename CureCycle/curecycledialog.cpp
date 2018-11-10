#include "curecycledialog.h"
#include "ui_curecycledialog.h"
#include <QMessageBox>

CureCycleDialog::CureCycleDialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::CureCycleDialog )
{
    ui->setupUi( this );
}

CureCycleDialog::~CureCycleDialog()
{
    delete ui;
}

void CureCycleDialog::on_doneButton_clicked()
{
    QString cycle_name = ui->cyclenamelineEdit->text();
    //Remove whitespace
    cycle_name = cycle_name.simplified();
    QString temperature = ui->templineEdit->text();
    QString rate = ui->ratelineEdit->text();

    QRegExp re("\\d*");
    if( cycle_name.isEmpty() )
    {
        QMessageBox::warning( this, "Cycle Name Error", "Cycle Name is empty" );
    }
    else if( !re.exactMatch( temperature ) || !re.exactMatch( rate ) )
    {
        QMessageBox::warning( this, "Temperature or Rate Error", "Only digits allowed" );
    }
    else
    {
        QStringList data;
        data.append( cycle_name );
        data.append( temperature );
        data.append( rate );

        emit send_name_temp_rate( data );

        this->close();
    }
}

void CureCycleDialog::on_cancelButton_clicked()
{
    this->close();
}
