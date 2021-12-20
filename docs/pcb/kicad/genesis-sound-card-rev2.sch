EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L genesis-sound-card:M5STAMP-C3 U2
U 1 1 61BDE623
P 1850 1650
F 0 "U2" H 1850 2415 50  0000 C CNN
F 1 "M5STAMP-C3" H 1850 2324 50  0000 C CNN
F 2 "genesis-sound-card:M5Stamp-C3" H 1850 1450 50  0001 C CNN
F 3 "" H 1850 1450 50  0001 C CNN
	1    1850 1650
	1    0    0    -1  
$EndComp
$Comp
L genesis-sound-card:KMR-1.8SPI U3
U 1 1 61BDF681
P 6900 1800
F 0 "U3" H 7878 1846 50  0000 L CNN
F 1 "KMR-1.8SPI" H 7878 1755 50  0000 L CNN
F 2 "genesis-sound-card:KMR-1.8SPI" H 4900 3150 50  0001 C CNN
F 3 "" H 4900 3150 50  0001 C CNN
	1    6900 1800
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 61BE0951
P 1600 3100
F 0 "SW1" H 1600 3385 50  0000 C CNN
F 1 "SW_Push" H 1600 3294 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 1600 3300 50  0001 C CNN
F 3 "~" H 1600 3300 50  0001 C CNN
	1    1600 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 61BE3D51
P 3500 3250
F 0 "R3" H 3570 3296 50  0000 L CNN
F 1 "10K" H 3570 3205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3430 3250 50  0001 C CNN
F 3 "~" H 3500 3250 50  0001 C CNN
	1    3500 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 61BE66E7
P 3900 3250
F 0 "R4" H 3970 3296 50  0000 L CNN
F 1 "10K" H 3970 3205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3830 3250 50  0001 C CNN
F 3 "~" H 3900 3250 50  0001 C CNN
	1    3900 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 61BE69A0
P 4300 3250
F 0 "R5" H 4370 3296 50  0000 L CNN
F 1 "10K" H 4370 3205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4230 3250 50  0001 C CNN
F 3 "~" H 4300 3250 50  0001 C CNN
	1    4300 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 61BE6B6E
P 4750 3250
F 0 "R6" H 4820 3296 50  0000 L CNN
F 1 "10K" H 4820 3205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4680 3250 50  0001 C CNN
F 3 "~" H 4750 3250 50  0001 C CNN
	1    4750 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 61BE6D6D
P 5200 3250
F 0 "R7" H 5270 3296 50  0000 L CNN
F 1 "10K" H 5270 3205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5130 3250 50  0001 C CNN
F 3 "~" H 5200 3250 50  0001 C CNN
	1    5200 3250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 61BEB851
P 1400 3100
F 0 "#PWR01" H 1400 2850 50  0001 C CNN
F 1 "GND" H 1405 2927 50  0000 C CNN
F 2 "" H 1400 3100 50  0001 C CNN
F 3 "" H 1400 3100 50  0001 C CNN
	1    1400 3100
	1    0    0    -1  
$EndComp
Text GLabel 1800 3100 2    50   Input ~ 0
BOOT_SW
Text GLabel 2400 1750 2    50   Output ~ 0
BOOT_SW
$Comp
L genesis-sound-card:AE-USB2.0-TYPE-C U1
U 1 1 61BF6BFC
P 1650 4200
F 0 "U1" H 1708 4765 50  0000 C CNN
F 1 "AE-USB2.0-TYPE-C" H 1708 4674 50  0000 C CNN
F 2 "genesis-sound-card:AE-USB2.0-TYPE-C" H 1650 3750 50  0001 C CNN
F 3 "" H 1650 3750 50  0001 C CNN
	1    1650 4200
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_DIP_x01 JP1
U 1 1 61BF8192
P 2800 3900
F 0 "JP1" H 2800 4167 50  0000 C CNN
F 1 "SW_DIP_x01" H 2800 4076 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2800 3900 50  0001 C CNN
F 3 "~" H 2800 3900 50  0001 C CNN
	1    2800 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 61C108A8
P 2500 4700
F 0 "R1" H 2570 4746 50  0000 L CNN
F 1 "5.1K" H 2570 4655 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 2430 4700 50  0001 C CNN
F 3 "~" H 2500 4700 50  0001 C CNN
	1    2500 4700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 61C108AE
P 2750 4700
F 0 "R2" H 2820 4746 50  0000 L CNN
F 1 "5.1K" H 2820 4655 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 2680 4700 50  0001 C CNN
F 3 "~" H 2750 4700 50  0001 C CNN
	1    2750 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 4400 2500 4400
Wire Wire Line
	2500 4400 2500 4550
Wire Wire Line
	2000 4300 2750 4300
Wire Wire Line
	2750 4300 2750 4550
Text GLabel 2400 1850 2    50   Input ~ 0
GND
Text GLabel 5800 2600 0    50   Input ~ 0
GND
Text GLabel 3100 3900 2    50   Output ~ 0
Vcc_USB_5V
Text GLabel 2400 2150 2    50   BiDi ~ 0
Vcc_USB_5V
Text GLabel 2400 1350 2    50   Output ~ 0
Vcc
NoConn ~ 1300 1850
Text GLabel 5800 2500 0    50   Input ~ 0
Vcc
Text GLabel 5800 1200 0    50   Input ~ 0
Vcc
Text GLabel 5800 1100 0    50   Input ~ 0
GND
Wire Wire Line
	5800 1400 5700 1400
Wire Wire Line
	5700 1400 5700 1900
Wire Wire Line
	5700 1900 5800 1900
Wire Wire Line
	5750 1600 5750 1800
Wire Wire Line
	5750 1800 5800 1800
Wire Wire Line
	5750 1600 5800 1600
