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
    ///
    /// \brief MainWindow::MainWindow
    /// Constructor.
    ///
    explicit MainWindow( QWidget *parent = nullptr );

    ///
    /// \brief MainWindow::~MainWindow
    /// Destructor
    ///
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
    /// \brief MainWindow::add_button_pressed
    /// Slot that adds row to cure cycle table.
    ///
    void add_button_pressed();

    ///
    /// \brief MainWindow::remove_button_pressed
    /// Slot that removes row from cure cycle table.
    ///
    void remove_button_pressed();

    ///
    /// \brief MainWindow::create_new_cycle
    /// \param data - List of cycle name, temperature, and rate
    /// Parses data. Creates new cure cycle table to be displayed on 
    /// new tab of TabWidget. Connects cellChanged signal to updateCells slot.
    ///
    void create_new_cycle( QStringList data );

    ///
    /// \brief MainWindow::closeTab
    /// \param tab - Number of the tab to close of TabWidget.
    /// Will close the specified tab of TabWidget. 
    ///
    void closeTab( int tab );

    ///
    /// \brief MainWindow::updateCells
    /// \param row - Row of cell to update.
    /// \param column - Column of cell to update.
    /// Updates information based on the updated cell.
    ///
    void updateCells( int row, int column );

    ///
    /// \brief MainWindow::updateCombo
    /// \param index - Index number of current combobox selection.
    /// Updates information based on the updated combobox.
    ///
    void updateCombo( int index );

private:
    ///
    /// \brief ui
    /// Instance of the MainWindow.
    ///
    Ui::MainWindow *ui;

    ///
    /// \brief cureCycleTabWidget
    /// Instance of QTabWidget for MainWindow
    ///
    QTabWidget * cureCycleTabWidget;

    ///
    /// \brief cure_cycle_dialog_
    /// Instance of the Cure Cycle Dialog.
    ///
    CureCycleDialog *cure_cycle_dialog_;

    ///
    /// \brief graph_dialog_
    /// Instance of the Graph Dialog.
    ///
    GraphDialog *graph_dialog_;

    ///
    /// \brief MainWindow::readFile
    /// \param file_path - The path of the cure cycle file.
    /// \return boolean true if successful false otherwise.
    /// Takes a cure cycle file and populates a new tab with
    /// the data from the file.
    ///
    bool ReadFile( const QString  &file_path );

    ///
    /// \brief MainWindow::LoadData
    /// \param cure_cycle_data - QString holding all cure cycle data
    /// Loads the data from cure_cycle_data into the QTableWidget
    ///
    void LoadData ( const QString &cure_cycle_data );

    ///
    /// \brief MainWindow::InitializeTable
    /// \param cycle_name - Cycle name.
    /// \param temperature - Initial temperature.
    /// \param rate - INitial rate.
    /// \return Pointer to the QTableWidget created.
    /// Creates and return pointer to QTableWidget with initial specified data.
    ///
    QTableWidget * InitializeTable( const QString &cycle_name, const QString &temperature, const QString &rate );
};

#endif // MAINWINDOW_H
