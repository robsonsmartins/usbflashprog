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
 * @file test/hal/flash_test.cpp
 * @brief Implementation of Unit Test for Pico Flash Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <cstring>
#include <vector>

#include "flash_test.hpp"
#include "mock/hardware/flash.h"

// ---------------------------------------------------------------------------

bool FlashTest::compareData_(const uint8_t *a, const uint8_t *b, size_t len) {
    return !std::memcmp(a, b, len);
}

// ---------------------------------------------------------------------------

TEST_F(FlashTest, read_write_verify) {
    constexpr size_t BUFFER_SIZE = 1024;
    uint8_t *buffer = new uint8_t[BUFFER_SIZE]();
    std::vector<uint8_t> zero(PICO_FLASH_SIZE_BYTES, 0);
    Flash::read(buffer, BUFFER_SIZE);
    EXPECT_EQ(compareData_(zero.data(), buffer, BUFFER_SIZE), false);
    for (size_t i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = i & 0xFF;
    }
    EXPECT_EQ(Flash::verify(nullptr, 0), false);
    EXPECT_EQ(Flash::verify(buffer, BUFFER_SIZE), false);
    Flash::write(buffer, BUFFER_SIZE);
    EXPECT_EQ(Flash::verify(buffer, BUFFER_SIZE), true);
    Flash::read(buffer, BUFFER_SIZE);
    EXPECT_EQ(compareData_(zero.data(), buffer, BUFFER_SIZE), false);
    delete[] buffer;
}
