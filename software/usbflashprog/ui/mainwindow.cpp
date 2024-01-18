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
 * @file ui/mainwindow.cpp
 * @brief Implementation of the Main Window Class.
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
#include <QDesktopServices>
#include <QFileDialog>
#include <QFileInfo>
#include <QMenu>
#include <QAction>
#include <QSettings>
#include <QSignalBlocker>

#include <cstdio>
#include <cstring>
#include <string>
#include <locale>
#include <cmath>

#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include "config.hpp"
#include "settings.hpp"
#include "backend/opcodes.hpp"
#include "backend/devices/parallel/dummy.hpp"
#include "backend/devices/parallel/sram.hpp"
#include "backend/devices/parallel/eprom.hpp"

// ---------------------------------------------------------------------------

/* @brief Minimum length of dialog labels, in characters. */
constexpr int kDialogLabelMinLength = 80;

/* @brief CRC32 table for calculation. */
constexpr uint32_t kCRC32Table[] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F,
    0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2,
    0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
    0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
    0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423,
    0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190, 0x01DB7106,
    0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D,
    0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950,
    0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7,
    0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9, 0x5005713C, 0x270241AA,
    0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
    0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84,
    0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB,
    0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E,
    0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55,
    0x316E8EEF, 0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28,
    0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F,
    0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
    0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69,
    0x616BFFD3, 0x166CCF45, 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC,
    0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693,
    0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D};

// ---------------------------------------------------------------------------
// General

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui_(new Ui::MainWindow),
      runner_(this),
      enumTimer_(this),
      refreshTimer_(this),
      device_(nullptr),
      noWarningDevice_(false) {
    ui_->setupUi(this);
    refreshPortComboBox_();
    ui_->frameVdd->setEnabled(false);
    ui_->frameVpp->setEnabled(false);
    ui_->frameBusCtrl->setEnabled(false);
    ui_->frameBusAddr->setEnabled(false);
    ui_->frameBusData->setEnabled(false);
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,
                                          this->size(),
                                          screen()->availableGeometry()));
    QFont font = ui_->spinBoxAddr->font();
    font.setCapitalization(QFont::AllUppercase);
    ui_->spinBoxAddr->setFont(font);
    font = ui_->spinBoxData->font();
    font.setCapitalization(QFont::AllUppercase);
    ui_->spinBoxData->setFont(font);

    progress_ = new QProgressDialog(this);
    progress_->close();
    progress_->setMinimumWidth(400);
    progress_->setMinimumDuration(0);
    progress_->setWindowModality(Qt::WindowModal);
    progress_->setWindowFlags(
        (progress_->windowFlags() | Qt::MSWindowsFixedSizeDialogHint) &
        ~Qt::WindowContextHelpButtonHint);

    hexeditor_ = new QHexEditor(ui_->frameEditor);
    hexeditor_->setMaximumWidth(620);
    hexeditor_->setFixedWidth(620);
    hexeditor_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    ui_->frameEditor->layout()->setAlignment(Qt::AlignLeft);
    ui_->frameEditor->layout()->addWidget(hexeditor_);
    ui_->frameEditor->layout()->setContentsMargins(0, 0, 20, 20);
    hexeditor_->setSize(2048);

    ui_->actionSave->setEnabled(false);
    connectSignals_();
    enableDiagControls_(false);
    enumTimer_.start(kUsbEnumerateInterval);

    checksumLabel_ = new QLabel();
    ui_->statusbar->addPermanentWidget(checksumLabel_);

    // restore window position and size
    QSettings settings;
    TApplicationSettings app;
    app.windowPos = settings.value(kSettingGeneralWindowPos).toPoint();
    app.windowSize = settings.value(kSettingGeneralWindowSize).toSize();
    if (app.windowPos.x() > 0 || app.windowPos.y() > 0 ||
        app.windowSize.width() > 0 || app.windowSize.height() > 0) {
        qDebug() << "Moving and resizing main window ("
                 << "left = " << app.windowPos.x()
                 << ", top = " << app.windowPos.y()
                 << ", width = " << app.windowSize.width()
                 << ", height = " << app.windowSize.height() << ")";
        move(app.windowPos);
        resize(app.windowSize);
    }

    loadProgSettings_();
    updateCheckSum_();
}

MainWindow::~MainWindow() {
    // save window position and size
    QSettings settings;
    TApplicationSettings app;
    app.windowPos = pos();
    app.windowSize = size();
    settings.setValue(kSettingGeneralWindowPos, app.windowPos);
    settings.setValue(kSettingGeneralWindowSize, app.windowSize);

    delete progress_;
    if (device_) {
        device_->disconnect();
        delete device_;
    }
    delete hexeditor_;
    delete checksumLabel_;
    delete ui_;
}

void MainWindow::connectSignals_() {
    connect(&enumTimer_, &QTimer::timeout, this,
            &MainWindow::onEnumTimerTimeout);
    connect(ui_->btnProgDevice, &QPushButton::clicked, this,
            &MainWindow::onBtnSelectDeviceClicked);
    for (QMenu *submenu : ui_->menuDevice->findChildren<QMenu *>()) {
        for (QAction *action : submenu->actions()) {
            connect(action, &QAction::triggered, this,
                    &MainWindow::onSelectDeviceTriggered);
        }
    }
    // editor
    connect(hexeditor_, &QHexEditor::changed, this, &MainWindow::onDataChanged);
    // diag
    connect(&refreshTimer_, &QTimer::timeout, this,
            &MainWindow::onRefreshTimerTimeout);
    QCheckBox *addrCheckBoxes[] = {
        ui_->checkBoxA0,  ui_->checkBoxA1,  ui_->checkBoxA2,  ui_->checkBoxA3,
        ui_->checkBoxA4,  ui_->checkBoxA5,  ui_->checkBoxA6,  ui_->checkBoxA7,
        ui_->checkBoxA8,  ui_->checkBoxA9,  ui_->checkBoxA10, ui_->checkBoxA11,
        ui_->checkBoxA12, ui_->checkBoxA13, ui_->checkBoxA14, ui_->checkBoxA15,
        ui_->checkBoxA16, ui_->checkBoxA17, ui_->checkBoxA18, ui_->checkBoxA19,
        ui_->checkBoxA20, ui_->checkBoxA21, ui_->checkBoxA22, ui_->checkBoxA23};
    for (QCheckBox *obj : addrCheckBoxes) {
        connect(obj, &QCheckBox::toggled, this,
                &MainWindow::onCheckBoxAddressToggled);
    }
    QCheckBox *dataCheckBoxes[] = {
        ui_->checkBoxD0,  ui_->checkBoxD1,  ui_->checkBoxD2,  ui_->checkBoxD3,
        ui_->checkBoxD4,  ui_->checkBoxD5,  ui_->checkBoxD6,  ui_->checkBoxD7,
        ui_->checkBoxD8,  ui_->checkBoxD9,  ui_->checkBoxD10, ui_->checkBoxD11,
        ui_->checkBoxD12, ui_->checkBoxD13, ui_->checkBoxD14, ui_->checkBoxD15};
    for (QCheckBox *obj : dataCheckBoxes) {
        connect(obj, &QCheckBox::toggled, this,
                &MainWindow::onCheckBoxDataToggled);
    }
}

QScreen *MainWindow::screen() const {
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
    return QGuiApplication::primaryScreen();
#else
    return QGuiApplication::screenAt(this->mapToGlobal({this->width() / 2, 0}));
#endif
}

void MainWindow::on_tabWidget_currentChanged(int index) {
    if (ui_->tabWidget->currentWidget() == ui_->tabDiagnostics) {
        enableEditorControls_(false);
    } else {
        connect_(false);
        enableEditorControls_(true);
    }
}

void MainWindow::on_actionBuffer_triggered(bool checked) {
    ui_->tabWidget->setCurrentWidget(ui_->tabBuffer);
}

void MainWindow::on_actionDiagnostics_triggered(bool checked) {
    ui_->tabWidget->setCurrentWidget(ui_->tabDiagnostics);
}

void MainWindow::on_actionExit_triggered(bool checked) {
    close();
}

void MainWindow::on_actionAbout_triggered(bool checked) {
    QMessageBox::about(
        this, tr("USB Flash/EPROM Programmer"),
        tr("A memory device programmer (Flash/EPROM/E2PROM) board and software,"
           "connected to PC by USB port.") +
            "\n\n" + tr("Version") + " " + kAppVersion + "\n\n" +
            tr("Project Home Page") + ": " + QString(kProjectHomePage) + "\n" +
            tr("Author") + ": " + QString(kAuthorName) + " (" +
            QString(kAuthorHomePage) + ")");
}

void MainWindow::on_actionAboutQt_triggered(bool checked) {
    QMessageBox::aboutQt(this);
}

void MainWindow::on_actionProjectHome_triggered(bool checked) {
    QDesktopServices::openUrl(QUrl(kProjectHomePage));
}

void MainWindow::on_actionAuthorHome_triggered(bool checked) {
    QDesktopServices::openUrl(QUrl(kAuthorHomePage));
}

