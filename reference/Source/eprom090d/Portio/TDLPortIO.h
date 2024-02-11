//*** TDLPortIO: DriverLINX Port IO Driver wrapper component *****************
//**                                                                        **
//** File: TDLPortIO.h                                                      **
//**                                                                        **
//** Copyright (c) 1999 John Pappas (DiskDude). All rights reserved.        **
//**     This software is Freeware.                                         **
//**                                                                        **
//**     Please notify me if you make any changes to this file.             **
//**     Email: diskdude@poboxes.com                                        **
//**                                                                        **
//*** http://diskdude.cjb.net/ ***********************************************

#ifndef __TDLPortIO_H__
#define __TDLPortIO_H__

//---------------------------------------------------------------------------
// DLL Import Types
//    Pointer to functions, to dynamically link into the DLL
//---------------------------------------------------------------------------

typedef unsigned char UCHAR;    // BYTE
typedef unsigned short USHORT;  // WORD
typedef unsigned long ULONG;    // DWORD

typedef UCHAR (__stdcall *TDlPortReadPortUchar)(ULONG Port);
typedef USHORT (__stdcall *TDlPortReadPortUshort)(ULONG Port);
typedef ULONG (__stdcall *TDlPortReadPortUlong)(ULONG Port);

typedef void (__stdcall *TDlPortWritePortUchar)(ULONG Port, UCHAR Value);
typedef void (__stdcall *TDlPortWritePortUshort)(ULONG Port, USHORT Value);
typedef void (__stdcall *TDlPortWritePortUlong)(ULONG Port, ULONG Value);

typedef void (__stdcall *TDlPortReadPortBufferUchar)(ULONG Port, UCHAR *Buffer, ULONG Count);
typedef void (__stdcall *TDlPortReadPortBufferUshort)(ULONG Port, USHORT *Buffer, ULONG Count);
typedef void (__stdcall *TDlPortReadPortBufferUlong)(ULONG Port, ULONG *Buffer, ULONG Count);

typedef void (__stdcall *TDlPortWritePortBufferUchar)(ULONG Port, UCHAR *Buffer, ULONG Count);
typedef void (__stdcall *TDlPortWritePortBufferUshort)(ULONG Port, USHORT *Buffer, ULONG Count);
typedef void (__stdcall *TDlPortWritePortBufferUlong)(ULONG Port, ULONG *Buffer, ULONG Count);


//---------------------------------------------------------------------------
// Data Types
//---------------------------------------------------------------------------

// Specifies the type of read or write in a TPortCommand
typedef enum {tmReadByte,  tmReadWord,  tmReadDWord,
              tmWriteByte, tmWriteWord, tmWriteDWord} TMode;


// Specifies the data required to do a block
// read/write of an array of port records.
// Extends the model TVicHW32/TVicPort uses
typedef struct
{
   WORD PortAddr;    // The address of the port to read/write
   union             // The data to read/write
   {
      BYTE  Byte;
      WORD  Word;
      DWORD DWord;
   } PortData;
   TMode PortMode;   // The mode of reading/writing
} TPortCommand;


// Standard TVicHW32/TVicPort PortRec for compatibility
typedef struct
{
   WORD PortAddr;
   BYTE PortData;
   bool fWrite;
} TPortRec;


//---------------------------------------------------------------------------
// TDLPortIO class
//    This is supposed to be compatible with TVicPort
//---------------------------------------------------------------------------

class PACKAGE TDLPortIO : public TComponent
{
private:
   bool FActiveHW;     // Is the DLL loaded?
   bool FHardAccess;   // Not used: for compatibility only
   bool FRunningWinNT; // True when we're running Windows NT

   HINSTANCE FDLLInst; // For use with DLL
   SC_HANDLE hSCMan;   // For use with WinNT Service Control Manager

   AnsiString FDriverPath; // Full path of WinNT driver
   AnsiString FDLLPath;    // Full path of DriverLINX DLL
   AnsiString FLastError;  // Last error which occurred in Open/CloseDriver()

   // Used for the Windows NT version only
   bool FDrvPrevInst;   // DriverLINX driver already installed?
   bool FDrvPrevStart;  // DriverLINX driver already running?

   // Connects and disconnects to the WinNT Service Control Manager
   bool ConnectSCM();
   void DisconnectSCM();

   // Installs, starts, stops and removes the WinNT kernel mode driver
   bool DriverInstall();
   bool DriverStart();
   bool DriverStop();
   bool DriverRemove();

   // Pointers to the functions within the DLL
   TDlPortReadPortUchar  DlReadByte;
   TDlPortReadPortUshort DlReadWord;
   TDlPortReadPortUlong  DlReadDWord;

   TDlPortWritePortUchar  DlWriteByte;
   TDlPortWritePortUshort DlWriteWord;
   TDlPortWritePortUlong  DlWriteDWord;

   TDlPortReadPortBufferUchar  DlReadBufferByte;
   TDlPortReadPortBufferUshort DlReadBufferWord;
   TDlPortReadPortBufferUlong  DlReadBufferDWord;

