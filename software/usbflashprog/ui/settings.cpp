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

// ---------------------------------------------------------------------------

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent), ui_(new Ui::SettingsDialog) {
    ui_->setupUi(this);
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    QString logFileInfo = ui_->labelLogFileInfo->text();
    logFileInfo =
        logFileInfo.arg("\n" + QDir::toNativeSeparators(QDir::homePath() + "/" +
                                                        QString(kLogFileName)));
    ui_->labelLogFileInfo->setText(logFileInfo);
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

TApplicationSettings SettingsDialog::loadSettings_() {
    QSettings settings;
    TApplicationSettings app = loadSettings();

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
    if (app.language.toLower() == "pt_br") {
        ui_->comboBoxLanguage->setCurrentIndex(2);
    } else if (app.language.toLower() == "en_us") {
        ui_->comboBoxLanguage->setCurrentIndex(1);
    } else {
        ui_->comboBoxLanguage->setCurrentIndex(0);
    }
    return app;
}

TApplicationSettings SettingsDialog::saveSettings_() {
    QSettings settings;
    TApplicationSettings app;

    app.logLevel = ui_->comboBoxLogLevel->currentIndex();
    switch (ui_->comboBoxLanguage->currentIndex()) {
        case 2:
            app.language = "pt_BR";
            break;
        case 1:
            app.language = "en_US";
            break;
        default:
            app.language = "";
            break;
    }

    settings.setValue(kSettingGeneralLogLevel, QString::number(app.logLevel));
    settings.setValue(kSettingGeneralLanguage, app.language);
    return app;
}
