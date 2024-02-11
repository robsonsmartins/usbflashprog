#include "uEprom.h"

#define EIN 128
#define outhi clkhi
#define outlo clklo
#define IN2 64
#define POW 4
#define RW  2
#define PP  1
#define PGM 8
#define ADR 2
#define CLK 1
#define CLA 4
#define CTW 160
#define CTR 161
#define BIC 0x20

#define einbit  (Inport(ipoort)&EIN)
#define inbit   (Inport(ipoort)&IN2)
#define powoff  (bits &= ~POW)
#define powon   (bits |= POW)
#define read    (bits &= ~RW)
#define write   (bits |= RW)
#define pgrhi   (bits &= ~PGM)
#define pgrlo   (bits |= PGM)
#define ppoff   (bits |= PP)
#define ppon    (bits &= ~PP)
#define adrlo   (outbit &= ~ADR)
#define adrhi   (outbit |= ADR)
#define clklo   (outbit &= ~CLK)
#define clkhi   (outbit |= CLK)
#define clalo   (outbit |=CLA)
#define clahi   (outbit &=~CLA)

#define wil_assert  (Outport(spoort,bits))
#define asadres (Outport(poort,outbit))
//-------------------------------------------------
#define ce_hi   {pgrhi;wil_assert;}     // WE,  code 8 =  C17/S4 Pin 31 High
#define ce_lo   {pgrlo;wil_assert;}     // WE,  code 8 =  C17/S4 Pin 31 Low
#define oe_lo   {read;wil_assert;}      // OE,  code 2 =  S6 high
#define oe_hi   {write;wil_assert;}     // OE,  code 2 =  S6 low
#define vpp_on  {ppon;wil_assert;}      // VPP, code 1 =  C1 12V
#define vpp_off {ppoff;wil_assert;}     // VPP, code 1 =  C1 0V
#define we_lo	ce_lo
#define we_hi	ce_hi
#define cs_lo	ce_lo
#define cs_hi	ce_hi

//-------------------------------------------------
#define	DIP40	40
#define	DIP20	20

#define M27CXXX    0
#define M24C16	   M27CXXX+1
#define M24C32	   M24C16+1
#define M28FXXX	   M24C32+1
#define M29FXXX    M28FXXX+1
#define M28CXXX	   M29FXXX+1

#define AT25CXXX   M28CXXX+1
#define AT28CXXX   AT25CXXX+1
#define M93CXXX    AT28CXXX+1
#define AT29CXXX   M93CXXX+1
#define AT89CXXX   AT29CXXX+1
#define AT89CX051  AT89CXXX+1
#define AT49FXXX   AT89CX051+1
#define M27XXX	   AT49FXXX+1
#define AT90SXXX   M27XXX+1
#define AM29FX00   AT90SXXX+1
#define M16C62X	   AM29FX00+1
#define M16F84	   M16C62X+1
#define M12C50X    M16F84+1
#define I28FX00	   M12C50X+1
#define DS12XX	   I28FX00+1
#define RAM	   DS12XX+1
#define W29EEXXX   RAM+1
#define AT49F00X   W29EEXXX+1
#define W27EXXX    AT49F00X+1

#define PROG1	 1
#define READ     PROG1+1
#define VERIFY	 READ+1
#define ERASE	 VERIFY+1
#define SIGN	 ERASE+1
#define EMPTY	 SIGN+1

#define ON	1
#define OFF	0

//---------- uEPROM-------------
void winstat(void);
void arw_delay(long );
void power_down(void);
void prog_f28(void);
void R_50fuses(void);   	// DW: added void
void getoff(void);		// DW: added void
int  in_word();
void ClearBuf(void);
void Compare(void);

void __fastcall ms_delay(int ticks);
void __fastcall r_adres_28C(unsigned long );
void __fastcall Outport(int IOadd, byte data);
byte __fastcall Inport(int IOadd);

void setup(void);
void error(String Notice);
void idle_mode(void);


