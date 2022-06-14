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
 * @file diag/diagwindow.cpp
 * @brief Implementation of the Diagnostics Window Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QMessageBox>

#include <cstdio>
#include <cstring>
#include <string>
#include <locale>

#include "diag/diagwindow.hpp"
#include "config.hpp"
#include "./ui_diagwindow.h"
#include "backend/opcodes.hpp"

// ---------------------------------------------------------------------------

DiagWindow::DiagWindow(QWidget *parent): QMainWindow(parent),
        ui_(new Ui::DiagWindow), enumTimer_(this), refreshTimer_(this) {
    ui_->setupUi(this);
    refreshPortComboBox_();
    ui_->frameVdd->setEnabled(false);
    ui_->frameVpp->setEnabled(false);
    connect(&enumTimer_, &QTimer::timeout,
            this, &DiagWindow::onEnumTimerTimeout);
    connect(&refreshTimer_, &QTimer::timeout,
            this, &DiagWindow::onRefreshTimerTimeout);
    enumTimer_.start(kUsbEnumerateInterval);
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    this->setFixedSize(this->size());
    enableControls_(false);
}

DiagWindow::~DiagWindow() {
    refreshTimer_.stop();
    enumTimer_.stop();
    runner_.close();
    delete ui_;
}

void DiagWindow::on_pushButtonConnect_clicked() {
    if (!runner_.isOpen()) {
        if (runner_.open(ui_->comboBoxPort->currentText())) {
            enableControls_();
            ui_->dialVdd->setValue(50);
            ui_->dialVpp->setValue(120);
            enumTimer_.stop();
            refreshTimer_.start(kUsbRefreshInterval);
        } else {
            QMessageBox::critical(this,
                tr("USB Flash/EPROM Programmer"),
                tr("Error opening the \"%1\" port.")
                    .arg(ui_->comboBoxPort->currentText()));
        }
    } else {
        runner_.send(kCmdVddCtrl, false);
        runner_.send(kCmdVppCtrl, false);
        runner_.send(kCmdVddOnVpp, false);
        runner_.send(kCmdVppOnA9, false);
        runner_.send(kCmdVppOnA18, false);
        runner_.send(kCmdVppOnCE, false);
        runner_.send(kCmdVppOnOE, false);
        runner_.send(kCmdVppOnWE, false);
        refreshTimer_.stop();
        runner_.close();
        enableControls_(false);
        enumTimer_.start();
    }
}

void DiagWindow::on_actionExit_triggered(bool checked) {
    close();
}

void DiagWindow::on_actionAbout_triggered(bool checked) {
    QMessageBox::about(this,
        tr("USB Flash/EPROM Programmer"),
        tr("A memory device programmer (Flash/EPROM/E2PROM) board and software,"
           "connected to PC by USB port."));
}

void DiagWindow::on_actionAboutQt_triggered(bool checked) {
    QMessageBox::aboutQt(this);
}

void DiagWindow::on_checkBoxVddCtrl_toggled(bool checked) {
    if (!runner_.isOpen()) { return; }
    runner_.send(kCmdVddCtrl, checked);
}

void DiagWindow::on_checkBoxVppCtrl_toggled(bool checked) {
    if (!runner_.isOpen()) { return; }
    runner_.send(kCmdVppCtrl, checked);
}

void DiagWindow::on_checkBoxVddOnVpp_toggled(bool checked) {
    if (!runner_.isOpen()) { return; }
    runner_.send(kCmdVddOnVpp, checked);
}

void DiagWindow::on_checkBoxVppOnA9_toggled(bool checked) {
    if (!runner_.isOpen()) { return; }
    runner_.send(kCmdVppOnA9, checked);
}

void DiagWindow::on_checkBoxVppOnA18_toggled(bool checked) {
    if (!runner_.isOpen()) { return; }
    runner_.send(kCmdVppOnA18, checked);
}

void DiagWindow::on_checkBoxVppOnCE_toggled(bool checked) {
    if (!runner_.isOpen()) { return; }
    runner_.send(kCmdVppOnCE, checked);
}

void DiagWindow::on_checkBoxVppOnOE_toggled(bool checked) {
    if (!runner_.isOpen()) { return; }
    runner_.send(kCmdVppOnOE, checked);
}

void DiagWindow::on_checkBoxVppOnWE_toggled(bool checked) {
    if (!runner_.isOpen()) { return; }
    runner_.send(kCmdVppOnWE, checked);
}

void DiagWindow::on_dialVdd_valueChanged(int value) {
    if (!runner_.isOpen()) { return; }
    float v = value / 10.0f;
    runner_.send(kCmdVddSetV, v);
}

