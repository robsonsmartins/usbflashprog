// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @defgroup emu 4. Emulator Software Project
 * @brief    Software project for Programmer Emulator.
 */
// ---------------------------------------------------------------------------
/** 
 * @ingroup emu
 * @file main.cpp
 * @brief Implementation of the Main Routine.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <QApplication>
#include "main/mainwindow.hpp"

// ---------------------------------------------------------------------------

/**
 * @ingroup emu
 * @brief Main routine.
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 * @return Error code (zero if success).
 */
int main(int argc, char *argv[]);

// ---------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();
    w->show();
    return a.exec();
}