void MainWindow::on_actionSettings_triggered(bool checked) {
    SettingsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        ui_->retranslateUi(this);
        loadProgSettings_();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    static bool readyToClose = false;
    static bool closing = false;
    if (!closing && hexeditor_->isChanged()) {
        if (!showDialogFileChanged_()) {
            event->ignore();
            ui_->tabWidget->setCurrentWidget(ui_->tabBuffer);
            return;
        }
    }
    closing = true;
    if (!readyToClose) {
        event->ignore();
    }
    connect_(false);
    QTimer::singleShot(500, this, [this] {
        readyToClose = true;
        this->close();
    });
    enumTimer_.stop();
}

void MainWindow::onEnumTimerTimeout() {
    refreshPortComboBox_();
}

void MainWindow::on_comboBoxProgPort_currentIndexChanged(int index) {
    configureProgControls_();
}

void MainWindow::onBtnSelectDeviceClicked(bool checked) {
    QPoint globalPos = ui_->btnProgDevice->mapToGlobal(
        QPoint(0, ui_->btnProgDevice->height()));
    ui_->menuDevice->exec(globalPos);
}

void MainWindow::onSelectDeviceTriggered(bool checked) {
    if (hexeditor_->isChanged()) {
        if (!showDialogFileChanged_()) return;
    }
    QAction *action = qobject_cast<QAction *>(sender());
    ui_->btnProgDevice->setText(action->text());
    createDevice_();
    configureProgControls_();
    saveProgSettings_();
    hexeditor_->fill(0xFF);
    noWarningDevice_ = false;
}

void MainWindow::onActionProgress(uint32_t current, uint32_t total, bool done,
                                  bool success, bool canceled) {
    static qint64 startTime = 0;
    if (!current) {
        progress_->setRange(current, total);
        startTime = QDateTime::currentMSecsSinceEpoch();
    }
    if (canceled) {
        progress_->setValue(total);
        QMessageBox::warning(
            this, progress_->windowTitle(),
            tr("Operation canceled.").leftJustified(kDialogLabelMinLength));
    } else if (done && success) {
        progress_->setValue(total);
        QMessageBox::information(this, progress_->windowTitle(),
                                 tr("Operation successfully completed.")
                                     .leftJustified(kDialogLabelMinLength));
    } else if (done && !success) {
        progress_->setValue(total);
        QMessageBox::critical(
            this, progress_->windowTitle(),
            tr("Error at address 0x%1")
                .arg(QString("%1").arg(current, 6, 16, QChar('0')).toUpper())
                .leftJustified(kDialogLabelMinLength));
    } else {
        qint64 elapsedTime = QDateTime::currentMSecsSinceEpoch() - startTime;
        progress_->setValue(current);
        progress_->setLabelText(
            tr("Processing address 0x%1 of 0x%2")
                .arg(QString("%1").arg(current, 6, 16, QChar('0')).toUpper())
                .arg(
                    QString("%1").arg(total - 1, 6, 16, QChar('0')).toUpper()) +
            "\n\n" +
            tr("Data rate: %1").arg(calculateDataRate_(elapsedTime, current)) +
            " | " +
            tr("Estimated remaining time: %1")
                .arg(calculateRemainingTime_(elapsedTime, current, total)));
    }
}

void MainWindow::on_actionRead_triggered(bool checked) {
    if (!device_ || !device_->getInfo().capability.hasRead) return;
    if (hexeditor_->isChanged()) {
        if (!showDialogFileChanged_()) return;
    }
    if (!showActionWarningDialog_()) return;
    configureDeviceFromControls_();
    showDialogActionProgress_(ui_->actionRead->text());
    QByteArray buffer;
    if (device_->read(buffer)) {
        hexeditor_->setSize(device_->getSize());
        hexeditor_->putData(buffer);
    }
}

void MainWindow::on_actionDoProgram_triggered(bool checked) {
    if (!device_ || !device_->getInfo().capability.hasProgram) return;
    QByteArray data = hexeditor_->getData();
    if (!showDifferentSizeDialog_(data)) return;
    if (!showActionWarningDialog_()) return;
    configureDeviceFromControls_();
    showDialogActionProgress_(ui_->actionDoProgram->text());
    device_->program(data);
}

void MainWindow::on_actionProgramAndVerify_triggered(bool checked) {
    if (!device_ || !device_->getInfo().capability.hasProgram ||
        !device_->getInfo().capability.hasVerify) {
        return;
    }
    QByteArray data = hexeditor_->getData();
    if (!showDifferentSizeDialog_(data)) return;
    if (!showActionWarningDialog_()) return;
    configureDeviceFromControls_();
    showDialogActionProgress_(ui_->actionProgramAndVerify->text());
    device_->program(data, true);
}

void MainWindow::on_actionVerify_triggered(bool checked) {
    if (!device_ || !device_->getInfo().capability.hasVerify) return;
    if (!showActionWarningDialog_()) return;
    configureDeviceFromControls_();
    showDialogActionProgress_(ui_->actionVerify->text());
    device_->verify(hexeditor_->getData());
}

void MainWindow::on_actionDoErase_triggered(bool checked) {
    if (!device_ || !device_->getInfo().capability.hasErase) return;
    if (!showActionWarningDialog_()) return;
    configureDeviceFromControls_();
    showDialogActionProgress_(ui_->actionDoErase->text());
    if (device_->erase()) {
        progress_->hide();
        QMessageBox::information(
            this, progress_->windowTitle(),
            tr("Device is blank").leftJustified(kDialogLabelMinLength));
    } else {
        progress_->hide();
        QMessageBox::critical(
            this, progress_->windowTitle(),
            tr("Device is not blank").leftJustified(kDialogLabelMinLength));
    }
}

void MainWindow::on_actionEraseAndBlankCheck_triggered(bool checked) {
    if (!device_ || !device_->getInfo().capability.hasErase ||
        !device_->getInfo().capability.hasBlankCheck) {
        return;
    }
    if (!showActionWarningDialog_()) return;
    configureDeviceFromControls_();
    showDialogActionProgress_(ui_->actionEraseAndBlankCheck->text());
    if (device_->erase(true)) {
        progress_->hide();
        QMessageBox::information(
            this, progress_->windowTitle(),
            tr("Device is blank").leftJustified(kDialogLabelMinLength));
    } else {
        progress_->hide();
        QMessageBox::critical(
            this, progress_->windowTitle(),
            tr("Device is not blank").leftJustified(kDialogLabelMinLength));
    }
}

void MainWindow::on_actionBlankCheck_triggered(bool checked) {
    if (!device_ || !device_->getInfo().capability.hasBlankCheck) return;
    if (!showActionWarningDialog_()) return;
    configureDeviceFromControls_();
    showDialogActionProgress_(ui_->actionBlankCheck->text());
    if (device_->blankCheck()) {
        progress_->hide();
        QMessageBox::information(
            this, progress_->windowTitle(),
            tr("Device is blank").leftJustified(kDialogLabelMinLength));
    } else {
        progress_->hide();
        QMessageBox::critical(
            this, progress_->windowTitle(),
            tr("Device is not blank").leftJustified(kDialogLabelMinLength));
    }
}

void MainWindow::on_actionGetID_triggered(bool checked) {
    if (!device_ || !device_->getInfo().capability.hasGetId) return;
    if (!showActionWarningDialog_()) return;
    TDeviceID deviceId;
    configureDeviceFromControls_();
    showDialogActionProgress_(ui_->actionGetID->text());
    if (device_->getId(deviceId)) {
        progress_->hide();
        QMessageBox::information(
            this, progress_->windowTitle(),
            tr("Manufacturer: 0x%1 (%2)")
                    .arg(QString("%1")
                             .arg(deviceId.manufacturer, 2, 16, QChar('0'))
                             .toUpper())
                    .arg(deviceId.getManufacturerName())
                    .leftJustified(kDialogLabelMinLength) +
                "\n" +
                tr("Device : 0x%1")
                    .arg(QString("%1")
                             .arg(deviceId.device, 2, 16, QChar('0'))
                             .toUpper())
                    .leftJustified(kDialogLabelMinLength));
    } else {
        progress_->hide();
        QMessageBox::critical(this, progress_->windowTitle(),
                              tr("Device doesn't support getting ID")
                                  .leftJustified(kDialogLabelMinLength));
    }
}

void MainWindow::on_actionUnprotect_triggered(bool checked) {
    if (!device_ || !device_->getInfo().capability.hasUnprotect) return;
    if (!showActionWarningDialog_()) return;
    configureDeviceFromControls_();
    showDialogActionProgress_(ui_->actionUnprotect->text());
    device_->unprotect();
}

void MainWindow::on_btnRead_clicked() {
    on_actionRead_triggered();
}

void MainWindow::on_btnProgram_clicked() {
    on_actionDoProgram_triggered();
}

void MainWindow::on_btnVerify_clicked() {
    on_actionVerify_triggered();
}

void MainWindow::on_btnErase_clicked() {
    on_actionDoErase_triggered();
}

