#include "willem2.h"
#include "willem.h"
#include <stdio.h>


//---------------------------------------
//             AT49Fxxx, Am29F0x0
//---------------------------------------
unsigned long  Chip_C_adresF[]   = {0x555,0x2AA,0x555,0x555,0x2AA,0x555};
unsigned char  Chip_disP_Data[]  = {0xAA,0x55,0x80,0xAA,0x55,0x20};
unsigned char  Chip_BootB_Data[] = {0xAA,0x55,0x80,0xAA,0x55,0x40};


// Send program command
void enSoftP_Am29F(void){
	for(int i=0 ;i<3;i++) {
	     r_adresx(Chip_C_adresF[i],2);
	     wr_data(EN_wr[i],1);
	     we_pulse(Epr.tWP);	     	   ms_delay(20);
	}
}

void disSoftP_Am29F(void){
	for(int i=0 ;i<6;i++) {
	     r_adresx(Chip_C_adresF[i],4);
	     wr_data(Chip_disP_Data[i],4);
	     we_pulse(Epr.tWP);	     	   ms_delay(20);
	}
}

void enSoftP_AT49F(void){
	for(int i=0 ;i<3;i++) {
	     r_adresx(Adr_29C[i],1);
	     wr_data(EN_wr[i],1);
	     we_pulse(Epr.tWP);	        ms_delay(20);
	}
}

void __fastcall prog_AT49F(void)
{
	unsigned int i, y;
	unsigned char g,ch;
	int q,twp,twc;
   	int hgrot= grot;
    bool flag_err = false;

	if(st!=0) getoff();
	if(grot>=Epr.BufferSize) grot=Epr.BufferSize-1;
   	frmEprom->SetProcessGauge("programming chip",grot,clRed);

	q = 1;

	twc = Epr.tWC;
	twp = Epr.tWP;

	setup();
	we_hi;
	r_adres_28C(0);
	for(i=0;i<=grot;i++){
		y=i;
		g=Buffer[y];

r:
        if (Epr.eep == M29FXXX)
			enSoftP_Am29F();  // program command
		else
			enSoftP_AT49F();
		we_hi;
		r_adresx(y,1);
		wr_data(g,1);
		we_pulse(twp);   ms_delay(twc);

		ms_delay(2);
		Outport(poort,0x00);
		oe_lo;
		ch = in_word();

		if(g!=ch) {
			if(q==20)  {
				error2(i,st,ch);
				power_down();		//ARW: was powoff
                flag_err = true;
				goto P8E;
			}
			else{
				q++;
				goto r;
			}
		}

       	frmEprom->gProcess->Progress = i;
       	Application->ProcessMessages();
        if(cancel){
          		frmEprom->Message("program interrupted!",1);
                flag_err = true;
          		goto P8E;
       	}
		q=1;
	}
P8E: if(!flag_err)  {
            Epr.Dis_showV = ON;
            verify_28C();
            if(!Epr.flag_err)frmEprom->Message("Device Programed OK",2);
     }
     grot=hgrot;
	 power_down();/*wil_assert;*/ 			//ARW: was powoff
}
//------------------------------------------------------------------------------
int __fastcall MXIC_F29040_Sect_Status(int sectAddr)
{
int sectProtStatus;
unsigned int i;

setup();we_hi;oe_hi;
ms_delay(100);
	for(i=0 ;i<3;i++) // Send first 3 cycles for Protect Verify
                {
	        r_adresx(Chip_C_adresF[i],4);  //Address with usec delay (optional)
                ms_delay(4);
                we_pulse(4);   //WE low ...usec (optional)....WE high
	        wr_data(Get_ID[i],4);
                ms_delay(4);
	        we_pulse(4);   //WE low ...usec (optional)....WE high
                ms_delay(4);
	        }

r_adresx(sectAddr,4);//Address for sector reading
ms_delay(4);
we_pulse(4);   //WE low ...usec (optional)....WE high
ms_delay(4);

sectProtStatus=in_word();  //Read status of sector
power_down();
return sectProtStatus;
}

