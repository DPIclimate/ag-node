## Version 0.3
- Uses Teensy 3.6 as mainboard controller (run at a low clock speed) and defaults to sleep
- Has 3 weigh station ports (controlled by a HX711 and a ATMega328) 
- ATMega328's have interrupts connected to the teensy to wake it up when they want to transfer some data (over I2C)
- Single lorawan module (connected to teensy, can sleep when not in use)
- Designed to be a low-power device with multiple processors for asynchronous readings

