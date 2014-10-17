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
Sheet 5 8
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
L TLV271-DBV IC?
U 1 1 540D533E
P 6500 3600
F 0 "IC?" H 6550 3800 60  0000 C CNN
F 1 "TLC272" H 6650 3400 50  0000 C CNN
F 2 "~" H 6500 3600 60  0000 C CNN
F 3 "~" H 6500 3600 60  0000 C CNN
	1    6500 3600
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 540D5379
P 5900 4000
F 0 "C?" H 5900 4100 40  0000 L CNN
F 1 "10n" H 5906 3915 40  0000 L CNN
F 2 "~" H 5938 3850 30  0000 C CNN
F 3 "~" H 5900 4000 60  0000 C CNN
	1    5900 4000
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 540D5394
P 5550 3700
F 0 "R?" V 5630 3700 40  0000 C CNN
F 1 "180k" V 5557 3701 40  0000 C CNN
F 2 "~" V 5480 3700 30  0000 C CNN
F 3 "~" H 5550 3700 30  0000 C CNN
	1    5550 3700
	0    -1   -1   0   
$EndComp
$Comp
L R R?
U 1 1 540D53A6
P 4850 3700
F 0 "R?" V 4930 3700 40  0000 C CNN
F 1 "100k" V 4857 3701 40  0000 C CNN
F 2 "~" V 4780 3700 30  0000 C CNN
F 3 "~" H 4850 3700 30  0000 C CNN
	1    4850 3700
	0    -1   -1   0   
$EndComp
$Comp
L C C?
U 1 1 540D53B1
P 5200 3400
F 0 "C?" H 5200 3500 40  0000 L CNN
F 1 "10n" H 5206 3315 40  0000 L CNN
F 2 "~" H 5238 3250 30  0000 C CNN
F 3 "~" H 5200 3400 60  0000 C CNN
	1    5200 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 3700 5200 3700
Wire Wire Line
	5200 3700 5300 3700
Wire Wire Line
	5200 3600 5200 3700
Connection ~ 5200 3700
Wire Wire Line
	5800 3700 5900 3700
Wire Wire Line
	5900 3700 6200 3700
Wire Wire Line
	5900 3800 5900 3700
Connection ~ 5900 3700
Wire Wire Line
	6200 3500 6100 3500
Wire Wire Line
	6800 3600 6900 3600
Wire Wire Line
	6900 3600 7000 3600
$Comp
L GND #PWR049
U 1 1 540D53FF
P 5900 4300
F 0 "#PWR049" H 5900 4300 30  0001 C CNN
F 1 "GND" H 5900 4230 30  0001 C CNN
F 2 "" H 5900 4300 60  0000 C CNN
F 3 "" H 5900 4300 60  0000 C CNN
	1    5900 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 4300 5900 4200
$Comp
L R R?
U 1 1 540D5418
P 6900 4650
F 0 "R?" V 6980 4650 40  0000 C CNN
F 1 "220k" V 6907 4651 40  0000 C CNN
F 2 "~" V 6830 4650 30  0000 C CNN
F 3 "~" H 6900 4650 30  0000 C CNN
	1    6900 4650
	-1   0    0    1   
$EndComp
$Comp
L R R?
U 1 1 540D5428
P 6900 3950
F 0 "R?" V 6980 3950 40  0000 C CNN
F 1 "56k" V 6907 3951 40  0000 C CNN
F 2 "~" V 6830 3950 30  0000 C CNN
F 3 "~" H 6900 3950 30  0000 C CNN
	1    6900 3950
	-1   0    0    1   
$EndComp
Wire Wire Line
	6900 3100 6900 3600
Wire Wire Line
	6900 3600 6900 3700
Wire Wire Line
	6900 4200 6900 4300
Wire Wire Line
	6900 4300 6900 4400
$Comp
L GND #PWR050
U 1 1 540D5454
P 6900 5000
F 0 "#PWR050" H 6900 5000 30  0001 C CNN
F 1 "GND" H 6900 4930 30  0001 C CNN
F 2 "" H 6900 5000 60  0000 C CNN
F 3 "" H 6900 5000 60  0000 C CNN
	1    6900 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 5000 6900 4900
Wire Wire Line
	6100 3500 6100 4300
Connection ~ 6900 4300
Wire Wire Line
	5200 3200 5200 3100
Wire Wire Line
	5200 3100 6900 3100
Connection ~ 6900 3600
Wire Wire Line
	6100 4300 6900 4300
Text HLabel 4500 3700 0    60   Input ~ 0
IN
Wire Wire Line
	4500 3700 4600 3700
Text HLabel 7000 3600 2    60   Output ~ 0
OUT
Text Notes 5300 2850 0    60   ~ 0
Фильтр Бесселя второго порядка,\nчастота среза примерно 100Гц
$Comp
L +5V #PWR?
U 1 1 543DE619
P 6400 3250
F 0 "#PWR?" H 6400 3340 20  0001 C CNN
F 1 "+5V" H 6400 3340 30  0000 C CNN
F 2 "" H 6400 3250 60  0001 C CNN
F 3 "" H 6400 3250 60  0001 C CNN
	1    6400 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 3250 6400 3350
$Comp
L GND #PWR?
U 1 1 543DE633
P 6400 3950
F 0 "#PWR?" H 6400 3950 30  0001 C CNN
F 1 "GND" H 6400 3880 30  0001 C CNN
F 2 "" H 6400 3950 60  0000 C CNN
F 3 "" H 6400 3950 60  0000 C CNN
	1    6400 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 3950 6400 3850
$EndSCHEMATC
