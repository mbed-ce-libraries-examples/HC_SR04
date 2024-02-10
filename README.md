# HC_SR04 library for MbedCE

Library for interfacing [HC_SR04 ultrasonic sensor](https://www.sparkfun.com/products/15569) under MbedCE.

## How to start
1. Create a new project according to [MbedCE instructions](https://github.com/mbed-ce/mbed-os/wiki)
2. Add this as submodule to zour project via `git submodule add --depth 1 https://github.com/mbed-ce-libraries-examples/HC_SR04 HC_SR04`
3. The top level `CMakeList.txt` (in root of your project) should be modified according to [this wiki page](https://github.com/mbed-ce/mbed-os/wiki/MbedOS-configuration#libraries-in-your-application)
4. Create your main.cpp file and copy & Paste example code below.
5. Build the project

## Example code
```
#include "mbed.h"
#include "HC_SR04.h"

void dist(int distance)
{
    //here put a code what should happen after the distance is measured
    printf("Distance %dmm\r\n", distance);
}

/* Set the trigger pin to D8 and the echo pin to D9.
Rate of updates every 500ms with a timeout after 1sec.
The dist is called after the distance is measured */
HC_SR04 mu(D8, D9, 500ms, 1s, &dist);

int main()
{
    printf("MbedCE_lib_example_HC_SR04\r\n");
    //start mesuring the distance
    mu.startUpdates();
    while(1)
    {
        //Do something else here
		
        /*call checkDistance() as much as possible, as this is where
        the class procesed call of dist function.*/
        mu.checkDistance();                              
    }
}
```
## Description

//TODO

### Status:
This library was tested (02/2024) with Nucleo-F446RE, HC_SR04 sensor, VS-Code under Win11, GCC 12.3 and MbedCE library