//------------------------------------------------------------------------------
void __fastcall MXIC_F29040_Sect_Protect(int sectAddr)
{
int i;

setup();we_hi;
ms_delay(100);

for(i=0 ;i<6;i++) // Send first 6 cycles for unlock sector prot/unprotect
        {
        r_adresx(Chip_C_adresF[i],4);  //Address with usec delay (optional)
        ms_delay(4);
        we_pulse(4);   //WE low ...usec (optional)....WE high
        wr_data(Chip_disP_Data[i],4);
        ms_delay(4);
        we_pulse(4);   //WE low ...usec (optional)....WE high
        ms_delay(4);
        }

r_adresx(sectAddr,4); //Address for sector protect + a6=0  a9=1
ms_delay(4);
oe_hi;
we_hi;
ms_delay(5);
we_pulse(4);   //WE low ...usec (optional)....WE high  Sector protect

power_down();
}

//------------------------------------------------------------------------------
void __fastcall MXIC_F29040_Chip_Unprot(void)
{
int i;

setup();we_hi;
ms_delay(100);

for(i=0 ;i<6;i++) // Send first 6 cycles for unlock sector prot/unprotect
        {
        r_adresx(Chip_C_adresF[i],4);  //Address with usec delay (optional)
        ms_delay(4);
        we_pulse(4);   //WE low ...usec (optional)....WE high
        wr_data(Chip_disP_Data[i],4);
        ms_delay(4);
        we_pulse(4);   //WE low ...usec (optional)....WE high
        ms_delay(4);
        }

r_adresx(0x0240,4); //Address for chip unprotect: a6=1  a9=1
ms_delay(4);
oe_hi;
we_hi;
ms_delay(5);
we_pulse(2000); //WE low...not less than 1000us...WE high  Chip Unprotect

power_down();
}

//------------------------------------------------------------------------------


void __fastcall erase_AT49F()
{
      int i,loop = 1000;
      bool flag_err = false;

      frmEprom->pProcess->Visible = true;
      frmEprom->SetProcessGauge("erasing chip",loop,clRed);
      setup(); //Prepare I/O
      we_hi;
      for(i=0;i<6;i++) {
		if(Epr.eep == M29FXXX)  r_adresx(Chip_C_adresF[i]);
		else 	 	            r_adresx(Adr_29C[i]);
		ms_delay(4);
		wr_data(Erase_29C[i]);  ms_delay(4);
		we_pulse(10); 	        ms_delay(20);
      }
      // delay 20 sec.
    for(i=0;i<loop;i++) {
         frmEprom->gProcess->Progress = i;
         Application->ProcessMessages();
	        if(cancel){
          		frmEprom->Message("program interrupted!",1);
                flag_err = true;
          		break;
         }
         ms_delay(20000);
    }

    if(!flag_err) frmEprom->Message("erase completed",2);
    power_down();/*wil_assert;*/        //ARW: was powoff
    idle_mode();   //ARW: duplication
    winstat();
}


//------------------ Am29Fxxx --------------
void __fastcall id_29Fxxx(void)
{           // DW: added void
	unsigned char a,b;
    int i;

	setup();we_hi;
	ms_delay(100);

	for(i=0 ;i<3;i++) {
	     r_adresx(Chip_C_adresF[i],4); 	ms_delay(4);
	     wr_data(Get_ID[i],4); 		ms_delay(4);
	     we_pulse(2);			ms_delay(20);
	}
	ms_delay(1000);
	r_adresx(0x00);
	a = in_word();
	ms_delay(1000);
	for(i=0 ;i<3;i++) {
	     r_adresx(Chip_C_adresF[i],4); 	ms_delay(4);
	     wr_data(Get_ID[i],4); 		ms_delay(4);
	     we_pulse(2); 			ms_delay(20);
	}
	ms_delay(100);
	r_adresx(0x01);
	b = in_word();
	ms_delay(100);    // Reset
	     wr_data(0xf0,4); 			ms_delay(4);
	     we_pulse(5);			ms_delay(100);
	power_down();

	show_ID(a,b);
}

//----------id 49Fxxx----------------
void __fastcall id_49Fxxx(void)
{           // DW: added void
	unsigned char a,b;
	int i;

	setup();we_hi;
	for (i=0;i<3;i++){
		oe_lo;
		r_adres_28C(Adr_29C[i]);
		oe_hi;
		Outport(poort,Get_ID[i]);
		we_lo;ms_delay(2);we_hi;
	}
	ms_delay(10);
	oe_lo;
	r_adres_28C(0x00);
	a = in_word();
	oe_lo;
	r_adres_28C(0x01);
	b = in_word();
	ms_delay(10);

	for (i=0;i<3;i++){
		oe_lo;
		r_adres_28C(Adr_29C[i]);
		oe_hi;
		Outport(poort,Rst_chip[i]);
		we_lo;ms_delay(1);we_hi;
	}
	ms_delay(10000);
	power_down();
	show_ID(a,b);
}
//-------------------- 28Fxxx -----------------------------