void MainWindow::on_btnBlankCheck_clicked() {
    on_actionBlankCheck_triggered();
}

void MainWindow::on_btnGetID_clicked() {
    on_actionGetID_triggered();
}

void MainWindow::on_btnUnprotect_clicked() {
    on_actionUnprotect_triggered();
}

void MainWindow::on_spinBoxProgTWP_valueChanged(int value) {
    saveProgSettings_();
}

void MainWindow::on_spinBoxProgTWC_valueChanged(int value) {
    saveProgSettings_();
}

void MainWindow::on_spinBoxProgVDDrd_valueChanged(double value) {
    saveProgSettings_();
}

void MainWindow::on_spinBoxProgVDDwr_valueChanged(double value) {
    saveProgSettings_();
}

void MainWindow::on_spinBoxProgVPP_valueChanged(double value) {
    saveProgSettings_();
}

void MainWindow::on_spinBoxProgVEE_valueChanged(double value) {
    saveProgSettings_();
}

void MainWindow::on_checkBoxProgSkipFF_toggled(bool checked) {
    saveProgSettings_();
}

void MainWindow::on_checkBoxProgFast_toggled(bool checked) {
    saveProgSettings_();
}

void MainWindow::on_comboBoxProgSectorSize_currentIndexChanged(int index) {
    saveProgSettings_();
}

void MainWindow::on_comboBoxProgSize_currentIndexChanged(int index) {
    if (device_) {
        if (hexeditor_->isChanged()) {
            if (!showDialogFileChanged_()) return;
        }
        uint32_t size = static_cast<uint32_t>(
            ceil(powf(2, ui_->comboBoxProgSize->currentIndex()) * 2048.0f));
        device_->setSize(size);
        configureProgControls_();
        hexeditor_->fill(0xFF);
        saveProgSettings_();
    }
}

void MainWindow::loadProgSettings_() {
    QSettings settings;
    TProgrammerSettings prog;

    prog.device = settings.value(kSettingProgDevice).toString();
    prog.size = settings.value(kSettingProgDeviceSize).toString().toUInt();
    prog.twp = settings.value(kSettingProgTwp).toString().toUInt();
    prog.twc = settings.value(kSettingProgTwc).toString().toUInt();
    prog.vddRd = settings.value(kSettingProgVddRd).toString().toFloat();
    prog.vddWr = settings.value(kSettingProgVddWr).toString().toFloat();
    prog.vpp = settings.value(kSettingProgVpp).toString().toFloat();
    prog.vee = settings.value(kSettingProgVee).toString().toFloat();
    prog.skipFF = settings.value(kSettingProgSkipFF).toString().toInt() != 0;
    prog.fastProg = settings.value(kSettingProgFast).toString().toInt() != 0;
    prog.sectorSize =
        settings.value(kSettingProgSectorSize).toString().toUInt();

    if (!prog.device.isEmpty()) {
        ui_->btnProgDevice->setText(prog.device);
        createDevice_();
        device_->setSize(prog.size);
        device_->setTwp(prog.twp);
        device_->setTwc(prog.twc);
        device_->setVddRd(prog.vddRd);
        device_->setVddWr(prog.vddWr);
        device_->setVpp(prog.vpp);
        device_->setVee(prog.vee);
        device_->setSkipFF(prog.skipFF);
        device_->setFastProg(prog.fastProg);
        device_->setSectorSize(prog.sectorSize);
    }
    configureProgControls_();
}

void MainWindow::saveProgSettings_() {
    QSettings settings;
    TProgrammerSettings prog;

    prog.device = ui_->btnProgDevice->text();
    prog.size = static_cast<uint32_t>(
        ceil(powf(2, ui_->comboBoxProgSize->currentIndex()) * 2048.0f));
    prog.twp = ui_->spinBoxProgTWP->value();
    if (ui_->comboBoxProgTWPUnit->currentIndex() == 1) prog.twp *= 1000;
    prog.twc = ui_->spinBoxProgTWC->value();
    if (ui_->comboBoxProgTWCUnit->currentIndex() == 1) prog.twc *= 1000;
    prog.vddRd = ui_->spinBoxProgVDDrd->value();
    prog.vddWr = ui_->spinBoxProgVDDwr->value();
    prog.vpp = ui_->spinBoxProgVPP->value();
    prog.vee = ui_->spinBoxProgVEE->value();
    prog.skipFF = ui_->checkBoxProgSkipFF->isChecked();
    prog.fastProg = ui_->checkBoxProgFast->isChecked();
    prog.sectorSize = 0;
    if (ui_->comboBoxProgSectorSize->currentIndex() != 0) {
        prog.sectorSize = ui_->comboBoxProgSectorSize->currentText().toUInt();
    }

    settings.setValue(kSettingProgDevice, prog.device);
    settings.setValue(kSettingProgDeviceSize, QString::number(prog.size));
    settings.setValue(kSettingProgTwp, QString::number(prog.twp));
    settings.setValue(kSettingProgTwc, QString::number(prog.twc));
    settings.setValue(kSettingProgVddRd, QString::number(prog.vddRd));
    settings.setValue(kSettingProgVddWr, QString::number(prog.vddWr));
    settings.setValue(kSettingProgVpp, QString::number(prog.vpp));
    settings.setValue(kSettingProgVee, QString::number(prog.vee));
    settings.setValue(kSettingProgSkipFF, QString::number(prog.skipFF ? 1 : 0));
    settings.setValue(kSettingProgFast, QString::number(prog.fastProg ? 1 : 0));
    settings.setValue(kSettingProgSectorSize, QString::number(prog.sectorSize));
}

void MainWindow::createDevice_() {
    if (device_) {
        device_->disconnect();
        delete device_;
        device_ = nullptr;
    }
    createDeviceIfSRAM_(ui_->btnProgDevice->text());
    createDeviceIfEPROM_(ui_->btnProgDevice->text());
    createDeviceIfErasableEPROM_(ui_->btnProgDevice->text());
    if (!device_) {
        if (ui_->btnProgDevice->text() == ui_->actionDummy->text()) {
            device_ = new Dummy(this);
            hexeditor_->setMode(QHexEditor::Mode8Bits);
        } else {
            device_ = new Dummy16Bit(this);
            hexeditor_->setMode(QHexEditor::Mode16Bits);
        }
        device_->setSize(2048);
        ui_->actionDoProgram->setText(tr("Program"));
        ui_->btnProgram->setToolTip(ui_->actionDoProgram->text());
    }
    connect(device_, &Device::onProgress, this, &MainWindow::onActionProgress);
}

void MainWindow::createDeviceIfSRAM_(const QString &label) {
    uint32_t size = 0x800;
    bool found = false, custom = false;
    if (label == ui_->actionSRAM_2KB->text()) {
        size = 0x800;
        found = true;
    } else if (label == ui_->actionSRAM_8KB->text()) {
        size = 0x2000;
        found = true;
    } else if (label == ui_->actionSRAM_16KB->text()) {
        size = 0x4000;
        found = true;
    } else if (label == ui_->actionSRAM_32KB->text()) {
        size = 0x8000;
        found = true;
    } else if (label == ui_->actionSRAM->text()) {
        found = true;
        custom = true;
    }
    if (found) {
        device_ = new SRAM(this);
        ui_->actionDoProgram->setText(tr("Test SRAM"));
        ui_->btnProgram->setToolTip(ui_->actionDoProgram->text());
        device_->setSize(size);
        hexeditor_->setMode(QHexEditor::Mode8Bits);
        ui_->comboBoxProgSize->setEnabled(custom);
        ui_->labelProgSize->setEnabled(custom);
    }
}

