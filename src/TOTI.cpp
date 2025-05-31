#include "TOTI.h"

// inputPin is active low, i.e. low means OCCUPIED.
// outputPin is active high, i.e. high lights the LED.

void TOTI::initialise() {
  // Configure input and output pins.
  pinMode(inputPin, INPUT_PULLUP);
  pinMode(outputPin, OUTPUT);
  
  // Set the initial state according to the actual state of the input pin
  // and set the ouput pin accordingly.
  if (digitalRead(inputPin) == LOW) {
    currentState = State::OCCUPIED;
    digitalWrite(outputPin, HIGH); // Turn LED on.
  } else {
    currentState = State::NOT_OCCUPIED;
    digitalWrite(outputPin, LOW); // Turn LED off.
  }
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
  if ((index == this->eventIndexOccupied) || (index == this->eventIndexNotOccupied)) {
    return true;
  } else {
    return false;
  }
}

bool TOTI::eventIndexMatchesCurrentState(uint16_t index) {
  if (index == this->eventIndexOccupied) {
    if (currentState == State::OCCUPIED) return true; else return false;
  } 
  
  if (index == this->eventIndexNotOccupied) {
    if (currentState == State::NOT_OCCUPIED) return true; else return false;
  }

  return false; // To keep the compiler happy!
}
