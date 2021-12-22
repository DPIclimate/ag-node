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
L power:GND #PWR0101
U 1 1 608244A2
P 1950 4800
F 0 "#PWR0101" H 1950 4550 50  0001 C CNN
F 1 "GND" H 1955 4627 50  0000 C CNN
F 2 "" H 1950 4800 50  0001 C CNN
F 3 "" H 1950 4800 50  0001 C CNN
	1    1950 4800
	1    0    0    -1  
$EndComp
Text Label 2700 3600 0    50   ~ 0
Reset
Text Label 2550 2400 0    50   ~ 0
MOSI
Text Label 2550 2500 0    50   ~ 0
MISO
$Comp
L power:+3.3V #PWR0102
U 1 1 60825F6D
P 2350 1400
F 0 "#PWR0102" H 2350 1250 50  0001 C CNN
F 1 "+3.3V" H 2365 1573 50  0000 C CNN
F 2 "" H 2350 1400 50  0001 C CNN
F 3 "" H 2350 1400 50  0001 C CNN
	1    2350 1400
	0    1    1    0   
$EndComp
Text Label 3650 2600 0    50   ~ 0
SCK
Text Label 3400 3400 0    50   ~ 0
SDA
Connection ~ 10750 6200
$Comp
L power:GNDA #PWR0143
U 1 1 61857292
P 10750 6200
F 0 "#PWR0143" H 10750 5950 50  0001 C CNN
F 1 "GNDA" H 10850 6050 50  0000 R CNN
F 2 "" H 10750 6200 50  0001 C CNN
F 3 "" H 10750 6200 50  0001 C CNN
	1    10750 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR0144
U 1 1 61856E8A
P 8900 6200
F 0 "#PWR0144" H 8900 5950 50  0001 C CNN
F 1 "GNDA" H 9000 6050 50  0000 R CNN
F 2 "" H 8900 6200 50  0001 C CNN
F 3 "" H 8900 6200 50  0001 C CNN
	1    8900 6200
	0    -1   -1   0   
$EndComp
Connection ~ 10750 5300
Wire Wire Line
	10550 5300 10750 5300
Wire Wire Line
	10750 5300 10800 5300
Wire Wire Line
	10750 5600 10750 5300
Wire Wire Line
	10550 6200 10750 6200
Wire Wire Line
	10750 5900 10750 6200
$Comp
L Device:C C12
U 1 1 610BB9D4
P 10750 5750
F 0 "C12" H 10865 5796 50  0000 L CNN
F 1 "10uF" H 10865 5705 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_5x5.4" H 10788 5600 50  0001 C CNN
F 3 "~" H 10750 5750 50  0001 C CNN
	1    10750 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 6100 9350 6100
$Comp
L CONUFL001-SMD-T:CONUFL001-SMD-T J3
U 1 1 6100024A
P 8800 6200
F 0 "J3" H 8794 6507 50  0000 C CNN
F 1 "CONUFL001-SMD-T" H 8794 6416 50  0000 C CNN
F 2 "CONUFL001-SMD-T:LINX_CONUFL001-SMD-T" H 8800 6200 50  0001 L BNN
F 3 "" H 8800 6200 50  0001 L BNN
F 4 "B" H 8800 6200 50  0001 L BNN "PARTREV"
F 5 "Linx Technologies" H 8800 6200 50  0001 L BNN "MANUFACTURER"
F 6 "Manufacturer Recommendations" H 8800 6200 50  0001 L BNN "STANDARD"
	1    8800 6200
	1    0    0    -1  
$EndComp
Text Label 9350 5700 2    50   ~ 0
SCK
Connection ~ 8750 5600
Wire Wire Line
	8650 5600 8750 5600
Wire Wire Line
	8750 5600 9350 5600
Connection ~ 9200 5300
Wire Wire Line
	9100 5300 9200 5300
Wire Wire Line
	9200 5300 9350 5300
$Comp
L Device:R R15
U 1 1 60ECE3EB
P 8750 5450
F 0 "R15" H 8680 5404 50  0000 R CNN
F 1 "100K" H 8680 5495 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8680 5450 50  0001 C CNN
F 3 "~" H 8750 5450 50  0001 C CNN
	1    8750 5450
	-1   0    0    1   
