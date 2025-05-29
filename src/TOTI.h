#ifndef TOTI_H
#define TOTI_H

/**
 * Provides the following functionality;-
 * 1. Initialises the object without sending any events. Uses initialise().
 * 2. Sends events based on the current state at the time the hub is connected. Uses getEventForCurrentState().
 * 3. Sends events when a TOTI changes state. Uses process().
 * 4. Responds to a query from JMRI for current state based on event index. Uses isNotOccupied() and isOccupied().
 */

#include <Arduino.h>

/**
 * Class TOTI represents one TOTI (Train On Track Indicator).
 */
class TOTI {
  public:
    void setInputPin(uint8_t inputPin) { this->inputPin = inputPin; }
    void setOutputPin(uint8_t outputPin) { this->outputPin = outputPin; }
    void setEventIndexON(uint16_t eventIndexON) { this->eventIndexON = eventIndexON; }
    void setEventIndexOFF(uint16_t eventIndexOFF) { this->eventIndexOFF = eventIndexOFF; }

    bool isOccupied() { if (this->currentState == State::OCCUPIED) return true; else return false; }
    bool isNotOccupied() { if (this->currentState == State::NOT_OCCUPIED) return true; else return false; }

    /**
     * Configures input and output pins.
     * Reads current state of input pin and sets currentState and output pin accordingly.
     */
    void initialise();

    /**
     * Returns the event index representing the current state.
     * Used when first connected to hub so that JMRI is set to the correct initial state.
     */
    int getEventForCurrentState();

    /**
     * Tests the input pin to see if a change has occurred.
     * Does any required de-bouncing. TO DO.
     * Returns -1 if no change or the event index if an event is to be sent.
     */
    int process();

  private:
    enum State { NOT_OCCUPIED, OCCUPIED };
    uint8_t inputPin;
    uint8_t outputPin;
    uint16_t eventIndexON;
    uint16_t eventIndexOFF;
    State currentState;
};

#endif
