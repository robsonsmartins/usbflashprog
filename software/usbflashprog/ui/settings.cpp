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
 * @file ui/settings.cpp
 * @brief Implementation of the Settings Dialog Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QString>
#include <QDir>
#include <QSettings>

#include "settings.hpp"
#include "main.hpp"
#include "./ui_settings.h"
#include "backend/devices/device.hpp"

// ---------------------------------------------------------------------------

SettingsDialog::SettingsDialog(QWidget *parent, bool enableBtnCal)
    : QDialog(parent), ui_(new Ui::SettingsDialog) {
    ui_->setupUi(this);
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowTitle(QString(kApplicationFullName) + " - " + tr("Settings"));
    QString logFileInfo = ui_->labelLogFileInfo->text();
    logFileInfo =
        logFileInfo.arg("\n" + QDir::toNativeSeparators(QDir::homePath() + "/" +
                                                        QString(kLogFileName)));
    ui_->labelLogFileInfo->setText(logFileInfo);
    ui_->pushButtonVddInitCal->setEnabled(enableBtnCal);
    ui_->pushButtonVppInitCal->setEnabled(enableBtnCal);
    size_t count = sizeof(kAppSupportedLanguages) / sizeof(TLanguageSettings);
    ui_->comboBoxLanguage->clear();
    for (size_t i = 0; i < count; i++) {
        TLanguageSettings lang = kAppSupportedLanguages[i];
        if (i == 0) lang.desc = tr("Autodetect");
        ui_->comboBoxLanguage->addItem(lang.desc);
    }
    loadSettings_();
}

SettingsDialog::~SettingsDialog() {
    delete ui_;
}

void SettingsDialog::on_buttonBox_accepted() {
    TApplicationSettings settings = saveSettings_();
    initLogging(settings.logLevel);
    loadLanguage(settings.language, qApp);
    ui_->retranslateUi(this);
}

void SettingsDialog::on_comboBoxLogLevel_currentIndexChanged(int index) {
    ui_->labelLogFileInfo->setVisible(index != 0);
}

void SettingsDialog::on_pushButtonVddInitCal_clicked() {
    emit onBtnVddCalClicked();
}

void SettingsDialog::on_pushButtonVppInitCal_clicked() {
    emit onBtnVppCalClicked();
}

void SettingsDialog::on_comboBoxBufferSize_currentIndexChanged(int index) {
    ui_->labelBufferSizeInfo->setVisible(index < 4);
}

TApplicationSettings SettingsDialog::loadSettings_() {
    QSettings settings;
    TApplicationSettings app = loadSettings();
    app.prog.bufferSize =
        settings.value(kSettingProgBufferSize).toString().toUInt();
    if (!app.prog.bufferSize) {
        // Default
        app.prog.bufferSize = kDefaultDeviceBufferSize;
    }

    if (app.logLevel >= 0 && app.logLevel <= 5) {
        ui_->comboBoxLogLevel->setCurrentIndex(app.logLevel);
        ui_->labelLogFileInfo->setVisible(app.logLevel != 0);
    } else if (app.logLevel > 5) {
        ui_->comboBoxLogLevel->setCurrentIndex(5);
        ui_->labelLogFileInfo->setVisible(true);
    } else {
        ui_->comboBoxLogLevel->setCurrentIndex(0);
        ui_->labelLogFileInfo->setVisible(false);
    }

    ui_->comboBoxLanguage->setCurrentIndex(0);
    size_t count = sizeof(kAppSupportedLanguages) / sizeof(TLanguageSettings);
    for (size_t i = 0; i < count; i++) {
        TLanguageSettings lang = kAppSupportedLanguages[i];
        if (lang.code.toLower() == app.language.toLower()) {
            ui_->comboBoxLanguage->setCurrentIndex(i);
            break;
        }
    }

    int index = static_cast<int>(std::log2(app.prog.bufferSize));
    ui_->comboBoxBufferSize->setCurrentIndex(index);
    ui_->labelBufferSizeInfo->setVisible(index < 4);

    return app;
}

TApplicationSettings SettingsDialog::saveSettings_() {
    QSettings settings;
    TApplicationSettings app;

    app.logLevel = ui_->comboBoxLogLevel->currentIndex();
    TLanguageSettings lang =
        kAppSupportedLanguages[ui_->comboBoxLanguage->currentIndex()];
    app.language = lang.code;
    app.prog.bufferSize = static_cast<uint16_t>(
        std::pow(2, ui_->comboBoxBufferSize->currentIndex()));

    settings.setValue(kSettingGeneralLogLevel, QString::number(app.logLevel));
    settings.setValue(kSettingGeneralLanguage, app.language);
    settings.setValue(kSettingProgBufferSize,
                      QString::number(app.prog.bufferSize));

    return app;
}
