#include "TOTI.h"

/**
 * inputPin is active low, i.e. low means OCCUPIED.
 */

// The TOTI constructor also constructs the Debounce object.
TOTI::TOTI(uint8_t totiNumber, uint8_t inputPin, uint8_t testPin) : debounce() {
  this->totiNumber = totiNumber;
  this->testPin = testPin;

  // Set the testPin as an output and set it high so that it doesn't interfere with normal operation.
  pinMode(testPin, OUTPUT);
  digitalWrite(testPin, HIGH);

  // Initialise the Debounce object.
  // Use defaults for other parameters unless overridden by
  //  debounce.set...() methods.
  debounce.setInputPin(inputPin);
  debounce.setSampleTimemS(DEBOUNCE_SAMPLE_TIME_mS);

  // Configure input pin (not done in the Debounce object).
  pinMode(inputPin, INPUT_PULLUP);
  
  // Set the initial state according to the actual state of the input pin.
  if (digitalRead(inputPin) == LOW) {
    currentState = State::OCCUPIED;
  } else {
    currentState = State::NOT_OCCUPIED;
  }
}

void TOTI::setEvents(uint16_t eventIndexOccupied, uint16_t eventIndexNotOccupied) {
  this->eventIndexOccupied = eventIndexOccupied;
  this->eventIndexNotOccupied = eventIndexNotOccupied;
}

void TOTI::sendEventsForCurrentState() {
  if (this->isOccupied()) {
    if (sendEvent) sendEvent(eventIndexOccupied);
  }
  if (this->isNotOccupied()) {
    if (sendEvent) sendEvent(eventIndexNotOccupied);
  }
}

void TOTI::setOccupiedDebounceDelay(uint16_t delaymS) {
  // Convert occupied debounce delay from mS to samples.
  uint16_t samples = delaymS / DEBOUNCE_SAMPLE_TIME_mS;

  // Low indicates occupied.
  debounce.setLowSamples(samples);
}

void TOTI::setNotOccupiedDebounceDelay(uint16_t delaymS) {
  // Convert not occupied debounce delay from mS to samples.
  uint16_t samples = delaymS / DEBOUNCE_SAMPLE_TIME_mS;

  // High indicates not occupied.
  debounce.setHighSamples(samples);
}

void TOTI::eventReceived(uint16_t index) {
  /**
   * Handle the test cycle start and stop events.
   */
  if (index == testStartEventIndex) {
    Serial.printf("\nTOTI %d starting the testing cycle.", totiNumber);

    // Set the first test.
    currentTest = OUTPUT_HIGH;

    // Set the timer so that testing starts immediately.
    testingTimer = millis();

    testing = true;
  }
  if (index == testStopEventIndex) {
    Serial.printf("\nTOTI %d stopping the testing cycle.", totiNumber);

    // Set the testPin high so it doesn't interfere with normal operataion.
    digitalWrite(testPin, HIGH);

    testing = false;
  }
}

void TOTI::loop() {
  debounce.loop();

  if ((this->isNotOccupied()) && (debounce.outputState == LOW)) {
    this->currentState = State::OCCUPIED;
    if (sendEvent) sendEvent(eventIndexOccupied);
  }

  if ((this->isOccupied()) && (debounce.outputState == HIGH)) {
    this->currentState = State::NOT_OCCUPIED;
    if (sendEvent) sendEvent(eventIndexNotOccupied);
  }

  if (testing) testLoop();
}

bool TOTI::eventIndexMatches(uint16_t index) {
  // Check for one of the testing event indexes.
  if ((index == this->testStartEventIndex) ||
      (index == this->testStopEventIndex)) return true;

  if ((index == this->eventIndexOccupied) ||
      (index == this->eventIndexNotOccupied)) return true;

  return false;
}

bool TOTI::eventIndexMatchesCurrentState(uint16_t index) {
  if (index == eventIndexOccupied) {
    return (currentState == State::OCCUPIED);
  }

  if (index == eventIndexNotOccupied) {
    return (currentState == State::NOT_OCCUPIED);
  }

  return false; // To keep the compiler happy!
}

void TOTI::print() {
  Serial.printf("\neventIndexOccupied=%#02X, eventIndexNotOccupied=%#02X", eventIndexOccupied, eventIndexNotOccupied);
}

void TOTI::testLoop() {
  if (millis() < testingTimer) return;

  // Time to move to the next part of the test cycle.

  switch (currentTest) {
    case OUTPUT_HIGH:
      digitalWrite(testPin, HIGH);
      testingTimer = millis() + 1000;
      currentTest = OUTPUT_LOW;
      break;
    
    case OUTPUT_LOW:
      digitalWrite(testPin, LOW);
      testingTimer = millis() + 1000;
      currentTest = OUTPUT_HIGH;
      break;
    
    default:
      break;
  }
}
