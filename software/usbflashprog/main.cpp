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
 * @file main.cpp
 * @brief Implementation of the Main Routine.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QLoggingCategory>
#include <QSettings>

#include <cstdlib>

#include "main.hpp"
#include "ui/mainwindow.hpp"

#ifdef Q_OS_WINDOWS
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/resource.h>
#endif

// ---------------------------------------------------------------------------
/**
 * @brief Main routine.
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 * @return Error code (zero if success).
 */
int main(int argc, char *argv[]);

/**
 * @brief Creates a QMainWindow object, according to the arguments
 *  passed on the command line.
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 * @return Pointer to the QMainWindow object.
 */
QMainWindow *createWindow(const int &argc, char *argv[]);

// ---------------------------------------------------------------------------
// Global

/* @brief Qt Base Translator. */
QTranslator *baseTranslator = nullptr;
/* @brief Custom Translator. */
QTranslator *customTranslator = nullptr;

// ---------------------------------------------------------------------------

QMainWindow *createWindow(const int &argc, char *argv[]) {
    (void)argc;
    (void)argv;
    return new MainWindow();
}

TApplicationSettings loadSettings() {
    TApplicationSettings result;
    QSettings settings;
    result.logLevel =
        settings.value(kSettingGeneralLogLevel).toString().toInt();
    result.language = settings.value(kSettingGeneralLanguage).toString();
    result.windowPos = settings.value(kSettingGeneralWindowPos).toPoint();
    result.windowSize = settings.value(kSettingGeneralWindowSize).toSize();
    return result;
}

void initLogging(int level) {
    // logging
    QString rules;
    QString strType;
    switch (level) {
        case 1:  // Fatal
            rules = "*.critical=false";
            strType = "FATAL";
            break;
        case 2:  // Critical
            rules = "*.warning=false";
            strType = "CRITICAL";
            break;
        case 3:  // Warning
            rules = "*.info=false";
            strType = "WARNING";
            break;
        case 4:  // Info
            rules = "*.debug=false";
            strType = "INFO";
            break;
        case 5:  // Debug
        default:
            if (level >= 5) {
                rules =
                    "*.debug=false\nbackend.*.debug=true\ndevice.*.debug=true";
                strType = "DEBUG";
            }
            break;
    }
    QFile logFile(QDir::homePath() + "/" + QString(kLogFileName));
    logFile.remove();
    // Disabled
    if (level <= 0) return;
    QLoggingCategory::setFilterRules(rules);
    qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &context,
                              const QString &msg) {
        QString localMsg = QString(msg.toLocal8Bit());
        localMsg.remove('"');
        QString strType;
        switch (type) {
            case QtFatalMsg:
                strType = "FATAL";
                break;
            case QtCriticalMsg:
                strType = "CRITICAL";
                break;
            case QtWarningMsg:
                strType = "WARNING";
                break;
            case QtInfoMsg:
                strType = "INFO";
                break;
            case QtDebugMsg:
            default:
                strType = "DEBUG";
                break;
        }
        QFile logFile(QDir::homePath() + "/" + QString(kLogFileName));
        logFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&logFile);
        QString timeStamp =
            QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
        ts << QString("%1 %2 %3 - %4")
                  .arg(timeStamp)
                  .arg("[" + QString(context.category) + "]", -25)
                  .arg(strType, -8)
                  .arg(localMsg)
           << endl;
    });
    qInfo() << "Setting Log Level:" << strType;
}

void loadLanguage(const QString &language, QApplication *app) {
    // remove current translators
    if (customTranslator) {
        app->removeTranslator(customTranslator);
        delete customTranslator;
    }
    if (baseTranslator) {
        app->removeTranslator(baseTranslator);
        delete baseTranslator;
    }
    customTranslator = new QTranslator(app);
    baseTranslator = new QTranslator(app);
    bool baseOk = false, customOk = false;
    QString localeName = language;
    if (localeName.isEmpty()) {
        // autodetect
        const QStringList uiLanguages = QLocale::system().uiLanguages();
        // translate Qt base strings
        for (const QString &locale : uiLanguages) {
            localeName = QLocale(locale).name();
            const QString baseName = "qtbase_" + localeName;
            if (baseTranslator->load(
                    baseName,
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
                baseOk = true;
                break;
            }
        }
        // translate application custom strings
        for (const QString &locale : uiLanguages) {
            localeName = QLocale(locale).name();
            const QString customName = "ufprog_" + localeName;
            if (customTranslator->load(":/i18n/" + customName)) {
                customOk = true;
                break;
            }
        }
    } else {
        // load specific language
        // translate Qt base strings
        const QString baseName = "qtbase_" + localeName;
        if (baseTranslator->load(
                baseName,
                QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
            baseOk = true;
        }
        // translate application custom strings
        const QString customName = "ufprog_" + localeName;
        if (customTranslator->load(":/i18n/" + customName)) {
            customOk = true;
        }
    }
    if (baseOk) {
        app->installTranslator(baseTranslator);
        qInfo() << "Installed Qt Base translator for language:" << localeName;
    }
    if (customOk) {
        app->installTranslator(customTranslator);
        qInfo() << "Installed Custom translator for language:" << localeName;
    }
}

int main(int argc, char *argv[]) {
#ifdef Q_OS_LINUX
    setenv("XDG_SESSION_TYPE", "x11", 1);
#endif
    // elevates priority of process
#ifdef Q_OS_WINDOWS
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
#else
    // only if running as superuser
    setpriority(PRIO_PROCESS, 0, -20);
#endif
    // create app
    QApplication app(argc, argv);
    app.setOrganizationName(kOrganizationName);
    app.setOrganizationDomain(kOrganizationDomain);
    app.setApplicationName(kApplicationName);
    // load settings
    TApplicationSettings settings = loadSettings();
    // setup the logger
    initLogging(settings.logLevel);
    // setup translator
    loadLanguage(settings.language, &app);
    // main window
    QScopedPointer<QMainWindow> window(createWindow(argc, argv));
    window->show();
    return app.exec();
}
