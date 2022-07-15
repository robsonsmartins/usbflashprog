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
#include "backend/runner.hpp"

// ---------------------------------------------------------------------------

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// ---------------------------------------------------------------------------

class MainWindow : public QMainWindow {
    Q_OBJECT

 public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QScreen *screen() const;

 private slots:
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
    void onEnumTimerTimeout();
    void onRefreshTimerTimeout();
    void onRunnerResultReady(const TRunnerCommand& command);

 protected:
    void closeEvent(QCloseEvent *event);

 private:
    Ui::MainWindow *ui_;
    QTimer enumTimer_;
    QTimer refreshTimer_;
    Runner runner_;
    void connect_(bool state = true);
    void refreshPortComboBox_();
    void enableControls_(bool state = true);
};

#endif  // MAIN_MAINWINDOW_HPP_
