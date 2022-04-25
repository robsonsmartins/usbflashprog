EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr User 8268 5846
encoding utf-8
Sheet 2 8
Title "USB Flash/EPROM Programmer"
Date "2022-04-23"
Rev "v0.1.0 rev.E"
Comp "Robson Martins"
Comment1 "https://usbflashprog.robsonmartins.com"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Entry Wire Line
	2700 1600 2800 1700
Entry Wire Line
	2700 2100 2800 2200
$Comp
L Device:R R?
U 1 1 62693EEA
P 3400 1700
F 0 "R?" V 3193 1700 50  0000 C CNN
F 1 "220R" V 3284 1700 50  0000 C CNN
F 2 "" V 3330 1700 50  0001 C CNN
F 3 "~" H 3400 1700 50  0001 C CNN
	1    3400 1700
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 6269431B
P 5400 2400
F 0 "R?" H 5470 2446 50  0000 L CNN
F 1 "470R" H 5470 2355 50  0000 L CNN
F 2 "" V 5330 2400 50  0001 C CNN
F 3 "~" H 5400 2400 50  0001 C CNN
	1    5400 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 6269455A
P 5400 1700
F 0 "R?" H 5470 1746 50  0000 L CNN
F 1 "3K9" H 5470 1655 50  0000 L CNN
F 2 "" V 5330 1700 50  0001 C CNN
F 3 "~" H 5400 1700 50  0001 C CNN
	1    5400 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C?
U 1 1 62694864
P 4600 1700
F 0 "C?" H 4700 1600 50  0000 L CNN
F 1 "100uF" H 4700 1521 50  0000 L CNN
F 2 "" H 4638 1550 50  0001 C CNN
F 3 "~" H 4600 1700 50  0001 C CNN
	1    4600 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 62694EE8
P 5000 1700
F 0 "C?" H 5100 1600 50  0000 L CNN
F 1 "100nF" H 5099 1524 50  0000 L CNN
F 2 "" H 5038 1550 50  0001 C CNN
F 3 "~" H 5000 1700 50  0001 C CNN
	1    5000 1700
	1    0    0    -1  
$EndComp
$Comp
L Diode:BAT46 D?
U 1 1 62695464
P 4300 1500
F 0 "D?" H 4300 1400 50  0000 C CNN
F 1 "BAT46" H 4293 1609 50  0000 C CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 4300 1325 50  0001 C CNN
F 3 "http://www.vishay.com/docs/85662/bat46.pdf" H 4300 1500 50  0001 C CNN
	1    4300 1500
	-1   0    0    1   
$EndComp
$Comp
L Transistor_BJT:TIP120 Q?
U 1 1 6269613F
P 3900 1700
F 0 "Q?" H 4137 1647 50  0000 L CNN
F 1 "TIP120" H 4143 1565 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 4100 1625 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/TIP120-D.PDF" H 3900 1700 50  0001 L CNN
	1    3900 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:L_Core_Ferrite L?
U 1 1 62697554
P 4000 1200
F 0 "L?" H 4088 1246 50  0000 L CNN
F 1 "300uH" H 4088 1155 50  0000 L CNN
F 2 "" H 4000 1200 50  0001 C CNN
F 3 "~" H 4000 1200 50  0001 C CNN
	1    4000 1200
	1    0    0    -1  
$EndComp
$Comp
L power:+9V #PWR?
U 1 1 626985EC
P 4000 900
F 0 "#PWR?" H 4000 750 50  0001 C CNN
F 1 "+9V" H 4015 1073 50  0000 C CNN
F 2 "" H 4000 900 50  0001 C CNN
F 3 "" H 4000 900 50  0001 C CNN
	1    4000 900 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62698B02
P 4000 2000
F 0 "#PWR?" H 4000 1750 50  0001 C CNN
F 1 "GND" H 4005 1827 50  0001 C CNN
F 2 "" H 4000 2000 50  0001 C CNN
F 3 "" H 4000 2000 50  0001 C CNN
	1    4000 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6269E37A
P 4600 2000
F 0 "#PWR?" H 4600 1750 50  0001 C CNN
F 1 "GND" H 4605 1827 50  0001 C CNN
F 2 "" H 4600 2000 50  0001 C CNN
F 3 "" H 4600 2000 50  0001 C CNN
	1    4600 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6269E625
P 5000 2000
F 0 "#PWR?" H 5000 1750 50  0001 C CNN
F 1 "GND" H 5005 1827 50  0001 C CNN
F 2 "" H 5000 2000 50  0001 C CNN
F 3 "" H 5000 2000 50  0001 C CNN
	1    5000 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6269E831
