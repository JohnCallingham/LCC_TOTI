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
    TOTI(uint8_t totiNumber, uint8_t inputPin);

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

    enum State { NOT_OCCUPIED, OCCUPIED };
    State currentState;

    uint16_t eventIndexOccupied;
    uint16_t eventIndexNotOccupied;

    bool isNotOccupied() { return (this->currentState == State::NOT_OCCUPIED); }
};

#endif

// #define DEBOUNCE_DELAY_mS 100 // 100 mS debounce delay.

// /***
//  * Class Debounce provides general debounce functionality.
//  * The input is by a specified input pin.
//  * ??? The output is by a callback function. ???
//  */
// class Debounce {
//   public:
//     /***
//      * inputPin is the pin which is to be debounced.
//      * sampleTimemS is the number of mS between each sample of inputPin.
//      * lowDebounceTime is the number of samples that inputPin needs to be permanently low before a low is output.
//      * highDebounceTime is the number of samples that inputPin needs to be permanently high before a high is output.
//      */
//     Debounce(uint8_t inputPin, uint16_t sampleTimemS, uint16_t lowDebounceTime, uint16_t highDebounceTime);

//     /**
//      * Called repeatedly from the main program loop.
//      * Calls sample() when it is time to sample the inputPin.
//      */
//     void process();

//   private:
//     /**
//      * Called every sampleTimemS to read the state of the input pin and determine if any output change has occurred.
//      */
//     void sample();

//     enum State { stLOW = 0x0, stHIGH = 0x1 };
//     State inputState; // The state of the input pin which is bouncing.
//     State outputState; // The state after the bouncing has been removed.
//     State lastSampledState; // Used to detect if the input pin has changed state since the last sample.

//     unsigned long nextUpdate = 0; // Used to ensure that sample() is called every sampleTimemS;

//     uint8_t inputPin; // The pin which is to be debounced.
//     uint16_t sampleTimemS; // The number of mS between each sample of inputPin.
//     uint16_t lowDebounceTime; // The number of samples that inputPin needs to be permanently low before a low is output.
//     uint16_t highDebounceTime; // The number of samples that inputPin needs to be permanently high before a high is output.
// };

    // /**
    //  * Used to start or restart the debounce delay.
    //  * The debounce timer is decremented every mS
    //  */
    // void startDebounceTimer() { debounceTimer = DEBOUNCE_DELAY_mS; }

