#include "willem2.h"
#include "willem.h"

void __fastcall prog_28Fx00(void)
{
	unsigned int i, y,q,hgrot = grot;
	unsigned char g,ch;
    int flag;

	if(st!=0) getoff();
	if(grot>=Epr.BufferSize) grot=Epr.BufferSize-1;
   	frmEprom->SetProcessGauge("programming chip",grot,clRed);

	q = 1;
	setup(); we_hi;
	vpp_on; ms_delay(100);
	r_adres_28C(0);

    Epr.flag_err = OFF;
	for(i=0;i<=grot;i++){
		y=i;
		g=Buffer[y];

rep_28F:
		r_adresx(y);
		wr_data(0x40);  ms_delay(2);
		we_pulse(2);	ms_delay(2);
		wr_data(g);     ms_delay(2);
		we_pulse(Epr.tWP);	ms_delay(Epr.tWC);

		Outport(poort,0x00);
		oe_lo;ms_delay(5);
		flag = OFF;
		for(;flag != ON;) {
      	     oe_lo;ms_delay(2);
			 ch = in_word();
		     oe_hi;ms_delay(2);
		     if(ch&0x80) flag = ON;
             Application->ProcessMessages();
             if(cancel){
       		    frmEprom->Message("program interrupted!",1);
               	Epr.flag_err = true;
       		    goto exit_28F;
       	      }
		}
		if(ch&0x04) {
		      Epr.flag_err = true;
      		  error("  Vpp Range Error");
		      power_down();
		      goto exit_28F;
		}else if(ch&0x10) {
			if(q == 10){
			  power_down();
			  error("  Byte Program Error");
			  Epr.flag_err = true;
			  goto exit_28F;
			}
            else { q++; goto rep_28F;}
		}

       	frmEprom->gProcess->Progress = i;
       	Application->ProcessMessages();
        if(cancel){
         		frmEprom->Message("program interrupted!",1);
               	Epr.flag_err = true;
          		break;
       	}
		q = 1;
	}
exit_28F: vpp_off;
    if(!Epr.flag_err)  {
           Epr.Dis_showV = ON;
           verify_28C();
           if(!Epr.flag_err)frmEprom->Message("Device Programed OK",2);
     }
     grot=hgrot;
	 power_down();/*wil_assert;*/
}

void __fastcall erase_28Fx00(void)
{
      int  e,y;
      bool flag ;
      unsigned int adr = 0;
      unsigned char ch;

      e = (Epr.BufferSize/0x2000);
      frmEprom->SetProcessGauge("Erasing chip 20S",e,clWhite);

      setup(); we_hi;
      vpp_on;ms_delay(200);
      e = 0;
      Epr.flag_err = OFF;
      for(; adr < Epr.BufferSize;) {
	    r_adresx(adr);
	    wr_data(0x20);
	    we_pulse(3);
	    wr_data(0xD0);
	    we_pulse(3);

	    ms_delay(200);
	    flag = false;
	    for(;!flag;) {
		     oe_lo;ms_delay(4);
 			 ch = in_word();
		     oe_hi;ms_delay(4);
		     if(ch&0x80) flag = true;
             if(cancel){
       		    frmEprom->Message("program interrupted!",1);
               	Epr.flag_err = true;
       		    goto exit_28F;
       	      }
		}
		if(ch&0x04) {
		      error("   Vpp Range Error");
		      Epr.flag_err = true;
		      goto exit_28F;
		}else if((ch&0x10) && (ch&0x20)) {
		      error(" Command Sequence Error");
		      Epr.flag_err = true;
		      goto exit_28F;
		}else if(ch&0x20) {
		      error(" Block Erase Error");;
		      Epr.flag_err = true;
		      goto exit_28F;
		}
        frmEprom->gProcess->Progress = e;
        Application->ProcessMessages();
        if(cancel){
         		frmEprom->Message("program interrupted!",1);
               	Epr.flag_err = true;
          		goto exit_28F;
       	}
		e++;
		adr = adr+0x2000;
	}

exit_28F:
	vpp_off;ms_delay(200);
	power_down();
    if(!Epr.flag_err)  frmEprom->Message("erase completed",2);
}

//---------------------
//     28F200,28F400,28F800
//---------------------
void id_i28Fx00(void)
{           // DW: added void
	unsigned char a,b;

	setup();
	r_adresx(0);
	vpp_on;ms_delay(200);

	wr_data(0x90,5);	ms_delay(10);
	we_pulse(5);		ms_delay(10);
	r_adresx(0x00,5);       ms_delay(5);
	a = in_word();

	ms_delay(10000);
	wr_data(0x90,5); 	ms_delay(10);
	we_pulse(10);		ms_delay(20);
	r_adresx(0x02,5); 	ms_delay(5);
	b = in_word();

	vpp_off;ms_delay(200);
	power_down();
    show_ID(a,b);
}

