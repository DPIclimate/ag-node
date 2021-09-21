/*
 * Functions for communications between node and TTN.
 */

#include "lora.h"

static bool state = false;

// LMIC job
static osjob_t sendjob; 

void Lora::init() {
  /* 
   *  Initialise LoRaWAN communications with appropiate settings.
   *  Create interrupt for testing LoRaWAN.
   */
  #ifdef DEBUG
    Serial.print("[LORAWAN]: Initialising... ");
  #endif
  
  // LoRaMAC-in-C (LMIC) init
  os_init();
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();
  // Compensates for inaccurate clock
//z You might need to define LMIC_ENABLE_arbitrary_clock_error.
//z See https://www.thethingsnetwork.org/forum/t/using-lmic-setclockerror-on-mcci-lmic-howto/39776  
  LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);
  // Disable link-check mode and ADR, because ADR tends to complicate testing.
  LMIC_setLinkCheckMode(0);
  // Set the data rate to Spreading Factor 7.  This is the fastest supported rate for 125 kHz channels, 
  // and it minimizes air time and battery power. Set the transmission power to 14 dBi (25 mW).
  LMIC_setDrTxpow(DR_SF7,14);
  // US ONLY, with TTN, it saves join time if we start on subband 1 (channels 8-15). This will
  // get overridden after the join by parameters from the network. If working with other
  // networks or in other regions, this will need to be changed.
//  LMIC_selectSubBand(1);
  // Disable data rate adaptation
  // This mode optimises data rates for transmission 
  // See: https://www.thethingsnetwork.org/docs/lorawan/adaptive-data-rate.html 
  LMIC_setAdrMode(0);

  // Send connection message to start comms with TTN
  int8_t connect[4] = {1, 2, 3, 4};
  Lora::request_send(connect, 3);
  
  // Setup interrupt button
  pinMode(IRQ, INPUT_PULLDOWN);
  attachInterrupt(IRQ, Lora::test_connection, RISING);
  
  #ifdef DEBUG
    Serial.println("success");
  #endif
}

void Lora::test_connection() {
  /*
   * Send a test packet to TTN 
   * @return Connection status on TTN (should be "success")
   */
  #ifdef DEBUG
    Serial.println("[LORAWAN]: Sending test message");
  #endif
  
  int8_t connect[4] = {1, 2, 3, 4};
  Lora::request_send(connect, 3);
  
  while(!Lora::check_state()) {
    os_runloop_once();
  }
}


void lorawan_send(osjob_t* j, int8_t* payload, uint8_t port) {
  /*
   * Send payload (packet) over LoRaWAN.
   * @param j The LMIC job (sendjob).
   * @param payload The payload to send.
   * @param port The fPort (TTN) to use - used to differentiate payloads.
   */
  Lora::set_state(false);
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND) {
    Serial.println(F("OP_TXRXPEND, not sending"));
  } else {
    if(port == 1) {
      // Send weighstation data over LoRaWAN port 0
      LMIC_setTxData2(port, (uint8_t*)payload, WEIGH_PAYLOAD_SIZE, 0);
    }
    else if(port == 2) {
      // Send sensor data over LoRaWAN port 1
      // sizeof(payload) doesn't work here as its a pointer to an array
      LMIC_setTxData2(port, (uint8_t*)payload, SENSORS_PAYLOAD_SIZE, 0);
    } 
    else{
      LMIC_setTxData2(port, (uint8_t*)payload, sizeof(payload), 0);
    }
  }
}


void Lora::request_send(int8_t* payload, uint8_t port) {
  /*
   * Interface between LMIC functions and Lora class.
   * Used so the LMIC sendjob varaible doesn't have to be passed around.
   * @param payload Payload to send.
   * @param port The fPort (TTN) to use - used to differentiate payloads.
   */
  lorawan_send(&sendjob, payload, port);
}


void printHex2(unsigned v) {
  /*
   * LMIC function to convert key to HEX
   */
  v &= 0xff;
  if (v < 16)
      Serial.print('0');
  Serial.print(v, HEX);
}


void onEvent (ev_t ev) {
  /*
   * LMIC function to identify LoRaWAN state.
   */
  switch(ev) {
    case EV_SCAN_TIMEOUT:
        Serial.println(F("EV_SCAN_TIMEOUT"));
        break;
    case EV_JOINING:
        Serial.println(F("EV_JOINING"));
        break;
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
    case EV_JOIN_FAILED:
        Serial.println(F("EV_JOIN_FAILED"));
        break;
    case EV_REJOIN_FAILED:
        Serial.println(F("EV_REJOIN_FAILED"));
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
        Serial.println();
        // Transmission was competed successfully 
        // TODO: This is probably blocking if the gateway cannot be reached
        Lora::set_state(true);
        
        break;
    case EV_LOST_TSYNC:
        Serial.println(F("EV_LOST_TSYNC"));
        break;
    case EV_RESET:
        Serial.println(F("EV_RESET"));
        break;
    case EV_RXCOMPLETE:
        // data received in ping slot
        Serial.println(F("EV_RXCOMPLETE"));
        break;
    case EV_TXSTART:
        Serial.println(F("EV_TXSTART"));
        break;
    case EV_JOIN_TXCOMPLETE:
        Serial.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
        break;
    default:
        Serial.print(F("Unknown event: "));
        Serial.println((unsigned) ev);
        break;
  }
}

//z I used this to check the state of the LoRa stack:
//z return LMIC.opmode & (OP_JOINING | OP_TXRXPEND | OP_SHUTDOWN) ? false : true;
//z That might not be great because I would see occasional huge delays in sending
//z messages, but it might be worth looking at that variable.
bool Lora::check_state() {
  /*
   * Check the state of a message which has been qued to send over LoRaWAN.
   * @return state True if sent - false if pending.
   */
  return state;
}


void Lora::set_state(bool s) {
  /*
   * Set the state of a pending message over LoRaWAN.
   * @param s State of message transmission.
   */
  state = s;
}


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
  /*
   * Pins used for SPI interface with LoRaWAN radio.
   * RST = Reset pin - 9
   * NSS = Chip select - 10
   * DIO = Radio pins - G0 - 2, G1 - 5, G2 - 6
   * Not shown* = MOSI - 11, MISO - 12, SCK - 13
   */
  .nss = 10,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 9,
  .dio = { 2, 5, 6 },
  .rxtx_rx_active = 0,
  .rssi_cal = 0,
  .spi_freq = 1000000,
};
