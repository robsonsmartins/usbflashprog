// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @ingroup emu
 * @file main/mainwindow.cpp
 * @brief Implementation of the Main Window Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QtGlobal>
#include <QStyle>
#include <QWindow>
#include <QSettings>

#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

// ---------------------------------------------------------------------------

#define EMUPROG_REGISTRY_KEY "HKEY_CURRENT_USER\\Software\\EmuProg"
#define PROGLIB_REGISTRY_VALUE "ProgLib"
#define CHIPLIB_REGISTRY_VALUE "ChipLib"
#define CHIPSIZE_REGISTRY_VALUE "ChipSize"

#define PROG_WILLEM "willem.dll"
#define PROG_PCB45 "pcb45.dll"

#define CHIP_SRAM "sram.dll"
#define CHIP_EEPROM28C "eeprom28c.dll"
#define CHIP_EPROM "eprom.dll"

// ---------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
    ui_->setupUi(this);
    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            screen()->availableGeometry()));
    this->setFixedSize(this->geometry().width(), this->geometry().height());
    loadSettings_();
}

MainWindow::~MainWindow() {
    delete ui_;
}

QScreen* MainWindow::screen() const {
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
    return QGuiApplication::primaryScreen();
#else
    return QGuiApplication::screenAt(
        this->mapToGlobal({this->width() / 2, 0}));
#endif
}

void MainWindow::on_cbProg_activated(int index) {
    QString item = ui_->cbProg->itemText(index).toLower();
    QSettings settings(EMUPROG_REGISTRY_KEY, QSettings::NativeFormat);
    if (item.contains("sivava")) {
        settings.setValue(PROGLIB_REGISTRY_VALUE, PROG_PCB45);
    } else {
        settings.setValue(PROGLIB_REGISTRY_VALUE, PROG_WILLEM);
    }
}

void MainWindow::on_cbChip_activated(int index) {
    QString item = ui_->cbChip->itemText(index).toLower();
    QSettings settings(EMUPROG_REGISTRY_KEY, QSettings::NativeFormat);
    if (item.contains("eeprom 28c")) {
        settings.setValue(CHIPLIB_REGISTRY_VALUE, CHIP_EEPROM28C);
    } else if (item.contains("eprom 27")) {
        settings.setValue(CHIPLIB_REGISTRY_VALUE, CHIP_EPROM);
    } else {
        settings.setValue(CHIPLIB_REGISTRY_VALUE, CHIP_SRAM);
    }
}

void MainWindow::on_cbSize_activated(int index) {
    QString item = ui_->cbSize->itemText(index).toLower();
    QSettings settings(EMUPROG_REGISTRY_KEY, QSettings::NativeFormat);
    if (item.contains("16m")) {
        settings.setValue(CHIPSIZE_REGISTRY_VALUE, 16 * 1024 * 1024);
    } else if (item.contains("8m")) {
        settings.setValue(CHIPSIZE_REGISTRY_VALUE, 8 * 1024 * 1024);
    } else if (item.contains("4m")) {
        settings.setValue(CHIPSIZE_REGISTRY_VALUE, 4 * 1024 * 1024);
    } else if (item.contains("2m")) {
        settings.setValue(CHIPSIZE_REGISTRY_VALUE, 2 * 1024 * 1024);
    } else if (item.contains("1m")) {
        settings.setValue(CHIPSIZE_REGISTRY_VALUE, 1 * 1024 * 1024);
    } else if (item.contains("512k")) {
        settings.setValue(CHIPSIZE_REGISTRY_VALUE, 512 * 1024);
    } else if (item.contains("256k")) {
        settings.setValue(CHIPSIZE_REGISTRY_VALUE, 256 * 1024);
    } else if (item.contains("128k")) {
        settings.setValue(CHIPSIZE_REGISTRY_VALUE, 128 * 1024);
    } else if (item.contains("64k")) {
        settings.setValue(CHIPSIZE_REGISTRY_VALUE, 64 * 1024);
    } else if (item.contains("32k")) {
        settings.setValue(CHIPSIZE_REGISTRY_VALUE, 32 * 1024);
    } else if (item.contains("16k")) {
        settings.setValue(CHIPSIZE_REGISTRY_VALUE, 16 * 1024);
    } else if (item.contains("8k")) {
        settings.setValue(CHIPSIZE_REGISTRY_VALUE, 8 * 1024);
    } else if (item.contains("4k")) {
        settings.setValue(CHIPSIZE_REGISTRY_VALUE, 4 * 1024);
    } else {
        settings.setValue(CHIPSIZE_REGISTRY_VALUE, 2 * 1024);
    }
}

void MainWindow::loadSettings_() {
    QSettings settings(EMUPROG_REGISTRY_KEY, QSettings::NativeFormat);
    QString value = settings.value(PROGLIB_REGISTRY_VALUE).toString().toLower();
    if (value.contains(PROG_PCB45)) {
        ui_->cbProg->setCurrentIndex(1);
    } else {
        ui_->cbProg->setCurrentIndex(0);
    }
    value = settings.value(CHIPLIB_REGISTRY_VALUE).toString().toLower();
    if (value.contains(CHIP_EPROM)) {
        ui_->cbChip->setCurrentIndex(2);
    } else if (value.contains(CHIP_EEPROM28C)) {
        ui_->cbChip->setCurrentIndex(1);
    } else {
        ui_->cbChip->setCurrentIndex(0);
    }
    int size = settings.value(CHIPLIB_REGISTRY_VALUE).toInt();
    switch (size) {
        case 16 * 1024 * 1024:
            ui_->cbSize->setCurrentIndex(13);
            break;
        case 8 * 1024 * 1024:
            ui_->cbSize->setCurrentIndex(12);
            break;
        case 4 * 1024 * 1024:
            ui_->cbSize->setCurrentIndex(11);
            break;
        case 2 * 1024 * 1024:
            ui_->cbSize->setCurrentIndex(10);
            break;
        case 1 * 1024 * 1024:
            ui_->cbSize->setCurrentIndex(9);
            break;
        case 512 * 1024:
            ui_->cbSize->setCurrentIndex(8);
            break;
        case 256 * 1024:
            ui_->cbSize->setCurrentIndex(7);
            break;
        case 128 * 1024:
            ui_->cbSize->setCurrentIndex(6);
            break;
        case 64 * 1024:
            ui_->cbSize->setCurrentIndex(5);
            break;
        case 32 * 1024:
            ui_->cbSize->setCurrentIndex(4);
            break;
        case 16 * 1024:
            ui_->cbSize->setCurrentIndex(3);
            break;
        case 8 * 1024:
            ui_->cbSize->setCurrentIndex(2);
            break;
        case 4 * 1024:
            ui_->cbSize->setCurrentIndex(1);
            break;
        case 2 * 1024:
        default:
            ui_->cbSize->setCurrentIndex(0);
            break;
    }
}
