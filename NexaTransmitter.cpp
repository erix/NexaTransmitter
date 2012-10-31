/*
 * HE300 Automatic Protocol Transmitter 'Simulator'
 * David Edmundson 21/11/2009
 *
 * This class allows use of the homeeasy automatic protocol to be used in any Arduino program
 *
 * Based heavily on demo code by
 * Martyn Henderson 02/10/2009  http://martgadget.blogspot.com
 *
 * The code has been moved into a class, for easy re-usage and has been optimised 
 * in regards to calculating and storing the transmitter address.
 *
 * You dont need to learn the Arduino into the socket, because it can 
 * pretend to be a transmitter you already have.
 *
 * Use the Automatic protocol reciever code above to find out
 * your transmitter address, and reciepient ID. 
 *
 * Original code from Martyn
 * *Only* tested with one HE300 transmitter (HE Address 272946) with its
 * slider switch set to 1 which appears to be 0 in fact.
 * Seems to work with a switch socket (HE302S) and can turn it on and off 
 * without the ardunio being learned into the socket.
 *
 * Edited code works with a HE305 transmitter, connecting to a HEXXX light fitting. The reciepient code for these 
 * appears to be 0b1010. I based my timings based on actual timings received from /my/ transmitter.
 * These seem to match the numbers used by Barnaby and Peter but not from Martyn. Adjust if needed.
 *
 * Cheers to Barnaby and Peter, because without the code above
 * that shows the receiver output this wouldnt have been possible!
 *
 * If you make something cool using Nexa and Arduino, please 
 * post back here with a link to your code so we can all enjoy.
 *
 */

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
  #else
  #include "WProgram.h"
  #endif
#include "NexaTransmitter.h"


NexaTransmitter::NexaTransmitter(short pin, unsigned long remote) :
  txPin(pin),
  transmitterId(remote),
  repeat(2) // repeat the transmition 2x
{
  pinMode(txPin, OUTPUT);      // transmitter pin.
}


//sends either an on/off signal to the main switch
//always seems to work best if we send it twice..
void NexaTransmitter::setSwitch(bool on, short recipient, short level)
{
  for (int i = 0; i < repeat; i++) {
    transmit(on, recipient, level);
    delay(10);
  }
}


//sends either an on/off signal to the main switch
void NexaTransmitter::transmit(bool blnOn, short recipient, short level)
{
  int i;
  // Do the latch sequence..
  digitalWrite(txPin, HIGH);
  delayMicroseconds(270);     // bit of radio shouting before we start. 
  digitalWrite(txPin, LOW);
  delayMicroseconds(2585);     // low for 9900 for latch 1
  digitalWrite(txPin, HIGH);   // high again 

  // End on a high
  digitalWrite(txPin, HIGH);

  // Send HE Device Address..
  // This is a 26 bit code.
  // Start at MSB and iterate through to the lowest
  for(i=25; i>=0; i--)
  {
    //The typecasting seems a bit overkill, but 26 bits needs a long and the arduino compiler seems to love trying to 
    //convert everything to an standard int.
    //creates bitmask of only relevant bit. Check and send a 1 or 0 as applicable.
    bool bitToSend = (unsigned long)(transmitterId & ((unsigned long)1 << i)) != 0;
    sendPair(bitToSend);

  }

  // Send 26th bit - group 1/0
  sendPair(false);

  if (level > 0) {
    // dimmer level was given send 00
    sendBit(false);
    sendBit(false);
  } else {
    // Send 27th bit - on/off 1/0
    sendPair(blnOn);
  }

  // 4 bits - recipient
  for(i=3; i>=0; i--)
  {
    bool bitToSend = (recipient & (1 << i)) != 0;
    sendPair(bitToSend);
  }

  if (level > 0) {
    // send the dimmer level
    for(i=3; i>=0; i--)
    {
      bool bitToSend = (level & (1 << i)) != 0;
      sendPair(bitToSend);
    }
  }

  digitalWrite(txPin, HIGH);   // high again (shut up)
  delayMicroseconds(275);      // wait a moment
  digitalWrite(txPin, LOW);    // low again for 2675 - latch 2.

}

void NexaTransmitter::sendBit(bool b)
{
  if (b)
  {
    digitalWrite(txPin, HIGH);
    delayMicroseconds(275);
    digitalWrite(txPin, LOW);
    delayMicroseconds(1240);
  }
  else
  {
    digitalWrite(txPin, HIGH);
    delayMicroseconds(258);
    digitalWrite(txPin, LOW);
    delayMicroseconds(258);
  }
}

void NexaTransmitter::sendPair(bool b)
{
  // Send the Manchester Encoded data 01 or 10, never 11 or 00
  if(b)
  {
    sendBit(true);
    sendBit(false);
  }
  else
  {
    sendBit(false);
    sendBit(true);
  }
}
