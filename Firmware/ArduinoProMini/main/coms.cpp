/*
 * Static functions for communications between Feather M0 and node. 
 */

#include "coms.h"

static bool state = false;
bool weightCaptured = false;

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


void lorawan_send(osjob_t* j, uint8_t* payload){
    Serial.println("Checking if job can be done");
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
      Serial.println(F("OP_TXRXPEND, not sending"));
    }
    else {
      Serial.println("Preparing Payload / Running Job");

      Communications::set_state(false);
      
      LMIC_setTxData2(1, payload, 12, 0); // TODO: Manually setting up array size, need to change this
      
      Serial.println("Payload queued");
    }
}


void printHex2(unsigned v) {
    v &= 0xff;
    if (v < 16)
        Serial.print('0');
    Serial.print(v, HEX);
}


void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            {
              u4_t netid = 0;
              devaddr_t devaddr = 0;
              u1_t nwkKey[16];
              u1_t artKey[16];
              LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
              Serial.print("netid: ");
              Serial.println(netid, DEC);
              Serial.print("devaddr: ");
              Serial.println(devaddr, HEX);
              Serial.print("AppSKey: ");
              for (size_t i=0; i<sizeof(artKey); ++i) {
                if (i != 0)
                  Serial.print("-");
                printHex2(artKey[i]);
              }
              Serial.println("");
              Serial.print("NwkSKey: ");
              for (size_t i=0; i<sizeof(nwkKey); ++i) {
                      if (i != 0)
                              Serial.print("-");
                      printHex2(nwkKey[i]);
              }
              Serial.println();
            }
            // Disable link check validation (automatically enabled
            // during join, but because slow data rates change max TX
            // size, we don't use it in this example.
            LMIC_setLinkCheckMode(0);
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.println(F("Received "));
              Serial.println(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));
            }
            // Transmission was competed successfully 
            // TODO: This is proberbly blocking if the gateway cannot be reached
            Communications::set_state(true);
            break;

        default:
            Serial.print(F("Unknown event: "));
            Serial.println((unsigned) ev);
            break;
    }
}


void Communications::init(){
  Serial.println("Sending JOB");
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

  Serial.println("Sending JOB");
  lorawan_send(&sendjob, {0});
  Serial.println("Finished Sending JOB");
}


bool Communications::check_state(){
  return state;
}


void Communications::set_state(bool s){
  state = s;
}


void Communications::request_send(uint8_t* payload){
  lorawan_send(&sendjob, payload);
}
