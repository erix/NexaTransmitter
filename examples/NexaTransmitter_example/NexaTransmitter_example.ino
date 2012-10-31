#include <NexaTransmitter.h>

NexaTransmitter remote(2,  5519106); // Create the nexa remote on pin2 with remote id

void setup() {
 Serial.begin(9600);
 Serial.println("start");
 
 remote.setSwitch(true, 2, 10); // switch on the unit 2 to dim level 10 (out of 15)
 delay(3000);                   // wait 3 sec
 remote.setSwitch(false, 2);    // switch off the unit 2
 
 Serial.println("stop");        // done. 
}

void loop() {
  // do nothing
}