void MainWindow::createDeviceIfEPROM_(const QString &label) {
    uint32_t size = 0x800;
    bool found = false, custom = false;
    // 27xxx
    if (label == ui_->actionEPROM27_2KB->text()) {
        size = 0x800;
        found = true;
    } else if (label == ui_->actionEPROM27_4KB->text()) {
        size = 0x1000;
        found = true;
    } else if (label == ui_->actionEPROM27_8KB->text()) {
        size = 0x2000;
        found = true;
    } else if (label == ui_->actionEPROM27_16KB->text()) {
        size = 0x4000;
        found = true;
    } else if (label == ui_->actionEPROM27_32KB->text()) {
        size = 0x8000;
        found = true;
    } else if (label == ui_->actionEPROM27_64KB->text()) {
        size = 0x10000;
        found = true;
    } else if (label == ui_->actionEPROM27_128KB->text()) {
        size = 0x20000;
        found = true;
    } else if (label == ui_->actionEPROM27_256KB->text()) {
        size = 0x40000;
        found = true;
    } else if (label == ui_->actionEPROM27_512KB->text()) {
        size = 0x80000;
        found = true;
    } else if (label == ui_->actionEPROM27xxx->text()) {
        found = true;
        custom = true;
    }
    if (found) {
        ui_->actionDoProgram->setText(tr("Program"));
        ui_->btnProgram->setToolTip(ui_->actionDoProgram->text());
        device_ = new EPROM27(this);
        device_->setSize(size);
        hexeditor_->setMode(QHexEditor::Mode8Bits);
        ui_->comboBoxProgSize->setEnabled(custom);
        ui_->labelProgSize->setEnabled(custom);
    }
    size = 0x800;
    found = false;
    custom = false;
    // 27Cxxx
    if (label == ui_->actionEPROM27C_2KB->text()) {
        size = 0x800;
        found = true;
    } else if (label == ui_->actionEPROM27C_4KB->text()) {
        size = 0x1000;
        found = true;
    } else if (label == ui_->actionEPROM27C_8KB->text()) {
        size = 0x2000;
        found = true;
    } else if (label == ui_->actionEPROM27C_16KB->text()) {
        size = 0x4000;
        found = true;
    } else if (label == ui_->actionEPROM27C_32KB->text()) {
        size = 0x8000;
        found = true;
    } else if (label == ui_->actionEPROM27C_64KB->text()) {
        size = 0x10000;
        found = true;
    } else if (label == ui_->actionEPROM27C_128KB->text()) {
        size = 0x20000;
        found = true;
    } else if (label == ui_->actionEPROM27C_256KB->text()) {
        size = 0x40000;
        found = true;
    } else if (label == ui_->actionEPROM27C_512KB->text()) {
        size = 0x80000;
        found = true;
    } else if (label == ui_->actionEPROM27C_1MB->text()) {
        size = 0x100000;
        found = true;
    } else if (label == ui_->actionEPROM27Cxxx->text()) {
        found = true;
        custom = true;
    }
    if (found) {
        ui_->actionDoProgram->setText(tr("Program"));
        ui_->btnProgram->setToolTip(ui_->actionDoProgram->text());
        device_ = new EPROM27C(this);
        device_->setSize(size);
        hexeditor_->setMode(QHexEditor::Mode8Bits);
        ui_->comboBoxProgSize->setEnabled(custom);
        ui_->labelProgSize->setEnabled(custom);
    }
    size = 0x800;
    found = false;
    custom = false;
    // 27Cxxx 16 bits
    if (label == ui_->actionEPROM27C16Bit_128KB->text()) {
        size = 0x20000;
        found = true;
    } else if (label == ui_->actionEPROM27C16Bit_256KB->text()) {
        size = 0x40000;
        found = true;
    } else if (label == ui_->actionEPROM27C16Bit_512KB->text()) {
        size = 0x80000;
        found = true;
    } else if (label == ui_->actionEPROM27C16Bit_1MB->text()) {
        size = 0x100000;
        found = true;
    } else if (label == ui_->actionEPROM27C16Bit_2MB->text()) {
        size = 0x200000;
        found = true;
    } else if (label == ui_->actionEPROM27C16Bit_4MB->text()) {
        size = 0x400000;
        found = true;
    } else if (label == ui_->actionEPROM27C16Bitxxx->text()) {
        found = true;
        custom = true;
    }
    if (found) {
        ui_->actionDoProgram->setText(tr("Program"));
        ui_->btnProgram->setToolTip(ui_->actionDoProgram->text());
        device_ = new EPROM27C16Bit(this);
        device_->setSize(size);
        hexeditor_->setMode(QHexEditor::Mode16Bits);
        ui_->comboBoxProgSize->setEnabled(custom);
        ui_->labelProgSize->setEnabled(custom);
    }
}

void MainWindow::createDeviceIfErasableEPROM_(const QString &label) {
    uint32_t size = 0x800;
    bool found = false, custom = false;
    float vpp = 12.0f, vee = 14.0f, vdd = 5.0f;
    // W27x
    if (label == ui_->actionEPROMW27_32KB->text()) {
        size = 0x8000;
        found = true;
    } else if (label == ui_->actionEPROMW27_64KB->text()) {
        size = 0x10000;
        found = true;
    } else if (label == ui_->actionEPROMW27_128KB->text()) {
        size = 0x20000;
        found = true;
    } else if (label == ui_->actionEPROMW27_256KB->text()) {
        size = 0x40000;
        found = true;
    } else if (label == ui_->actionEPROMW27_256KB_12V->text()) {
        size = 0x40000;
        found = true;
        vee = 12.0f;
    } else if (label == ui_->actionEPROMW27_512KB->text()) {
        size = 0x80000;
        found = true;
    } else if (label == ui_->actionEPROM27Exxx->text()) {
        size = 0x800;
        found = true;
        custom = true;
    }
    if (found) {
        ui_->actionDoProgram->setText(tr("Program"));
        ui_->btnProgram->setToolTip(ui_->actionDoProgram->text());
        device_ = new EPROM27E(this);
        device_->setSize(size);
        device_->setVpp(vpp);
        device_->setVee(vee);
        device_->setVddWr(vdd);
        hexeditor_->setMode(QHexEditor::Mode8Bits);
        ui_->comboBoxProgSize->setEnabled(custom);
        ui_->labelProgSize->setEnabled(custom);
    }
    found = false;
    vpp = 12.0f;
    vee = 12.0f;
    vdd = 5.0f;
    // 27SF
    if (label == ui_->actionEPROM27SF_32KB->text()) {
        size = 0x8000;
        found = true;
    } else if (label == ui_->actionEPROM27SF_64KB->text()) {
        size = 0x10000;
        found = true;
    } else if (label == ui_->actionEPROM27SF_128KB->text()) {
        size = 0x20000;
        found = true;
    } else if (label == ui_->actionEPROM27SF_256KB->text()) {
        size = 0x40000;
        found = true;
    } else if (label == ui_->actionEPROM27SF_512KB->text()) {
        size = 0x80000;
        found = true;
    }
    if (found) {
        ui_->actionDoProgram->setText(tr("Program"));
        ui_->btnProgram->setToolTip(ui_->actionDoProgram->text());
        device_ = new EPROM27E(this);
        device_->setSize(size);
        device_->setVpp(vpp);
        device_->setVee(vee);
        device_->setVddWr(vdd);
        hexeditor_->setMode(QHexEditor::Mode8Bits);
        ui_->comboBoxProgSize->setEnabled(false);
        ui_->labelProgSize->setEnabled(false);
    }
    found = false;
    vpp = 12.75f;
    vee = 12.75f;
    vdd = 6.25f;
    // MX26C
    if (label == ui_->actionEPROM26C_128KB->text()) {
        size = 0x20000;
        found = true;
    } else if (label == ui_->actionEPROM26C_256KB->text()) {
        size = 0x40000;
        found = true;
    } else if (label == ui_->actionEPROM26C_512KB->text()) {
        size = 0x80000;
        found = true;
        vee = 12.5f;
    }
    if (found) {
        ui_->actionDoProgram->setText(tr("Program"));
        ui_->btnProgram->setToolTip(ui_->actionDoProgram->text());
        device_ = new EPROM27E(this);
        device_->setSize(size);
        device_->setVpp(vpp);
        device_->setVee(vee);
        device_->setVddWr(vdd);
        hexeditor_->setMode(QHexEditor::Mode8Bits);
        ui_->comboBoxProgSize->setEnabled(false);
        ui_->labelProgSize->setEnabled(false);
    }
    found = false;
    vpp = 12.0f;
    vee = 12.0f;
    vdd = 3.3f;
    // 37VF
    if (label == ui_->actionEPROM37VF_64KB->text()) {
        size = 0x10000;
        found = true;
    } else if (label == ui_->actionEPROM37VF_128KB->text()) {
        size = 0x20000;
        found = true;
    } else if (label == ui_->actionEPROM37VF_256KB->text()) {
        size = 0x40000;
        found = true;
    } else if (label == ui_->actionEPROM37VF_512KB->text()) {
        size = 0x80000;
        found = true;
    }
    if (found) {
        ui_->actionDoProgram->setText(tr("Program"));
        ui_->btnProgram->setToolTip(ui_->actionDoProgram->text());
        device_ = new EPROM27E(this);
        device_->setSize(size);
        device_->setVpp(vpp);
        device_->setVee(vee);
        device_->setVddRd(vdd);
        device_->setVddWr(vdd);
        hexeditor_->setMode(QHexEditor::Mode8Bits);
        ui_->comboBoxProgSize->setEnabled(false);
        ui_->labelProgSize->setEnabled(false);
    }
}

