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
 * @file ui/mainwindow.hpp
 * @brief Header of the Main Window Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef UI_MAINWINDOW_HPP_
#define UI_MAINWINDOW_HPP_

#include <QMainWindow>
#include <QTimer>
#include <QCloseEvent>
#include <QProgressDialog>

#include "ui/qhexeditor.hpp"
#include "backend/runner.hpp"
#include "backend/devices/device.hpp"
#include "config.hpp"

// ---------------------------------------------------------------------------

// clang-format off
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
// clang-format on

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
    /* main window */
    void on_tabWidget_currentChanged(int index);
    void on_actionBuffer_triggered(bool checked = false);
    void on_actionDiagnostics_triggered(bool checked = false);
    void on_actionExit_triggered(bool checked = false);
    void on_actionAbout_triggered(bool checked = false);
    void on_actionAboutQt_triggered(bool checked = false);
    void on_actionProjectHome_triggered(bool checked = false);
    void on_actionAuthorHome_triggered(bool checked = false);
    void on_actionSettings_triggered(bool checked = false);
    /* programmer */
    void on_comboBoxProgPort_currentIndexChanged(int index);
    void on_actionRead_triggered(bool checked = false);
    void on_actionDoProgram_triggered(bool checked = false);
    void on_actionProgramAndVerify_triggered(bool checked = false);
    void on_actionVerify_triggered(bool checked = false);
    void on_actionDoErase_triggered(bool checked = false);
    void on_actionEraseAndBlankCheck_triggered(bool checked = false);
    void on_actionBlankCheck_triggered(bool checked = false);
    void on_actionGetID_triggered(bool checked = false);
    void on_actionUnprotect_triggered(bool checked = false);
    void on_actionProtect_triggered(bool checked = false);
    void on_btnRead_clicked();
    void on_btnProgram_clicked();
    void on_btnVerify_clicked();
    void on_btnErase_clicked();
    void on_btnBlankCheck_clicked();
    void on_btnGetID_clicked();
    void on_btnUnprotect_clicked();
    void on_btnProtect_clicked();
    void on_spinBoxProgTWP_valueChanged(int value);
    void on_spinBoxProgTWC_valueChanged(int value);
    void on_spinBoxProgVDDrd_valueChanged(double value);
    void on_spinBoxProgVDDwr_valueChanged(double value);
    void on_spinBoxProgVPP_valueChanged(double value);
    void on_spinBoxProgVEE_valueChanged(double value);
    void on_checkBoxProgSkipFF_toggled(bool checked = false);
    void on_checkBoxProgFast_toggled(bool checked = false);
    void on_comboBoxProgSectorSize_currentIndexChanged(int index);
    void on_comboBoxProgSize_currentIndexChanged(int index);
    /* editor */
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
    void on_btnFind_clicked();
    void on_btnReplace_clicked();
    void on_btnFillFF_clicked();
    void on_btnFill00_clicked();
    void on_btnFillRandom_clicked();
    /* diagnostics */
    void on_pushButtonConnect_clicked();
    void on_pushButtonGetDataW_clicked();
    void on_pushButtonGetDataB_clicked();
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
    /* manual slots */
    void onEnumTimerTimeout();
    void onRefreshTimerTimeout();
    void onCheckBoxAddressToggled(bool checked = false);
    void onCheckBoxDataToggled(bool checked = false);
    void onDataChanged(bool status = true);
    void onBtnSelectDeviceClicked(bool checked = false);
    void onSelectDeviceTriggered(bool checked = false);
    void onActionProgress(uint32_t current = 0, uint32_t total = 0,
                          bool done = false, bool success = true,
                          bool canceled = false);
    void onBtnVddCalClicked();
    void onBtnVppCalClicked();

  protected:
    /*
     * @brief OnClose event of the Window.
     * @param event Pointer to event object.
     */
    void closeEvent(QCloseEvent *event);

  private:
    /* @brief Application settings. */
    TApplicationSettings settings_;
    /* @brief Pointer to UI object. */
    Ui::MainWindow *ui_;
    /* @brief Pointer to QHexEditor widget. */
    QHexEditor *hexeditor_;
    /* @brief Pointer to checksum QLabel widget. */
    QLabel *checksumLabel_;
    /* @brief Pointer to QProgressDialog widget (Prog). */
    QProgressDialog *progress_;
    /* @brief Ports Enumerator Timer (Diag). */
    QTimer enumTimer_;
    /* @brief Refresh Timer (Diag). */
    QTimer refreshTimer_;
    /* @brief Runner object (Diag). */
    Runner runner_;
    /* @brief Device pointer (Prog). */
    Device *device_;
    /* @brief If true, no shows warning about action over device (Prog). */
    bool noWarningDevice_;
    /* @brief Connects signals of the widgets. */
    void connectSignals_();
    /* @brief Loads configuration settings. */
    void loadSettings_();
    /* @brief Saves configuration settings. */
    void saveSettings_();
    /* @brief Creates device (Prog). */
    void createDevice_();
    /*
     * @brief Creates device if it's a SRAM (Prog).
     * @param label The triggered action text.
     */
    void createDeviceIfSRAM_(const QString &label);
    /*
     * @brief Creates device if it's an EPROM (Prog).
     * @param label The triggered action text.
     */
    void createDeviceIfEPROM_(const QString &label);
    /*
     * @brief Creates device if it's an Erasable EPROM (Prog).
     * @param label The triggered action text.
     */
    void createDeviceIfErasableEPROM_(const QString &label);
    /*
     * @brief Creates device if it's an EEPROM (Prog).
     * @param label The triggered action text.
     */
    void createDeviceIfEEPROM_(const QString &label);
    /* @brief Enables/Disables the controls (Prog). */
    void configureProgControls_();
    /* @brief Configures the device based in the ui values (Prog). */
    void configureDeviceFromControls_();
    /*
     * @brief Shows the progress dialog (Prog).
     * @param msg Text to display.
     */
    void showDialogActionProgress_(const QString &msg);
    /*
     * @brief Shows the different size dialog (Prog).
     * @param data Data to compare with device (size).
     * @return True if it can continue, false otherwise.
     */
    bool showDifferentSizeDialog_(const QByteArray &data);
    /*
     * @brief Shows the warning about Device Action (Prog).
     * @return True if it can continue, false otherwise.
     */
    bool showActionWarningDialog_();
    /*
     * @brief Calculates the Data Rate (Prog).
     * @param elapsed Elapsed time, in milliseconds.
     * @param amount Amount of data, in bytes.
     * @return String describing the Data Rate, in bytes/sec or KB/s or MB/s.
     */
    QString calculateDataRate_(int64_t elapsed, uint32_t amount);
    /*
     * @brief Calculates the Remaining Time (Prog).
     * @param elapsed Elapsed time, in milliseconds.
     * @param current Current address.
     * @param total Total addresses.
     * @return String describing the Remaining Time, in hours
     *   or minutes or seconds.
     */
    QString calculateRemainingTime_(int64_t elapsed, uint32_t current,
                                    uint32_t total);
    /* @brief Refreshes the port comboboxes (Prog/Diag). */
    void refreshPortComboBox_();
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
    /* @brief Updates checksum info (Editor). */
    void updateCheckSum_();
    /*
     * @brief Connects to board via CDC port (Diag).
     * @param state True to connect, false to disconnect.
     */
    void connect_(bool state = true);
    /* @brief Sends value to data bus (Diag). */
    void sendDataBus_();
    /* @brief Sends value to address bus (Diag). */
    void sendAddrBus_();
    /*
     * @brief Enables/Disables the controls (Diag).
     * @param state True to enable, false otherwise.
     */
    void enableDiagControls_(bool state = true);
    /* @brief Converts Addr Bus Checkbox to Spinbox Value (Diag). */
    void addressBinToHex_();
    /* @brief Converts Addr Bus Spinbox Value to Checkbox (Diag). */
    void addressHexToBin_();
    /* @brief Converts Data Bus Checkbox to Spinbox Value (Diag). */
    void dataBinToHex_();
    /* @brief Converts Data Bus Spinbox Value to Checkbox (Diag). */
    void dataHexToBin_();
};

#endif  // UI_MAINWINDOW_HPP_
