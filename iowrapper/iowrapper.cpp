// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup lib_iowrapper
 * @file    iowrapper.cpp
 * @brief   Implementation file (C++) for IO.DLL Wrapper
 *
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <string>
#include <cstdio>
#include <cstdlib>

#include "iowrapper.hpp"

// ---------------------------------------------------------------------------

/* internal */
/* prog lib config: registry key */
#define PROGLIB_REGISTRY_KEY "Software\\EmuProg"
/* prog lib config: registry name */
#define PROGLIB_REGISTRY_NAME "ProgLib"
/* prog lib: default lib name */
#define PROGLIB_DEFAULT "willem.dll"

// ---------------------------------------------------------------------------
/* types */

/* SetPort function imported from Prog Lib */
typedef DLLIMPORT void (*PSetPort)(uint16_t addr,  // NOLINT
                                   uint8_t data);
/* GetPort function imported from Prog Lib */
typedef DLLIMPORT uint8_t (*PGetPort)(uint16_t addr);  // NOLINT

// ---------------------------------------------------------------------------
/* global vars */

/* Pointer for SetPort Prog Lib function */
static PSetPort SetPort = NULL;
/* Pointer for GetPort Prog Lib function */
static PGetPort GetPort = NULL;
/* Prog Lib Handle */
static HMODULE hProgLib = NULL;
/* Prog Lib name */
static std::string sProgLibName = "";

// ---------------------------------------------------------------------------
/* functions */

static bool _Load(void);
static void _Unload(void);

// ---------------------------------------------------------------------------
/* implementation */

DLLEXPORT void PortOut(short int Port, char Data) {  // NOLINT
    /* initialize objects */
    if (!_Load()) return;
    /* set port data */
    SetPort((uint16_t)Port, (uint8_t)Data);  // NOLINT
}

DLLEXPORT void PortWordOut(short int /*Port*/, short int /*Data*/) {  // NOLINT
    /* not used */
}

DLLEXPORT void PortDWordOut(short int /*Port*/, int /*Data*/) {  // NOLINT
                                                                 /* not used */
}

DLLEXPORT char PortIn(short int Port) {  // NOLINT
    /* initialize objects */
    if (!_Load()) return static_cast<char>(0xFF);
    /* get port data */
    return static_cast<char>(GetPort(static_cast<uint16_t>(Port)));  // NOLINT
}

DLLEXPORT short int PortWordIn(short int /*Port*/) {  // NOLINT
    /* not used */
    return 0xFFFF;
}

DLLEXPORT int PortDWordIn(short int /*Port*/) {  // NOLINT
    /* not used */
    return 0xFFFFFFFF;
}

DLLEXPORT void SetPortBit(short int Port, char Bit) {  // NOLINT
    /* initialize objects */
    if (!_Load()) return;
    /* get port data */
    uint8_t tmp = GetPort(static_cast<uint16_t>(Port));  // NOLINT
    /* calculate new data */
    tmp |= (0x01 << Bit);
    /* set port data */
    SetPort(static_cast<uint16_t>(Port), tmp);  // NOLINT
}

DLLEXPORT void ClrPortBit(short int Port, char Bit) {  // NOLINT
    /* initialize objects */
    if (!_Load()) return;
    /* get port data */
    uint8_t tmp = GetPort(static_cast<uint16_t>(Port));  // NOLINT
    /* calculate new data */
    tmp &= ~(0x01 << Bit);
    /* set port data */
    SetPort(static_cast<uint16_t>(Port), tmp);  // NOLINT
}

DLLEXPORT void NotPortBit(short int Port, char Bit) {  // NOLINT
    /* initialize objects */
    if (!_Load()) return;
    /* get port data */
    uint8_t tmp = GetPort(static_cast<uint16_t>(Port));  // NOLINT
    /* calculate new data */
    tmp ^= (0x01 << Bit);
    /* set port data */
    SetPort(static_cast<uint16_t>(Port), tmp);  // NOLINT
}

