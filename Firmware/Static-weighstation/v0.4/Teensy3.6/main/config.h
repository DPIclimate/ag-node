#ifndef CONFIG
#define CONFIG

/*  DEBUG mode
 *  Comment out if not needed
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


/*  Number of bytes to send over LoRaWAN
 *  Needs to match the total size of parameters_t in sensors.h
 */
#ifndef WEIGH_PAYLOAD_SIZE
#define WEIGH_PAYLOAD_SIZE 22
#endif


/*  Number of bytes to send over LoRaWAN
 *  Needs to match the total size of parameters_t in sensors.h
 */
#ifndef SENSORS_PAYLOAD_SIZE
#define SENSORS_PAYLOAD_SIZE 19
#endif


#include "lmic.h"
#include <hal/hal.h>
/* LoRaWAN route identifiers
 */
// Application unique identifier (Format = LSB)
const u1_t PROGMEM APPEUI[8] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// Device unique identifier (Format = LSB)
const u1_t PROGMEM DEVEUI[8] = { 0x96, 0x51, 0x04, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };

// APP-KEY (Sensitive - Remove if going public) (Format = MSB)
const u1_t PROGMEM APPKEY[16] = { 0x66, 0x6E, 0x40, 0x03, 0x05, 0x8E, 0x52, 0xE2, 0xA1, 0x03, 0xD2, 0x16, 0xFE, 0xD0, 0x59, 0x89 };


/* SD-card filename must have a .txt file extension
 */
#ifndef RAW_DATA_FILENAME
#define RAW_DATA_FILENAME "raw.txt"
#endif

#ifndef PARAMETERS_FILENAME
#define PARAMETERS_FILENAME "para.txt"
#endif


/*
 * OneWire interface (temperature probe)
 * Theres two one wire intefaces on pin 38 (J7) and 37 (J6)
 * Need to implement the dev on pin 37
 */
#ifndef ONE_WIRE_BUS
#define ONE_WIRE_BUS 38
#endif

#endif