$EndComp
$Comp
L Device:R R19
U 1 1 60EAA21A
P 9200 5150
F 0 "R19" H 9130 5104 50  0000 R CNN
F 1 "100K" H 9130 5195 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 9130 5150 50  0001 C CNN
F 3 "~" H 9200 5150 50  0001 C CNN
	1    9200 5150
	-1   0    0    1   
$EndComp
NoConn ~ 10550 6000
NoConn ~ 10550 5900
NoConn ~ 10550 5800
$Comp
L RFM69HCW:COM-13909 U4
U 1 1 60E55C5B
P 9950 5700
F 0 "U4" H 9950 6367 50  0000 C CNN
F 1 "COM-13909" H 9950 6276 50  0000 C CNN
F 2 "RFM69HCW:MOD_COM-13909" H 9950 5700 50  0001 L BNN
F 3 "" H 9950 5700 50  0001 L BNN
F 4 "SparkFun Electronics" H 9950 5700 50  0001 L BNN "MANUFACTURER"
	1    9950 5700
	1    0    0    -1  
$EndComp
Wire Notes Line
	8300 6400 8300 4500
Wire Notes Line
	11050 4500 11050 6400
Text Label 10550 5700 0    50   ~ 0
G2
Text Label 10550 5600 0    50   ~ 0
G1
Text Label 9100 5300 2    50   ~ 0
Reset
Text Label 8650 5600 2    50   ~ 0
Lora_CS
Text Label 9350 5800 2    50   ~ 0
MOSI
Text Label 9350 5900 2    50   ~ 0
MISO
Text Label 10550 5500 0    50   ~ 0
G0
Text Notes 7400 7500 0    79   ~ 0
ATMega328_LoRa
Text Notes 8150 7650 0    50   ~ 0
21th June 2021
Text Notes 8450 7500 0    50   ~ 0
- Harvey Bates
Wire Wire Line
	1950 1800 1950 1400
$Comp
L power:+3.3V #PWR0115
U 1 1 60AAADFB
P 3200 3100
F 0 "#PWR0115" H 3200 2950 50  0001 C CNN
F 1 "+3.3V" H 3215 3273 50  0000 C CNN
F 2 "" H 3200 3100 50  0001 C CNN
F 3 "" H 3200 3100 50  0001 C CNN
	1    3200 3100
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 6082BC57
P 3200 3250
F 0 "R7" H 3270 3296 50  0000 L CNN
F 1 "10K" H 3270 3205 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3130 3250 50  0001 C CNN
F 3 "~" H 3200 3250 50  0001 C CNN
	1    3200 3250
	1    0    0    -1  
$EndComp
Connection ~ 3200 3400
Wire Wire Line
	3200 3400 3400 3400
Text Label 2550 3800 0    50   ~ 0
Rx
Text Label 2550 3900 0    50   ~ 0
Tx
Wire Wire Line
	3500 3600 3500 3700
$Comp
L Device:C C18
U 1 1 610460CC
P 1150 2100
F 0 "C18" V 898 2100 50  0000 C CNN
F 1 "0.1uF" V 989 2100 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1188 1950 50  0001 C CNN
F 3 "~" H 1150 2100 50  0001 C CNN
	1    1150 2100
	0    1    1    0   
$EndComp
Wire Wire Line
	1300 2100 1350 2100
$Comp
L power:GND #PWR0116
U 1 1 610879BE
P 1000 2100
F 0 "#PWR0116" H 1000 1850 50  0001 C CNN
F 1 "GND" H 1005 1927 50  0000 C CNN
F 2 "" H 1000 2100 50  0001 C CNN
F 3 "" H 1000 2100 50  0001 C CNN
	1    1000 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 2800 2550 2850
Wire Wire Line
	2550 2850 3150 2850
Wire Wire Line
	3150 2850 3150 2750
Wire Wire Line
	2750 2750 2750 2700
$Comp
L Device:C C19
U 1 1 611CA7E1
P 1700 1400
F 0 "C19" V 1448 1400 50  0000 C CNN
F 1 "0.1uF" V 1539 1400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1738 1250 50  0001 C CNN
F 3 "~" H 1700 1400 50  0001 C CNN
	1    1700 1400
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0117
U 1 1 611E2343
P 1550 1400
F 0 "#PWR0117" H 1550 1150 50  0001 C CNN
F 1 "GND" V 1555 1272 50  0000 R CNN
F 2 "" H 1550 1400 50  0001 C CNN
F 3 "" H 1550 1400 50  0001 C CNN
	1    1550 1400
	0    1    1    0   