void _fastcall id_28Fxxx(void)			// DW: added void
{
	unsigned char a,b;

	setup();
	we_hi;
	r_adres_28C(0);
	vpp_on;ms_delay(200);

	wr_data(0x90,5);	ms_delay(50);
	we_pulse(10);   	ms_delay(15);
	r_adresx(0x00); 	ms_delay(5);
	a = in_word();

	wr_data(0x90,5); 	ms_delay(50);
	we_pulse(10); 		ms_delay(20);
	r_adresx(0x01,5); 	ms_delay(5);
	b = in_word();

	vpp_off;ms_delay(200);
	power_down();
	show_ID(a,b);
}

//---------------------------------------
//              AT29Cxx
//---------------------------------------
void __fastcall id_29Cxxx(void)
{           // DW: added void
	unsigned char a,c;
	int b,i,adr,r,bit;

	setup();we_hi;
    ms_delay(30000);
	for (i=0;i<3;i++){
		oe_lo;
		adr = Adr_29C[i];
		r = Epr.pattern_adr;
		Outport(poort,0x00);
		for (b=(Epr.shift_adr+1); b!=0; b--){
			bit=(adr&r);
			bit ? adrhi:adrlo; asadres;
			clkhi, asadres;
			clklo, asadres;
			r=r>>1;
		}
		oe_hi;
        Outport(poort,Get_ID[i]);
		we_lo;ms_delay(2);we_hi;
	}
	ms_delay(20);
	oe_lo;
	r_adres_28C(0x00);
	a = in_word();
	oe_lo;
	r_adres_28C(0x01);
	c = in_word();
	ms_delay(10);

    for (i=0;i<3;i++){
		oe_lo;
		adr = Adr_29C[i];
		r = Epr.pattern_adr;
		Outport(poort,0x00);
		for (b=(Epr.shift_adr+1); b!=0; b--){
			bit=(adr&r);
			bit ? adrhi:adrlo; asadres;
			clkhi, asadres;
			clklo, asadres;
			r=r>>1;
		}
		oe_hi;
		Outport(poort,Rst_chip[i]);
		we_lo;ms_delay(2);we_hi;
	}
	ms_delay(10000);
   	power_down();
	show_ID(a,c);
}

void __fastcall id_W29EExxx(void)
{           // DW: added void
	unsigned char a,c;
	int b,i,adr,r,bit;

	setup();we_hi;
    ms_delay(30000);
	for (i=0;i<6;i++){
		oe_lo;
		adr = Adr_29C[i];
		r = Epr.pattern_adr;
		Outport(poort,0x00);
		for (b=(Epr.shift_adr+1); b!=0; b--){
			bit=(adr&r);
			bit ? adrhi:adrlo; asadres;
			clkhi, asadres;
			clklo, asadres;
			r=r>>1;
		}
		oe_hi;
        Outport(poort,Get_IDw29E[i]);
		we_lo;ms_delay(2);we_hi;
	}
	ms_delay(10);
	oe_lo;
	r_adres_28C(0x00);
	a = in_word();
	oe_lo;
	r_adres_28C(0x01);
	c = in_word();
	ms_delay(10);

    for (i=0;i<3;i++){
		oe_lo;
		adr = Adr_29C[i];
		r = Epr.pattern_adr;
		Outport(poort,0x00);
		for (b=(Epr.shift_adr+1); b!=0; b--){
			bit=(adr&r);
			bit ? adrhi:adrlo; asadres;
			clkhi, asadres;
			clklo, asadres;
			r=r>>1;
		}
		oe_hi;
		Outport(poort,Rst_chip[i]);
		we_lo;ms_delay(2);we_hi;
	}
	ms_delay(10000);
   	power_down();
	show_ID(a,c);
}

