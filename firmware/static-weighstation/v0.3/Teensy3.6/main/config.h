#ifndef CONFIG
#define CONFIG


/*  Number of bytes to send over LoRaWAN
 *  Needs to match the total size of parameters_t in sensors.h
 */
#ifndef PAYLOAD_SIZE
#define PAYLOAD_SIZE 19
#endif


/* RESPONSE_SIZE is the expected array size from the weigh scale
 * RESPONSE_SIZE = size of weights + size of timestamps
 */
#ifndef RESPONSE_SIZE
#define RESPONSE_SIZE 400
#endif


/* DEBUG mode
 * Comment out if not needed
 */
#ifndef DEBUG
#define DEBUG
#endif

/* LOW_POWER mode
 * Comment out if not needed
 */
//#ifndef LOW_POWER
//#define LOW_POWER
//#endif


/* LoRaWAN Mode
 * Comment out if not in use
 */
#ifndef ENABLE_LORAWAN
#define ENABLE_LORAWAN
#endif


// Interrupt pins 
#define WEIGH_SCALE_1 21
#define WEIGH_SCALE_2 20
#define WEIGH_SCALE_3 17


#include "lmic.h"
#include <hal/hal.h>
/* LoRaWAN route identifiers
 */
// Application unique identifier (Format = LSB)
const u1_t PROGMEM APPEUI[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };

// Device unique identifier (Format = LSB)
const u1_t PROGMEM DEVEUI[8] = { 0x96, 0x51, 0x04, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };

// APP-KEY (Sensitive - Remove if going public) (Format = MSB)
const u1_t PROGMEM APPKEY[16] = { 0x66, 0x6E, 0x40, 0x03, 0x05, 0x8E, 0x52, 0xE2, 0xA1, 0x03, 0xD2, 0x16, 0xFE, 0xD0, 0x59, 0x89 };


/* SD-card filename, must have a .txt file extension
 */
#ifndef RAW_DATA_FILENAME
#define RAW_DATA_FILENAME "raw.txt"
#endif

#ifndef PARAMETERS_FILENAME
#define PARAMETERS_FILENAME "para.txt"
#endif


#endif