void MainWindow::configureProgControls_() {
    ui_->spinBoxProgTWP->blockSignals(true);
    ui_->comboBoxProgTWPUnit->blockSignals(true);
    ui_->spinBoxProgTWC->blockSignals(true);
    ui_->comboBoxProgTWCUnit->blockSignals(true);
    ui_->spinBoxProgVDDrd->blockSignals(true);
    ui_->spinBoxProgVDDwr->blockSignals(true);
    ui_->spinBoxProgVPP->blockSignals(true);
    ui_->spinBoxProgVEE->blockSignals(true);
    ui_->checkBoxProgFast->blockSignals(true);
    ui_->checkBoxProgSkipFF->blockSignals(true);
    ui_->comboBoxProgSectorSize->blockSignals(true);
    ui_->comboBoxProgSize->blockSignals(true);

    TDeviceInformation info;
    if (device_) info = device_->getInfo();
    TDeviceCapabilities capability = info.capability;
    bool port = !ui_->comboBoxProgPort->currentText().isEmpty();

    ui_->checkBoxProgFast->setEnabled(capability.hasFastProg && port);
    ui_->checkBoxProgSkipFF->setEnabled(capability.hasSkipFF && port);
    ui_->comboBoxProgSectorSize->setEnabled(capability.hasSectorSize && port);
    ui_->labelProgSectorSize->setEnabled(
        ui_->comboBoxProgSectorSize->isEnabled());
    ui_->actionRead->setEnabled(capability.hasRead && port);
    ui_->menuProgram->setEnabled(capability.hasProgram && port);
    ui_->actionDoProgram->setEnabled(ui_->menuProgram->isEnabled());
    ui_->actionProgramAndVerify->setEnabled(capability.hasProgram &&
                                            capability.hasVerify && port);
    ui_->actionVerify->setEnabled(capability.hasVerify && port);
    ui_->menuErase->setEnabled(capability.hasErase && port);
    ui_->actionDoErase->setEnabled(ui_->menuErase->isEnabled());
    ui_->actionEraseAndBlankCheck->setEnabled(capability.hasErase &&
                                              capability.hasBlankCheck && port);
    ui_->actionBlankCheck->setEnabled(capability.hasBlankCheck && port);
    ui_->actionGetID->setEnabled(capability.hasGetId && port);
    ui_->actionUnprotect->setEnabled(capability.hasUnprotect && port);

    ui_->btnRead->setEnabled(ui_->actionRead->isEnabled());
    ui_->btnProgram->setEnabled(ui_->actionDoProgram->isEnabled());
    ui_->btnVerify->setEnabled(ui_->actionVerify->isEnabled());
    ui_->btnErase->setEnabled(ui_->actionDoErase->isEnabled());
    ui_->btnBlankCheck->setEnabled(ui_->actionBlankCheck->isEnabled());
    ui_->btnGetID->setEnabled(ui_->actionGetID->isEnabled());
    ui_->btnUnprotect->setEnabled(ui_->actionUnprotect->isEnabled());

    ui_->spinBoxProgVDDrd->setEnabled(capability.hasVDD && port);
    ui_->labelProgVDDrd->setEnabled(ui_->spinBoxProgVDDrd->isEnabled());
    ui_->spinBoxProgVDDwr->setEnabled(capability.hasVPP && port);
    ui_->labelProgVDDwr->setEnabled(ui_->spinBoxProgVDDwr->isEnabled());
    ui_->spinBoxProgVPP->setEnabled(capability.hasVPP && port);
    ui_->labelProgVPP->setEnabled(ui_->spinBoxProgVPP->isEnabled());
    ui_->spinBoxProgVEE->setEnabled(capability.hasVPP && port &&
                                    (capability.hasErase ||
                                     capability.hasGetId ||
                                     capability.hasUnprotect));
    ui_->labelProgVEE->setEnabled(ui_->spinBoxProgVEE->isEnabled());

    if (device_) {
        uint32_t twp = device_->getTwp();
        if (twp < 1000) {
            ui_->spinBoxProgTWP->setValue(twp);
            ui_->comboBoxProgTWPUnit->setCurrentIndex(0);
        } else {
            ui_->spinBoxProgTWP->setValue(twp / 1000);
            ui_->comboBoxProgTWPUnit->setCurrentIndex(1);
        }

        uint32_t twc = device_->getTwc();
        if (twc < 1000) {
            ui_->spinBoxProgTWC->setValue(twc);
            ui_->comboBoxProgTWCUnit->setCurrentIndex(0);
        } else {
            ui_->spinBoxProgTWC->setValue(twc / 1000);
            ui_->comboBoxProgTWCUnit->setCurrentIndex(1);
        }

        ui_->spinBoxProgVDDrd->setValue(device_->getVddRd());
        ui_->spinBoxProgVDDwr->setValue(device_->getVddWr());
        ui_->spinBoxProgVPP->setValue(device_->getVpp());
        ui_->spinBoxProgVEE->setValue(device_->getVee());
        ui_->checkBoxProgFast->setChecked(device_->getFastProg());
        ui_->checkBoxProgSkipFF->setChecked(device_->getSkipFF());

        uint16_t sectorSize = device_->getSectorSize();
        int currentIndex = 0;
        if (sectorSize != 0) {
            currentIndex = static_cast<int>(ceil(log2(sectorSize / 64.0))) + 1;
            currentIndex = qMax(0, currentIndex);
            currentIndex =
                qMin(ui_->comboBoxProgSectorSize->count() - 1, currentIndex);
        }
        ui_->comboBoxProgSectorSize->setCurrentIndex(currentIndex);

        uint32_t size = device_->getSize();
        currentIndex = 0;
        if (size != 0) {
            currentIndex = static_cast<int>(ceil(log2(size / 2048.0)));
            currentIndex = qMax(0, currentIndex);
            currentIndex =
                qMin(ui_->comboBoxProgSize->count() - 1, currentIndex);
        }
        ui_->comboBoxProgSize->setCurrentIndex(currentIndex);
        hexeditor_->setSize(size);
    }

    ui_->spinBoxProgTWP->blockSignals(false);
    ui_->comboBoxProgTWPUnit->blockSignals(false);
    ui_->spinBoxProgTWC->blockSignals(false);
    ui_->comboBoxProgTWCUnit->blockSignals(false);
    ui_->spinBoxProgVDDrd->blockSignals(false);
    ui_->spinBoxProgVDDwr->blockSignals(false);
    ui_->spinBoxProgVPP->blockSignals(false);
    ui_->spinBoxProgVEE->blockSignals(false);
    ui_->checkBoxProgFast->blockSignals(false);
    ui_->checkBoxProgSkipFF->blockSignals(false);
    ui_->comboBoxProgSectorSize->blockSignals(false);
    ui_->comboBoxProgSize->blockSignals(false);
}

void MainWindow::configureDeviceFromControls_() {
    device_->setPort(ui_->comboBoxPort->currentText());
    uint32_t twp = ui_->spinBoxProgTWP->value();
    if (ui_->comboBoxProgTWPUnit->currentIndex() == 1) twp *= 1000;
    device_->setTwp(twp);
    uint32_t twc = ui_->spinBoxProgTWC->value();
    if (ui_->comboBoxProgTWCUnit->currentIndex() == 1) twc *= 1000;
    device_->setTwc(twc);
    device_->setVddRd(ui_->spinBoxProgVDDrd->value());
    device_->setVddWr(ui_->spinBoxProgVDDwr->value());
    device_->setVpp(ui_->spinBoxProgVPP->value());
    device_->setVee(ui_->spinBoxProgVEE->value());
    device_->setSkipFF(ui_->checkBoxProgSkipFF->isChecked());
    device_->setFastProg(ui_->checkBoxProgFast->isChecked());
    uint16_t sectorSize = 0;
    if (ui_->comboBoxProgSectorSize->currentIndex() != 0) {
        sectorSize = ui_->comboBoxProgSectorSize->currentText().toInt();
    }
    device_->setSectorSize(sectorSize);
}

void MainWindow::showDialogActionProgress_(const QString &msg) {
    progress_->setWindowTitle(tr("USB Flash/EPROM Programmer") + " - " + msg);
    progress_->setCancelButtonText(tr("Cancel"));
    progress_->setRange(0, 100);
    progress_->setAutoClose(true);
    connect(progress_, &QProgressDialog::canceled, [&] { device_->cancel(); });
    progress_->setValue(0);
    progress_->open();
}

bool MainWindow::showDifferentSizeDialog_(const QByteArray &data) {
    if (data.size() != device_->getSize()) {
        if (QMessageBox::question(
                this, tr("USB Flash/EPROM Programmer"),
                tr("The amount of data in the buffer is different from the "
                   "device size.")
                        .leftJustified(kDialogLabelMinLength) +
                    "\n" +
                    tr("Do you want to write the truncated data anyway?")
                        .leftJustified(kDialogLabelMinLength)) ==
            QMessageBox::No) {
            return false;
        }
    }
    return true;
}

