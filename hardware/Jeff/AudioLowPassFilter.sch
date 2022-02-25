EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 8
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
L SCF_filters:MAX7424 U?
U 1 1 62178EF6
P 5800 3950
AR Path="/621761CD/62178EF6" Ref="U?"  Part="1" 
AR Path="/621EA432/62178EF6" Ref="U?"  Part="1" 
F 0 "U?" H 5875 4375 50  0000 C CNN
F 1 "MAX7424" H 5875 4284 50  0000 C CNN
F 2 "Package_SO:MSOP-8_3x3mm_P0.65mm" H 5900 3750 50  0001 C CNN
F 3 "" H 5800 3900 50  0001 C CNN
	1    5800 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 3750 5100 3750
$Comp
L Device:C C?
U 1 1 6217ACC0
P 6900 4100
AR Path="/621761CD/6217ACC0" Ref="C?"  Part="1" 
AR Path="/621EA432/6217ACC0" Ref="C?"  Part="1" 
F 0 "C?" H 7015 4146 50  0000 L CNN
F 1 "10n" H 7015 4055 50  0000 L CNN
F 2 "" H 6938 3950 50  0001 C CNN
F 3 "~" H 6900 4100 50  0001 C CNN
	1    6900 4100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6217BBD7
P 6900 4450
AR Path="/621761CD/6217BBD7" Ref="#PWR?"  Part="1" 
AR Path="/621EA432/6217BBD7" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6900 4200 50  0001 C CNN
F 1 "GND" H 6905 4277 50  0000 C CNN
F 2 "" H 6900 4450 50  0001 C CNN
F 3 "" H 6900 4450 50  0001 C CNN
	1    6900 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 4250 6900 4450
Text HLabel 5000 4050 0    50   Input ~ 0
Vdd
Text HLabel 5000 3850 0    50   Input ~ 0
In
Text HLabel 6600 3750 2    50   Input ~ 0
CLK
Text HLabel 6600 4050 2    50   Output ~ 0
Out
Text Label 5250 4200 3    50   ~ 0
Vdd
Text Label 6600 3850 0    50   ~ 0
Vdd
Wire Wire Line
	5300 3850 5000 3850
Wire Wire Line
	5000 4050 5250 4050
$Comp
L power:GND #PWR?
U 1 1 62180E0B
P 5100 4250
AR Path="/621761CD/62180E0B" Ref="#PWR?"  Part="1" 
AR Path="/621EA432/62180E0B" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5100 4000 50  0001 C CNN
F 1 "GND" H 5105 4077 50  0000 C CNN
F 2 "" H 5100 4250 50  0001 C CNN
F 3 "" H 5100 4250 50  0001 C CNN
	1    5100 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 3950 5100 3950
Wire Wire Line
	5100 3950 5100 4250
Text Notes 5300 3150 0    50   ~ 0
5th Order, Lowpass, SCF\nButterworth\nProgrammable cutoff frequency: f_clk / 100.\n
Wire Wire Line
	6450 4050 6600 4050
Wire Wire Line
	6600 3750 6450 3750
Wire Wire Line
	6600 3850 6450 3850
Wire Wire Line
	6450 3950 6900 3950
Wire Wire Line
	5100 3750 5100 3450
Wire Wire Line
	5100 3450 6900 3450
Wire Wire Line
	6900 3450 6900 3950
Connection ~ 6900 3950
Wire Wire Line
	5250 4050 5250 4200
Connection ~ 5250 4050
Wire Wire Line
	5250 4050 5300 4050
$EndSCHEMATC