//--------------------------------------
void __fastcall prog_sector(unsigned long i_adres)			// DW: added void
{
	int bit,r,adr;
	unsigned int b,i;


	for (i=0;i<3;i++){
		oe_lo;
		adr = Adr_29C[i];
		r = Epr.pattern_adr;
		Outport(poort,0x00);
		for (b=(Epr.shift_adr+1); b!=0; b--){
			bit=(adr&r);
			bit ? adrhi:adrlo; asadres;
			clkhi, asadres;
			clklo, asadres;
			r=r>>1;
		}
//		adrlo; asadres;
		oe_hi;
		Outport(poort,EN_wr[i]);
		we_lo;ms_delay(2);we_hi;
	}
	for(i=0;i<Epr.Ssize;i++){
		oe_lo;
		adr = i_adres;
		r = Epr.pattern_adr;
		Outport(poort,0x00);
		for (b=(Epr.shift_adr+1); b!=0; b--){
			bit=(adr&r);
			bit ? adrhi:adrlo; asadres;
			clkhi, asadres;
			clklo, asadres;
			r=r>>1;
		}
//		adrlo; asadres;
		oe_hi;
		Outport(poort,Epr.data[i]);
		we_lo;ms_delay(2);we_hi; //ms_delay(2);
		i_adres++;
	}

}

int __fastcall verify_sector(unsigned long i_adres)			// DW: added void
{
	unsigned int i,q,error = OFF;
    unsigned char g;
	q = 0;	oe_lo;

	for(i=0;i<Epr.Ssize;i++){
loop_v:
		r_adres_28C(i_adres);
		g = in_word();
		if(g!=Buffer[i_adres]) {
		     if(q == 4) { error = ON; break; }
		     q++;
		     goto loop_v;
		}
		i_adres++;
	}
	return error;
}

//------------------------------------------------------
void __fastcall prog_29C(void)			// DW: added void
{
	unsigned int i,j,y,e,loop;
	unsigned char g,q,flag_out;
 //	char buffer[100];


	if(grot>=Epr.BufferSize) grot=Epr.BufferSize-1;
	e = grot/Epr.Ssize;
   	frmEprom->SetProcessGauge("programming chip",e,clRed);

	setup();
	e = 0;
	for(i=0;i<=grot;){
		Epr.index_adres = i+st;
		for(j=0; j<Epr.Ssize ; j++) {
			Epr.data[j] = Buffer[i];
			i++;
		 }
		 Epr.flag_err = OFF;
		 for(loop =0; loop < 15;loop++) {
		 //	sprintf(buffer," Sector Size = %d :write sector %4d loop = %2d\r",
		 //		Epr.Ssize,e,loop);
			prog_sector(Epr.index_adres);
			ms_delay(Epr.tWC);
			Epr.flag_err = verify_sector(Epr.index_adres);
			if(Epr.flag_err == OFF) loop = 15;
		 }
	     if(Epr.flag_err == ON)	 goto exit_29C;

	     frmEprom->gProcess->Progress = e;
       	 Application->ProcessMessages();
         if(cancel){
          		frmEprom->Message("program interrupted!",1);
                	Epr.flag_err = true;
          		goto exit_29C;
       	 }
		e++;
	}
exit_29C:
    if(!Epr.flag_err)  {
            Epr.Dis_showV = ON;
            verify_28C();
            if(!Epr.flag_err)frmEprom->Message("Device Programed OK",2);
     }else { error("error");
//		sprintf(buffer," Sector Size = %d :write sector %4d loop = %2d\r",
//	  	Epr.Ssize,e,loop);
	}
	power_down();         // ARW: was powoff
}

void __fastcall erase_AT29Cxxx(void)
{
      unsigned int i,j,loop = 100;
      bool flag_err = false;

      frmEprom->pProcess->Visible = true;
      frmEprom->SetProcessGauge("erasing chip",loop,clRed);
      setup();
      we_hi;
      for(j=0;j<6;j++) {
	    oe_lo;
	    r_adres_28C(BB_29C_Ad_h[j]);
	    oe_hi;Outport(poort,Erase_29C[j]);
	    we_lo;ms_delay(2);we_hi;
      }
     // delay 4 sec.
     for(i=0;i<loop;i++) {
         frmEprom->gProcess->Progress = i;
         Application->ProcessMessages();
	        if(cancel){
          		frmEprom->Message("program interrupted!",1);
                flag_err = true;
          		break;
         }
         ms_delay(20000);
    }

    if(!flag_err) frmEprom->Message("erase completed",2);
    power_down();/*wil_assert;*/        //ARW: was powoff
    idle_mode();   //ARW: duplication
    winstat();
}


