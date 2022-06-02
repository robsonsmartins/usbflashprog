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

#include "diag/diagwindow.hpp"
#include "config.hpp"
#include "./ui_diagwindow.h"

// ---------------------------------------------------------------------------

DiagWindow::DiagWindow(QWidget *parent):
        QMainWindow(parent), ui_(new Ui::DiagWindow), timer_(this) {
    ui_->setupUi(this);
    refreshPortComboBox_();
    ui_->frameVdd->setEnabled(false);
    ui_->frameVpp->setEnabled(false);
    connect(&timer_, &QTimer::timeout, this, &DiagWindow::onTimerTimeout);
    timer_.start(kUsbEnumerateInterval);
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
            timer_.stop();
        } else {
            QMessageBox::critical(this,
                tr("USB Flash/EPROM Programmer"),
                tr("Error opening the \"%1\" port.")
                    .arg(ui_->comboBoxPort->currentText()));
        }
    } else {
        serial_.close();
        ui_->frameVdd->setEnabled(false);
        ui_->frameVpp->setEnabled(false);
        ui_->pushButtonConnect->setText(tr("&Connect"));
        ui_->comboBoxPort->setEnabled(true);
        timer_.start();
    }
}

void DiagWindow::on_actionExit_triggered(bool checked) {
    close();
}

void DiagWindow::on_actionAboutQt_triggered(bool checked) {
    QMessageBox::aboutQt(this);
}

void DiagWindow::onTimerTimeout() {
    refreshPortComboBox_();
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
