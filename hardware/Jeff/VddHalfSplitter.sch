EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 8
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
U 1 1 621A3701
P 6200 4300
F 0 "U?" H 6175 4675 50  0000 C CNN
F 1 "LM2662" H 6175 4584 50  0000 C CNN
F 2 "Package_SO:SO-8_3.9x4.9mm_P1.27mm" H 6250 4050 50  0001 C CNN
F 3 "" H 6100 4250 50  0001 C CNN
	1    6200 4300
	1    0    0    -1  
$EndComp
Text HLabel 5300 3850 0    50   Input ~ 0
Vdd
Wire Wire Line
	5300 3850 5500 3850
Text Label 5500 3850 0    50   ~ 0
Vdd
Text Label 5650 4150 2    50   ~ 0
Vdd
Text Label 6700 4150 0    50   ~ 0
Vdd
$Comp
L power:GND #PWR?
U 1 1 621A47EE
P 6750 4550
F 0 "#PWR?" H 6750 4300 50  0001 C CNN
F 1 "GND" H 6755 4377 50  0000 C CNN
F 2 "" H 6750 4550 50  0001 C CNN
F 3 "" H 6750 4550 50  0001 C CNN
	1    6750 4550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 621A4CB2
P 5600 5000
F 0 "#PWR?" H 5600 4750 50  0001 C CNN
F 1 "GND" H 5605 4827 50  0000 C CNN
F 2 "" H 5600 5000 50  0001 C CNN
F 3 "" H 5600 5000 50  0001 C CNN
	1    5600 5000
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1 C?
U 1 1 621A5319
P 5350 4350
F 0 "C?" H 5150 4400 50  0000 L CNN
F 1 "47u" H 5100 4250 50  0000 L CNN
F 2 "" H 5350 4350 50  0001 C CNN
F 3 "~" H 5350 4350 50  0001 C CNN
	1    5350 4350
	1    0    0    -1  
$EndComp
$Comp
L Device:CP1 C?
U 1 1 621A6462
P 5600 4850
F 0 "C?" H 5715 4896 50  0000 L CNN
F 1 "47u" H 5715 4805 50  0000 L CNN
F 2 "" H 5600 4850 50  0001 C CNN
F 3 "~" H 5600 4850 50  0001 C CNN
	1    5600 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 4250 5450 4250
Wire Wire Line
	5450 4250 5450 4200
Wire Wire Line
	5450 4200 5350 4200
Wire Wire Line
	5350 4500 5450 4500
Wire Wire Line
	5450 4500 5450 4450
Wire Wire Line
	5450 4450 5750 4450
Wire Wire Line
	5750 4350 5600 4350
Wire Wire Line
	5600 4350 5600 4650
Wire Wire Line
	5600 4650 6100 4650
Connection ~ 5600 4650
Wire Wire Line
	5600 4650 5600 4700
Text HLabel 6100 4650 2    50   Output ~ 0
Vdd_half
Wire Wire Line
	5750 4150 5650 4150
Wire Wire Line
	6600 4150 6700 4150
Wire Wire Line
	6600 4350 6750 4350
Wire Wire Line
	6750 4350 6750 4450
Wire Wire Line
	6600 4450 6750 4450
Connection ~ 6750 4450
Wire Wire Line
	6750 4450 6750 4550
$EndSCHEMATC