   TDlPortWritePortBufferUchar  DlWriteBufferByte;
   TDlPortWritePortBufferUshort DlWriteBufferWord;
   TDlPortWritePortBufferUlong  DlWriteBufferDWord;

protected:
   // returns true if the DLL/Driver is loaded
   bool __fastcall IsLoaded()
      { return FActiveHW; }

   // Wrappers for the properties below
   BYTE __fastcall GetPortByte(WORD Address)
     { return DlReadByte(Address); }
   void __fastcall SetPortByte(WORD Address, BYTE Data)
     { DlWriteByte(Address, Data); }
   WORD __fastcall GetPortWord(WORD Address)
     { return DlReadWord(Address); }
   void __fastcall SetPortWord(WORD Address, WORD Data)
     { DlWriteWord(Address, Data); }
   DWORD __fastcall GetPortDWord(WORD Address)
     { return DlReadDWord(Address); }
   void __fastcall SetPortDWord(WORD Address, DWORD Data)
     { DlWriteDWord(Address, Data); }

public:
   // Constructor
   __fastcall TDLPortIO(TComponent *Owner);
   // Destructor
   __fastcall ~TDLPortIO();

   // These open and close the DLL/Driver
   void __fastcall OpenDriver();
   void __fastcall CloseDriver();

   // Allows write/read array of ports.
   void __fastcall PortControl(TPortRec Ports[], WORD NumPorts);
   void __fastcall PortCommand(TPortCommand Ports[], WORD NumPorts);

   // Allows read/write array of bytes from single port.
   void __fastcall ReadPortFIFO(WORD PortAddr, WORD NumPorts, BYTE Buffer[])
     { DlReadBufferByte(PortAddr, Buffer, NumPorts); }
   void __fastcall WritePortFIFO(WORD PortAddr, WORD NumPorts, BYTE Buffer[])
     { DlWriteBufferByte(PortAddr, Buffer, NumPorts); }

   // Extended block read/write routines for WORD and DWORD
   void __fastcall ReadWPortFIFO(WORD PortAddr, WORD NumPorts, WORD Buffer[])
     { DlReadBufferWord(PortAddr, Buffer, NumPorts); }
   void __fastcall WriteWPortFIFO(WORD PortAddr, WORD NumPorts, WORD Buffer[])
     { DlWriteBufferWord(PortAddr, Buffer, NumPorts); }
   void __fastcall ReadLPortFIFO(WORD PortAddr, WORD NumPorts, DWORD Buffer[])
     { DlReadBufferDWord(PortAddr, Buffer, NumPorts); }
   void __fastcall WriteLPortFIFO(WORD PortAddr, WORD NumPorts, DWORD Buffer[])
     { DlWriteBufferDWord(PortAddr, Buffer, NumPorts); }

   // Access any port as you like, similar to the old pascal way of doing things
   __property BYTE Port[WORD Address]={read=GetPortByte,write=SetPortByte};
   __property WORD PortW[WORD Address]={read=GetPortWord,write=SetPortWord};
   __property DWORD PortL[WORD Address]={read=GetPortDWord,write=SetPortDWord};

__published:
   // Sets the path (no ending \, nor any filename) of the DLPortIO.SYS file
   // Assumed to be <windows system directory>\DRIVERS if not specified
   // Has no effect if the DriverLINX driver is already installed
   __property AnsiString DriverPath={read=FDriverPath,write=FDriverPath};

   // Sets the path (no ending \, nor any filename) of the DLPortIO.DLL file
   // Assumed to be "" if not specified, meaning it will search the program
   // path, windows directory and computer's path for the DLL
   __property AnsiString DLLPath={read=FDLLPath,write=FDLLPath};

   // True when the DLL/Driver has been loaded successfully after OpenDriver()
   __property bool ActiveHW={read=FActiveHW};
   // This doesn't really do anything; provided for compatibility only
   __property bool HardAccess={read=FHardAccess,write=FHardAccess,default=true};

   // Returns the last error which occurred in Open/CloseDriver()
   __property AnsiString LastError={read=FLastError};
};


//---------------------------------------------------------------------------
// TDLPrinterPortIO class
//    This is supposed to be compatible with TVicLPT
//---------------------------------------------------------------------------

// Maximum number of printer ports that would be installed on a system
const MAX_LPT_PORTS = 8;


class PACKAGE TDLPrinterPortIO : public TDLPortIO
{
private:
   // Masks
   static const BYTE BIT0 = 0x01;
   static const BYTE BIT1 = 0x02;
   static const BYTE BIT2 = 0x04;
   static const BYTE BIT3 = 0x08;
   static const BYTE BIT4 = 0x10;
   static const BYTE BIT5 = 0x20;
   static const BYTE BIT6 = 0x40;
   static const BYTE BIT7 = 0x80;

