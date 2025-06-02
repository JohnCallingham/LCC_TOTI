/**
 * TO DO: to be updated for new methods.
 * Also loop() and sendInitialEvents().
 */
...
#define NUM_TOTI 2

#include "TOTI.h"
TOTI toti[NUM_TOTI];

// Define the pins for the TOTI inputs.
uint8_t inputPin[NUM_TOTI] = { 19, 18 };

// Define the pins for the TOTI LED outputs.
uint8_t outputPin[NUM_TOTI] = { 6, 10 };

/**
 * userState() is called when JMRI queries the state of an event index.
 */
enum evStates { VALID=4, INVALID=5, UNKNOWN=7 };
uint8_t userState(uint16_t index) {
  //Serial.printf("\nIn userState() for event index = %d", index);

  // Determine which TOTI object has this event index.
  for (uint8_t i=0; i<NUM_TOTI; i++) {
    //Serial.printf("\nChecking for matching TOTI %d", i);
    if (toti[i].eventIndexMatchesThisTOTI(index)) {
      // This TOTI object has this event index.
      //Serial.printf("\nChecking for matching event");
      if (toti[i].eventIndexMatchesCurrentState(index)) {
        return VALID;
      } else {
        return INVALID;
      }
    }
  }

  return UNKNOWN; // In case index is not recognised.
}

void produceFromInputs() {
  // called from loop(), this looks at changes in input pins and
  // and decides which events to fire with OpenLcb.produce(i).
  int eventToSend;

  for (uint8_t i=0; i<NUM_TOTI; i++) {
    // Check for any change to TOTI state and send the appropriate event if required.
    eventToSend = toti[i].process();
    if (eventToSend != -1) OpenLcb.produce(eventToSend);
  }
}

void sendInitialEvents() {
  int eventToSend;

  for (uint8_t i=0; i<NUM_TOTI; i++) {
    eventToSend = toti[i].getEventForCurrentState();
    if (eventToSend != -1) OpenLcb.produce(eventToSend);
  }
}

void setup() {
  ...
  // Initialise all TOTI objects.
  for (uint8_t i=0; i<NUM_TOTI; i++) {
    toti[i].setInputPin(inputPin[i]);
    toti[i].setOutputPin(outputPin[i]);
    toti[i].setEventIndexON((i * 2) + 0);
    toti[i].setEventIndexOFF((i * 2) + 1);
    toti[i].initialise();
  }
}

void loop() {
  // Do OpenLCB/LCC processing.
  Olcb_process();

  // Check for any input processing required.
  produceFromInputs();

  // Attempt to connect to the OpenLCB/LCC hub and reconnect if contact has been lost.
  if (hubConnectionMade()) {
    // Turn the blue LED on.
    digitalWrite(LED_BLUE, LOW);

    sendInitialEvents();
  }

  if (hubConnectionLost()) {
    // Turn the blue LED off.
    digitalWrite(LED_BLUE, HIGH);
  }
}