$EndComp
Wire Wire Line
	1850 1400 1950 1400
Wire Wire Line
	2050 1800 1950 1800
$Comp
L Device:R R38
U 1 1 6125A9C1
P 3400 2450
F 0 "R38" H 3470 2496 50  0000 L CNN
F 1 "10K" H 3470 2405 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3330 2450 50  0001 C CNN
F 3 "~" H 3400 2450 50  0001 C CNN
	1    3400 2450
	1    0    0    -1  
$EndComp
Connection ~ 3400 2600
Wire Wire Line
	3400 2600 3650 2600
$Comp
L power:GND #PWR0120
U 1 1 6125B800
P 3400 2000
F 0 "#PWR0120" H 3400 1750 50  0001 C CNN
F 1 "GND" H 3405 1827 50  0000 C CNN
F 2 "" H 3400 2000 50  0001 C CNN
F 3 "" H 3400 2000 50  0001 C CNN
	1    3400 2000
	-1   0    0    1   
$EndComp
$Comp
L Device:LED D14
U 1 1 6125AEBC
P 3400 2150
F 0 "D14" H 3393 1895 50  0000 C CNN
F 1 "LED" H 3393 1986 50  0000 C CNN
F 2 "LED_SMD:LED_0805_2012Metric" H 3400 2150 50  0001 C CNN
F 3 "~" H 3400 2150 50  0001 C CNN
	1    3400 2150
	0    1    1    0   
$EndComp
Connection ~ 1950 1800
Wire Wire Line
	2750 2700 2550 2700
$Comp
L power:GNDA #PWR0126
U 1 1 616E472D
P 2150 4750
F 0 "#PWR0126" H 2150 4500 50  0001 C CNN
F 1 "GNDA" V 2155 4622 50  0000 R CNN
F 2 "" H 2150 4750 50  0001 C CNN
F 3 "" H 2150 4750 50  0001 C CNN
	1    2150 4750
	1    0    0    -1  
$EndComp
$Comp
L MCU_Microchip_ATmega:ATmega328-AU U1
U 1 1 60C93DCF
P 1950 3300
F 0 "U1" H 1950 3400 50  0000 C CNN
F 1 "ATmega328-AU" H 1900 3250 50  0000 C CNN
F 2 "Package_QFP:TQFP-32_7x7mm_P0.8mm" H 1950 3300 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega328_P%20AVR%20MCU%20with%20picoPower%20Technology%20Data%20Sheet%2040001984A.pdf" H 1950 3300 50  0001 C CNN
	1    1950 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 4800 2000 4800
Wire Wire Line
	2000 4800 2000 4750
Wire Wire Line
	2000 4750 2150 4750
Connection ~ 1950 4800
Text Label 2550 4400 0    50   ~ 0
Lora_CS
Text Label 2550 4500 0    50   ~ 0
G2
Text Label 2550 4300 0    50   ~ 0
G1
Text Label 2550 4200 0    50   ~ 0
G0
Wire Wire Line
	2550 3400 3200 3400
Wire Wire Line
	2550 3600 3500 3600
$Comp
L MEM2075-00-140-01-A:MEM2075-00-140-01-A J12
U 1 1 616D2996
P 5200 7200
F 0 "J12" H 5200 7767 50  0000 C CNN
F 1 "MEM2075-00-140-01-A" H 5200 7676 50  0000 C CNN
F 2 "MEM2075-00-140-01-A:GCT_MEM2075-00-140-01-A" H 5200 7200 50  0001 L BNN
F 3 "" H 5200 7200 50  0001 L BNN
F 4 "A" H 5200 7200 50  0001 L BNN "PARTREV"
F 5 "GCT" H 5200 7200 50  0001 L BNN "MANUFACTURER"
F 6 "Manufacturer recommendations" H 5200 7200 50  0001 L BNN "STANDARD"
	1    5200 7200
	1    0    0    -1  
