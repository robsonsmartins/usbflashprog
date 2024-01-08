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
 * @file hal/flash.hpp
 * @brief Header of the Pico Flash Class.
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#ifndef HAL_FLASH_HPP_
#define HAL_FLASH_HPP_

#include "pico/stdlib.h"

// ---------------------------------------------------------------------------

/**
 * @ingroup Firmware
 * @brief Pico Flash Class
 * @details The purpose of this static class is to manipulate the Flash space
 *   on board.
 * @nosubgrouping
 */
class Flash {
  public:
    /**
     * @brief Writes data to the flash space.
     * @details Writes data to the end of the program's flash memory space.<br/>
     *   <b>WARNINGS</b>:
     *   <ul>
     *     <li>The second CPU core is stopped for this operation!</li>
     *     <li>All data on the sector (flash sector size) will be erased
     * first!</li> <li>If the buffer size is greater than the available free
     * space (total size of flash memory minus program size), the program will
     * be permanently corrupted at runtime!</li>
     *   </ul>
     * @param data Pointer to a buffer to write.
     * @param len Size of buffer, in bytes.
     */
    static void write(const uint8_t *data, size_t len);
    /**
     * @brief Reads data from the flash space.
     * @details Reads data from the end of the program's flash memory space.
     * @param data Pointer to a buffer to receive data.
     * @param len Size of buffer, in bytes.
     */
    static void read(uint8_t *data, size_t len);
    /**
     * @brief Verifies data written to the flash space.
     * @details Reads data from the end of the program's flash memory space,
     *   and compares with the buffer.
     * @param data Pointer to a buffer to compare.
     * @param len Size of buffer, in bytes.
     * @return True if the flash data is equal the buffer. False otherwise.
     */
    static bool verify(const uint8_t *data, size_t len);

  private:
    /*
     * @brief Writes data to the flash space.
     * @param buf Pointer to a buffer to write.
     * @param offset Offset at the flash space.
     * @param len Size of buffer, in bytes.
     */
    static void write_(const uint8_t *buf, size_t offset, size_t len);
    /*
     * @brief Reads data from the flash space.
     * @param buf Pointer to a buffer to receive data.
     * @param offset Offset at the flash space.
     * @param len Size of buffer, in bytes.
     */
    static void read_(uint8_t *buf, size_t offset, size_t len);
    /*
     * @brief Verifies data written to the flash space.
     * @param buf Pointer to a buffer to compare.
     * @param offset Offset at the flash space.
     * @param len Size of buffer, in bytes.
     * @return True if the flash data is equal the buffer. False otherwise.
     */
    static bool verify_(const uint8_t *buf, size_t offset, size_t len);
    /*
     * @brief Returns the page size.
     * @param len Length of requested operation, in bytes.
     * @return Page Size, in bytes.
     */
    static size_t pageSize_(size_t len);
    /*
     * @brief Returns the sector size.
     * @param len Length of requested operation, in bytes.
     * @return Sector Size, in bytes.
     */
    static size_t sectorSize_(size_t len);
    /*
     * @brief Returns the offset from start.
     * @param len Length of requested operation, in bytes.
     * @return Offset, in bytes.
     */
    static size_t offset_(size_t len);
};

#endif  // HAL_FLASH_HPP_
