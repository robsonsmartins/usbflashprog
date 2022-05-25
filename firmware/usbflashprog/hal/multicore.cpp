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
 * @file hal/multicore.cpp
 * @brief Implementation of the Pico Multi Core Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "hal/multicore.hpp"

#include "pico/multicore.h"
#include "pico/mutex.h"

void multicore_core_1_entry_(void);

// ---------------------------------------------------------------------------

MultiCore::MultiCore(MultiCoreEntry entry):
                        entry_(entry), status_(csStopped) {
    mutex_t *mutex = new mutex_t();
    mutex_init(mutex);
    mutex_ = reinterpret_cast<void*>(mutex);
}

MultiCore::~MultiCore() {
    stop();
    mutex_t *mutex = reinterpret_cast<mutex_t*>(mutex_);
    delete mutex;
}

void MultiCore::launch() {
    if (status_ != csStopped) { return; }
    status_ = csStarting;
    multicore_launch_core1(multicore_core_1_entry_);
    multicore_fifo_push_blocking(reinterpret_cast<uintptr_t>(this));
    while (status_ == csStarting) { usleep(1); }
}

void MultiCore::stop() {
    if (status_ != csRunning) { return; }
    lock();
    status_ = csStopping;
    unlock();
    multicore_reset_core1();
#ifdef __arm__
    lock();
    status_ = csStopped;
    unlock();
#endif
}

bool MultiCore::isRunning() const {
    return (status_ != csStopped);
}

bool MultiCore::isStopRequested() const {
    return (status_ == csStopping);
}

MultiCore::CoreStatus MultiCore::getStatus() const {
    return status_;
}

void MultiCore::putParam(uintptr_t src) {
    if (status_ != csRunning) { return; }
    multicore_fifo_push_blocking(src);
}

uintptr_t MultiCore::getParam() {
    if (status_ != csRunning) { return 0; }
    return multicore_fifo_pop_blocking();
}

void MultiCore::lock() {
    mutex_t *mutex = reinterpret_cast<mutex_t*>(mutex_);
    mutex_enter_blocking(mutex);
}

void MultiCore::unlock() {
    mutex_t *mutex = reinterpret_cast<mutex_t*>(mutex_);
    mutex_exit(mutex);
}

void MultiCore::usleep(uint64_t us) {
    sleep_us(us);
}

void MultiCore::msleep(uint32_t ms) {
    sleep_ms(ms);
}

// ---------------------------------------------------------------------------

void multicore_core_1_entry_(void) {
    uintptr_t p = multicore_fifo_pop_blocking();
    MultiCore *core = reinterpret_cast<MultiCore*>(p);
    core->lock();
    core->status_ = MultiCore::csRunning;
    core->unlock();
    core->entry_(*core);
    core->lock();
    core->status_ = MultiCore::csStopped;
    core->unlock();
}