$EndComp
Wire Notes Line
	8300 4500 11050 4500
Wire Notes Line
	8300 6400 11050 6400
Text Label 4500 7500 2    50   ~ 0
SD_CS
NoConn ~ 4500 7400
NoConn ~ 4500 7300
Text Label 4500 6900 2    50   ~ 0
SCK
Text Label 4500 7000 2    50   ~ 0
MOSI
Text Label 4500 7200 2    50   ~ 0
MISO
Wire Wire Line
	5900 7200 6100 7200
Text Label 6300 7200 0    50   ~ 0
Card_Detect
$Comp
L Device:R R41
U 1 1 6185CA1F
P 6100 7050
F 0 "R41" H 6170 7096 50  0000 L CNN
F 1 "10K" H 6170 7005 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6030 7050 50  0001 C CNN
F 3 "~" H 6100 7050 50  0001 C CNN
	1    6100 7050
	1    0    0    -1  
$EndComp
Connection ~ 6100 7200
Wire Wire Line
	6100 7200 6300 7200
Wire Wire Line
	5900 6900 6100 6900
Wire Wire Line
	5900 7500 5900 7450
$Comp
L power:GND #PWR0121
U 1 1 6189CF7F
P 5900 7450
F 0 "#PWR0121" H 5900 7200 50  0001 C CNN
F 1 "GND" H 5905 7277 50  0000 C CNN
F 2 "" H 5900 7450 50  0001 C CNN
F 3 "" H 5900 7450 50  0001 C CNN
	1    5900 7450
	0    -1   -1   0   
$EndComp
Connection ~ 5900 7450
Wire Wire Line
	5900 7450 5900 7400
Wire Notes Line
	3750 6550 6800 6550
Wire Notes Line
	6800 6550 6800 7650
Wire Notes Line
	6800 7650 3750 7650
Wire Notes Line
	3750 7650 3750 6550
Text Notes 3800 6700 0    98   ~ 0
SD-Reader
Text Label 2550 2300 0    50   ~ 0
Card_Detect
Wire Wire Line
	2550 2600 3400 2600
$Comp
L 9C-8.000MEEJ-T:9C-8.000MEEJ-T Y1
U 1 1 60CC095F
P 2950 2750
F 0 "Y1" H 2800 2800 50  0000 C CNN
F 1 "9C-8.000MEEJ-T" H 3350 2800 50  0000 C CNN
F 2 "9C-8:XTAL_9C-8.000MEEJ-T" H 2950 2750 50  0001 L BNN
F 3 "" H 2950 2750 50  0001 L BNN
F 4 "Manufacturer Recommendations" H 2950 2750 50  0001 L BNN "STANDARD"
F 5 "" H 2950 2750 50  0001 L BNN "PARTREV"
F 6 "TXC" H 2950 2750 50  0001 L BNN "MANUFACTURER"
	1    2950 2750
	1    0    0    -1  
$EndComp
Text Label 2550 3200 0    50   ~ 0
SD_CS
$Comp
L Device:R R37
U 1 1 60ECCED9
P 2600 1750
F 0 "R37" H 2670 1796 50  0000 L CNN
F 1 "10K" H 2670 1705 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2530 1750 50  0001 C CNN
F 3 "~" H 2600 1750 50  0001 C CNN
	1    2600 1750
	0    1    1    0   
$EndComp
$Comp
L Device:LED D13
U 1 1 60ECE5BB
P 2900 1750
F 0 "D13" H 2893 1495 50  0000 C CNN
F 1 "LED" H 2893 1586 50  0000 C CNN
F 2 "LED_SMD:LED_0805_2012Metric" H 2900 1750 50  0001 C CNN
F 3 "~" H 2900 1750 50  0001 C CNN
	1    2900 1750
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0145
U 1 1 60ECEC62
P 3050 1750
F 0 "#PWR0145" H 3050 1500 50  0001 C CNN
F 1 "GND" H 3055 1577 50  0000 C CNN
F 2 "" H 3050 1750 50  0001 C CNN
F 3 "" H 3050 1750 50  0001 C CNN
	1    3050 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 1750 2050 1750
Wire Wire Line
	2050 1750 2050 1800
Connection ~ 2050 1800
Text Notes 8400 4750 0    118  ~ 0
LoRaWAN
Wire Notes Line
	3700 1100 4900 1100
