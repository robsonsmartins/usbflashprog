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
 * @file main/mainwindow.hpp
 * @brief Header of the Main Window Class.
 *  
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef MAIN_MAINWINDOW_HPP_
#define MAIN_MAINWINDOW_HPP_

#include <QMainWindow>
#include <QScreen>

// ---------------------------------------------------------------------------

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// ---------------------------------------------------------------------------

/**
 * @ingroup emu
 * @brief Main Window GUI Class
 * @details The Main Window of Application.
 * @nosubgrouping
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

 public:
    /**
     * @brief Constructor.
     * @param parent Pointer to parent object. Default is nullptr.
     */
    explicit MainWindow(QWidget *parent = nullptr);
    /** @brief Destructor. */
    ~MainWindow();
    /**
     * @brief Gets a pointer to the primary screen.
     * @return Pointer to the primary screen object.
     */
    QScreen *screen() const;

 private slots:
    /* auto slots */
    void on_cbProg_activated(int index);
    void on_cbChip_activated(int index);
    void on_cbSize_activated(int index);

 private:
    /* @brief Pointer to UI object. */
    Ui::MainWindow *ui_;
    /* @brief Loads registry settings. */
    void loadSettings_();
};

#endif  // MAIN_MAINWINDOW_HPP_
