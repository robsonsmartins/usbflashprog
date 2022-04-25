EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 8268 5846
encoding utf-8
Sheet 7 8
Title "USB Flash/EPROM Programmer"
Date "2022-04-23"
Rev "v0.1.0 rev.E"
Comp "Robson Martins"
Comment1 "https://usbflashprog.robsonmartins.com"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_RaspberryPi_and_Boards:Pico U?
U 1 1 6265E206
P 2600 2450
F 0 "U?" H 2600 3665 50  0000 C CNN
F 1 "Pico" H 2600 3574 50  0000 C CNN
F 2 "RPi_Pico:RPi_Pico_SMD_TH" V 2600 2450 50  0001 C CNN
F 3 "" H 2600 2450 50  0001 C CNN
	1    2600 2450
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74HC595 U?
U 1 1 62661EF3
P 5600 2400
F 0 "U?" H 5800 3100 50  0000 C CNN
F 1 "74HC595" H 5900 3000 50  0000 C CNN
F 2 "" H 5600 2400 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 5600 2400 50  0001 C CNN
	1    5600 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 6266368E
P 1800 4700
F 0 "C?" H 1915 4746 50  0000 L CNN
F 1 "100nF" H 1915 4655 50  0000 L CNN
F 2 "" H 1838 4550 50  0001 C CNN
F 3 "~" H 1800 4700 50  0001 C CNN
	1    1800 4700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 626642E6
P 3600 2500
F 0 "R?" V 3458 2502 50  0000 C CNN
F 1 "1K" V 3551 2683 50  0000 C CNN
F 2 "" V 3530 2500 50  0001 C CNN
F 3 "~" H 3600 2500 50  0001 C CNN
	1    3600 2500
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62664AE0
P 5600 3700
F 0 "#PWR?" H 5600 3450 50  0001 C CNN
F 1 "GND" H 5605 3527 50  0001 C CNN
F 2 "" H 5600 3700 50  0001 C CNN
F 3 "" H 5600 3700 50  0001 C CNN
	1    5600 3700
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 6266517D
P 3400 1200
F 0 "#PWR?" H 3400 1050 50  0001 C CNN
F 1 "+5V" H 3415 1373 50  0000 C CNN
F 2 "" H 3400 1200 50  0001 C CNN
F 3 "" H 3400 1200 50  0001 C CNN
	1    3400 1200
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 62665936
P 3900 1200
F 0 "#PWR?" H 3900 1050 50  0001 C CNN
F 1 "+3V3" H 3915 1373 50  0000 C CNN
F 2 "" H 3900 1200 50  0001 C CNN
F 3 "" H 3900 1200 50  0001 C CNN
	1    3900 1200
	1    0    0    -1  
$EndComp
Entry Wire Line
	1200 3000 1300 3100
Entry Wire Line
	1200 2900 1300 3000
Entry Wire Line
	1200 3200 1300 3300
Entry Wire Line
	1200 3300 1300 3400
Wire Wire Line
	1300 3000 1900 3000
Wire Wire Line
	1900 3100 1300 3100
Wire Wire Line
	1900 3300 1300 3300
Wire Wire Line
	1900 3400 1300 3400
$Comp
L power:GND #PWR?
U 1 1 62669B4A
P 1800 3700
F 0 "#PWR?" H 1800 3450 50  0001 C CNN
F 1 "GND" H 1805 3527 50  0001 C CNN
F 2 "" H 1800 3700 50  0001 C CNN
F 3 "" H 1800 3700 50  0001 C CNN
	1    1800 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 3700 1800 3200
Wire Wire Line
	1800 1700 1900 1700
Wire Wire Line
	1900 2200 1800 2200
Connection ~ 1800 2200
Wire Wire Line
	1800 2200 1800 1700
Wire Wire Line
	1900 2700 1800 2700
Connection ~ 1800 2700
Wire Wire Line
	1800 2700 1800 2200
Wire Wire Line
	1900 3200 1800 3200
Connection ~ 1800 3200
Wire Wire Line
	1800 3200 1800 2700
$Comp
L power:GND #PWR?
U 1 1 6266AAD1
P 3400 3700
F 0 "#PWR?" H 3400 3450 50  0001 C CNN
F 1 "GND" H 3405 3527 50  0001 C CNN
F 2 "" H 3400 3700 50  0001 C CNN
F 3 "" H 3400 3700 50  0001 C CNN
	1    3400 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 3700 3400 3200