Wire Notes Line
	4900 1100 4900 2000
Wire Notes Line
	4900 2000 3700 2000
Wire Notes Line
	3700 2000 3700 1100
Text Notes 3750 1200 0    59   ~ 0
FTDI Connector Plug
Wire Wire Line
	1950 1400 2350 1400
Connection ~ 1950 1400
Connection ~ 3500 3600
Wire Wire Line
	3650 3500 3900 3500
Connection ~ 3650 3500
Wire Wire Line
	2550 3500 3650 3500
Wire Wire Line
	4100 3600 4150 3600
Connection ~ 4100 3600
Wire Wire Line
	4100 3300 4100 3600
$Comp
L Device:R R39
U 1 1 6102E553
P 4100 3150
F 0 "R39" H 4170 3196 50  0000 L CNN
F 1 "10K" H 4170 3105 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4030 3150 50  0001 C CNN
F 3 "~" H 4100 3150 50  0001 C CNN
	1    4100 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 60FB02A7
P 3700 4200
F 0 "#PWR0106" H 3700 3950 50  0001 C CNN
F 1 "GND" H 3705 4027 50  0000 C CNN
F 2 "" H 3700 4200 50  0001 C CNN
F 3 "" H 3700 4200 50  0001 C CNN
	1    3700 4200
	1    0    0    -1  
$EndComp
$Comp
L FSM8JH:FSM8JH S3
U 1 1 60EE21C4
P 3600 3950
F 0 "S3" H 3600 4265 50  0000 C CNN
F 1 "FSM8JH" H 3600 4174 50  0000 C CNN
F 2 "FSM8JH:SW_FSM8JH" H 3600 3950 50  0001 L BNN
F 3 "" H 3600 3950 50  0001 L BNN
F 4 "TE CONNECTIVITY" H 3600 3950 50  0001 L BNN "MANUFACTURER"
F 5 "C10" H 3600 3950 50  0001 L BNN "PARTREV"
F 6 "MANUFACTURER RECOMMENDATIONS" H 3600 3950 50  0001 L BNN "STANDARD"
	1    3600 3950
	0    1    -1   0   
$EndComp
Text Label 4450 3600 0    50   ~ 0
DTR
$Comp
L Device:C C20
U 1 1 60EDF877
P 4300 3600
F 0 "C20" V 4048 3600 50  0000 C CNN
F 1 "0.1uF" V 4139 3600 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4338 3450 50  0001 C CNN
F 3 "~" H 4300 3600 50  0001 C CNN
	1    4300 3600
	0    1    1    0   
$EndComp
Text Label 3900 3500 0    50   ~ 0
SCL
$Comp
L Device:R R2
U 1 1 6082B716
P 3650 3350
F 0 "R2" H 3720 3396 50  0000 L CNN
F 1 "10K" H 3720 3305 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3580 3350 50  0001 C CNN
F 3 "~" H 3650 3350 50  0001 C CNN
	1    3650 3350
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0105
U 1 1 60AAAA83
P 3650 3200
F 0 "#PWR0105" H 3650 3050 50  0001 C CNN
F 1 "+3.3V" H 3665 3373 50  0000 C CNN
F 2 "" H 3650 3200 50  0001 C CNN
F 3 "" H 3650 3200 50  0001 C CNN
	1    3650 3200
	1    0    0    -1  
$EndComp
Wire Notes Line
	700  700  5100 700 
Wire Notes Line
	5100 700  5100 5150
Wire Notes Line
	5100 5150 700  5150
Wire Notes Line
	700  5150 700  700 
Text Notes 850  950  0    118  ~ 0
Master ATMega328
$Comp
L power:+3.3V #PWR0156
U 1 1 615E0B98
P 4100 3000
F 0 "#PWR0156" H 4100 2850 50  0001 C CNN
F 1 "+3.3V" H 4115 3173 50  0000 C CNN
F 2 "" H 4100 3000 50  0001 C CNN
F 3 "" H 4100 3000 50  0001 C CNN
	1    4100 3000
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0184
U 1 1 61C216F8
P 9200 5000
F 0 "#PWR0184" H 9200 4850 50  0001 C CNN
F 1 "+3.3V" H 9215 5173 50  0000 C CNN
F 2 "" H 9200 5000 50  0001 C CNN
F 3 "" H 9200 5000 50  0001 C CNN
	1    9200 5000
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0185
U 1 1 61CB3E96
P 10800 5300
F 0 "#PWR0185" H 10800 5150 50  0001 C CNN
F 1 "+3.3V" H 10815 5473 50  0000 C CNN
F 2 "" H 10800 5300 50  0001 C CNN
F 3 "" H 10800 5300 50  0001 C CNN
	1    10800 5300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0186
