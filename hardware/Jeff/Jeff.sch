EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
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
L Connector:AudioJack2 J?
U 1 1 62166804
P 3500 3700
F 0 "J?" H 3532 4025 50  0000 C CNN
F 1 "GuitarOutput" H 3532 3934 50  0000 C CNN
F 2 "" H 3500 3700 50  0001 C CNN
F 3 "~" H 3500 3700 50  0001 C CNN
	1    3500 3700
	1    0    0    -1  
$EndComp
$Sheet
S 4300 3250 550  900 
U 62166F90
F0 "GuitarInputBuffer" 50
F1 "GuitarInputBuffer.sch" 50
F2 "In" I L 4300 3700 50 
F3 "Out" O R 4850 3700 50 
F4 "Vss" I L 4300 4000 50 
F5 "Vdd" I L 4300 3400 50 
$EndSheet
$Comp
L power:GND #PWR?
U 1 1 6216A036
P 3950 3450
F 0 "#PWR?" H 3950 3200 50  0001 C CNN
F 1 "GND" H 3955 3277 50  0000 C CNN
F 2 "" H 3950 3450 50  0001 C CNN
F 3 "" H 3950 3450 50  0001 C CNN
	1    3950 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 3600 3850 3600
Wire Wire Line
	3850 3600 3850 3400
Wire Wire Line
	4300 3700 3700 3700
Wire Wire Line
	3950 3450 3950 3400
Wire Wire Line
	3950 3400 3850 3400
$Sheet
S 3850 4850 550  900 
U 6216C0D1
F0 "DcOffsetRemover" 50
F1 "DcOffsetRemover.sch" 50
$EndSheet
Text Notes 3150 4100 0    50   ~ 0
±500mV AC\n0V DC\n5-15 kOhm
$Sheet
S 6000 3250 550  900 
U 6216D512
F0 "DcOffsetApplier" 50
F1 "DcOffsetApplier.sch" 50
F2 "In" I L 6000 3700 50 
F3 "Vdd_half" I L 6000 3550 50 
F4 "Out" O R 6550 3700 50 
F5 "Vdd" I L 6000 3400 50 
$EndSheet
Wire Wire Line
	4850 3700 6000 3700
Text Notes 4900 4000 0    50   ~ 0
±500mV AC\n0V DC\nhigh output impedance
Text Notes 2650 3150 0    50   ~ 0
Guitar output voltage and output impedance\ndepends on the guitar.\nAssumed max. possible output voltage and\nsome output impedance range.
Text Notes 6600 4000 0    50   ~ 0
±1500mV AC\n+1.65 V DC (3.3V/2)\n~~(0:3V3) range
$EndSCHEMATC
