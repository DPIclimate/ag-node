/*
 * Static functions for communications between Feather M0 and node. 
 */
 
#include "coms.h"


void os_getArtEui (u1_t* buf) {
  // Application unique identifier to buffer
  memcpy_P(buf, APPEUI, 8);
}


void os_getDevEui (u1_t* buf) {
  // Copy Device unique identifier to buffer
  memcpy_P(buf, DEVEUI, 8);
}


void os_getDevKey (u1_t* buf) {
  // Copy APP-Key to buffer
  memcpy_P(buf, APPKEY, 16);
}


const lmic_pinmap lmic_pins = {
  .nss = 6,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 9,
  .dio = { 4, 5, LMIC_UNUSED_PIN },
};


void do_send(osjob_t* j, uint8_t* payload){
  LMIC_setTxData2(1, payload, sizeof(payload)-1, 0); // TODO: Manually setting up array size, need to change this
  Communications::state = false;
}


void onEvent (ev_t ev) {
    switch(ev) {
        case EV_TXCOMPLETE:
            // Transmission was competed successfully 
            // TODO: This is proberbly blocking if the gateway cannot be reached
            Serial.println("TC");
            Communications::state = true;
            break;
        default:
          Serial.println("UA");
    }
}


void Communications::init(){
  // LoRaMAC-in-C (LMIC) init
  os_init();
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();
  // Compensates for inaccurate clock
  LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);
  // Disable link-check mode and ADR, because ADR tends to complicate testing.
  LMIC_setLinkCheckMode(0);
  // Set the data rate to Spreading Factor 7.  This is the fastest supported rate for 125 kHz channels, and it
  // minimizes air time and battery power. Set the transmission power to 14 dBi (25 mW).
  LMIC_setDrTxpow(DR_SF7,14);
  // in the US, with TTN, it saves join time if we start on subband 1 (channels 8-15). This will
  // get overridden after the join by parameters from the network. If working with other
  // networks or in other regions, this will need to be changed.
  LMIC_selectSubBand(1);
  // Disable data rate adaptation
  // This mode optimises data rates for transmission 
  // See: https://www.thethingsnetwork.org/docs/lorawan/adaptive-data-rate.html 
  LMIC_setAdrMode(0);
  // Initial send starts coms
  Serial.println("IL");
  request_send("Test");
  Serial.println("RS");
}


void Communications::request_send(uint8_t* payload){
  do_send(&sendjob, payload);
}
