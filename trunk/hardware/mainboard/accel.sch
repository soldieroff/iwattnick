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
LIBS:special
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
LIBS:zap
LIBS:iwattnick-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 8
Title "Электровелокомпьютер \"iВаттник\""
Date "14 oct 2014"
Rev "1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LSM303DLHC IC?
U 1 1 540DB909
P 5350 3800
F 0 "IC?" H 5350 4400 60  0000 C CNN
F 1 "LSM303DLHC" H 5350 3150 60  0000 C CNN
F 2 "" H 5350 3900 60  0000 C CNN
F 3 "" H 5350 3900 60  0000 C CNN
	1    5350 3800
	1    0    0    -1  
$EndComp
Text HLabel 5900 3750 2    59   Output ~ 0
SDA
Text HLabel 5900 3650 2    59   Output ~ 0
SCL
Wire Wire Line
	5900 3650 5800 3650
Wire Wire Line
	5800 3750 5900 3750
$Comp
L C C?
U 1 1 540DBC8F
P 4600 3550
F 0 "C?" H 4600 3650 40  0000 L CNN
F 1 "0.22u" H 4606 3465 40  0000 L CNN
F 2 "~" H 4638 3400 30  0000 C CNN
F 3 "~" H 4600 3550 60  0000 C CNN
	1    4600 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 3750 4900 3750
Wire Wire Line
	4900 3350 4600 3350
$Comp
L C C?
U 1 1 540DBCC3
P 4600 4050
F 0 "C?" H 4600 4150 40  0000 L CNN
F 1 "4.7u" H 4606 3965 40  0000 L CNN
F 2 "~" H 4638 3900 30  0000 C CNN
F 3 "~" H 4600 4050 60  0000 C CNN
	1    4600 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 3850 4900 3850
Wire Wire Line
	4600 4250 4900 4250
Wire Wire Line
	4900 4050 4800 4050
Wire Wire Line
	4800 4050 4800 4350
Connection ~ 4800 4250
Wire Wire Line
	4900 4150 4800 4150
Connection ~ 4800 4150
NoConn ~ 5800 4250
NoConn ~ 5800 4050
NoConn ~ 5800 3950
$Comp
L VAA #PWR055
U 1 1 540DBECF
P 5900 3300
F 0 "#PWR055" H 5900 3360 30  0001 C CNN
F 1 "VAA" H 5900 3410 30  0000 C CNN
F 2 "" H 5900 3300 60  0000 C CNN
F 3 "" H 5900 3300 60  0000 C CNN
	1    5900 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 3300 5900 3450
Wire Wire Line
	5900 3450 5800 3450
Wire Wire Line
	5800 3350 6350 3350
Connection ~ 5900 3350
NoConn ~ 4900 3950
$Comp
L C C?
U 1 1 540E2F15
P 6350 3750
F 0 "C?" H 6350 3850 40  0000 L CNN
F 1 "100n" H 6356 3665 40  0000 L CNN
F 2 "~" H 6388 3600 30  0000 C CNN
F 3 "~" H 6350 3750 60  0000 C CNN
	1    6350 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 3350 6350 3550
Wire Wire Line
	6350 4050 6350 3950
$Comp
L GND #PWR?
U 1 1 54148423
P 6350 4050
F 0 "#PWR?" H 6350 4050 30  0001 C CNN
F 1 "GND" H 6350 3980 30  0001 C CNN
F 2 "" H 6350 4050 60  0000 C CNN
F 3 "" H 6350 4050 60  0000 C CNN
	1    6350 4050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 543D3287
P 4800 4350
F 0 "#PWR?" H 4800 4350 30  0001 C CNN
F 1 "GND" H 4800 4280 30  0001 C CNN
F 2 "" H 4800 4350 60  0000 C CNN
F 3 "" H 4800 4350 60  0000 C CNN
	1    4800 4350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
