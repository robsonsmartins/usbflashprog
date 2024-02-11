//*** TDLPortIO: DriverLINX Port IO Driver wrapper component *****************
//**                                                                        **
//** File: TDLPortIO.cpp                                                    **
//**                                                                        **
//** Copyright (c) 1999 John Pappas (DiskDude). All rights reserved.        **
//**     This software is FreeWare.                                         **
//**                                                                        **
//**     Please notify me if you make any changes to this file.             **
//**     Email: diskdude@poboxes.com                                        **
//**                                                                        **
//**                                                                        **
//** The following resources helped in developing the install, start, stop  **
//** and remove code for dynamically opening/closing the DriverLINX WinNT   **
//** kernel mode driver.                                                    **
//**                                                                        **
//**   "Dynamically Loading Drivers in Windows NT" by Paula Tomlinson       **
//**   from "Windows Developer's Journal", Volume 6, Issue 5. (C code)      **
//**      ftp://ftp.mfi.com/pub/windev/1995/may95.zip                       **
//**                                                                        **
//**   "Hardware I/O Port Programming with Delphi and NT" by Graham Wideman **
//**      http://www.wideman-one.com/tech/Delphi/IOPM/index.htm             **
//**                                                                        **
//**                                                                        **
//** Special thanks to Peter Holm <comtext3@post4.tele.dk> for his          **
//** algorithm and code for detecting the number and addresses of the       **
//** installed printer ports, on which the detection code below is based.   **
//**                                                                        **
//*** http://diskdude.cjb.net/ ***********************************************

#include <vcl.h>
#include <cstring>
#include <stdlib.h>
#include <string.h>
#pragma hdrstop
#pragma package(smart_init)

#include "TDLPortIO.h"

//---------------------------------------------------------------------------
// Constants
//---------------------------------------------------------------------------

// DriverLINX DLL filename
const AnsiString LIBRARY_FILENAME = "DLPortIO.dll";

// WinNT DriverLINX Information
const AnsiString DRIVER_NAME = "DLPortIO";
const AnsiString DISPLAY_NAME = "DriverLINX Port I/O Driver";
const AnsiString DRIVER_GROUP = "SST miniport drivers";

// Define to do a little less error checking
#define FAST


//*************************************************************************//
// TDLPortIO class implementation
//*************************************************************************//


//---------------------------------------------------------------------------
// TDLPortIO()
//---------------------------------------------------------------------------
__fastcall TDLPortIO::TDLPortIO(TComponent *Owner) : TComponent(Owner)
{
   // Are we running Windows NT?
   OSVERSIONINFO os;
   memset(&os, NULL, sizeof(OSVERSIONINFO));
   os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
   GetVersionEx(&os);
   FRunningWinNT=(os.dwPlatformId==VER_PLATFORM_WIN32_NT);

   // Set default WinNT driver path
   char Buffer[MAX_PATH];
   GetSystemDirectory(Buffer, MAX_PATH);
   FDriverPath = AnsiString(Buffer)+"\\DRIVERS";

   // Set the default DLL path
   FDLLPath="";

   FActiveHW=false;  // DLL/Driver not loaded
   FHardAccess=true; // Not used, default true

   FLastError="";    // No errors yet
}


//---------------------------------------------------------------------------
// ~TDLPortIO()
//---------------------------------------------------------------------------
__fastcall TDLPortIO::~TDLPortIO()
{
   // Make sure we close the DLL
   if (IsLoaded()) CloseDriver();
}


//---------------------------------------------------------------------------
// ConnectSCM()
//    Connects to the WinNT Service Control Manager
//---------------------------------------------------------------------------
bool TDLPortIO::ConnectSCM()
{
   DWORD dwStatus = 0; // Assume success, until we prove otherwise
   DWORD scAccess;     // Access mode when connecting to SCM

   // Try and connect as administrator
   scAccess = SC_MANAGER_CONNECT |
              SC_MANAGER_QUERY_LOCK_STATUS |
              SC_MANAGER_ENUMERATE_SERVICE |
              SC_MANAGER_CREATE_SERVICE;      // Admin only

   // Connect to the SCM
   hSCMan = OpenSCManager(NULL, NULL, scAccess);

   // If we're not in administrator mode, try and reconnect
   if (hSCMan==NULL && GetLastError()==ERROR_ACCESS_DENIED)
   {
      scAccess = SC_MANAGER_CONNECT |
                 SC_MANAGER_QUERY_LOCK_STATUS |
                 SC_MANAGER_ENUMERATE_SERVICE;

      // Connect to the SCM
      hSCMan = OpenSCManager(NULL, NULL, scAccess);
   }

   // Did it succeed?
   if (hSCMan==NULL)
   {
      // Failed, save error information
      dwStatus=GetLastError();
      FLastError="ConnectSCM: Error #"+IntToStr(dwStatus);
   }

   return dwStatus==0; // Success == 0
}