//--------------------------------
//            MCS-51
//--------------------------------
void __fastcall mcs51::initl_code(void)
{
	unsigned int P26,P27,P34,P33,P35,P36,P37;

	P26 = 0x08000;
	P27 = 0x10000;
	P36 = 0x20000;
	P37 = 0x40000;
	mode_wr   =  P27|P36|P37;    // 0111
	mode_rd   =  P36|P37;	     // 0011
	mode_erase = P26;            // 1000
	mode_sig  =  0x00;           // 0000
	mode_bit1 =  P26|P27|P36|P37;// 1111
	mode_bit2 =  P26|P27;        // 1100
	mode_bit3 =  P26|P36;        // 1010
	mode_Lbit =  P26|P27|P36;    // 1110

	//    AT89Cx051
	//    DIP 20 pin

	P33 = 0x00040;
	P34 = 0x00100;
	P35 = 0x00080;
	P37 = 0x00200;

	Prog = 0x40000;
	Rst  = 0x20000;
	Clk  = 0x18000;
	Pwrite = P34|P35|P37;
	Pread  = P35|P37;
	Plb1   = P33|P34|P35|P37;
	Plb2   = P33|P34;
	Perase = P33;
	//----------------

	at89s 	 = 0;
	tWP 	 = 10;
	tWC 	 = 2100;
	Vpp 	 = 0xff;
	Pulses   = 1;
	type 	 = DIP40;
	tErase   = 0;
	tWPb 	 = 10;
	tWPe 	 = 25000;
	ird_lbit = OFF;
}

//--------------- AT89Cx051 -------------
void __fastcall mcs51::inc_adres(void){
     r_adresx(Pwrite|Clk); ms_delay(10);
     we_pulse(10);     	   ms_delay(10);
}

unsigned char __fastcall mcs51::Dout(unsigned char data){

       unsigned ch = 0x00;     // arrange D0-D7 Data to AT89Cx051
       ch |= ((data >> 4) & 0x08);   // D7 -> D3
       ch |= ((data >> 4) & 0x04);   // D6 -> D2
       ch |= ((data >> 1) & 0x10);   // D5 -> D4
       ch |= ((data >> 3) & 0x02);   // D4 -> D1
       ch |= ((data << 2) & 0x20);   // D3 -> D5
       ch |= ((data >> 2) & 0x01);   // D2 -> D0
       ch |= ((data << 5) & 0x40);   // D1 -> D6
       ch |= ((data << 7) & 0x80);   // D0 -> D7
       return(ch);
}

unsigned char __fastcall mcs51::Din(unsigned char data){

       unsigned char ch2 = 0x00;   // arrange D0-D7 from AT89Cx051
       ch2 |= ((data << 4) & 0x80);   // D7 <- D3
       ch2 |= ((data << 4) & 0x40);   // D6 <- D2
       ch2 |= ((data << 1) & 0x20);   // D5 <- D4
       ch2 |= ((data << 3) & 0x10);   // D4 <- D1
       ch2 |= ((data >> 2) & 0x08);   // D3 <- D5
       ch2 |= ((data << 2) & 0x04);   // D2 <- D0
       ch2 |= ((data >> 5) & 0x02);   // D1 <- D6
       ch2 |= ((data >> 7) & 0x01);   // D0 <- D7
       return(ch2);
}

void __fastcall mcs51::setup2051(void){

     initl_code();
     type = DIP20;
     setup();we_hi;
     r_adresx(Rst); ms_delay(20);
     we_lo;
     ms_delay(30000);  // RST,XTAL1 = LOW
     ms_delay(30000);  // delay 60 mS
     we_hi;
     ms_delay(20);
}
// ----- end sub. AT89Cx051 -----


// ----- AT89Cxx,AT89Sxx,AT89Cx051 -----

void __fastcall mcs51::write_code(unsigned long address,unsigned char data)
{
	int loop;

	we_hi;
	if(Vpp==0xff){	ppon;wil_assert; }
	switch(type) {
		case DIP40: r_adresx(address|mode_wr);	ms_delay(5);
			    wr_data(data,5);		ms_delay(5);
			    break;
		case DIP20: r_adresx(Pwrite|Prog);	ms_delay(10);
			    wr_data(Dout(data),10); 	ms_delay(15);
			    break;
	}

	for(loop = Pulses; loop > 0; loop--) {
		we_pulse(tWP);
		ms_delay(20);
	}
	ppoff;wil_assert; adrlo;clklo;clalo;asadres;
	ms_delay(tWC);
}

void __fastcall mcs51::lockbit(unsigned long bit)
{
	we_hi;
	if(Vpp==0xff){	ppon;wil_assert; }
	switch(type) {
		case DIP40: r_adresx(bit);	ms_delay(15);
			    wr_data(d);		ms_delay(15);
			    break;
		case DIP20: r_adresx(bit|Prog); ms_delay(15);
			    break;
	}
	we_pulse(tWPb);
	ms_delay(20);
	ppoff;wil_assert;adrlo;clklo;clalo;asadres;
	ms_delay(tWC);
}

