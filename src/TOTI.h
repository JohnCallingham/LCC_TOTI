#ifndef TOTI_H
#define TOTI_H

/**
 * Provides the following functionality;-
 * 1. Initialises the object without sending any events. Uses initialise().
 * 2. Sends events based on the current state at the time the hub is connected. Uses getEventForCurrentState().
 * 3. Sends events when a TOTI changes state. Uses process().
 * 4. Responds to a query from JMRI for current state based on event index. Uses eventIndexMatchesThisTOTI() and eventIndexMatchesCurrentState().
 */

#include <Arduino.h>

/**
 * Class TOTI represents one TOTI (Train On Track Indicator).
 */
class TOTI {
  public:
    TOTI(uint8_t inputPin, uint8_t outputPin);

    void setEvents(uint16_t eventIndexOccupied, uint16_t eventIndexNotOccupied);

    /**
     * Returns true if index matches either of eventIndexOccupied or eventIndexNotOccupied, else false.
     */
    bool eventIndexMatchesThisTOTI(uint16_t index);

    /**
     * Returns true if index matches the current state, else false.
     * e.g. if index == eventIndexOccupied and the current state is occupied, then return true.
     */
    bool eventIndexMatchesCurrentState(uint16_t index);

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
    uint16_t eventIndexOccupied;
    uint16_t eventIndexNotOccupied;
    State currentState;

    bool isOccupied() { if (this->currentState == State::OCCUPIED) return true; else return false; }
    bool isNotOccupied() { if (this->currentState == State::NOT_OCCUPIED) return true; else return false; }
};

#endif
