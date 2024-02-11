//---------------------------
extern int poort,ipoort,spoort;
extern bits,outbit;
extern unsigned long grot;
extern unsigned long st;
extern unsigned char Buffer[];
extern cancel;
//---------------------------
extern class mcs51 AT89Cxx;
extern class Eprom Epr,Epr1;
extern class spi25 AT25xxx;
extern class avr   AT90Sxx;
extern class AT29C at29Cxxx;
extern class AT29C at28Cxxx;
extern class m93c46 m93Cxx;
//--------------AT29Cxxx-------
extern unsigned long  BB_29C_Ad_h[];
extern unsigned char  BB_29C_Da_h[];
extern unsigned long  BB_29C_Ad_l[];
extern unsigned char  BB_29C_Da_l[];

extern unsigned long  Adr_29C[];
extern unsigned char  Erase_29C[];
extern unsigned char  EN_wr[];
extern unsigned char  Get_ID[];
extern unsigned char  Get_IDw29E[];
extern unsigned char  Rst_chip[];
//-------------------------