//---------------------------------------------------------------------------
// DisconnectSCM()
//    Disconnects from the WinNT Service Control Manager
//---------------------------------------------------------------------------
void TDLPortIO::DisconnectSCM()
{
   if (hSCMan != NULL)
   {
      // Disconnect from our local Service Control Manager
      CloseServiceHandle(hSCMan);
      hSCMan=NULL;
   }
}


//---------------------------------------------------------------------------
// DriverInstall()
//    Installs the DriverLINX driver into Windows NT's registry
//---------------------------------------------------------------------------
bool TDLPortIO::DriverInstall()
{
   SC_HANDLE hService; // Handle to the new service
   DWORD dwStatus = 0; // Assume success, until we prove otherwise

   FDrvPrevInst=false; // Assume the driver wasn't installed previously

   // Path including filename
   AnsiString DriverPath = FDriverPath+"\\"+DRIVER_NAME+".SYS";

   // Is the DriverLINX driver already in the SCM? If so,
   // indicate success and set FDrvPrevInst to true.
   hService=OpenService(hSCMan, DRIVER_NAME.c_str(), SERVICE_QUERY_STATUS);
   if (hService!=NULL)
   {
      FDrvPrevInst=true;            // Driver previously installed, don't remove
      CloseServiceHandle(hService); // Close the service
      return true;                  // Success
   }

   // Add to our Service Control Manager's database
   hService=CreateService(
               hSCMan,
               DRIVER_NAME.c_str(),
               DISPLAY_NAME.c_str(),
               SERVICE_START | SERVICE_STOP | DELETE | SERVICE_QUERY_STATUS,
               SERVICE_KERNEL_DRIVER,
               SERVICE_DEMAND_START,
               SERVICE_ERROR_NORMAL,
               DriverPath.c_str(),
               DRIVER_GROUP.c_str(),
               NULL, NULL, NULL, NULL);

   if (hService==NULL)
      dwStatus=GetLastError();
   else
      // Close the service for now...
      CloseServiceHandle(hService);

   if (dwStatus!=0)
      FLastError="DriverInstall: Error #"+IntToStr(dwStatus);

   return dwStatus==0; // Success == 0
}


//---------------------------------------------------------------------------
// DriverStart()
//    Starts the Windows NT DriverLINX driver for use
//---------------------------------------------------------------------------
bool TDLPortIO::DriverStart()
{
   SC_HANDLE hService; // Handle to the service to start
   DWORD dwStatus = 0; // Assume success, until we prove otherwise

   SERVICE_STATUS sStatus;

   FDrvPrevStart=false; // Assume the driver was not already running

   hService = OpenService(hSCMan, DRIVER_NAME.c_str(), SERVICE_QUERY_STATUS);
   if (hService!=NULL && QueryServiceStatus(hService, &sStatus))
   {
      // Got the service status, now check it
      if (sStatus.dwCurrentState==SERVICE_RUNNING)
      {
         FDrvPrevStart=true;           // Driver was previously started
         CloseServiceHandle(hService); // Close service
         return true;                  // Success
      }
      else if (sStatus.dwCurrentState==SERVICE_STOPPED)
      {
         // Driver was stopped. Start the driver.
         CloseServiceHandle(hService);
         hService = OpenService(hSCMan, DRIVER_NAME.c_str(), SERVICE_START);
         if (!StartService(hService, 0, NULL))
            dwStatus=GetLastError();
         CloseServiceHandle(hService); // Close service
      }
      else dwStatus=-1; // Can't run the service
   }
   else
      dwStatus=GetLastError();

   if (dwStatus!=0)
      FLastError="DriverStart: Error #"+IntToStr(dwStatus);

   return dwStatus==0; // Success == 0
}


//---------------------------------------------------------------------------
// DriverStop()
//    Stops a Windows NT driver from use
//---------------------------------------------------------------------------
bool TDLPortIO::DriverStop()
{
   SC_HANDLE hService; // Handle to the service to start
   DWORD dwStatus = 0; // Assume success, until we prove otherwise

   // If we didn't start the driver, then don't stop it.
   // Pretend we stopped it, by indicating success.
   if (FDrvPrevStart) return true;

   // Get a handle to the service to stop
   hService = OpenService(
                 hSCMan,
                 DRIVER_NAME.c_str(),
                 SERVICE_STOP | SERVICE_QUERY_STATUS);

   if (hService!=NULL)
   {
      // Stop the driver, then close the service
      SERVICE_STATUS sStatus;

      if (!ControlService(hService, SERVICE_CONTROL_STOP, &sStatus))
         dwStatus = GetLastError();

      // Close the service
      CloseServiceHandle(hService);
   }
   else
      dwStatus = GetLastError();

   if (dwStatus!=0)
      FLastError="DriverStop: Error #"+IntToStr(dwStatus);

   return dwStatus==0; // Success == 0
}


