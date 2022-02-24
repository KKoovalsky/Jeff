EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 8 8
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
L pspice:OPAMP U?
U 1 1 621FD5D1
P 6200 3950
F 0 "U?" H 6544 3996 50  0000 L CNN
F 1 "OPAMP" H 6544 3905 50  0000 L CNN
F 2 "" H 6200 3950 50  0001 C CNN
F 3 "~" H 6200 3950 50  0001 C CNN
	1    6200 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 621FE496
P 5350 3400
F 0 "R?" H 5420 3446 50  0000 L CNN
F 1 "100k" H 5420 3355 50  0000 L CNN
F 2 "" V 5280 3400 50  0001 C CNN
F 3 "~" H 5350 3400 50  0001 C CNN
	1    5350 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 621FEC7E
P 5350 3800
F 0 "R?" H 5420 3846 50  0000 L CNN
F 1 "100k" H 5420 3755 50  0000 L CNN
F 2 "" V 5280 3800 50  0001 C CNN
F 3 "~" H 5350 3800 50  0001 C CNN
	1    5350 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 3650 5350 3600
$Comp
L power:GND #PWR?
U 1 1 621FEE1B
P 5350 4000
F 0 "#PWR?" H 5350 3750 50  0001 C CNN
F 1 "GND" H 5355 3827 50  0000 C CNN
F 2 "" H 5350 4000 50  0001 C CNN
F 3 "" H 5350 4000 50  0001 C CNN
	1    5350 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 3950 5350 4000
Text HLabel 5200 3100 0    50   Input ~ 0
Vdd_half
Wire Wire Line
	5200 3100 5350 3100
Wire Wire Line
	5350 3100 5350 3250
Text Label 5350 3100 0    50   ~ 0
Vref
Wire Wire Line
	5900 3850 5900 3600
Wire Wire Line
	5900 3600 5350 3600
Connection ~ 5350 3600
Wire Wire Line
	5350 3600 5350 3550
$Comp
L Device:R R?
U 1 1 6220013C
P 5300 4600
F 0 "R?" H 5370 4646 50  0000 L CNN
F 1 "100k" H 5370 4555 50  0000 L CNN
F 2 "" V 5230 4600 50  0001 C CNN
F 3 "~" H 5300 4600 50  0001 C CNN
	1    5300 4600
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 622003D2
P 6200 4850
F 0 "R?" H 6270 4896 50  0000 L CNN
F 1 "100k" H 6270 4805 50  0000 L CNN
F 2 "" V 6130 4850 50  0001 C CNN
F 3 "~" H 6200 4850 50  0001 C CNN
	1    6200 4850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5900 4050 5600 4050
Wire Wire Line
	5600 4050 5600 4600
Connection ~ 5600 4600
Text HLabel 4900 4600 0    50   Input ~ 0
In
Text HLabel 7150 3950 2    50   Output ~ 0
Out
Wire Wire Line
	6850 3950 6500 3950
Wire Wire Line
	6850 3950 7150 3950
Connection ~ 6850 3950
Wire Wire Line
	4900 4600 5150 4600
Text HLabel 6100 4350 3    50   Input ~ 0
Vss
Wire Wire Line
	6100 4350 6100 4250
Wire Wire Line
	6850 4850 6350 4850
Wire Wire Line
	6850 3950 6850 4850
Wire Wire Line
	5600 4600 5450 4600
Wire Wire Line
	5600 4600 5600 4850
Wire Wire Line
	5600 4850 6050 4850
Text HLabel 6100 3450 1    50   Input ~ 0
Vdd
Wire Wire Line
	6100 3450 6100 3650
Text Notes 6500 3550 0    50   ~ 0
When all resistors are equal then:\nVout = Vin - Vref\nG = 1\nDC offset = - Vref
$EndSCHEMATC
