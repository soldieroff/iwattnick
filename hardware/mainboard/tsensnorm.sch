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
Sheet 2 8
Title "Электровелокомпьютер \"iВаттник\""
Date "14 oct 2014"
Rev "1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 5600 3800 0    39   Input ~ 0
T_IN
Text HLabel 6500 3800 2    39   Output ~ 0
T_OUT
$Comp
L C C?
U 1 1 53FBC0DF
P 6400 4100
AR Path="/53FBBE3A/53FBC0DF" Ref="C?"  Part="1" 
AR Path="/53FBBE43/53FBC0DF" Ref="C?"  Part="1" 
AR Path="/53FBBE3F/53FBC0DF" Ref="C?"  Part="1" 
F 0 "C?" H 6400 4200 40  0000 L CNN
F 1 "100n" H 6406 4015 40  0000 L CNN
F 2 "~" H 6438 3950 30  0000 C CNN
F 3 "~" H 6400 4100 60  0000 C CNN
	1    6400 4100
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 53FBC0EE
P 6050 3800
AR Path="/53FBBE3A/53FBC0EE" Ref="R?"  Part="1" 
AR Path="/53FBBE43/53FBC0EE" Ref="R?"  Part="1" 
AR Path="/53FBBE3F/53FBC0EE" Ref="R?"  Part="1" 
F 0 "R?" V 6130 3800 40  0000 C CNN
F 1 "22k" V 6057 3801 40  0000 C CNN
F 2 "~" V 5980 3800 30  0000 C CNN
F 3 "~" H 6050 3800 30  0000 C CNN
	1    6050 3800
	0    -1   -1   0   
$EndComp
$Comp
L R R?
U 1 1 53FBC13B
P 5700 3450
AR Path="/53FBBE3A/53FBC13B" Ref="R?"  Part="1" 
AR Path="/53FBBE43/53FBC13B" Ref="R?"  Part="1" 
AR Path="/53FBBE3F/53FBC13B" Ref="R?"  Part="1" 
F 0 "R?" V 5780 3450 40  0000 C CNN
F 1 "5.1k" V 5707 3451 40  0000 C CNN
F 2 "~" V 5630 3450 30  0000 C CNN
F 3 "~" H 5700 3450 30  0000 C CNN
	1    5700 3450
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR047
U 1 1 5413004E
P 6400 4400
AR Path="/53FBBE43/5413004E" Ref="#PWR047"  Part="1" 
AR Path="/53FBBE3A/5413004E" Ref="#PWR043"  Part="1" 
AR Path="/53FBBE3F/5413004E" Ref="#PWR045"  Part="1" 
F 0 "#PWR043" H 6400 4400 30  0001 C CNN
F 1 "GND" H 6400 4330 30  0001 C CNN
F 2 "" H 6400 4400 60  0000 C CNN
F 3 "" H 6400 4400 60  0000 C CNN
	1    6400 4400
	1    0    0    -1  
$EndComp
$Comp
L VAA #PWR044
U 1 1 53FCA63C
P 5700 3150
AR Path="/53FBBE3A/53FCA63C" Ref="#PWR044"  Part="1" 
AR Path="/53FBBE3F/53FCA63C" Ref="#PWR046"  Part="1" 
AR Path="/53FBBE43/53FCA63C" Ref="#PWR048"  Part="1" 
F 0 "#PWR044" H 5700 3210 30  0001 C CNN
F 1 "VAA" H 5700 3260 30  0000 C CNN
F 2 "" H 5700 3150 60  0000 C CNN
F 3 "" H 5700 3150 60  0000 C CNN
	1    5700 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 3800 5700 3800
Wire Wire Line
	5700 3800 5800 3800
Wire Wire Line
	6300 3800 6400 3800
Wire Wire Line
	6400 3800 6500 3800
Wire Wire Line
	6400 3800 6400 3900
Wire Wire Line
	6400 4400 6400 4300
Connection ~ 6400 3800
Wire Wire Line
	5700 3700 5700 3800
Connection ~ 5700 3800
Wire Wire Line
	5700 3150 5700 3200
$EndSCHEMATC