//---------------------------------------------------------------------------
// DriverRemove()
//    Removes a driver from the Windows NT system (service manager)
//---------------------------------------------------------------------------
bool TDLPortIO::DriverRemove()
{
   SC_HANDLE hService; // Handle to the service to start
   DWORD dwStatus = 0; // Assume success, until we prove otherwise

   // If we didn't install the driver, then don't remove it.
   // Pretend we removed it, by indicating success.
   if (FDrvPrevInst) return true;

   // Get a handle to the service to remove
   hService = OpenService(
                 hSCMan,
                 DRIVER_NAME.c_str(),
                 DELETE);

   if (hService!=NULL)
   {
      // Remove the driver then close the service again
      if (!DeleteService(hService))
         dwStatus = GetLastError();

      // Close the service
      CloseServiceHandle(hService);
   }
   else
      dwStatus = GetLastError();

   if (dwStatus!=0)
      FLastError="DriverRemove: Error #"+IntToStr(dwStatus);

   return dwStatus==0; // Success == 0
}


//---------------------------------------------------------------------------
// OpenDriver()
//    Opens the DLL dynamically
//---------------------------------------------------------------------------
void __fastcall TDLPortIO::OpenDriver()
{
   // If the DLL/driver is already open, then forget it!
   if (IsLoaded()) return;

   // If we're running Windows NT, install the driver then start it
   if (FRunningWinNT)
   {
      // Connect to the Service Control Manager
      if (!ConnectSCM()) return;

      // Install the driver
      if (!DriverInstall())
      {
         // Driver install failed, so disconnect from the SCM
         DisconnectSCM();
         return;
      }

      // Start the driver
      if (!DriverStart())
      {
         // Driver start failed, so remove it then disconnect from SCM
         DriverRemove();
         DisconnectSCM();
         return;
      }
   }

   // Load DLL library
   AnsiString LibraryFileName = LIBRARY_FILENAME;

   if (FDLLPath.Length()>0)
      LibraryFileName = FDLLPath+"\\"+LIBRARY_FILENAME;

   FDLLInst=LoadLibrary(LibraryFileName.c_str());
   if (FDLLInst!=NULL)
   {
      DlReadByte=(TDlPortReadPortUchar)
                  GetProcAddress(FDLLInst,"DlPortReadPortUchar");
      DlReadWord=(TDlPortReadPortUshort)
                  GetProcAddress(FDLLInst,"DlPortReadPortUshort");
      DlReadDWord=(TDlPortReadPortUlong)
                   GetProcAddress(FDLLInst,"DlPortReadPortUlong");

      DlWriteByte=(TDlPortWritePortUchar)
                   GetProcAddress(FDLLInst,"DlPortWritePortUchar");
      DlWriteWord=(TDlPortWritePortUshort)
                   GetProcAddress(FDLLInst,"DlPortWritePortUshort");
      DlWriteDWord=(TDlPortWritePortUlong)
                    GetProcAddress(FDLLInst,"DlPortWritePortUlong");

      DlReadBufferByte=(TDlPortReadPortBufferUchar)
                        GetProcAddress(FDLLInst,"DlPortReadPortBufferUchar");
      DlReadBufferWord=(TDlPortReadPortBufferUshort)
                        GetProcAddress(FDLLInst,"DlPortReadPortBufferUshort");
      DlReadBufferDWord=(TDlPortReadPortBufferUlong)
                         GetProcAddress(FDLLInst,"DlPortReadPortBufferUlong");

      DlWriteBufferByte=(TDlPortWritePortBufferUchar)
                         GetProcAddress(FDLLInst,"DlPortWritePortBufferUchar");
      DlWriteBufferWord=(TDlPortWritePortBufferUshort)
                         GetProcAddress(FDLLInst,"DlPortWritePortBufferUshort");
      DlWriteBufferDWord=(TDlPortWritePortBufferUlong)
                          GetProcAddress(FDLLInst,"DlPortWritePortBufferUlong");

      // Make sure all our functions are there
      if (DlReadByte!=NULL && DlReadWord!=NULL && DlReadDWord!=NULL &&
          DlWriteByte!=NULL && DlWriteWord!=NULL && DlWriteDWord!=NULL &&
          DlReadBufferByte!=NULL && DlReadBufferWord!=NULL &&
          DlReadBufferDWord!=NULL && DlWriteBufferByte!=NULL &&
          DlWriteBufferWord!=NULL && DlWriteBufferDWord!=NULL)
         FActiveHW=true; // Success
   }

   // Did we fail?
   if (!FActiveHW)
   {
      // If we're running Windows NT, stop the driver then remove it
      // Forget about any return (error) values we might get...
      if (FRunningWinNT)
      {
         DriverStop();
         DriverRemove();
         DisconnectSCM();
      }

      // Free the library
      if (FDLLInst!=NULL)
      {
         FreeLibrary(FDLLInst);
         FDLLInst=NULL;
      }
   }
}


