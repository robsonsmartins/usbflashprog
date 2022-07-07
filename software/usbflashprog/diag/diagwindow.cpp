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

#include <QtGlobal>
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>
#include <QThread>
#include <QStyle>
#include <QScreen>
#include <QWindow>
#include <QDesktopWidget>

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
        ui_(new Ui::DiagWindow), runner_(this), enumTimer_(this),
        refreshTimer_(this) {
    ui_->setupUi(this);
    refreshPortComboBox_();
    ui_->frameVdd->setEnabled(false);
    ui_->frameVpp->setEnabled(false);
    ui_->frameBusCtrl->setEnabled(false);
    connect(&enumTimer_, &QTimer::timeout,
            this, &DiagWindow::onEnumTimerTimeout);
    connect(&refreshTimer_, &QTimer::timeout,
            this, &DiagWindow::onRefreshTimerTimeout);
    connect(&runner_, &Runner::resultReady,
            this, &DiagWindow::onRunnerResultReady);
    enumTimer_.start(kUsbEnumerateInterval);
    enableControls_(false);
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    this->setFixedSize(this->size());
    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            screen()->availableGeometry()));
}

DiagWindow::~DiagWindow() {
    delete ui_;
}

QScreen* DiagWindow::screen() const {
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
    return QGuiApplication::primaryScreen();
#else
    return QGuiApplication::screenAt(
        this->mapToGlobal({this->width() / 2, 0}));
#endif
}

void DiagWindow::on_pushButtonConnect_clicked() {
    if (!runner_.isOpen()) {
        connect_();
    } else {
        connect_(false);
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

void DiagWindow::on_pushButtonVddInitCal_clicked() {
    if (!runner_.isOpen()) { return; }
    enableControls_(false);
    runner_.send(kCmdVddInitCal);
    bool ok;
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
    float measured = QInputDialog::getDouble(this,
        tr("VDD Calibration"),
        tr("Voltage measured on the VDD:"),
        5.0f, 3.0f, 7.0f, 2, &ok,
        Qt::WindowFlags());
#else
    float measured = QInputDialog::getDouble(this,
        tr("VDD Calibration"),
        tr("Voltage measured on the VDD:"),
        5.0f, 3.0f, 7.0f, 2, &ok,
        Qt::WindowFlags(), 0.1f);
#endif
    if (ok) {
        runner_.send(kCmdVddSaveCal, measured);
    }
    enableControls_(true);
}

void DiagWindow::on_pushButtonVppInitCal_clicked() {
    if (!runner_.isOpen()) { return; }
    enableControls_(false);
    runner_.send(kCmdVppInitCal);
    bool ok;
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
    float measured = QInputDialog::getDouble(this,
        tr("VPP Calibration"),
        tr("Voltage measured on the VPP:"),
        12.0f, 10.0f, 14.0f, 2, &ok,
        Qt::WindowFlags());
#else
    float measured = QInputDialog::getDouble(this,
        tr("VPP Calibration"),
        tr("Voltage measured on the VPP:"),
        12.0f, 10.0f, 14.0f, 2, &ok,
        Qt::WindowFlags(), 0.1f);
#endif        
    if (ok) {
        runner_.send(kCmdVppSaveCal, measured);
    }
    enableControls_(true);
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

void DiagWindow::on_checkBoxCE_toggled(bool checked) {
    if (!runner_.isOpen()) { return; }
    runner_.send(kCmdBusCE, checked);
}

void DiagWindow::on_checkBoxOE_toggled(bool checked) {
    if (!runner_.isOpen()) { return; }
    runner_.send(kCmdBusOE, checked);
}

void DiagWindow::on_checkBoxWE_toggled(bool checked) {
    if (!runner_.isOpen()) { return; }
    runner_.send(kCmdBusWE, checked);
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
    if (!runner_.isOpen()) {
        // Connection lost
        QString port = ui_->comboBoxPort->currentText();
        connect_(false);
        enableControls_(false);
        QMessageBox::critical(this,
            tr("USB Flash/EPROM Programmer"),
            tr("The device has been disconnected from the \"%1\" port.")
                .arg(port));
        return;
    }
    runner_.send(kCmdVddGetV);
    runner_.send(kCmdVppGetV);
    runner_.send(kCmdVddGetDuty);
    runner_.send(kCmdVppGetDuty);
}

void DiagWindow::onRunnerResultReady(const TRunnerCommand& command) {
    auto originalLocale = std::locale::global(std::locale::classic());
    char buffer[5];
    switch (command.opcode.code) {
        case kCmdVddGetV:
            snprintf(buffer, sizeof(buffer), "%4.1f",
                     command.responseAsFloat());
            ui_->lcdNumberVdd->display(QString(buffer));
            break;
        case kCmdVppGetV:
            snprintf(buffer, sizeof(buffer), "%4.1f",
                     command.responseAsFloat());
            ui_->lcdNumberVpp->display(QString(buffer));
            break;
        case kCmdVddGetDuty:
            snprintf(buffer, sizeof(buffer), "%4.1f",
                     command.responseAsFloat());
            ui_->lcdNumberVddDuty->display(QString(buffer));
            break;
        case kCmdVppGetDuty:
            snprintf(buffer, sizeof(buffer), "%4.1f",
                     command.responseAsFloat());
            ui_->lcdNumberVppDuty->display(QString(buffer));
            break;
    }
    std::locale::global(originalLocale);
}

void DiagWindow::closeEvent(QCloseEvent *event) {
    static bool readyToClose = false;
    if (!readyToClose) { event->ignore(); }
    connect_(false);
    QTimer::singleShot(500, this,
        [this]{
            readyToClose = true;
            this->close();
        });
    enumTimer_.stop();
}

void DiagWindow::refreshPortComboBox_() {
    TSerialPortList list = runner_.list();
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

void DiagWindow::connect_(bool state) {
    if (state) {
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
    }
    if (runner_.isOpen()) {
        runner_.send(kCmdVppCtrl, false);
        runner_.send(kCmdVddCtrl, false);
        runner_.send(kCmdVddOnVpp, false);
        runner_.send(kCmdVppOnA9, false);
        runner_.send(kCmdVppOnA18, false);
        runner_.send(kCmdVppOnCE, false);
        runner_.send(kCmdVppOnOE, false);
        runner_.send(kCmdVppOnWE, false);
        runner_.send(kCmdBusCE, false);
        runner_.send(kCmdBusOE, false);
        runner_.send(kCmdBusWE, false);
    }
    if (!state) {
        refreshTimer_.stop();
        if (runner_.isOpen()) {
            QTimer::singleShot(500, this,
                [this]{
                    runner_.close();
                    enableControls_(false);
                });
        }
        enumTimer_.start();
    }
}

void DiagWindow::enableControls_(bool state) {
    ui_->frameVdd->setEnabled(state);
    ui_->frameVpp->setEnabled(state);
    ui_->frameBusCtrl->setEnabled(state);
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
        ui_->checkBoxCE->setChecked(false);
        ui_->checkBoxOE->setChecked(false);
        ui_->checkBoxWE->setChecked(false);
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