Wire Wire Line
	3400 1700 3300 1700
Wire Wire Line
	3400 3200 3300 3200
Connection ~ 3400 3200
Wire Wire Line
	3400 3200 3400 2700
Wire Wire Line
	3400 2700 3300 2700
Connection ~ 3400 2700
Wire Wire Line
	3400 2700 3400 2200
Wire Wire Line
	3300 2200 3400 2200
Connection ~ 3400 2200
Wire Wire Line
	3400 1200 3400 1500
Wire Wire Line
	3400 1600 3300 1600
Wire Wire Line
	3300 1500 3400 1500
Connection ~ 3400 1500
Wire Wire Line
	3400 1500 3400 1600
Wire Wire Line
	3300 1900 3900 1900
Wire Wire Line
	3900 1900 3900 1200
Entry Wire Line
	4400 2100 4500 2200
Entry Wire Line
	4400 2300 4500 2400
Entry Wire Line
	4400 2400 4500 2500
Entry Wire Line
	4400 2600 4500 2700
Entry Wire Line
	4400 2800 4500 2900
Entry Wire Line
	4400 2900 4500 3000
Entry Wire Line
	4400 3300 4500 3400
$Comp
L power:GND #PWR?
U 1 1 62670BC9
P 5100 3700
F 0 "#PWR?" H 5100 3450 50  0001 C CNN
F 1 "GND" H 5105 3527 50  0001 C CNN
F 2 "" H 5100 3700 50  0001 C CNN
F 3 "" H 5100 3700 50  0001 C CNN
	1    5100 3700
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 62670E0E
P 5600 1200
F 0 "#PWR?" H 5600 1050 50  0001 C CNN
F 1 "+5V" H 5615 1373 50  0000 C CNN
F 2 "" H 5600 1200 50  0001 C CNN
F 3 "" H 5600 1200 50  0001 C CNN
	1    5600 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 3700 5100 2600
Wire Wire Line
	5100 2600 5200 2600
Wire Wire Line
	5600 3700 5600 3100
Wire Wire Line
	5600 1800 5600 1200
Entry Wire Line
	4500 1900 4600 2000
Entry Wire Line
	4500 2100 4600 2200
Entry Wire Line
	4500 2200 4600 2300
Entry Wire Line
	4500 2400 4600 2500
Wire Wire Line
	4600 2000 5200 2000
Wire Wire Line
	4600 2200 5200 2200
Wire Wire Line
	4600 2300 5200 2300
Wire Wire Line
	4600 2500 5200 2500
Wire Wire Line
	3400 2200 3400 1700
Wire Wire Line
	3300 2100 4400 2100
Wire Wire Line
	3300 2300 4400 2300
Wire Wire Line
	3300 2400 4400 2400
Wire Wire Line
	4400 2600 3300 2600
Wire Wire Line
	4400 2800 3300 2800
Wire Wire Line
	3300 2900 4400 2900
Wire Wire Line
	3300 3300 4400 3300
Wire Bus Line
	1200 900  4500 900 
Wire Bus Line
	4500 900  6800 900 
Connection ~ 4500 900 
Entry Wire Line
	6700 2000 6800 2100
Entry Wire Line
	6700 2100 6800 2200
Entry Wire Line
	6700 2200 6800 2300
Entry Wire Line
	6700 2300 6800 2400
Entry Wire Line
	6700 2400 6800 2500
Wire Wire Line
	6000 2000 6700 2000
Wire Wire Line
	6000 2100 6700 2100
Wire Wire Line
	6000 2200 6700 2200
Wire Wire Line
	6000 2300 6700 2300
Wire Wire Line
	6700 2400 6000 2400
Wire Wire Line
	3300 2500 3450 2500
Wire Wire Line
	3750 2500 3900 2500
Wire Wire Line
	3900 2500 3900 1900
Connection ~ 3900 1900
Text Label 4600 2000 0    50   ~ 0
VCSIN
Text Label 4600 2200 0    50   ~ 0
VCCLK
Text Label 4600 2300 0    50   ~ 0
~VCCLR~
Text Label 4600 2500 0    50   ~ 0
VCRCK
$Comp
L power:GND #PWR?
U 1 1 626816A9
P 1800 5000
F 0 "#PWR?" H 1800 4750 50  0001 C CNN
F 1 "GND" H 1805 4827 50  0001 C CNN
F 2 "" H 1800 5000 50  0001 C CNN
F 3 "" H 1800 5000 50  0001 C CNN
	1    1800 5000
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 62681869
P 1800 4400
F 0 "#PWR?" H 1800 4250 50  0001 C CNN
F 1 "+5V" H 1815 4573 50  0000 C CNN
F 2 "" H 1800 4400 50  0001 C CNN
F 3 "" H 1800 4400 50  0001 C CNN
	1    1800 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 4400 1800 4550
