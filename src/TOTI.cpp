#include "TOTI.h"

// inputPin is active low, i.e. low means OCCUPIED.
// outputPin is active high, i.e. high lights the LED.

TOTI::TOTI(uint8_t inputPin, uint8_t outputPin) {
  // Set the pins.
  this->inputPin = inputPin;
  this->outputPin = outputPin;

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

void TOTI::setEvents(uint16_t eventIndexOccupied, uint16_t eventIndexNotOccupied) {
  this->eventIndexOccupied = eventIndexOccupied;
  this->eventIndexNotOccupied = eventIndexNotOccupied;
}

int TOTI::getEventForCurrentState() {
  if (this->isOccupied()) return eventIndexOccupied;
  if (this->isNotOccupied()) return eventIndexNotOccupied;
  return -1; //error.
}

int TOTI::process() {
  if (this->isNotOccupied() && digitalRead(inputPin) == LOW) {
    this->currentState = State::OCCUPIED;
    digitalWrite(outputPin, HIGH); // light the output LED.
    return eventIndexOccupied;
  }

  if (this->isOccupied() && digitalRead(inputPin) == HIGH) {
    this->currentState = State::NOT_OCCUPIED;
    digitalWrite(outputPin, LOW); // extinguish the output LED.
    return eventIndexNotOccupied;
  }

  return -1;
}

bool TOTI::eventIndexMatchesThisTOTI(uint16_t index) {
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