//---------------------------------------------------------------------------
// CloseDriver()
//    Closes the dynamically opened DLL
//---------------------------------------------------------------------------
void __fastcall TDLPortIO::CloseDriver()
{
   // Don't close anything if it wasn't opened previously
   if (!IsLoaded()) return;

   // If we're running Windows NT, stop the driver then remove it
   if (FRunningWinNT)
   {
      if (!DriverStop()) return;
      if (!DriverRemove()) return;
      DisconnectSCM();
   }

   // Free the library
   if (FreeLibrary(FDLLInst)==0) return;
   FDLLInst=NULL;

   FActiveHW=false; // Success
}


//---------------------------------------------------------------------------
// PortControl()
//    Reads/writes a TVicPort/TVicHW32 compatible port record array
//---------------------------------------------------------------------------
void __fastcall TDLPortIO::PortControl(TPortRec Ports[], WORD NumPorts)
{
#ifndef FAST
   if (!IsLoaded()) return;
#endif

   for (int Index=0; Index<NumPorts; Index++)
      if (Ports[Index].fWrite)
         DlWriteByte(Ports[Index].PortAddr, Ports[Index].PortData);
      else
         Ports[Index].PortData=DlReadByte(Ports[Index].PortAddr);
}


//---------------------------------------------------------------------------
// PortCommand()
//    Reads/writes a port command array
//---------------------------------------------------------------------------
void __fastcall TDLPortIO::PortCommand(TPortCommand Ports[], WORD NumPorts)
{
#ifndef FAST
   if (!IsLoaded()) return;
#endif

   for (int Index=0; Index<NumPorts; Index++)
      switch (Ports[Index].PortMode)
      {
         case tmReadByte:
            Ports[Index].PortData.Byte=DlReadByte(Ports[Index].PortAddr);
            break;

         case tmReadWord:
            Ports[Index].PortData.Word=DlReadWord(Ports[Index].PortAddr);
            break;

         case tmReadDWord:
            Ports[Index].PortData.DWord=DlReadDWord(Ports[Index].PortAddr);
            break;

         case tmWriteByte:
            DlWriteByte(Ports[Index].PortAddr, Ports[Index].PortData.Byte);
            break;

         case tmWriteWord:
            DlWriteWord(Ports[Index].PortAddr, Ports[Index].PortData.Word);
            break;

         case tmWriteDWord:
            DlWriteDWord(Ports[Index].PortAddr, Ports[Index].PortData.DWord);
            break;

         default:
            break; // Ignore it
      }
}



//*************************************************************************//
// TDLPrinterPortIO class implementation
//*************************************************************************//


//---------------------------------------------------------------------------
// TDLPrinterPortIO()
//---------------------------------------------------------------------------
__fastcall TDLPrinterPortIO::TDLPrinterPortIO(TComponent *Owner)
   : TDLPortIO(Owner),
     FLPTNumber(0),    // No LPT selected
     FLPTBase(0),      // No base address
     FLPTCount(0)      // No printer ports counted
{
   // Detect the printer ports available
   DetectPorts();

   // Set the default LPT number
   SetLPTNumber(1);
}


//---------------------------------------------------------------------------
// DetectPorts()
//---------------------------------------------------------------------------
void __fastcall TDLPrinterPortIO::DetectPorts()
{
   bool RunningWinNT;

   // Are we running Windows NT?
   OSVERSIONINFO os;
   memset(&os, NULL, sizeof(OSVERSIONINFO));
   os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
   GetVersionEx(&os);
   RunningWinNT=(os.dwPlatformId==VER_PLATFORM_WIN32_NT);

   // Detect the printer ports available
   if (RunningWinNT)
      DetectPortsNT(); // WinNT version
   else
      DetectPorts9x(); // Win9x version
}