//---------------------------------------
//
//---------------------------------------
unsigned long  C_adres[]   	= {0xAAA,0x555,0xAAA,0xAAA,0x555,0xAAA};
unsigned char  C_Erase_chip[] 	= {0xAA,0x55,0x80,0xAA,0x55,0x10};
unsigned char  C_Erase_sector[] = {0xAA,0x55,0x80,0xAA,0x55,0x30};
unsigned char  C_Prog_data[] 	= {0xAA,0x55,0xA0};
unsigned char  C_ID1_data[] 	= {0xAA,0x55,0x90};
unsigned char  C_ID2_data[] 	= {0xAA,0x55,0xF0};

void enSoftP_AM29Fx00(void){
	for(int i=0 ;i<3;i++) {
         oe_lo; r_adres_28C(C_adres[i]);
         oe_hi; Outport(poort,C_Prog_data[i]);
//	     r_adresx(C_adres[i],0);     // ms_delay(1);
//	     wr_data(C_Prog_data[i],0);  ///ms_delay(1);
	     we_pulse(2);	        ms_delay(4);
	}
}

void __fastcall prog_AM29Fx00(void)
{
	unsigned int i;
    int y,q,hgrot = grot;
    int tBP = 20;
	unsigned char g,ch;

	if(st!=0) getoff();
	if(grot>=Epr.BufferSize) grot=Epr.BufferSize-1;
      	frmEprom->SetProcessGauge("Programming chip",grot,clRed);
	Application->ProcessMessages();

	q = 1;
	setup(); we_hi;
	r_adres_28C(0);
    Epr.flag_err = OFF;

	for(i=0;i<=grot;i++){
		y =i+st;
rep_29F:
		g=Buffer[y];
	    enSoftP_AM29Fx00();

		oe_lo; r_adres_28C(i);
		oe_hi; Outport(poort,g);  ms_delay(1);
		we_pulse(2); ms_delay(tBP);

		Outport(poort,0x00); ms_delay(1);
		oe_lo;
		ch = in_word();
		if(g!=ch ){
			if(q==10){
				error2(i,st,ch);
				Epr.flag_err = true;
				goto exit_29F;
			}
			else {
				q++; tBP += 20;
				if(tBP > 200) tBP = 200;
				goto rep_29F;
			}
		}

       		frmEprom->gProcess->Progress = i;
       		Application->ProcessMessages();
        	if(cancel){
         		frmEprom->Message("program interrupted!",1);
               		Epr.flag_err = true;
          		goto exit_29F;
       		}
		q = 1;
	}

exit_29F:
    if(!Epr.flag_err)  {
            Epr.Dis_showV = ON;
            verify_28C();
            if(!Epr.flag_err)frmEprom->Message("Device Programed OK",2);
     }
     grot=hgrot;
	 power_down();/*wil_assert; */           //ARW: was powoff
}

void __fastcall erase_AM29Fx00()
{
      int i,loop = 1100;

      frmEprom->pProcess->Visible = true;
      frmEprom->SetProcessGauge("Erasing chip",loop,clRed);

      setup(); we_hi;
      for(i=0;i<6;i++) {
	    r_adresx(C_adres[i]);
	    wr_data(C_Erase_chip[i]);
	    we_pulse(3);
      }
      // delay 20 sec.
     Epr.flag_err = OFF;
     for(i=0;i<loop;i++) {
         frmEprom->gProcess->Progress = i;
         Application->ProcessMessages();
	        if(cancel){
          		frmEprom->Message("program interrupted!",1);
                Epr.flag_err = true;
          		break;
         }
         ms_delay(20000);
    }

    if(!Epr.flag_err) frmEprom->Message("erase completed",2);
    power_down();/*wil_assert;*/        //ARW: was powoff
    idle_mode();   //ARW: duplication
    winstat();
}


void __fastcall id_29Fx00(void)
{           // DW: added void
	unsigned char a,b;
    int i;

	setup();we_hi;

	for(i=0 ;i<3;i++) {
	     r_adresx(C_adres[i],4);   ms_delay(4);
	     wr_data(C_ID1_data[i],4); ms_delay(4);
	     we_pulse(2);	       ms_delay(10);
	}
	ms_delay(2000);
	r_adresx(0x00);
	a = in_word();  oe_hi;
	ms_delay(1000);
	r_adresx(0x02);
	b = in_word();  oe_hi;
	ms_delay(1000);

	for(i=0 ;i<3;i++) {
	     r_adresx(C_adres[i],4);    ms_delay(4);
	     wr_data(C_ID2_data[i],4);  ms_delay(4);
	     we_pulse(2);	            ms_delay(10);
	}
	ms_delay(4000);
	power_down();
	show_ID(a,b);
}


