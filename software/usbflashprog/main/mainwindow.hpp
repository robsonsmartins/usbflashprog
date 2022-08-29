// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @ingroup Software
 * @file main/mainwindow.hpp
 * @brief Header of the Main Window Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef MAIN_MAINWINDOW_HPP_
#define MAIN_MAINWINDOW_HPP_

#include <QMainWindow>
#include <QTimer>
#include <QCloseEvent>

#include "ui/qhexeditor.hpp"
#include "backend/runner.hpp"

// ---------------------------------------------------------------------------

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Main Window GUI Class
 * @details The Main Window of Application.
 * @nosubgrouping
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

 public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit MainWindow(QWidget *parent = nullptr);
    /** @brief Destructor. */
    ~MainWindow();
    /**
     * @brief Gets a pointer to the primary screen.
     * @return Pointer to the primary screen object.
     */
    QScreen *screen() const;

 private slots:
    /* auto slots */
    void on_tabWidget_currentChanged(int index);
    void on_actionBuffer_triggered(bool checked = false);
    void on_actionDiagnostics_triggered(bool checked = false);
    void on_pushButtonConnect_clicked();
    void on_pushButtonVddInitCal_clicked();
    void on_pushButtonVppInitCal_clicked();
    void on_actionExit_triggered(bool checked = false);
    void on_actionAbout_triggered(bool checked = false);
    void on_actionAboutQt_triggered(bool checked = false);
    void on_actionProjectHome_triggered(bool checked = false);
    void on_actionAuthorHome_triggered(bool checked = false);
    void on_checkBoxVddCtrl_toggled(bool checked = false);
    void on_checkBoxVppCtrl_toggled(bool checked = false);
    void on_checkBoxVddOnVpp_toggled(bool checked = false);
    void on_checkBoxVppOnA9_toggled(bool checked = false);
    void on_checkBoxVppOnA18_toggled(bool checked = false);
    void on_checkBoxVppOnCE_toggled(bool checked = false);
    void on_checkBoxVppOnOE_toggled(bool checked = false);
    void on_checkBoxVppOnWE_toggled(bool checked = false);
    void on_checkBoxCE_toggled(bool checked = false);
    void on_checkBoxOE_toggled(bool checked = false);
    void on_checkBoxWE_toggled(bool checked = false);
    void on_dialVdd_valueChanged(int value);
    void on_dialVpp_valueChanged(int value);
    void on_spinBoxAddr_valueChanged(int value);
    void on_spinBoxData_valueChanged(int value);
    void on_actionOpen_triggered(bool checked = false);
    void on_actionSave_triggered(bool checked = false);
    void on_actionSaveAs_triggered(bool checked = false);
    void on_actionFillFF_triggered(bool checked = false);
    void on_actionFill00_triggered(bool checked = false);
    void on_actionFillRandom_triggered(bool checked = false);
    void on_actionFind_triggered(bool checked = false);
    void on_actionReplace_triggered(bool checked = false);
    void on_btnOpen_clicked();
    void on_btnSave_clicked();
    /* manual slots */
    void onEnumTimerTimeout();
    void onRefreshTimerTimeout();
    void onRunnerResultReady(const TRunnerCommand& command);
    void onCheckBoxAddressToggled(bool checked = false);
    void onCheckBoxDataToggled(bool checked = false);
    void onDataChanged(bool status = true);

 protected:
    /*
     * @brief OnClose event of the Window.
     * @param event Pointer to event object.
     */
    void closeEvent(QCloseEvent *event);

 private:
    /* @brief Pointer to UI object. */
    Ui::MainWindow *ui_;
    /* @brief Pointer to QHexEditor widget. */
    QHexEditor *hexeditor_;
    /* @brief Ports Enumerator Timer (Diag). */
    QTimer enumTimer_;
    /* @brief Refresh Timer (Diag). */
    QTimer refreshTimer_;
    /* @brief Runner object. */
    Runner runner_;
    /* @brief Connects signals of the widgets. */
    void connectSignals_();
    /*
     * @brief Connects to board via CDC port (Diag).
     * @param state True to connect, false to disconnect.
     */
    void connect_(bool state = true);
    /* @brief Refreshes the port combobox (Diag). */
    void refreshPortComboBox_();
    /*
     * @brief Enables/Disables the controls (Diag).
     * @param state True to enable, false otherwise.
     */
    void enableDiagControls_(bool state = true);
    /*
     * @brief Enables/Disables the controls (Editor).
     * @param state True to enable, false otherwise.
     */
    void enableEditorControls_(bool state = true);
    /*
     * @brief Shows the file changed dialog to confirm data loses (Editor).
     * @return True if data will be lose, false otherwise.
     */
    bool showDialogFileChanged_();
    /* @brief Gets file filter of the open dialog (Editor). */
    QString getOpenDialogFilter_();
    /* @brief Gets file filter of the save dialog (Editor). */
    QString getSaveDialogFilter_();
    /* @brief Converts Addr Bus Checkbox to Spinbox Value (Diag). */
    void addressBinToHex_();
    /* @brief Converts Addr Bus Spinbox Value to Checkbox (Diag). */
    void addressHexToBin_();
    /* @brief Converts Data Bus Checkbox to Spinbox Value (Diag). */
    void dataBinToHex_();
    /* @brief Converts Data Bus Spinbox Value to Checkbox (Diag). */
    void dataHexToBin_();
};

#endif  // MAIN_MAINWINDOW_HPP_
