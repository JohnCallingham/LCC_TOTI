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
  Serial.printf("\nIn userState() for event index = %d", index);

  // Determine the TOTI for this event index.
  uint8_t myToti = index / 2;
  Serial.printf("\ntoti = %d", myToti);

  // Determine whether this event index is for an ON or an OFF event.
  if (index % 2 == 0) {
    // index is even so this is an ON event.
    // if (toti[myToti].getCurrentState() == State::OCCUPIED) return VALID; else return INVALID;
    if (toti[myToti].isOccupied()) return VALID; else return INVALID;
  } else {
    // index is odd so this is an OFF event.
    // if (toti[myToti].getCurrentState() == State::NOT_OCCUPIED) return VALID; else return INVALID;
    if (toti[myToti].isNotOccupied()) return VALID; else return INVALID;
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
