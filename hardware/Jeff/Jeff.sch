EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 7
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
P 1800 1800
F 0 "J?" H 1832 2125 50  0000 C CNN
F 1 "GuitarOutput" H 1832 2034 50  0000 C CNN
F 2 "" H 1800 1800 50  0001 C CNN
F 3 "~" H 1800 1800 50  0001 C CNN
	1    1800 1800
	1    0    0    -1  
$EndComp
$Sheet
S 2600 1350 550  900 
U 62166F90
F0 "GuitarInputBuffer" 50
F1 "GuitarInputBuffer.sch" 50
F2 "In" I L 2600 1800 50 
F3 "Out" O R 3150 1800 50 
F4 "Vss" I L 2600 2100 50 
F5 "Vdd" I L 2600 1500 50 
$EndSheet
$Comp
L power:GND #PWR?
U 1 1 6216A036
P 2250 1550
F 0 "#PWR?" H 2250 1300 50  0001 C CNN
F 1 "GND" H 2255 1377 50  0000 C CNN
F 2 "" H 2250 1550 50  0001 C CNN
F 3 "" H 2250 1550 50  0001 C CNN
	1    2250 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 1700 2150 1700
Wire Wire Line
	2150 1700 2150 1500
Wire Wire Line
	2600 1800 2000 1800
Wire Wire Line
	2250 1550 2250 1500
Wire Wire Line
	2250 1500 2150 1500
$Sheet
S 6850 3450 550  900 
U 6216C0D1
F0 "DcOffsetRemover" 50
F1 "DcOffsetRemover.sch" 50
$EndSheet
Text Notes 1450 2200 0    50   ~ 0
±500mV AC\n0V DC\n5-15 kOhm
$Sheet
S 4300 1350 550  900 
U 6216D512
F0 "DcOffsetApplier" 50
F1 "DcOffsetApplier.sch" 50
F2 "In" I L 4300 1800 50 
F3 "Vdd_half" I L 4300 1650 50 
F4 "Out" O R 4850 1800 50 
F5 "Vdd" I L 4300 1500 50 
$EndSheet
Wire Wire Line
	3150 1800 4300 1800
Text Notes 3200 2100 0    50   ~ 0
±500mV AC\n0V DC\nhigh output impedance
Text Notes 1100 1200 0    50   ~ 0
Guitar output voltage and output impedance\ndepends on the guitar.\nAssumed max. possible output voltage and\nsome output impedance range.
Text Notes 4900 2100 0    50   ~ 0
±1500mV AC\n+1.65 V DC (3.3V/2)\n~~(0:3V3) range
$Sheet
S 5950 1350 550  900 
U 621761CD
F0 "AudioLowPassFilter" 50
F1 "AudioLowPassFilter.sch" 50
F2 "Vdd" I L 5950 1500 50 
F3 "In" I L 5950 1800 50 
F4 "CLK" I L 5950 1950 50 
F5 "Out" O R 6500 1800 50 
$EndSheet
Wire Wire Line
	5950 1800 4850 1800
Wire Wire Line
	6500 1800 6950 1800
Text Label 6950 1800 0    50   ~ 0
ADC_In
Wire Notes Line
	1000 800  1000 2850
Wire Notes Line
	7850 2850 7850 800 
Text Notes 1100 2750 0    118  ~ 24
Analog Front End
$Comp
L power:+3.3V #PWR?
U 1 1 6219044E
P 2450 1450
F 0 "#PWR?" H 2450 1300 50  0001 C CNN
F 1 "+3.3V" H 2465 1623 50  0000 C CNN
F 2 "" H 2450 1450 50  0001 C CNN
F 3 "" H 2450 1450 50  0001 C CNN
	1    2450 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 1500 2450 1500
Wire Wire Line
	2450 1500 2450 1450
$Comp
L power:+3.3V #PWR?
U 1 1 62191985
P 4150 1450
F 0 "#PWR?" H 4150 1300 50  0001 C CNN
F 1 "+3.3V" H 4165 1623 50  0000 C CNN
F 2 "" H 4150 1450 50  0001 C CNN
F 3 "" H 4150 1450 50  0001 C CNN
	1    4150 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 1500 4150 1500
