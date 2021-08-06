#ifndef CONFIG
#define CONFIG


/* DEBUG mode
 * 0 = off
 * 1 = on (verbose)
 */
#ifndef DEBUG
#define DEBUG 1
#endif


// Interrupts 
#define WEIGH_SCALE_1 21
#define WEIGH_SCALE_2 20
#define WEIGH_SCALE_3 17


/* HIGH_PRECISION uses floats rather than ints for weights. 
 * If no scale is connected or values are greater than 655.36 kg this will overflow.
 * Needs to be defined on both the ATMega328 and the Teensy
 */
#ifndef HIGH_PRECISION
#define HIGH_PRECISION
#endif


/* RESPONSE_SIZE is the expected array size from the weigh scale
 * RESPONSE_SIZE = size of weights + size of timestamps
 */
#ifndef RESPONSE_SIZE
#define RESPONSE_SIZE 400
#endif


#include "lmic.h"
#include <hal/hal.h>
/* LoRaWAN route identifiers
 */
// Application unique identifier (Format = LSB)
const u1_t PROGMEM APPEUI[8] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// Device unique identifier (Format = LSB)
const u1_t PROGMEM DEVEUI[8] = { 0x13, 0x39, 0x04, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };

// APP-KEY (Sensitive - Remove if going public) (Format = MSB)
const u1_t PROGMEM APPKEY[16] = { 0x40, 0xF2, 0xC2, 0x46, 0x39, 0x41, 0x8A, 0x3D, 0xA8, 0xCB, 0x99, 0xD5, 0x1C, 0x3A, 0x32, 0x50 };


/* SD-card filename must have a .txt file extension
 */
#ifndef FILENAME
#define FILENAME "test.txt"
#endif


#endif