//---------------------------------------------------------------------------
// DetectPorts9x()
//---------------------------------------------------------------------------
void __fastcall TDLPrinterPortIO::DetectPorts9x()
{
   const char *BASE_KEY = "Config Manager\\Enum";
   const char *PROBLEM = "Problem";
   const char *ALLOCATION = "Allocation";
   const char *PORTNAME = "PortName";
   const char *HARDWARE_KEY = "HardwareKey";

   const REGSAM KEY_PERMISSIONS = KEY_ENUMERATE_SUB_KEYS |
                                  KEY_QUERY_VALUE;

   HKEY CurKey;               // Current key when using the registry
   char KeyName[MAX_PATH];    // A key name when using the registry

   char **KeyList;            // List of keys
   DWORD KeyCount;            // Count of the number of keys in KeyList

   // Clear the port count
   FLPTCount = 0;

   // Clear the port array
   for (int index=0; index<=MAX_LPT_PORTS; index++)
      FLPTAddress[index] = 0;

   // Open the registry
   RegOpenKeyEx(HKEY_DYN_DATA, BASE_KEY, 0, KEY_PERMISSIONS, &CurKey);

   // Grab all the key names under HKEY_DYN_DATA
   //
   // Do this by first counting the number of keys,
   // then creating an array big enough to hold them
   // using the KeyList pointer.

   FILETIME DummyFileTime;
   DWORD DummyLength = MAX_PATH;
   KeyCount = 0;
   while (RegEnumKeyEx(
            CurKey, KeyCount++, KeyName, &DummyLength,
            NULL, NULL, NULL, &DummyFileTime
                       ) != ERROR_NO_MORE_ITEMS)
   {
      DummyLength = MAX_PATH;
   }

   KeyList = new char*[KeyCount];

   KeyCount = 0;
   DummyLength = MAX_PATH;
   while (RegEnumKeyEx(
            CurKey, KeyCount, KeyName, &DummyLength,
            NULL, NULL, NULL, &DummyFileTime
                       ) != ERROR_NO_MORE_ITEMS)
   {
      KeyList[KeyCount] = new char[DummyLength+1];
      strcpy(KeyList[KeyCount], KeyName);
      DummyLength = MAX_PATH;
      KeyCount++;
   }

   // Close the key
   RegCloseKey(CurKey);

   // Cycle through all keys; looking for a string valued subkey called
   // 'HardWareKey' which is not NULL, and another subkey called 'Problem'
   // whose fields are all valued 0.
   for (DWORD KeyIndex=0; KeyIndex<KeyCount; KeyIndex++)
   {
      bool HasProblem = false; // Is 'Problem' non-zero? Assume it is Ok

      // Open the key
      strcpy(KeyName, BASE_KEY);
      strcat(KeyName, "\\");
      strcat(KeyName, KeyList[KeyIndex]);
      if (RegOpenKeyEx(
            HKEY_DYN_DATA, KeyName, 0, KEY_PERMISSIONS, &CurKey
                        ) != ERROR_SUCCESS)
         continue;

      // Test for a 0 valued Problem sub-key,
      // which must only consist of raw data
      DWORD DataType, DataSize;
      RegQueryValueEx(CurKey, PROBLEM, NULL, &DataType, NULL, &DataSize);
      if (DataType == REG_BINARY)
      {
         // We have a valid, binary "Problem" sub-key
         // Test to see if the fields are zero

         char HardwareSubKey[MAX_PATH];
               // Data from the "Hardware" sub-key

         BYTE *Data = new BYTE[DataSize];
               // Data from "Problem" sub-key

         // Read the data from the "Problem" sub-key
         if (RegQueryValueEx(
                  CurKey, PROBLEM, NULL,
                  NULL, Data, &DataSize
                             ) == ERROR_SUCCESS)
         {
            // See if it has any problems
            for (DWORD index=0; index<DataSize; index++)
               HasProblem |= Data[index];
         }
         else
            HasProblem = true; // No good

         delete[] Data;

         // Now try and read the Hardware sub-key
         DataSize = MAX_PATH;
         RegQueryValueEx(
            CurKey, HARDWARE_KEY, NULL, &DataType, HardwareSubKey, &DataSize
                         );
         if (DataType != REG_SZ)
            HasProblem = true; // No good

         // Do we have no problem, and a non-null Hardware sub-key?
         if (!HasProblem && strlen(HardwareSubKey) > 0)
         {
            // Now open the key which is "pointed at" by HardwareSubKey
            RegCloseKey(CurKey);

            strcpy(KeyName, "Enum\\");
            strcat(KeyName, HardwareSubKey);
            if (RegOpenKeyEx(
                  HKEY_LOCAL_MACHINE, KeyName, 0, KEY_PERMISSIONS, &CurKey
                              ) != ERROR_SUCCESS)
               continue;

            // Now read in the PortName and obtain the LPT number from it
            char PortName[MAX_PATH];
            DataSize = MAX_PATH;
            RegQueryValueEx(
               CurKey, PORTNAME, NULL, &DataType, PortName, &DataSize
                            );
            if (DataType != REG_SZ)
               strcpy(PortName, ""); // No good

            // Make sure it has LPT in it
            if (strstr(PortName, "LPT") != NULL)
            {
               int PortNumber;
                     // The nubmer of the port
               char PortNumberStr[MAX_PATH];
                     // Holds the number of the port as a string

               WORD Allocation[64];
                     // Holds the registry data for the port address allocation

               memset(PortNumberStr, '\0', MAX_PATH);
               strncpy(PortNumberStr,
                       strstr(PortName, "LPT")+3,
                       strlen(PortName)-(strstr(PortName, "LPT")-PortName)-2);

               // Find the port number
               PortNumber = atoi(PortNumberStr);

               // Find the address
               RegCloseKey(CurKey);

               strcpy(KeyName, BASE_KEY);
               strcat(KeyName, "\\");
               strcat(KeyName, KeyList[KeyIndex]);
               RegOpenKeyEx(HKEY_DYN_DATA, KeyName, 0, KEY_PERMISSIONS, &CurKey);

               DataSize = sizeof(Allocation);
               RegQueryValueEx(
                  CurKey, ALLOCATION, NULL, &DataType,
                  (unsigned char*)Allocation, &DataSize
                               );
               if (DataType == REG_BINARY)
               {
                  // Decode the Allocation data: the port address is present
                  // directly after a 0x000C entry (which doesn't have 0x0000
                  // after it).
                  for (int pos=0; pos<63; pos++)
                     if (Allocation[pos] == 0x000C &&
                         Allocation[pos+1] != 0x0000 &&
                         PortNumber<=MAX_LPT_PORTS)
                     {
                        // Found a port; add it to the list
                        FLPTAddress[PortNumber] = Allocation[pos+1];
                        FLPTCount++;
                        break;
                     }

               }

            }

         }
      }

      RegCloseKey(CurKey);
   }

   // Destroy our key list
   for (DWORD index=0; index<=KeyCount; index++)
      delete[] KeyList[index];
   delete KeyList;
}


