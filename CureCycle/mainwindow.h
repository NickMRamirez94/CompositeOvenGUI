#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include "curecycledialog.h"
#include "graphdialog.h"

///
/// \brief MainWindow class.
///
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow();

private slots:
    ///
    /// \brief MainWindow::on_actionExit_triggered
    /// Slot that exits application.
    ///
    void on_actionExit_triggered();

    ///
    /// \brief MainWindow::on_actionSave_Cure_Cycle_triggered
    /// Slot that saves the cure cycle that is on the current tab
    /// to a file.
    ///
    void on_actionSave_Cure_Cycle_triggered();

    ///
    /// \brief MainWindow::on_actionLoad_Cure_Cycle_triggered
    /// Slot that loads a cure cycle from a file and places it
    /// as the active tab.
    ///
    void on_actionLoad_Cure_Cycle_triggered();

    ///
    /// \brief MainWindow::on_actionCreate_New_Cure_Cycle_triggered
    /// Slot opens a cure cycle creation dialog for the user to
    /// begin creating a new cure cycle.
    ///
    void on_actionCreate_New_Cure_Cycle_triggered();

    ///
    /// \brief MainWindow::on_actionGraph_triggered
    /// Slot that allows the user to view a temperature graph
    /// and vacuum graph of the data collected during a cure cycle.
    /// The user will use this to view the health of the temperature
    /// and vacuum.
    ///
    void on_actionGraph_triggered();

    ///
    /// \brief add_button_pressed
    ///
    void add_button_pressed();

    ///
    /// \brief remove_button_pressed
    ///
    void remove_button_pressed();

    ///
    /// \brief create_new_cycle
    /// \param data
    ///
    void create_new_cycle( QStringList data );

    ///
    /// \brief closeTab
    /// \param tab
    ///
    void closeTab( int tab );

    ///
    /// \brief updateCells
    /// \param row
    /// \param column
    ///
    void updateCells( int row, int column );

    ///
    /// \brief updateCombo
    /// \param index
    ///
    void updateCombo( int index );

private:
    ///
    /// \brief ui
    /// Instance of the MainWindow.
    Ui::MainWindow *ui;

    QTabWidget * cureCycleTabWidget;

    ///
    /// \brief cure_cycle_dialog_
    /// Instance of the Cure Cycle Dialog.
    CureCycleDialog *cure_cycle_dialog_;

    ///
    /// \brief graph_dialog_
    /// Instance of the Graph Dialog.
    GraphDialog *graph_dialog_;

    ///
    /// \brief readFile
    /// \param file_name the name of the cure cycle file.
    /// \return boolean true if successful false otherwise.
    /// Takes a cure cycle file and populates a new tab with
    /// the data from the file.
    ///
    bool ReadFile( const QString  &file_path );

    ///
    /// \brief LoadData
    /// \param cure_cycle_data
    ///
    void LoadData ( const QString &cure_cycle_data );

    ///
    /// \brief InitializeTable
    /// \param cycle_name
    /// \param temperature
    /// \param rate
    /// \return
    ///
    QTableWidget * InitializeTable( const QString &cycle_name, const QString &temperature, const QString &rate );
};

#endif // MAINWINDOW_H