P 5400 2600
F 0 "#PWR?" H 5400 2350 50  0001 C CNN
F 1 "GND" H 5405 2427 50  0001 C CNN
F 2 "" H 5400 2600 50  0001 C CNN
F 3 "" H 5400 2600 50  0001 C CNN
	1    5400 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 1700 3250 1700
Wire Wire Line
	3550 1700 3700 1700
Wire Wire Line
	4000 1900 4000 2000
Wire Wire Line
	4600 2000 4600 1850
Wire Wire Line
	5000 2000 5000 1850
Wire Wire Line
	5400 1850 5400 2200
Wire Wire Line
	5400 2600 5400 2550
Wire Wire Line
	4000 1500 4000 1350
Wire Wire Line
	4000 1050 4000 900 
Wire Wire Line
	4150 1500 4000 1500
Connection ~ 4000 1500
Wire Wire Line
	4450 1500 4600 1500
Wire Wire Line
	5400 1500 5400 1550
Wire Wire Line
	5000 1550 5000 1500
Connection ~ 5000 1500
Wire Wire Line
	5000 1500 5400 1500
Wire Wire Line
	4600 1550 4600 1500
Connection ~ 4600 1500
Wire Wire Line
	4600 1500 5000 1500
Wire Wire Line
	2800 2200 5400 2200
Connection ~ 5400 2200
Wire Wire Line
	5400 2200 5400 2250
Text GLabel 5500 1400 2    50   UnSpc ~ 0
VPP_RAW
Wire Wire Line
	5400 1500 5400 1400
Wire Wire Line
	5400 1400 5500 1400
Connection ~ 5400 1500
Text Label 2800 1700 0    50   ~ 0
VPP_PWM
Text Label 2800 2200 0    50   ~ 0
VPP_FB
Text Notes 4700 2000 0    50   ~ 0
50V
Text HLabel 2500 2400 0    50   Input ~ 0
VPP_PWM
Entry Wire Line
	2600 2400 2700 2500
Entry Wire Line
	2600 2500 2700 2600
Wire Wire Line
	2500 2400 2600 2400
Text HLabel 2500 2500 0    50   Output ~ 0
VPP_FB
Wire Wire Line
	2500 2500 2600 2500
Entry Wire Line
	2700 3000 2800 3100
$Comp
L Device:LED D?
U 1 1 6265B7A1
P 3200 3700
F 0 "D?" V 3239 3582 50  0000 R CNN
F 1 "YELLOW" V 3148 3582 50  0000 R CNN
F 2 "" H 3200 3700 50  0001 C CNN
F 3 "~" H 3200 3700 50  0001 C CNN
	1    3200 3700
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_BJT:BC327 Q?
U 1 1 6265C36A
P 4500 3100
F 0 "Q?" H 4691 3054 50  0000 L CNN
F 1 "BC327" H 4691 3145 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 4700 3025 50  0001 L CIN
F 3 "http://www.onsemi.com/pub_link/Collateral/BC327-D.PDF" H 4500 3100 50  0001 L CNN
	1    4500 3100
	1    0    0    1   
$EndComp
$Comp
L 74xx:74LS06N U?
U 1 1 6265CE94
P 3600 3100
F 0 "U?" H 3600 3417 50  0000 C CNN
F 1 "74LS06N" H 3600 3326 50  0000 C CNN
F 2 "" H 3600 3100 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06N" H 3600 3100 50  0001 C CNN
	1    3600 3100
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS06N U?
U 7 1 6265DC51
P 1300 4400
F 0 "U?" H 1530 4446 50  0000 L CNN
F 1 "74LS06N" H 1530 4355 50  0000 L CNN
F 2 "" H 1300 4400 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74LS06N" H 1300 4400 50  0001 C CNN
	7    1300 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 6265EEA8
P 4100 3100
F 0 "R?" V 3893 3100 50  0000 C CNN
F 1 "4K7" V 3984 3100 50  0000 C CNN
F 2 "" V 4030 3100 50  0001 C CNN
F 3 "~" H 4100 3100 50  0001 C CNN
	1    4100 3100
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 6266293B
P 3200 3300
F 0 "R?" H 3270 3346 50  0000 L CNN
F 1 "100R" H 3270 3255 50  0000 L CNN
F 2 "" V 3130 3300 50  0001 C CNN
F 3 "~" H 3200 3300 50  0001 C CNN
	1    3200 3300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62663333
