// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/** 
 * @ingroup lib_prog_willem
 * @file    willem.cpp
 * @brief   Implementation file (C++) for Willem/Sivava Programmer
 *          Emulation Library
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <new>
#include <string>
#include "willem.hpp"

// ---------------------------------------------------------------------------
/* internal */

/* prog lib config: registry key */
#define CHIPLIB_REGISTRY_KEY      "Software\\EmuProg"
/* prog lib config: registry name */
#define CHIPLIB_REGISTRY_NAME     "ChipLib"
/* chip size: registry name */
#define CHIPLIB_REGISTRY_CHIPSIZE "ChipSize"
/* prog lib: default lib name */
#define CHIPLIB_DEFAULT           "sram.dll"

// ---------------------------------------------------------------------------
/* types */

/* Functions imported from Chip Lib */
typedef DLLIMPORT void           (*PSetSize      )(unsigned long size);
typedef DLLIMPORT void           (*PSetVDD       )(unsigned char state);
typedef DLLIMPORT void           (*PSetVPP       )(unsigned char state);
typedef DLLIMPORT void           (*PSetOE        )(unsigned char state);
typedef DLLIMPORT void           (*PSetCE        )(unsigned char state);
typedef DLLIMPORT void           (*PSetWE        )(unsigned char state);
typedef DLLIMPORT void           (*PSetAddrBus   )(unsigned long addr);
typedef DLLIMPORT void           (*PSetDataBus   )(unsigned short data);
typedef DLLIMPORT unsigned short (*PGetDataBus   )(void);
typedef DLLIMPORT void           (*PSerialSetCS  )(unsigned char state);
typedef DLLIMPORT void           (*PSerialSetClk )(unsigned char state);
typedef DLLIMPORT void           (*PSerialSetData)(unsigned char state);
typedef DLLIMPORT unsigned char  (*PSerialGetData)(void);

// ---------------------------------------------------------------------------
/* global vars */

/* Pointers for Chip Lib functions */
static PSetSize       SetSize       = NULL;
static PSetVDD        SetVDD        = NULL;
static PSetVPP        SetVPP        = NULL;
static PSetOE         SetOE         = NULL;
static PSetCE         SetCE         = NULL;
static PSetWE         SetWE         = NULL;
static PSetAddrBus    SetAddrBus    = NULL;
static PSetDataBus    SetDataBus    = NULL;
static PGetDataBus    GetDataBus    = NULL;
static PSerialSetCS   SerialSetCS   = NULL;
static PSerialSetClk  SerialSetClk  = NULL;
static PSerialSetData SerialSetData = NULL;
static PSerialGetData SerialGetData = NULL;

/* Chip Lib Handle */
static HMODULE hChipLib = NULL;
/* Chip Lib Name */
static std::string sChipLibName = "";
/* Chip Size */
static DWORD currentChipSize = 0;
/* Programmer object */
static WillemProg* objProgrammer = NULL;

// ---------------------------------------------------------------------------
/* Willem Ports Map */

/* Programmer Address Bus settings */

/* Max address */
#define MAX_ADDR     0xFFFFFF
/* Inverted mask of A0 Address */
#define ADDR_MASK_D0 0xFFFFFE

/* Data Port */

/* Address Bus shift register clock */
#define ADDR_BUS_CLK  0x01
/* Address Bus shift register data */
#define ADDR_BUS_DATA 0x02
/* Data Bus shift register clock (inverted) */
#define DATA_BUS_CLK  0x04

/* Status Port */

/* Hardware Test (inverted) */
#define HDW_TEST        0x80
/* Data Bus shift register data (inverted) */
#define DATA_BUS_SER_IN 0x40

/* Control Port */

/* VPP Control (inverted) */
#define VPP 0x01
/* OE Control (inverted) */
#define OE  0x02
/* VDD Control */
#define VDD 0x04
/* WE Control */
#define WE  0x08

/* Serial Device Capability Ports Map */

/* Data Port */

/* Serial Device Data IN (Data: Prog to Device) */
#define SERIAL_DIN  0x01
/* Serial Device Clock (Clock: Prog to Device) */
#define SERIAL_CLK  0x02

/* Status Port */

/* Serial Device Data OUT (Data: Device to Prog) (inverted) */
#define SERIAL_DOUT 0x80

/* Control Port */

/* Serial Device CS (inverted) */
#define SERIAL_CS   0x08

// ---------------------------------------------------------------------------
/* functions */

static bool _Load(void);
static void _Unload(void);

