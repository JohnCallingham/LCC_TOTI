#include "TOTI.h"

/**
 * inputPin is active low, i.e. low means OCCUPIED.
 * outputPin is active high, i.e. high lights the LED.
 */

TOTI::TOTI(uint8_t inputPin, uint8_t outputPin) {
  // Set the pins.
  this->inputPin = inputPin;
  this->outputPin = outputPin;

  this->outputEnable = true;

  // Configure input and output pins.
  pinMode(inputPin, INPUT_PULLUP);
  pinMode(outputPin, OUTPUT);
  
  // Set the initial state according to the actual state of the input pin
  // and set the output pin accordingly.
  if (digitalRead(inputPin) == LOW) {
    currentState = State::OCCUPIED;
    digitalWrite(outputPin, HIGH); // Turn LED on.
  } else {
    currentState = State::NOT_OCCUPIED;
    digitalWrite(outputPin, LOW); // Turn LED off.
  }
}

TOTI::TOTI(uint8_t inputPin) {
  // Set the pins.
  this->inputPin = inputPin;

  this->outputEnable = false;

  // Configure input and output pins.
  pinMode(inputPin, INPUT_PULLUP);
  
  // Set the initial state according to the actual state of the input pin
  // and set the output pin accordingly.
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

void TOTI::process() {
  // Every 1 mS decrement the debounce timer if it is non zero.
  if (millis() >= milliSecondTimer) {
    milliSecondTimer = millis() + 1;

    // Decrement the debounce timer if it is non zero.
    if (debounceTimer > 0) {
      debounceTimer--;

      // If input goes low, restart the debounce timer.
      if (digitalRead(inputPin) == LOW) {
        debounceTimer = DEBOUNCE_DELAY_mS;
      }

      // Check if the debounce timer has just expired.
      if (debounceTimer == 0) {
        // The debounce timer has just expired.
        this->currentState = State::NOT_OCCUPIED;
        if (outputEnable) digitalWrite(outputPin, LOW); // extinguish the output LED.
        if (sendEvent) sendEvent(eventIndexNotOccupied);
      }
    }
  }

  if (this->isNotOccupied() && digitalRead(inputPin) == LOW) {
    this->currentState = State::OCCUPIED;
    if (outputEnable) digitalWrite(outputPin, HIGH); // light the output LED.
    if (sendEvent) sendEvent(eventIndexOccupied);
  }

  if (this->isOccupied() && digitalRead(inputPin) == HIGH) {
    // Start the debounce timer.
    debounceTimer = DEBOUNCE_DELAY_mS;

    // this->currentState = State::NOT_OCCUPIED;
    // if (outputEnable) digitalWrite(outputPin, LOW); // extinguish the output LED.
    // if (sendEvent) sendEvent(eventIndexNotOccupied);
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

// Original without de-bounce.

// void TOTI::process() {
//   if (this->isNotOccupied() && digitalRead(inputPin) == LOW) {
//     this->currentState = State::OCCUPIED;
//     if (outputEnable) digitalWrite(outputPin, HIGH); // light the output LED.
//     if (sendEvent) sendEvent(eventIndexOccupied);
//   }

//   if (this->isOccupied() && digitalRead(inputPin) == HIGH) {
//     this->currentState = State::NOT_OCCUPIED;
//     if (outputEnable) digitalWrite(outputPin, LOW); // extinguish the output LED.
//     if (sendEvent) sendEvent(eventIndexNotOccupied);
//   }
// }
