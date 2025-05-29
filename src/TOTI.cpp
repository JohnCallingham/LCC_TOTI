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
  if (this->isNotOccupied()) return eventIndexOFF;
  if (this->isOccupied()) return eventIndexON;
  return -1; //error.
}

int TOTI::process() {
  if (this->isNotOccupied() && digitalRead(inputPin) == LOW) {
    this->currentState = State::OCCUPIED;
    digitalWrite(outputPin, HIGH); // light the output LED.
    return eventIndexON;
  }

  if (this->isOccupied() && digitalRead(inputPin) == HIGH) {
    this->currentState = State::NOT_OCCUPIED;
    digitalWrite(outputPin, LOW); // extinguish the output LED.
    return eventIndexOFF;
  }

  return -1;
}
