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
#include "backend/serial.hpp"

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
    void on_actionAboutQt_triggered(bool checked = false);
    void onTimerTimeout();

 private:
    Ui::DiagWindow *ui_;
    QTimer timer_;
    Serial serial_;
    void refreshPortComboBox_();
};

#endif  // DIAG_DIAGWINDOW_HPP_
