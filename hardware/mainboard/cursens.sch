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
Sheet 6 8
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
L C C?
U 1 1 540D71D4
P 6650 4200
F 0 "C?" H 6700 4300 50  0000 L CNN
F 1 "100n" H 6700 4100 50  0000 L CNN
F 2 "SM0805" H 6700 4200 50  0001 C CNN
F 3 "" H 6650 4200 60  0001 C CNN
	1    6650 4200
	1    0    0    -1  
$EndComp
Text Notes 4900 2250 0    60   ~ 0
Фильтр-делитель напряжения батареи, t=22мс\nПараллельно нижнему резистору включено внутреннее\nсопротивление INA220 (320k), нижнее плечо = 76.2k,\nинтегральный коэф. деления = 0.257\nт.е. при Uвх = 100В на входе VBUS будем иметь U=25.7В\n(по документации должно быть не более 26В).
$Comp
L R R?
U 1 1 540D71E1
P 6100 2850
F 0 "R?" V 6180 2850 50  0000 C CNN
F 1 "220k" V 6100 2850 50  0000 C CNN
F 2 "SM0805" V 6200 2850 50  0001 C CNN
F 3 "" H 6100 2850 60  0001 C CNN
	1    6100 2850
	0    -1   -1   0   
$EndComp
$Comp
L R R?
U 1 1 540D71E7
P 5500 3200
F 0 "R?" V 5580 3200 50  0000 C CNN
F 1 "100k" V 5500 3200 50  0000 C CNN
F 2 "SM0805" V 5600 3200 50  0001 C CNN
F 3 "" H 5500 3200 60  0001 C CNN
	1    5500 3200
	-1   0    0    1   
$EndComp
$Comp
L CP1 C?
U 1 1 540D71ED
P 5250 3200
F 0 "C?" H 5300 3300 50  0000 L CNN
F 1 "100n" H 5300 3100 50  0000 L CNN
F 2 "SM0805" H 5300 3200 50  0001 C CNN
F 3 "" H 5250 3200 60  0001 C CNN
	1    5250 3200
	-1   0    0    -1  
$EndComp
$Comp
L INA22X IC?
U 1 1 540D71FF
P 5800 4250
F 0 "IC?" H 5900 3750 60  0000 C CNN
F 1 "INA220" H 5750 4700 60  0000 C CNN
F 2 "MSOP10-0.5" H 5750 4800 60  0001 C CNN
F 3 "" H 5800 4250 60  0001 C CNN
	1    5800 4250
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 540D7207
P 4650 3550
F 0 "R?" V 4730 3550 50  0000 C CNN
F 1 "10" V 4650 3550 50  0000 C CNN
F 2 "SM0805" V 4750 3550 50  0001 C CNN
F 3 "" H 4650 3550 60  0001 C CNN
	1    4650 3550
	-1   0    0    1   
$EndComp
$Comp
L C C?
U 1 1 540D720D
P 4650 4100
F 0 "C?" H 4700 4200 50  0000 L CNN
F 1 "100n" H 4700 4000 50  0000 L CNN
F 2 "SM0805" H 4700 4100 50  0001 C CNN
F 3 "" H 4650 4100 60  0001 C CNN
	1    4650 4100
	-1   0    0    -1  
$EndComp
Connection ~ 6250 3950
Wire Wire Line
	6150 3950 6650 3950
Wire Wire Line
	6650 3950 6650 4000
Wire Wire Line
	5250 3550 5750 3550
Wire Wire Line
	5250 3550 5250 3400
Connection ~ 5250 2850
Wire Wire Line
	5250 3000 5250 2850
Connection ~ 5500 2850
Wire Wire Line
	5500 3450 5500 3650
Connection ~ 5500 3550
Wire Wire Line
	6250 3900 6250 3950
Wire Wire Line
	6250 4250 6150 4250
Wire Wire Line
	6150 4450 6250 4450
Wire Wire Line
	6250 4450 6250 4650