//---------------------------------------------------------------------------
// DetectPortsNT()
//---------------------------------------------------------------------------
void __fastcall TDLPrinterPortIO::DetectPortsNT()
{
   const char *BASE_KEY = "HARDWARE\\DEVICEMAP\\PARALLEL PORTS";
   const char *LOADED_KEY = "HARDWARE\\RESOURCEMAP\\LOADED PARALLEL DRIVER RESOURCES\\Parport";
   const char *DOS_DEVICES = "\\DosDevices\\LPT";
   const char *DEVICE_PARALLEL = "\\Device\\Parallel";

   const REGSAM KEY_PERMISSIONS = KEY_ENUMERATE_SUB_KEYS |
                                  KEY_QUERY_VALUE;

   HKEY CurKey;               // Current key when using the registry
   char KeyName[MAX_PATH];    // A key name when using the registry

   char **ValueList;          // List of value names
   DWORD ValueCount;          // Count of the number of value names in ValueList

   // Clear the port count
   FLPTCount = 0;

   // Clear the port array
   for (int index=0; index<=MAX_LPT_PORTS; index++)
      FLPTAddress[index] = 0;


   // Open the registry
   if (RegOpenKeyEx(
         HKEY_LOCAL_MACHINE, BASE_KEY, 0, KEY_PERMISSIONS, &CurKey
                     ) != ERROR_SUCCESS)
      return; // Can't do anything without this BASE_KEY

   // Grab all the value names under HKEY_LOCAL_MACHINE
   //
   // Do this by first counting the number of value names,
   // then creating an array big enough to hold them
   // using the ValueList pointer.

   DWORD DummyLength = MAX_PATH;
   DWORD ValueType;
   ValueCount = 0;
   while (RegEnumValue(
            CurKey, ValueCount++, KeyName, &DummyLength,
            NULL, &ValueType, NULL, NULL
                       ) != ERROR_NO_MORE_ITEMS)
   {
      DummyLength = MAX_PATH;
   }

   ValueList = new char*[ValueCount];

   ValueCount = 0;
   DummyLength = MAX_PATH;
   while (RegEnumValue(
            CurKey, ValueCount, KeyName, &DummyLength,
            NULL, &ValueType, NULL, NULL
                       ) != ERROR_NO_MORE_ITEMS)
   {
      ValueList[ValueCount] = new char[DummyLength+1];
      strcpy(ValueList[ValueCount], KeyName);
      DummyLength = MAX_PATH;
      ValueCount++;
   }

   // Close the key
   RegCloseKey(CurKey);

   for (DWORD index=0; index<ValueCount; index++)
   {
      char DosDev[MAX_PATH];    // Key value for \DosDevices\LPT
      DWORD DataType, DataSize; // Type and size of data read from the registry

      // Is it a \DosDevices\LPT key?
      strcpy(KeyName, BASE_KEY);
      if (RegOpenKeyEx(
            HKEY_LOCAL_MACHINE, KeyName, 0, KEY_PERMISSIONS, &CurKey
                        ) == ERROR_SUCCESS)
      {
         DataSize = MAX_PATH;
         RegQueryValueEx(
            CurKey, ValueList[index], NULL, &DataType, DosDev, &DataSize
                         );
         RegCloseKey(CurKey);

         // Make sure it was a string
         if (DataType != REG_SZ)
            strcpy(DosDev, "");
      }
      else
         strcpy(DosDev, "");

      if (strstr(DosDev, DOS_DEVICES) != NULL)
      {
         int PortNumber;                  // The nubmer of the port
         char PortNumberStr[MAX_PATH];    // String version of PortNumber
         char PortIDStr[MAX_PATH];        // PortID

         memset(PortNumberStr, '\0', MAX_PATH);
         strncpy(PortNumberStr,
                 strstr(DosDev, DOS_DEVICES) + strlen(DOS_DEVICES),
                 strlen(DosDev) - (strstr(DosDev, DOS_DEVICES)-DosDev)
                                - strlen(DOS_DEVICES) + 1
                 );

         // Get the Port ID
         memset(PortIDStr, '\0', MAX_PATH);
         strncpy(PortIDStr,
                 strstr(ValueList[index], DEVICE_PARALLEL) + strlen(DEVICE_PARALLEL),
                 strlen(ValueList[index])
                     - (strstr(ValueList[index], DEVICE_PARALLEL)-ValueList[index])
                     - strlen(DEVICE_PARALLEL) + 1
                 );

         // Get the port number
         PortNumber = atoi(PortNumberStr);

         // Get the port address
         RegOpenKeyEx(HKEY_LOCAL_MACHINE, LOADED_KEY, 0, KEY_PERMISSIONS, &CurKey);

         strcpy(DosDev, "\\Device\\ParallelPort");
         strcat(DosDev, PortIDStr);
         strcat(DosDev, ".Raw");

         if (RegQueryValueEx(
               CurKey, DosDev, NULL, &DataType, NULL, NULL
                             ) == ERROR_SUCCESS &&
             DataType == REG_RESOURCE_LIST)
         {
            WORD Allocation[64]; // Binary data with port number inside

            // Read in the binary data
            DataSize = sizeof(Allocation);
            RegQueryValueEx(
               CurKey, DosDev, NULL, NULL,
               (unsigned char*)Allocation, &DataSize
                            );

            // Found a port; add it to the list
            if (DataSize>0 && PortNumber<=MAX_LPT_PORTS)
            {
               FLPTAddress[PortNumber] = Allocation[12];
               FLPTCount++;
            }
         }

         RegCloseKey(CurKey);
      }
   }

   // Destroy our key value list
   // Destroy our key list
   for (DWORD index=0; index<=ValueCount; index++)
      delete[] ValueList[index];
   delete ValueList;
}


