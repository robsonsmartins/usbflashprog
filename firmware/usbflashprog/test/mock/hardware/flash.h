// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------

#ifndef TEST_MOCK_HARDWARE_FLASH_H_
#define TEST_MOCK_HARDWARE_FLASH_H_

#include <vector>
#include "pico/stdlib.h"

// ---------------------------------------------------------------------------

#define PICO_FLASH_SIZE_BYTES (2 * 1024 * 1024)
#define FLASH_PAGE_SIZE (1u << 8)
#define FLASH_SECTOR_SIZE (1u << 12)

static std::vector<uint8_t> flashData_(PICO_FLASH_SIZE_BYTES, 0xFF);

#define XIP_BASE (reinterpret_cast<uintptr_t>(flashData_.data()))

extern "C" inline void flash_range_erase(uint32_t flash_offs, size_t count) {
    if (flash_offs >= flashData_.size() || !count) {
        return;
    }
    for (size_t i = flash_offs; i < count && i < flashData_.size(); i++) {
        flashData_[i] = 0;
    }
}

extern "C" inline void flash_range_program(uint32_t flash_offs,
                                           const uint8_t *data, size_t count) {
    if (!count || !data || flash_offs >= flashData_.size()) {
        return;
    }
    size_t size = (flash_offs + count - 1) >= flashData_.size()
                      ? flashData_.size() - flash_offs
                      : count;
    for (size_t i = 0; i < size; i++) {
        flashData_[i + flash_offs] = data[i];
    }
}

#endif  // TEST_MOCK_HARDWARE_FLASH_H_
