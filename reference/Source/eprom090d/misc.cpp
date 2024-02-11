#include "uEprom.h"
#include "willem.h"
#include "willem2.h"
#include <stdio.h>

//-------------------------------
void __fastcall verify_28C(void)
{
	unsigned int  i, y,index;
//    bool OK = true;
    unsigned char g;

  	if(st!=0)getoff();

	setup();
	if(Epr.epr64){we_hi;}
    else { we_lo; }

	if(grot>=Epr.BufferSize) grot=Epr.BufferSize-1;
    frmEprom->SetProcessGauge("Verifying chip",grot-1,clGreen);

    Epr.flag_err = OFF;
	for(i=0;i<=grot;i++)
    {
		y=i;   index = y;

		// if (Epr.swap == ON) shift A14-->A15-->A18
		if(Epr.swap == ON) index =   swap_adr(index);

		r_adres_28C(index);
        g = in_word();
		if(g!=Buffer[y])
        {
            error2(i,0,g);
      		power_down();/*wil_assert;*/         //ARW: was powoff
            Epr.flag_err = ON;
			break;//for i
		}

        frmEprom->gProcess->Progress = i;

        Application->ProcessMessages();
        if(cancel)
        {
          frmEprom->Message("program interrupted!",1);
          Epr.flag_err = ON;
          break;
        }
	}
    if(!Epr.flag_err) {
        if(!Epr.Dis_showV)frmEprom->Message("Device Verify OK",2);
    }
	power_down();/*wil_assert;*/          //ARW: was powoff
    winstat();
}
//------------------------------------------
void __fastcall CRead_28C(void)
{
  unsigned int i, y,index,step,e;

  ClearBuf();

  if(st!=0)getoff();

  frmEprom->SetProcessGauge("Reading Chip", Epr.BufferSize,clBlack);

  step = Epr.BufferSize/100;
  e = 0;

  setup();
  if(Epr.epr64) {we_hi;}
  else we_lo;

  r_adres_28C(0);
  for(i=0;i<Epr.BufferSize;i++)
  {
  	y=i+st; index = y;
	if (Epr.swap == ON)  index = swap_adr(index);

  	r_adres_28C(index);
  	Buffer[y]=in_word();
    e++ ;
    if( e == step) {
        frmEprom->gProcess->Progress = i;
        Application->ProcessMessages();
        e = 0;
    }
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

void calcu_adr(void)
{
     unsigned long adr;
     int i = 0;
     if(!Epr.Dis_calcu )   {
         adr = Epr.BufferSize;
         for(;adr>0;) {
    	     adr = adr/2;
    	    i++;
         }
        Epr.shift_adr = i-2;
     }
     Epr.pattern_adr = (unsigned long)0x01 << Epr.shift_adr;

}

void __fastcall r_adresx(unsigned long adr,unsigned int delay)
{
	unsigned long bit,r;
	int b;

	r = Epr.pattern_adr;
	oe_lo; if(delay>0) ms_delay(delay);
	Outport(poort,0x00);
	for (b=(Epr.shift_adr+1); b!=0; b--){
		bit=(adr&r);
		bit ? adrhi:adrlo; asadres;
		clkhi, asadres;
		clklo, asadres;
		r=r>>1;
	}
	adrlo; asadres;
}

void __fastcall wr_data(unsigned char data,unsigned int delay)
{
	oe_hi;if(delay > 0) ms_delay(delay);
	Outport(poort,data);
}

void __fastcall we_pulse(unsigned int delay){
	we_lo;if(delay>0) ms_delay(delay); we_hi
}

void saveEpr(void){
      Epr1.tWC 	     = Epr.tWC;
      Epr1.tWP 	     = Epr.tWP;
      Epr1.shift_adr = Epr.shift_adr;
      Epr1.epr64     = Epr.epr64;
      Epr1.eep 	     = Epr.eep;
      Epr1.oe_vpp    = Epr.oe_vpp;
      Epr1.swap	     = Epr.swap;
      Epr1.pin28     = Epr.pin28;
      Epr1.pin30     = Epr.pin30;
      Epr1.serial    = Epr.serial;
      Epr1.PIC	     = Epr.PIC;
      Epr1.Dis_calcu  = Epr.Dis_calcu;
      Epr1.Vpp       = Epr.Vpp;

      Epr.epr64  = OFF;
      Epr.oe_vpp = OFF;
      Epr.swap   = OFF;
      Epr.tWC    = 10;
      Epr.tWP    = 1;
      Epr.shift_adr  =  12;
      Epr.pin28  = OFF;
      Epr.pin30  = OFF;
      Epr.serial = OFF;
      Epr.PIC	 = 0;
      Epr.Dis_calcu  = OFF;
      Epr.Vpp   = OFF;
}

void  restoreEpr(void){
      Epr.tWC 	     = Epr1.tWC;
      Epr.tWP 	     = Epr1.tWP;
      Epr.shift_adr  = Epr1.shift_adr;
      Epr.epr64      = Epr1.epr64;
      Epr.eep 	     = Epr1.eep;
      Epr.oe_vpp     = Epr1.oe_vpp;
      Epr.swap       = Epr1.swap;
      Epr.pin28	     = Epr1.pin28;
      Epr.pin30	     = Epr1.pin30;
      Epr.serial     = Epr1.serial;
      Epr.PIC	     = Epr1.PIC;
      Epr.Dis_calcu  = Epr1.Dis_calcu;
      Epr.Vpp        = Epr1.Vpp;
}

//---------------------------------------------------------

void show_twptwc(void)
{
     char buffer[20];
     int i,twp,twc;
     switch(Epr.eep) {
        case AT89CX051:
        case AT89CXXX: twp = AT89Cxx.tWP; twc = AT89Cxx.tWC; break;
        default:       twp = Epr.tWP; twc = Epr.tWC;   break;
     }

     if(twp <= 999) sprintf(buffer," %d uS",twp);
     else sprintf(buffer," %d mS",twp/1000);
     frmEprom->edTWP->Text = buffer;

     if(twc <= 999) sprintf(buffer," %d uS",twc);
     else sprintf(buffer," %d mS",twc/1000);
     frmEprom->edTWC->Text = buffer;
}

void show_ID(unsigned char a,unsigned char b)
{
      frmEprom->pIDcode->Visible = true;
      char ch[20],Manufac[20];
      switch(a) {
	     case 0x1F: strcpy(Manufac," ATMEL"); 	break;
	     case 0xDA: strcpy(Manufac," ASD/WinBond"); 	break;
	     case 0x20: strcpy(Manufac," ST"); 		break;
	     case 0x01: strcpy(Manufac," AMD"); 	break;
	     case 0x31: strcpy(Manufac," CATALYST"); 	break;
	     case 0x89: strcpy(Manufac," Intel"); 	break;
	     case 0xAD: strcpy(Manufac," Hyudai"); 	break;
	     case 0xBF: strcpy(Manufac," SST"); 	break;
	     case 0xC2: strcpy(Manufac," MXIC"); 	break;
	     case 0x04: strcpy(Manufac," Fujitsu"); break;
	     case 0xB0: strcpy(Manufac," SHARP"); 	break;
   	     case 0x40: strcpy(Manufac," SyncMOS"); 	break;
   	     case 0x42: strcpy(Manufac," SyncMOS"); 	break;
	     default: strcpy(Manufac," [----]"); 	break;
      }
      if(Epr.eep == AT89CXXX || Epr.eep == AT89CX051 ) {
          switch(a) {
      	     case 0x1E: strcpy(Manufac," ATMEL"); 	break;
    	     default: strcpy(Manufac," [----]"); 	break;
           }
       }
      frmEprom->edManufacName->Text = Manufac;
      sprintf(ch," 0x%0.2X",a);
      frmEprom->edManufacID->Text = ch;
      sprintf(ch," 0x%0.2X",b);
      frmEprom->edDeviceID->Text = ch;
}

void status_Eprom(void)
{
    frmEprom->edEPRtwc->Text =      Epr.tWC;
    frmEprom->edEPRtwp->Text =      Epr.tWP;
    frmEprom->edEPRerp64->Text =    Epr.epr64;
    frmEprom->edEPRpic->Text =      Epr.PIC;
    frmEprom->edEPRserial->Text =   Epr.serial;
    frmEprom->edEPRwillem->Text =   Epr.willem;
    frmEprom->edEPRdip->Text =      Epr.DIP;
    frmEprom->edEPRswap->Text =     Epr.swap;
    frmEprom->edEPRadStep->Text =   Epr.add_step;
    frmEprom->edEPReep->Text =      Epr.eep;
    frmEprom->edEPRpin28->Text =    Epr.pin28;
    frmEprom->edEPRpin30->Text =    Epr.pin30;
    frmEprom->edEPRpin7->Text =     Epr.pin7;
    frmEprom->edEPRqe->Text =       Epr.qe;
    frmEprom->edEPRat49F00x->Text = Epr.AT49F00x;
    frmEprom->edEPRSsize->Text =    Epr.Ssize;

}


unsigned long __fastcall swap_adr(unsigned long adr)
{
	unsigned long i,j,k,g;
	j = i= adr;
	// A14 --> A15 pin1(DIP28) or pin3(DIP32)
	// A15 --> A18
	j = (j<<1) & 0x8000;
	i = (i<<3) & 0x40000;
	adr = adr & 0xB7FFF;
	adr = adr | j;
	adr = adr | i;
	return adr;
}

void __fastcall error2(int adres,int offset,unsigned char data)
{
     char buffer[100];
     int i;

	sprintf(buffer," Error at 0x%0.5X  Chip = 0x%0.2X  buffer = 0x%0.2X",
		    adres+offset,data,Buffer[adres+offset]);
    frmEprom->edTemp->Text = buffer;
    error(frmEprom->edTemp->Text);
}


