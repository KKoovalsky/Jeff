EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 7
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L SwitchedCapacitorVoltageConverters:LM2662 U?
U 1 1 62195855
P 6050 3500
F 0 "U?" H 6025 3875 50  0000 C CNN
F 1 "LM2662" H 6025 3784 50  0000 C CNN
F 2 "Package_SO:SO-8_3.9x4.9mm_P1.27mm" H 6100 3250 50  0001 C CNN
F 3 "" H 5950 3450 50  0001 C CNN
	1    6050 3500
	1    0    0    -1  
$EndComp
Text HLabel 5150 3000 0    50   Input ~ 0
Vdd
Wire Wire Line
	5150 3000 5400 3000
Text Label 5400 3000 0    50   ~ 0
Vdd
Text Label 5500 3350 2    50   ~ 0
Vdd
Text Label 6550 3350 0    50   ~ 0
Vdd
Wire Wire Line
	5600 3350 5500 3350
Wire Wire Line
	6450 3350 6550 3350
$Comp
L Device:CP1 C?
U 1 1 62196D66
P 5200 3550
F 0 "C?" H 5000 3600 50  0000 L CNN
F 1 "47u" H 4950 3500 50  0000 L CNN
F 2 "" H 5200 3550 50  0001 C CNN
F 3 "~" H 5200 3550 50  0001 C CNN
	1    5200 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 3450 5400 3450
Wire Wire Line
	5400 3450 5400 3400
Wire Wire Line
	5400 3400 5200 3400
Wire Wire Line
	5200 3700 5400 3700
Wire Wire Line
	5400 3700 5400 3650
Wire Wire Line
	5400 3650 5600 3650
$Comp
L power:GND #PWR?
U 1 1 621981ED
P 5550 3850
F 0 "#PWR?" H 5550 3600 50  0001 C CNN
F 1 "GND" H 5555 3677 50  0000 C CNN
F 2 "" H 5550 3850 50  0001 C CNN
F 3 "" H 5550 3850 50  0001 C CNN
	1    5550 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 3550 5550 3550
Wire Wire Line
	5550 3550 5550 3850
$Comp
L power:GND #PWR?
U 1 1 62199D03
P 6600 3800
F 0 "#PWR?" H 6600 3550 50  0001 C CNN
F 1 "GND" H 6605 3627 50  0000 C CNN
F 2 "" H 6600 3800 50  0001 C CNN
F 3 "" H 6600 3800 50  0001 C CNN
	1    6600 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 3550 6600 3550
Wire Wire Line
	6600 3550 6600 3800
$Comp
L Device:CP1 C?
U 1 1 6219B5A9
P 7000 3800
F 0 "C?" H 6800 3850 50  0000 L CNN
F 1 "47u" H 6750 3750 50  0000 L CNN
F 2 "" H 7000 3800 50  0001 C CNN
F 3 "~" H 7000 3800 50  0001 C CNN
	1    7000 3800
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6219BADA
P 7000 3950
F 0 "#PWR?" H 7000 3700 50  0001 C CNN
F 1 "GND" H 7005 3777 50  0000 C CNN
F 2 "" H 7000 3950 50  0001 C CNN
F 3 "" H 7000 3950 50  0001 C CNN
	1    7000 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 3650 6450 3650
Wire Wire Line
	7000 3650 7400 3650
Connection ~ 7000 3650
Text HLabel 7400 3650 2    50   Output ~ 0
-Vdd
Text Notes 6200 2850 0    50   ~ 0
Capacitors shall be low ESR.\nCapacitor values may be higher (e.g. 100u).\nThe lower the ESR, the lower the output voltage ripple.\nCharge pump programmed to 150 kHz oscillation frequency,\nto not interfere with audio.
$EndSCHEMATC
