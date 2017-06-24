EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:custom_components_library
LIBS:trackingDevice_circuitBoard-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Arduino GPS Tracker - Tracking Device"
Date "2017-06-04"
Rev "1"
Comp "By Megasaturnv"
Comment1 "For Veroboard / Stripboard"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 10350 1500 1    60   ~ 0
RAW
Text Label 9550 1000 1    60   ~ 0
DTR
Text Label 9750 1000 1    60   ~ 0
0(Rx)
Text Label 9000 1600 0    60   ~ 0
1(Tx)
Text Label 9000 1700 0    60   ~ 0
0(Rx)
Text Label 9000 1800 0    60   ~ 0
Reset
Text Label 10500 1800 0    60   ~ 0
Reset
Text Label 9000 2000 0    60   ~ 0
2
Text Label 9000 2100 0    60   ~ 0
3(**)
Text Label 9000 2200 0    60   ~ 0
4
Text Label 9000 2300 0    60   ~ 0
5(**)
Text Label 9000 2400 0    60   ~ 0
6(**)
Text Label 9000 2500 0    60   ~ 0
7
Text Label 9000 2600 0    60   ~ 0
8
Text Label 9000 2700 0    60   ~ 0
9(**)
Text Label 10500 2700 0    60   ~ 0
10(**/SS)
Text Label 10500 2600 0    60   ~ 0
11(**/MOSI)
Text Label 10500 2500 0    60   ~ 0
12(MISO)
Text Label 10500 2400 0    60   ~ 0
13(SCK)
Text Label 10500 2300 0    60   ~ 0
A0
Text Label 10500 2200 0    60   ~ 0
A1
Text Label 10500 2100 0    60   ~ 0
A2
Text Label 10500 2000 0    60   ~ 0
A3
Text Notes 8550 575  0    60   ~ 0
Shield Arduino Pro Mini \n
$Comp
L CONN_01X06 P2
U 1 1 56D75238
P 9800 1200
F 0 "P2" H 9800 1550 50  0000 C CNN
F 1 "COM" V 9900 1200 50  0000 C CNN
F 2 "" H 9800 1200 50  0001 C CNN
F 3 "" H 9800 1200 50  0000 C CNN
	1    9800 1200
	0    -1   1    0   
$EndComp
Text Label 9650 1000 1    60   ~ 0
1(Tx)
$Comp
L GND #PWR01
U 1 1 56D7539A
P 10200 1000
F 0 "#PWR01" H 10200 750 50  0001 C CNN
F 1 "GND" H 10200 850 50  0000 C CNN
F 2 "" H 10200 1000 50  0000 C CNN
F 3 "" H 10200 1000 50  0000 C CNN
	1    10200 1000
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR02
U 1 1 56D753B8
P 9850 1000
F 0 "#PWR02" H 9850 850 50  0001 C CNN
F 1 "VCC" H 9850 1150 50  0000 C CNN
F 2 "" H 9850 1000 50  0000 C CNN
F 3 "" H 9850 1000 50  0000 C CNN
	1    9850 1000
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X12 P1
U 1 1 56D754D1
P 9550 2150
F 0 "P1" H 9550 2800 50  0000 C CNN
F 1 "Digital" V 9650 2150 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x12_Pitch2.54mm" H 9550 2150 50  0001 C CNN
F 3 "" H 9550 2150 50  0000 C CNN
	1    9550 2150
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X12 P4
U 1 1 56D755F3
P 10050 2150
F 0 "P4" H 10050 2800 50  0000 C CNN
F 1 "Analog" V 10150 2150 50  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x12_Pitch2.54mm" H 10050 2150 50  0001 C CNN
F 3 "" H 10050 2150 50  0000 C CNN
	1    10050 2150
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 56D756B8
P 9250 2800
F 0 "#PWR03" H 9250 2550 50  0001 C CNN
F 1 "GND" H 9250 2650 50  0000 C CNN
F 2 "" H 9250 2800 50  0000 C CNN
F 3 "" H 9250 2800 50  0000 C CNN
	1    9250 2800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 56D75A03
