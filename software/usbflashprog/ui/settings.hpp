// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2024) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Software
 * @file ui/settings.hpp
 * @brief Header of the Settings Dialog Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef UI_SETTINGSDIALOG_HPP_
#define UI_SETTINGSDIALOG_HPP_

#include <QDialog>
#include "config.hpp"

// ---------------------------------------------------------------------------

// clang-format off
QT_BEGIN_NAMESPACE
namespace Ui { class SettingsDialog; }
QT_END_NAMESPACE
// clang-format on

// ---------------------------------------------------------------------------

/**
 * @ingroup Software
 * @brief Settings Dialog GUI Class
 * @details The Settings Dialog.
 * @nosubgrouping
 */
class SettingsDialog : public QDialog {
    Q_OBJECT

  public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     * @param enableBtnCal If true, enables VDD/VPP calibration buttons.
     *   False otherwise.
     */
    explicit SettingsDialog(QWidget *parent = nullptr,
                            bool enableBtnCal = true);
    /** @brief Destructor. */
    ~SettingsDialog();

  Q_SIGNALS:
    /** @brief Triggered when VDD Calibration button is clicked. */
    void onBtnVddCalClicked();
    /** @brief Triggered when VPP Calibration button is clicked. */
    void onBtnVppCalClicked();

  private slots:
    /* auto slots */
    void on_buttonBox_accepted();
    void on_comboBoxLogLevel_currentIndexChanged(int index);
    void on_pushButtonVddInitCal_clicked();
    void on_pushButtonVppInitCal_clicked();
    void on_comboBoxBufferSize_currentIndexChanged(int index);

  private:
    /* @brief Pointer to UI object. */
    Ui::SettingsDialog *ui_;
    /*
     * @brief Load the settings and update the controls.
     * @return Application Settings.
     */
    TApplicationSettings loadSettings_();
    /*
     * @brief Save the settings.
     * @return Application Settings.
     */
    TApplicationSettings saveSettings_();
};

#endif  // UI_SETTINGSDIALOG_HPP_
