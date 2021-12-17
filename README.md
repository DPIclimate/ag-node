# ag-node

<img src="Imgs/IMG_0173.GIF" style="display: flex; align-items: center; justify-content: space-around;"/>

## Description
The Universal Agriculture Node (AgU-Node) is a device aimed at providing a range of environmental and biological telemetry from a single device. It is capable of reading from three weigh stations simultaneously using dedicated low-powered micro-controllers (ATMega328) while maintaining expandability for external sensors such as trough level sensors, weather stations, soil moisture probes etc.

## Features
- Teensy 3.6 as master controller
- Three ATmega328 micro-controllers and dedicated HX711 weigh scale readers for asynchronous readings 
- LoRaWAN communications
- Onboard SD card storage
- Debug mode
- Solar charger:
	- Fault and charging LED indicators
	- Reverse polarity protection
	- Adjustable solar input voltages
	- ON/OFF switch to shut-off system
	- Adjustable current output
- Regulated outputs:
	- 5 volt:
		- Always on when battery or solar connected
		- LED indicator (desolder for low-power mode)
	- 9 volt:
		- Programatically enabled (allowing shut-off when not in use)
		- Adjustable output (9 - 18 V)
		- LED indicator
- Battery:
	- 3 x 18650
	- Reverse polarity protection
	- On PCB test points
	- Protection:
		- Push button switch to activate protection
		- Over-discharge/charge
- Weigh scale amplifier (HX711)
	- Screw terminal input
	- Adjustable frequency (10 Hz default, 80 Hz with jumper cut)
- SDI-12 inputs:
	- 5 V always on
	- 9 V can turn off
	- Useful for weather stations, soil moisture probes, tank level sensors etc.
- Power isolation for unneeded components


