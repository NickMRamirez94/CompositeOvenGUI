#include "curecycledialog.h"
#include "ui_curecycledialog.h"
#include "utilities.h"

//Qt Library Files
#include <QMessageBox>

///
/// \brief CureCycleDialog::CureCycleDialog
/// \param parent - Parent widget.
///
CureCycleDialog::CureCycleDialog( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::CureCycleDialog )
{
    ui->setupUi( this );
}

///
/// \brief CureCycleDialog::~CureCycleDialog
///
CureCycleDialog::~CureCycleDialog()
{
    delete ui;
}

///
/// \brief CureCycleDialog::on_doneButton_clicked
/// - Retrieve cycle name, temperature, and rate from LineEdit objects
/// - Check to make sure cycle name is not empty
/// - Check to make sure temperature and rate are numbers
/// - Append data to QStringList
/// - Emit send_name_temp_rate signal with QStringList data
///
void CureCycleDialog::on_doneButton_clicked()
{
    QString cycle_name = ui->cyclenamelineEdit->text();
    QString temperature = ui->templineEdit->text();
    QString rate = ui->ratelineEdit->text();
    cycle_name = cycle_name.simplified();

    if( !Utilities::CheckName( cycle_name ) )
    {
        QMessageBox::warning( this, "Cycle Name Error", "Requirements: Non-empty | Max limit 20 characters" );
    }
    else if( !Utilities::CheckNumber( temperature) || !Utilities::CheckNumber( rate ) )
    {
        QMessageBox::warning( this, "Temperature or Rate Error", "Must be positive number" );
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

///
/// \brief CureCycleDialog::on_cancelButton_clicked
/// Close the CureCycleDialog.
///
void CureCycleDialog::on_cancelButton_clicked()
{
    this->close();
}
