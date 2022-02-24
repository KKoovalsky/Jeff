EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 5
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
U 1 1 6216739F
P 5900 3700
AR Path="/62166F90/6216739F" Ref="U?"  Part="1" 
AR Path="/6217D375/6216739F" Ref="U?"  Part="1" 
F 0 "U?" H 6244 3746 50  0000 L CNN
F 1 "OPAMP" H 6244 3655 50  0000 L CNN
F 2 "" H 5900 3700 50  0001 C CNN
F 3 "~" H 5900 3700 50  0001 C CNN
	1    5900 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 3600 4950 3600
Text HLabel 4950 3600 0    50   Input ~ 0
In
Wire Wire Line
	5600 3800 5450 3800
Wire Wire Line
	6300 3700 6200 3700
Wire Wire Line
	6300 3700 6650 3700
Connection ~ 6300 3700
Text HLabel 6650 3700 2    50   Output ~ 0
Out
Text HLabel 5800 4400 3    50   Input ~ 0
Vss
Wire Wire Line
	5800 4400 5800 4000
Wire Wire Line
	5450 4800 6300 4800
Wire Wire Line
	5450 3800 5450 4800
Wire Wire Line
	6300 3700 6300 4800
Text HLabel 5800 2950 1    50   Input ~ 0
Vdd
Wire Wire Line
	5800 2950 5800 3400
$EndSCHEMATC
