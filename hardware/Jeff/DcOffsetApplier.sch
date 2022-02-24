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
L Device:R R1
U 1 1 6216E416
P 4350 3850
AR Path="/6216D512/6216E416" Ref="R1"  Part="1" 
AR Path="/6217D37B/6216E416" Ref="R?"  Part="1" 
F 0 "R?" V 4143 3850 50  0000 C CNN
F 1 "33k" V 4234 3850 50  0000 C CNN
F 2 "" V 4280 3850 50  0001 C CNN
F 3 "~" H 4350 3850 50  0001 C CNN
	1    4350 3850
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 6216E932
P 4850 3550
AR Path="/6216D512/6216E932" Ref="R2"  Part="1" 
AR Path="/6217D37B/6216E932" Ref="R?"  Part="1" 
F 0 "R?" H 4920 3596 50  0000 L CNN
F 1 "100k" H 4920 3505 50  0000 L CNN
F 2 "" V 4780 3550 50  0001 C CNN
F 3 "~" H 4850 3550 50  0001 C CNN
	1    4850 3550
	1    0    0    -1  
$EndComp
$Comp
L Device:R Rf
U 1 1 6216EE97
P 4400 4800
AR Path="/6216D512/6216EE97" Ref="Rf"  Part="1" 
AR Path="/6217D37B/6216EE97" Ref="Rf?"  Part="1" 
F 0 "Rf?" V 4193 4800 50  0000 C CNN
F 1 "33k" V 4284 4800 50  0000 C CNN
F 2 "" V 4330 4800 50  0001 C CNN
F 3 "~" H 4400 4800 50  0001 C CNN
	1    4400 4800
	0    1    1    0   
$EndComp
$Comp
L Device:R Rg
U 1 1 6216F918
P 5000 4800
AR Path="/6216D512/6216F918" Ref="Rg"  Part="1" 
AR Path="/6217D37B/6216F918" Ref="Rg?"  Part="1" 
F 0 "Rg?" V 4793 4800 50  0000 C CNN
F 1 "100k" V 4884 4800 50  0000 C CNN
F 2 "" V 4930 4800 50  0001 C CNN
F 3 "~" H 5000 4800 50  0001 C CNN
	1    5000 4800
	0    1    1    0   
$EndComp
$Comp
L pspice:OPAMP U?
U 1 1 6216D882
P 5400 3950
AR Path="/6216D512/6216D882" Ref="U?"  Part="1" 
AR Path="/6217D37B/6216D882" Ref="U?"  Part="1" 
F 0 "U?" H 5744 3996 50  0000 L CNN
F 1 "OPAMP" H 5744 3905 50  0000 L CNN
F 2 "" H 5400 3950 50  0001 C CNN
F 3 "~" H 5400 3950 50  0001 C CNN
	1    5400 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 4250 5300 4350
$Comp
L power:GND #PWR?
U 1 1 621743FD
P 5300 4350
AR Path="/6216D512/621743FD" Ref="#PWR?"  Part="1" 
AR Path="/6217D37B/621743FD" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5300 4100 50  0001 C CNN
F 1 "GND" H 5300 4200 50  0000 C CNN
F 2 "" H 5300 4350 50  0001 C CNN
F 3 "" H 5300 4350 50  0001 C CNN
	1    5300 4350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 621763F3
P 4000 4850
AR Path="/6216D512/621763F3" Ref="#PWR?"  Part="1" 
AR Path="/6217D37B/621763F3" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4000 4600 50  0001 C CNN
F 1 "GND" H 4000 4700 50  0000 C CNN
F 2 "" H 4000 4850 50  0001 C CNN
F 3 "" H 4000 4850 50  0001 C CNN
	1    4000 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 4800 4000 4800
Wire Wire Line
	4000 4800 4000 4850
Wire Wire Line
	4850 4800 4700 4800
Wire Wire Line
	4700 4800 4700 4050
Wire Wire Line
	4700 4050 5100 4050
Connection ~ 4700 4800
Wire Wire Line
	4700 4800 4550 4800
Wire Wire Line
	5150 4800 5950 4800
Wire Wire Line
	5950 4800 5950 3950
Wire Wire Line
	5950 3950 5700 3950
Text HLabel 3900 3850 0    50   Input ~ 0
In
Wire Wire Line
	4200 3850 3900 3850
Wire Wire Line
	4500 3850 4850 3850
Wire Wire Line
	4850 3850 4850 3700
Text HLabel 4850 3200 1    50   Input ~ 0
Vdd_half
Wire Wire Line
	5100 3850 4850 3850
Connection ~ 4850 3850
Wire Wire Line
	5950 3950 6450 3950
Connection ~ 5950 3950
Text HLabel 6450 3950 2    50   Output ~ 0
Out
Text HLabel 5300 3400 1    50   Input ~ 0
Vdd
Wire Wire Line
	5300 3400 5300 3650
Text Notes 6950 3500 2    50   ~ 0
When R1 == Rf and R2 == RG:\nG = R2/R1\nDc_offset = Vref
Text Label 4850 3350 2    50   ~ 0
Vref
Wire Wire Line
	4850 3200 4850 3400
$EndSCHEMATC
