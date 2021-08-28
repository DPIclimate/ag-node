#ifndef CONFIG
#define CONFIG


/* DEBUG mode
 * 0 = off
 * 1 = on (verbose)
 */
#ifndef DEBUG
#define DEBUG
#endif


/* LoRaWAN Mode
 * Comment out if not in use
 */
//#ifndef ENABLE_LORAWAN
//#define ENABLE_LORAWAN
//#endif


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
const u1_t PROGMEM DEVEUI[8] = { 0x25, 0x3C, 0x04, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };

// APP-KEY (Sensitive - Remove if going public) (Format = MSB)
const u1_t PROGMEM APPKEY[16] = { 0x57, 0x3D, 0x7E, 0xF9, 0x42, 0x02, 0x92, 0x5C, 0xC2, 0x0D, 0xE2, 0xB0, 0x4E, 0x9B, 0xB6, 0xF7 };


/* SD-card filename must have a .txt file extension
 */
#ifndef FILENAME
#define FILENAME "test.txt"
#endif


#endif