//--------- AT29Cxx-----------
int  getch_yes();
void Boot_Block_29C(void);

//-------- AT25xxx --------
void verify_25C(void);
void read_25C(void);
void prog_25C(void);

//----------AVR------------
void verify_AT90S(void);
void prog_AT90S(void);
void erase_AT90S(void);
void CRead_AT90S(void);
void id_AT90S(void);
void empty_AT90S(void);
void lockbit_AT90S(void);
//--------------------------
void Atmel(unsigned char b);


//----------Misc----------------------------
void __fastcall verify_28C(void);
void saveEpr(void);
void restoreEpr(void);
void calcu_adr(void);
void show_twptwc(void);
void __fastcall r_adresx(unsigned long adr,unsigned int delay = 0);
void __fastcall wr_data(unsigned char data,unsigned int delay = 0);
void __fastcall we_pulse(unsigned int delay = 0);
unsigned long __fastcall swap_adr(unsigned long adr);
void __fastcall error2(int adres,int offset,unsigned char data);
void show_ID(unsigned char a,unsigned char b);
void __fastcall CRead_28C(void);
void status_Eprom(void);
//-------------M28CXXX----------------------
void __fastcall prog_M28C(void);
// 27Cxx
void __fastcall prog_M27XXX(void);
// test SRAM
void __fastcall prog_SRAM(void);
void __fastcall tRAM2(void);
// DS12xx
void __fastcall prog_DS12(void);
// 93Cxx
void __fastcall verify_93(void);
void __fastcall read_93(void);
void __fastcall prog_93(void);
void __fastcall erase_93(void);
//-----------i28Fx00,29Fx00-----------------
void __fastcall prog_28Fx00(void);
void __fastcall erase_28Fx00(void);
void id_i28Fx00(void);
// 29Fx00
void __fastcall prog_AM29Fx00(void);
void __fastcall erase_AM29Fx00();
void __fastcall id_29Fx00(void);
// W27EXXX
void __fastcall erase_W27E();
//-----------------------------------------
//          49Fxxx.cpp
//-----------------------------------------
//-------- AT49Fxx-----
void __fastcall prog_AT49F(void);
void __fastcall erase_AT49F();
void __fastcall id_28Fxxx(void);
void __fastcall id_29Fxxx(void);
void __fastcall id_49Fxxx(void);
void __fastcall id_29Cxxx(void);
void __fastcall id_W29EExxx(void);
void __fastcall prog_sector(unsigned long i_adres);
int  __fastcall verify_sector(unsigned long i_adres);
void __fastcall prog_29C(void);
void __fastcall erase_AT29Cxxx(void);
int __fastcall MXIC_F29040_Sect_Status(int sectAddr);   //NC added
void __fastcall MXIC_F29040_Sect_Protect(int sectAddr); //NC added
void __fastcall MXIC_F29040_Chip_Unprot(void);   //NC added
//-------- MCS-51 ----------
void __fastcall verify_AT89C(void);
void __fastcall prog_AT89C(void);
void __fastcall erase_AT89C(void);
void __fastcall CRead_AT89C(void);
void __fastcall id_AT89C(void);
void __fastcall MCS51_lockbit(void);

void __fastcall lockbit_AT89C(void);
int __fastcall empty_AT89C(void);
void __fastcall rdLbit_AT89C(void);
void status_8051(void);
//----------AT49F00x---------------
void __fastcall prog_AT49F00x(void);
//---------------------------------

class Eprom
{
    private:

    public:
	unsigned int  type1,type2,type3;

	unsigned char data[600];
	unsigned int  BL_size,top_bottom,pin28,pin30,serial,flag,Vpp;
	unsigned int  tWP,tWC,add_step,hardware_er,qe,pin7,ep,AT49F00x;
	unsigned int  shift_adr,eep,epr64,oe_vpp,DIP,PIC,Dis_calcu;
	unsigned int  Ssize,flag_err,swap,erase,willem,Dis_showV;
	unsigned long index_adres,pattern_adr,BufferSize;

};

