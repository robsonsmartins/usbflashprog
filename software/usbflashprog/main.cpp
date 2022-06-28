// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @defgroup Software Software Project
 * @brief    Software project for USB EPROM/Flash Programmer.
 */
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
#include <QLocale>
#include <QTranslator>
#include <cstdlib>

#include "main/mainwindow.hpp"
#include "diag/diagwindow.hpp"
#include "config.hpp"

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
QMainWindow* createWindow(const int &argc, char *argv[]);

// ---------------------------------------------------------------------------

QMainWindow* createWindow(const int &argc, char *argv[]) {
    /*
    for (int i = 1; i < argc; ++i) {
        if (!qstrcmp(argv[i], "--diag")) {
            return new DiagWindow();
        }
    }
    return new MainWindow();
    */
    return new DiagWindow();
}

int main(int argc, char *argv[]) {
#ifdef Q_OS_LINUX
    setenv("XDG_SESSION_TYPE", "x11", 1);
#endif
    QApplication a(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ufprog_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    QScopedPointer<QMainWindow> w(createWindow(argc, argv));
    w->show();
    return a.exec();
}