U 1 1 61CB42D2
P 8750 5300
F 0 "#PWR0186" H 8750 5150 50  0001 C CNN
F 1 "+3.3V" H 8765 5473 50  0000 C CNN
F 2 "" H 8750 5300 50  0001 C CNN
F 3 "" H 8750 5300 50  0001 C CNN
	1    8750 5300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0187
U 1 1 61CE7F9D
P 6100 6900
F 0 "#PWR0187" H 6100 6750 50  0001 C CNN
F 1 "+3.3V" H 6115 7073 50  0000 C CNN
F 2 "" H 6100 6900 50  0001 C CNN
F 3 "" H 6100 6900 50  0001 C CNN
	1    6100 6900
	1    0    0    -1  
$EndComp
Connection ~ 6100 6900
$Comp
L Connector:Conn_01x16_Male J2
U 1 1 60DF783B
P 1250 6850
F 0 "J2" H 1222 6824 50  0000 R CNN
F 1 "Conn_01x16_Male" H 1222 6733 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x16_P2.54mm_Vertical" H 1250 6850 50  0001 C CNN
F 3 "~" H 1250 6850 50  0001 C CNN
	1    1250 6850
	-1   0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0103
U 1 1 60DFBA19
P 1050 6150
F 0 "#PWR0103" H 1050 6000 50  0001 C CNN
F 1 "+3.3V" H 1065 6323 50  0000 C CNN
F 2 "" H 1050 6150 50  0001 C CNN
F 3 "" H 1050 6150 50  0001 C CNN
	1    1050 6150
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 60DFC304
P 800 6250
F 0 "#PWR0104" H 800 6000 50  0001 C CNN
F 1 "GND" H 805 6077 50  0000 C CNN
F 2 "" H 800 6250 50  0001 C CNN
F 3 "" H 800 6250 50  0001 C CNN
	1    800  6250
	0    1    1    0   
$EndComp
Wire Wire Line
	800  6250 1050 6250
Text Label 2550 4000 0    50   ~ 0
PD2
Text Label 2550 4100 0    50   ~ 0
PD3
Text Label 1050 6950 2    50   ~ 0
PD3
Text Label 1050 6850 2    50   ~ 0
PD2
Text Label 1050 6350 2    50   ~ 0
Tx
Text Label 1050 6750 2    50   ~ 0
Reset
Text Label 1050 6650 2    50   ~ 0
SCL
Text Label 1050 6550 2    50   ~ 0
SDA
Text Label 2550 3300 0    50   ~ 0
PC3
Text Label 1050 6450 2    50   ~ 0
PC3
Text Label 2550 3100 0    50   ~ 0
PC1
Text Label 2550 3000 0    50   ~ 0
PC0
Text Label 2550 6100 2    50   ~ 0
PC1
Text Label 2550 6200 2    50   ~ 0
PC0
Text Label 1350 2400 2    50   ~ 0
ADC7
Text Label 1350 2300 2    50   ~ 0
ADC6
$Comp
L power:GND #PWR0107
U 1 1 60E071FA
P 2400 6400
F 0 "#PWR0107" H 2400 6150 50  0001 C CNN
F 1 "GND" H 2405 6227 50  0000 C CNN
F 2 "" H 2400 6400 50  0001 C CNN
F 3 "" H 2400 6400 50  0001 C CNN
	1    2400 6400
	0    1    1    0   
$EndComp
Text Label 1350 2100 2    50   ~ 0
AREF
$Comp
L Connector:Conn_01x16_Male J4
U 1 1 60E07B82
P 2750 6800
F 0 "J4" H 2722 6774 50  0000 R CNN
F 1 "Conn_01x16_Male" H 2722 6683 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x16_P2.54mm_Vertical" H 2750 6800 50  0001 C CNN
F 3 "~" H 2750 6800 50  0001 C CNN
	1    2750 6800
	-1   0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0108