Wire Wire Line
	1800 4850 1800 5000
Text HLabel 7000 1300 2    50   Output ~ 0
VPP_ON_A9
Text HLabel 7000 1400 2    50   Output ~ 0
VPP_ON_A18
Text HLabel 7000 1500 2    50   Output ~ 0
VPP_ON_CE
Text HLabel 7000 1600 2    50   Output ~ 0
VPP_ON_OE
Text HLabel 7000 1700 2    50   Output ~ 0
VPP_ON_WE
Entry Wire Line
	6800 1200 6900 1300
Entry Wire Line
	6800 1300 6900 1400
Entry Wire Line
	6800 1400 6900 1500
Entry Wire Line
	6800 1500 6900 1600
Entry Wire Line
	6800 1600 6900 1700
Wire Wire Line
	6900 1300 7000 1300
Wire Wire Line
	6900 1400 7000 1400
Wire Wire Line
	6900 1500 7000 1500
Wire Wire Line
	6900 1600 7000 1600
Wire Wire Line
	6900 1700 7000 1700
Text Label 6700 2000 2    50   ~ 0
VPP_ON_A18
Text Label 6700 2100 2    50   ~ 0
VPP_ON_A9
Text Label 6700 2200 2    50   ~ 0
VPP_ON_CE
Text Label 6700 2300 2    50   ~ 0
VPP_ON_OE
Text Label 6700 2400 2    50   ~ 0
VPP_ON_WE
Text HLabel 7000 1900 2    50   Output ~ 0
VPP_CTRL
Text HLabel 7000 2000 2    50   Output ~ 0
VPP_PWM
Text HLabel 7000 2100 2    50   Input ~ 0
VPP_FB
Text HLabel 7000 2300 2    50   Output ~ 0
VDD_CTRL
Text HLabel 7000 2400 2    50   Output ~ 0
VDD_PWM
Text HLabel 7000 2500 2    50   Input ~ 0
VDD_FB
Text HLabel 7000 2700 2    50   Output ~ 0
VDD_ON_VPP
Entry Wire Line
	6800 1800 6900 1900
Entry Wire Line
	6800 1900 6900 2000
Entry Wire Line
	6800 2000 6900 2100
Entry Wire Line
	6800 2200 6900 2300
Entry Wire Line
	6800 2300 6900 2400
Entry Wire Line
	6800 2400 6900 2500
Entry Wire Line
	6800 2600 6900 2700
Wire Wire Line
	6900 1900 7000 1900
Wire Wire Line
	6900 2000 7000 2000
Wire Wire Line
	6900 2100 7000 2100
Wire Wire Line
	6900 2300 7000 2300
Wire Wire Line
	6900 2400 7000 2400
Wire Wire Line
	6900 2500 7000 2500
Wire Wire Line
	6900 2700 7000 2700
Entry Wire Line
	4400 3000 4500 3100
Entry Wire Line
	4400 3100 4500 3200
Entry Wire Line
	4400 3400 4500 3500
Entry Wire Line
	1200 2800 1300 2900
Entry Wire Line
	1200 2700 1300 2800
Entry Wire Line
	1200 2500 1300 2600
Entry Wire Line
	1200 2400 1300 2500
Entry Wire Line
	1200 2300 1300 2400
Entry Wire Line
	1200 2200 1300 2300
Entry Wire Line
	1200 2000 1300 2100
Entry Wire Line
	1200 1900 1300 2000
Entry Wire Line
	1200 1900 1300 2000
Entry Wire Line
	1200 1800 1300 1900
Entry Wire Line
	1200 1700 1300 1800
Entry Wire Line
	1200 1400 1300 1500
Entry Wire Line
	1200 1500 1300 1600
Wire Wire Line
	1300 1500 1900 1500
Wire Wire Line
	1300 1600 1900 1600
Wire Wire Line
	1300 1800 1900 1800
Wire Wire Line
	1300 1900 1900 1900
Wire Wire Line
	1300 2000 1900 2000
Wire Wire Line
	1300 2100 1900 2100