P 10350 2800
F 0 "#PWR04" H 10350 2550 50  0001 C CNN
F 1 "GND" H 10350 2650 50  0000 C CNN
F 2 "" H 10350 2800 50  0000 C CNN
F 3 "" H 10350 2800 50  0000 C CNN
	1    10350 2800
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR05
U 1 1 56D75A9D
P 10450 1500
F 0 "#PWR05" H 10450 1350 50  0001 C CNN
F 1 "VCC" H 10450 1650 50  0000 C CNN
F 2 "" H 10450 1500 50  0000 C CNN
F 3 "" H 10450 1500 50  0000 C CNN
	1    10450 1500
	1    0    0    -1  
$EndComp
Text Notes 9650 1600 0    60   ~ 0
1
NoConn ~ 9000 2700
NoConn ~ 9000 2600
NoConn ~ 9000 2500
NoConn ~ 9000 2400
NoConn ~ 9000 2300
NoConn ~ 10500 2700
NoConn ~ 10500 2600
NoConn ~ 10500 2500
NoConn ~ 10500 2400
NoConn ~ 10500 2300
NoConn ~ 10500 2200
NoConn ~ 10500 2100
NoConn ~ 10500 2000
$Comp
L GY-NEO6MV2 U2
U 1 1 592C7D71
P 1600 2000
F 0 "U2" H 1650 1300 60  0000 C CNN
F 1 "GY-NEO6MV2" H 1650 1400 60  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x04_Pitch2.54mm" H 1600 2000 60  0001 C CNN
F 3 "" H 1600 2000 60  0001 C CNN
	1    1600 2000
	0    1    1    0   
$EndComp
Text Label 3100 2900 0    60   ~ 0
2
$Comp
L HC-12 U1
U 1 1 592C8583
P 1600 1100
F 0 "U1" H 1600 700 60  0000 C CNN
F 1 "HC-12" H 1600 800 60  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x05_Pitch2.54mm" H 1600 1100 60  0001 C CNN
F 3 "" H 1600 1100 60  0001 C CNN
	1    1600 1100
	0    1    1    0   
$EndComp
Text Label 2000 1100 0    60   ~ 0
1(Tx)
Text Label 2000 1000 0    60   ~ 0
0(Rx)
NoConn ~ 2000 900 
Text Label 3600 3600 0    60   ~ 0
3(**)
Text Label 4350 2500 0    60   ~ 0
4
$Comp
L GND #PWR06
U 1 1 592C9BB4
P 4800 2200
F 0 "#PWR06" H 4800 1950 50  0001 C CNN
F 1 "GND" H 4800 2050 50  0000 C CNN
F 2 "" H 4800 2200 50  0001 C CNN
F 3 "" H 4800 2200 50  0001 C CNN
	1    4800 2200
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR07
U 1 1 592CA29E
P 2000 1900
F 0 "#PWR07" H 2000 1750 50  0001 C CNN
F 1 "VCC" H 2000 2050 50  0000 C CNN
F 2 "" H 2000 1900 50  0000 C CNN
F 3 "" H 2000 1900 50  0000 C CNN
	1    2000 1900
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR08
U 1 1 592CA46F
P 2000 1300
F 0 "#PWR08" H 2000 1150 50  0001 C CNN
F 1 "VCC" H 2000 1450 50  0000 C CNN
F 2 "" H 2000 1300 50  0000 C CNN
F 3 "" H 2000 1300 50  0000 C CNN
	1    2000 1300
	0    1    1    0   
$EndComp
NoConn ~ 9550 1000
$Comp
L Battery_Cell BT1
U 1 1 592CB467
P 6800 1500
F 0 "BT1" H 6900 1600 50  0000 L CNN
F 1 "Battery_Cell" H 6900 1500 50  0000 L CNN
F 2 "Capacitors_THT:CP_Axial_L67.0mm_D23.0mm_P75.00mm_Horizontal" V 6800 1560 50  0001 C CNN
F 3 "" V 6800 1560 50  0001 C CNN
	1    6800 1500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 592CBF25