   // Printer Port pin numbers
   static const ACK_PIN       = 10;
   static const BUSY_PIN      = 11;
   static const PAPEREND_PIN  = 12;
   static const SELECTOUT_PIN = 13;
   static const ERROR_PIN     = 15;
   static const STROBE_PIN    = 1;
   static const AUTOFD_PIN    = 14;
   static const INIT_PIN      = 16;
   static const SELECTIN_PIN  = 17;

private:
   BYTE FLPTNumber;    // Current number of the printer port, default=1
   WORD FLPTBase;      // The address of the current printer port (faster)

   int  FLPTCount;     // Number of LPT ports on the system

   // List of port addresses installed on the system
   WORD FLPTAddress[MAX_LPT_PORTS+1];

   // Detects the printer ports using the registry
   void __fastcall DetectPorts();
   void __fastcall DetectPorts9x(); // Win9x version
   void __fastcall DetectPortsNT(); // WinNT version

protected:
   BYTE __fastcall GetLPTNumPorts() { return FLPTCount; }
   WORD __fastcall GetLPTBasePort() { return FLPTBase; }
   void __fastcall SetLPTNumber(BYTE Number);

   bool __fastcall GetPin(BYTE Index);
   void __fastcall SetPin(BYTE Index, bool State);

   bool __fastcall GetLPTAckwl()    { return GetPin(ACK_PIN); }
   bool __fastcall GetLPTBusy()     { return GetPin(BUSY_PIN); }
   bool __fastcall GetLPTPaperEnd() { return GetPin(PAPEREND_PIN); }
   bool __fastcall GetLPTSlct()     { return GetPin(SELECTOUT_PIN); }
   bool __fastcall GetLPTError()    { return GetPin(ERROR_PIN); }

public:
   // Constructor
   __fastcall TDLPrinterPortIO(TComponent *Owner);

   // Sends STROBE signal to the printer
   void __fastcall LPTStrobe();
   // Sends AUTOFD (auto line feed) signal to the printer
   void __fastcall LPTAutofd(bool Flag);
   // Resets printer by sending INIT signal
   void __fastcall LPTInit();
   // Sends SLCTIN signal to the printer
   void __fastcall LPTSlctIn();
   // Sends a character to the printer.
   // Returns true on success. Repeat as neccessary.
   bool __fastcall LPTPrintChar(char Ch);

   // Index valid is in the range 1-25 only (other values return false)
   // Reading the pin returns true when it is 5V, or false when it at 0V.
   // Writing true sets the pin to 5V, or 0V when false.
   __property bool Pin[BYTE Index]={read=GetPin,write=SetPin};

   // Returns ACKWL state from the printer
   __property bool LPTAckwl={read=GetLPTAckwl};
   // Returns BUSY state from the printer
   __property bool LPTBusy={read=GetLPTBusy};
   // Returns PAPER END state from the printer
   __property bool LPTPaperEnd={read=GetLPTPaperEnd};
   // Returns SLCT state from the printer
   __property bool LPTSlct={read=GetLPTSlct};
   // Returns ERROR state from the printer
   __property bool LPTError={read=GetLPTError};

__published:
   // Returns how many LPT ports are installed on your PC.
   __property BYTE LPTNumPorts={read=GetLPTNumPorts};
   // Selects the LPT port to use for all LPT operations
   __property BYTE LPTNumber={read=FLPTNumber,write=SetLPTNumber,default=1};
   // Returns a base address of the current LPT port.
   __property WORD LPTBasePort={read=GetLPTBasePort};
};


//---------------------------------------------------------------------------
// Inline Functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// LPTStrobe()
//    Sends STROBE signal to the printer
//---------------------------------------------------------------------------
inline
void __fastcall TDLPrinterPortIO::LPTStrobe()
{
   // Set to strobe pin to 0V
   SetPin(STROBE_PIN, false);
   // Wait one millisecond
   Sleep(1);
   // Set strobe pin back to 5V
   SetPin(STROBE_PIN, true);
}


//---------------------------------------------------------------------------
// LPTAutofd()
//    Sends AUTOFD (auto line feed) signal to the printer
//---------------------------------------------------------------------------
inline
void __fastcall TDLPrinterPortIO::LPTAutofd(bool Flag)
{
   // Set the auto line feed pin
   SetPin(AUTOFD_PIN, Flag);
}


//---------------------------------------------------------------------------
// LPTInit()
//    Resets printer by sending INIT signal
//---------------------------------------------------------------------------
inline
void __fastcall TDLPrinterPortIO::LPTInit()
{
   // Set pin to a 0V
   SetPin(INIT_PIN, false);
   // Wait 1 ms
   Sleep(1);
   // Set pin back to 5V
   SetPin(INIT_PIN, true);
}


//---------------------------------------------------------------------------
// LPTSlctIn()
//    Sends SLCTIN signal to the printer
//---------------------------------------------------------------------------
inline
void __fastcall TDLPrinterPortIO::LPTSlctIn()
{
   // Send the signal (0V)
   SetPin(SELECTIN_PIN, false);
}


#endif
