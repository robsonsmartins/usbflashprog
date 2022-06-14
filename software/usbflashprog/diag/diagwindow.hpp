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
 * @file diag/diagwindow.hpp
 * @brief Header of the Diagnostics Window Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef DIAG_DIAGWINDOW_HPP_
#define DIAG_DIAGWINDOW_HPP_

#include <QMainWindow>
#include <QTimer>
#include "backend/runner.hpp"

// ---------------------------------------------------------------------------

QT_BEGIN_NAMESPACE
namespace Ui { class DiagWindow; }
QT_END_NAMESPACE

// ---------------------------------------------------------------------------

class DiagWindow : public QMainWindow {
    Q_OBJECT

 public:
    explicit DiagWindow(QWidget *parent = nullptr);
    ~DiagWindow();

 private slots:
    void on_pushButtonConnect_clicked();
    void on_actionExit_triggered(bool checked = false);
    void on_actionAbout_triggered(bool checked = false);
    void on_actionAboutQt_triggered(bool checked = false);
    void on_checkBoxVddCtrl_toggled(bool checked = false);
    void on_checkBoxVppCtrl_toggled(bool checked = false);
    void on_checkBoxVddOnVpp_toggled(bool checked = false);
    void on_checkBoxVppOnA9_toggled(bool checked = false);
    void on_checkBoxVppOnA18_toggled(bool checked = false);
    void on_checkBoxVppOnCE_toggled(bool checked = false);
    void on_checkBoxVppOnOE_toggled(bool checked = false);
    void on_checkBoxVppOnWE_toggled(bool checked = false);
    void on_dialVdd_valueChanged(int value);
    void on_dialVpp_valueChanged(int value);
    void onEnumTimerTimeout();
    void onRefreshTimerTimeout();

 private:
    Ui::DiagWindow *ui_;
    QTimer enumTimer_;
    QTimer refreshTimer_;
    Runner runner_;
    void refreshPortComboBox_();
    void enableControls_(bool state = true);
};

#endif  // DIAG_DIAGWINDOW_HPP_
