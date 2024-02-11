//---------------------------------------------------------------------------
// This C-Builder version of Willem's Eprom software was
// constructed by Mike Coates (mike.coates@btinternet.com)
//---------------------------------------------------------------------------
// IO Port Access made possible (under NT) by
// DriverLINX (c) 1996 Scientific Software Tools, inc.
// C-Builder Component tDLPortIO (c) 1999 John Pappas
//---------------------------------------------------------------------------
// Notes (anything I find out that may be useful later)
//
// ** Support for 27C1000 ? **
// To read a TC541001 (from a socket marked 27C1000)
// swop pins 2 (A16) and 24 (OE) over. Apply Vcc to Pin 31 (PGM)
//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <string.h>
#pragma hdrstop
#include "uEprom.h"
#include "willem.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cgauges"
#pragma link "TDLPortIO"
#pragma link "CGAUGES"
#pragma resource "*.dfm"


TfrmEprom *frmEprom;
//---------------------------------------------------------------------------
__fastcall TfrmEprom::TfrmEprom(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

#define MAXROMSIZE 0x100000
#define LDCONF  0
#define LDPROG  2
#define RDPROG  4
#define INCADD  6
#define BEGPRG  8
#define LDDATA  3
#define RDDATA  5
#define ENDPROG 14

#define CP      16
#define PWRTE   8
#define WDTE    4
#define RC      3
#define HS      2
#define XT      1
#define LP      0

#define PSIZE   1024
#define DSIZE   64

#define TICKS(us)   ((unsigned) (1.193182*(us)+1))  /* us to ticks */
#define MAX(a,b)    ((a > b) ? a : b)

// Easier to recognise Functions

#define WE_High     {pgrhi;wil_assert;}     // WE, code 8 = C17/S4 Pin 31 High
#define WE_Low      {pgrlo;wil_assert;}     // WE, code 8 = C17/S4 Pin 31 Low
#define OE_High     {read;wil_assert;}      // OE, code 2 = S6 high
#define OE_Low      {write;wil_assert;}     // OE, code 2 = S6 low
#define VPP_On      {ppon;wil_assert;}      // VPP, code 1 = C1 12v
#define VPP_Off     {ppoff;wil_assert;}     // VPP, code 1 = C1 0v

//---------------------------------------------------------------------------
// Global Variables
//---------------------------------------------------------------------------

unsigned char bits,outbit;

int poort  = 0x378;     // Default to LPT1
int ipoort = 0x379;
int spoort = 0x37a;

int qe = 0;             // Fast Erase Off
int st = 0;             // Offset = 0
int burn = 30;
int fast=0;             //ARW: added fast intelligent mode 1ms pulses doubling till done
int e8751=0;            //ARW: 8751 type flag
unsigned int grot=0;    // Size of Loaded data
int ZIFImage=0;         // ZIF Image to Display

int pe=0;
int C622,C50,C89,F84,C93;
int C66,C56,A46,B46;

int B56=0;              // Used to switch code blocks but always 0!
int zes=0;              // Used for a delay timing, but never set

int osc=1;int cp=0;int pt=0;int wd=0;

//int Epr.BufferSize;
short int fuses;        // PIC Fuse Settings
unsigned char Buffer[MAXROMSIZE];
unsigned short int *picbuf = (unsigned short int *)Buffer;
int FuseUpdate = FALSE;
int check;
int cancel=FALSE;       // Cancel selected
int erase = 0;          // Program or Erase Flag

FILE *fp;
int image_highwater=0;

char inp_line[160];     // NC modified for c++ builder v. 4.0
//unsigned char inp_line[160];  //for c++ builder v. 3.0

unsigned bytes[80];

LARGE_INTEGER PerformanceFrequency;

TImage *DipSwitch[14];
char dip[32]=" ¦ | ¦ ¦ ¦ ¦ ¦ ¦ ¦ ¦ ¦ ¦ ¦";


//--------------------------------------------

int adapter = 0;
unsigned char code_28F00x;
//------------------------------
class mcs51 	AT89Cxx;
class Eprom 	Epr,Epr1;
class spi25  	AT25xxx;
class m93c46 	m93Cxx;
class avr    	AT90Sxx;
class microchip mPIC;

//----------------- AT29Cxxx -------------
unsigned long  BB_29C_Ad_h[] = {0x5555,0x2AAA,0x5555,0x5555,0x2AAA,0x5555,0xFFFFF};
unsigned char  BB_29C_Da_h[] = {0xAA,0x55,0x80,0xAA,0x55,0x40,0xFF};
unsigned long  BB_29C_Ad_l[] = {0x5555,0x2AAA,0x5555,0x5555,0x2AAA,0x5555,0x00000};
unsigned char  BB_29C_Da_l[] = {0xAA,0x55,0x80,0xAA,0x55,0x40,0x00};

unsigned long  Adr_29C[] =    {0x5555,0x2AAA,0x5555,0x5555,0x2AAA,0x5555};
unsigned char  Erase_29C[] =  {0xAA,0x55,0x80,0xAA,0x55,0x10};
unsigned char  Get_IDw29E[] = {0xAA,0x55,0x80,0xAA,0x55,0x60};
unsigned char  EN_wr[] =      {0xAA,0x55,0xA0};
unsigned char  Get_ID[] =     {0xAA,0x55,0x90};
unsigned char  Rst_chip[] =   {0xAA,0x55,0xF0};

//------------------------------------------------------

/* Table from CRC32 by Mark Adler */
unsigned int crc_table[256] =
{
  0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
  0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
  0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
  0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
  0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
  0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
  0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
  0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
  0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
  0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
  0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
  0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
  0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
  0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
  0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
  0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
  0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
  0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
  0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
  0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
  0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
  0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
  0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
  0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
  0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
  0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
  0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
  0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
  0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
  0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
  0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
  0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
  0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
  0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
  0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
  0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
  0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
  0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
  0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
  0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
  0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
  0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
  0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
  0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
  0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
  0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
  0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
  0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
  0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
  0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
  0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
  0x2d02ef8dL
};

//---------------------------------------------------------------------------
// Windows routines to replicate ones used by Dos code
//---------------------------------------------------------------------------

void __fastcall ms_delay(int ticks)
{
   // Dos version Returns after roughly ticks*0.8381 microseconds
   // we convert figures used for Dos version to no of cycles.

   // Currently set so 1,000,000 = 1 second

   LARGE_INTEGER Count,EndCount;

   QueryPerformanceCounter(&Count);
   EndCount.QuadPart = Count.QuadPart + (ticks * PerformanceFrequency.QuadPart * 0.0000008381);
   while (Count.QuadPart < EndCount.QuadPart) QueryPerformanceCounter(&Count);
}

void ChecksumBuf(void)
{
	unsigned int crc;
    int y, grot;

	if(Epr.PIC) {grot=20000;}
    else {grot=Epr.BufferSize; }

    if(frmEprom->cbCheckType->ItemIndex == 0)
    {
      crc = 0;

      for( y=0; y<grot; y++ )
      {
          crc = Buffer[y];
      }

      crc &= 0xffff;
      frmEprom->edChecksum->Text = Format("0x%0.4X",ARRAYOFCONST(((int)crc)));
    }
    else
    {
      crc = 0xffffffff;
      for(y=0; y<grot; y++)
        crc = crc_table[((int)crc ^ (Buffer[y])) & 0xff] ^ (crc >> 8);

      frmEprom->edChecksum->Text = Format(" 0x%0.8X",ARRAYOFCONST(((int)(crc ^ 0xffffffff))));
    }
}

void winstat(void)
{
    // Updates display with current settings for device
    // and generally sorts out all of the display options.

    unsigned int Count,ImageNo;
    char buffer[50];

    // Update Dip Switches from Dos style string
  if(Epr.willem) {
        DipSwitch[2]  = frmEprom->dip1;
        DipSwitch[3]  = frmEprom->dip2;
        DipSwitch[4]  = frmEprom->dip3;
        DipSwitch[5]  = frmEprom->dip4;
        DipSwitch[6]  = frmEprom->dip5;
        DipSwitch[7]  = frmEprom->dip6;
        DipSwitch[8]  = frmEprom->dip7;
        DipSwitch[9]  = frmEprom->dip8;
        DipSwitch[10] = frmEprom->dip9;
        DipSwitch[11] = frmEprom->dip10;
        DipSwitch[12] = frmEprom->dip11;
        DipSwitch[13] = frmEprom->dip12;
        DipSwitch[1]  = frmEprom->JP_A18Vpp;
        frmEprom->sbWillem->Caption = "PCB3";
        frmEprom->pPCB3->Visible = true;
        frmEprom->pWillem->Visible = false;

        frmEprom->lbON->Top = 116;    //NC was 126
     }
     else {
        DipSwitch[2]  = frmEprom->dip12;
        DipSwitch[3]  = frmEprom->dip11;
        DipSwitch[4]  = frmEprom->dip10;
        DipSwitch[5]  = frmEprom->dip9;
        DipSwitch[6]  = frmEprom->dip8;
        DipSwitch[7]  = frmEprom->dip7;
        DipSwitch[8]  = frmEprom->dip6;
        DipSwitch[9]  = frmEprom->dip5;
        DipSwitch[10] = frmEprom->dip4;
        DipSwitch[11] = frmEprom->dip3;
        DipSwitch[12] = frmEprom->dip2;
        DipSwitch[13] = frmEprom->dip1;
        DipSwitch[1]  = frmEprom->JP_A18Vpp;
        frmEprom->sbWillem->Caption = "Willem";
        frmEprom->pPCB3->Visible = false;     //Left= 30, Top=110
        frmEprom->pWillem->Visible = true;
        frmEprom->lbON->Top = 140;     //NC was 150
     }

    for(Count=13;Count>0;Count--)
    {
      switch(dip[Count*2])
      {
          case ' '  : ImageNo = 0;break;
          case '0'  :  if(Epr.willem) ImageNo = 2;
                       else ImageNo = 1;
                       break;
          case '1'  :  if(Epr.willem)  ImageNo = 1;
                       else ImageNo = 2;
                       break;
          case '3'  : ImageNo = 3;break;
          case '4'  : ImageNo = 4;break;
          case '5'  : ImageNo = 5;break;
      }
      frmEprom->ilDip->GetBitmap(ImageNo,DipSwitch[Count]->Picture->Bitmap);
      DipSwitch[Count]->Refresh();
    }

    // Device Panel
    ChecksumBuf();
    sprintf(buffer, " 0x%X", Epr.BufferSize-1);
    frmEprom->edBuffer->Text = buffer;
    //--------------------------------------------
    frmEprom->p24CSettings->Visible = false;
    frmEprom->pF28Settings->Visible = false;
    frmEprom->p27CSettings->Visible = false;
    frmEprom->pPICSettings->Visible = false;
    frmEprom->p49F00xsetting->Visible = false;
    frmEprom->p29CxxxSetting->Visible = false;
    frmEprom->pMCS51Setting->Visible = false;
    frmEprom->pMXICF29040->Visible = false;     //NC added block protect panel
    // speed Button
    frmEprom->sbErase->Enabled = true;
    frmEprom->sbGetID->Enabled = true;
    frmEprom->sbLockBit->Enabled = false;
    frmEprom->sbVerify->Enabled = true;
    frmEprom->sbRead->Enabled = true;
    frmEprom->sbBlank->Enabled = true;
    frmEprom->sbRdLB->Enabled   = false;
    // menu
    frmEprom->mnuErase->Enabled = true;
    frmEprom->mnuGetID->Enabled = true;
//    frmEprom->mnuLockBit->Enabled = false;
    frmEprom->mnuVerify->Enabled = true;
    frmEprom->mnuRead->Enabled = true;
    frmEprom->mnuBlank->Enabled = true;

    switch(Epr.eep) {
        case M12C50X:
        case M16C62X:
        case M16F84:  frmEprom->pPICSettings->Visible =    true; break;
        case M24C16:
        case M24C32:  frmEprom->mnuGetID->Enabled = false;
                      frmEprom->sbGetID->Enabled = false;
                      frmEprom->p24CSettings->Visible =    true; break;
        case M28FXXX: frmEprom->pF28Settings->Visible =    true; break;
        case M27CXXX:
        case M27XXX:  frmEprom->sbErase->Enabled = false;
                      frmEprom->sbGetID->Enabled = false;
                      frmEprom->mnuErase->Enabled = false;
                      frmEprom->mnuGetID->Enabled = false;
                      frmEprom->p27CSettings->Visible =   true;
                      break;
        case M29FXXX: if (Epr.BufferSize == 0x80000)
                        {
                        frmEprom->pMXICF29040->Visible = true;
                        }
                        break; //NC visible only if 29F040
        case W27EXXX:
                      frmEprom->sbGetID->Enabled = false;
                      frmEprom->mnuGetID->Enabled = false;
                      frmEprom->p27CSettings->Visible =   true;
                      break;
        case M28CXXX: frmEprom->mnuGetID->Enabled = false;
                      frmEprom->sbGetID->Enabled = false;
                      frmEprom->p27CSettings->Visible =   true;
                      break;
        case AT49F00X: frmEprom->p49F00xsetting->Visible = true;
                      break;
        case AT89CX051:
        case AT89CXXX:  frmEprom->sbRdLB->Enabled   = true;
                        frmEprom->sbLockBit->Enabled = true;
                        frmEprom->pMCS51Setting->Visible = true;
                        switch(AT89Cxx.ModeLock) {
                            case 0: frmEprom->rbLock0->Checked = true;  break;
                            case 1: frmEprom->rbLock1->Checked = true;  break;
                            case 2: frmEprom->rbLock12->Checked = true; break;
                            case 3: frmEprom->rbLock123->Checked = true;break;
                        }
                        break;
        case AT28CXXX:  switch(Epr.tWC) {
                            case 5000: frmEprom->cb29C_twc->ItemIndex = 0; break;
                            case 10000: frmEprom->cb29C_twc->ItemIndex = 1; break;
                            case 15000: frmEprom->cb29C_twc->ItemIndex = 2; break;
                            case 20000: frmEprom->cb29C_twc->ItemIndex = 3; break;
                            case 25000: frmEprom->cb29C_twc->ItemIndex = 4; break;

                        }
                        switch(Epr.Ssize) {
                            case 64: frmEprom->cbSsize->ItemIndex = 0; break;
                            case 128: frmEprom->cbSsize->ItemIndex = 1; break;
                            case 256: frmEprom->cbSsize->ItemIndex = 2; break;
                            case 512: frmEprom->cbSsize->ItemIndex = 3; break;
                            case 1024: frmEprom->cbSsize->ItemIndex = 4; break;
                            case 2048: frmEprom->cbSsize->ItemIndex = 5; break;
                        }
                        frmEprom->p29CxxxSetting->Visible = true;
                        frmEprom->mnuGetID->Enabled = false;
                        frmEprom->sbGetID->Enabled = false;
                        break;
        case AT29CXXX:
        case W29EEXXX:  switch(Epr.tWC) {
                            case 5000: frmEprom->cb29C_twc->ItemIndex = 0; break;
                            case 10000: frmEprom->cb29C_twc->ItemIndex = 1; break;
                            case 15000: frmEprom->cb29C_twc->ItemIndex = 2; break;
                            case 20000: frmEprom->cb29C_twc->ItemIndex = 3; break;
                            case 25000: frmEprom->cb29C_twc->ItemIndex = 4; break;
                        }
                        switch(Epr.Ssize) {
                            case 64: frmEprom->cbSsize->ItemIndex = 0; break;
                            case 128: frmEprom->cbSsize->ItemIndex = 1; break;
                            case 256: frmEprom->cbSsize->ItemIndex = 2; break;
                            case 512: frmEprom->cbSsize->ItemIndex = 3; break;
                            case 1024: frmEprom->cbSsize->ItemIndex = 4; break;
                            case 2048: frmEprom->cbSsize->ItemIndex = 5; break;
                        }
                        frmEprom->p29CxxxSetting->Visible = true;
                        break;
     }

    // Settings and Position Panel
    if(Epr.PIC)
    {
        // PIC Settings Panel
        if(C50)
        {
            frmEprom->lPIC2->Caption = "Master Clear";
            frmEprom->cbOscillator->Items->Strings[2] = "iRC";
            frmEprom->cbOscillator->Items->Strings[3] = "eRC";
        }
        else
        {
            frmEprom->lPIC2->Caption = "Powerup Timer";
            frmEprom->cbOscillator->Items->Strings[2] = "HS";
            frmEprom->cbOscillator->Items->Strings[3] = "RS";
        }
        frmEprom->SetFuses();
    }

    // ZIF Position Panel
    frmEprom->img32->Visible =      false;
    frmEprom->img28->Visible =      false;
    frmEprom->img24->Visible =      false;
    frmEprom->img12c50->Visible =   false;
    frmEprom->img16x84->Visible =   false;
    frmEprom->img24xx->Visible =    false;
    frmEprom->img25xx->Visible =    false;
    frmEprom->img93xx->Visible =    false;
    frmEprom->imgTSOP48->Visible =  false;
    //  willem PCB
    frmEprom->imgwPIC12C50->Visible = false;
    frmEprom->imgwPIC16c84->Visible = false;
    frmEprom->imgwPIC24Cxx->Visible = false;
    frmEprom->imgwPIC25cxx->Visible = false;
    frmEprom->imgwPIC93Cxx->Visible = false;
    //------------------------------
    frmEprom->img8051->Visible = false;
    frmEprom->img2051->Visible = false;

    switch (ZIFImage)
    {
      case 32 : frmEprom->img32->Visible= true;      break;
      case 28 : frmEprom->img28->Visible= true;      break;
      case 24 : frmEprom->img24->Visible= true;      break;
      case 240: if(Epr.willem)  frmEprom->img24xx->Visible=true;
                else            frmEprom->imgwPIC24Cxx->Visible=true;
                break;
      case 25 : if(Epr.willem)  frmEprom->img25xx->Visible=true;
                else            frmEprom->imgwPIC25cxx->Visible=true;
                break;
      case 93 : if(Epr.willem)  frmEprom->img93xx->Visible=true;
                else            frmEprom->imgwPIC93Cxx->Visible=true;
                break;
      case 12 : if(Epr.willem)  frmEprom->img12c50->Visible=true;
                else            frmEprom->imgwPIC12C50->Visible=true;
                break;
      case 16 : if(Epr.willem)  frmEprom->img16x84->Visible=true;
                else            frmEprom->imgwPIC16c84->Visible=true;
                break;
      case 48 :     frmEprom->imgTSOP48->Visible=   true; break;
      case 51 :     frmEprom->img8051->Visible =    true; break;
      case 2051:    frmEprom->img2051->Visible =    true; break;
    }

    // Hex Grids
    frmEprom->HexGrid->Refresh();
    frmEprom->ProgGrid->Refresh();

    // Get rid of Progress Bar
    frmEprom->gProcess->Visible = false;
    frmEprom->btnCancel->Visible = false;

   // show shift address, pattern for shift address
    sprintf(buffer," A0 --> A%d",Epr.shift_adr);
    frmEprom->edShiftAdr->Text = buffer;
    sprintf(buffer," 0x%0.6X",Epr.pattern_adr);
    frmEprom->edPatternAdr->Text = buffer;
    status_8051();
    status_Eprom();

   // show tWP tWC
    show_twptwc();
}

void Dip(int DIPid)
{
    switch(DIPid)
    {
        case  0 : strcpy(dip," |4| | | | | | | | | | | | |");break;    // serial prog.

        case  1 : strcpy(dip," |4|1|1|0|1|0|1|0|0|1|0|0|0|");break;    // 27C64+
        case  2 : strcpy(dip," |4|1|1|0|1|0|1|0|0|1|0|0|0|");break;    // 27C128+
        case  3 : strcpy(dip," |4|1|1|0|0|1|1|0|1|1|0|0|0|");break;    // 27C256+
        case  4 : strcpy(dip," |4|0|0|1|0|1|0|1|1|1|0|0|0|");break;    // 27C512+
        case  5 : strcpy(dip," |4¦0¦0¦0¦1¦0¦1¦0¦0¦1¦0¦0¦0¦");break;    // 28C64
        case  6 : strcpy(dip," |4¦0¦0¦0¦0¦1¦1¦0¦1¦1¦0¦1¦0¦");break;
        case  7 : strcpy(dip," |4¦0¦0¦1¦0¦1¦1¦0¦0¦0¦1¦1¦0¦");break;    //27C010/020
        case  8 : strcpy(dip," |4¦0¦0¦1|0¦1¦1¦0¦1¦1¦0¦1¦0¦");break;
        case  9 : strcpy(dip," |4¦0¦0¦0¦0¦0¦0¦1¦1¦1¦0¦0¦0¦");break;    //27C32
        case 10 : strcpy(dip," |4¦0¦0¦1¦0¦1¦1¦0¦1¦0¦1¦0¦1¦");break;    //27C040
        case 11 : strcpy(dip," |4¦0¦0¦1¦0¦1¦1¦0¦1¦1¦0¦0¦1¦");break;
        case 12 : strcpy(dip," |4¦0¦0¦1¦0¦1¦1¦0¦0¦0¦1¦0¦1¦");break;
//--------------
        case 13 : strcpy(dip," |4¦0¦1¦1¦0¦1¦1¦0¦1¦0¦1¦0¦1¦");break;   // MCS-51 40 pin
        case 14 : strcpy(dip," |4¦0¦0¦1¦0¦1¦1¦0¦1¦0¦1¦0¦1¦");break;   // AT89C2051 20 pin
        case 16 : strcpy(dip," |4¦0¦1¦1¦0¦1¦1¦0¦0¦0¦1¦1¦0¦");break;   // 49F001/002
        case 18 : strcpy(dip," |4¦0¦0¦1¦1¦0¦1¦0¦0¦1¦0¦0¦0¦");break;   // 28C256
        case 19 : strcpy(dip," |5¦0¦0¦1¦0¦1¦1¦0¦0¦0¦1¦1¦0¦");break;   // 29C/29F/39F/49F040
        case 20 : strcpy(dip," |4¦0¦0¦1¦0¦1¦1¦0¦1¦0¦1¦0¦1¦");break;   // 28F/29F200-160
        case 21 : strcpy(dip," |3¦0¦0¦1¦0¦1¦1¦0¦1¦0¦1¦0¦1¦");break;    //27C080

        case 24 : strcpy(dip," |4¦0¦0¦1¦1¦0¦1¦0¦0¦0¦1¦0¦1¦");break;   // 62512,628128,DS1245,DS1249
        case 25 : strcpy(dip," |4¦0¦0¦1¦1¦0¦1¦0¦0¦1¦0¦0¦0¦");break;   // 6264,62256,DS1225 DS1260
        case 26 : strcpy(dip," |4¦0¦0¦0¦1¦0¦1¦0¦0¦1¦0¦1¦0¦");break;   // 6116,2816,28C16
        case 27 : strcpy(dip," |4¦1¦1¦0¦0¦0¦1¦0¦1¦1¦0¦0¦0¦");break;   // 27C16,2716
        case 28 : strcpy(dip," |4¦0¦0¦0¦0¦1¦1¦0¦0¦1¦0¦1¦0¦");break;   // willem AT29C256
        case 29 : strcpy(dip," |4¦0¦0¦1¦0¦1¦1¦0¦0¦1¦0¦1¦0¦");break;   // PCB3   AT29C256
        case 30 : strcpy(dip," |4¦0¦0¦1¦0¦1¦1¦0¦0¦1¦0¦1¦0¦");break;   // 28F001
    }
    Epr.DIP = DIPid;
    winstat();
}

//---------------------------------------------------------------------------
// Output to LPT port routines
//---------------------------------------------------------------------------
void __fastcall Outport(int IOadd, byte data)
{
    frmEprom->DLPortIO->Port[IOadd] = data;
}

byte __fastcall Inport(int IOadd)
{
    return frmEprom->DLPortIO->Port[IOadd];
}

//---------------------------------------------------------------------------
// Routines from Dos version
//---------------------------------------------------------------------------

void __fastcall clock_out(unsigned long bit)
{
         bit? adrhi: adrlo;asadres;
         ms_delay(1);
         clkhi, asadres;
         ms_delay(1);
         clklo, asadres;
         ms_delay(1);
         adrlo, asadres;
}

void __fastcall eclock_out(int bit)			// DW: ANSIfied
{
	 bit? outhi:outlo;asadres;
	 ms_delay(2);
	 adrhi;asadres;
	 ms_delay(2);
	 adrlo; asadres;
	 ms_delay(2);
	 outlo;asadres;
}

void __fastcall r_adres_28C(unsigned long adr)
{
	unsigned long bit,r;
	int b;

	r = Epr.pattern_adr;
	Outport(poort,0x00);
	for (b=(Epr.shift_adr+1); b!=0; b--){
		bit=(adr&r);
		bit ? adrhi: adrlo;asadres;
		clkhi, asadres;
		clklo, asadres;
		r=r>>1;
	}
	adrlo; asadres;
}

int clock_in(void)			// DW: added void
{
	 int b;
//	 ms_delay(1);
	 b = inbit? 0: 1;
	 clahi, asadres;
//	 ms_delay(1);
	 clalo;asadres;
	 return b;
}

int eclock_in(void)			// DW: added void
{
	 int b;

	 outhi;adrlo; asadres;
	 ms_delay(2);
	 adrhi;asadres;
	 ms_delay(2);
	 b = einbit? 0: 1;
	 ms_delay(2);
	 adrlo;asadres;
	 return b;
}

void __fastcall pclock_out(int bit)	// DW: ANSIfied
{
	bit? outhi: outlo, adrhi, asadres;
	ms_delay(2);
	adrlo, asadres;
	ms_delay(2);
	outlo, asadres;
}

int pclock_in(void)			// DW: added void
{
	int b;

	outhi, adrhi, asadres;
	ms_delay(2);
	adrlo, asadres;
	b = einbit? 0: 1;
	ms_delay(2);
	return b;
}

void __fastcall out_word(unsigned char w) // DW: added void
{
	 int b;

	 for ( b=7; b>=0;--b )
		 eclock_out(w&(1<<b));
}

int in_word(void)			// DW: added void
{
	 int b, w;
	 adrhi;asadres;
//	 ms_delay(1);
	 clahi;asadres;
//	 ms_delay(1);
	 adrlo;asadres;clalo;asadres;

	 for ( w=0, b=7; b>=0; --b)
		 w += clock_in()<<b;
	 return w;
}

int ein_word(void)			// DW: added void
{
	int b, w;

	for ( w=0, b=7; b>=0; --b )
		w += eclock_in()<<b;

	return w;
}

void __fastcall pout_word(int w)		// DW: ANSIfied
{
	int b;

	pclock_out(0);
	for ( b=0; b<14; ++b )
	  pclock_out(w&(1<<b));
	pclock_out(0);
}

int pin_word(void)			// DW: added void
{
	int b, w;

	(void) pclock_in();
	for ( w=0, b=0; b<14; ++b )
	  w += pclock_in()<<b;
	(void) pclock_in();
	return w;
}

void __fastcall command(int cmd)		// DW: ANSIfied
{
	int b;

	outlo, asadres;
	ms_delay(5);
	for ( b=0; b<6; ++b )
		pclock_out(cmd&(1<<b));
	outhi, asadres;
	ms_delay(5);
}

void power_down(void)     // ARW: added proc to set all lines to low on
{                           //exit to avoid cmos latchup & allow chip removal
    r_adres_28C(0);
    read,adrlo,pgrlo,ppoff,powoff,clklo,clalo,wil_assert;asadres;
    r_adres_28C(0);
}

void setup(void)        // DW: added void
{
    r_adres_28C(0);             //ARW: added, make sure we know where

    read,adrlo,pgrhi,ppoff,powoff,clklo,clalo,wil_assert;asadres;
    ms_delay(25000);powon;wil_assert;       //ARW: increased delays was 5,50
    ms_delay(30000);
}

void start(void)			// DW: added void
{
	outlo;asadres;ms_delay(5);
	adrlo;asadres;ms_delay(5);
}

void stop(void)			    // DW: added void
{
	adrhi;asadres;ms_delay(5);
	outhi;asadres;ms_delay(5);
}

int strct(void)			// DW: added void
{
	start(),out_word(CTW);
	if(!eclock_in()) return 1;
	else return 0;
}

int strctR(void)			// DW: added void
{
	start(),out_word(CTR);
	if(!eclock_in()) return 1;
	else return 0;
}

void prog_mode(void)			// DW: added void
{
	ppoff, powon, adrlo, outlo, wil_assert;asadres;
	ms_delay(TICKS(1000));
	ppon, wil_assert;
	ms_delay(12000);
}

void idle_mode(void)				// DW: added void
{
	ppoff, adrlo, outlo, wil_assert;asadres;
	ms_delay(TICKS(10000));
	powoff, wil_assert;
}

void p5_mode(void)			// DW: added void
{
	ppoff, clklo, outlo, wil_assert;
	ms_delay(3000);

	power_down();		//ARW: added
	//powoff, wil_assert;
}

int mclock_in(void)			// DW: added void
{
	 int b;

	 ms_delay(2);
	 adrhi;asadres;
	 ms_delay(2);
	 b = einbit? 0: 1;
	 ms_delay(2);
	 adrlo;asadres;
	 return b;
}

int min_word(void)			// DW: added void
{
	int b, w=0;
	adrlo;asadres;
	for (b=7;b>=0;b-- )
	    w = w |( mclock_in()<<b);
    adrlo;asadres;
	return w;
}

void __fastcall mclock_out(int bit)			// DW: ANSIfied
{
	bit? outlo:outhi;asadres;
	ms_delay(2);
	adrhi;asadres;
	ms_delay(2);
	outhi;asadres;
    ms_delay(2);
	adrlo; asadres;
}

void __fastcall mout_word(unsigned char w)			// DW: added void
{
	 int b, di;

	 for ( b=7; b>=0;--b )
	 {
         di=(w>>b) & 0x01;
		 mclock_out(di);
     }
}

int __fastcall W_BYTE(unsigned char out)			// DW: ANSIfied
{
	out_word(out);
	if(!eclock_in()) return 1;
	else return 0;
}


int test_hw(void)           //DW: added void
{
    int result;
    int l,r;

    setup();
    OE_Low;
    powon;wil_assert;ms_delay(15000);
    outhi;asadres;ms_delay(1000);
    r=einbit;
    outlo;asadres;ms_delay(1000);
    l=einbit;

    if(l==128 && r==0)
    {
        result = 0;
        frmEprom->Message("Hardware present ",2);
    }
    else
    {
        result = 1;
        frmEprom->Message("Hardware Error :  Check Power && connection",1);
    }
    power_down();           //ARW: was powoff
    OE_High;            //ARW: duplication

    return result;
}

void __fastcall pr622(int i)
{
	command(LDPROG);
	ms_delay(1200);
	pout_word(picbuf[i]);
	ms_delay(4);
	command(BEGPRG);
	ms_delay(119);
	command(ENDPROG);
}

void error(String Notice)
{
	if(!test_hw())
      frmEprom->Message(Notice,1);

	power_down();           //ARW: was powoff
	VPP_Off;           //ARW duplication also in power_down
}

void getoff()
{
  // Confirm that we wish to run with the Offset specified
  if (MessageDlg(Format("Use offset of $%4.4x",ARRAYOFCONST(((int)(st)))), mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrNo)
  {
    frmEprom->edOffset->Text = "0000";
    st = 0;
  }
}

int config(void)			// DW: added void
{
	int i;
    fuses = picbuf[0x2007];
	prog_mode();
	command(LDCONF);
	pout_word(fuses);
	for ( i=0; i<7; ++i )
		command(INCADD);
	command(LDPROG);
	pout_word(fuses);
	command(BEGPRG);
	ms_delay(TICKS(12000));
	command(RDPROG);
	if ( fuses != (pin_word()&0x1F) )
    {
        error("Error blowing fuses");
		idle_mode();
		return 0;
	}
	return 1;
}

void ClearBuf(void)
{
  frmEprom->Clear1Click(NULL);
}

//---------------------------------------------------------------------------
// Check Empty
//---------------------------------------------------------------------------

int Check(void)
{
    int Answer=-1;
	unsigned long i,index;
	unsigned char h;

  	if(st!=0) getoff();

    frmEprom->SetProcessGauge("Checking Device is Empty",Epr.BufferSize,clWhite);

	setup();

	if(Epr.epr64){WE_High;}
    else { WE_Low; }

	for(i=0;i<Epr.BufferSize;i++)
    {
        index = i+st;
   		if(Epr.swap == ON) index =   swap_adr(index);

   		r_adres_28C(index);
		h=in_word();
		if(h!=0xFF)
        {
            Answer = i+st;
			break;
		}
        frmEprom->gProcess->Progress = i;
        Application->ProcessMessages();
        if(cancel)
        {
          Answer = -2;
          break;
        }
	}
	power_down();
    winstat();
    return Answer;
}

//---------------------------------------------------------------------------
// Verify Device Routines
//---------------------------------------------------------------------------

int verify(void)
{
	int i, w, mask, rdcmd;	// DW: also had *buf, ldcmd, n but they are unused

    frmEprom->SetProcessGauge("verifying PIC program area",PSIZE-1,clGreen);

	OE_Low;

	mask = 0x3FFF;

	rdcmd = RDPROG;
	prog_mode();
	for ( i=0; i<PSIZE; ++i )
    {
		command(rdcmd);
		w = pin_word() & mask;
		if ( w != picbuf[i])
        {
            error("Error verifying PIC");
			idle_mode();
			return 0;
		}

		command(INCADD);

        frmEprom->gProcess->Progress = i;
	}

	mask = 0xFF;

	rdcmd = RDDATA;
	prog_mode();
	ms_delay(12000);

    frmEprom->SetProcessGauge("verifying PIC data area",DSIZE-1,clGreen);

	for ( i=0; i<DSIZE; ++i )
    {
		command(rdcmd);
		w = pin_word() & mask;
		if ( w != picbuf[i+0x2100] )
        {
            error("Error verifying PIC");
			idle_mode();
			return 0;
		}

		command(INCADD);

        frmEprom->gProcess->Progress = i;
	}

	idle_mode();
	OE_High;

    frmEprom->Message("Device Verified OK",2);
    winstat();
	return 1;
}

int verify50(void)
{
	unsigned int i;
    int  w, mask, rdcmd;		//DW: *buf, ldcmd, n unused

    frmEprom->SetProcessGauge("verifying PIC program area",(Epr.BufferSize/2)-1,clGreen);

	OE_Low;
	mask = 0x3FFF;

	rdcmd = RDPROG;
	prog_mode();
	command(INCADD);
	for ( i=0; i<(Epr.BufferSize/2)-1; ++i )
    {
		command(rdcmd);
		ms_delay(1200);
		w = pin_word() & mask;
		if ( w != picbuf[i])
        {
            error(Format("Error at $%6x",ARRAYOFCONST(((int)(i)))));
        	idle_mode();
			return 0;
		}

		command(INCADD);

        frmEprom->gProcess->Progress = i;
	}

	idle_mode();
	OE_High;
	return 1;
}

void Compare(void)
{
	unsigned int  i, y,q;
    unsigned char g;
    bool OK = true;

  	if(st!=0)getoff();

	setup();

	if(Epr.epr64){WE_High;}
    else { WE_Low; }

	if(grot>=Epr.BufferSize) grot=Epr.BufferSize-1;

    frmEprom->SetProcessGauge("Comparing chip",grot-1,clGreen);

	for(i=0;i<=grot;i++)
    {
		y=i;
rep:
		r_adres_28C(i+st);
        g = in_word();
		if(g!=Buffer[y]) {
            if (q == 5) {
                error2(i,st,g);
	    		power_down();/*wil_assert;*/         //ARW: was powoff
                OK = false;
			    break;//for i
		    }else {q++; goto rep; }
        }

        frmEprom->gProcess->Progress = i;
        Application->ProcessMessages();
        if(cancel)
        {
          frmEprom->Message("program interrupted!",1);
          OK = false;
          break;
        }
        q=1;
	}

    if((OK) && (!cancel)) frmEprom->Message("Device Compares OK",2);

	power_down();/*wil_assert;*/          //ARW: was powoff
    winstat();
}

void verify_65(void)
{
	unsigned char adrby1, adrby2;
	unsigned int i;
    bool OK=true;

	unsigned char g;

    frmEprom->SetProcessGauge("Comparing chip",grot,clGreen);

	outhi,adrhi;asadres;

    if(Epr.pin7)WE_High
    else WE_Low;

	powon;wil_assert; OE_Low;
	ms_delay(15000);

	for(i=0;i<grot;i++)
    {
		adrby1=i>>8;
		adrby2=i;

		if(strct())
        {
			if(W_BYTE(adrby1))
            {
				if(W_BYTE(adrby2))
                {
					start();
					stop();
					if(strctR())
                    {
						g=ein_word();eclock_out(1);stop();
					}
					else		//DW: clean up a bit
                    {
						error("Unknown Error");
                        OK=false;
						break;
					}
				}
				else
                {
					error("Unknown Error");
                    OK=false;
					break;
				}
			}
			else
            {
				error("Unknown Error");
                OK=false;
				break;
			}
		}
		else
        {
			error("Unknown Error");
            OK=false;
			break;
		}

		if(g!=Buffer[i])
        {
            error(Format("Error at $%4.4x",ARRAYOFCONST(((int)(i)))));
            OK=false;
			break;
		}

        frmEprom->gProcess->Progress = i;

        Application->ProcessMessages();
        if(cancel)
        {
          frmEprom->Message("program interrupted!",1);
          break;
        }
	}

    if ((OK) && (!cancel)) frmEprom->Message("Device Compares OK",2);

	idle_mode();
    winstat();
}

void verify_16(void)		//DW: added void
{
	unsigned char adrby1, adrby2;
	unsigned int i;
    bool OK=true;
	unsigned char g;

    frmEprom->SetProcessGauge("Comparing chip",grot,clGreen);

	outhi,adrhi;asadres;

    if(Epr.pin7)WE_High
    else WE_Low;

	powon;wil_assert;

	OE_Low;
	ms_delay(15000);
	for(i=0;i<grot;i++)
    {
		adrby1=i>>7;
		adrby1&=126;
		adrby1=adrby1+CTW;
		adrby2=i;

		start();
		if(W_BYTE(adrby1))
        {
			if(W_BYTE(adrby2))
            {
				start();
				stop();
				adrby1=adrby1+1;
				start();
				if(W_BYTE(adrby1))
                {
					g=ein_word();
					eclock_out(1);
					stop();
				}
				else
                {
					error("Unknown Error");
                    OK=false;
					break;
				}
			}
			else
            {
				error("Unknown Error");
                OK=false;
				break;
			}
		}
		else
        {
			error("Unknown Error");
            OK=false;
			break;
		}

		if(g!=Buffer[i])
        {
            error(Format("Error at $%4.4x",ARRAYOFCONST(((int)(i)))));
            OK=false;
			break;
		}

        frmEprom->gProcess->Progress = i;

        Application->ProcessMessages();
        if(cancel)
        {
          frmEprom->Message("program interrupted!",1);
          break;
        }
	}

    if ((OK) && (!cancel)) frmEprom->Message("Device Compares OK",2);

	idle_mode();
    winstat();
}

//---------------------------------------------------------------------------
// Erase Device Routines
//---------------------------------------------------------------------------

void P_erase(void)
{
	int i;
	OE_Low;

	prog_mode();

	command(LDCONF);
	pout_word(0x3FFF);
	for ( i=0; i<7; ++i )
		command(INCADD);
	command(1);
	command(7);
	command(BEGPRG);
	ms_delay(TICKS(12000));
	command(1);
	command(7);
	idle_mode();
	OE_High;
}


//-------------------------------------
// Called by Erase routin
void erase_f28(void)
{
	int  q=1;
    unsigned int i=0;
    bool OK = true;

	if(st!=0) getoff();

	erase=1;
	if(!qe)
    {
      prog_f28();
    }
    else
    {
      frmEprom->SetProcessGauge("erasing chip",Epr.BufferSize,clWhite);

      setup();
      WE_High;
      r_adres_28C(0x20000);
      VPP_On;

F:
      WE_High;

      OE_Low;
      ms_delay(5);
      Outport(poort,0x20);

      ms_delay(5);
      WE_Low;ms_delay(TICKS(6));WE_High;

      ms_delay(1);
      Outport(poort,0x20);
      WE_Low;ms_delay(TICKS(6));WE_High;
      ms_delay(10000);
      do
      {
          Outport(poort,0xA0);
          ms_delay(5);
          WE_Low;ms_delay(TICKS(6));WE_High;
          ms_delay(TICKS(6));
          adrlo;clklo;clalo;asadres;
          ms_delay(5);
          OE_High;
          ms_delay(5);
          if(in_word()!=0xff)
          {
              if(q==1000)
              {
                  error("Error erasing chip");
                  OK = false;
                  ppoff;
                  power_down();/*wil_assert;*/         //ARW: was powoff
                  break;
              }
              else
              {
                  q++;
                  goto F;
              }
          }

          if(qe) i=i+20;
          else i++;

          r_adres_28C((i+st)|0x20000);

          frmEprom->gProcess->Progress = i;

          Application->ProcessMessages();
          if(cancel)
          {
              frmEprom->Message("Erase interrupted!",1);
              OK = false;
              break;
          }

      } while(i<=Epr.BufferSize);
    }

    if(OK) frmEprom->Message("Erase Completed",2);

	power_down();                  //ARW was powoff
	VPP_Off;                  //ARW: duplication
	erase=0;
    winstat();
}

//---------------------------------------------------------------------------
// Write Device Routines
//---------------------------------------------------------------------------

void PIC508_pr(void)
{
	unsigned int i;
    int x, w,te, mask, rdcmd;
	int q=0;

    if(Application->MessageBox("This routine is known to have problems!", "Warning", MB_OKCANCEL + MB_DEFBUTTON1) == IDOK)
    {
      frmEprom->SetProcessGauge("programming PIC program area",(Epr.BufferSize/2)-1,clRed);

      mask = 0x3FFF;

      rdcmd = RDPROG;
      OE_Low;
      prog_mode();
      ms_delay(14);
      command(INCADD);

      for ( i=0; i<(Epr.BufferSize/2)-1; ++i )
      {
          W:
              pr622(i);
              ms_delay(4);
              command(rdcmd);
              ms_delay(4);
              w = pin_word() & mask;
              if ( w != picbuf[i])
              {
                  q++;
                  if(q>25)
                  {
                      error("Error writing program area");
                      idle_mode();
                      return;
                  }
                  else goto W;
              }
              for(x=0;x<(q*3);x++)
                  pr622(i);
              command(INCADD);

              frmEprom->gProcess->Progress = i;
              q=0;
      }

      if(verify50())
      {
          frmEprom->SetProcessGauge("programming PIC config area",1,clRed);

          fuses=picbuf[0x2007];

          p5_mode();
          ms_delay(5000);
          prog_mode();

          ms_delay(4000);

          OE_Low;
          for ( i=0; i<100; ++i )
              pr622(0x2007);

          p5_mode();
          ms_delay(5000);
          prog_mode();
          command(RDPROG);
          te = pin_word()&0x1F;
          if ( fuses != te)
          {
              p5_mode();
              error("Error writing configuration");
              idle_mode();
              return;
          }

          ms_delay(35000);
      }
    }
	idle_mode();
	OE_High;
    winstat();
}

void PIC_pr(void)
{
	int i, w, mask, ldcmd, rdcmd;

	P_erase();

    frmEprom->SetProcessGauge("programming PIC program area",PSIZE,clRed);

	mask = 0x3FFF;
	ldcmd = LDPROG;
	rdcmd = RDPROG;
	prog_mode();
	OE_Low;

	ms_delay(5000);
	for ( i=0; i<PSIZE; ++i )
    {
		command(ldcmd);
		ms_delay(100);
		pout_word(picbuf[i]);
		ms_delay(100);
		command(BEGPRG);
		ms_delay(TICKS(12000));
		command(rdcmd);
		ms_delay(100);
		w = pin_word() & mask;
		if ( w != picbuf[i])
        {
            error("Error writing PIC program");
			idle_mode();
			return;
		}
		ms_delay(100);
		command(INCADD);
		ms_delay(100);

        frmEprom->gProcess->Progress = i;
	}

    frmEprom->SetProcessGauge("programming PIC data area",DSIZE,clRed);

	mask = 0xFF;
	ldcmd = LDDATA;
	rdcmd = RDDATA;
	prog_mode();

	for ( i=0; i<DSIZE; ++i )
    {
		command(ldcmd);
		pout_word(picbuf[i+0x2100]);
		command(BEGPRG);
		ms_delay(TICKS(12000));
		command(rdcmd);
		w = pin_word() & mask;
		if ( w != picbuf[i+0x2100] )
        {
            error("Error writing PIC data");
			idle_mode();
			return;
		}
		command(INCADD);

        frmEprom->gProcess->Progress = i;
	}

	if(verify())
    {
		OE_Low;
        frmEprom->SetProcessGauge("programming PIC config area",1,clRed);
		config();
	}

	ms_delay(35000);
	power_down();VPP_Off;
	OE_High;
    winstat();
}

//-------------------------------------
void prog_f28(void)
{
	unsigned int i, y;
	byte g;
	int q=1;
    int hgrot= grot;
    bool OK = true;

	if(st!=0) getoff();

	if(grot>=Epr.BufferSize) grot=Epr.BufferSize-1;

	if(erase)
    {
      grot=Epr.BufferSize-1;
      frmEprom->SetProcessGauge("Erasing chip",grot,clWhite);
    }
    else
      frmEprom->SetProcessGauge("programming chip",grot,clRed);

	setup();

	WE_High;
	r_adres_28C(0);
	VPP_On;

	for(i=0;i<=grot;i++)
    {
		y=i;

		if(erase) g=0xff;
		else g=Buffer[y];

r:
		we_hi;oe_lo;
		r_adres_28C(i+st);
		oe_hi;
		ms_delay(5);
		Outport(poort,0x40);

		ms_delay(10);
		WE_Low;ms_delay(6);WE_High;
		ms_delay(10);

		Outport(poort,g);
		WE_Low;ms_delay(6);WE_High;
		ms_delay(10);

		Outport(poort,0xC0);
		ms_delay(5);
		WE_Low;ms_delay(Epr.tWP) ;WE_High;
		ms_delay(Epr.tWC);

		Outport(poort,0x00);
		oe_lo;
		ms_delay(5);

		if(g!=(unsigned char)in_word())
        {
			if(q==20)
            {
                g = (unsigned char)in_word();
				error2(i,st,g);
				power_down();		//ARW: was powoff
				VPP_Off;       //ARW: duplication
				goto P8E;
			}
			else
            {
				q++;
				goto r;
			}
		}

        frmEprom->gProcess->Progress = i;

        Application->ProcessMessages();
        if(cancel)
        {
          frmEprom->Message("program interrupted!",1);
          break;
        }
		q=1;
	}

	VPP_Off;

	if(erase)
    {
   	  grot=hgrot;
      return;
    }

    if(!cancel)
    {
      frmEprom->SetProcessGauge("verifying chip",grot,clGreen);

      for(i=0;i<=grot;i++)
      {
          y=i;

          r_adres_28C(i+st);
          if(in_word()!=Buffer[y])
          {
              frmEprom->Message(Format("Error at $%6x",ARRAYOFCONST(((int)(i+st)))),1);
              power_down();
              OK = false;
              break;
          }

          frmEprom->gProcess->Progress = i;
      }

      if(OK) frmEprom->Message("Device Compares OK",2);
    }

P8E:
	grot=hgrot;
	power_down();/*wil_assert;*/ 			//ARW: was powoff
    winstat();
}


void eprogram65(void)
{
	unsigned char adrby1, adrby2;
	unsigned int i;
	int w=0;
	unsigned char g,k;
    bool OK=true;

    frmEprom->SetProcessGauge("programming chip",Epr.BufferSize,clRed);

	outhi,adrhi;asadres;

    if(Epr.pin7)WE_High
    else WE_Low;

	OE_Low;
	powon;wil_assert;

	ms_delay(30000);
	for(i=0;i<Epr.BufferSize;i++)
    {
    	g=Buffer[i];

		adrby1=i>>8;
		adrby2=i;

		if(strct())
        {
			if(W_BYTE(adrby1))
            {
				if(W_BYTE(adrby2))
                {
					if(W_BYTE(g))
                    {
						outlo;asadres;stop();
					}
					else
                    {
						error("Unknown Error");
                        OK=false;
						break;
					}
				}
			}
			else
            {
				error("Unknown Error");
                OK=false;
				break;
			}
		}
		else
        {
			error("Unknown Error");
            OK=false;
			break;
		}
		W:
		if(strct())
        {
			if(W_BYTE(adrby1))
            {
				if(W_BYTE(adrby2))
                {
					start();
					stop();
					if(strctR())
                    {
						k=ein_word();eclock_out(1);stop();
					}
					else
                    {
						error("Unknown Error");
                        OK=false;
						break;
					}
				}
				else
                {
					error("Unknown Error");
                    OK=false;
					break;
				}
			}
			else
            {
				error("Unknown Error");
                OK=false;
				break;
			}
		}
		else
        {
			ms_delay(TICKS(1000));
			w++;
			if(w==10)
            {
				error("Too many Retries - would not program");
                OK=false;
                break;
			}
			stop();
			goto W;
		}
		if(k!=g)
        {
			error("Verify Error");
            OK=false;
			break;
		}
		w=0;

        frmEprom->gProcess->Progress = i;
	}

    if (OK) frmEprom->Message("Device Programmed OK",2);

	power_down();		//ARW: replaces powoff
	OE_High;        //duplication but left for clarity
    winstat();
}

void prog_16(void)
{
	unsigned char adrby1, adrby2;
	unsigned int i;
	int w=0;
	int g,k;
    bool OK=true;

    frmEprom->SetProcessGauge("programming chip",Epr.BufferSize,clRed);

	OE_Low;
	outhi,adrhi;asadres;

    if(Epr.pin7)WE_High
    else WE_Low;

	powon;wil_assert;
	ms_delay(30000);
	for(i=0;i<Epr.BufferSize;i++)  {
		g=Buffer[i];

		adrby1=i>>7;
		adrby1&=126;
		adrby1=adrby1+CTW;
		adrby2=i;

		start();
		if(W_BYTE(adrby1)) {
			if(W_BYTE(adrby2)) {
				if(W_BYTE(g)) { outlo;asadres;stop();ms_delay(TICKS(1000)); }
				else { error("Unknown Error"); OK=false; break; }
			}
		}else {	error("Unknown Error"); OK=false; break; }
W:
		start();
		if(W_BYTE(adrby1)) {
			if(W_BYTE(adrby2)) {
				start();
				stop();
				adrby1=adrby1+1;
				start();
				if(W_BYTE(adrby1)) {
					k=ein_word();
					eclock_out(1);
					stop();
				} else { error("Unknown Error"); OK=false; break; }
			} else { error("Unknown Error");OK=false; break;}
		}else {
			ms_delay(TICKS(1000));
			w++;
			if(w==10){ error("Unknown Error"); OK=false; break;	}
			stop();
			goto W;
		}

		if(k!=g) {error("Verify Error"); OK=false; break;}
		w=1;
        frmEprom->gProcess->Progress = i;
	}
	power_down();
	OE_High; 		//ARW: duplication but left for clarity

    if (OK) frmEprom->Message("Device Programmed OK",2);
    winstat();
}


//---------------------------------------------------------------------------
// Read Device Routines
//---------------------------------------------------------------------------

void P50read(void)
{
	unsigned int i;
    int mask, rdcmd;

    frmEprom->SetProcessGauge("Reading PIC program area",(Epr.BufferSize/2),clBlack);

	OE_Low;

	mask = 0x3FFF;

	rdcmd = RDPROG;
	prog_mode();
	command(INCADD);

	for (i=0;i<(Epr.BufferSize/2);++i)
    {
		command(rdcmd);
		picbuf[i] = pin_word() & mask;
		command(INCADD);
        frmEprom->gProcess->Progress = i;

        Application->ProcessMessages();
        if(cancel)
        {
          frmEprom->Message("program interrupted!",1);
          idle_mode();break;
        }
	}

    if(!cancel)
    {
  	  idle_mode();
	  prog_mode();
	  command(RDPROG);
      picbuf[0x2007] = pin_word()&0x1F;
	  fuses = picbuf[0x2007];
    }

	idle_mode();OE_High;
    if(!cancel)
      frmEprom->Message("PIC dumped OK",0);
    else
      frmEprom->Message("Process was Cancelled",1);
    winstat();
}

void Pread(void)
{
	unsigned int i, mask, rdcmd;

    frmEprom->SetProcessGauge("Reading PIC program area", Epr.BufferSize,clBlack);

	OE_Low;

	mask = 0x3FFF;

	rdcmd = RDPROG;
	prog_mode();
	for ( i=0; i<Epr.BufferSize; ++i ) {
		command(rdcmd);
		picbuf[i] = pin_word() & mask;
		command(INCADD);
        frmEprom->gProcess->Progress = i;

        Application->ProcessMessages();
        if(cancel)
          break;
	}

    if(!cancel)
    {
      mask = 0xFF;

      frmEprom->SetProcessGauge("Reading Data program area", DSIZE,clBlack);

      rdcmd = RDDATA;
      prog_mode();

      for ( i=0; i<DSIZE; ++i ) {
          command(rdcmd);
          picbuf[i + 0x2100]= pin_word() & mask;
          command(INCADD);
          frmEprom->gProcess->Progress = i;

          Application->ProcessMessages();
          if(cancel)
            break;
      }
    }

    if(!cancel)
    {
      command(LDCONF);
      pout_word(fuses);
      for ( i=0; i<7; ++i )
           command(INCADD);
      command(RDPROG);
      picbuf[0x2007] = pin_word()&0x1F;
      fuses = picbuf[0x2007];
    }

	idle_mode();OE_High;

    if(!cancel)
      frmEprom->Message("PIC dumped OK",0);
    else
      frmEprom->Message("Process Cancelled",1);
    winstat();
}

void CRead(void)
{
  unsigned int i, y;

  ClearBuf();

  if(st!=0)getoff();

  frmEprom->SetProcessGauge("Reading Chip", Epr.BufferSize,clBlack);

  setup();

  if(Epr.epr64) {WE_High;}
  else we_lo;

  r_adres_28C(0);
  for(i=0;i<Epr.BufferSize;i++)
  {
  	y=i+st;
  	r_adres_28C(i+st);
  	Buffer[y]=in_word();
    frmEprom->gProcess->Progress = i;

    Application->ProcessMessages();
    if(cancel)
      break;
  }

  power_down();/* */wil_assert;          //ARW: was powoff , wil_assert;
  if(!cancel)
  {
    frmEprom->Message("Chip read OK",0);
    frmEprom->Caption = "Willem Eprom Programmer - " + frmEprom->edDevice->Caption + " Image";
    grot = Epr.BufferSize;
  }
  else
    frmEprom->Message("Process Cancelled",1);

  winstat();
}

void read_65(void)		//DW: added void
{
	unsigned char adrby1, adrby2;
	unsigned char g;
	unsigned int i;

	OE_Low;

    frmEprom->SetProcessGauge("Reading Chip", Epr.BufferSize,clBlack);

	outhi,adrhi;asadres;

    if(Epr.pin7)WE_High
    else WE_Low;

	powon;wil_assert;
	ms_delay(15000);
	for(i=0;i<=Epr.BufferSize;i++)
    {
		adrby1=i>>8;
		adrby2=i;

		if(strct())
        {
			if(W_BYTE(adrby1))
            {
				if(W_BYTE(adrby2))
                {
					start();
					stop();
					if(strctR())
                    {
						g=ein_word();eclock_out(1);stop();
					}
					else
                    {        // DW: removed some unused code
						error("Unknown Error");
						break;
					}
				}
				else
                {
					error("Unknown Error");
					break;
				}
			}
			else
            {
				error("Unknown Error");
				break;
			}
		}
		else
        {
			error("Unknown Error");
			break;
		}
		Buffer[i]=g;
        frmEprom->gProcess->Progress = i;

        Application->ProcessMessages();
        if(cancel)
          break;
	}

	power_down();              //ARW:
	//powoff,
	read,wil_assert;               //ARW:    duplication

    if(!cancel)
    {
      frmEprom->Message("Chip read OK",0);
      frmEprom->Caption = "Willem Eprom Programmer - " + frmEprom->edDevice->Caption + " Image";
    }
    else
      frmEprom->Message("Process Cancelled",1);

    winstat();
}

void read_16(void)
{
  unsigned char adrby1, adrby2;
  unsigned char g;
  unsigned int i;

  frmEprom->SetProcessGauge("Reading Chip", Epr.BufferSize,clBlack);

  OE_Low;
  outhi,adrhi;asadres;

  if(Epr.pin7)WE_High
  else WE_Low;

  powon;wil_assert;
  ms_delay(15000);
  for(i=0;i<Epr.BufferSize;i++) {
	adrby1=i>>7;
	adrby1&=126;
	adrby1=adrby1+CTW;
	adrby2=i;
	start();
	if(W_BYTE(adrby1)) {
      if(W_BYTE(adrby2)) {
	  	start();
	  	stop();
	  	adrby1=adrby1+1;
	  	start();
	  	if(W_BYTE(adrby1)) { g=ein_word();clock_out(1);stop(); }
		else { error("Unknown Error"); break; }
       }else { error("Unknown Error"); 	break; }
	} else { error("Unknown Error");  break;}
	Buffer[i]=g;

    frmEprom->gProcess->Progress = i;
    Application->ProcessMessages();
    if(cancel)
      break;
  }

  power_down();
  read,wil_assert;      //ARW: duplication but left for clarity
  if(!cancel)
  {
    frmEprom->Message("Chip read OK",0);
    frmEprom->Caption = "Willem Eprom Programmer - " + frmEprom->edDevice->Caption + " Image";
    grot = Epr.BufferSize;
  }
  else  frmEprom->Message("Process Cancelled",1);

  winstat();
}

//---------------------------------------------------------------------------
// Load File Routines
//---------------------------------------------------------------------------

void Load_S(bool Autoswitch);

// Process line of Intel Hex file

int segment;

unsigned Process_Line (unsigned char *inp_line)
{
	int file_chksum, calc_chksum;
	int i, status;
	int count, type;
	int address, addhi, addlo;
	char  *inp_ptr;

	inp_line=strstr((char *)inp_line, ":") ; //ARW: reset inp_line to point to colon
	if ( inp_line != NULL)                   //ARW: avoids errors due to LFCR / CRLF
	{									     //ARW: warning now accepts bytes before the colon
		sscanf(inp_line, ":%2X%2lX%2lX%2X", &count, &addhi, &addlo, &type);
		calc_chksum = count + addhi + addlo + type;
		inp_ptr = inp_line + 9;
		for (i=0; i<count; i++) {
			sscanf(inp_ptr, "%2X", &bytes[i]);
			calc_chksum += bytes[i];
			inp_ptr +=2;
		}
		calc_chksum =-(calc_chksum & 0xFF) & 0xFF;

		sscanf(inp_ptr, "%2X", &file_chksum);

		if (calc_chksum == file_chksum)
        {
          switch(type)
          {
              case 0  : // Data
                        address = segment + (addhi << 8) + addlo;
                       	for (i=0; i<count; i++)
            				Buffer[address+i] = bytes[i];
            			image_highwater=MAX(image_highwater,segment+address+i);
                        break;

              case 1  : // Terminator
                        break;

              case 2  : // 20 bit Segment Address
                        segment = ((bytes[0] << 12) + bytes[1]) << 4;
                        break;

              case 4  : // 32 bit Segment Address
                        segment = ((bytes[0] << 24) + bytes[1] << 16);
                        break;
          }
          status = 1;
		}
		else
        {
          error("Checksum Error reading Hexfile");
          status = 0;
		}
	}
	else
    {
	  error("Error reading Hexfile");
      status = 0;
	}
	return status;
}

unsigned Process_Line_S (unsigned char *inp_line)
{
	unsigned file_chksum, calc_chksum;
	int i, status;
	int count=0;
	int address, addhi;
	char  *inp_ptr;

    // Only process S1,S2 and S3 lines

	if(strstr((char *)inp_line, "S1") != NULL)
    {
      sscanf(inp_line+2, "%2X%4X", &count, &addhi);
	  calc_chksum = count + (addhi & 0xFF) + ((addhi >> 8) & 0xFF);
	  inp_ptr = inp_line + 8;
	  count=count-3;
    }
    else
    {
   	  if(strstr((char *)inp_line, "S2") != NULL)
      {
        sscanf(inp_line+2, "%2X%6X", &count, &addhi);
        calc_chksum = count + (addhi & 0xFF) + ((addhi >> 8) & 0xFF) + ((addhi >> 16) & 0xFF);
        inp_ptr = inp_line + 10;
        count=count-4;
      }
      else
      {
   	    if(strstr((char *)inp_line, "S3") != NULL)
        {
          sscanf(inp_line+2, "%2X%8X", &count, &addhi);
          calc_chksum = count + (addhi & 0xFF) + ((addhi >> 8) & 0xFF) + ((addhi >> 16) & 0xFF)+ ((addhi >> 24) & 0xFF);
          inp_ptr = inp_line + 12;
          count=count-5;
        }
      }
    }

    if(count > 0)
    {
	  for(i=0; i<count; i++)
      {
	    sscanf(inp_ptr, "%2X", &bytes[i]);
	    calc_chksum += bytes[i];
	    inp_ptr +=2;
	  }
	  calc_chksum =~calc_chksum;
	  calc_chksum=calc_chksum & 0xFF;
	  sscanf(inp_ptr, "%2X", &file_chksum);

	  if (calc_chksum == file_chksum)
      {
	  	address = addhi;

      	for (i=0; i<count; i++)
		  Buffer[address+i]= bytes[i];

 		image_highwater=MAX(image_highwater,address+i);
		status = 1;
      }
      else
      {
        error("Checksum Error reading Hexfile");
        status = 0;
      }
	}
	return status;
}

void Load_Hex(bool Autoswitch)
{
	int status=1;

    segment=0;
	fp=fopen((frmEprom->FileOpen->FileName.c_str()),"r");

	if(fp==NULL)
    {
      error("Error opening file");
      return;
    }

	ClearBuf();

	while ((fgets(inp_line,80,fp) != NULL) && (status))
      status = Process_Line (inp_line);

	fclose(fp);

	if(status)
    {
        grot = image_highwater;
        image_highwater = 0;
        frmEprom->Message("Hex File loaded OK",0);
        frmEprom->Caption = "Willem Eprom Programmer - " + ExtractFileName(frmEprom->FileOpen->FileName);
	}
    // If failed, check that file is not really S record
    else if((Autoswitch) && ((strstr((char *)inp_line, "S") != NULL)))
        Load_S(false);
}

void Load_Bin(void)
{
    fp=fopen((frmEprom->FileOpen->FileName.c_str()),"rb");

	if(fp==NULL)
    {
      error("Error opening file");
      return;
    }

	ClearBuf();

    grot = fread(Buffer,1,MAXROMSIZE,fp);

	fclose(fp);

    if(grot)
    {
      frmEprom->Message("Binary File loaded OK",0);
      frmEprom->Caption = "Willem Eprom Programmer - " + ExtractFileName(frmEprom->FileOpen->FileName);
    }
}

void Load_S(bool Autoswitch)
{
    int status=1;

    fp=fopen((frmEprom->FileOpen->FileName.c_str()),"rb");

	if(fp==NULL)
    {
      error("Error opening file");
      return;
    }

	ClearBuf();

	while ((fgets(inp_line,80,fp) != NULL) && (status))
      status=Process_Line_S(inp_line);

	fclose(fp);

	if(status)
    {
        grot = image_highwater;
        image_highwater = 0;
        frmEprom->Message("S Record File loaded OK",0);
        frmEprom->Caption = "Willem Eprom Programmer - " + ExtractFileName(frmEprom->FileOpen->FileName);
	}
    // If failed, check that file is not really Intel Hex
    else if((Autoswitch) && (strstr((char *)inp_line, ":") != NULL))
        Load_Hex(false);
}

//---------------------------------------------------------------------------
// Save File Routines
//---------------------------------------------------------------------------

void Save_Bin(void)
{
    if(Epr.PIC)
    {
      ShowMessage("This format will not preserve any Fuse and Data information");
    }

    if(UpperCase(ExtractFileExt(frmEprom->FileSave->FileName)) != ".BIN")
      frmEprom->FileSave->FileName = frmEprom->FileSave->FileName + ".BIN";

    fp=fopen((frmEprom->FileSave->FileName.c_str()),"wb");

    if(fp==NULL)
    {
      error("Error opening file");
      return;
    }

    if(Epr.PIC)
    {
        fwrite(Buffer,1,Epr.BufferSize * 2,fp);
    }
    else
    {
        fwrite(Buffer,1,Epr.BufferSize,fp);
    }
    fclose(fp);

    frmEprom->Caption = "Willem Eprom Programmer - " + ExtractFileName(frmEprom->FileSave->FileName);
}

void Save_Rom(void)
{
    if(Epr.PIC)
    {
      ShowMessage("This format will not preserve any Fuse and Data information");
    }

    if(UpperCase(ExtractFileExt(frmEprom->FileSave->FileName)) != ".ROM")
      frmEprom->FileSave->FileName = frmEprom->FileSave->FileName + ".ROM";

    fp=fopen((frmEprom->FileSave->FileName.c_str()),"wb");

    if(fp==NULL)
    {
      error("Error opening file");
      return;
    }

    if(Epr.PIC)
    {
        fwrite(Buffer,1,Epr.BufferSize * 2,fp);
    }
    else
    {
        fwrite(Buffer,1,Epr.BufferSize,fp);
    }
    fclose(fp);

    frmEprom->Caption = "Willem Eprom Programmer - " + ExtractFileName(frmEprom->FileSave->FileName);
}

static void d_hexbyte(FILE *fp, int b)
{
		fprintf(fp,"%02X",b);
		check += b;
}

static void d_hexword(FILE *fp, int w)
{
		d_hexbyte(fp,w>>8);
		d_hexbyte(fp,w&0xFF);
}

static void hexrec(FILE *fp, long address, long nw)
{
	int i, n;

    segment=0;
	while ( nw > 0 )
    {
        if (segment != (address >> 16))
        {
            // write new segment offset
    		check = 0;
            segment = (address >> 16);
	    	fprintf(fp,":");
            d_hexbyte(fp,2);
    		d_hexword(fp,0);
            d_hexbyte(fp,4);
    		d_hexbyte(fp,segment >> 8);         // write hi-lo format
    		d_hexbyte(fp,segment & 0xFF);
    		d_hexbyte(fp,(-check)&0xFF);
    		fprintf(fp,"\n");
        }

		check = 0;
		fprintf(fp,":");
		n = (nw > 16)? 16: nw;

		d_hexbyte(fp,n);
		d_hexword(fp,(address & 0xffff));

		d_hexbyte(fp,0);
		for ( i=0; i < n; ++i ) {
			d_hexbyte(fp,Buffer[address+i]);
		}
		d_hexbyte(fp,(-check)&0xFF);
		fprintf(fp,"\n");
		nw -= 16;
		address += 16;
	}
}

void Save_Hex(void)
{
    if(UpperCase(ExtractFileExt(frmEprom->FileSave->FileName)) != ".HEX")
      frmEprom->FileSave->FileName = frmEprom->FileSave->FileName + ".HEX";

    fp=fopen((frmEprom->FileSave->FileName.c_str()),"w");

	if(fp==NULL)
    {
      error("Error opening file");
      return;
    }

    if(Epr.PIC){
        hexrec(fp,0,Epr.BufferSize * 2);
        hexrec(fp,0x400E,1);
        hexrec(fp,0x4200,128);
    }
    else
    {
        hexrec(fp,0,Epr.BufferSize);
    }
    fprintf(fp,":00000001FF\n");
    fclose(fp);

    frmEprom->Caption = "Willem Eprom Programmer - " + ExtractFileName(frmEprom->FileSave->FileName);
}

static void S_rec(FILE *fp, long address, long nw, int S3)
{
  long i, n;

  while ( nw > 0 )
  {
  	n = (nw > 16)? 16: nw;

  	if(S3)
    {
      // 4 bytes address, 1 byte checksum
      fprintf(fp,"S3%02X%08X",n+5,address);
      check = n + 1 + 5 + (address & 0xFF) + ((address >> 8) & 0xFF) + ((address >> 16) & 0xFF)+ ((address >> 24) & 0xFF);
    }
    else
    {
      // 2 bytes address, 1 byte checksum
      fprintf(fp,"S1%02X%04X",n+3,address);
      check = n + 1 + 3 + (address & 0xFF) + ((address >> 8) & 0xFF);
    }

	for ( i=0; i < n; ++i )
	  d_hexbyte(fp,Buffer[address+i]);

	d_hexbyte(fp,(-check) & 0xFF);
	fprintf(fp,"\n");
	nw -= 16;
	address += 16;
  }
}

void Save_S(void)
{
  // We could use S1,S2 and S3 dependant on address size
  // but for simplicity we force the entire file to be
  // S1 or S3 type records.

  int S3=0;

  if(Epr.BufferSize > 0xFFFF)
  {
    S3 = -1;
  }

  if(UpperCase(ExtractFileExt(frmEprom->FileSave->FileName)) != ".S")
    frmEprom->FileSave->FileName = frmEprom->FileSave->FileName + ".S";

  fp=fopen((frmEprom->FileSave->FileName.c_str()),"w");

  if(fp==NULL)
  {
    error("Error opening file");
    return;
  }

  if(Epr.PIC){
    S_rec(fp,0,Epr.BufferSize * 2,S3);
    S_rec(fp,0x400E,1,S3);
    S_rec(fp,0x4200,128,S3);
  }
  else
  {
    S_rec(fp,0,Epr.BufferSize,S3);
  }
  fprintf(fp,"S9030000FC\n");
  fclose(fp);

  frmEprom->Caption = "Willem Eprom Programmer - " + ExtractFileName(frmEprom->FileSave->FileName);
}
//---------------------------------------------------------------------------
// GUI interface routines
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::Message(String Notice, int Level)
{
    switch (Level)
    {
      case 1 : StatusBar->Font->Color = clRed;
               MessageBeep(MB_ICONEXCLAMATION);
               break;

      case 2 : StatusBar->Font->Color = clBlack;
               Beep();
               break;

      default: StatusBar->Font->Color = clBlue;
    }

    StatusBar->Caption = Notice;
    StatusBar->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::SetProcessGauge(String Notice, int Max, TColor Colour)
{
  pIDcode->Visible = false;
  pProcess->Visible = true;
  pProcessCap->Caption = Notice;
  Message(Notice,0);

  cancel = false;
  btnCancel->Visible  = true;

  if(Colour == clWhite)
    gProcess->BackColor = clNavy;
  else
    gProcess->BackColor = clWhite;

  gProcess->Progress  = 0;
  gProcess->MaxValue  = Max;
  gProcess->ForeColor = Colour;
  gProcess->Visible   = true;
  gProcess->Refresh();
  btnCancel->Refresh();
  btnCancel->SetFocus();
  Application->ProcessMessages();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::SetBufferSize(int Size)
{
    TGridRect myRect;
    Epr.BufferSize = Size;

    // Change size of HexGrid (I always want a scrollbar!)
    if((Epr.BufferSize / 16) < 28)
        HexGrid->RowCount = 29;
    else
        HexGrid->RowCount = (Epr.BufferSize / 16) + 1;

    // Sort out size of ProgGrid
    if (Epr.PIC)
        ProgGrid->RowCount = (Epr.BufferSize / 8) + 1;
    else
        ProgGrid->RowCount = 2;

    // Sort out what we need to see
    HexGrid->Visible  = FALSE;
    ProgGrid->Visible = FALSE;
    if(Epr.PIC) {
        if(!C50) {
            // Need Both Grids
            ProgGrid->Height     = frmEprom->Height - 200;
            HexGrid->Cells[0][0] = " Data";
            HexGrid->Height      = 112;

            HexGrid->Visible     = TRUE;
            ProgGrid->Visible    = TRUE;
        }
        else
        {
            // Just Program Grid
            ProgGrid->Height  = frmEprom->Height - 26;
            ProgGrid->Visible = TRUE;
        }
    }
    else
    {
        // Just Data Grid
        HexGrid->Cells[0][0] = "Offset";
        HexGrid->Top         = 0;
        HexGrid->Height      = frmEprom->Height - 26;
        HexGrid->Visible     = TRUE;
    }

    // Lose Selected Boxes
	myRect.Left   = -1;
	myRect.Top    = -1;
	myRect.Right  = -1;
	myRect.Bottom = -1;
	HexGrid->Selection  = myRect;
    ProgGrid->Selection = myRect;

    HexGrid->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::SetFuses()
{
    // Set the comboboxes correctly for the fuse data

    FuseUpdate = TRUE;

   	cp=(fuses&CP)? CP: 0;
	wd=(fuses&WDTE)? WDTE: 0;
	if(F84)
        pt=(fuses&PWRTE)? 0: PWRTE;
    else
    	pt=(fuses&PWRTE)? PWRTE: 0;

	osc=(fuses&3);

	if(!cp) cbCodeProtect->ItemIndex = 1;
	else cbCodeProtect->ItemIndex = 0;
	if(pt)cbPic2->ItemIndex = 1;
	else cbPic2->ItemIndex = 0;
	if(wd)cbWatchDog->ItemIndex = 1;
	else cbWatchDog->ItemIndex = 0;
    cbOscillator->ItemIndex = osc;

    FuseUpdate = FALSE;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::mnuTestClick(TObject *Sender)
{
    test_hw();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::About1Click(TObject *Sender)
{
    ShowMessage(" A Windows version of Willem's Eprom Programmer Software\n"
                " Constructed by Mike Coates (mike.coates@btinternet.com)\n"
                " Modify by avr_mpu@hotmail.com\n"
                "\n"
                " Hints, Bug Reports, Improvements, please let either one\n"
                " of us know. For full details of the eprom programmer and\n"
                "             other projects visit www.willem.org\n"
                " or          http://www.geocities.com/mpu51\n"
                " or          http://members.nbci.com/mpu51\n"
                "\n"
                "DriverLINX Port I/O Driver for Win95 and WinNT\n"
                "(C) Copyright 1996, Scientific Software Tools, Inc.\n"
                "All Rights Reserved.\n"
                "\n"
                "TDLPortIO: DriverLINX Port IO Driver wrapper component\n"
                "Copyright (c) 1999 John Pappas (DiskDude). All rights reserved.\n"
                "\n"
                "                              Version 0.90d\n");
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::FormCreate(TObject *Sender)
{
    // Get performance details for timing
    QueryPerformanceFrequency(&PerformanceFrequency);

    // Port Access DLL's for Windows 95 and NT
    DLPortIO->DriverPath=ExtractFileDir(ParamStr(0));
    DLPortIO->OpenDriver();

    if (!DLPortIO->ActiveHW)
       MessageDlg("Could not open the DriverLINX driver.",mtError, TMsgDlgButtons() << mbOK, 0);

    // Default controls to correct values (add registry settings later!)
    cbPort->ItemIndex = 0;
    cbMethod->ItemIndex = 0;
    cbFastErase->ItemIndex = 1;
    cbCheckType->ItemIndex = 0;
    cbSkipFF->ItemIndex = 1;

    edOffset->Text = "0000";

    // Assign dip switch elements to array for ease of use
    DipSwitch[2]  = dip1;
    DipSwitch[3]  = dip2;
    DipSwitch[4]  = dip3;
    DipSwitch[5]  = dip4;
    DipSwitch[6]  = dip5;
    DipSwitch[7]  = dip6;
    DipSwitch[8]  = dip7;
    DipSwitch[9]  = dip8;
    DipSwitch[10]  = dip9;
    DipSwitch[11] = dip10;
    DipSwitch[12] = dip11;
    DipSwitch[13] = dip12;
    DipSwitch[1] = JP_A18Vpp;

    // Setup headings for Hex Grid - Widths are 52,16 * 16, 114
    HexGrid->Cells[1][0] = "0";
    HexGrid->Cells[2][0] = "1";
    HexGrid->Cells[3][0] = "2";
    HexGrid->Cells[4][0] = "3";
    HexGrid->Cells[5][0] = "4";
    HexGrid->Cells[6][0] = "5";
    HexGrid->Cells[7][0] = "6";
    HexGrid->Cells[8][0] = "7";
    HexGrid->Cells[9][0] = "8";
    HexGrid->Cells[10][0] = "9";
    HexGrid->Cells[11][0] = "A";
    HexGrid->Cells[12][0] = "B";
    HexGrid->Cells[13][0] = "C";
    HexGrid->Cells[14][0] = "D";
    HexGrid->Cells[15][0] = "E";
    HexGrid->Cells[16][0] = "F";
    HexGrid->Cells[17][0] = "    Ascii";

    ProgGrid->Cells[0][0] = "Program";
    ProgGrid->Cells[1][0] = "0";
    ProgGrid->Cells[2][0] = "1";
    ProgGrid->Cells[3][0] = "2";
    ProgGrid->Cells[4][0] = "3";
    ProgGrid->Cells[5][0] = "4";
    ProgGrid->Cells[6][0] = "5";
    ProgGrid->Cells[7][0] = "6";
    ProgGrid->Cells[8][0] = "7";
    ProgGrid->Cells[9][0] = "    Ascii";


//----  NC added  for MXIC F29040 sectors prot.
sgMXICF29SecVfyF29->Cells[0][0]="Sec        Address        Stat";
//Disable sector protect/chip unprotect buttons untill display sector status
sbMXICF29SecProt->Enabled=false;
sbMXICF29ChipUnprt->Enabled=false;

//------------- setting position ------
  frmEprom->Height =  444;  frmEprom->Width = 490;
  pProcess->Left = 48;      pProcess->Top = 240;
  pIDcode->Left = 112;      pIDcode->Top = 225;
  pdLockBit->Left =120;     pdLockBit->Top =135;
  pRdLB->Left =135;         pRdLB->Top = 222;
//------------------------------------
    power_down();
    SetBufferSize(0);
    ClearBuf();
    AT89Cxx.initl_code();
    LoadSettings();
    winstat();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::mnuExitClick(TObject *Sender)
{
    // Leave programmer in safe state for chip removal etc.
    power_down();
    SaveSettings();
    Application->Terminate();
}

//---------------------------------------------------------------------------

void __fastcall TfrmEprom::FormClose(TObject *Sender, TCloseAction &Action)
{
    // Leave programmer in safe state for chip removal etc.
    power_down();
    SaveSettings();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::cbPortChange(TObject *Sender)
{
  switch(cbPort->ItemIndex) {
    case 0:  poort  = 0x378; break;
    case 1:  poort  = 0x278; break;
    case 2:  poort  = 0x3bc; break;
  }
  ipoort = poort+1;
  spoort = poort+2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::cbMethodChange(TObject *Sender)
{
  if(cbMethod->ItemIndex == 0)
    Epr.ep = 1;
  else
    Epr.ep = 0;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TfrmEprom::cbFastEraseChange(TObject *Sender)
{
    qe = cbFastErase->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::MaskEdit1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TPoint P, Q;

    // Popup Device Menu

    if(Button==mbLeft)
    {
        P.x = X + edDevice->Left + Panel4->Left;
	    P.y = Y + edDevice->Top + Panel4->Top+30;
	    Q = ClientToScreen(P);
        mnuDevice->Popup(Q.x,Q.y);
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::M24C_Click(TObject *Sender)
{
    // Setup for selected 24Cxx device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    ZIFImage = 240;

    saveEpr();
    switch(DeviceID)
    {
        case 1 : SetBufferSize(0x100);
                 Epr.eep=M24C16;
                 break;
        case 2 : SetBufferSize(0x200);
                 Epr.eep=M24C16;
                 break;
        case 3 : SetBufferSize(0x400);
                 Epr.eep=M24C16;
                 break;
        case 4 : SetBufferSize(0x800);
                 Epr.eep=M24C16;
                 break;
        case 5 : SetBufferSize(0x1000);
                 Epr.eep=M24C32;
                 break;
        case 6 : SetBufferSize(0x2000);
                 Epr.eep=M24C32;
                 break;
        case 7 : SetBufferSize(0x4000);
                 Epr.eep=M24C32;
                 break;
        case 8 : SetBufferSize(0x8000);
                 Epr.eep=M24C32;
                 break;

        case 9 : SetBufferSize(0x10000);
                 Epr.eep=M24C32;
                 break;
    }
    if (DeviceID >0) {
         Epr.serial = ON;
         Dip(0);
    }else restoreEpr();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::M27C_Click(TObject *Sender)
{
    // Setup for selected 27Cxx device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;
    unsigned int DIP;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    pe=0;

    saveEpr();
    switch(DeviceID)
    {
        case 15 : SetBufferSize(0x800);  ZIFImage=24; DIP = 27;  break;
        case 1 : SetBufferSize(0x1000);  ZIFImage=24; DIP = 9;  break;
        case 2 : SetBufferSize(0x2000);  ZIFImage=28; DIP = 1;  Epr.Vpp   = ON; Epr.epr64 = ON; break;
        case 3 : SetBufferSize(0x4000);  ZIFImage=28; DIP = 2;  Epr.Vpp   = ON; Epr.epr64 = ON; break;
        case 4 : SetBufferSize(0x8000);  ZIFImage=28; DIP = 3;  Epr.Vpp   = ON; break;
        case 5 : SetBufferSize(0x10000); ZIFImage=28; DIP = 4;  break;
        case 6 : SetBufferSize(0x20000); ZIFImage=32; DIP = 7;  Epr.Vpp   = ON; Epr.epr64 = ON; break;
        case 7 : SetBufferSize(0x40000); ZIFImage=32; DIP = 7;  Epr.Vpp   = ON; Epr.epr64 = ON; break;
        case 8 : SetBufferSize(0x80000); ZIFImage=32; DIP = 10; Epr.Vpp   = ON; break;

        case 9 : SetBufferSize(0x20000); ZIFImage=32; DIP = 7;  Epr.epr64 = ON; break;
        case 10 : SetBufferSize(0x40000); ZIFImage=32; DIP = 7; Epr.epr64 = ON; break;
        case 11 : SetBufferSize(0x80000); ZIFImage=32; DIP = 10;  break;
    }
   	if(DeviceID>0){
	     Epr.eep = M27CXXX;
	     Epr.tWP = TICKS(110);    // tWP = 120uS
         Epr.tWC = 15;
	     Epr.add_step = 100;
	     calcu_adr();
         Dip(DIP);
	}else restoreEpr();
}


//---------------------------------------------------------------------------

void __fastcall TfrmEprom::M28C_Click(TObject *Sender)
{
    // Setup for selected 28Cxx device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;
    unsigned int DIP;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    pe=1;

    saveEpr();
    switch(DeviceID)
    {
        case 1 : SetBufferSize(0x2000);   ZIFImage=28;DIP=5;  break;
        case 2 : SetBufferSize(0x4000);   ZIFImage=28;DIP=5;  break;
        case 3 : SetBufferSize(0x8000);   ZIFImage=28;DIP=18; Epr.swap = ON;  break;
        case 4 : SetBufferSize(0x10000);  ZIFImage=28;DIP=7;  break;
        case 5 : SetBufferSize(0x20000);  ZIFImage=32;DIP=7;  break;
        case 6 : SetBufferSize(0x40000);  ZIFImage=32;DIP=7;  break;
        case 7 : SetBufferSize(0x80000);  ZIFImage=32;DIP=19; break;
    }
   	if(DeviceID>0) {
	   calcu_adr();
	   Epr.eep=M28CXXX;
       Epr.tWP = 110;
       Epr.tWC = 1000;
	   Epr.epr64 = ON;
       Dip(DIP);
	}else restoreEpr();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::M28F_Click(TObject *Sender)
{
    // Setup for selected 28Fxx device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;
    unsigned int DIP;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    pe=0;

    saveEpr();
    switch(DeviceID)
    {
        case 1 : SetBufferSize(0x2000); ZIFImage=28;DIP=(5);break;
        case 2 : SetBufferSize(0x4000); ZIFImage=28;DIP=(5);break;
        case 3 : SetBufferSize(0x8000); ZIFImage=28;DIP=(3);break;
        case 4 : SetBufferSize(0x10000);ZIFImage=28;DIP=(7);break;
        case 5 : SetBufferSize(0x20000);ZIFImage=32;DIP=(7);break;
        case 6 : SetBufferSize(0x40000);ZIFImage=32;DIP=(7);break;
        case 7 : SetBufferSize(0x80000);ZIFImage=32;DIP=(12);break;
        case 8 : SetBufferSize(0x20000);ZIFImage=32;Epr.pin30 = ON; DIP=(7);break;
    }
	if(DeviceID>0) {
	   calcu_adr();
	   Epr.eep=M28FXXX;
	   Epr.epr64 = ON;
       Epr.tWP = 5;
       Epr.tWC = 25;
       Dip(DIP);
	}else restoreEpr();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::M29F_Click(TObject *Sender)
{
    // Setup for selected 29Fxx device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;
    unsigned int DIP;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    pe=0;

    saveEpr();
    switch(DeviceID)
    {
        case 1 : SetBufferSize(0x2000); ZIFImage=28;DIP=5;break;
        case 2 : SetBufferSize(0x4000); ZIFImage=28;DIP=5;break;
        case 3 : SetBufferSize(0x8000); ZIFImage=28;DIP=18;break;
        case 4 : SetBufferSize(0x10000);ZIFImage=32;DIP=7;break;
        case 5 : SetBufferSize(0x20000);ZIFImage=32;DIP=7;break;
        case 6 : SetBufferSize(0x40000);ZIFImage=32;DIP=7;break;
        case 7 : SetBufferSize(0x80000);ZIFImage=32;DIP=19;break;
        // AT49F512/010/020/040
        case 8 : SetBufferSize(0x10000);ZIFImage=28;DIP=7;break;
        case 9 : SetBufferSize(0x20000);ZIFImage=32;DIP=7;break;
        case 10 : SetBufferSize(0x40000);ZIFImage=32;DIP=7;break;
        case 11 : SetBufferSize(0x80000);ZIFImage=32;DIP=19;break;
    }
    if(DeviceID>0)
        {
        calcu_adr();

        if(DeviceID >7){Epr.eep=AT49FXXX;Epr.tWP = 5;Epr.tWC = 20;}
        else{Epr.eep=M29FXXX;Epr.tWP = 5;Epr.tWC = 25;}

        Dip(DIP);
        Epr.epr64 = ON;
	}
    else restoreEpr();

}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::M12C_Click(TObject *Sender)
{
    // Setup for selected 12C5xx device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;

    edDevice->Caption = Selected->Caption;
    // Setup for selected 12C5xx device

    saveEpr();
    switch(DeviceID)
    {
        case 1 :
            	 C622=0;C50=1;C89=0;F84=0;
           		 Epr.eep=M12C50X; Epr.PIC = 1;
    	    	 fuses=picbuf[0x2007];
    		     ZIFImage = 12;
                 SetBufferSize(512); //12C508
                 break;

        case 2 :
            	 C622=0;C50=1;C89=0;F84=0;
               	 Epr.eep=M12C50X; Epr.PIC = 1;
        		 fuses=picbuf[0x2007];
    	    	 ZIFImage = 12;
                 SetBufferSize(1024); //12C509
                 break;
//------------------------16C62x--
       case 3 :
           	     C622=1;C50=0;C89=0;F84=0;
               	 Epr.eep=M16C62X; Epr.PIC = 1;
        		 fuses=picbuf[0x2007];
    	    	 ZIFImage = 16;
                 SetBufferSize(512); //16C620
                 break;

        case 4 :
            	 C622=1;C50=0;F84=0;
           	     Epr.eep=M16C62X; Epr.PIC = 1;
        		 fuses=picbuf[0x2007];
        		 ZIFImage = 16;
                 SetBufferSize(1024); //16C621
                 break;

        case 5 :
    	    	 C622=1;C50=0;F84=0;
           	     Epr.eep=M16C62X; Epr.PIC = 1;
        		 fuses=picbuf[0x2007];
        		 ZIFImage = 16;
                 SetBufferSize(2048); //16C622
                 break;
        case 6 :
            	 C622=0;C50=0;F84=0;
               	 Epr.eep=M16C62X; Epr.PIC = 1;
        		 fuses=picbuf[0x2007];
    	    	 ZIFImage = 16;
                 SetBufferSize(1024); //16C84
                 break;
        case 7 :
            	 C622=0;C50=0;F84=1;
               	 Epr.eep=M16C62X; Epr.PIC = 1;
        		 fuses=picbuf[0x2007];
    	    	 ZIFImage = 16;
                 SetBufferSize(1024); //16F84
                 break;
	}
    if(DeviceID > 0) {
      Dip(0);
      Epr.serial = ON;
    }else restoreEpr();
}



//---------------------------------------------------------------------------

void __fastcall TfrmEprom::M93C_Click(TObject *Sender)
{
    // Setup for selected 93Cxx device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;

    edDevice->Caption = Selected->Caption;
    C622=0;C50=0;C89=0;F84=0;C93=1;

	m93Cxx.init_code();
	saveEpr();
	switch(DeviceID){
		case 1: SetBufferSize(0x20);    break;
		case 2: SetBufferSize(0x80);    break;
		case 3: SetBufferSize(0x80);     break;
		case 4: SetBufferSize(0x100); m93Cxx.shift_L(2);break;
		case 5: SetBufferSize(0x100); m93Cxx.shift_L(1);break;
		case 6: SetBufferSize(0x200); m93Cxx.shift_L(2);break;
		case 7: SetBufferSize(0x400); m93Cxx.shift_L(4);break;
		case 8: SetBufferSize(0x800); m93Cxx.shift_L(4);break;
		case 9: SetBufferSize(0x20);  m93Cxx.shift_R(1); m93Cxx.bit16 = 2;break;
		case 10:SetBufferSize(0x80);  m93Cxx.shift_R(1); m93Cxx.bit16 = 2;break;
		case 11:SetBufferSize(0x100); m93Cxx.shift_L(1); m93Cxx.bit16 = 2;break;
		case 12:SetBufferSize(0x200); m93Cxx.shift_L(1); m93Cxx.bit16 = 2;break;
		case 13:SetBufferSize(0x400); m93Cxx.shift_L(3); m93Cxx.bit16 = 2;break;
		case 14:SetBufferSize(0x800); m93Cxx.shift_L(3); m93Cxx.bit16 = 2;break;
	}
	if(DeviceID > 0) {
		Epr.eep = M93CXXX;
        ZIFImage = 93;
		Epr.serial = ON;
		Dip(0);
	} else restoreEpr();
}


//---------------------------------------------------------------------------

void __fastcall TfrmEprom::mnuBlankClick(TObject *Sender)
{
    // Check eprom empty

    int ErrorLocation;

    if((Epr.BufferSize > 0) && (Epr.PIC == 0))  {
      switch(Epr.eep) {
            case M27CXXX:
            case M28FXXX:
            case M29FXXX:
            case M28CXXX:
            case AT28CXXX:
            case AT29CXXX:
            case AT49FXXX:
            case M27XXX:
            case AM29FX00:
            case I28FX00:
            case DS12XX:
            case W29EEXXX:
            case W27EXXX:
            case AT49F00X:  ErrorLocation= Check(); break;
            case AT89CX051:
            case AT89CXXX:  ErrorLocation = empty_AT89C(); break;
       }
       if(ErrorLocation== -1){
          Message("Device is Empty",2);
        }
        else{
          if(ErrorLocation== -2)
            Message("Device Check was Cancelled",1);
          else
            Message(Format("Device is NOT Empty ($%4.4x)",ARRAYOFCONST((ErrorLocation))),1);
        }
    }
    else
        Message("Cannot check this Device",1);
    frmEprom->pProcess->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::Clear1Click(TObject *Sender)
{
    frmEprom->Caption = "Willem Eprom Programmer";
    memset(Buffer,0xFF,0x100000);     // Pic Data or Parallel Eprom
    fuses = 0;
    winstat();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::HexGridDrawCell(TObject *Sender, int Col,
      int Row, TRect &Rect, TGridDrawState State)
{
  // Data Grid (Parallel Eprom & Data from PICS)
  int  DataSize;
  int  Offset = (Row - 1) * 16;
  int  DataStart = 0;
  int  Count;
  char ch[17];

  if(Epr.PIC)
  {
    DataSize  = 128;
    DataStart = 0x4200;
  }
  else
    DataSize = Epr.BufferSize;

  if(Row > 0) {
    if(Offset < DataSize) {
      switch(Col) {
          case 0  : HexGrid->Canvas->TextRect(Rect, Rect.Left + 1, Rect.Top + 1, Format("$%6.6x",ARRAYOFCONST((Offset))));
                    break;
          case 17 : ch[16]=0;
                    for(Count=0;Count<16;Count++)
                    {
                      if((Buffer[DataStart+Offset+Count] < 33) || (Buffer[DataStart+Offset+Count] > 127))
                        ch[Count] = '.';
                      else
                        ch[Count] = Buffer[DataStart+Offset+Count];
                    }

                    HexGrid->Canvas->TextRect(Rect, Rect.Left + 1, Rect.Top + 1, ch);
                    break;

          default : Offset += (Col - 1);
                    if(Offset <= DataSize)
                    {
                      HexGrid->Canvas->TextRect(Rect, Rect.Left + 1, Rect.Top + 1,Format("%2.2x",ARRAYOFCONST((Buffer[DataStart+Offset]))));
                    }
                    break;
      }
    }
  }
}

//---------------------------------------------------------------------------
void __fastcall TfrmEprom::ProgGridDrawCell(TObject *Sender, int Col,
      int Row, TRect &Rect, TGridDrawState State)
{
  // Program Grid (Program code from PICS)


  int  Offset = (Row - 1) * 16;
  int  Count;
  char ch[17];

  if(Row > 0)
  {
  //if(Offset < (Epr.BufferSize * 2))  // Original statement  (C++ Builder V.3.0)
    if((unsigned int)Offset < (Epr.BufferSize * 2))  //NC cast of Offset (C++ Builder V.4.0)
    {
      switch(Col)
      {
          case 0  : ProgGrid->Canvas->TextRect(Rect, Rect.Left + 1, Rect.Top + 1, Format("$%6.6x",ARRAYOFCONST((Offset/2))));
                    break;

          case 9  : ch[16]=0;
                    for(Count=0;Count<16;Count++)
                    {
                      if((Buffer[Offset+Count] < 33) || (Buffer[Offset+Count] > 127))
                        ch[Count] = '.';
                      else
                        ch[Count] = Buffer[Offset+Count];
                    }

                    ProgGrid->Canvas->TextRect(Rect, Rect.Left + 1, Rect.Top + 1, ch);
                    break;

          default : Offset += (Col - 1);
                    //if(Offset <= (Epr.BufferSize * 2)) //Original statement  (C++ Builder V.3.0)
                    if((unsigned int)Offset <= (Epr.BufferSize * 2))  //NC cast of Offset   (C++ Builder V.4.0)
                    {
                      ProgGrid->Canvas->TextRect(Rect, Rect.Left + 1, Rect.Top + 1,Format("%4.4x",ARRAYOFCONST((picbuf[(Row - 1) * 8 + (Col - 1)]))));
                    }
                    break;
      }
    }
  }
}

//---------------------------------------------------------------------------
void __fastcall TfrmEprom::mnuReadClick(TObject *Sender)
{
    if(Epr.BufferSize > 0)
    {
        if(Epr.serial == ON) Epr.flag = OFF;
		else Epr.flag = test_hw();
		if(!Epr.flag) {
            switch(Epr.eep) {
                case M93CXXX:   read_93();  break;
                case M12C50X:   P50read(); break;
                case M16C62X:
                case M16F84:    Pread();   break;

                case W27EXXX:
                case M27CXXX:   CRead();  break;
                case M28CXXX:
                case M28FXXX:
                case M29FXXX:
                case I28FX00:
                case AM29FX00:
                case AT29CXXX:
                case W29EEXXX:
                case AT49F00X:
                case AT49FXXX:  CRead_28C(); ; break;
                case AT89CX051: CRead_AT89C();   break;
                case AT89CXXX:  CRead_AT89C();   break;
                case AT28CXXX:
                case DS12XX:    CRead_28C();    break;

                case M24C32:  read_65();                  break;
                case M24C16:  read_16();                  break;
            }  // end switch(Epr.eep);
        }
     }
     else
        Message("Please select a Device Type",1);
     frmEprom->pProcess->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::mnuProgramClick(TObject *Sender)
{
  // Write Buffer to Device
  if(Epr.BufferSize > 0)
  {
    if(grot > 0)
    {
        if(Epr.serial == ON) Epr.flag = OFF;
		else Epr.flag = test_hw();
		if(!Epr.flag) {
            switch(Epr.eep) {
                case M93CXXX:   prog_93();  break;
                case M12C50X:   PIC508_pr(); break;
                case M16C62X:
                case M16F84:    PIC_pr(); break;
                case W27EXXX:
                case M27CXXX:   prog_M27XXX(); ; break;
                case M28CXXX:   prog_M28C();    break;
                case M28FXXX:   prog_f28();     break;
                case M29FXXX:
                case AT49FXXX:  prog_AT49F();   break;
                case I28FX00:   prog_28Fx00();  break;
                case AM29FX00:  prog_AM29Fx00();  break;

                case AT28CXXX:
                case W29EEXXX:
                case AT29CXXX:  ChangePriority(1);
                                prog_29C();
                                ChangePriority(0);
                                break;
                case AT89CX051:
                case AT89CXXX:  prog_AT89C();   break;
                case AT49F00X:  prog_AT49F00x(); break;
                case M24C32:    eprogram65();   break;
                case M24C16:    prog_16();      break;
                case RAM:       prog_SRAM();    break;
                case DS12XX:    prog_DS12();    break;
            }  // end switch(Epr.eep);
         }
     }
     else
         Message("Please load some information to program",1);
    }
    else
         Message("Please select a Device Type",1);
     frmEprom->pProcess->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::mnuVerifyClick(TObject *Sender)
{
  if(Epr.BufferSize > 0)
  {
    if(grot > 0)
    {
        if(Epr.serial == ON) Epr.flag = OFF;
		else Epr.flag = test_hw();
		if(!Epr.flag) {
             switch(Epr.eep) {
                case M93CXXX:   verify_93();    break;
                case M12C50X:   verify50();     break;
                case M16C62X:
                case M16F84:    verify();       break;
                case W27EXXX:
                case M27CXXX:   Compare();      break;
                case M28CXXX:
                case M28FXXX:
                case M29FXXX:
                case I28FX00:
                case AM29FX00:
                case AT29CXXX:
                case W29EEXXX:
                case AT49F00X:
                case AT49FXXX:  verify_28C(); ; break;
                case AT89CX051:
                case AT89CXXX:  verify_AT89C();  break;
                case AT28CXXX:
                case DS12XX:    verify_28C();    break;

                case M24C32:  verify_65();      break;
                case M24C16:  verify_16();      break;
            }  // end switch(Epr.eep);
        }
    }
    else
      Message("Please load some information to compare against",1);
  }
  else
    Message("Please select a Device Type",1);
  frmEprom->pProcess->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::cbPICChange(TObject *Sender)
{
    // Calculate new Fuses value

    if(!FuseUpdate)
    {
      cp=(cbCodeProtect->ItemIndex==1) ? 0: CP;

      wd=(cbPic2->ItemIndex==1)? WDTE: 0;

      if(F84)
          pt=(cbWatchDog->ItemIndex==1)? 0: PWRTE;
      else
          pt=(cbWatchDog->ItemIndex==1)? PWRTE: 0;

      osc=cbOscillator->ItemIndex;

      fuses = cp | wd | pt | osc;
      picbuf[0x2007] = fuses;
    }
}
//---------------------------------------------------------------------------


void __fastcall TfrmEprom::mnuLoadClick(TObject *Sender)
{
  // Get filename & load selected file
  AnsiString Extension;

  if (FileOpen->Execute())
  {
    Extension = UpperCase(ExtractFileExt(FileOpen->FileName));

    if(Extension==".S")
    {
      Load_S(true);
    }
    else
    {
      if(Extension==".HEX")
        Load_Hex(true);
      else
        Load_Bin();
    }

    if(Epr.PIC)
    {
      Epr.BufferSize=1024;
      fuses=picbuf[0x2007];
    }
    winstat();

    // Default start Directory for future loads & Saves
    FileOpen->InitialDir = ExtractFilePath(FileOpen->FileName);
    FileSave->InitialDir = FileOpen->InitialDir;
    FileSave->FileName   = FileOpen->FileName;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::mnuSaveClick(TObject *Sender)
{
  if(Epr.PIC) FileSave->FilterIndex = 2;
  else FileSave->FilterIndex = 1;

  if (FileSave->Execute())
  {
    switch (FileSave->FilterIndex)
    {
      case 1 : Save_Bin();  break;
      case 2 : Save_Hex();  break;
      case 3 : Save_S();    break;
      case 4 : Save_Rom();  break;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnCancelClick(TObject *Sender)
{
  // Cancel Button - Set Flag to Exit Processing
  cancel = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnCancelKeyPress(TObject *Sender, char &Key)
{
  // Cancel Button - Set Flag to Exit Processing
  if(Key == 27)
  {
    cancel = true;
    Key = 0;
  }
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::mnuEraseClick(TObject *Sender)
{
  if(Epr.BufferSize > 0)
  {
//     if(Epr.serial == ON) Epr.flag = OFF;
//  	 else Epr.flag = test_hw();
  Epr.flag = OFF;
	 if(!Epr.flag) {
        switch(Epr.eep) {
            case M93CXXX:   erase_93();    break;
            case M12C50X:
            case M16C62X:
            case M16F84:    P_erase();   break;
            case AT49F00X:
            case AT49FXXX:
            case M29FXXX:   erase_AT49F();  break;
            case M28FXXX:   erase_f28();    break;
            case I28FX00:   erase_28Fx00(); break;
            case AM29FX00:  erase_AM29Fx00(); break;
            case W29EEXXX:
            case AT29CXXX:  erase_AT29Cxxx(); break;
            case AT89CX051:
            case AT89CXXX:  erase_AT89C();   break;
            case W27EXXX:   erase_W27E();   break;
            default: Message("Cannot Erase this Device",1); break;
        }
      }
  }
  else
    Message("Please select a Device Type",1);
  frmEprom->pProcess->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::edOffsetExit(TObject *Sender)
{
  edOffset->Text = Format("%4.4x",ARRAYOFCONST(((int)st)));
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::edOffsetChange(TObject *Sender)
{
  sscanf(edOffset->Text.c_str(),"%X",&st);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::LoadSettings(void)
{
    int    ct1,ct2;
    String DeviceType;

    TRegistry *MyKey = new TRegistry;

    if(MyKey->OpenKey("\\Software\\Willem\\Eprom\\",true))
    {
      if(MyKey->ValueExists("PORT"))
      {
        cbPort->ItemIndex = MyKey->ReadInteger("PORT");
        cbPortChange(0);
      }

      if(MyKey->ValueExists("METHOD"))
      {
        cbMethod->ItemIndex = MyKey->ReadInteger("METHOD");
        cbMethodChange(0);
      }

      if(MyKey->ValueExists("ERASE"))
      {
        cbFastErase->ItemIndex = MyKey->ReadInteger("ERASE");
        cbFastEraseChange(0);
      }

      if(MyKey->ValueExists("CHECKSUM"))
      {
        cbCheckType->ItemIndex = MyKey->ReadInteger("CHECKSUM");
      }

      if(MyKey->ValueExists("SKIP-FF"))
      {
        cbSkipFF->ItemIndex = MyKey->ReadInteger("SKIP-FF");
      }

      if(MyKey->ValueExists("DEVICE"))
      {
        // Loop through device menu, look for this entry
        // Will work with 1 level of submenu only!

        DeviceType = MyKey->ReadString("DEVICE");

        for(ct1=0;ct1<mnuDevice->Items->Count;ct1++)
        {
            if(mnuDevice->Items->Items[ct1]->Count > 0)
            {
                for(ct2=0;ct2<mnuDevice->Items->Items[ct1]->Count;ct2++)
                {
                    if(mnuDevice->Items->Items[ct1]->Items[ct2]->Caption == DeviceType)
                        mnuDevice->Items->Items[ct1]->Items[ct2]->Click();
                }
            }
            else
            {
                if(mnuDevice->Items->Items[ct1]->Caption == DeviceType)
                    mnuDevice->Items->Items[ct1]->Click();
            };
        }
      }

      if(MyKey->ValueExists("FILES"))
      {
        FileOpen->InitialDir = MyKey->ReadString("FILES");
        FileSave->InitialDir = FileOpen->InitialDir;
      }

      if(MyKey->ValueExists("BURN"))
        burn = MyKey->ReadInteger("BURN");

      if(MyKey->ValueExists("E8751"))
        e8751 = MyKey->ReadInteger("E8751");
 if(MyKey->ValueExists("Epr_eep="))         Epr.eep =           MyKey->ReadInteger("Epr_eep=");
 if(MyKey->ValueExists("Epr_DipSW="))       Epr.DIP =           MyKey->ReadInteger("Epr_DipSW=");
 if(MyKey->ValueExists("Epr_MemorySize="))  Epr.BufferSize =    MyKey->ReadInteger("Epr_MemorySize=");
 if(MyKey->ValueExists("Epr_SectorSize="))  Epr.Ssize =         MyKey->ReadInteger("Epr_SectorSize=");
 if(MyKey->ValueExists("Epr_tWC="))         Epr.tWC =           MyKey->ReadInteger("Epr_tWC=");
 if(MyKey->ValueExists("Epr_tWP=")) 	    Epr.tWP =           MyKey->ReadInteger("Epr_tWP=");
 if(MyKey->ValueExists("Epr_ShiftAdrress=")) Epr.shift_adr =    MyKey->ReadInteger("Epr_ShiftAdrress=");
 if(MyKey->ValueExists("Epr_Erp64="))	    Epr.epr64 =         MyKey->ReadInteger("Epr_Erp64=");
 if(MyKey->ValueExists("Epr_AddStep="))	    Epr.add_step =      MyKey->ReadInteger("Epr_AddStep=");
 if(MyKey->ValueExists("Epr_SwapAdress="))	Epr.swap =          MyKey->ReadInteger("Epr_SwapAdress=");
 if(MyKey->ValueExists("Epr_FastErase="))	Epr.qe =            MyKey->ReadInteger("Epr_FastErase=");
 if(MyKey->ValueExists("24Cpin7="))		    Epr.pin7 =          MyKey->ReadInteger("24Cpin7=");
 if(MyKey->ValueExists("Epr_BootBlockSize="))   Epr.BL_size =   MyKey->ReadInteger("Epr_BootBlockSize=");
 if(MyKey->ValueExists("Epr_TopBottom="))	Epr.top_bottom =    MyKey->ReadInteger("Epr_TopBottom=");
 if(MyKey->ValueExists("Epr_pin28="))	    Epr.pin28 =         MyKey->ReadInteger("Epr_pin28=");
 if(MyKey->ValueExists("Epr_pin30="))	    Epr.pin30 =         MyKey->ReadInteger("Epr_pin30=");
 if(MyKey->ValueExists("Epr_serial="))	    Epr.serial =        MyKey->ReadInteger("Epr_serial=");
 if(MyKey->ValueExists("Epr_PIC="))	        Epr.PIC =           MyKey->ReadInteger("Epr_PIC=");
 if(MyKey->ValueExists("Epr_willem="))	    Epr.willem =        MyKey->ReadInteger("Epr_willem=");
 if(MyKey->ValueExists("Epr_Dis_calcu="))	Epr.Dis_calcu =     MyKey->ReadInteger("Epr_Dis_calcu=");
 if(MyKey->ValueExists("Epr_Vpp="))	        Epr.Vpp =           MyKey->ReadInteger("Epr_Vpp=");

                                                	//------------
 if(MyKey->ValueExists("mcs51_Vpp="))	    AT89Cxx.Vpp =        MyKey->ReadInteger("mcs51_Vpp=");
 if(MyKey->ValueExists("mcs51_erase="))	    AT89Cxx.tErase =     MyKey->ReadInteger("mcs51_erase=");
 if(MyKey->ValueExists("mcs51_sLbit="))	    AT89Cxx.sig_Lbit =   MyKey->ReadInteger("mcs51_sLbit=");
 if(MyKey->ValueExists("mcs51_at89s="))	    AT89Cxx.at89s =      MyKey->ReadInteger("mcs51_at89s=");
 if(MyKey->ValueExists("mcs51_tWP="))	    AT89Cxx.tWP =        MyKey->ReadInteger("mcs51_tWP=");
 if(MyKey->ValueExists("mcs51_tWC="))	    AT89Cxx.tWC =        MyKey->ReadInteger("mcs51_tWC=");
 if(MyKey->ValueExists("mcs51_type="))	    AT89Cxx.type =       MyKey->ReadInteger("mcs51_type=");
 if(MyKey->ValueExists("mcs51_tWPb="))	    AT89Cxx.tWPb =       MyKey->ReadInteger("mcs51_tWPb=");
 if(MyKey->ValueExists("mcs51_tWPe="))	    AT89Cxx.tWPe =       MyKey->ReadInteger("mcs51_tWPe=");
 if(MyKey->ValueExists("mcs51_ird_lbit="))	AT89Cxx.ird_lbit =   MyKey->ReadInteger("mcs51_ird_lbit=");
 if(MyKey->ValueExists("mcs51_ModeLock="))	AT89Cxx.ModeLock =   MyKey->ReadInteger("mcs51_ModeLock=");
 if(MyKey->ValueExists("AVR_tWP="))		    AT90Sxx.tWP =        MyKey->ReadInteger("AVR_tWP=");
 if(MyKey->ValueExists("AVR_tWC="))		    AT90Sxx.tWC =        MyKey->ReadInteger("AVR_tWC=");


                                                	//--------
 if(MyKey->ValueExists("M93C_rd="))		m93Cxx.rd =          MyKey->ReadInteger("M93C_rd=");
 if(MyKey->ValueExists("M93C_wr="))		m93Cxx.wr =          MyKey->ReadInteger("M93C_wr=");
 if(MyKey->ValueExists("M93C_ewen="))	m93Cxx.ewen =        MyKey->ReadInteger("M93C_ewen=");
 if(MyKey->ValueExists("M93C_ewds="))	m93Cxx.ewds =        MyKey->ReadInteger("M93C_ewds=");
 if(MyKey->ValueExists("M93C_erase="))	m93Cxx.erase =       MyKey->ReadInteger("M93C_erase=");
 if(MyKey->ValueExists("M93C_eral="))	m93Cxx.eral =        MyKey->ReadInteger("M93C_eral=");
 if(MyKey->ValueExists("M93C_wral="))	m93Cxx.wral =        MyKey->ReadInteger("M93C_wral=");
 if(MyKey->ValueExists("M93C_count_bit="))	m93Cxx.count_bit =   MyKey->ReadInteger("M93C_count_bit=");
 if(MyKey->ValueExists("M93C_adres="))	m93Cxx.adres =       MyKey->ReadInteger("M93C_adres=");
 if(MyKey->ValueExists("M93C_data="))	m93Cxx.data =        MyKey->ReadInteger("M93C_data=");
 if(MyKey->ValueExists("M93C_bit16="))	m93Cxx.bit16 =       MyKey->ReadInteger("M93C_bit16=");
                                                //-----------
 if(MyKey->ValueExists("AT25_flag_page=")) AT25xxx.flag_page =  MyKey->ReadInteger("AT25_flag_page=");
 if(MyKey->ValueExists("AT25_pag_size="))  AT25xxx.page_size =  MyKey->ReadInteger("AT25_pag_size=");
   }
   MyKey->CloseKey();

   SetBufferSize(Epr.BufferSize);
   calcu_adr();
   Epr.pattern_adr = (unsigned int)0x01 << Epr.shift_adr;
   Dip(Epr.DIP);
}

//---------------------------------------------------------------------------
void __fastcall TfrmEprom::SaveSettings(void)
{
    TRegistry *MyKey = new TRegistry;

    if(MyKey->OpenKey("\\Software\\Willem\\Eprom\\",true))
    {
      // Form Controls
      MyKey->WriteInteger("PORT",cbPort->ItemIndex);
      MyKey->WriteInteger("METHOD",cbMethod->ItemIndex);
      MyKey->WriteInteger("ERASE",cbFastErase->ItemIndex);
      MyKey->WriteInteger("CHECKSUM",cbCheckType->ItemIndex);
      MyKey->WriteInteger("SKIP-FF",cbSkipFF->ItemIndex);
      MyKey->WriteString("DEVICE",edDevice->Caption);
      MyKey->WriteString("FILES",FileOpen->InitialDir);

      // Hidden Options (at the Moment)
      MyKey->WriteInteger("BURN",burn);
      MyKey->WriteInteger("E8751",e8751);

	MyKey->WriteInteger("Epr_eep=",  Epr.eep);      //therefore not  implemented
	MyKey->WriteInteger("Epr_DipSW=",Epr.DIP);
	MyKey->WriteInteger("Epr_MemorySize=",Epr.BufferSize);
	MyKey->WriteInteger("Epr_SectorSize=",Epr.Ssize);
	MyKey->WriteInteger("Epr_tWC=",Epr.tWC);
	MyKey->WriteInteger("Epr_tWP=",Epr.tWP);
	MyKey->WriteInteger("Epr_ShiftAdrress=",Epr.shift_adr);
	MyKey->WriteInteger("Epr_Erp64=",Epr.epr64);
	MyKey->WriteInteger("Epr_AddStep=",Epr.add_step);
	MyKey->WriteInteger("Epr_SwapAdress=",Epr.swap);
	MyKey->WriteInteger("Epr_FastErase=",Epr.qe);
	MyKey->WriteInteger("24Cpin7=",Epr.pin7);
	MyKey->WriteInteger("Epr_BootBlockSize=",Epr.BL_size);
	MyKey->WriteInteger("Epr_TopBottom=",Epr.top_bottom);
	MyKey->WriteInteger("Epr_pin28=",Epr.pin28);
	MyKey->WriteInteger("Epr_pin30=",Epr.pin30);
	MyKey->WriteInteger("Epr_serial=",Epr.serial);
	MyKey->WriteInteger("Epr_PIC=",Epr.PIC);
	MyKey->WriteInteger("Epr_willem=",Epr.willem);
  	MyKey->WriteInteger("Epr_Dis_calcu=",Epr.Dis_calcu);
  	MyKey->WriteInteger("Epr_Vpp=",Epr.Vpp);


	//--------------------------------------------------
	MyKey->WriteInteger("mcs51_Vpp=",AT89Cxx.Vpp);
	MyKey->WriteInteger("mcs51_erase=",AT89Cxx.tErase);
	MyKey->WriteInteger("mcs51_sLbit=",AT89Cxx.sig_Lbit);
	MyKey->WriteInteger("mcs51_at89s=",AT89Cxx.at89s);
	MyKey->WriteInteger("mcs51_tWP=",AT89Cxx.tWP);
	MyKey->WriteInteger("mcs51_tWC=",AT89Cxx.tWC);
	MyKey->WriteInteger("mcs51_type=",AT89Cxx.type);
	MyKey->WriteInteger("mcs51_tWPb=",AT89Cxx.tWPb);
	MyKey->WriteInteger("mcs51_tWPe=",AT89Cxx.tWPe);
	MyKey->WriteInteger("mcs51_ird_lbit=",AT89Cxx.ird_lbit);
	MyKey->WriteInteger("mcs51_ModeLock=",AT89Cxx.ModeLock);
	MyKey->WriteInteger("AVR_tWP=",AT90Sxx.tWP);
	MyKey->WriteInteger("AVR_tWC=",AT90Sxx.tWC);

	//----------------------------------------------
	MyKey->WriteInteger("M93C_rd=",m93Cxx.rd);
	MyKey->WriteInteger("M93C_wr=",m93Cxx.wr);
	MyKey->WriteInteger("M93C_ewen=",m93Cxx.ewen);
	MyKey->WriteInteger("M93C_ewds=",m93Cxx.ewds);
	MyKey->WriteInteger("M93C_erase=",m93Cxx.erase);
	MyKey->WriteInteger("M93C_eral=",m93Cxx.eral);
	MyKey->WriteInteger("M93C_wral=",m93Cxx.wral);
	MyKey->WriteInteger("M93C_count_bit=",m93Cxx.count_bit);
	MyKey->WriteInteger("M93C_adres=",m93Cxx.adres);
	MyKey->WriteInteger("M93C_data=",m93Cxx.data);
	MyKey->WriteInteger("M93C_bit16=",m93Cxx.bit16);
	//-------------------------------------------------
	MyKey->WriteInteger("AT25_flag_page=",AT25xxx.flag_page);
	MyKey->WriteInteger("AT25_pag_size=",AT25xxx.page_size);
    }
    MyKey->CloseKey();

}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::imgFlip1Click(TObject *Sender)
{
    // Flip between PIC settings and ZIF Position
    switch (Epr.eep) {
        case M12C50X:
        case M16C62X:
        case M16F84:  pPICSettings->Visible = !pPICSettings->Visible; break;
        case M24C16:
        case M24C32:  p24CSettings->Visible = !p24CSettings->Visible; break;
        case M28FXXX: pF28Settings->Visible = !pF28Settings->Visible; break;
        case M27XXX:
        case M28CXXX:
        case M27CXXX: p27CSettings->Visible = !p27CSettings->Visible; break;
    }
}

//---------------------------------------------------------------------------
void __fastcall TfrmEprom::i28Fx00_Click(TObject *Sender)
{
   // Setup for selected 28Fxx device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;
    int DIP;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    pe=0;

    saveEpr();
    switch(DeviceID)
    {
        case 1 : SetBufferSize(0x40000); ZIFImage=48;DIP = 20;break;
        case 2 : SetBufferSize(0x80000); ZIFImage=48;DIP = 20;break;
        case 3 : SetBufferSize(0x100000);ZIFImage=48;DIP = 20;break;
        case 4 : SetBufferSize(0x20000); ZIFImage=32;DIP = 30;break;
    }
	if(DeviceID>0) {
	   calcu_adr();
	   Epr.eep=I28FX00;
       Epr.tWP = 3;
       Epr.tWC = 12;
       Dip(DIP);
	   Epr.epr64 = ON;
	}else restoreEpr();
}

//---------------------------------------------------------------------------
void __fastcall TfrmEprom::GetID_Cick(TObject *Sender)
{
   if(Epr.serial == ON) Epr.flag = OFF;
   else Epr.flag = test_hw();
   if(!Epr.flag) {
    switch(Epr.eep) {
        case M28FXXX:   id_28Fxxx();    break;
        case AT49F00X:
        case AT49FXXX:  id_49Fxxx();    break;
        case M29FXXX:   id_29Fxxx();    break;
        case AM29FX00:  id_29Fx00();    break;
        case I28FX00:   id_i28Fx00();   break;
        case W29EEXXX:  id_W29EExxx();  break;
        case AT29CXXX:  id_29Cxxx();    break;
        case AT89CX051:
        case AT89CXXX:  id_AT89C();     break;
    }
   }
}

//---------------------------------------------------------------------------
void __fastcall TfrmEprom::r24Cpin7highClick(TObject *Sender)
{
    Epr.pin7 = 1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::r24Cpin7LowClick(TObject *Sender)
{
    Epr.pin7 = 0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::M27_Click(TObject *Sender)
{

    // Setup for selected 27xx device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;
    unsigned int DIP;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    pe=0;

    saveEpr();
    Epr.eep = M27CXXX;
    Epr.tWP = (1100);    // tWP = 1.1mS
    Epr.tWC = 20;
    Epr.add_step = 500;

    switch(DeviceID)
    {
        case 1 : SetBufferSize(0x800);   ZIFImage=24; DIP = 27; Epr.tWP = 25000; break;
        case 2 : SetBufferSize(0x1000);  ZIFImage=24; DIP = 9;  break;
        case 3 : SetBufferSize(0x2000);  ZIFImage=28; DIP = 1; Epr.Vpp = ON; Epr.epr64 = ON; break;
        case 4 : SetBufferSize(0x4000);  ZIFImage=28; DIP = 2; Epr.Vpp = ON; Epr.epr64 = ON; break;
        case 5 : SetBufferSize(0x8000);  ZIFImage=28; DIP = 3; Epr.Vpp = ON;  break;
        case 6 : SetBufferSize(0x10000); ZIFImage=28; DIP = 4;  break;
        case 7 : SetBufferSize(0x20000); ZIFImage=32; DIP = 7; Epr.Vpp = ON; Epr.epr64 = ON; break;
        case 8 : SetBufferSize(0x40000); ZIFImage=32; DIP = 7; Epr.Vpp = ON; Epr.epr64 = ON; break;
        case 9 : SetBufferSize(0x80000); ZIFImage=32; DIP = 10;Epr.Vpp = ON; break;
    }
   	if(DeviceID>0){
	     calcu_adr();
         Dip(DIP);
	}else restoreEpr();
}

//---------------------------------------------------------------------------


void __fastcall TfrmEprom::Am29Fx00_Click(TObject *Sender)
{
  // Setup for selected 29Fx00 device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;
    int DIP;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    pe=0;

    saveEpr();
    switch(DeviceID)
    {
        case 1 : SetBufferSize(0x40000); ZIFImage=48;DIP = 20;break;
        case 2 : SetBufferSize(0x80000); ZIFImage=48;DIP = 20;break;
        case 3 : SetBufferSize(0x100000);ZIFImage=48;DIP = 20;break;
    }
	if(DeviceID>0) {
	   calcu_adr();
	   Epr.eep=AM29FX00;
       Epr.tWP = 2;
       Epr.tWC = 20;
	   Epr.epr64 = ON;
       Dip(DIP);
	}else restoreEpr();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnLOCKBIT_Click(TObject *Sender)
{
    frmEprom->pIDcode->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::Device1Click(TObject *Sender)
{
    TPoint P, Q;

        P.x = 32;  //X + edDevice->Left + Panel4->Left;
	    P.y = 0;   // Y + edDevice->Top + Panel4->Top;
	    Q = ClientToScreen(P);
        mnuDevice->Popup(Q.x,Q.y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEprom::SRAM_Click(TObject *Sender)
{
    // Setup for selected 28Fxx device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;
    unsigned int DIP;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    pe=0;

    saveEpr();

	switch(DeviceID){
		case 1:SetBufferSize(0x800);
                DIP = 26;
                ZIFImage=24;
				Epr.shift_adr = 10;
		        break;
		case 2:SetBufferSize(0x2000);
				DIP = 25;
                ZIFImage=28;
				Epr.shift_adr = 13;
				Epr.pin28 = ON;
		        break;
		case 3:SetBufferSize(0x8000);
				DIP = 25;
                ZIFImage=28;
				Epr.swap = ON;
				Epr.shift_adr = 15;
		        break;
		case 4:SetBufferSize(0x10000);
				DIP = 24;
                ZIFImage=32;
				Epr.swap = ON;
				Epr.shift_adr = 18;
				Epr.pin30  = ON;
		        break;
		case 5:SetBufferSize(0x20000);
				DIP = 24;
                ZIFImage=32;
				Epr.swap = ON;
				Epr.shift_adr = 18;
				Epr.pin30 = ON;
		        break;
	}
	if(DeviceID>0) {
         Epr.Dis_calcu   = ON;
	     Epr.pattern_adr = (unsigned long)0x01 << Epr.shift_adr;
	     Epr.eep= RAM,pe=1;
	     Epr.tWP = 3; Epr.tWC = 5;
	     Epr.epr64 = ON;
         grot = Epr.BufferSize;
         Dip(DIP);
	} else restoreEpr();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::DS12XX_Click(TObject *Sender)
{
    // Setup for selected 28Fxx device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;
    unsigned int DIP;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    pe=0;

	saveEpr();
	switch(DeviceID){
		case 1:SetBufferSize(0x800);
				DIP =26;
                ZIFImage=24;
				Epr.shift_adr = 10;
		        break;
		case 2:SetBufferSize(0x2000);
				DIP = 25;
                ZIFImage=28;
				Epr.shift_adr = 12;
		        break;
		case 3:SetBufferSize(0x8000);
				DIP = 25;
                ZIFImage=28;
				Epr.swap = ON;
				Epr.shift_adr = 15;
		        break;
		case 4:SetBufferSize(0x20000);
				DIP = 24;
                ZIFImage=32;
				Epr.swap = ON;
				Epr.shift_adr = 18;
		        break;
		case 5:SetBufferSize(0x40000);
				DIP = 24;
                ZIFImage=32;
				Epr.swap = ON;
				Epr.shift_adr = 18;
		        break;
	}
	if(DeviceID>0) {
         Epr.Dis_calcu   = ON;
	     Epr.pattern_adr = (unsigned long)0x01 << Epr.shift_adr;
	     Epr.eep= DS12XX,pe=1;;
	     Epr.tWP = 3; Epr.tWC = 5;
	     Epr.epr64 = ON;
         Dip(DIP);
	} else restoreEpr();
}

//---------------------------------------------------------------------------

void __fastcall TfrmEprom::sbWillem_Click(TObject *Sender)
{
     if(Epr.willem)Epr.willem  = 0;
     else  Epr.willem  = 1;
     winstat();
}
//---------------------------------------------------------------------------


void __fastcall TfrmEprom::AT29C_Click(TObject *Sender)
{
    // Setup for selected 29Cxx device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;
    unsigned int DIP;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    pe=0;

    saveEpr();
       Epr.eep=AT29CXXX;
       Epr.tWP = 2;
       Epr.tWC = 15000;
   	   Epr.epr64 = ON;
    switch(DeviceID)
    {
        case 10: SetBufferSize(0x8000); ZIFImage=28;DIP=28;
                 Epr.swap = ON;Epr.Ssize = 64; break;    // willem PCB  AT29C256
        case 11: SetBufferSize(0x8000); ZIFImage=28;DIP=29;
                 Epr.swap = ON;Epr.Ssize = 64; break;    // PCB3  AT29C256
        case 1 : SetBufferSize(0x10000);ZIFImage=32;DIP=7;Epr.Ssize = 128; break;
        case 2 : SetBufferSize(0x20000);ZIFImage=32;DIP=7;Epr.Ssize = 128; break;
        case 3 : SetBufferSize(0x40000);ZIFImage=32;DIP=7;Epr.Ssize = 256; break;
        case 4 : SetBufferSize(0x80000);ZIFImage=32;DIP=19;Epr.Ssize = 256; break;
        case 5 : SetBufferSize(0x80000);ZIFImage=32;DIP=19;Epr.Ssize = 512; break;

        case 6 : SetBufferSize(0x10000);ZIFImage=32;DIP=7;Epr.Ssize = 128; Epr.eep = W29EEXXX;break;
        case 7 : SetBufferSize(0x20000);ZIFImage=32;DIP=7;Epr.Ssize = 128; Epr.eep = W29EEXXX;break;
        case 8 : SetBufferSize(0x40000);ZIFImage=32;DIP=7;Epr.Ssize = 128; Epr.eep = W29EEXXX;break;
        case 9 : SetBufferSize(0x80000);ZIFImage=32;DIP=19;Epr.Ssize = 256; Epr.eep = W29EEXXX;break;

    }
    if(DeviceID>0) {
	   calcu_adr();
       Dip(DIP);
	}else restoreEpr();
}
//---------------------------------------------------------------------------



void __fastcall TfrmEprom::muMCS51_Click(TObject *Sender)
{
    // Setup for selected MCS-51 device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;
    unsigned int DIP;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    pe=0;

    saveEpr();
    Epr.eep = AT89CXXX;
	AT89Cxx.initl_code();
	AT89Cxx.at89s 	= OFF;
    AT89Cxx.tWC     = 2000;
	AT89Cxx.tWP 	= 12;
	AT89Cxx.Pulses 	= 1;
	AT89Cxx.type 	= DIP40;
	Epr.shift_adr	= 18;
	Epr.pattern_adr = (unsigned long)0x01 << Epr.shift_adr;
    AT89Cxx.ModeLock = 0;
    rbLock0->Checked = true;
    rbLock123->Enabled = true;

	switch(DeviceID){
		 case 2: SetBufferSize(0x1000);  break; // AT89C51
		 case 3: SetBufferSize(0x2000);  break; // AT89C52
   		 case 4: SetBufferSize(0x5000); break;  // AT89C55
		 case 5: SetBufferSize(0x2800);         // AT89S8252
			    AT89Cxx.at89s 	= ON;
			    AT89Cxx.tWP 	= 30;
			    AT89Cxx.ird_lbit 	= ON;
			    break;
		 case 6: SetBufferSize(0x3000);         // AT89S53
			    AT89Cxx.at89s 	= ON;
			    AT89Cxx.tWP 	= 30;
			    AT89Cxx.ird_lbit 	= ON;
			    break;

		 case 8: SetBufferSize(0x8000);     // AT89C51RC
              AT89Cxx.tWC       = 80;
			  AT89Cxx.tWP	   	= 5;
			  AT89Cxx.tWPb   	= 120;
			  AT89Cxx.tWPe   	= 3;
			  AT89Cxx.ird_lbit 	= ON;
			  AT89Cxx.tErase 	= 1;
			  break;
		 case 7: SetBufferSize(0x5000);     // AT89C55WD
              AT89Cxx.tWC       = 80;
			  AT89Cxx.tWP	   	= 5;
			  AT89Cxx.tWPb   	= 120;
			  AT89Cxx.tWPe   	= 3;
			  AT89Cxx.ird_lbit 	= ON;
			  AT89Cxx.tErase 	= 1;
			  break;

		 case 9: SetBufferSize(0x1000);
			 AT89Cxx.tWP = 110; AT89Cxx.Pulses = 25;
			 break;
		 case 10: SetBufferSize(0x2000);
			 AT89Cxx.tWP = 110; AT89Cxx.Pulses = 25;
			 break;
		 case 11: SetBufferSize(0x4000);
			 AT89Cxx.tWP = 110; AT89Cxx.Pulses = 25;
			 break;
		 case 12: SetBufferSize(0x8000);
			  AT89Cxx.tWP = 110; AT89Cxx.Pulses = 25;
             break;

	}
    if(DeviceID>1) {
    	ZIFImage=51;
        Epr.Dis_calcu   = ON;
        Dip(13);
    }
	else restoreEpr();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::AT89Cx051_Click(TObject *Sender)
{
    // Setup for selected MCS-51 device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;
    unsigned int DIP;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    pe=0;

	saveEpr();
    Epr.eep = AT89CX051;
	AT89Cxx.initl_code();
	AT89Cxx.at89s 	= 0;
	AT89Cxx.tWP 	= 12;
	AT89Cxx.Pulses 	= 1;
	AT89Cxx.type 	= DIP20;
    Epr.Dis_calcu   = ON;
    Epr.shift_adr   = 18;
	Epr.pattern_adr = (unsigned long)0x01 << Epr.shift_adr;
    rbLock0->Checked = true;
    rbLock123->Enabled = false;

	switch(DeviceID){
		case 2:SetBufferSize(0x400); break;
		case 3:SetBufferSize(0x800); break;
		case 4:SetBufferSize(0x1000); break;
	}
	if(DeviceID > 0) {
        Epr.Dis_calcu   = ON;
	    ZIFImage=2051;
        Dip(14);
    }
	else restoreEpr();
}

//---------------------------------------------------------------------------
void __fastcall TfrmEprom::Mcs51_lockbitChange(TObject *Sender)
{
 AT89Cxx.ModeLock = Mcs51_lockbit->ItemIndex;
}
//---------------------------------------------------------------------------


void __fastcall TfrmEprom::btnLockBitClick(TObject *Sender)
{
        MCS51_lockbit();
        pdLockBit->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnCancelLock_Click(TObject *Sender)
{
        pdLockBit->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::sbLockBitClick(TObject *Sender)
{
   if(Epr.serial == ON) Epr.flag = OFF;
   else Epr.flag = test_hw();
   if(!Epr.flag) {
      Mcs51_lockbit->ItemIndex = 0;
      pdLockBit->Visible = true;
   }   
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::AT49F00x_Click(TObject *Sender)
{
    // Setup for selected 29Fxx device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;
    unsigned int DIP;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    pe=0;

    saveEpr();
    frmEprom->cbProtect->Checked = false;
    Epr.AT49F00x = OFF;
    frmEprom->cbProtect->Enabled = false;
    switch(DeviceID)
    {
        case 1 : SetBufferSize(0x20000);ZIFImage=32;DIP=16;
                 frmEprom->cbProtect->Enabled = true;break;
        case 2 : SetBufferSize(0x20000);ZIFImage=32;DIP=16;
                 break;
        case 3 : SetBufferSize(0x40000);ZIFImage=32;DIP=16;
                 frmEprom->cbProtect->Enabled = true;break;
        case 4 : SetBufferSize(0x40000);ZIFImage=32;DIP=16;
                 break;
    }
    if(DeviceID>0) {
	   calcu_adr();
       Epr.eep = AT49F00X; Epr.tWP = 5; Epr.tWC = 20;
       Epr.AT49F00x = frmEprom->cbProtect->Checked;
       Dip(DIP);
	   Epr.epr64 = ON;
	}else restoreEpr();

}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::cbProtectClick(TObject *Sender)
{
  Epr.AT49F00x = cbProtect->Checked;
}
//---------------------------------------------------------------------------


void __fastcall TfrmEprom::sbRdLBClick(TObject *Sender)
{
        pRdLB->Visible = true;
        rdLbit_AT89C();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnRdLbOKClick(TObject *Sender)
{
        pRdLB->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::AT28Cxxx_Click(TObject *Sender)
{
   // Setup for selected 29Cxx device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;
    unsigned int DIP;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    pe=0;

    saveEpr();
    switch(DeviceID)
    {
        case 1 : SetBufferSize(0x2000);ZIFImage=28;DIP=5;Epr.Ssize = 64; break;
        case 2 : SetBufferSize(0x8000);ZIFImage=28;DIP=18;Epr.Ssize = 64;
                 Epr.Dis_calcu = ON;  Epr.swap = ON;
                 break;
        case 3 : SetBufferSize(0x10000);ZIFImage=32;DIP=7;Epr.Ssize = 128; break;
        case 4 : SetBufferSize(0x20000);ZIFImage=32;DIP=7;Epr.Ssize = 128; break;
        case 5 : SetBufferSize(0x40000);ZIFImage=32;DIP=7;Epr.Ssize = 256; break;
        case 6 : SetBufferSize(0x80000);ZIFImage=32;DIP=19;Epr.Ssize = 256; break;
    }
    if(DeviceID>0) {
	   calcu_adr();
       Epr.eep = AT28CXXX;
       Epr.tWP = 2;
       Epr.tWC = 10000;
   	   Epr.epr64 = ON;
       Dip(DIP);
	}else restoreEpr();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::ChangePriority(bool Fast)
{
  unsigned long ProcessID;
  HANDLE ProcessHandle;
  HANDLE ThreadHandle;

  ProcessID = GetCurrentProcessId();
  ProcessHandle = OpenProcess(PROCESS_SET_INFORMATION,false,ProcessID);
  ThreadHandle = GetCurrentThread;

  if(Fast)
  {
    SetPriorityClass(ProcessHandle, HIGH_PRIORITY_CLASS);     // REALTIME_PRIORITY_CLASS
    SetThreadPriority(ThreadHandle, THREAD_PRIORITY_HIGHEST); // THREAD_PRIORITY_TIME_CRITICAL
  }
  else
  {
    SetPriorityClass(ProcessHandle, NORMAL_PRIORITY_CLASS);
    SetThreadPriority(ThreadHandle, THREAD_PRIORITY_NORMAL);
  }
}

//---------------------------------------------------------------------------
void __fastcall TfrmEprom::cb29Ctwc_Change(TObject *Sender)
{
  switch(cb29C_twc->ItemIndex) {
    case 0:  Epr.tWC  = 5000; break;
    case 1:  Epr.tWC  = 10000; break;
    case 2:  Epr.tWC  = 15000; break;
    case 3:  Epr.tWC  = 20000; break;
    case 4:  Epr.tWC  = 25000; break;
  }
  winstat();
}

//---------------------------------------------------------------------------
void __fastcall TfrmEprom::cbSsizeChange(TObject *Sender)
{
  switch(cbSsize->ItemIndex) {
    case 0:  Epr.Ssize  = 64; break;
    case 1:  Epr.Ssize  = 128; break;
    case 2:  Epr.Ssize  = 256; break;
    case 3:  Epr.Ssize  = 512; break;
    case 4:  Epr.Ssize  = 1024; break;
    case 5:  Epr.Ssize  = 2048; break;
  }
  winstat();
}

//---------------------------------------------------------------------------
void __fastcall TfrmEprom::rbLock0Click(TObject *Sender)
{
    AT89Cxx.ModeLock = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEprom::rbLock1Click(TObject *Sender)
{
    AT89Cxx.ModeLock = 1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::rbLock12Click(TObject *Sender)
{
    AT89Cxx.ModeLock = 2;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::rbLock123Click(TObject *Sender)
{
    AT89Cxx.ModeLock = 3;
}
//---------------------------------------------------------------------------


void __fastcall TfrmEprom::M27E_Click(TObject *Sender)
{
    // Setup for selected 27Cxx device

    TMenuItem *Selected = (TMenuItem *)(Sender);
    int DeviceID = Selected->Tag;
    unsigned int DIP;

    edDevice->Caption = Selected->Caption;
    C622=0;C89=0;F84=0;C50=0;C93=0;
    pe=0;

    saveEpr();
    switch(DeviceID)
    {
        case 1 : SetBufferSize(0x8000);  ZIFImage=28; DIP = 3;  Epr.Vpp   = ON; break;
        case 2 : SetBufferSize(0x10000); ZIFImage=28; DIP = 4;  break;
        case 3 : SetBufferSize(0x20000); ZIFImage=32; DIP = 7;  Epr.Vpp   = ON; Epr.epr64 = ON; break;
        case 4 : SetBufferSize(0x40000); ZIFImage=32; DIP = 7;  Epr.Vpp   = ON; Epr.epr64 = ON; break;
        case 5 : SetBufferSize(0x80000); ZIFImage=32; DIP = 10; Epr.Vpp   = ON; break;

     }
   	if(DeviceID>0){
	     Epr.eep = W27EXXX;
	     Epr.tWP = TICKS(110);    // tWP = 120uS
         Epr.tWC = 15;
	     Epr.add_step = 100;
	     calcu_adr();
         Dip(DIP);
	}else restoreEpr();
}


void __fastcall TfrmEprom::btnPowerOnClick(TObject *Sender)
{
powon;wil_assert;
}

void __fastcall TfrmEprom::TabSheet3Exit(TObject *Sender)
{
power_down();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnPowerOffClick(TObject *Sender)
{
powoff;wil_assert;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnVppOnClick(TObject *Sender)
{
VPP_On;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnVppOffClick(TObject *Sender)
{
VPP_Off;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnWEHighClick(TObject *Sender)
{
WE_High;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnWELowClick(TObject *Sender)
{
WE_Low;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnOEHighClick(TObject *Sender)
{
OE_High;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnOELowClick(TObject *Sender)
{
OE_Low;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnAddr0Click(TObject *Sender)
{
unsigned long r;
OE_High;
r = Epr.pattern_adr;
Epr.pattern_adr = (unsigned long)0x01 << 18; // A0->A18
r_adres_28C(0x0000);
Epr.pattern_adr=r;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnAddr2aaaaClick(TObject *Sender)
{
unsigned long r;
OE_High;
r = Epr.pattern_adr;
Epr.pattern_adr = (unsigned long)0x01 << 18; // A0->A18
r_adres_28C(0x2aaaa);
Epr.pattern_adr=r;

}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnAddr55555Click(TObject *Sender)
{
unsigned long r;
OE_High;
r = Epr.pattern_adr;
Epr.pattern_adr = (unsigned long)0x01 << 18; // A0->A18
r_adres_28C(0x55555);
Epr.pattern_adr=r;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnAddr7ffffClick(TObject *Sender)
{
unsigned long r;
OE_High;
r = Epr.pattern_adr;
Epr.pattern_adr = (unsigned long)0x01 << 18; // A0->A18
r_adres_28C(0x7ffff);
Epr.pattern_adr=r;
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnData0Click(TObject *Sender)
{
OE_Low;
Outport(poort,0x00);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnDataAAClick(TObject *Sender)
{
OE_Low;
Outport(poort,0xaa);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnData55Click(TObject *Sender)
{
OE_Low;
Outport(poort,0x55);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnDataFFClick(TObject *Sender)
{
OE_Low;
Outport(poort,0xff);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnHWPresenceClick(TObject *Sender)
{
test_hw();        
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::btnAllOffClick(TObject *Sender)
{
power_down();
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::edDataOutChange(TObject *Sender)
{
int dataout;
dataout=StrToIntDef(edDataOut->Text,0); //If input data not valid, dataout=0.
if (dataout<0)
        {
        dataout=0;
        }
if (dataout>255)
        {
        dataout=0;
        }
OE_Low;
Outport(poort,dataout);
}
//---------------------------------------------------------------------------

void __fastcall TfrmEprom::edAddrOutChange(TObject *Sender)
{
int AddrOut;
unsigned long r;

AddrOut=StrToIntDef(edAddrOut->Text,0); //If input data not valid, AddrOut=0.
if (AddrOut<0)
        {
        AddrOut=0;
        }
if (AddrOut>0x7ffff)
        {
        AddrOut=0;
        }
r = Epr.pattern_adr;
Epr.pattern_adr = (unsigned long)0x01 << 18; // A0->A18
OE_High;
r_adres_28C(AddrOut);
Epr.pattern_adr=r;
}
//---------------------------------------------------------------------------





//---MXIC F29040 chip  unprotection
void __fastcall TfrmEprom::sbMXICF29ChipUnprtClick(TObject *Sender)
{
// NOTE : MXIC recomand that all sectors must be protected
// before performing chip unprotect.
int ProtCheck,sectProtStatus,sectAddr,i;
bool FlagExit=false;

//StatusBar->Caption="";

//If all sect. are unprotect do nothing.
ProtCheck=sectProtStatus=0;
for (i=0; i<8; i++)
        {
        sectAddr=(0x10000*i)+2; // Sector address + a2=1
        sectProtStatus=MXIC_F29040_Sect_Status(sectAddr);
        ProtCheck=sectProtStatus+ProtCheck;
        }
if (ProtCheck==0)
        {
        frmEprom->Message("All sectors are Unprotected !",1);
        goto Exit;
        }

//Loocking for unprotected sectors
for (i=0; i<8; i++)
        {
        sectAddr=(0x10000*i)+2;
        sectProtStatus=MXIC_F29040_Sect_Status(sectAddr);
        if (sectProtStatus!=1)
                {
                sectAddr=(0x10000*i)+ 512; // Sect. address + a9=1
                MXIC_F29040_Sect_Protect(sectAddr); // Sector protection
                sectAddr=(0x10000*i)+2; // Sect. address + a2=1
                sectProtStatus=MXIC_F29040_Sect_Status(sectAddr);
                if (sectProtStatus!=1)
                        {
                        frmEprom->Message("Protection of Sector N. "+IntToStr(i)+" may be not possible!",1);
                        FlagExit=true;
                        break;
                        }
                }
        }
//If a sect. is not protectable exit.
if (FlagExit==true)
        {
        goto Exit;
        }

MXIC_F29040_Chip_Unprot();  //Chip Unprotect

// Display status of all sectors
ProtCheck=sectProtStatus=0;
for (i=0; i<8; i++)
        {
        sectAddr=(0x10000*i)+2;
        sectProtStatus=MXIC_F29040_Sect_Status(sectAddr);
        ProtCheck=sectProtStatus+ProtCheck;
        sgMXICF29SecVfyF29->Cells[0][i+1]="S"+Format("%1.1d",ARRAYOFCONST(((int)i)))+
                                          "     "+Format("%5.5x",ARRAYOFCONST(((int)0x10000*i)))+
                                          "   "+Format("%5.5x",ARRAYOFCONST(((int)(0x10000*i)+0xffff)))+
                                          "    "+IntToStr(sectProtStatus);
        }
// All sectors unprotected ?
if (ProtCheck>0)
        {
        frmEprom->Message("Sectors NOT unprotected !",1);
        goto Exit;
        }
frmEprom->Message("Chip Unprotection performed OK!",2);
Exit:

}
//---------------------------------------------------------------------------


//---MXIC F29040 sector protection
void __fastcall TfrmEprom::sbMXICF29SecProtClick(TObject *Sender)
{
int N_Sector,sectProtStatus,sectAddr,i;

//StatusBar->Caption="";

//Select sector to be protect
N_Sector= (sgMXICF29SecVfyF29->Row)-1;

//Check for sectors already protected
sectAddr=(N_Sector*0x10000)+2; // Sector address + a2=1
sectProtStatus=MXIC_F29040_Sect_Status(sectAddr);

if (sectProtStatus==1)
        {
        frmEprom->Message("Sector already protected !",1);
        goto Exit;
        }
//Check if protect code status is valid
if ((sectProtStatus !=0) & (sectProtStatus !=1))
        {
        frmEprom->Message("Protect/Unprotect status error !",1);
        goto Exit;
        }

//Protection of selected sector
sectAddr=(N_Sector*0x10000)+ 512; // Sector address + a9=1
MXIC_F29040_Sect_Protect(sectAddr);

//Verify if sector correctly protected
sectAddr=(N_Sector*0x10000)+2; // Sector address + a2=1
sectProtStatus=MXIC_F29040_Sect_Status(sectAddr);
if (sectProtStatus!=1)
        {
        frmEprom->Message("Sector not protected !",1);
        goto Exit;
        }

// Display status of all sectors
for (i=0; i<8; i++)
        {
        sectAddr=(0x10000*i)+2; // Sector address + a2=1
        sectProtStatus=MXIC_F29040_Sect_Status(sectAddr);
        sgMXICF29SecVfyF29->Cells[0][i+1]="S"+Format("%1.1d",ARRAYOFCONST(((int)i)))+
                                          "     "+Format("%5.5x",ARRAYOFCONST(((int)0x10000*i)))+
                                          "   "+Format("%5.5x",ARRAYOFCONST(((int)(0x10000*i)+0xffff)))+
                                          "    "+IntToStr(sectProtStatus);
        }
frmEprom->Message("Sector protect performed OK!",2);

Exit:
}

//---------------------------------------------------------------------------

//--- Display MXIC F29040 sector protection status
void __fastcall TfrmEprom::sbMXICF29SecStatusClick(TObject *Sender)
{
int i,addr,sectProtStatus;

//Disable sector protect/chip unprotect untill display sector status
sbMXICF29SecProt->Enabled=true;
sbMXICF29ChipUnprt->Enabled=true;

//sgMXICF29SecVfyF29->Cells[0][0]="Sec        Address        Stat"; -->In form create
for (i=0; i<8; i++)
        {
        addr=(0x10000*i)+2;
        sectProtStatus=MXIC_F29040_Sect_Status(addr);
        sgMXICF29SecVfyF29->Cells[0][i+1]="S"+Format("%1.1d",ARRAYOFCONST(((int)i)))+
                                          "     "+Format("%5.5x",ARRAYOFCONST(((int)0x10000*i)))+
                                          "   "+Format("%5.5x",ARRAYOFCONST(((int)(0x10000*i)+0xffff)))+
                                          "    "+IntToStr(sectProtStatus);
        }
}
//---------------------------------------------------------------------------