U 1 1 60E164F9
P 2450 6700
F 0 "#PWR0108" H 2450 6550 50  0001 C CNN
F 1 "+3.3V" H 2465 6873 50  0000 C CNN
F 2 "" H 2450 6700 50  0001 C CNN
F 3 "" H 2450 6700 50  0001 C CNN
	1    2450 6700
	0    -1   -1   0   
$EndComp
Text Label 2550 6800 2    50   ~ 0
SCK
Wire Wire Line
	2400 6400 2550 6400
Wire Wire Line
	2450 6700 2550 6700
Text Label 2550 6900 2    50   ~ 0
MISO
Text Label 2550 7000 2    50   ~ 0
MOSI
Text Label 2550 2200 0    50   ~ 0
PB1
Text Label 2550 2100 0    50   ~ 0
PB0
Text Label 2550 7100 2    50   ~ 0
PB1
Text Label 2550 7200 2    50   ~ 0
PB0
Connection ~ 4050 1650
$Comp
L Connector:Conn_01x06_Female J1
U 1 1 60E21657
P 4250 1450
F 0 "J1" V 4450 1450 50  0000 R CNN
F 1 "Conn_01x06_Female" V 4350 1800 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Horizontal" H 4250 1450 50  0001 C CNN
F 3 "~" H 4250 1450 50  0001 C CNN
	1    4250 1450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4250 1750 4250 1650
$Comp
L power:+3.3V #PWR0155
U 1 1 61575D3E
P 4250 1750
F 0 "#PWR0155" H 4250 1600 50  0001 C CNN
F 1 "+3.3V" H 4265 1923 50  0000 C CNN
F 2 "" H 4250 1750 50  0001 C CNN
F 3 "" H 4250 1750 50  0001 C CNN
	1    4250 1750
	-1   0    0    1   
$EndComp
Wire Wire Line
	4150 1650 4050 1650
$Comp
L power:GND #PWR0118
U 1 1 611E26A9
P 4050 1650
F 0 "#PWR0118" H 4050 1400 50  0001 C CNN
F 1 "GND" H 4055 1477 50  0000 C CNN
F 2 "" H 4050 1650 50  0001 C CNN
F 3 "" H 4050 1650 50  0001 C CNN
	1    4050 1650
	1    0    0    -1  
$EndComp
Text Label 4450 1650 3    50   ~ 0
Tx
Text Label 4350 1650 3    50   ~ 0
Rx
Text Label 4550 1650 3    50   ~ 0
DTR
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J5
U 1 1 60CFFDCC
P 9600 3800
F 0 "J5" H 9650 4117 50  0000 C CNN
F 1 "Conn_02x03_Odd_Even" H 9650 4026 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical" H 9600 3800 50  0001 C CNN
F 3 "~" H 9600 3800 50  0001 C CNN
	1    9600 3800
	1    0    0    -1  
$EndComp
Text Label 9400 3900 2    50   ~ 0
Reset
$Comp
L power:GND #PWR0109
U 1 1 60D013BF
P 9900 3900
F 0 "#PWR0109" H 9900 3650 50  0001 C CNN
F 1 "GND" H 9905 3727 50  0000 C CNN
F 2 "" H 9900 3900 50  0001 C CNN
F 3 "" H 9900 3900 50  0001 C CNN
	1    9900 3900
	1    0    0    -1  
$EndComp
Text Label 9900 3800 0    50   ~ 0
MOSI
$Comp
L power:+3.3V #PWR0110
U 1 1 60D01993
P 10200 3700
F 0 "#PWR0110" H 10200 3550 50  0001 C CNN
F 1 "+3.3V" H 10215 3873 50  0000 C CNN
F 2 "" H 10200 3700 50  0001 C CNN
F 3 "" H 10200 3700 50  0001 C CNN
	1    10200 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 3700 10200 3700
Text Label 9400 3800 2    50   ~ 0
SCK
Text Label 9400 3700 2    50   ~ 0
MISO
Wire Wire Line
	3500 3600 4100 3600