P 7100 1700
F 0 "#PWR09" H 7100 1450 50  0001 C CNN
F 1 "GND" H 7100 1550 50  0000 C CNN
F 2 "" H 7100 1700 50  0001 C CNN
F 3 "" H 7100 1700 50  0001 C CNN
	1    7100 1700
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR010
U 1 1 592CBF54
P 7100 1200
F 0 "#PWR010" H 7100 1050 50  0001 C CNN
F 1 "VCC" H 7100 1350 50  0000 C CNN
F 2 "" H 7100 1200 50  0001 C CNN
F 3 "" H 7100 1200 50  0001 C CNN
	1    7100 1200
	0    1    1    0   
$EndComp
$Comp
L TP4056 U3
U 1 1 592CC948
P 6100 1400
F 0 "U3" H 6150 1000 60  0000 C CNN
F 1 "TP4056" H 6150 1100 60  0000 C CNN
F 2 "custom_components_library:TP4056_with_protection" H 6100 1400 60  0001 C CNN
F 3 "" H 6100 1400 60  0001 C CNN
	1    6100 1400
	0    1    1    0   
$EndComp
$Comp
L R R1
U 1 1 592CD0A2
P 2900 2900
F 0 "R1" V 2980 2900 50  0000 C CNN
F 1 "1000R" V 2900 2900 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2830 2900 50  0001 C CNN
F 3 "" H 2900 2900 50  0001 C CNN
	1    2900 2900
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 592CD0ED
P 2500 2900
F 0 "R2" V 2580 2900 50  0000 C CNN
F 1 "2000R" V 2500 2900 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2430 2900 50  0001 C CNN
F 3 "" H 2500 2900 50  0001 C CNN
	1    2500 2900
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 592CDBF3
P 2300 4000
F 0 "R3" V 2380 4000 50  0000 C CNN
F 1 "1000R" V 2300 4000 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2230 4000 50  0001 C CNN
F 3 "" H 2300 4000 50  0001 C CNN
	1    2300 4000
	0    1    1    0   
$EndComp
$Comp
L R R4
U 1 1 592CDC33
P 3000 3800
F 0 "R4" V 3080 3800 50  0000 C CNN
F 1 "1000R" V 3000 3800 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2930 3800 50  0001 C CNN
F 3 "" H 3000 3800 50  0001 C CNN
	1    3000 3800
	0    1    1    0   
$EndComp
$Comp
L R R5
U 1 1 592CDCA0
P 2800 3600
F 0 "R5" V 2880 3600 50  0000 C CNN
F 1 "1000R" V 2800 3600 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2730 3600 50  0001 C CNN
F 3 "" H 2800 3600 50  0001 C CNN
	1    2800 3600
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 592CDD04
P 3500 3400
F 0 "R6" V 3580 3400 50  0000 C CNN
F 1 "1000R" V 3500 3400 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3430 3400 50  0001 C CNN
F 3 "" H 3500 3400 50  0001 C CNN
	1    3500 3400
	1    0    0    -1  
$EndComp
$Comp
L Q_NPN_EBC Q2
U 1 1 592CE2BB
P 2700 4000
F 0 "Q2" H 2900 4050 50  0000 L CNN
F 1 "Q_NPN_EBC" H 2900 3950 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Inline_Wide" H 2900 4100 50  0001 C CNN
F 3 "" H 2700 4000 50  0001 C CNN
	1    2700 4000
	1    0    0    -1  
$EndComp
$Comp
L Q_NPN_EBC Q3
U 1 1 592CE337
P 3400 3800
F 0 "Q3" H 3600 3850 50  0000 L CNN
F 1 "Q_NPN_EBC" H 3600 3750 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Inline_Wide" H 3600 3900 50  0001 C CNN
F 3 "" H 3400 3800 50  0001 C CNN
	1    3400 3800
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR011
U 1 1 592CEA4D
P 2700 3200
F 0 "#PWR011" H 2700 3050 50  0001 C CNN
F 1 "VCC" H 2700 3350 50  0000 C CNN
F 2 "" H 2700 3200 50  0001 C CNN
F 3 "" H 2700 3200 50  0001 C CNN
	1    2700 3200
	0    -1   -1   0   
$EndComp
Wire Notes Line
	8525 650  9700 650 
Wire Notes Line
	9700 650  9700 475 
Wire Wire Line
	9950 1000 10200 1000
Connection ~ 10050 1000
Wire Wire Line
	9350 1900 9250 1900