void __fastcall mcs51::write_mode_LB(int mode)
{
	switch(type) {
		case DIP40:if(at89s == 0) {
				d = read_code(0);
				switch (mode) {  //AT89Cxx
					case 1: lockbit(mode_bit1); break;
					case 2: lockbit(mode_bit1);
						lockbit(mode_bit2); break;
					case 3: lockbit(mode_bit1);
						lockbit(mode_bit2);
						lockbit(mode_bit3); break;
				}
			}
			else { switch (mode) {  // AT89Sxx
					case 1: d = 0x7F;lockbit(mode_bit3); break;
					case 2: d = 0x3F;lockbit(mode_bit3); break;
					case 3: d = 0x1F;lockbit(mode_bit3); break;
				}
			}break;
		case DIP20: switch(mode) {  //AT89Cx051
				case 1: lockbit(Plb1); break;
				case 2: lockbit(Plb1);
					lockbit(Plb2); break;
			}break;
	}
}

unsigned char __fastcall mcs51::read_code(unsigned long address)
{
	unsigned char ch;

	read;pgrhi;ppoff;wil_assert;
	ms_delay(10);
	switch(type) {
		case DIP40:if(at89s == 1){
				vpp_on;	ms_delay(30);
			}
			r_adres_28C(address|mode_rd); ms_delay(20);
			vpp_off;
			ch = in_word();
			break;

		case DIP20:r_adres_28C(Pread);ms_delay(20);
			   ch = Din(in_word()) ;
			   break;
	}
	return(ch);
}

void __fastcall mcs51::erase_chip(void)
{
	switch (type) {
		case DIP40: r_adresx(mode_erase);
			    break;
		case DIP20: r_adresx(Perase|Prog);
			    break;
       }
       oe_hi;  ms_delay(20);
       if(Vpp == 0xff) {vpp_on;}
       ms_delay(200);
       we_pulse(tWPe);            // 25 mS or 2uS
       ms_delay(30000);

       ms_delay(1000); vpp_off;
       ms_delay(20000);
}

void __fastcall mcs51::read_sign(void)
{
	pgrhi;read;wil_assert;
	ms_delay(100);
	switch(type) {
		case DIP40:r_adres_28C(0x30);ms_delay(10);sig30=in_word();
			if(sig30 == 0xFF) {
				vpp_on; ms_delay(20);
				r_adres_28C(0x30);ms_delay(10);sig30=in_word();
			}
			r_adres_28C(0x31);ms_delay(10);sig31=in_word();
			r_adres_28C(0x32);ms_delay(10);sig32=in_word();

			if (sig31 == 0xFF) { // ID adress 000,100,200
				r_adres_28C(0x100);ms_delay(10);sig31=in_word();
				r_adres_28C(0x200);ms_delay(10);sig32=in_word();
			}
			break;
		case DIP20:
			r_adres_28C(0x00);ms_delay(20);sig30=Din(in_word());
			inc_adres();
			r_adres_28C(0x00);ms_delay(10);sig31=Din(in_word());
			inc_adres();
			r_adres_28C(0x00);ms_delay(10);sig32=Din(in_word());
			break;
	}
	///////----------- Vpp on/off------////
	Vpp = 0xff;

	ms_delay(20);
	vpp_off;
	ms_delay(100);
}

void __fastcall mcs51::read_lockbit(void)
{
	pgrhi;wil_assert;
	ms_delay(100);

	if(at89s == ON) {r_adresx(mode_bit2); ms_delay(10);}
	else 		{r_adresx(mode_Lbit); ms_delay(10);}

	sig_Lbit = in_word();
	ms_delay(100);
}

//------------------ MCS-51 --------------

