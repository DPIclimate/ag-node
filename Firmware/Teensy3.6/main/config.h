#ifndef CONFIG
#define CONFIG


/* DEBUG mode
 * 0 = off
 * 1 = on
 */
#ifndef DEBUG
#define DEBUG 0
#endif


// Interrupts 
#define WEIGH_SCALE_1 21
#define WEIGH_SCALE_2 20
#define WEIGH_SCALE_3 17


/* HIGH_PRECISION uses floats rather than ints for weights. 
 * If no scale is connected or values are greater than 655.36 kg this will overflow.
 * Needs to be defined on both the ATMega328 and the Teensy
 */
//#ifndef HIGH_PRECISION
//#define HIGH_PRECISION
//#endif


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
const u1_t PROGMEM APPEUI[8] = { 0x22, 0xFD, 0x03, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };

// Device unique identifier (Format = LSB)
const u1_t PROGMEM DEVEUI[8] = { 0x47, 0xC1, 0x11, 0x00, 0x00, 0xB6, 0x76, 0x98 };

// APP-KEY (Sensitive - Remove if going public) (Format = MSB)
const u1_t PROGMEM APPKEY[16] = { 0x8C, 0x41, 0x2C, 0xEF, 0x73, 0x63, 0x49, 0xF1, 0x4F, 0xAF, 0xF6, 0x6F, 0x2E, 0x0E, 0x46, 0xD4 };


/* SD-card filename must have a .txt file extension
 */
#ifndef FILENAME
#define FILENAME "test.txt"
#endif


#endif