Wire Wire Line
	9250 1900 9250 2800
Wire Wire Line
	9350 1600 9000 1600
Wire Wire Line
	9000 1700 9350 1700
Wire Wire Line
	9350 1800 9000 1800
Wire Wire Line
	9000 2000 9350 2000
Wire Wire Line
	9350 2100 9000 2100
Wire Wire Line
	9350 2200 9000 2200
Wire Wire Line
	9000 2300 9350 2300
Wire Wire Line
	9350 2400 9000 2400
Wire Wire Line
	9000 2500 9350 2500
Wire Wire Line
	9350 2600 9000 2600
Wire Wire Line
	9350 2700 9000 2700
Wire Wire Line
	10350 1500 10350 1600
Wire Wire Line
	10350 1600 10250 1600
Wire Wire Line
	10250 1700 10350 1700
Wire Wire Line
	10350 1700 10350 2800
Wire Wire Line
	10250 1900 10450 1900
Wire Wire Line
	10450 1900 10450 1500
Wire Wire Line
	10250 1800 10500 1800
Wire Wire Line
	10500 2000 10250 2000
Wire Wire Line
	10250 2100 10500 2100
Wire Wire Line
	10500 2200 10250 2200
Wire Wire Line
	10250 2300 10500 2300
Wire Wire Line
	10500 2400 10250 2400
Wire Wire Line
	10250 2500 10500 2500
Wire Wire Line
	10500 2600 10250 2600
Wire Wire Line
	10250 2700 10500 2700
Wire Notes Line
	11200 3400 8500 3400
Wire Notes Line
	8500 3400 8500 500 
Wire Wire Line
	4500 2200 4800 2200
Wire Wire Line
	6800 1600 6500 1600
Wire Wire Line
	6800 1300 6500 1300
Wire Wire Line
	6500 1200 7100 1200
Wire Wire Line
	6500 1700 7100 1700
Wire Wire Line
	2000 2100 2100 2100
Wire Wire Line
	2100 2100 2100 4000
Wire Wire Line
	2000 2000 2700 2000
Wire Wire Line
	3500 4300 3500 4000
Wire Wire Line
	2700 3200 3500 3200
Wire Wire Line
	2800 3200 2800 3450
Connection ~ 2800 3200
Wire Wire Line
	2800 4300 2800 4200
Wire Wire Line
	2450 4000 2500 4000
Wire Wire Line
	2800 3750 2800 3800
Wire Wire Line
	2800 3800 2850 3800
Wire Wire Line
	3200 3800 3150 3800
Wire Wire Line
	3500 3600 3500 3550
Wire Wire Line
	3500 3200 3500 3250
Wire Wire Line
	3500 3600 3600 3600
Wire Wire Line
	2300 2900 2350 2900
Wire Wire Line
	2650 2900 2750 2900
Connection ~ 2700 2900
Wire Wire Line
	3050 2900 3100 2900
Wire Wire Line
	2700 2000 2700 2900
Wire Wire Line
	2100 4000 2150 4000
$Comp
L GND #PWR012
U 1 1 592D4392
P 2200 1200
F 0 "#PWR012" H 2200 950 50  0001 C CNN
F 1 "GND" H 2200 1050 50  0000 C CNN
F 2 "" H 2200 1200 50  0001 C CNN
F 3 "" H 2200 1200 50  0001 C CNN
	1    2200 1200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2000 1200 2200 1200
$Comp
L 2N7000 Q1
U 1 1 59346998
P 4300 2300
F 0 "Q1" H 4500 2375 50  0000 L CNN
F 1 "2N7000" H 4500 2300 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Inline_Wide" H 4500 2225 50  0001 L CIN
F 3 "" H 4300 2300 50  0001 L CNN
	1    4300 2300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2800 4300 4050 4300
Wire Wire Line
	2300 2900 2300 2200
Connection ~ 2300 2200
Wire Wire Line
	4050 2200 4050 4300
Connection ~ 3500 4300
Connection ~ 4050 2200
NoConn ~ 10350 1500
Text Label 3200 2200 0    60   ~ 0
GPS_GND
Wire Wire Line
	2000 2200 4100 2200
$EndSCHEMATC