void __fastcall verify_AT89C(void)
{		//DW: added void
	unsigned int i;
	unsigned char g,q;

	if(st!=0)getoff();
	if(grot>=Epr.BufferSize) grot=Epr.BufferSize-1;
   	frmEprom->SetProcessGauge("Verify chip",grot,clGreen);

	if(Epr.eep == AT89CX051) AT89Cxx.setup2051();		// AT89Cx051
	else setup();

	q = 1;
    Epr.flag_err = OFF;
	for(i=0;i<=grot;i++){

loop_comp:
		g = AT89Cxx.read_code(i);
		if( g!= Buffer[i]){		// DW: Made this XMS-aware
		      if(q == 10){
			        error2(i,st,g);
                    Epr.flag_err = ON;
        			break;
		       }
		       q++; goto loop_comp;
		}
		if(Epr.eep == AT89CX051) AT89Cxx.inc_adres();	// AT89Cx051

	       	frmEprom->gProcess->Progress = i;
       		Application->ProcessMessages();
        	if(cancel){
        		frmEprom->Message("program interrupted!",1);
                Epr.flag_err = ON;
          		break;
       		}
		q = 1;
	}
exit_verify:
    if(!Epr.flag_err) frmEprom->Message("Device Verify OK",2);
	power_down();/*wil_assert;*/          //ARW: was powoff
}


void __fastcall prog_AT89C(void)
{
	unsigned int y,i;
	unsigned char g,at;
	int flag_err,q=1;

	if(st!=0) getoff();
	if(grot>=Epr.BufferSize) grot=Epr.BufferSize-1;
   	frmEprom->SetProcessGauge("programming chip",grot,clRed);

	if(Epr.eep == AT89CX051) AT89Cxx.setup2051();		// AT89Cx051
	else setup();

	Epr.flag_err = OFF;
	for(i=0;i<=grot;i++){
		g= Buffer[i];
w89:
		AT89Cxx.write_code(i+st,g);
		at = AT89Cxx.read_code(i+st);
		if(at!=g){
		     if(q>10){
			    error2(i,0,at);
			    Epr.flag_err = true;
			    goto P89E;
			}
			else {
			q++;
			AT89Cxx.tWC += 100;
			goto w89; }
		}
		if(Epr.eep == AT89CX051) AT89Cxx.inc_adres();	// AT89Cx051

       		frmEprom->gProcess->Progress = i;
       		Application->ProcessMessages();
        	if(cancel){
          		frmEprom->Message("Program interrupted!",1);
                	Epr.flag_err = true;
          		goto P89E;
       		}
		q=1;
	}
P89E:
	if(!Epr.flag_err) verify_AT89C();
    if(!Epr.flag_err) {
        if(AT89Cxx.ModeLock > 0) {
       	     power_down();
             ms_delay(30000);
             MCS51_lockbit();
             frmEprom->Message("Device Programmed & Lock bit",2);
        }else frmEprom->Message("Device Programmed OK",2);
    }
	power_down();/*wil_assert;*/        //ARW: was powoff
}

void __fastcall erase_AT89C(void)
{           // DW: added void
      int loop = 10;
      frmEprom->pProcess->Visible = true;
      frmEprom->SetProcessGauge("erasing chip",loop,clRed);

      setup();we_hi;
      AT89Cxx.erase_chip();
      power_down();
      ms_delay(25000);
      if (AT89Cxx.tErase == 1) {
		setup();
		AT89Cxx.erase_chip();
		power_down();
		ms_delay(25000);
		setup();
		AT89Cxx.erase_chip();
		power_down();
      }
}

void __fastcall CRead_AT89C(void)
{		//DW: added void

	unsigned int i;

	ClearBuf();		// DW: Made this XMS-aware	memset(rombuf,0,aantal);
	if(st!=0) getoff();
   	frmEprom->SetProcessGauge("READ chip",Epr.BufferSize,clGreen);

	if(Epr.eep==AT89CX051) AT89Cxx.setup2051();   // AT89Cx051
	else setup();
    Epr.flag_err = OFF;
	for(i=0;i<Epr.BufferSize;i++){

		Buffer[i]=AT89Cxx.read_code(i);
		if(Epr.eep == AT89CX051)  AT89Cxx.inc_adres();     	// AT89Cx051

   		frmEprom->gProcess->Progress = i;
   		Application->ProcessMessages();
       	if(cancel){
       		frmEprom->Message("program interrupted!",1);
       		Epr.flag_err = ON;
            break;
   	 	}
	}
    if(!Epr.flag_err)frmEprom->Message("Device Read OK",2);
	grot= Epr.BufferSize;
	power_down();
}

void __fastcall id_AT89C(void)
{           // DW: added void
      setup();we_hi
      AT89Cxx.read_sign();
      power_down();
	  show_ID(AT89Cxx.sig30,AT89Cxx.sig31);
}

void __fastcall MCS51_lockbit(void)
{
	if(AT89Cxx.ModeLock > 0) {
		if(Epr.eep == AT89CX051) AT89Cxx.setup2051();
		else setup();

        we_hi;
		AT89Cxx.write_mode_LB(AT89Cxx.ModeLock);
		ms_delay(30000);
        frmEprom->Message("Device LockBit ",2);
		power_down();
	}
}

