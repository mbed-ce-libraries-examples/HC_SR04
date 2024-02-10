/* Copyright (c) 2024 Mbed-CE Libraries & Examples
 * 
 * This library is based on https://os.mbed.com/components/HC-SR04/
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef HC_SR04_H
#define HC_SR04_H

/**
 * @note
 *   Trigger pulse
 *        _______ 
 *   ____|  10us |_______________________________
 * 
 *   Echo pulse
 *                          _____________ _ _ _ _ 
 *   ______________________| 150us-25ms, or 38ms if no obstacle
 * 
 * @code
 * #include "mbed.h"
 * #include "HC_SR04.h"
 * 
 * void dist(int distance)
 * {
 *     //here put a code what should happen after the distance is measured
 *     printf("Distance %dmm\r\n", distance);
 * }
 *
 * // Set the trigger pin to D8 and the echo pin to D9.
 * // Rate of updates every 500ms with a timeout after 1sec.
 * // The dist is called after the distance is measured
 * HC_SR04 mu(D8, D9, 500ms, 1s, &dist);
 * 
 * int main()
 * {
 *     printf("MbedCE_lib_example_HC_SR04\r\n");
 *     //start mesuring the distance
 *     mu.startUpdates();
 *     while(1)
 *     {
 *         //Do something else here
 * 		
 *         // call checkDistance() as much as possible, as this is where
 *         // the class procesed call of dist function..
 *         mu.checkDistance();                              
 *     }
 * }
 * @endcode
 */

#include "mbed.h"

class HC_SR04
{
    public:
        /** HC_SR04 iniates the pbject with the specified pins
         * @param trigPin       PinName to be conected to trigger pin of the sensor
         * @param echoPin       PinName to be conected to echo pin of the sensor
         * @param updateRate    Chrono time for how often should be the distance measured
         * @param timeout       Chrono time for timeout
         * 
         * @note    With chrono time you will use it like this
         *          for exampele updateRate = 100ms and timeout = 1s.
         * 
         * **/
        HC_SR04(PinName trigPin, PinName echoPin, Kernel::Clock::duration_u32 updateRate, Kernel::Clock::duration_u32 timeout);
        
         /** HC_SR04 iniates the pbject with the specified pins
         * @param trigPin       PinName to be conected to trigger pin of the sensor
         * @param echoPin       PinName to be conected to echo pin of the sensor
         * @param updateRate    Chrono time for how often should be the distance measured
         * @param timeout       Chrono time for timeout
         * @param onUpdate      A callback function what is called after the time specified via updateRate
         * 
         * @note    With chrono time you will use it like this
         *          for exampele updateRate = 100ms and timeout = 1s.
         * 
         * **/
        HC_SR04(PinName trigPin, PinName echoPin, Kernel::Clock::duration_u32 updateRate, Kernel::Clock::duration_u32 timeout, void onUpdate(int));

        /** 
         * @returns inteager of the last measured distance
         */
        int getCurrentDistance(void);

        /**
         * Pauses measuring the distance
         */
        void pauseUpdates(void);

        /**
         * Starts mesuring the distance
         */
        void startUpdates(void);

        /** 
         * Attachs a function what should be called after the time specified via updateRate
         * @param method(int)   Attach a function with int parameter
         */
        void attachOnUpdate(void method(int));

        /**
         * Changes the rate of updates
         * @param updateRate   Chrono time for changing the update rate
         */
        void setUpdateRate(Kernel::Clock::duration_u32 updateRate);


        /** 
         * Gets whether the distance has been changed since the last call of isUpdated() or checkDistance()
         * @return true if distance was changed since the last call
         * 
         */
        bool isUpdated(void);

        /**
         * This will process if attached function should be called
         * @note Call this as often as possible in your code, eg. at the end of a while(1) loop,
         *  and it will check whether the method you have attached needs to be called
         */
        void checkDistance(void);

    private:
        DigitalOut _trig;
        InterruptIn _echo;
        Timer _t;
        Timeout _tout;
        int _distance;
        Kernel::Clock::duration_u32 _updateRate;
        int start;
        int end;
        void (*_onUpdateMethod)(int);
        void _startT(void);
        void _updateDist(void);
        void _startTrig(void);
        Kernel::Clock::duration_u32 _timeout;
        volatile bool done_flag;
};
#endif