Wire Wire Line
	4150 1500 4150 1450
$Comp
L power:+3.3V #PWR?
U 1 1 621932B2
P 5800 1450
F 0 "#PWR?" H 5800 1300 50  0001 C CNN
F 1 "+3.3V" H 5815 1623 50  0000 C CNN
F 2 "" H 5800 1450 50  0001 C CNN
F 3 "" H 5800 1450 50  0001 C CNN
	1    5800 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 1500 5800 1500
Wire Wire Line
	5800 1500 5800 1450
$Sheet
S 1300 6650 750  200 
U 6219438A
F0 "NegativeVoltageGenerator" 50
F1 "NegativeVoltageGenerator.sch" 50
F2 "Vdd" I L 1300 6750 50 
F3 "-Vdd" O R 2050 6750 50 
$EndSheet
$Comp
L power:+3.3V #PWR?
U 1 1 6219F15D
P 1050 6650
F 0 "#PWR?" H 1050 6500 50  0001 C CNN
F 1 "+3.3V" H 1065 6823 50  0000 C CNN
F 2 "" H 1050 6650 50  0001 C CNN
F 3 "" H 1050 6650 50  0001 C CNN
	1    1050 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 6750 1050 6750
Wire Wire Line
	1050 6750 1050 6650
$Comp
L power:-3V3 #PWR?
U 1 1 6219FAA4
P 2250 7000
F 0 "#PWR?" H 2250 7100 50  0001 C CNN
F 1 "-3V3" H 2265 7173 50  0000 C CNN
F 2 "" H 2250 7000 50  0001 C CNN
F 3 "" H 2250 7000 50  0001 C CNN
	1    2250 7000
	-1   0    0    1   
$EndComp
Wire Wire Line
	2050 6750 2250 6750
Wire Wire Line
	2250 6750 2250 7000
$Comp
L power:-3V3 #PWR?
U 1 1 621A13F6
P 2400 2200
F 0 "#PWR?" H 2400 2300 50  0001 C CNN
F 1 "-3V3" H 2415 2373 50  0000 C CNN
F 2 "" H 2400 2200 50  0001 C CNN
F 3 "" H 2400 2200 50  0001 C CNN
	1    2400 2200
	-1   0    0    1   
$EndComp
Wire Wire Line
	2600 2100 2400 2100
Wire Wire Line
	2400 2100 2400 2200
$Sheet
S 3100 6650 750  200 
U 621A29F4
F0 "VddHalfSplitter" 50
F1 "VddHalfSplitter.sch" 50
F2 "Vdd" I L 3100 6750 50 
F3 "Vdd_half" O R 3850 6750 50 
$EndSheet
$Comp
L power:+3.3V #PWR?
U 1 1 621A9A4A
P 2850 6650
F 0 "#PWR?" H 2850 6500 50  0001 C CNN
F 1 "+3.3V" H 2865 6823 50  0000 C CNN
F 2 "" H 2850 6650 50  0001 C CNN
F 3 "" H 2850 6650 50  0001 C CNN
	1    2850 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 6750 2850 6750
Wire Wire Line
	2850 6750 2850 6650
$Comp
L PowerSources:+1V65 #PWR?
U 1 1 621AA734
P 4150 6650
F 0 "#PWR?" H 4150 6500 50  0001 C CNN
F 1 "+1V65" H 4165 6823 50  0000 C CNN
F 2 "" H 4150 6650 50  0001 C CNN
F 3 "" H 4150 6650 50  0001 C CNN
	1    4150 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 6650 4150 6750
Wire Wire Line
	4150 6750 3850 6750
$Comp
L PowerSources:+1V65 #PWR?
U 1 1 621AB60E
P 3900 1600
F 0 "#PWR?" H 3900 1450 50  0001 C CNN
F 1 "+1V65" H 3915 1773 50  0000 C CNN
F 2 "" H 3900 1600 50  0001 C CNN
F 3 "" H 3900 1600 50  0001 C CNN
	1    3900 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 1600 3900 1650