//---------------------------------------------------------------------------
// SetLPTNumber()
//---------------------------------------------------------------------------
void __fastcall TDLPrinterPortIO::SetLPTNumber(BYTE Number)
{
   // Note that we don't make sure it is within the range 1..FLPTCount
   // because there _might_ (can someone claify this?) be a port numbered
   // as #2, where it may be the _only_ port installed on the system.
   if (Number>0 && Number<=MAX_LPT_PORTS)
   {
      FLPTNumber=Number;
      FLPTBase=FLPTAddress[Number];
   }
}


//---------------------------------------------------------------------------
// GetPin()
//---------------------------------------------------------------------------
bool __fastcall TDLPrinterPortIO::GetPin(BYTE Index)
{
   switch (Index)
   {
      case 1:  return (Port[FLPTBase+2]&BIT0)==0; // Inverted
      case 2:  return (Port[FLPTBase]&BIT0)!=0;
      case 3:  return (Port[FLPTBase]&BIT1)!=0;
      case 4:  return (Port[FLPTBase]&BIT2)!=0;
      case 5:  return (Port[FLPTBase]&BIT3)!=0;
      case 6:  return (Port[FLPTBase]&BIT4)!=0;
      case 7:  return (Port[FLPTBase]&BIT5)!=0;
      case 8:  return (Port[FLPTBase]&BIT6)!=0;
      case 9:  return (Port[FLPTBase]&BIT7)!=0;
      case 10: return (Port[FLPTBase+1]&BIT6)!=0;
      case 11: return (Port[FLPTBase+1]&BIT7)==0; // Inverted
      case 12: return (Port[FLPTBase+1]&BIT5)!=0;
      case 13: return (Port[FLPTBase+1]&BIT4)!=0;
      case 14: return (Port[FLPTBase+2]&BIT1)==0; // Inverted
      case 15: return (Port[FLPTBase+1]&BIT3)!=0;
      case 16: return (Port[FLPTBase+2]&BIT2)!=0;
      case 17: return (Port[FLPTBase+2]&BIT3)==0; // Inverted
      default: return false; // pins 18-25 (GND), and other invalid pins
   }
}