// ---------------------------------------------------------------------------
/* implementation */

DLLEXPORT void SetPort(unsigned short int addr, unsigned char data) {  // NOLINT
  /* initialize objects */
  if (!_Load() || !objProgrammer) { return; }
  /* set port data into programmer object */
  objProgrammer->SetPort(addr, data);
}

DLLEXPORT unsigned char GetPort(unsigned short int addr) {  // NOLINT
  /* initialize objects */
  if (!_Load() || !objProgrammer) { return 0xFF; }
  /* get port data from programmer object */
  return objProgrammer->GetPort(addr);
}

/* DLL entry point for MSWindows */
BOOL WINAPI DllMain (HINSTANCE hInst,   /* Library instance handle. */
                     DWORD reason,      /* Reason this is being called. */
                     LPVOID reserved) { /* Not used. */
  switch (reason){
    case DLL_PROCESS_ATTACH: _Load  (); break;
    case DLL_PROCESS_DETACH: _Unload(); break;
    case DLL_THREAD_ATTACH :            break;
    case DLL_THREAD_DETACH :            break;
  }
  /* Returns TRUE on success, FALSE on failure */
  return TRUE;
}

// ---------------------------------------------------------------------------

bool _Load(void){
  /* check for load library */
  /* variables */
  HKEY hKey;
  char strLibName[4096];
  DWORD dwChipSize = 2048;
  DWORD dwBufLen = static_cast<DWORD>(sizeof(strLibName));
  /* default lib name */
  strncpy(strLibName, CHIPLIB_DEFAULT, dwBufLen);
  /* read registry value: EmuProg\\ChipLib */
  /* open the reg key */
  if (RegCreateKeyEx(HKEY_CURRENT_USER, CHIPLIB_REGISTRY_KEY, 0, NULL, 0,
                     KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS) {
    /* get the value */
    if (RegQueryValueExA(hKey, CHIPLIB_REGISTRY_NAME, NULL, NULL,
                         reinterpret_cast<LPBYTE>(strLibName), &dwBufLen) != ERROR_SUCCESS) {
      /* if error, then creates the default value in registry */
      RegSetValueEx(hKey, CHIPLIB_REGISTRY_NAME, 0,
                    REG_SZ, reinterpret_cast<const BYTE*>(strLibName), dwBufLen);
    }
    /* get the value */
    dwBufLen = static_cast<DWORD>(sizeof(dwChipSize));
    if (RegQueryValueExA(hKey, CHIPLIB_REGISTRY_CHIPSIZE, NULL, NULL,
                         reinterpret_cast<LPBYTE>(&dwChipSize), &dwBufLen) != ERROR_SUCCESS) {
      /* if error, then creates the default value in registry */
      RegSetValueEx(hKey, CHIPLIB_REGISTRY_CHIPSIZE, 0,
                    REG_DWORD, reinterpret_cast<const BYTE*>(&dwChipSize), dwBufLen);
    }
    RegCloseKey(hKey);
  }                     
  /* library is loaded, exit */
  if (sChipLibName == strLibName && currentChipSize == dwChipSize && hChipLib) { return true; }
  /* unload if loaded */
  _Unload();
  /* load library */
  hChipLib = LoadLibraryA(strLibName);
  /* get func pointers */
  if (hChipLib) {
    SetSize       = reinterpret_cast<PSetSize      >(GetProcAddress(hChipLib, "SetSize"      ));
    SetVDD        = reinterpret_cast<PSetVDD       >(GetProcAddress(hChipLib, "SetVDD"       ));
    SetVPP        = reinterpret_cast<PSetVPP       >(GetProcAddress(hChipLib, "SetVPP"       ));
    SetOE         = reinterpret_cast<PSetOE        >(GetProcAddress(hChipLib, "SetOE"        ));
    SetCE         = reinterpret_cast<PSetCE        >(GetProcAddress(hChipLib, "SetCE"        ));
    SetWE         = reinterpret_cast<PSetWE        >(GetProcAddress(hChipLib, "SetWE"        ));
    SetAddrBus    = reinterpret_cast<PSetAddrBus   >(GetProcAddress(hChipLib, "SetAddrBus"   ));
    SetDataBus    = reinterpret_cast<PSetDataBus   >(GetProcAddress(hChipLib, "SetDataBus"   ));
    GetDataBus    = reinterpret_cast<PGetDataBus   >(GetProcAddress(hChipLib, "GetDataBus"   ));
    SerialSetCS   = reinterpret_cast<PSerialSetCS  >(GetProcAddress(hChipLib, "SerialSetCS"  ));
    SerialSetClk  = reinterpret_cast<PSerialSetClk >(GetProcAddress(hChipLib, "SerialSetClk" ));
    SerialSetData = reinterpret_cast<PSerialSetData>(GetProcAddress(hChipLib, "SerialSetData"));
    SerialGetData = reinterpret_cast<PSerialGetData>(GetProcAddress(hChipLib, "SerialGetData"));
  }
  /* if error, unload */
  if (SetSize       == NULL ||
      SetVDD        == NULL || SetVPP        == NULL ||
      SetOE         == NULL || SetCE         == NULL ||
      SetWE         == NULL || SetAddrBus    == NULL ||
      SetDataBus    == NULL || GetDataBus    == NULL ||
      SerialSetCS   == NULL || SerialSetClk  == NULL ||
      SerialSetData == NULL || SerialGetData == NULL)  { _Unload(); }
  /* check for errors */
  if (!hChipLib) { return false; }
  /* creates a Programmer object */
  try { 
    objProgrammer = new WillemProg();
  } catch (std::bad_alloc&) {
    objProgrammer = NULL;
  }
  /* check for errors */
  if (!objProgrammer) {
    _Unload();
    return false;
  } else {
    /* ok, sets the chip size and does CE = 1; */
    SetSize(static_cast<unsigned long>(dwChipSize));  // NOLINT
    SetCE(1);
  }
  /* returns */
  sChipLibName = strLibName;
  currentChipSize = dwChipSize;
  return true;
}

void _Unload(void) {
  /* if loaded, unload */
  if (hChipLib) {
    FreeLibrary(hChipLib);
    hChipLib = NULL;
    sChipLibName.clear();
    currentChipSize = 0;
  }
  /* do pointer to functions = NULL */
  SetSize       = NULL;
  SetVDD        = NULL;
  SetVPP        = NULL;
  SetOE         = NULL;
  SetCE         = NULL;
  SetWE         = NULL;
  SetAddrBus    = NULL;
  SetDataBus    = NULL;
  GetDataBus    = NULL;
  SerialSetCS   = NULL;
  SerialSetClk  = NULL;
  SerialSetData = NULL;
  SerialGetData = NULL;
  /* destroies the Programmer object */
  if (objProgrammer) {
    delete objProgrammer;
    objProgrammer = NULL;
  }
}

// ---------------------------------------------------------------------------

WillemProg::WillemProg(): BaseProg() {
  f_addr_serial_sr = MAX_ADDR;
}

WillemProg::~WillemProg() {}

void WillemProg::EmuCtrlPort(unsigned char data) {  // NOLINT
  /* evaluates port changes and parse the programmer commands */
  unsigned char port_old, port_new;
  bool flag_old, flag_new;
  /* old value */
  port_old = f_control_port;
  /* new value */
  port_new = data;
  /* refresh mirror register */
  f_control_port = data;
  /*---------------------- VPP --------------------------------*/
  flag_old = (port_old & VPP);
  flag_new = (port_new & VPP);
  /* Set/Reset VPP */
  if (flag_new != flag_old) { SetVPP(!flag_new); /* inv */ }
  /*----------------------- OE --------------------------------*/
  flag_old = (port_old & OE);
  flag_new = (port_new & OE);
  /* Set/Reset OE */
  if (flag_new != flag_old) {
    SetOE(!flag_new); /* inv */
    /* OE Off */
    if (port_new & OE) {
      /* Write to AddressBus */
      SetAddrBus(f_addr_serial_sr);
    }
  }
  /*---------------------- VDD --------------------------------*/
  flag_old = (port_old & VDD);
  flag_new = (port_new & VDD);
  /* Set/Reset VDD */
  if (flag_new != flag_old) { SetVDD(flag_new); }
  /*----------------------- WE --------------------------------*/
  flag_old = (port_old & WE);
  flag_new = (port_new & WE);
  /* Set/Reset WE */
  if (flag_new != flag_old) { SetWE(flag_new); }
  /*------------ serial device --------------------*/
  /* SERIAL_CS */
  flag_old = (port_old & SERIAL_CS);
  flag_new = (port_new & SERIAL_CS);
  /* Set/Reset Serial CS */
  if (flag_new != flag_old) { SerialSetCS(!flag_new); /* inv */ }
  /*------------ /serial device -------------------*/
}

void WillemProg::EmuDataPort(unsigned char data) {  // NOLINT
  /* evaluates port changes and parse the programmer commands */
  bool serial_data_forced = false; /* indicates serial data bit port in 0 */
  unsigned char port_old, port_new;
  bool flag_old, flag_new;
  /* old value */
  port_old = f_data_port;
  /* new value */
  port_new = data;
  /* refresh mirror register */
  f_data_port = data;
  /*---------------------- SetDataBus --------------------------------*/
  /* only if OE==Off - Write data to DataBus */
  if (f_control_port & OE) { SetDataBus(data); }
  /*------------------------------------------------------------------*/
  /*------------ serial device --------------------*/
  /*------------- SERIAL_CLK ----------------------*/
  /* only if OE==Off */
  if (f_control_port & OE) {
    /* OE==Off */
    flag_old = (port_old & SERIAL_CLK);
    flag_new = (port_new & SERIAL_CLK);
    /* Set Serial Clock */
    if (flag_new != flag_old) {
      /* SERIAL_CLK == 1  -  Set Serial Data */
      if (flag_new) { SerialSetData((data & SERIAL_DIN) != 0); }
      /* Set Serial Clock */
      SerialSetClk(flag_new);
      /* refresh serial data read pin */
      if (flag_new) {
        /* SERIAL_CLK == 1 */
        /* Get Serial Data -  pin serial_data_forced is inverted */
        serial_data_forced = (SerialGetData() == 0); /* inv */
        /* update SERIAL_DOUT */
        if (serial_data_forced) {
          f_status_port |=  SERIAL_DOUT;
        } else {
          f_status_port &= ~SERIAL_DOUT;
        }
      }
    }
  }
  /*------------ /serial device -------------------*/
  /*---------------------- ADDR_BUS_CLK --------------------------------*/
  /* only if OE==On */
  if ((f_control_port & OE) == 0) {
    /* OE==On */
    flag_old = (port_old & ADDR_BUS_CLK);
    flag_new = (port_new & ADDR_BUS_CLK);
    /* SetAddr serial control */
    // if (flag_new != flag_old && !flag_new) { /* ADDR_BUS_CLK Toggle Off */
    if (flag_new != flag_old && flag_new) { /* ADDR_BUS_CLK Toggle On */
      /* shift addr register */
      f_addr_serial_sr <<= 1;
      /*------------------------------------------------------------*/
      /* set address bit in ADDR_BUS_DATA */
      if (port_new & ADDR_BUS_DATA) {
        f_addr_serial_sr |= ~ADDR_MASK_D0;
      } else {
        f_addr_serial_sr &= ADDR_MASK_D0;
      }
      /*------------------------------------------------------------*/
      /* mask register with max address size */
      f_addr_serial_sr &= MAX_ADDR;
      /* Write to AddressBus */
      SetAddrBus(f_addr_serial_sr);
    }
  }
  /*---------------------- DATA_BUS_CLK --------------------------------*/
  flag_old = (port_old & DATA_BUS_CLK);
  flag_new = (port_new & DATA_BUS_CLK);
  /* DATA_BUS_CLK On */
  if (flag_new != flag_old && !flag_new /* inv */) {
    /* data bus shift register parallel mode */
    if ((port_new & ADDR_BUS_DATA) && !(f_control_port & OE)) {
      /* Write to AddressBus */
      SetAddrBus(f_addr_serial_sr);
      /* load data_sr
        (emulate parallel load of the data bus shift register) */
      f_data_serial_sr = GetDataBus();
    } else {
      /* data bus shift register serial mode */
      /* shift data register */
      f_data_serial_sr <<=  1;
    }
    /* refresh status register with data bus shift reg */
    /*----------------------------------------------------------------------*/
    /* refresh status with data MSB bit */
    if (f_data_serial_sr & 0x80 /*inv*/) {
      f_status_port &= ~DATA_BUS_SER_IN;
    } else {
      f_status_port |= DATA_BUS_SER_IN;
    }
    /*----------------------------------------------------------------------*/
  }
  /* update status/HDW_TEST */
  /* only serial data not forced by SERIAL_DOUT */
  if (!serial_data_forced) { /*--- serial device ---*/
    /* formula: HDW_TEST = ~ADDR_BUS_CLK . VDD  */
    if ((port_new & ADDR_BUS_CLK) || !(f_control_port & VDD)) {
      f_status_port &= ~HDW_TEST;
    } else {
      f_status_port |= HDW_TEST;
    }
  }
}
