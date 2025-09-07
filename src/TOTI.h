#ifndef TOTI_H
#define TOTI_H

/**
 * Provides the following functionality;-
 * 1. Initialises the object without sending any events. Uses initialise().
 * 2. Sends events based on the current state at the time the hub is connected. Uses getEventForCurrentState().
 * 3. Sends events when a TOTI changes state. Uses process().
 * 4. Responds to a query from JMRI for current state based on event index. Uses eventIndexMatches() and eventIndexMatchesCurrentState().
 */

#include <Arduino.h>
#include "LCC_Node_Component_Base.h"

#define DEBOUNCE_DELAY_mS 100 // 100 mS debounce delay.

/**
 * Class TOTI represents one TOTI (Train On Track Indicator).
 */
class TOTI : public LCC_Node_Component_Base {
  public:
    TOTI(uint8_t inputPin, uint8_t outputPin);
    TOTI(uint8_t inputPin);

    void setEvents(uint16_t eventIndexOccupied, uint16_t eventIndexNotOccupied);

    /**
     * Returns true if index matches either of eventIndexOccupied or eventIndexNotOccupied, else false.
     */
    bool eventIndexMatches(uint16_t index) override;

    /**
     * Returns true if index matches the current state, else false.
     * e.g. if index == eventIndexOccupied and the current state is occupied, then return true.
     */
    bool eventIndexMatchesCurrentState(uint16_t index) override;

    /***
     * Called when sending initial events.
     * Used to initialise JMRI when JMRI starts after the node has started.
     */
    void sendEventsForCurrentState() override;

    /**
     * Used to start or restart the debounce delay.
     * The debounce timer is decremented every mS
     */
    void startDebounceTimer() { debounceTimer = DEBOUNCE_DELAY_mS; }

    /**
     * Tests the input pin to see if a change has occurred.
     * Does any required de-bouncing. TO DO.
     */
    void process();

    void print();

    // Made public so that the selected LED can be controlled.
    bool isOccupied() { return (this->currentState == State::OCCUPIED); }

  private:
    enum State { NOT_OCCUPIED, OCCUPIED };
    State currentState;

    uint16_t debounceTimer = 0; // Used to debounce the input.
    unsigned long milliSecondTimer = 0; // used to create an action every mS.

    uint8_t inputPin;
    uint8_t outputPin;

    uint16_t eventIndexOccupied;
    uint16_t eventIndexNotOccupied;

    bool outputEnable; // True if an output pin has been defined, else False.

    bool isNotOccupied() { return (this->currentState == State::NOT_OCCUPIED); }
};

#endif
