// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Firmware
 * @file hal/multicore.hpp
 * @brief Header of the Pico Multi Core Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef HAL_MULTICORE_HPP_
#define HAL_MULTICORE_HPP_

#include "pico/stdlib.h"

// forward
class MultiCore;

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Pico Multi Core Class
 * @details The purpose of this class is to allow processing on the
 *   second CPU core on the board.
 * @nosubgrouping
 */
class MultiCore {
  public:
    /**
     * @brief Entry point to second core routine.
     * @details A valid routine to be used as an entry point for the second CPU
     *  core must be of this type.<br/>
     *  <p><b>For example</b>:<br/>
     *  <code>
     *   void second_core_my_entry(MultiCore& core) {
     *     ...
     *   }
     *  </code></p>
     */
    typedef void (*MultiCoreEntry)(MultiCore &);
    /**
     * @brief Defines possible values for CPU core status.
     */
    enum CoreStatus {
        /** @brief Core is stopped. */
        csStopped,
        /** @brief Core is running. */
        csRunning,
        /** @brief Core is starting (initialization). */
        csStarting,
        /** @brief Core is stopping (finalization). */
        csStopping
    };
    /**
     * @brief Constructor.
     * @param entry Entry point (function) for second CPU core.
     */
    explicit MultiCore(MultiCoreEntry entry);
    /** @brief Destructor. */
    ~MultiCore();
    /** @brief Runs the entry point routine into second CPU core. */
    void launch();
    /** @brief Stops the execution of second CPU core. */
    void stop();
    /**
     * @brief Return if second CPU core is running.
     * @return True if second CPU core is running, false otherwise.
     */
    bool isRunning() const;
    /**
     * @brief Returns if the second CPU core has been requested to stop.
     * @return True if second CPU core is stopping, false otherwise.
     */
    bool isStopRequested() const;
    /**
     * @brief Gets the status of second CPU core.
     * @return Status value. One of CoreStatus values.
     */
    CoreStatus getStatus() const;
    /**
     * @brief Sends a 32-bit parameter to second core routine.
     * @details Sends a parameter to second CPU core routine.<br/>
     *  Blocks the caller (on first core) until the second core processes
     *  this parameter through getParam().
     * @param src Value to send.
     */
    void putParam(uintptr_t src);
    /**
     * @brief Receives a 32-bit parameter sent from first core routine.
     * @details Receives a parameter sent from first CPU core routine.<br/>
     *  Blocks the caller (on second core) until the first core sends
     *  one parameter through putParam(uint32_t).
     * @return Value received.
     */
    uintptr_t getParam();
    /**
     * @brief Locks execution line (using mutex).
     * @details Protects code of the simultaneous write access (and race
     * conditions).
     */
    void lock();
    /**
     * @brief Unlocks execution line (using mutex).
     * @details Protects code of the simultaneous write access (and race
     * conditions).
     */
    void unlock();
    /**
     * @brief Sleeps the execution of current CPU core for a number of
     * microseconds.
     * @param us Number of microseconds to sleep.
     */
    static void usleep(uint64_t us);
    /**
     * @brief Sleeps the execution of current CPU core for a number of
     * milliseconds.
     * @param ms Number of milliseconds to sleep.
     */
    static void msleep(uint32_t ms);

  private:
    /*
     * @brief Entry point (pointer to function) of the second CPU
     *  core routine.
     */
    MultiCoreEntry entry_;
    /* @brief Current second core status flag. */
    CoreStatus status_;
    /* @brief Pointer to mutex object (for synchronization). */
    void *mutex_;

    /** @cond */
    /*
     * @brief Internal function used as entry point for the
     *  second core routine.
     */
    friend void multicore_core_1_entry_();
    /** @endcond */
};

#endif  // HAL_MULTICORE_HPP_
