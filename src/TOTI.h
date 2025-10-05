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
#include "Debounce.h"

#define DEBOUNCE_SAMPLE_TIME_mS 1 // Debounce sampling every 1 mS.

/**
 * Class TOTI represents one TOTI (Train On Track Indicator).
 */
class TOTI : public LCC_Node_Component_Base {
  public:
    TOTI(uint8_t totiNumber, uint8_t inputPin, uint8_t testPin);

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

    /**
     * Called when sending initial events.
     * Used to initialise JMRI when JMRI starts after the node has started.
     */
    void sendEventsForCurrentState() override;

    /**
     * Sets the number of milliSeconds that the TOTI must permanantly indicate occupied
     *  before the occupied event is sent.
     * Needs to take into account the Debounce object's sample time (mS).
     *  E.g. if the sample time is 1 mS and the required delay is 100 mS,
     *  then the debounce object must take 100 samples all indicating occupied
     *  before the occupied event is sent.
     */
    void setOccupiedDebounceDelay(uint16_t delaymS);

    /**
     * Sets the number of milliSeconds that the TOTI must permanantly indicate not occupied
     *  before the not occupied event is sent.
     * Needs to take into account the Debounce object's sample time (mS).
     *  E.g. if the sample time is 2 mS and the required delay is 1000 mS,
     *  then the debounce object must take 500 samples all indicating not occupied
     *  before the not occupied event is sent.
     */
    void setNotOccupiedDebounceDelay(uint16_t delaymS);

    /**
     * Only used for the test start and test stop events.
     */
    void eventReceived(uint16_t index);

    /**
     * Called repeatedly from the main program loop.
     * Tests the input pin to see if a change has occurred.
     * Does any required de-bouncing.
     */
    // void process();
    void loop();

    void print();

    // Made public so that the selected LED can be controlled.
    bool isOccupied() { return (this->currentState == State::OCCUPIED); }

  private:
    // Declare a Debounce object which is contructed in the TOTI constructor.
    Debounce debounce;

    uint8_t totiNumber;
    uint8_t testPin;

    enum State { NOT_OCCUPIED, OCCUPIED };
    State currentState;

    uint16_t eventIndexOccupied;
    uint16_t eventIndexNotOccupied;

    bool isNotOccupied() { return (this->currentState == State::NOT_OCCUPIED); }

    // Define the test cycles and track the current test cycle.
    // This test causes the testPin to cycle between high and low.
    // The testPin will need to be connected to the TOTI input pin for testing.
    enum Test { OUTPUT_HIGH, OUTPUT_LOW };
    Test currentTest;

    void testLoop();
};

#endif