Connection ~ 6250 4550
Wire Wire Line
	6150 4150 6250 4150
Wire Wire Line
	4950 2850 4950 3950
Wire Wire Line
	4950 3950 5450 3950
Wire Wire Line
	6650 4550 6650 4400
Wire Wire Line
	6150 4550 6650 4550
Wire Wire Line
	4650 3800 4650 3900
Wire Wire Line
	4850 4250 5450 4250
Wire Wire Line
	5350 4550 5350 4650
Wire Wire Line
	4650 3150 4650 3300
$Comp
L VAA #PWR051
U 1 1 540D7230
P 6250 3900
F 0 "#PWR051" H 6250 3960 30  0001 C CNN
F 1 "VAA" H 6250 4010 30  0000 C CNN
F 2 "" H 6250 3900 60  0000 C CNN
F 3 "" H 6250 3900 60  0000 C CNN
	1    6250 3900
	1    0    0    -1  
$EndComp
$Comp
L DIODE D?
U 1 1 540D7236
P 5750 3200
F 0 "D?" H 5950 3100 40  0000 C CNN
F 1 "1N4148" H 5750 3100 40  0000 C CNN
F 2 "~" H 5750 3200 60  0000 C CNN
F 3 "~" H 5750 3200 60  0000 C CNN
	1    5750 3200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5750 3000 5750 2850
Wire Wire Line
	5750 3550 5750 3400
$Comp
L R R?
U 1 1 540D723F
P 4650 4650
F 0 "R?" V 4730 4650 50  0000 C CNN
F 1 "10" V 4650 4650 50  0000 C CNN
F 2 "SM0805" V 4750 4650 50  0001 C CNN
F 3 "" H 4650 4650 60  0001 C CNN
	1    4650 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 4300 4650 4400
Wire Wire Line
	4650 4350 4850 4350
Wire Wire Line
	4850 4350 4850 4250
Connection ~ 4650 4350
Wire Wire Line
	4650 3850 4850 3850
Wire Wire Line
	4850 3850 4850 4150
Connection ~ 4650 3850
Wire Wire Line
	4850 4150 5450 4150
Wire Wire Line
	5350 4550 5450 4550
Wire Wire Line
	4650 4900 4650 5000
Text HLabel 4650 3150 1    60   Input ~ 0
SHUNT+
Text HLabel 4650 5000 3    60   Input ~ 0
SHUNT-
Text HLabel 6250 4150 2    60   Output ~ 0
SDA
Text HLabel 6250 4250 2    60   Output ~ 0
SCL
$Comp
L GND #PWR?
U 1 1 54146440
P 5350 4650
F 0 "#PWR?" H 5350 4650 30  0001 C CNN
F 1 "GND" H 5350 4580 30  0001 C CNN
F 2 "" H 5350 4650 60  0000 C CNN
F 3 "" H 5350 4650 60  0000 C CNN
	1    5350 4650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 5414644D
P 6250 4650
F 0 "#PWR?" H 6250 4650 30  0001 C CNN
F 1 "GND" H 6250 4580 30  0001 C CNN
F 2 "" H 6250 4650 60  0000 C CNN
F 3 "" H 6250 4650 60  0000 C CNN
	1    6250 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 2850 5850 2850
Text GLabel 6450 2850 2    60   Input ~ 0
+BAT_PROT
Wire Wire Line
	6450 2850 6350 2850
Connection ~ 5750 2850
Wire Wire Line
	5500 2950 5500 2850
$Comp
L GND #PWR?
U 1 1 543D3248
P 5500 3650
F 0 "#PWR?" H 5500 3650 30  0001 C CNN
F 1 "GND" H 5500 3580 30  0001 C CNN
F 2 "" H 5500 3650 60  0000 C CNN
F 3 "" H 5500 3650 60  0000 C CNN
	1    5500 3650
	1    0    0    -1  
$EndComp
$EndSCHEMATC
