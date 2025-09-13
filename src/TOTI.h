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

/**
 * Class TOTI represents one TOTI (Train On Track Indicator).
 */
class TOTI : public LCC_Node_Component_Base {
  public:
    // TOTI(uint8_t inputPin, uint8_t outputPin);
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
     * Called repeatedly from the main program loop.
     * Tests the input pin to see if a change has occurred.
     * Does any required de-bouncing. TO DO.
     */
    void process();

    void print();

    // Made public so that the selected LED can be controlled.
    bool isOccupied() { return (this->currentState == State::OCCUPIED); }

  private:
    // Create a Debounce object.
    Debounce debounce;

    enum State { NOT_OCCUPIED, OCCUPIED };
    State currentState;

    // uint16_t debounceTimer = 0; // Used to debounce the input.
    // unsigned long milliSecondTimer = 0; // used to create an action every mS.

    // uint8_t inputPin;
    // uint8_t outputPin;

    uint16_t eventIndexOccupied;
    uint16_t eventIndexNotOccupied;

    // bool outputEnable; // True if an output pin has been defined, else False.

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

