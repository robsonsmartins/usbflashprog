// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------

#ifndef TEST_MOCK_PICO_STDLIB_H_
#define TEST_MOCK_PICO_STDLIB_H_

#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <iostream>

// ---------------------------------------------------------------------------

#ifdef REAL_MOCK_IMPLEMENTATION
    static void reset_terminal_mode();
    static void set_conio_terminal_mode();
    static int kbhit(uint32_t timeout_us);
    static int getch();
#endif  // REAL_MOCK_IMPLEMENTATION

// ---------------------------------------------------------------------------

typedef unsigned int uint;

#define __not_in_flash(group) __attribute__((section(".time_critical." group)))

#define __not_in_flash_func(func_name) \
    __not_in_flash(__STRING(func_name)) func_name

// ---------------------------------------------------------------------------

#define PICO_DEFAULT_LED_PIN 25
#define PICO_ERROR_TIMEOUT -1

// ---------------------------------------------------------------------------

#ifdef REAL_MOCK_IMPLEMENTATION
    static struct termios _orig_termios;
#else
    constexpr char kStdioMockPredefinedChar = 'A';
#endif  // REAL_MOCK_IMPLEMENTATION

// ---------------------------------------------------------------------------

extern "C" inline void sleep_us(uint64_t us) {
    usleep(us);
}

extern "C" inline void sleep_ms(uint32_t ms) {
    usleep(static_cast<uint64_t>(ms) * 1000);
}

extern "C" inline void stdio_init_all(void) {
#ifdef REAL_MOCK_IMPLEMENTATION
    set_conio_terminal_mode();
#endif  // REAL_MOCK_IMPLEMENTATION
}

extern "C" inline int getchar_timeout_us(uint32_t timeout_us) {
#ifdef REAL_MOCK_IMPLEMENTATION
    if (!kbhit(timeout_us)) { return PICO_ERROR_TIMEOUT; }
    return getch();
#else
    return (timeout_us ? kStdioMockPredefinedChar : PICO_ERROR_TIMEOUT);
#endif  // REAL_MOCK_IMPLEMENTATION
}

extern "C" inline int putchar_raw(int c) {
#ifdef REAL_MOCK_IMPLEMENTATION
    std::cout << static_cast<char>(c);
#endif  // REAL_MOCK_IMPLEMENTATION
    return c;
}

// ---------------------------------------------------------------------------

#ifdef REAL_MOCK_IMPLEMENTATION

void reset_terminal_mode() {
    tcsetattr(0, TCSANOW, &_orig_termios);
}

void set_conio_terminal_mode() {
    struct termios new_termios;
    tcgetattr(0, &_orig_termios);
    memcpy(&new_termios, &_orig_termios, sizeof(new_termios));
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit(uint32_t timeout_us) {
    struct timeval tv = { 0L, timeout_us };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

int getch() {
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

#endif  // REAL_MOCK_IMPLEMENTATION

#endif  // TEST_MOCK_PICO_STDLIB_H_