Text GLabel 2000 4100 2    50   Output ~ 0
GPIO19(D+)
Text GLabel 2400 2050 2    50   Input ~ 0
GPIO19(D+)
Text GLabel 2400 1950 2    50   Output ~ 0
GPIO18(D-)
Text GLabel 2000 4000 2    50   Input ~ 0
GPIO18(D-)
Wire Wire Line
	2000 4200 2000 4500
Connection ~ 2000 4500
NoConn ~ 1300 2050
Text GLabel 1300 1150 0    50   Input ~ 0
GND
Text GLabel 1300 1950 0    50   Input ~ 0
GND
Text GLabel 2000 4850 3    50   Input ~ 0
GND
Text GLabel 2500 4850 3    50   Input ~ 0
GND
Text GLabel 2750 4850 3    50   Input ~ 0
GND
Wire Wire Line
	2000 4500 2000 4850
Text GLabel 3500 3400 3    50   Output ~ 0
Vcc
Text GLabel 3900 3400 3    50   Output ~ 0
Vcc
Text GLabel 4300 3400 3    50   Output ~ 0
Vcc
Text GLabel 4750 3400 3    50   Output ~ 0
Vcc
Text GLabel 5200 3400 3    50   Output ~ 0
Vcc
$Comp
L Connector_Generic:Conn_01x12 J1
U 1 1 61C067D5
P 4550 4550
F 0 "J1" V 4675 4496 50  0000 C CNN
F 1 "Conn_01x12" V 4766 4496 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x12_P2.54mm_Vertical" H 4550 4550 50  0001 C CNN
F 3 "~" H 4550 4550 50  0001 C CNN
	1    4550 4550
	0    1    1    0   
$EndComp
Text GLabel 3950 4350 1    50   Input ~ 0
Vcc
Text GLabel 4750 4350 1    50   Input ~ 0
GND
Text GLabel 4850 4350 1    50   Input ~ 0
Vcc_USB_5V
Text GLabel 4050 4350 1    50   Input ~ 0
GND
Text GLabel 2400 2250 2    50   Input ~ 0
GND
NoConn ~ 2400 1650
Text GLabel 1300 1650 0    50   Output ~ 0
LCD_RST
Text GLabel 5800 2100 0    50   Input ~ 0
LCD_RST
Text GLabel 1300 1750 0    50   Output ~ 0
LCD_DC
Text GLabel 5800 2000 0    50   Input ~ 0
LCD_DC
Text GLabel 1300 1250 0    50   Output ~ 0
SCLK
Text GLabel 1300 1350 0    50   Output ~ 0
MOSI
Text GLabel 1300 1450 0    50   Input ~ 0
MISO
Text GLabel 1300 1550 0    50   Output ~ 0
LCD_CS
Text GLabel 1300 2150 0    50   Output ~ 0
SD_CS
Wire Wire Line
	3400 1400 3500 1400
Text GLabel 3400 1400 0    50   Input ~ 0
MOSI
Wire Wire Line
	3500 3100 3500 1400
Connection ~ 3500 1400
Text GLabel 3800 1500 0    50   Output ~ 0
MISO
Wire Wire Line
	3800 1500 3900 1500
Wire Wire Line
	3900 1500 3900 3100
Connection ~ 3900 1500
Text GLabel 4200 1600 0    50   Input ~ 0
SCLK
Wire Wire Line
	4200 1600 4300 1600
Wire Wire Line
	4300 1600 4300 3100
Connection ~ 4300 1600
Text GLabel 4700 1700 0    50   Input ~ 0
LCD_CS
Wire Wire Line
	4750 1700 4750 3100
Wire Wire Line
	3500 1400 5700 1400
Connection ~ 5700 1400
Wire Wire Line
	3900 1500 5800 1500
Wire Wire Line
	4300 1600 5750 1600
Connection ~ 5750 1600
Text GLabel 5100 1300 0    50   Input ~ 0
SD_CS
Wire Wire Line
	5100 1300 5200 1300
Connection ~ 4750 1700
Wire Wire Line
	4750 1700 4700 1700
Wire Wire Line
	4750 1700 5800 1700
Wire Wire Line
	5200 3100 5200 1300
Connection ~ 5200 1300
Wire Wire Line
	5200 1300 5800 1300
Text GLabel 4350 4350 1    50   Input ~ 0
SCLK
Text GLabel 4450 4350 1    50   Input ~ 0
MOSI
Text GLabel 4550 4350 1    50   Input ~ 0
MISO
Text GLabel 4650 4350 1    50   Input ~ 0
GPIO0
Text GLabel 4950 4350 1    50   Input ~ 0
GPIO18(D-)
Text GLabel 5050 4350 1    50   Input ~ 0
GPIO19(D+)
Text GLabel 4150 4350 1    50   Input ~ 0
GPIO20(RX0)
Text GLabel 4250 4350 1    50   Input ~ 0
GPIO21(TX0)
Text GLabel 2400 1450 2    50   Output ~ 0
GPIO21(TX0)
Text GLabel 2400 1550 2    50   Output ~ 0
GPIO20(RX0)
Wire Wire Line
	2000 3900 2500 3900
Text GLabel 1300 2250 0    50   Output ~ 0
GPIO0
Text GLabel 5400 4550 1    50   Input ~ 0
GND
$Comp
L power:GND #PWR?
U 1 1 61C07F9F
P 5400 4550
F 0 "#PWR?" H 5400 4300 50  0001 C CNN
F 1 "GND" H 5405 4377 50  0000 C CNN
F 2 "" H 5400 4550 50  0001 C CNN
F 3 "" H 5400 4550 50  0001 C CNN
	1    5400 4550
	1    0    0    -1  
$EndComp
$EndSCHEMATC
