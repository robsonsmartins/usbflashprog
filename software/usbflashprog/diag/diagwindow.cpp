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
#include <cstring>
#include <string>

#include "diag/diagwindow.hpp"
#include "config.hpp"
#include "./ui_diagwindow.h"
#include "backend/opcodes.hpp"

// ---------------------------------------------------------------------------

DiagWindow::DiagWindow(QWidget *parent):
        QMainWindow(parent), ui_(new Ui::DiagWindow), enumTimer_(this),
        refreshTimer_(this) {
    ui_->setupUi(this);
    refreshPortComboBox_();
    ui_->frameVdd->setEnabled(false);
    ui_->frameVpp->setEnabled(false);
    connect(&enumTimer_, &QTimer::timeout, this, &DiagWindow::onEnumTimerTimeout);
    connect(&refreshTimer_, &QTimer::timeout, this, &DiagWindow::onRefreshTimerTimeout);
    enumTimer_.start(kUsbEnumerateInterval);
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    this->setFixedSize(this->size());
}

DiagWindow::~DiagWindow() {
    delete ui_;
}

void DiagWindow::on_pushButtonConnect_clicked() {
    if (!serial_.isOpen()) {
        if (serial_.open(ui_->comboBoxPort->currentText())) {
            ui_->frameVdd->setEnabled(true);
            ui_->frameVpp->setEnabled(true);
            ui_->pushButtonConnect->setText(tr("&Disconnect"));
            ui_->comboBoxPort->setEnabled(false);
            enumTimer_.stop();
            refreshTimer_.start(kUsbRefreshInterval);
        } else {
            QMessageBox::critical(this,
                tr("USB Flash/EPROM Programmer"),
                tr("Error opening the \"%1\" port.")
                    .arg(ui_->comboBoxPort->currentText()));
        }
    } else {
        refreshTimer_.stop();
        serial_.close();
        ui_->frameVdd->setEnabled(false);
        ui_->frameVpp->setEnabled(false);
        ui_->pushButtonConnect->setText(tr("&Connect"));
        ui_->comboBoxPort->setEnabled(true);
        enumTimer_.start();
    }
}

void DiagWindow::on_actionExit_triggered(bool checked) {
    close();
}

void DiagWindow::on_actionAboutQt_triggered(bool checked) {
    QMessageBox::aboutQt(this);
}

void DiagWindow::onEnumTimerTimeout() {
    refreshPortComboBox_();
}

void DiagWindow::onRefreshTimerTimeout() {
    //if (!serial_.isOpen()) { return; }
    std::string cmd, response;
    char buf[255];
    // Get VDD
    float vdd = 0.0f;
    cmd = "0101";
    serial_.write(cmd.c_str(), cmd.length()), true;
    if (serial_.read(buf, 1) == 1) { }
    cmd = "03";
    serial_.write(cmd.c_str(), cmd.length(), true);
    TCmdOpCode opcode = kCmdOpCodes.at(cmd);
    if (serial_.read(buf, 6) == 6) {
        response = std::string(buf, 0, 6);
        vdd = std::stoi(response.substr(2, 2), nullptr, 16) + 
            std::stoi(response.substr(4, 2), nullptr, 16) * 0.1f;
        ui_->lcdNumberVdd->display(vdd);
    }
}

void DiagWindow::refreshPortComboBox_() {
    Serial::TSerialPortList list = serial_.list();
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
