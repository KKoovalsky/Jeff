EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 5
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
P 2150 1900
F 0 "J?" H 2182 2225 50  0000 C CNN
F 1 "GuitarOutput" H 2182 2134 50  0000 C CNN
F 2 "" H 2150 1900 50  0001 C CNN
F 3 "~" H 2150 1900 50  0001 C CNN
	1    2150 1900
	1    0    0    -1  
$EndComp
$Sheet
S 2950 1450 550  900 
U 62166F90
F0 "GuitarInputBuffer" 50
F1 "GuitarInputBuffer.sch" 50
F2 "In" I L 2950 1900 50 
F3 "Out" O R 3500 1900 50 
F4 "Vss" I L 2950 2200 50 
F5 "Vdd" I L 2950 1600 50 
$EndSheet
$Comp
L power:GND #PWR?
U 1 1 6216A036
P 2600 1650
F 0 "#PWR?" H 2600 1400 50  0001 C CNN
F 1 "GND" H 2605 1477 50  0000 C CNN
F 2 "" H 2600 1650 50  0001 C CNN
F 3 "" H 2600 1650 50  0001 C CNN
	1    2600 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 1800 2500 1800
Wire Wire Line
	2500 1800 2500 1600
Wire Wire Line
	2950 1900 2350 1900
Wire Wire Line
	2600 1650 2600 1600
Wire Wire Line
	2600 1600 2500 1600
$Sheet
S 3850 4850 550  900 
U 6216C0D1
F0 "DcOffsetRemover" 50
F1 "DcOffsetRemover.sch" 50
$EndSheet
Text Notes 1800 2300 0    50   ~ 0
±500mV AC\n0V DC\n5-15 kOhm
$Sheet
S 4650 1450 550  900 
U 6216D512
F0 "DcOffsetApplier" 50
F1 "DcOffsetApplier.sch" 50
F2 "In" I L 4650 1900 50 
F3 "Vdd_half" I L 4650 1750 50 
F4 "Out" O R 5200 1900 50 
F5 "Vdd" I L 4650 1600 50 
$EndSheet
Wire Wire Line
	3500 1900 4650 1900
Text Notes 3550 2200 0    50   ~ 0
±500mV AC\n0V DC\nhigh output impedance
Text Notes 1300 1350 0    50   ~ 0
Guitar output voltage and output impedance\ndepends on the guitar.\nAssumed max. possible output voltage and\nsome output impedance range.
Text Notes 5250 2200 0    50   ~ 0
±1500mV AC\n+1.65 V DC (3.3V/2)\n~~(0:3V3) range
$Sheet
S 6300 1450 550  900 
U 621761CD
F0 "AudioLowPassFilter" 50
F1 "AudioLowPassFilter.sch" 50
F2 "Vdd" I L 6300 1600 50 
F3 "In" I L 6300 1900 50 
F4 "CLK" I L 6300 2050 50 
F5 "Out" O R 6850 1900 50 
$EndSheet
Wire Wire Line
	6300 1900 5200 1900
Wire Wire Line
	6850 1900 7300 1900
Text Label 7300 1900 0    50   ~ 0
ADC_In
Wire Notes Line
	1000 800  1000 2850
Wire Notes Line
	1000 2850 8250 2850
Wire Notes Line
	8250 2850 8250 800 
Wire Notes Line
	8250 800  1000 800 
Text Notes 1100 2750 0    118  ~ 24
Analog Front End
$EndSCHEMATC