bool MainWindow::showActionWarningDialog_() {
    if (noWarningDevice_) return true;
    if (QMessageBox::warning(
            this, tr("USB Flash/EPROM Programmer"),
            tr("Caution! Check the VDD, VPP and VEE voltages and the size of "
               "the device before running, otherwise you will damage it!")
                    .leftJustified(kDialogLabelMinLength) +
                "\n\n" +
                tr("Consult the device datasheet for more information.")
                    .leftJustified(kDialogLabelMinLength) +
                "\n\n" +
                tr("Are you sure you want to continue?")
                    .leftJustified(kDialogLabelMinLength),
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
        return false;
    }
    noWarningDevice_ = true;
    return true;
}

void MainWindow::refreshPortComboBox_() {
    TSerialPortList list = runner_.list();
    QStringList paths;
    for (const auto item : list) {
        paths.push_back(item.portName());
    }
    if (paths.length() != ui_->comboBoxPort->count()) {
        ui_->comboBoxPort->clear();
        ui_->comboBoxProgPort->clear();
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
        if (!found) {
            break;
        }
    }
    if (!found) {
        ui_->comboBoxPort->clear();
        ui_->comboBoxProgPort->clear();
        for (const auto path : paths) {
            ui_->comboBoxPort->addItem(path);
            ui_->comboBoxProgPort->addItem(path);
        }
    }
    ui_->pushButtonConnect->setEnabled(!paths.empty());
}

QString MainWindow::calculateDataRate_(int64_t elapsed, uint32_t amount) {
    // Bytes/s
    double value = (elapsed ? (amount * 1.0 / elapsed) : 0.0) * 1000.0;
    // bps
    value *= (!device_ || !device_->is16Bit()) ? 8 : 16;
    if (value >= 1000 * 1000) {  // Mbps
        return QString("%1 Mbps").arg(
            static_cast<int>(round(value / 1000.0 / 1000.0)));
    } else if (value >= 1000) {  // Kbps
        return QString("%1 Kbps").arg(static_cast<int>(round(value / 1000.0)));
    } else {  // bps
        return QString("%1 bps").arg(static_cast<int>(round(value)));
    }
}

QString MainWindow::calculateRemainingTime_(int64_t elapsed, uint32_t current,
                                            uint32_t total) {
    double value =
        (current ? (elapsed * 1.0 * (total - current) / current) : 0.0) /
        1000.0;
    if (value >= 60 * 60) {  // hr
        return tr("%1 hour(s)")
            .arg(static_cast<int>(round(value / 60.0 / 60.0)));
    } else if (value >= 60) {  // min
        return tr("%1 minute(s)").arg(static_cast<int>(round(value / 60.0)));
    } else {  // sec
        return tr("%1 second(s)").arg(static_cast<int>(round(value)));
    }
}

// ---------------------------------------------------------------------------
// Diagnostics

void MainWindow::on_pushButtonConnect_clicked() {
    if (!runner_.isOpen()) {
        connect_();
    } else {
        connect_(false);
    }
    noWarningDevice_ = false;
}

void MainWindow::on_pushButtonVddInitCal_clicked() {
    if (!runner_.isOpen()) return;
    refreshTimer_.stop();
    enableDiagControls_(false);
    float measured = 0.0f;
    bool ok = runner_.vddInitCal();
    if (ok) {
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
        measured = QInputDialog::getDouble(
            this, tr("VDD Calibration"),
            tr("Voltage measured on the VDD:")
                .leftJustified(kDialogLabelMinLength),
            5.0f, 3.0f, 7.0f, 2, &ok,
            Qt::WindowSystemMenuHint | Qt::WindowTitleHint |
                Qt::MSWindowsFixedSizeDialogHint);
#else
        measured = QInputDialog::getDouble(
            this, tr("VDD Calibration"),
            tr("Voltage measured on the VDD:")
                .leftJustified(kDialogLabelMinLength),
            5.0f, 3.0f, 7.0f, 2, &ok,
            Qt::WindowSystemMenuHint | Qt::WindowTitleHint |
                Qt::MSWindowsFixedSizeDialogHint,
            0.1f);
#endif
        if (ok) {
            ok = runner_.vddSaveCal(measured);
        } else {
            ok = true;
        }
    }
    if (!ok) {
        QMessageBox::critical(
            this, tr("USB Flash/EPROM Programmer"),
            tr("Error calibrating VDD").leftJustified(kDialogLabelMinLength));
    }
    enableDiagControls_(true);
    refreshTimer_.start();
}

void MainWindow::on_pushButtonVppInitCal_clicked() {
    if (!runner_.isOpen()) return;
    enableDiagControls_(false);
    refreshTimer_.stop();
    float measured = 0.0f;
    bool ok = runner_.vppInitCal();
    if (ok) {
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
        measured = QInputDialog::getDouble(
            this, tr("VPP Calibration"),
            tr("Voltage measured on the VPP:")
                .leftJustified(kDialogLabelMinLength),
            12.0f, 10.0f, 14.0f, 2, &ok,
            Qt::WindowSystemMenuHint | Qt::WindowTitleHint |
                Qt::MSWindowsFixedSizeDialogHint);
#else
        measured = QInputDialog::getDouble(
            this, tr("VPP Calibration"),
            tr("Voltage measured on the VPP:")
                .leftJustified(kDialogLabelMinLength),
            12.0f, 10.0f, 14.0f, 2, &ok,
            Qt::WindowSystemMenuHint | Qt::WindowTitleHint |
                Qt::MSWindowsFixedSizeDialogHint,
            0.1f);
#endif
        if (ok) {
            ok = runner_.vppSaveCal(measured);
        } else {
            ok = true;
        }
    }
    if (!ok) {
        QMessageBox::critical(
            this, tr("USB Flash/EPROM Programmer"),
            tr("Error calibrating VPP").leftJustified(kDialogLabelMinLength));
    }
    enableDiagControls_(true);
    refreshTimer_.start();
}

void MainWindow::on_pushButtonGetDataW_clicked() {
    if (!runner_.isOpen()) return;
    uint16_t value = runner_.dataGetW();
    ui_->spinBoxData->setValue(value);
}

void MainWindow::on_pushButtonGetDataB_clicked() {
    if (!runner_.isOpen()) return;
    uint8_t value = runner_.dataGet();
    ui_->spinBoxData->setValue(value);
}

void MainWindow::on_checkBoxVddCtrl_toggled(bool checked) {
    if (!runner_.isOpen()) return;
    runner_.vddCtrl(checked);
}

void MainWindow::on_checkBoxVppCtrl_toggled(bool checked) {
    if (!runner_.isOpen()) return;
    runner_.vppCtrl(checked);
}

void MainWindow::on_checkBoxVddOnVpp_toggled(bool checked) {
    if (!runner_.isOpen()) return;
    runner_.vddOnVpp(checked);
}

void MainWindow::on_checkBoxVppOnA9_toggled(bool checked) {
    if (!runner_.isOpen()) return;
    runner_.vppOnA9(checked);
}

void MainWindow::on_checkBoxVppOnA18_toggled(bool checked) {
    if (!runner_.isOpen()) return;
    runner_.vppOnA18(checked);
}

void MainWindow::on_checkBoxVppOnCE_toggled(bool checked) {
    if (!runner_.isOpen()) return;
    runner_.vppOnCE(checked);
}

void MainWindow::on_checkBoxVppOnOE_toggled(bool checked) {
    if (!runner_.isOpen()) return;
    runner_.vppOnOE(checked);
}

void MainWindow::on_checkBoxVppOnWE_toggled(bool checked) {
    if (!runner_.isOpen()) return;
    runner_.vppOnWE(checked);
}

void MainWindow::on_checkBoxCE_toggled(bool checked) {
    if (!runner_.isOpen()) return;
    runner_.setCE(checked);
}

void MainWindow::on_checkBoxOE_toggled(bool checked) {
    if (!runner_.isOpen()) return;
    runner_.setOE(checked);
}

void MainWindow::on_checkBoxWE_toggled(bool checked) {
    if (!runner_.isOpen()) return;
    runner_.setWE(checked);
}

void MainWindow::on_dialVdd_valueChanged(int value) {
    if (!runner_.isOpen()) return;
    float v = value / 10.0f;
    runner_.vddSet(v);
}

void MainWindow::on_dialVpp_valueChanged(int value) {
    if (!runner_.isOpen()) return;
    float v = value / 10.0f;
    runner_.vppSet(v);
}

void MainWindow::onRefreshTimerTimeout() {
    if (!runner_.isOpen()) {
        // Connection lost
        QString port = ui_->comboBoxPort->currentText();
        connect_(false);
        enableDiagControls_(false);
        QMessageBox::critical(
            this, tr("USB Flash/EPROM Programmer"),
            tr("The device has been disconnected from the \"%1\" port.")
                .arg(port)
                .leftJustified(kDialogLabelMinLength));
        return;
    }

    auto originalLocale = std::locale::global(std::locale::classic());
    char buffer[5];
    float value = runner_.vddGet();
    if (value < 0.0f) {
        value = 0.0f;
    }
    snprintf(buffer, sizeof(buffer), "%4.1f", value);
    ui_->lcdNumberVdd->display(QString(buffer));
    value = runner_.vppGet();
    if (value < 0.0f) {
        value = 0.0f;
    }
    snprintf(buffer, sizeof(buffer), "%4.1f", value);
    ui_->lcdNumberVpp->display(QString(buffer));
    value = runner_.vddGetDuty();
    if (value < 0.0f) {
        value = 0.0f;
    }
    snprintf(buffer, sizeof(buffer), "%4.1f", value);
    ui_->lcdNumberVddDuty->display(QString(buffer));
    value = runner_.vppGetDuty();
    if (value < 0.0f) {
        value = 0.0f;
    }
    snprintf(buffer, sizeof(buffer), "%4.1f", value);
    ui_->lcdNumberVppDuty->display(QString(buffer));
    std::locale::global(originalLocale);
}

void MainWindow::onCheckBoxAddressToggled(bool checked) {
    addressBinToHex_();
}

void MainWindow::on_spinBoxAddr_valueChanged(int value) {
    addressHexToBin_();
    sendAddrBus_();
}

void MainWindow::onCheckBoxDataToggled(bool checked) {
    dataBinToHex_();
}

void MainWindow::on_spinBoxData_valueChanged(int value) {
    dataHexToBin_();
    sendDataBus_();
}

void MainWindow::connect_(bool state) {
    if (state) {
        if (runner_.open(ui_->comboBoxPort->currentText())) {
            enableDiagControls_();
            ui_->dialVdd->setValue(50);
            ui_->dialVpp->setValue(120);
            enumTimer_.stop();
            refreshTimer_.start(kUsbRefreshInterval);
        } else {
            QMessageBox::critical(this, tr("USB Flash/EPROM Programmer"),
                                  tr("Error opening the \"%1\" port.")
                                      .arg(ui_->comboBoxPort->currentText())
                                      .leftJustified(kDialogLabelMinLength));
        }
    }
    if (runner_.isOpen()) {
        runner_.vppCtrl(false);
        runner_.vddCtrl(false);
        runner_.vddOnVpp(false);
        runner_.vppOnA9(false);
        runner_.vppOnA18(false);
        runner_.vppOnCE(false);
        runner_.vppOnOE(false);
        runner_.vppOnWE(false);
        runner_.setCE(false);
        runner_.setOE(false);
        runner_.setWE(false);
    }
    if (!state) {
        refreshTimer_.stop();
        if (runner_.isOpen()) {
            QTimer::singleShot(500, this, [this] {
                runner_.close();
                enableDiagControls_(false);
            });
        }
        enumTimer_.start();
    }
}

void MainWindow::sendDataBus_() {
    if (!runner_.isOpen()) return;
    uint16_t data = ui_->spinBoxData->value();
    if (!runner_.dataSetW(data)) {
        QMessageBox::critical(
            this, tr("USB Flash/EPROM Programmer"),
            tr("The device has been disconnected from the \"%1\" port.")
                .arg(runner_.getPath())
                .leftJustified(kDialogLabelMinLength));
    }
}

void MainWindow::sendAddrBus_() {
    if (!runner_.isOpen()) return;
    uint32_t data = ui_->spinBoxAddr->value();
    runner_.addrSet(data);
}

void MainWindow::enableDiagControls_(bool state) {
    ui_->frameVdd->setEnabled(state);
    ui_->frameVpp->setEnabled(state);
    ui_->frameBusCtrl->setEnabled(state);
    ui_->frameBusAddr->setEnabled(state);
    ui_->frameBusData->setEnabled(state);
    ui_->comboBoxPort->setEnabled(!state);
    if (state) {
        ui_->pushButtonConnect->setText(tr("Disconnect"));
        ui_->lcdNumberVdd->setStyleSheet(
            "background-color:rgb(0,0,0);color:rgb(143,240,164);");
        ui_->lcdNumberVddDuty->setStyleSheet(
            "background-color:rgb(0,0,0);color:rgb(143,240,164);");
        ui_->lcdNumberVpp->setStyleSheet(
            "background-color:rgb(0,0,0);color:rgb(249,240,107);");
        ui_->lcdNumberVppDuty->setStyleSheet(
            "background-color:rgb(0,0,0);color:rgb(249,240,107);");
    } else {
        ui_->pushButtonConnect->setText(tr("Connect"));
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
        ui_->spinBoxAddr->setValue(0);
        ui_->spinBoxData->setValue(0);
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

void MainWindow::addressBinToHex_() {
    int value = (ui_->checkBoxA0->isChecked() ? 1 : 0) |
                (ui_->checkBoxA1->isChecked() ? 1 << 1 : 0) |
                (ui_->checkBoxA2->isChecked() ? 1 << 2 : 0) |
                (ui_->checkBoxA3->isChecked() ? 1 << 3 : 0) |
                (ui_->checkBoxA4->isChecked() ? 1 << 4 : 0) |
                (ui_->checkBoxA5->isChecked() ? 1 << 5 : 0) |
                (ui_->checkBoxA6->isChecked() ? 1 << 6 : 0) |
                (ui_->checkBoxA7->isChecked() ? 1 << 7 : 0) |
                (ui_->checkBoxA8->isChecked() ? 1 << 8 : 0) |
                (ui_->checkBoxA9->isChecked() ? 1 << 9 : 0) |
                (ui_->checkBoxA10->isChecked() ? 1 << 10 : 0) |
                (ui_->checkBoxA11->isChecked() ? 1 << 11 : 0) |
                (ui_->checkBoxA12->isChecked() ? 1 << 12 : 0) |
                (ui_->checkBoxA13->isChecked() ? 1 << 13 : 0) |
                (ui_->checkBoxA14->isChecked() ? 1 << 14 : 0) |
                (ui_->checkBoxA15->isChecked() ? 1 << 15 : 0) |
                (ui_->checkBoxA16->isChecked() ? 1 << 16 : 0) |
                (ui_->checkBoxA17->isChecked() ? 1 << 17 : 0) |
                (ui_->checkBoxA18->isChecked() ? 1 << 18 : 0) |
                (ui_->checkBoxA19->isChecked() ? 1 << 19 : 0) |
                (ui_->checkBoxA20->isChecked() ? 1 << 20 : 0) |
                (ui_->checkBoxA21->isChecked() ? 1 << 21 : 0) |
                (ui_->checkBoxA22->isChecked() ? 1 << 22 : 0) |
                (ui_->checkBoxA23->isChecked() ? 1 << 23 : 0);
    ui_->spinBoxAddr->setValue(value);
}

void MainWindow::addressHexToBin_() {
    int value = ui_->spinBoxAddr->value();
    ui_->checkBoxA0->setChecked(value & (1 << 0));
    ui_->checkBoxA1->setChecked(value & (1 << 1));
    ui_->checkBoxA2->setChecked(value & (1 << 2));
    ui_->checkBoxA3->setChecked(value & (1 << 3));
    ui_->checkBoxA4->setChecked(value & (1 << 4));
    ui_->checkBoxA5->setChecked(value & (1 << 5));
    ui_->checkBoxA6->setChecked(value & (1 << 6));
    ui_->checkBoxA7->setChecked(value & (1 << 7));
    ui_->checkBoxA8->setChecked(value & (1 << 8));
    ui_->checkBoxA9->setChecked(value & (1 << 9));
    ui_->checkBoxA10->setChecked(value & (1 << 10));
    ui_->checkBoxA11->setChecked(value & (1 << 11));
    ui_->checkBoxA12->setChecked(value & (1 << 12));
    ui_->checkBoxA13->setChecked(value & (1 << 13));
    ui_->checkBoxA14->setChecked(value & (1 << 14));
    ui_->checkBoxA15->setChecked(value & (1 << 15));
    ui_->checkBoxA16->setChecked(value & (1 << 16));
    ui_->checkBoxA17->setChecked(value & (1 << 17));
    ui_->checkBoxA18->setChecked(value & (1 << 18));
    ui_->checkBoxA19->setChecked(value & (1 << 19));
    ui_->checkBoxA20->setChecked(value & (1 << 20));
    ui_->checkBoxA21->setChecked(value & (1 << 21));
    ui_->checkBoxA22->setChecked(value & (1 << 22));
    ui_->checkBoxA23->setChecked(value & (1 << 23));
}

void MainWindow::dataBinToHex_() {
    int value = (ui_->checkBoxD0->isChecked() ? 1 : 0) |
                (ui_->checkBoxD1->isChecked() ? 1 << 1 : 0) |
                (ui_->checkBoxD2->isChecked() ? 1 << 2 : 0) |
                (ui_->checkBoxD3->isChecked() ? 1 << 3 : 0) |
                (ui_->checkBoxD4->isChecked() ? 1 << 4 : 0) |
                (ui_->checkBoxD5->isChecked() ? 1 << 5 : 0) |
                (ui_->checkBoxD6->isChecked() ? 1 << 6 : 0) |
                (ui_->checkBoxD7->isChecked() ? 1 << 7 : 0) |
                (ui_->checkBoxD8->isChecked() ? 1 << 8 : 0) |
                (ui_->checkBoxD9->isChecked() ? 1 << 9 : 0) |
                (ui_->checkBoxD10->isChecked() ? 1 << 10 : 0) |
                (ui_->checkBoxD11->isChecked() ? 1 << 11 : 0) |
                (ui_->checkBoxD12->isChecked() ? 1 << 12 : 0) |
                (ui_->checkBoxD13->isChecked() ? 1 << 13 : 0) |
                (ui_->checkBoxD14->isChecked() ? 1 << 14 : 0) |
                (ui_->checkBoxD15->isChecked() ? 1 << 15 : 0);
    ui_->spinBoxData->setValue(value);
}

void MainWindow::dataHexToBin_() {
    int value = ui_->spinBoxData->value();
    ui_->checkBoxD0->setChecked(value & (1 << 0));
    ui_->checkBoxD1->setChecked(value & (1 << 1));
    ui_->checkBoxD2->setChecked(value & (1 << 2));
    ui_->checkBoxD3->setChecked(value & (1 << 3));
    ui_->checkBoxD4->setChecked(value & (1 << 4));
    ui_->checkBoxD5->setChecked(value & (1 << 5));
    ui_->checkBoxD6->setChecked(value & (1 << 6));
    ui_->checkBoxD7->setChecked(value & (1 << 7));
    ui_->checkBoxD8->setChecked(value & (1 << 8));
    ui_->checkBoxD9->setChecked(value & (1 << 9));
    ui_->checkBoxD10->setChecked(value & (1 << 10));
    ui_->checkBoxD11->setChecked(value & (1 << 11));
    ui_->checkBoxD12->setChecked(value & (1 << 12));
    ui_->checkBoxD13->setChecked(value & (1 << 13));
    ui_->checkBoxD14->setChecked(value & (1 << 14));
    ui_->checkBoxD15->setChecked(value & (1 << 15));
}

// ---------------------------------------------------------------------------
// Buffer Editor

void MainWindow::on_actionOpen_triggered(bool checked) {
    if (hexeditor_->isChanged()) {
        if (!showDialogFileChanged_()) return;
    }
    // load last dir setting
    QSettings settings;
    TApplicationSettings app;
    app.lastDir = settings.value(kSettingGeneralLastDir).toString();

    QEpromFile::QEpromFileType type;
    QString filename = QFileDialog::getOpenFileName(
        this, tr("Open Binary File"), app.lastDir, getOpenDialogFilter_());

    if (filename.isEmpty()) return;

    // save last dir setting
    app.lastDir = QFileInfo(filename).absoluteFilePath();
    settings.setValue(kSettingGeneralLastDir, app.lastDir);

    if (!hexeditor_->open(filename)) {
        QMessageBox::critical(this, tr("USB Flash/EPROM Programmer"),
                              tr("Error reading file \"%1\".")
                                  .arg(filename)
                                  .leftJustified(kDialogLabelMinLength));
        return;
    }
    ui_->tabWidget->setCurrentWidget(ui_->tabBuffer);
    setWindowTitle(tr("USB Flash/EPROM Programmer") + " - " +
                   QFileInfo(filename).fileName());
}

void MainWindow::on_actionSave_triggered(bool checked) {
    if (!hexeditor_->save()) {
        QMessageBox::critical(this, tr("USB Flash/EPROM Programmer"),
                              tr("Error writing file \"%1\".")
                                  .arg(hexeditor_->filename())
                                  .leftJustified(kDialogLabelMinLength));
        return;
    }
    ui_->tabWidget->setCurrentWidget(ui_->tabBuffer);
    setWindowTitle(tr("USB Flash/EPROM Programmer") + " - " +
                   QFileInfo(hexeditor_->filename()).fileName());
}

void MainWindow::on_actionSaveAs_triggered(bool checked) {
    QString selectedFilter;
    QEpromFile::QEpromFileType type;

    // load last dir setting
    QSettings settings;
    TApplicationSettings app;
    app.lastDir = settings.value(kSettingGeneralLastDir).toString();

    QString filename =
        QFileDialog::getSaveFileName(this, tr("Save Binary File"), app.lastDir,
                                     getSaveDialogFilter_(), &selectedFilter);

    if (filename.isEmpty()) return;

    // save last dir setting
    app.lastDir = QFileInfo(filename).absoluteFilePath();
    settings.setValue(kSettingGeneralLastDir, app.lastDir);

    type = QEpromFile::typeFromStr(selectedFilter);
    if (!hexeditor_->saveAs(type, filename)) {
        QMessageBox::critical(this, tr("USB Flash/EPROM Programmer"),
                              tr("Error writing file \"%1\".")
                                  .arg(filename)
                                  .leftJustified(kDialogLabelMinLength));
        return;
    }
    ui_->tabWidget->setCurrentWidget(ui_->tabBuffer);
    setWindowTitle(tr("USB Flash/EPROM Programmer") + " - " +
                   QFileInfo(filename).fileName());
}

void MainWindow::on_actionFillFF_triggered(bool checked) {
    if (hexeditor_->isChanged()) {
        if (!showDialogFileChanged_()) return;
    }
    hexeditor_->fill(0xFF);
}

void MainWindow::on_actionFill00_triggered(bool checked) {
    if (hexeditor_->isChanged()) {
        if (!showDialogFileChanged_()) return;
    }
    hexeditor_->fill(0x00);
}

void MainWindow::on_actionFillRandom_triggered(bool checked) {
    if (hexeditor_->isChanged()) {
        if (!showDialogFileChanged_()) return;
    }
    hexeditor_->random();
}

void MainWindow::on_actionFind_triggered(bool checked) {
    hexeditor_->showFindDialog();
}

void MainWindow::on_actionReplace_triggered(bool checked) {
    hexeditor_->showReplaceDialog();
}

void MainWindow::on_btnOpen_clicked() {
    on_actionOpen_triggered();
}

void MainWindow::on_btnSave_clicked() {
    if (ui_->actionSave->isEnabled()) {
        on_actionSave_triggered();
    } else {
        on_actionSaveAs_triggered();
    }
}

void MainWindow::on_btnFind_clicked() {
    on_actionFind_triggered();
}

void MainWindow::on_btnReplace_clicked() {
    on_actionReplace_triggered();
}

void MainWindow::on_btnFillFF_clicked() {
    on_actionFillFF_triggered();
}

void MainWindow::on_btnFill00_clicked() {
    on_actionFill00_triggered();
}

void MainWindow::on_btnFillRandom_clicked() {
    on_actionFillRandom_triggered();
}

void MainWindow::onDataChanged(bool status) {
    QString title = tr("USB Flash/EPROM Programmer");
    if (!hexeditor_->filename().isEmpty()) {
        title += " - " + QFileInfo(hexeditor_->filename()).fileName();
        ui_->actionSave->setEnabled(status);
    }
    if (status) {
        title += "*";
    }
    setWindowTitle(title);
    updateCheckSum_();
}

void MainWindow::updateCheckSum_() {
    uint32_t sum16 = 0, crc32 = 0xFFFFFFFF;
    QByteArray data = hexeditor_->getData();
    for (const auto &d : data) {
        sum16 += d;
        sum16 &= 0xFFFF;
        crc32 = (crc32 >> 8) ^ kCRC32Table[(crc32 ^ d) & 0xFF];
    }
    crc32 ^= 0xFFFFFFFF;
    QString size;
    if (data.size() >= 1024 * 1024) {
        size = QString("%1 MB").arg(ceilf(data.size() / (1024.0f * 1024.0f)));
    } else if (data.size() >= 1024) {
        size = QString("%1 KB").arg(ceilf(data.size() / 1024.0f));
    } else {
        size = QString("%1 Bytes").arg(data.size());
    }
    checksumLabel_->setText(
        tr("Size: %1 | Checksum: 0x%2 [ADD16] | 0x%3 [CRC32]")
            .arg(size)
            .arg(QString("%1").arg(sum16, 4, 16, QChar('0')).toUpper())
            .arg(QString("%1").arg(crc32, 8, 16, QChar('0')).toUpper()));
}

void MainWindow::enableEditorControls_(bool state) {
    ui_->frameToolBar->setVisible(state);
    ui_->actionOpen->setEnabled(state);
    ui_->actionSave->setEnabled(state && !hexeditor_->filename().isEmpty() &&
                                hexeditor_->isChanged());
    ui_->actionSaveAs->setEnabled(state);
    ui_->actionFillFF->setEnabled(state);
    ui_->actionFill00->setEnabled(state);
    ui_->actionFillRandom->setEnabled(state);
    ui_->actionFind->setEnabled(state);
    ui_->actionReplace->setEnabled(state);
}

bool MainWindow::showDialogFileChanged_() {
    return QMessageBox::question(
               this, tr("USB Flash/EPROM Programmer"),
               tr("There is unsaved data in the editor.")
                       .leftJustified(kDialogLabelMinLength) +
                   "\n" +
                   tr("Are you sure you want to lose this data?")
                       .leftJustified(kDialogLabelMinLength)) ==
           QMessageBox::Yes;
}

QString MainWindow::getOpenDialogFilter_() {
    return MainWindow::tr("All Flash/EPROM Files") +
           " (*.bin *.rom *.s19 *.s28 *.s37 *.s *.s2 *.s3 *.hex *.eep);;" +
           getSaveDialogFilter_() + ";;" + MainWindow::tr("All Files") +
           " (*.*)";
}

QString MainWindow::getSaveDialogFilter_() {
    return MainWindow::tr("Binary Files") + " (*.bin *.rom);;" +
           MainWindow::tr("Motorola S-REC Files") +
           " (*.s19 *.s28 *.s37 *.s *.s2 *.s3);;" +
           MainWindow::tr("Intel Hex Files") + " (*.hex *.eep);;" +
           MainWindow::tr("Atmel Generic Files") + " (*.hex *.rom *.eep)";
}