#include "TOTI.h"

/**
 * inputPin is active low, i.e. low means OCCUPIED.
 */

// The TOTI constructor also constructs the Debounce object.
TOTI::TOTI(uint8_t inputPin) : debounce() {
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

// void TOTI::process() {
void TOTI::loop() {
  debounce.loop();

  // if (this->isNotOccupied() && digitalRead(inputPin) == LOW) {
  if ((this->isNotOccupied()) && (debounce.outputState == LOW)) {
    this->currentState = State::OCCUPIED;
    // if (outputEnable) digitalWrite(outputPin, HIGH); // light the output LED.
    if (sendEvent) sendEvent(eventIndexOccupied);
  }

  // if (this->isOccupied() && digitalRead(inputPin) == HIGH) {
  if ((this->isOccupied()) && (debounce.outputState == HIGH)) {
    this->currentState = State::NOT_OCCUPIED;
    // if (outputEnable) digitalWrite(outputPin, LOW); // extinguish the output LED.
    if (sendEvent) sendEvent(eventIndexNotOccupied);
  }
}

bool TOTI::eventIndexMatches(uint16_t index) {
  if ((index == this->eventIndexOccupied) || (index == this->eventIndexNotOccupied)) return true;

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



// Original de-bounced version.

// void TOTI::process() {
//   Every 1 mS decrement the debounce timer if it is non zero.
//   if (millis() >= milliSecondTimer) {

//     //Serial.printf("\n%d in TOTI::process() inputPin %d = %d", millis(), inputPin, digitalRead(inputPin));

//     milliSecondTimer = millis() + 1;

//     // Decrement the debounce timer if it is non zero.
//     if (debounceTimer > 0) {
//       //Serial.printf("\n%d debounceTimer=%d", millis(), debounceTimer);
//       debounceTimer--;

//       // If input goes low, restart the debounce timer.
//       if (digitalRead(inputPin) == LOW) {
//         debounceTimer = DEBOUNCE_DELAY_mS;
//       }

//       // Check if the debounce timer has just expired.
//       if (debounceTimer == 0) {
//         // The debounce timer has just expired.
//         this->currentState = State::NOT_OCCUPIED;
//         if (outputEnable) digitalWrite(outputPin, LOW); // extinguish the output LED.
//         if (sendEvent) sendEvent(eventIndexNotOccupied);
//       }
//     }
//   }

//   if (this->isNotOccupied() && digitalRead(inputPin) == LOW) {
//     this->currentState = State::OCCUPIED;
//     if (outputEnable) digitalWrite(outputPin, HIGH); // light the output LED.
//     if (sendEvent) sendEvent(eventIndexOccupied);
//   }

//   if ((this->isOccupied()) && (digitalRead(inputPin) == HIGH) && (debounceTimer == 0)) {
//     // Start the debounce timer.
//     debounceTimer = DEBOUNCE_DELAY_mS;

//     // this->currentState = State::NOT_OCCUPIED;
//     // if (outputEnable) digitalWrite(outputPin, LOW); // extinguish the output LED.
//     // if (sendEvent) sendEvent(eventIndexNotOccupied);
//   }
// }


// Debounce::Debounce(uint8_t inputPin, uint16_t sampleTimemS, uint16_t lowDebounceTime, uint16_t highDebounceTime) {
//   this->inputPin = inputPin;
//   this->sampleTimemS = sampleTimemS;
//   this->lowDebounceTime = lowDebounceTime;
//   this->highDebounceTime = highDebounceTime;
// }

// void Debounce::process() {
//   // Implement a non blocking delay for sampleTimemS.
//   if (millis() < nextUpdate) return;
//   nextUpdate = millis() + sampleTimemS;

//   // sampleTimemS has passed since the last call to sample().
//   sample();
// }

// void Debounce::sample() {

// }

