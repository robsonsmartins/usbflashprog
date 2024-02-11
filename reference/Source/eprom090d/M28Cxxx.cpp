#include "willem2.h"
#include "willem.h"
/*----------------------
void prog_M28C(void)
void prog_M27XXX(void)
void verify_93(void)
void prog_93(void)
void prog_SRAM(void)
void prog_DS12xx(void)


*///---------------------
void __fastcall prog_M28C(void)
{
	unsigned int i, y;
	unsigned char g,ch;
	int q,twp,twc; //,flag_err;
   	int hgrot= grot;
   	bool OK = true;

	if(st!=0) getoff();
	if(grot>=Epr.BufferSize) grot=Epr.BufferSize-1;
      	frmEprom->SetProcessGauge("programming chip",grot,clRed);

	q = 1;

	twc = Epr.tWC;
	twp = Epr.tWP;
//	flag_err = OFF;

	setup();
	we_hi;
	r_adres_28C(0);
	for(i=0;i<=grot;i++){
		y=i;
		g=Buffer[y];
r:
		if (Epr.pin30 == ON) y = y | 0x20000;
		r_adresx(y);
		we_hi;
		r_adresx(y,2);
		wr_data(g,2);
		we_pulse(twp);   ms_delay(twc);

		ms_delay(2);
		Outport(poort,0x00);
		oe_lo;
		ch = in_word();

		if(g!=ch) {
			if(q==20)  {
				error2(i,st,ch);
				power_down();		//ARW: was powoff
				goto P8E;
			}
			else{
				q++;
				twc += 1000;
				if(twc > 15000) twc = 15000;
				goto r;
			}
		}

        	frmEprom->gProcess->Progress = i;
        	Application->ProcessMessages();
	        if(cancel){
          		frmEprom->Message("program interrupted!",1);
          		break;
        	}
		q=1;
	}

    	if(!cancel){
      		frmEprom->SetProcessGauge("verifying chip",grot,clGreen);
      		for(i=0;i<=grot;i++) {
          		y=i;

          		r_adres_28C(i+st);
          		if(in_word()!=Buffer[y]){
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
}

//----------------------------------
void __fastcall prog_M27XXX(void)
{

	unsigned int i, y;
	unsigned char g,ch;
	int q,twp,twc;

   	int hgrot= grot;

	if(st!=0) getoff();
	if(grot>=Epr.BufferSize) grot=Epr.BufferSize-1;
      	frmEprom->SetProcessGauge("programming chip",grot,clRed);

	twp = Epr.tWP;
	setup(); we_hi;
	Epr.flag_err = false;

	vpp_on;
	for(i=0;i<=grot;i++){
		y=i;

		g=Buffer[y];
		if(g==0xff)goto SKIP;	// DW: Made this XMS-aware
        if(Epr.BufferSize > 0x800) {
rep_27:
        	we_hi;oe_lo;ms_delay(10);
	    	vpp_on;ms_delay(100);  	r_adres_28C(i+st);
	    	wr_data(g,5);		    ms_delay(5);
		    we_pulse(twp);		    ms_delay(Epr.tWC);

    		Outport(poort,0x00);    ms_delay(10);

	    	if(!Epr.Vpp) { vpp_off; ms_delay(180); } // Epr.epr64 = 0
		    oe_lo; 		    if(!Epr.Vpp) { ce_lo;}
    		ms_delay(7);   	ch = in_word();
    		if(g!=ch){
	    		if(q==25){error2(i,st,ch);Epr.flag_err = true; goto PE;	}
		    	else{q++; twp += Epr.add_step; goto rep_27;	}
    		}
         }

        else {  // subrotine 2716
rep_2716:
		    we_lo;oe_lo;ms_delay(5);
		    r_adres_28C(i);
		    vpp_on;ms_delay(100);
			wr_data(g);     	   ms_delay(10);
			we_hi;ms_delay(twp);   ms_delay(twp);we_lo;
		    ms_delay(20);

    		Outport(poort,0x00);   ms_delay(10);

	    	vpp_off;ms_delay(200); oe_lo;ms_delay(20);
    		ch = in_word();
	    	if(g!=ch){
            	if(q==25){ error2(i,st,ch);	Epr.flag_err = ON;	goto PE;}
			    else{ q++; goto rep_2716; }
    		}
          } // End else 2716
SKIP:
		twp = Epr.tWP;
       	frmEprom->gProcess->Progress = i;
       	Application->ProcessMessages();
        if(cancel){
          		frmEprom->Message("program interrupted!",1);
	            Epr.flag_err = true;
          		goto PE;
        	}
		q=1;
	}
PE:	vpp_off;		//DW: added. Used to leave program power on!

    if(!Epr.flag_err)  {
            Epr.Dis_showV = ON;
            verify_28C();
            if(!Epr.flag_err)frmEprom->Message("Device Programed OK",2);
     }
     grot=hgrot;
PE2: power_down();
}


//--------------------------------
//           Microwire
//--------------------------------
void m93c46::init_code(void) {
	  // 8bit
     rd    = 0x0300;	// 0011 0aaa aaaa
     wr    = 0x0280;	// 0010 1aaa aaaa
     ewen  = 0x0260;	// 0010 011x xxxx
     ewds  = 0x0200;	// 0010 000x xxxx
     erase = 0x0380;	// 0011 1aaa aaaa
     eral  = 0x0240;	// 0010 010x xxxx
     wral  = 0x0220;	// 0010 001x xxxx
     count_bit = 10;
     adres =  7;
     data  =  8;
     bit16 =  1;
}

void m93c46::shift_L(int shift) {
	  // 8bit
     if(shift>0) {
	     rd    = rd   <<shift;	// 0011 0aaa aaaa
	     wr    = wr   <<shift;	// 0010 1aaa aaaa
	     ewen  = ewen <<shift;	// 0010 011x xxxx
	     ewds  = ewds <<shift;	// 0010 000x xxxx
	     erase = erase<<shift;	// 0011 1aaa aaaa
	     eral  = eral <<shift;	// 0010 010x xxxx
	     wral  = wral <<shift;	// 0010 001x xxxx
	     count_bit = 10+shift;
	     adres =  7+shift;
      }
}

void m93c46::shift_R(int shift) {
	  // 8bit
     if(shift>0) {
	     rd    = rd   >>shift;	// 0011 0aaa aaaa
	     wr    = wr   >>shift;	// 0010 1aaa aaaa
	     ewen  = ewen >>shift;	// 0010 011x xxxx
	     ewds  = ewds >>shift;	// 0010 000x xxxx
	     erase = erase>>shift;	// 0011 1aaa aaaa
	     eral  = eral >>shift;	// 0010 010x xxxx
	     wral  = wral >>shift;	// 0010 001x xxxx
	     count_bit = 10-shift;
	     adres =  7-shift;
     }
}

int m93c46::mclock_in(void)			// DW: added void
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

int m93c46::min_word(void)			// DW: added void
{
	int b, w=0;
    adrlo;asadres;
	for (b=7;b>=0;b-- )
	w =w |( mclock_in()<<b);
	  adrlo;asadres;
	return w;
}

void m93c46::mclock_out(int bit)			// DW: ANSIfied
{
	 bit? outlo:outhi;asadres;
	 ms_delay(2);
	 adrhi;asadres;
	 ms_delay(2);
	 outhi;asadres;
	  ms_delay(2);
	 adrlo; asadres;

}
void m93c46::mout_word(unsigned char w)			// DW: added void
{
	 int b, di;

	 for ( b=7; b>=0;--b )
	 {di=(w>>b) & 0x01;
		 mclock_out(di);}
}

void m93c46::send_instruction(int instr){

	   int n, di;
	   for(n=m93Cxx.count_bit-1;n>=0; n--){
		   di=(instr>>n) & 0x01;
		   mclock_out(di);
	   }
}
//------------ end class m93C -------------------
void __fastcall verify_93(void){

	unsigned int i,instr;
	unsigned char g;

	oe_hi;

	if(grot>=Epr.BufferSize) grot=Epr.BufferSize-1;
    frmEprom->SetProcessGauge("Comparing chip",grot-1,clGreen);

	outhi,adrlo;pgrlo;asadres;wil_assert;
	powon;wil_assert;
	ms_delay(30000);

    Epr.flag_err = OFF;
	for(i=0;i<=grot;i++){

		cs_hi;
		ms_delay(1);
		instr= m93Cxx.rd | (i/m93Cxx.bit16);
		m93Cxx.send_instruction(instr);
		ms_delay(10);

		g=m93Cxx.min_word();
		if( g!= Buffer[i]){
			 error2(i,0,g);
			 goto EXIT_93C;
		}
		if(m93Cxx.bit16 == 2){
			i++;
			g=m93Cxx.min_word();
			if( g!= Buffer[i] ){
				error2(i,0,g);
                Epr.flag_err = ON;
				goto EXIT_93C;
			}
		}
		cs_lo;
       	frmEprom->gProcess->Progress = i;
       	Application->ProcessMessages();
        if(cancel){
       		frmEprom->Message("program interrupted!",1);
            Epr.flag_err = ON;
       		goto EXIT_93C;
       	}
	}

EXIT_93C:
	cs_lo;

     if(!Epr.flag_err)frmEprom->Message("Device Verify OK",2);

	power_down();
	outhi;asadres;
	//powoff,
	read;wil_assert; 		//ARW: duplication but left for clarity
}


void __fastcall read_93(void){

	unsigned int i;
    //a=0,
    unsigned int instr;
	unsigned char g;

    ClearBuf();
	write;wil_assert;
    frmEprom->SetProcessGauge("Reading Chip", Epr.BufferSize,clBlack);

	outhi,adrlo;pgrlo;asadres;wil_assert;
	powon;wil_assert;
	ms_delay(20000);

    Epr.flag_err = OFF;
	for(i=0;i<Epr.BufferSize;i++){

		cs_hi;
		ms_delay(1);
		instr= m93Cxx.rd | (i/m93Cxx.bit16);
		m93Cxx.send_instruction(instr);

			ms_delay(10);

		g=m93Cxx.min_word();
		Buffer[i]=g;
		if(m93Cxx.bit16 == 2){i++;
			g=m93Cxx.min_word();
			Buffer[i]=g;
        }
		cs_lo;
       	frmEprom->gProcess->Progress = i;
       	Application->ProcessMessages();
	    if(cancel) {
            Epr.flag_err = ON;
            goto exit_C93;
        }
	}
exit_C93:
   	cs_lo;
    power_down();
   	outhi;asadres;
	//powoff,
   	read;wil_assert;
    if(!cancel){
         frmEprom->Message("Chip dumped OK",0);
         frmEprom->Caption = "Willem Eprom Programmer - " + frmEprom->edDevice->Caption + " Image";
          grot = Epr.BufferSize;
     }
     else frmEprom->Message("Process Cancelled",1);
     winstat();
}

void __fastcall erase_93(void)  // erases all bytes
{
   int instr;

	write;wil_assert;
    frmEprom->Message("Erase Chip 93Cxx ",0);
    powon;wil_assert;
    ms_delay(30000);

   	outhi,adrlo;pgrlo;asadres;wil_assert;
	powon;wil_assert;

	ms_delay(30000);
	cs_hi;
	m93Cxx.send_instruction(m93Cxx.ewen);       //0001 0011
	cs_lo;

    ms_delay(3000);
    cs_hi;
    m93Cxx.send_instruction(m93Cxx.eral);        //0001 0010
    we_pulse(5);
    ms_delay(30000);
    cs_lo;
    power_down();
}

void __fastcall erase_93C(void)  // erases all bytes
{
    int instr;int wt=0;

    //  powon;wil_assert;
//  ms_delay(30000);

    we_hi;
    m93Cxx.send_instruction(m93Cxx.erase);           //0001 0010
    we_pulse(5);

    while(!einbit){
        ms_delay(1000);
        wt++;
        if(wt==50)
        break;
    }
    cs_lo;
}

void __fastcall prog_93(void){			// DW: added void

	unsigned int i;
//	int wt=0;
	int g,k;


	write;wil_assert;

	if(grot>Epr.BufferSize) grot=Epr.BufferSize;
    frmEprom->SetProcessGauge("programming chip",grot,clRed);

	outhi,adrlo;pgrlo;asadres;wil_assert;
	powon;wil_assert;

	ms_delay(30000);
	cs_hi;
	m93Cxx.send_instruction(m93Cxx.ewen);       //0001 0011
	cs_lo;
	erase_93C();
	ms_delay(3000);
	cs_hi;
	m93Cxx.send_instruction(m93Cxx.ewen);
	cs_lo;
	ms_delay(3000);

	for(i=0;i<grot;i++){
		g= Buffer[i];
		cs_hi;
		m93Cxx.send_instruction(m93Cxx.wr|(i/m93Cxx.bit16) );   // 0001 0100
		m93Cxx.mout_word(g);
		if(m93Cxx.bit16 == 2){i++;
			g= Buffer[i];
			m93Cxx.mout_word(g); }

		we_pulse(5);
		ms_delay(12000);
		cs_lo;

        frmEprom->gProcess->Progress = i;
	}
S:
	m93Cxx.send_instruction(m93Cxx.ewds);
	we_lo;
	power_down();
	oe_hi;

    frmEprom->Message("Device Programmed OK",2);
    winstat();

}

//---------------------------------------
//        Test SRAM
//---------------------------------------
void __fastcall prog_SRAM(void){		//DW: added void

	unsigned int i;
	unsigned char g,q,ch;

	g =0x55;
    frmEprom->SetProcessGauge(" Test#1 SRAM chip",Epr.BufferSize,clRed);
	setup();
	for(i=0;i<Epr.BufferSize;i++){
//		y=i;
		g = ~g;
		Buffer[i]=g;
	}
	grot = Epr.BufferSize-1;
	tRAM2();
//------------------------------------
    if(Epr.flag_err == ON) goto exit;

//	g = 55;
    frmEprom->SetProcessGauge(" Test#2 SRAM chip",Epr.BufferSize,clRed);
	setup();
	for(i=0;i<Epr.BufferSize;i++){
//		y=i;
//      randomize();
		g = rand();
		Buffer[i]=g;
	}
	grot = Epr.BufferSize-1;
	tRAM2();

exit:
    if(!Epr.flag_err)frmEprom->Message("Device Test OK",2);
    power_down();
}

void __fastcall tRAM2(){
	unsigned int i, y;
	unsigned int index,index2;
	unsigned char g,q,ch,flag_err;

	Epr.flag_err = OFF;
	for(i=0;i<grot;i++){
		y =i; index = y;
		// if (Epr.swap == ON) shift A14-->A15-->A18

		if (Epr.swap == ON)  index = swap_adr(index);
		if (Epr.pin28 == ON) index = index | 0x2000;
		if (Epr.pin30 == ON) index = index | 0x20000;

rep_28C:
		g=Buffer[i];
		oe_lo;
		r_adres_28C(index);
		oe_hi;
		Outport(poort,g);
		we_lo; ms_delay(2); we_hi; ms_delay(7);
		Outport(poort,0x00);
		oe_lo;
		ch = in_word();
		if(g!=ch){
			if(q==5){
				error2(i,0,ch);
				Epr.flag_err = ON;
				goto exit_28C;
			}else {
			     q++;
			     goto rep_28C;
			}
		}

        frmEprom->gProcess->Progress = i;
        Application->ProcessMessages();
		q = 1;
	}

//--------------- Verify SRAM --------------
	we_hi;oe_lo;
	if(!Epr.flag_err) {
    frmEprom->SetProcessGauge(" Verify SRAM chip",Epr.BufferSize,clGreen);
		for(i=0;i<grot;i++){
			y=i; index = y;
			// if (Epr.swap == ON) shift A14-->A15-->A18
			if (Epr.swap == ON) index =   swap_adr(index);
			if (Epr.pin28 == ON) index = index | 0x2000;
			if (Epr.pin30 == ON) index = index | 0x20000;
loop_comp:
			r_adres_28C(index);
			g = in_word();
			if( g!=Buffer[i]){		// DW: Made this XMS-aware
			      if(q == 10) {
                        error2(i,0,g);
                        Epr.flag_err = ON;
                        goto exit_28C;
                  }
                  else { q++; goto loop_comp; }
			}
        	frmEprom->gProcess->Progress = i;
        	Application->ProcessMessages();
	        if(cancel){
          		frmEprom->Message("program interrupted!",1);
                Epr.flag_err = ON;
          		goto exit_28C;
        	}
			q = 1;
		}
	  }
exit_28C:
 //	  power_down();/*wil_assert;*/          //ARW: was powoff
}

//---------------------------------
//
//----------------------------------
void __fastcall prog_DS12(void)			// DW: added void
{
    unsigned int i,y,index;
	int tWC;
	unsigned char g,q,ch,flag_err;


	if(st!=0) getoff();

	if(grot>=Epr.BufferSize) grot=Epr.BufferSize-1;
   	frmEprom->SetProcessGauge("programming chip",grot,clRed);

	setup();we_hi;
	q = 1;

	tWC = Epr.tWC;
	Epr.flag_err = OFF;
	for(i=0;i<=grot;i++){
		y =i; index = y+st;
		// A14 -->A15, A15-->A18
		index = swap_adr(index);
		g= Buffer[i];
rep_DS12:
		we_hi;
		r_adresx(index);	ms_delay(1);
		wr_data(g); 		ms_delay(1);
		we_pulse(Epr.tWP);	ms_delay(tWC);

                ms_delay(2);
		Outport(poort,0x00);
		oe_lo;
		ch = in_word();
		if(g!=ch){
			if(q==10){
				error2(i,st,ch);
				Epr.flag_err = ON;
				goto exit_DS12;
			}
			else {
				q++;
				tWC = tWC+5;
				if(tWC > 100) tWC = 100;
				goto rep_DS12;
			}
		}

       	frmEprom->gProcess->Progress = i;
       	Application->ProcessMessages();
        if(cancel){
          		frmEprom->Message("program interrupted!",1);
                Epr.flag_err = true;
          		break;;
       	}
		q = 1;
		tWC = Epr.tWC;
	}
exit_DS12:
    if(!Epr.flag_err)  {
            Epr.Dis_showV = ON;
            verify_28C();
            if(!Epr.flag_err)frmEprom->Message("Device Programed OK",2);
    }
	power_down();/*wil_assert; */           //ARW: was powoff
}

void __fastcall erase_W27E()
{
      unsigned int  i,j,k;
      bool  flag_err = false;
      unsigned char ch;

      for(k=0;k<20;) {
            frmEprom->SetProcessGauge("Erase and Check Device is Empty",Epr.BufferSize,clWhite);
            setup(); oe_lo; ce_hi;
            vpp_on; ms_delay(200);
	        r_adresx(00);
    	    wr_data(0xFF);
	        ce_lo;
            for(j=0;j<9;j++)  ms_delay(20000);
    	    wr_data(0xD0);
	        ce_hi; ms_delay(100);
            vpp_off;
            ms_delay(1000);
//--------------- check Blank
            flag_err = false;
            vpp_off; oe_lo; ce_lo;
            ms_delay(200);
        	for(i=0;i<Epr.BufferSize;i++)
            {
           		r_adres_28C(i);
	        	ch=in_word();
		        if(ch!=0xFF)
                {
                    flag_err = true;
    			    goto loop;
        		}
                frmEprom->gProcess->Progress = i;
                Application->ProcessMessages();
                if(cancel)
                {
                   flag_err = true;
                   goto exit1;
                }
        	}
        k = 20;
loop:  k++;
       }
exit1:
     if(!flag_err) frmEprom->Message("Device Erase OK",2);
     else  frmEprom->Message("Device Erase False",2);
     power_down();
//     winstat();
}