class mcs51
{
    private:
	unsigned long Prog,Clk,Rst,Pwrite,Pread,Plb1,Plb2,
		      Perase,Psig;
	unsigned long mode_wr,mode_rd,mode_erase, mode_sig,
		      mode_bit1, mode_bit2, mode_bit3, mode_Lbit;
	unsigned char d;
	void __fastcall lockbit(unsigned long bit);

    public:
	unsigned char sig30,sig31,sig32,Vpp,tErase,sig_Lbit;
	unsigned int  at89s,tWP,tWC,Pulses,type,tWPb,tWPe,ird_lbit,ModeLock;
	unsigned char __fastcall Dout(unsigned char data);    // arrange D0-D7
	unsigned char __fastcall Din(unsigned char data);
	void __fastcall initl_code(void);
	void __fastcall write_code(unsigned long address,unsigned char data);
	unsigned char __fastcall read_code(unsigned long address);
	void __fastcall write_mode_LB(int mode);
	void __fastcall erase_chip(void);
	void __fastcall read_sign(void);
	void __fastcall inc_adres(void);  // AT89Cx051
	void __fastcall setup2051(void);
	void __fastcall read_lockbit();
};

class spi25{
    private:
	    unsigned long rd,wr;
	    unsigned char mclock_in(void);
	    void mclock_out(unsigned long bit);
    public:
	    unsigned char wren,wrdi,rdsr,wrsr;
	    unsigned long index_adres;
	    unsigned int  data,status,page_size,flag_page;
	    unsigned char protect14,protect12,protectAL;
	    void __fastcall init_code(void);
	    unsigned char spi_data(void);
	    void __fastcall spi_command(unsigned char w);
	    unsigned char spi_read(unsigned long adres);
	    void __fastcall spi_write(unsigned long adres,unsigned char data);
	    int  __fastcall verify_page(unsigned long i_adres);
};

class avr
{
    private:
	unsigned long bs,xa0,xa1,bs_low,bs_hi;
	unsigned long wr,oe,rst,clk;
	unsigned long xadres,xdata,xcommand;
	unsigned char b_erase,b_wr_fuse,b_wr_lock,b_wr_flash;
	unsigned char b_rd_sig,b_rd_lock,b_read,b_eeprom;
	unsigned char p12,d;
	void __fastcall pulse_xtal1(unsigned long ad,unsigned char data);
	void __fastcall pulse_wr(unsigned long xbs,int twp = 0);
	unsigned char __fastcall pulse_oe(unsigned long xbs);
	unsigned char __fastcall Dout(unsigned char data);
	unsigned char __fastcall Din(unsigned char data);
    public:
	unsigned char sig30;
	unsigned char sig31;
	unsigned char sig32;
	unsigned char data0,data1,lock;
	unsigned int  tWP,tWC;
	void __fastcall initl_code(void);
	void __fastcall setup(void);
	void __fastcall erase_chip(void);
	void __fastcall write_flash(unsigned long adres);
	void __fastcall write_eeprom(unsigned int adres);
	void __fastcall read_flash(unsigned long adres);
	void __fastcall read_eeprom(unsigned int adres);
	void __fastcall read_lockbit(void);
	void __fastcall lockbit(unsigned char data);
	void __fastcall fusebit(unsigned char data);
	void __fastcall read_sig(void);
	void __fastcall write_mode_LB(int mode);
};

class m93c46{
    public:
	    unsigned int rd,wr,ewen,ewds,erase,eral,wral;
	    unsigned int adres,data,count_bit,bit16;
	    void init_code(void);
	    void shift_L(int shift);
	    void shift_R(int shift);
	    int mclock_in(void);
	    int min_word(void);
	    void mclock_out(int bit);
	    void mout_word(unsigned char w);
	    void send_instruction(int instr);
};

class microchip{
	public:
		int cp,wd,pt,osc;
		int fuses;
		int w7;
};