void DiagWindow::on_dialVpp_valueChanged(int value) {
    if (!runner_.isOpen()) { return; }
    float v = value / 10.0f;
    runner_.send(kCmdVppSetV, v);
}

void DiagWindow::onEnumTimerTimeout() {
    refreshPortComboBox_();
}

void DiagWindow::onRefreshTimerTimeout() {
    if (!runner_.isOpen()) { return; }
    auto originalLocale = std::locale::global(std::locale::classic());
    float value;
    char buffer[5];
    // Get VDD
    value = 0.0f;
    if (runner_.send(kCmdVddGetV, &value) == kCmdResponseOk) {
        snprintf(buffer, sizeof(buffer), "%4.1f", value);
        ui_->lcdNumberVdd->display(QString(buffer));
    }
    // Get VPP
    value = 0.0f;
    if (runner_.send(kCmdVppGetV, &value) == kCmdResponseOk) {
        snprintf(buffer, sizeof(buffer), "%4.1f", value);
        ui_->lcdNumberVpp->display(QString(buffer));
    }
    // Get VDD Duty
    value = 0.0f;
    if (runner_.send(kCmdVddGetDuty, &value) == kCmdResponseOk) {
        snprintf(buffer, sizeof(buffer), "%4.1f", value);
        ui_->lcdNumberVddDuty->display(QString(buffer));
    }
    // Get VPP Duty
    value = 0.0f;
    if (runner_.send(kCmdVppGetDuty, &value) == kCmdResponseOk) {
        snprintf(buffer, sizeof(buffer), "%4.1f", value);
        ui_->lcdNumberVppDuty->display(QString(buffer));
    }
    std::locale::global(originalLocale);
}

void DiagWindow::refreshPortComboBox_() {
    Serial::TSerialPortList list = runner_.list();
    QStringList paths;
    for (const auto item : list) {
        paths.push_back(item.portName());
    }
    if (paths.length() != ui_->comboBoxPort->count()) {
        ui_->comboBoxPort->clear();
    }
    bool found = false;
    for (int i = 0; i < ui_->comboBoxPort->count(); i++) {
        found = false;
        for (const auto path : paths) {
            if (ui_->comboBoxPort->itemText(i) == path) {
                found = true;
                break;
            }
        }
        if (!found) { break; }
    }
    if (!found) {
        ui_->comboBoxPort->clear();
        for (const auto path : paths) {
            ui_->comboBoxPort->addItem(path);
        }
    }
    ui_->pushButtonConnect->setEnabled(!paths.empty());
}

void DiagWindow::enableControls_(bool state) {
    ui_->frameVdd->setEnabled(state);
    ui_->frameVpp->setEnabled(state);
    ui_->comboBoxPort->setEnabled(!state);
    if (state) {
        ui_->pushButtonConnect->setText(tr("&Disconnect"));
        ui_->lcdNumberVdd->setStyleSheet(
            "background-color:rgb(0,0,0);color:rgb(143,240,164);");
        ui_->lcdNumberVddDuty->setStyleSheet(
            "background-color:rgb(0,0,0);color:rgb(143,240,164);");
        ui_->lcdNumberVpp->setStyleSheet(
            "background-color:rgb(0,0,0);color:rgb(249,240,107);");
        ui_->lcdNumberVppDuty->setStyleSheet(
            "background-color:rgb(0,0,0);color:rgb(249,240,107);");
    } else {
        ui_->pushButtonConnect->setText(tr("&Connect"));
        ui_->checkBoxVddCtrl->setChecked(false);
        ui_->checkBoxVppCtrl->setChecked(false);
        ui_->checkBoxVddOnVpp->setChecked(false);
        ui_->checkBoxVppOnA9->setChecked(false);
        ui_->checkBoxVppOnA18->setChecked(false);
        ui_->checkBoxVppOnCE->setChecked(false);
        ui_->checkBoxVppOnOE->setChecked(false);
        ui_->checkBoxVppOnWE->setChecked(false);
        ui_->lcdNumberVdd->setStyleSheet(
            "background-color:rgb(76,76,76);color:rgb(89,150,101);");
        ui_->lcdNumberVddDuty->setStyleSheet(
            "background-color:rgb(76,76,76);color:rgb(89,150,101);");
        ui_->lcdNumberVpp->setStyleSheet(
            "background-color:rgb(76,76,76);color:rgb(166,166,82);");
        ui_->lcdNumberVppDuty->setStyleSheet(
            "background-color:rgb(76,76,76);color:rgb(166,166,82);");
        ui_->lcdNumberVdd->display(QString("0.0"));
        ui_->lcdNumberVpp->display(QString("0.0"));
        ui_->lcdNumberVddDuty->display(QString("0.0"));
        ui_->lcdNumberVppDuty->display(QString("0.0"));
    }
}
