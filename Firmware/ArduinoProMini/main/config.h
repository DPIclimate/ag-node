#ifndef CONFIG
#define CONFIG


/* DEVICE_ID specific to each ATmega328 (and therefore weighscale)
 */
#ifndef DEVICE_ID
#define DEVICE_ID 1
#endif


/* WAKE_UP used for interrupting Teensy
 * Default = 11
 */
#ifndef WAKE_UP
#define WAKE_UP 11 // Output for interrupt
#endif


/* DEBUG mode
 * 0 = off
 * 1 = on
 */
#ifndef DEBUG
#define DEBUG 1
#endif 


/* HIGH_PRECISION uses floats rather than ints for weights. 
 * If no scale is connected or values are greater than 655.36 kg this will overflow.
 * Needs to be defined on both the ATMega328 and the Teensy
 */
//#ifndef HIGH_PRECISION
//#define HIGH_PRECISION
//#endif


#endif