P 3200 3900
F 0 "#PWR?" H 3200 3650 50  0001 C CNN
F 1 "GND" H 3205 3727 50  0001 C CNN
F 2 "" H 3200 3900 50  0001 C CNN
F 3 "" H 3200 3900 50  0001 C CNN
	1    3200 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 62665933
P 4300 2900
F 0 "R?" H 4370 2946 50  0000 L CNN
F 1 "2K2" H 4370 2855 50  0000 L CNN
F 2 "" V 4230 2900 50  0001 C CNN
F 3 "~" H 4300 2900 50  0001 C CNN
	1    4300 2900
	1    0    0    -1  
$EndComp
Text GLabel 4700 2700 2    50   UnSpc ~ 0
VPP_RAW
$Comp
L Device:R R?
U 1 1 626666AA
P 4600 3700
F 0 "R?" H 4670 3746 50  0000 L CNN
F 1 "4K7" H 4670 3655 50  0000 L CNN
F 2 "" V 4530 3700 50  0001 C CNN
F 3 "~" H 4600 3700 50  0001 C CNN
	1    4600 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6266704F
P 4600 3900
F 0 "#PWR?" H 4600 3650 50  0001 C CNN
F 1 "GND" H 4605 3727 50  0001 C CNN
F 2 "" H 4600 3900 50  0001 C CNN
F 3 "" H 4600 3900 50  0001 C CNN
	1    4600 3900
	1    0    0    -1  
$EndComp
$Comp
L power:VPP #PWR?
U 1 1 62667823
P 5400 3300
F 0 "#PWR?" H 5400 3150 50  0001 C CNN
F 1 "VPP" H 5415 3473 50  0000 C CNN
F 2 "" H 5400 3300 50  0001 C CNN
F 3 "" H 5400 3300 50  0001 C CNN
	1    5400 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 3300 5400 3400
Wire Wire Line
	5400 3400 4600 3400
Wire Wire Line
	4600 3400 4600 3300
Connection ~ 4600 3400
Wire Wire Line
	4600 2900 4600 2700
Wire Wire Line
	4600 2700 4700 2700
Wire Wire Line
	3300 3100 3200 3100
Text Label 2800 3100 0    50   ~ 0
VPP_CTRL
Text HLabel 2500 2700 0    50   Input ~ 0
VPP_CTRL
Entry Wire Line
	2600 2700 2700 2800
Wire Wire Line
	2500 2700 2600 2700
Text Notes 3324 3875 0    50   ~ 0
VPP
Wire Wire Line
	3200 3150 3200 3100
Connection ~ 3200 3100
Wire Wire Line
	3200 3100 2800 3100
Wire Wire Line
	3200 3450 3200 3550
Wire Wire Line
	3200 3850 3200 3900
Wire Wire Line
	3900 3100 3950 3100
Wire Wire Line
	4250 3100 4300 3100
Wire Wire Line
	4300 3050 4300 3100
Connection ~ 4300 3100
Wire Wire Line
	4300 2750 4300 2700
Wire Wire Line
	4300 2700 4600 2700
Connection ~ 4600 2700
Wire Wire Line
	4600 3400 4600 3550
Wire Wire Line
	4600 3850 4600 3900
$Comp
L power:GND #PWR?
U 1 1 6267262F
P 1300 5000
F 0 "#PWR?" H 1300 4750 50  0001 C CNN
F 1 "GND" H 1305 4827 50  0001 C CNN
F 2 "" H 1300 5000 50  0001 C CNN
F 3 "" H 1300 5000 50  0001 C CNN
	1    1300 5000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 62672922
P 2100 4400
F 0 "C?" H 2200 4300 50  0000 L CNN
F 1 "100nF" H 2199 4224 50  0000 L CNN
F 2 "" H 2138 4250 50  0001 C CNN
F 3 "~" H 2100 4400 50  0001 C CNN
	1    2100 4400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 6267329B
P 1300 3800
F 0 "#PWR?" H 1300 3650 50  0001 C CNN
F 1 "+5V" H 1315 3973 50  0000 C CNN
F 2 "" H 1300 3800 50  0001 C CNN
F 3 "" H 1300 3800 50  0001 C CNN
	1    1300 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 3900 1300 3800
Wire Wire Line
	1300 4900 1300 5000
Wire Wire Line
	1300 3900 2100 3900
Wire Wire Line
	2100 3900 2100 4250
Connection ~ 1300 3900
Wire Wire Line
	2100 4550 2100 4900
Wire Wire Line
	2100 4900 1300 4900
Connection ~ 1300 4900
Wire Bus Line
	2700 1500 2700 3200
$EndSCHEMATC