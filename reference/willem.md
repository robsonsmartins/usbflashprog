## Willem Standard Programmer

**Applies To:**

*   [Willem Standard 3.1, 4.0, 4.1 and 4.5](http://web.archive.org/web/20071003005002/http://www.willem.org/)
*   [Sivava PCB3B](http://www.mpu51.com/eprom/eprom.html)
*   [EzoFlash+ 4.4](http://www.ezoflash.com)

**Description:**

The Willem Standard programmer (and variants) is a Parallel Port based memory device programmer.  
It addresses to 24 bits (for parallel target chip).

**Ports Map:**

<table border="1" cellspacing="0" cellpadding="4">
    <th colspan="4">Data Port (offset: 0x00)</th>
    <tr><th>Bit</th><th colspan="2">Parallel Chip</th><th>Serial Chip*</th></tr>
    <tr><td>D7</td><td>Data Bus Q7</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>D6</td><td>Data Bus Q6</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>D5</td><td>Data Bus Q5</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>D4</td><td>Data Bus Q4</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>D3</td><td>Data Bus Q3</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>D2</td><td>Data Bus Q2</td><td>D̅A̅T̅A̅_B̅U̅S̅_C̅L̅K̅</td><td>N/A</td></tr>
    <tr><td>D1</td><td>Data Bus Q1</td><td>ADDR_BUS_DATA</td><td>SERIAL_CLK</td></tr>
    <tr><td>D0</td><td>Data Bus Q0</td><td>ADDR_BUS_CLK</td><td>SERIAL_DIN**</td></tr>
</table>

\* Not applies to Willem 4.5 and EzoFlash+ 4.4.  
\*\* Inverted for 93Cxx and 25Cxx.


<table border="1" cellspacing="0" cellpadding="4">
    <th colspan="3">Status Port (offset: 0x01)</th>
    <tr><th>Bit</th><th>Parallel Chip</th><th>Serial Chip*</th></tr>
    <tr><td>S7</td><td>HARDWARE_TEST</td><td>N/A</td></tr>
    <tr><td>S6</td><td>D̅A̅T̅A̅_B̅U̅S̅_S̅E̅R̅I̅A̅L̅_I̅N̅P̅U̅T̅</td><td>SERIAL_DOUT**</td></tr>
    <tr><td>S5</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>S4</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>S3</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>S2</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>S1</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>S0</td><td>N/A</td><td>N/A</td></tr>
</table>

\* Not applies to Willem 4.5 and EzoFlash+ 4.4.  
\*\* SERIAL\_DIN Bit (DataPort, D0 pin) must be in High(1).


<table border="1" cellspacing="0" cellpadding="4">
    <th colspan="3">Control Port (offset: 0x02)</th>
    <tr><th>Bit</th><th>Parallel Chip</th><th>Serial Chip*</th></tr>
    <tr><td>C7</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>C6</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>C5</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>C4</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>C3</td><td>WE_CONTROL</td><td>S̅E̅R̅I̅A̅L̅_C̅S̅**</td></tr>
    <tr><td>C2</td><td>VDD_CONTROL</td><td>N/A</td></tr>
    <tr><td>C1</td><td>O̅E̅_C̅O̅N̅T̅R̅O̅L̅</td><td>N/A</td></tr>
    <tr><td>C0</td><td>V̅P̅P̅_C̅O̅N̅T̅R̅O̅L̅</td><td>N/A</td></tr>
</table>

\* Not applies to Willem 4.5 and EzoFlash+ 4.4.  
\*\* Can be inverted if the target chip have direct logic (active in high) CS pin(CS).


**Steps for programmer functions:**

* **Startup:**
  * Write 0x01 to Control Port (OE=On, WE=Off, VPP=Off, VDD=Off);
  * Write 0x04 to Data Port (ADDR\_BUS\_DATA=Off, ADDR\_BUS\_CLK=Off, DATA\_BUS\_CLK\=Off);
  * Write 0x000000 to Address Bus

  
* **Hardware Test:**
  * Wait 10 ms;
  * Set VDD to On (C2=1);
  * Wait 100 ms;
  * Set OE to On (C1=0);
  * Set ADDR\_BUS\_CLK to On (D0=1);
  * Wait 10 ms;
  * Read HARDWARE\_TEST bit (S7) to bit\_before variable;
  * Set ADDR\_BUS\_CLK to Off(D0=0);
  * Wait 10 ms;
  * Read HARDWARE\_TEST bit (S7) to bit\_after variable;
  * Set VDD to Off (C2=0);
  * If (bit\_after == 0) or (bit\_before == 1), then "Hardware Not Present";
  * Else, "Hardware Present";

  
* **VDD On:**
  * Set C2 (in Control Port) to 1 (C2=0 to VDD Off);


* **VPP On:**
  * Set C0 (Control Port) to 0 (C0=1 to VPP Off);

  
* **WE On:**
  * Set C3 (Control Port) to 1 (C3=0 to WE Off);

 
* **OE On:**
  * Set C1 (Control Port) to 0 (C1=1 to OE Off);

  
* **CE On:**
  * The CE signal always On;

  
* **Address Bus Write:**
  * Set OE to On (C1=0);
  * Set ADDR\_BUS\_CLK to On (D0=1);
  * Repeat for all address bits, starting at MSB (24 bits max):
    * Write bit to ADDR\_BUS\_DATA (D1=address bit);
    * Set ADDR\_BUS\_CLK to Off (D0=0): clock low;
    * Set ADDR\_BUS\_CLK to On (D0=1): clock high;
  * Set ADDR\_BUS\_DATA to Off (D1=0): out data low;
  * Set ADDR\_BUS\_CLK to Off (D0=0): clock low;
  
* **Data Bus Write:**
  * Set OE to Off (C1=1);
  * Write the data byte to Data Port;

  
* **Data Bus Read:**
  * Set OE to On (C1=0);
  * Set DATA\_BUS\_CLK to Off (D2=1): SR serial clock low;
  * Set ADDR\_BUS\_DATA to On (D1=1): SR parallel load mode;
  * Set DATA\_BUS\_CLK to On (D2=0): SR parallel clock;
  * Set ADDR\_BUS\_DATA to Off (D1=0): SR serial mode;
  * Set DATA\_BUS\_CLK to Off (D2=1): SR serial clock low;
  * Repeat for all data bits, starting at MSB (8 bits):
    * Read DATA\_BUS\_SERIAL\_INPUT bit (data bit=S6);
    * Set DATA\_BUS\_CLK to On (D2=0): SR serial clock high;
    * Set DATA\_BUS\_CLK to Off (D2=1): SR serial clock low;
