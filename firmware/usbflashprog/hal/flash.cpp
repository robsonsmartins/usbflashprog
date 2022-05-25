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
 * @file hal/flash.cpp
 * @brief Implementation of the Pico Flash Class.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include "hal/flash.hpp"

#include <cstring>

#include "hardware/flash.h"
#include "hardware/sync.h"
#include "pico/multicore.h"

// ---------------------------------------------------------------------------

void Flash::write(const uint8_t *data, size_t len) {
    size_t tmpLen = pageSize_(len);
    size_t eraseLen = sectorSize_(tmpLen);
    size_t offset = offset_(eraseLen);
    uint8_t *tmpBuf = new uint8_t[tmpLen]();
    std::memcpy(tmpBuf, data, len);
    write_(tmpBuf, offset, tmpLen);
    delete[] tmpBuf;
}

void Flash::read(uint8_t *data, size_t len) {
    size_t tmpLen = pageSize_(len);
    size_t eraseLen = sectorSize_(tmpLen);
    size_t offset = offset_(eraseLen);
    uint8_t *tmpBuf = new uint8_t[tmpLen]();
    read_(tmpBuf, offset, tmpLen);
    std::memcpy(data, tmpBuf, len);
    delete[] tmpBuf;
}

bool Flash::verify(const uint8_t *data, size_t len) {
    size_t tmpLen = pageSize_(len);
    size_t eraseLen = sectorSize_(tmpLen);
    size_t offset = offset_(eraseLen);
    uint8_t *tmpBuf = new uint8_t[tmpLen]();
    std::memcpy(tmpBuf, data, len);
    bool result = verify_(tmpBuf, offset, len);
    delete[] tmpBuf;
    return result;
}

void Flash::write_(const uint8_t *buf, size_t offset, size_t len) {
    if (!buf || !len || offset + len > PICO_FLASH_SIZE_BYTES) { return; }
    size_t eraseLen = sectorSize_(len);
    if (offset + eraseLen > PICO_FLASH_SIZE_BYTES) { return; }
    multicore_reset_core1();
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(offset, eraseLen);
    flash_range_program(offset, buf, len);
    restore_interrupts(ints);
}

void Flash::read_(uint8_t *buf, size_t offset, size_t len) {
    if (!buf || !len || offset + len > PICO_FLASH_SIZE_BYTES) { return; }
    const uint8_t *pFlash = reinterpret_cast<const uint8_t*>(XIP_BASE + offset);
    std::memcpy(buf, pFlash, len);
}

bool Flash::verify_(const uint8_t *buf, size_t offset, size_t len) {
    if (!buf || !len || offset + len > PICO_FLASH_SIZE_BYTES) {
        return false;
    }
    uint8_t *tmp = new uint8_t[len]();
    read_(tmp, offset, len);
    bool result = !std::memcmp(tmp, buf, len);
    delete[] tmp;
    return result;
}

size_t Flash::pageSize_(size_t len) {
    return (len + FLASH_PAGE_SIZE - 1) /
        FLASH_PAGE_SIZE * FLASH_PAGE_SIZE;
}

size_t Flash::sectorSize_(size_t len) {
    return (len + FLASH_SECTOR_SIZE - 1) /
        FLASH_SECTOR_SIZE * FLASH_SECTOR_SIZE;
}

size_t Flash::offset_(size_t len) {
    return PICO_FLASH_SIZE_BYTES - len;
}
