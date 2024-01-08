// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------

#ifndef TEST_MOCK_PICO_MUTEX_H_
#define TEST_MOCK_PICO_MUTEX_H_

#include <mutex>  // NOLINT

#include "pico/stdlib.h"

// ---------------------------------------------------------------------------

typedef std::mutex mutex_t;

// ---------------------------------------------------------------------------

extern "C" inline void mutex_init(mutex_t *mtx) {}

extern "C" inline void mutex_enter_blocking(mutex_t *mtx) {
    mtx->lock();
}

extern "C" inline void mutex_exit(mutex_t *mtx) {
    mtx->unlock();
}

#endif  // TEST_MOCK_PICO_MUTEX_H_