DLLEXPORT short int GetPortBit(short int Port, char Bit) {  // NOLINT
    /* initialize objects */
    if (!_Load()) return 1;
    /* get port data */
    uint8_t tmp = GetPort(static_cast<uint16_t>(Port));  // NOLINT
    /* get bit info */
    return static_cast<short int>((tmp & (0x01 << Bit)) != 0);  // NOLINT
}

DLLEXPORT short int RightPortShift(short int Port, short int Val) {  // NOLINT
    /* initialize objects */
    if (!_Load()) return 1;
    /* get port data */
    uint8_t tmp = GetPort(static_cast<uint16_t>(Port));  // NOLINT
    /* calculate new port byte */
    tmp >>= Val;
    /* set port data */
    SetPort(static_cast<uint16_t>(Port), tmp);  // NOLINT
    /* return */
    return static_cast<short int>(tmp);  // NOLINT
}

DLLEXPORT short int LeftPortShift(short int Port, short int Val) {  // NOLINT
    /* initialize objects */
    if (!_Load()) return 1;
    /* get port data */
    uint8_t tmp = GetPort(static_cast<uint16_t>(Port));  // NOLINT
    /* calculate new port byte */
    tmp <<= Val;
    /* set port data */
    SetPort(static_cast<uint16_t>(Port), tmp);  // NOLINT
    /* return */
    return static_cast<short int>(tmp);  // NOLINT
}

DLLEXPORT short int IsDriverInstalled(void) {  // NOLINT
    /* return always non-zero value */
    return 1;
}

/* DLL entry point for MSWindows */
BOOL WINAPI DllMain(HINSTANCE hInst,   /* Library instance handle. */
                    DWORD reason,      /* Reason this is being called. */
                    LPVOID reserved) { /* Not used. */
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            _Load();
            break;
        case DLL_PROCESS_DETACH:
            _Unload();
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
    /* Returns TRUE on success, FALSE on failure */
    return TRUE;
}

// ---------------------------------------------------------------------------

bool _Load(void) {
    HKEY hKey;
    char strLibName[4096];
    DWORD dwBufLen = static_cast<DWORD>(sizeof(strLibName));
    memset(strLibName, 0, dwBufLen);
    /* default lib name */
    strncpy(strLibName, PROGLIB_DEFAULT, dwBufLen);
    /* read registry value: ProgLib */
    /* open the reg key */
    if (RegCreateKeyEx(HKEY_CURRENT_USER, PROGLIB_REGISTRY_KEY, 0, NULL, 0,
                       KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        /* get the value */
        if (RegQueryValueExA(hKey, PROGLIB_REGISTRY_NAME, NULL, NULL,
                             reinterpret_cast<LPBYTE>(strLibName),
                             &dwBufLen) != ERROR_SUCCESS) {
            /* if error, then creates the default value in registry */
            RegSetValueEx(hKey, PROGLIB_REGISTRY_NAME, 0, REG_SZ,
                          reinterpret_cast<const BYTE*>(strLibName), dwBufLen);
        }
        RegCloseKey(hKey);
    }
    /* check for load library */
    /* library is loaded, exit */
    if (sProgLibName == strLibName && hProgLib) {
        return true;
    }
    /* unload if loaded */
    _Unload();
    /* load library */
    hProgLib = LoadLibraryA(strLibName);
    /* get func pointers */
    if (hProgLib) {
        SetPort =
            reinterpret_cast<PSetPort>(GetProcAddress(hProgLib, "SetPort"));
        GetPort =
            reinterpret_cast<PGetPort>(GetProcAddress(hProgLib, "GetPort"));
    }
    /* if error, unload */
    if (!SetPort || !GetPort) {
        _Unload();
    }
    /* return */
    sProgLibName = strLibName;
    return (hProgLib);
}

void _Unload(void) {
    /* if loaded, unload */
    if (hProgLib != NULL) {
        FreeLibrary(hProgLib);
        hProgLib = NULL;
        sProgLibName.clear();
    }
    /* do pointer to functions = NULL */
    SetPort = NULL;
    GetPort = NULL;
}
