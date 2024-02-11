## Sivava PCB4.5/PCB5.0 Programmer

**Applies To:**

*   [Sivava PCB4.5](http://www.sivava.com/)
*   [Sivava PCB5.0](http://www.sivava.com/)
*   [Willem Mod to PCB4.5 by PY2BBS](http://www.py2bbs.qsl.br/willem_pcb45.php)

**Description:**

The Sivava PCB4.5 / PCB5.0 programmer (and variants) is a Parallel Port based memory device programmer.  
It addresses to 24 bits (for parallel target chip).  
This programmer is a fast version of Willem Standard, in a commercial version.

**Ports Map:**

<table border="1" cellspacing="0" cellpadding="4">
    <th colspan="4">Data Port (offset: 0x00)</th>
    <tr><th>Bit</th><th colspan="2">Parallel Chip</th><th>Serial Chip</th></tr>
    <tr><td>D7</td><td>Data Bus Q7</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>D6</td><td>Data Bus Q6</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>D5</td><td>Data Bus Q5</td><td>ADDR_BUS_DATA_D2</td><td>N/A</td></tr>
    <tr><td>D4</td><td>Data Bus Q4</td><td>ADDR_BUS_DATA_D1</td><td>N/A</td></tr>
    <tr><td>D3</td><td>Data Bus Q3</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>D2</td><td>Data Bus Q2</td><td>D̅A̅T̅A̅_B̅U̅S̅_C̅L̅K̅</td><td>N/A</td></tr>
    <tr><td>D1</td><td>Data Bus Q1</td><td>ADDR_BUS_DATA_D0</td><td>SERIAL_CLK</td></tr>
    <tr><td>D0</td><td>Data Bus Q0</td><td>ADDR_BUS_CLK</td><td>SERIAL_DIN*</td></tr>
</table>

\* Inverted for 93Cxx e 25Cxx.


<table border="1" cellspacing="0" cellpadding="4">
    <th colspan="3">Status Port (offset: 0x01)</th>
    <tr><th>Bit</th><th>Parallel Chip</th><th>Serial Chip</th></tr>
    <tr><td>S7</td><td>HARDWARE_TEST</td><td>SERIAL_DOUT*</td></tr>
    <tr><td>S6</td><td>D̅A̅T̅A̅_B̅U̅S̅_S̅E̅R̅I̅A̅L̅_I̅N̅P̅U̅T̅_D̅2</td><td>N/A</td></tr>
    <tr><td>S5</td><td>D̅A̅T̅A̅_B̅U̅S̅_S̅E̅R̅I̅A̅L̅_I̅N̅P̅U̅T̅_D̅1</td><td>N/A</td></tr>
    <tr><td>S4</td><td>D̅A̅T̅A̅_B̅U̅S̅_S̅E̅R̅I̅A̅L̅_I̅N̅P̅U̅T̅_D̅0</td><td>N/A</td></tr>
    <tr><td>S3</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>S2</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>S1</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>S0</td><td>N/A</td><td>N/A</td></tr>
</table>

\* SERIAL\_DIN Bit (DataPort, D0 pin) must be in High(1).


<table border="1" cellspacing="0" cellpadding="4">
    <th colspan="3">Control Port (offset: 0x02)</th>
    <tr><th>Bit</th><th>Parallel Chip</th><th>Serial Chip</th></tr>
    <tr><td>C7</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>C6</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>C5</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>C4</td><td>N/A</td><td>N/A</td></tr>
    <tr><td>C3</td><td>WE_CONTROL</td><td>S̅E̅R̅I̅A̅L̅_C̅S̅*</td></tr>
    <tr><td>C2</td><td>VDD_CONTROL</td><td>N/A</td></tr>
    <tr><td>C1</td><td>O̅E̅_C̅O̅N̅T̅R̅O̅L̅</td><td>N/A</td></tr>
    <tr><td>C0</td><td>V̅P̅P̅_C̅O̅N̅T̅R̅O̅L̅</td><td>N/A</td></tr>
</table>

\* Can be inverted if the target chip have direct logic (active in high) CS pin(CS).

**Bits Map:**

<table border="1" cellspacing="0" cellpadding="4">
    <th colspan="25">Address Bus Bits Map</th>
    <tr><th>Pass</th><th>A23</th><th>A22</th><th>A21</th><th>A20</th><th>A19</th><th>A18</th><th>A17</th><th>A16</th><th>A15</th><th>A14</th><th>A13</th><th>A12</th><th>A11</th><th>A10</th><th>A9</th><th>A8</th><th>A7</th><th>A6</th><th>A5</th><th>A4</th><th>A3</th><th>A2</th><th>A1</th><th>A0</th></tr>
    <tr><th>1</th><td>D2</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D1</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D0</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
    <tr><th>2</th><td>&nbsp;</td><td>D2</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D1</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D0</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
    <tr><th>3</th><td>&nbsp;</td><td>&nbsp;</td><td>D2</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D1</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D0</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
    <tr><th>4</th><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D2</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D1</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D0</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
    <tr><th>5</th><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D2</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D1</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D0</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
    <tr><th>6</th><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D2</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D1</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D0</td>    <td>&nbsp;</td><td>&nbsp;</td></tr>
    <tr><th>7</th><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D2</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D1</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D0</td>    <td>&nbsp;</td></tr>
    <tr><th>8</th><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D2</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D1</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D0</td>    </tr>
</table>


<table border="1" cellspacing="0" cellpadding="4">
    <th colspan="9">Data Bus Bits Map (for Read)</th>
    <tr><th>Pass</th><th>Q7</th><th>Q6</th><th>Q5</th><th>Q4</th><th>Q3</th><th>Q2</th><th>Q1</th><th>Q0</th></tr>
    <tr><th>1</th><td>D2</td>    <td>D1</td>    <td>D0</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
    <tr><th>2</th><td>&nbsp;</td><td>D2</td>    <td>D1</td>    <td>D0</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
    <tr><th>3</th><td>&nbsp;</td><td>&nbsp;</td><td>D2</td>    <td>D1</td>    <td>D0</td>    <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
    <tr><th>4</th><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D2</td>    <td>D1</td>    <td>D0</td>    <td>&nbsp;</td><td>&nbsp;</td></tr>
    <tr><th>5</th><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D2</td>    <td>D1</td>    <td>D0</td>    <td>&nbsp;</td></tr>
    <tr><th>6</th><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>D2</td>    <td>D1</td>    <td>D0</td>    </tr>
</table>

**Steps for programmer functions:**

* **Startup:**
  * Write 0x01 to Control Port (OE=On, WE=Off, VPP=Off, VDD=Off);
  * Write 0x04 to Data Port (ADDR\_BUS\_DATA\_D0=Off, ADDR\_BUS\_DATA\_D1=Off, ADDR\_BUS\_DATA\_D2=Off, ADDR\_BUS\_CLK=Off, DATA\_BUS\_CLK\=Off);
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
  * Repeat for all address bits (in groups of 3), starting at MSB (24 bits max):
    * Write bit to ADDR\_BUS\_DATA\_D0 (D1=address bit X);
    * Write bit to ADDR\_BUS\_DATA\_D1 (D4=address bit Y);
    * Write bit to ADDR\_BUS\_DATA\_D2 (D5=address bit Z);
    * Set ADDR\_BUS\_CLK to Off (D0=0): clock low;
    * Set ADDR\_BUS\_CLK to On (D0=1): clock high;
  * Set ADDR\_BUS\_DATA\_D0 to Off (D1=0);
  * Set ADDR\_BUS\_DATA\_D1 to Off (D4=0);
  * Set ADDR\_BUS\_DATA\_D2 to Off (D5=0);
  * Set ADDR\_BUS\_CLK to Off (D0=0): clock low;

  
* **Data Bus Write:**
  * Set OE to Off (C1=1);
  * Write the data byte to Data Port;

  
* **Data Bus Read:**
  * Set OE to On (C1=0);
  * Set DATA\_BUS\_CLK to Off (D2=1): SR serial clock low;
  * Set ADDR\_BUS\_DATA\_D0 to On (D1=1): SR parallel load mode;
  * Set ADDR\_BUS\_DATA\_D1 to On (D4=1): SR parallel load mode;
  * Set ADDR\_BUS\_DATA\_D2 to On (D5=1): SR parallel load mode;
  * Set DATA\_BUS\_CLK to On (D2=0): SR parallel clock;
  * Set ADDR\_BUS\_DATA\_D0 to Off (D1=0): SR serial mode;
  * Set ADDR\_BUS\_DATA\_D1 to Off (D4=0): SR serial mode;
  * Set ADDR\_BUS\_DATA\_D2 to Off (D5=0): SR serial mode;
  * Set DATA\_BUS\_CLK to Off (D2=1): SR serial clock low;
  * Repeat for all data bits (in groups of 3), starting at MSB (8 bits):
    * Read DATA\_BUS\_SERIAL\_INPUT\_D2 bit (data bit X=S6);
    * Read DATA\_BUS\_SERIAL\_INPUT\_D1 bit (data bit Y=S5);
    * Read DATA\_BUS\_SERIAL\_INPUT\_D0 bit (data bit Z=S4);
    * Set DATA\_BUS\_CLK to On (D2=0): SR serial clock high;
    * Set DATA\_BUS\_CLK to Off (D2=1): SR serial clock low;