//---------------------------------------------------------------------------
// SetPin()
//---------------------------------------------------------------------------
void __fastcall TDLPrinterPortIO::SetPin(BYTE Index, bool State)
{
   if (State)
      switch (Index)
      {
         case 1:  Port[FLPTBase+2]&=~BIT0; break; // Inverted
         case 2:  Port[FLPTBase]|=BIT0;    break;
         case 3:  Port[FLPTBase]|=BIT1;    break;
         case 4:  Port[FLPTBase]|=BIT2;    break;
         case 5:  Port[FLPTBase]|=BIT3;    break;
         case 6:  Port[FLPTBase]|=BIT4;    break;
         case 7:  Port[FLPTBase]|=BIT5;    break;
         case 8:  Port[FLPTBase]|=BIT6;    break;
         case 9:  Port[FLPTBase]|=BIT7;    break;
         /*
         case 10: Port[FLPTBase+1]|=BIT6;  break;
         case 11: Port[FLPTBase+1]&=~BIT7; break; // Inverted
         case 12: Port[FLPTBase+1]|=BIT5;  break;
         case 13: Port[FLPTBase+1]|=BIT4;  break;
         */
         case 14: Port[FLPTBase+2]&=~BIT1; break; // Inverted
         /*
         case 15: Port[FLPTBase+1]|=BIT3;  break;
         */
         case 16: Port[FLPTBase+2]|=BIT2;  break;
         case 17: Port[FLPTBase+2]&=~BIT3; break; // Inverted
         default: break; // pins 18-25 (GND), and other invalid pins
      }
   else
      switch (Index)
      {
         case 1:  Port[FLPTBase+2]|=BIT0;   break; // Inverted
         case 2:  Port[FLPTBase]&=~BIT0;    break;
         case 3:  Port[FLPTBase]&=~BIT1;    break;
         case 4:  Port[FLPTBase]&=~BIT2;    break;
         case 5:  Port[FLPTBase]&=~BIT3;    break;
         case 6:  Port[FLPTBase]&=~BIT4;    break;
         case 7:  Port[FLPTBase]&=~BIT5;    break;
         case 8:  Port[FLPTBase]&=~BIT6;    break;
         case 9:  Port[FLPTBase]&=~BIT7;    break;
         /*
         case 10: Port[FLPTBase+1]&=~BIT6;  break;
         case 11: Port[FLPTBase+1]|=BIT7;   break; // Inverted
         case 12: Port[FLPTBase+1]&=~BIT5;  break;
         case 13: Port[FLPTBase+1]&=~BIT4;  break;
         */
         case 14: Port[FLPTBase+2]|=BIT1;   break; // Inverted
         /*
         case 15: Port[FLPTBase+1]&=~BIT3;  break;
         */
         case 16: Port[FLPTBase+2]&=~BIT2;  break;
         case 17: Port[FLPTBase+2]|=BIT3;   break; // Inverted
         default: break; // pins 18-25 (GND), and other invalid pins
      }
}


//---------------------------------------------------------------------------
// LPTPrintChar
//    Sends a character to the printer.
//    Returns true on success. Repeat as neccessary.
//---------------------------------------------------------------------------
bool __fastcall TDLPrinterPortIO::LPTPrintChar(char Ch)
{
    // Write data to Base+0
    Port[FLPTBase]=(BYTE)Ch;
    // Write 0Dh to Base+2.
    Port[FLPTBase+2]=0x0D;
    // Make sure there's a delay of at least one microsecond
    Sleep(1);
    // Write 0Ch to Base+2.
    Port[FLPTBase+2]=0x0C;
    // Input from Base+1 and check if Bit 7 is 1.
    // Return this status as whether the character was printed
    return (Port[FLPTBase+1]&BIT7)!=0;
}


//---------------------------------------------------------------------------
namespace Dlportio
{
  void __fastcall PACKAGE Register()
   {
      TComponentClass classes[2] = {__classid(TDLPortIO),
                                    __classid(TDLPrinterPortIO)};
      RegisterComponents("DiskDude", classes, 1);
   }
}

#pragma package(smart_init)

