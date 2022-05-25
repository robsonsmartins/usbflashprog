// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------

#ifndef TEST_MOCK_PICO_MULTICORE_H_
#define TEST_MOCK_PICO_MULTICORE_H_

#include <thread> // NOLINT
#include <chrono> // NOLINT
#include <mutex> // NOLINT
#include <queue>

#include "pico/stdlib.h"

// ---------------------------------------------------------------------------

static std::thread *_thread = nullptr;
static std::queue<uintptr_t> _fifo[2];
static std::thread::id _id[2];
static std::mutex _mutex;
static bool _stopreq = false;

typedef void (*TThreadEntryPoint)();

static void _internal_entry_point(TThreadEntryPoint entry);

// ---------------------------------------------------------------------------

extern "C" inline void multicore_launch_core1(TThreadEntryPoint entry) {
    if (_thread) { return; }
    _id[0] = std::this_thread::get_id();
    _thread = new std::thread(_internal_entry_point, entry);
    _id[1] = _thread->get_id();
    _stopreq = false;
}

extern "C" inline void multicore_fifo_push_blocking(uintptr_t data) {
    uint index = (std::this_thread::get_id() == _id[0]) ? 1 : 0;
    _mutex.lock();
    _fifo[index].push(data);
    _mutex.unlock();
}

extern "C" inline uintptr_t multicore_fifo_pop_blocking() {
    uint index = (std::this_thread::get_id() == _id[0]) ? 0 : 1;
    uintptr_t data = 0;
    while (_fifo[index].empty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (_stopreq) { return 0; }
    }
    _mutex.lock();
    data = _fifo[index].front();
    _fifo[index].pop();
    _mutex.unlock();
    return data;
}

extern "C" inline void multicore_reset_core1() {
    if (!_thread) { return; }
    if (_thread->joinable()) {
        _stopreq = true;
        _thread->join();
        _stopreq = false;
        delete _thread;
        _thread = nullptr;
    }
}

// ---------------------------------------------------------------------------

static void _internal_entry_point(TThreadEntryPoint entry) {
    entry();
}

#endif  // TEST_MOCK_PICO_MULTICORE_H_