int __fastcall empty_AT89C(void)
{		//DW: added void
    int Answer=-1;
	unsigned long i;
	unsigned char h;

  	if(st!=0) getoff();
    frmEprom->SetProcessGauge("Checking Device is Empty",Epr.BufferSize,clWhite);

	if(Epr.eep==AT89CX051) AT89Cxx.setup2051();
	else setup();

	for(i=0;i<Epr.BufferSize;i++){
rep_check:
		h = AT89Cxx.read_code(i+st);
		if(h!=0xff){
            Answer = i+st;
            break;
		}
		if(Epr.eep == AT89CX051) AT89Cxx.inc_adres();     // AT89Cx051
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

void __fastcall rdLbit_AT89C(void)
{
      int b;
      char buf[50];

      if (AT89Cxx.ird_lbit == ON) {
	    setup();
	    AT89Cxx.read_lockbit();
	    power_down();
	    b = AT89Cxx.sig_Lbit;
	    if(AT89Cxx.at89s == OFF)
		  sprintf(buf," Bit 1,2,3 (D2,3,4) = %X%X%X",
		       (b>>2 & 0x01),(b>>3 & 0x01),(b>>4 & 0x01) );
	    else
		  sprintf(buf,"  Bit 1,2,3 (D2,1,0) = %X%X%X",
		       (b>>2 & 0x01),(b>>1 & 0x01),(b & 0x01) );
       }else {
	      sprintf(buf," Chip can not read Lock bit..");
       }
      frmEprom->edRdLB->Text = buf;
}

void status_8051(void){

    frmEprom->ed51at89S->Text = AT89Cxx.at89s;
    frmEprom->ed51tWP->Text = AT89Cxx.tWP;
    frmEprom->ed51tWC->Text = AT89Cxx.tWC;
    frmEprom->ed51Pulses->Text = AT89Cxx.Pulses;
    frmEprom->ed51type->Text = AT89Cxx.type;
    frmEprom->ed51tWPb->Text = AT89Cxx.tWPb;
    frmEprom->ed51tWPe->Text = AT89Cxx.tWPe;
    frmEprom->ed51ird_lbit->Text = AT89Cxx.ird_lbit;
    frmEprom->ed51sig_Lbit->Text = AT89Cxx.sig_Lbit;
    frmEprom->ed51tErase->Text = AT89Cxx.tErase;
    frmEprom->ed51LockBit->Text = AT89Cxx.ModeLock;
}
//------------------------------------------
//           AT49F00x
//------------------------------------------
void __fastcall prog_AT49F00x(void)
{
	unsigned int i, y;
	unsigned char g,ch;
	int q,twp,twc;
   	int hgrot= grot;
    bool flag_err = false;

	if(st!=0) getoff();
	if(grot>=Epr.BufferSize) grot=Epr.BufferSize-1;
   	frmEprom->SetProcessGauge("programming chip",grot,clRed);

	q = 1;

	twc = Epr.tWC;
	twp = Epr.tWP;

	setup();	we_hi;
    if(Epr.AT49F00x) { vpp_on; ms_delay(100); }
    else { vpp_off; ms_delay(100); }

	r_adres_28C(0);
	for(i=0;i<=grot;i++){
		y=i;
		g=Buffer[y];

r:
   		enSoftP_AT49F();
		we_hi;
		r_adresx(y,1);
		wr_data(g,1);
		we_pulse(twp);   ms_delay(twc);

		ms_delay(2);
		Outport(poort,0x00);
		oe_lo;
		ch = in_word();

		if(g!=ch) {
			if(q==20)  {
				error2(i,st,ch);
				power_down();		//ARW: was powoff
                flag_err = true;
				goto P8E;
			}
			else{
				q++;
				goto r;
			}
		}

       	frmEprom->gProcess->Progress = i;
       	Application->ProcessMessages();
        if(cancel){
          		frmEprom->Message("program interrupted!",1);
                flag_err = true;
          		goto P8E;
       	}
		q=1;
	}
P8E:
    vpp_off; ms_delay(100);
    if(!flag_err)  {
            Epr.Dis_showV = ON;
            verify_28C();
            if(!Epr.flag_err)frmEprom->Message("Device Programed OK",2);
     }
     grot=hgrot;
	 power_down();/*wil_assert;*/ 			//ARW: was powoff
}