Wire Wire Line
	1300 2300 1900 2300
Wire Wire Line
	1300 2400 1900 2400
Wire Wire Line
	1300 2500 1900 2500
Wire Wire Line
	1300 2600 1900 2600
Wire Wire Line
	1300 2800 1900 2800
Wire Wire Line
	1300 2900 1900 2900
Wire Wire Line
	3300 3400 4400 3400
Wire Wire Line
	3300 3100 4400 3100
Wire Wire Line
	4400 3000 3300 3000
Text Label 4400 2100 2    50   ~ 0
VPP_CTRL
Text Label 4400 2300 2    50   ~ 0
VPP_FB
Text Label 4400 2400 2    50   ~ 0
VDD_FB
Text Label 4400 2600 2    50   ~ 0
VDD_CTRL
Text Label 4400 2800 2    50   ~ 0
VPP_PWM
Text Label 4400 2900 2    50   ~ 0
VDD_PWM
Text Label 4400 3300 2    50   ~ 0
VDD_ON_VPP
Text Label 1300 3000 0    50   ~ 0
VCSIN
Text Label 1300 3100 0    50   ~ 0
VCRCK
Text Label 1300 3300 0    50   ~ 0
VCCLK
Text Label 1300 3400 0    50   ~ 0
~VCCLR~
Text HLabel 900  1200 0    50   Output ~ 0
ASIN
Text HLabel 900  1300 0    50   Output ~ 0
ACLK
Text HLabel 900  1400 0    50   Output ~ 0
~ACLR~
Text HLabel 900  1500 0    50   Output ~ 0
ARCK
Entry Wire Line
	1100 1200 1200 1300
Entry Wire Line
	1100 1300 1200 1400
Entry Wire Line
	1100 1400 1200 1500
Entry Wire Line
	1100 1500 1200 1600
Wire Wire Line
	900  1200 1100 1200
Wire Wire Line
	1100 1300 900  1300
Wire Wire Line
	900  1400 1100 1400
Wire Wire Line
	1100 1500 900  1500
Text Label 1300 1500 0    50   ~ 0
ASIN
Text Label 1300 1600 0    50   ~ 0
ARCK
Text Label 1300 1800 0    50   ~ 0
ACLK
Text Label 1300 1900 0    50   ~ 0
~ACLR~
Text Label 1300 2600 0    50   ~ 0
OE
Text Label 1300 2800 0    50   ~ 0
WE
Text Label 1300 2900 0    50   ~ 0
CE
Text HLabel 900  1700 0    50   Output ~ 0
OE
Text HLabel 900  1800 0    50   Output ~ 0
WE
Text HLabel 900  1900 0    50   Output ~ 0
CE
Entry Wire Line
	1100 1700 1200 1800
Entry Wire Line
	1100 1800 1200 1900
Entry Wire Line
	1100 1900 1200 2000
Wire Wire Line
	900  1700 1100 1700
Wire Wire Line
	900  1800 1100 1800
Wire Wire Line
	900  1900 1100 1900
Text Label 1300 2000 0    50   ~ 0
DSIN
Text Label 1300 2100 0    50   ~ 0
DRCK
Text Label 1300 2300 0    50   ~ 0
DCLK
Text Label 1300 2400 0    50   ~ 0
~DCLR~
Text Label 1300 2500 0    50   ~ 0
DSOUT
Text HLabel 900  2100 0    50   Output ~ 0
DSIN
Text HLabel 900  2200 0    50   Output ~ 0
DCLK
Text HLabel 900  2300 0    50   Output ~ 0
~DCLR~
Text HLabel 900  2400 0    50   Output ~ 0
DRCK
Text HLabel 900  2500 0    50   Input ~ 0
DSOUT
Entry Wire Line
	1100 2100 1200 2200
Entry Wire Line
	1100 2200 1200 2300
Entry Wire Line
	1100 2300 1200 2400
Entry Wire Line
	1100 2400 1200 2500
Entry Wire Line
	1100 2500 1200 2600
Wire Wire Line
	900  2100 1100 2100
Wire Wire Line
	1100 2200 900  2200
Wire Wire Line
	900  2300 1100 2300
Wire Wire Line
	1100 2400 900  2400
Wire Wire Line
	900  2500 1100 2500
Wire Bus Line
	4500 900  4500 4000
Wire Bus Line
	6800 900  6800 4000
Wire Bus Line
	1200 900  1200 4000
$EndSCHEMATC