Wire Wire Line
	4300 1650 3900 1650
$Comp
L Nucleo-L432KC:NUCLEO-L432KC U?
U 1 1 621AE9B8
P 9550 1950
F 0 "U?" H 9550 2927 50  0000 C CNN
F 1 "NUCLEO-L432KC" H 9550 2836 50  0000 C CNN
F 2 "MODULE_NUCLEO-L432KC" H 9550 1950 50  0001 L BNN
F 3 "" H 9550 1950 50  0001 L BNN
F 4 "ST Microelectronics" H 9550 1950 50  0001 L BNN "MANUFACTURER"
F 5 "N/A" H 9550 1950 50  0001 L BNN "MAXIMUM_PACKAGE_HEIGHT"
F 6 "N/A" H 9550 1950 50  0001 L BNN "PARTREV"
F 7 "Manufacturer Recommendations" H 9550 1950 50  0001 L BNN "STANDARD"
	1    9550 1950
	1    0    0    -1  
$EndComp
$Comp
L Nucleo-L432KC:NUCLEO-L432KC U?
U 2 1 621B1CE7
P 9550 4150
F 0 "U?" H 9550 5127 50  0000 C CNN
F 1 "NUCLEO-L432KC" H 9550 5036 50  0000 C CNN
F 2 "MODULE_NUCLEO-L432KC" H 9550 4150 50  0001 L BNN
F 3 "" H 9550 4150 50  0001 L BNN
F 4 "ST Microelectronics" H 9550 4150 50  0001 L BNN "MANUFACTURER"
F 5 "N/A" H 9550 4150 50  0001 L BNN "MAXIMUM_PACKAGE_HEIGHT"
F 6 "N/A" H 9550 4150 50  0001 L BNN "PARTREV"
F 7 "Manufacturer Recommendations" H 9550 4150 50  0001 L BNN "STANDARD"
	2    9550 4150
	1    0    0    -1  
$EndComp
Wire Notes Line
	1000 800  7850 800 
Wire Notes Line
	7850 2850 1000 2850
Wire Notes Line
	8050 800  10950 800 
Wire Notes Line
	10950 800  10950 6200
Wire Notes Line
	10950 6150 8050 6150
Wire Notes Line
	8050 6150 8050 800 
Text Notes 8150 6050 0    118  ~ 24
Digital Signal Processing
Wire Wire Line
	10150 3650 10450 3650
Wire Wire Line
	10450 3650 10450 3500
$Comp
L power:+3.3V #PWR?
U 1 1 621C7894
P 10450 3500
F 0 "#PWR?" H 10450 3350 50  0001 C CNN
F 1 "+3.3V" H 10465 3673 50  0000 C CNN
F 2 "" H 10450 3500 50  0001 C CNN
F 3 "" H 10450 3500 50  0001 C CNN
	1    10450 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 621C878F
P 10350 4850
F 0 "#PWR?" H 10350 4600 50  0001 C CNN
F 1 "GND" H 10355 4677 50  0000 C CNN
F 2 "" H 10350 4850 50  0001 C CNN
F 3 "" H 10350 4850 50  0001 C CNN
	1    10350 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	10150 4750 10350 4750
Wire Wire Line
	10350 4750 10350 4850
Wire Wire Line
	5950 1950 5750 1950
Text Label 5700 2450 2    50   ~ 0
Filter_CLK
Text Label 8650 4250 2    50   ~ 0
ADC_In
Wire Wire Line
	8950 4250 8650 4250
Text Label 10300 4450 0    50   ~ 0
Filter_CLK
Wire Wire Line
	10300 4450 10150 4450
Wire Wire Line
	5750 2450 5700 2450
Wire Wire Line
	5750 1950 5750 2450
Text Notes 4400 2750 0    50   ~ 0
Filter cutoff frequency shall be 20kHz.\nThe clock, which sets the cutoff frequency shall have 100 times higher frequency.\nThe clock parameters are: 2MHz, 50% duty cycle.
$EndSCHEMATC
