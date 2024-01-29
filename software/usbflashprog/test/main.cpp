// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup UnitTests
 * @file test/main.cpp
 * @brief Implementation of the Unit Tests Main Routine.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QTimer>
#include <QString>
#include <QLoggingCategory>

#include "main.hpp"

// ---------------------------------------------------------------------------

/* GTest macro to print to console */
#define GTEST_COUT std::cerr << "[          ] "

// ---------------------------------------------------------------------------

/**
 * @ingroup UnitTests
 * @brief Initialize logger.
 */
void initLogging() {
    // logging
    uint8_t level = kTestLogLevel;
    if (level == 0) return;
    QString rules;
    switch (level) {
        case 1:  // Fatal
            rules = "*.critical=false";
            break;
        case 2:  // Critical
            rules = "*.warning=false";
            break;
        case 3:  // Warning
            rules = "*.info=false";
            break;
        case 4:  // Info
            rules = "*.debug=false";
            break;
        case 5:  // Debug
        default:
            if (level >= 5) {
                rules = "*.debug=true";
            }
            break;
    }
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
        GTEST_COUT << QString("%1 %2 - %3")
                          .arg("[" + QString(context.category) + "]", -25)
                          .arg(strType, -8)
                          .arg(localMsg)
                          .toStdString()
                   << std::endl;
    });
}

/**
 * @ingroup UnitTests
 * @brief Main routine for tests.
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 * @return Error code (zero if success).
 */
int main(int argc, char **argv) {
    QCoreApplication app{argc, argv};
    // logging
    initLogging();
    QTimer::singleShot(0, [&]() {
        ::testing::InitGoogleTest(&argc, argv);
        auto testResult = RUN_ALL_TESTS();
        app.exit(testResult);
    });
    return app.exec();
}