$Comp
L BK-18650-PC2:BK-18650-PC2 BT?
U 1 1 60DF2220
P 6150 2000
F 0 "BT?" H 6150 2267 50  0000 C CNN
F 1 "BK-18650-PC2" H 6150 2176 50  0000 C CNN
F 2 "BAT_BK-18650-PC2" H 6150 2000 50  0001 L BNN
F 3 "" H 6150 2000 50  0001 L BNN
F 4 "MPD" H 6150 2000 50  0001 L BNN "MANUFACTURER"
F 5 "F" H 6150 2000 50  0001 L BNN "PARTREV"
F 6 "21.41 mm" H 6150 2000 50  0001 L BNN "MAXIMUM_PACKAGE_HEIGHT"
F 7 "Manufacturer Recommendations" H 6150 2000 50  0001 L BNN "STANDARD"
	1    6150 2000
	0    1    1    0   
$EndComp
$Comp
L AOD417:AOD417 Q?
U 1 1 60E20ADB
P 6700 1350
F 0 "Q?" V 7035 1350 50  0000 C CNN
F 1 "AOD417" V 6944 1350 50  0000 C CNN
F 2 "DPAK228P994X240_3N" H 6700 1350 50  0001 L BNN
F 3 "" H 6700 1350 50  0001 L BNN
F 4 "IPC7351B" H 6700 1350 50  0001 L BNN "STANDARD"
F 5 "U" H 6700 1350 50  0001 L BNN "PARTREV"
F 6 "Alpha & Omega Semiconductor Inc." H 6700 1350 50  0001 L BNN "MANUFACTURER"
	1    6700 1350
	0    -1   -1   0   
$EndComp
$Comp
L Battery_Management:DW06D U?
U 1 1 60E229E6
P 7300 1500
F 0 "U?" H 7300 1631 79  0000 C CNN
F 1 "DW06D" H 7300 1496 79  0000 C CNN
F 2 "" H 7300 1500 79  0001 C CNN
F 3 "" H 7300 1500 79  0001 C CNN
	1    7300 1500
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:TPS73633DBV U?
U 1 1 60E23B76
P 8700 1600
F 0 "U?" H 8700 1967 50  0000 C CNN
F 1 "TPS73633DBV" H 8700 1876 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 8700 1925 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/tps736.pdf" H 8700 1550 50  0001 C CNN
	1    8700 1600
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Switching:MT3608 U?
U 1 1 60E28CFC
P 7450 2950
F 0 "U?" H 7450 3317 50  0000 C CNN
F 1 "MT3608" H 7450 3226 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6" H 7500 2700 50  0001 L CIN
F 3 "https://www.olimex.com/Products/Breadboarding/BB-PWR-3608/resources/MT3608.pdf" H 7200 3400 50  0001 C CNN
	1    7450 2950
	1    0    0    -1  
$EndComp
$Comp
L pspice:INDUCTOR L?
U 1 1 60E2AB2E
P 6300 3400
F 0 "L?" H 6300 3615 50  0000 C CNN
F 1 "INDUCTOR" H 6300 3524 50  0000 C CNN
F 2 "" H 6300 3400 50  0001 C CNN
F 3 "~" H 6300 3400 50  0001 C CNN
	1    6300 3400
	1    0    0    -1  
$EndComp
$Comp
L SS34:SS34 D?
U 1 1 60E2B8A0
P 6900 3600
F 0 "D?" H 6900 3817 50  0000 C CNN
F 1 "SS34" H 6900 3726 50  0000 C CNN
F 2 "DIOM7959X265N" H 6900 3600 50  0001 L BNN
F 3 "" H 6900 3600 50  0001 L BNN
F 4 "2.65mm" H 6900 3600 50  0001 L BNN "MAXIMUM_PACKAGE_HEIGHT"
F 5 "36301" H 6900 3600 50  0001 L BNN "SNAPEDA_PACKAGE_ID"
F 6 "On Semiconductor" H 6900 3600 50  0001 L BNN "MANUFACTURER"
F 7 "31 Aug 2016" H 6900 3600 50  0001 L BNN "PARTREV"
F 8 "IPC-7351B" H 6900 3600 50  0001 L BNN "STANDARD"
	1    6900 3600
	1    0    0    -1  
$EndComp
$EndSCHEMATC