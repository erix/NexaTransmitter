#ifndef NexaTransmitter_h
#define NexaTransmitter_h

class NexaTransmitter
{

  public:
    //Constructor.
    //arg1 = pin to connected to 434Mhz transmitter
    //arg2 = the 26 bit long transmitter ID
    NexaTransmitter(short, unsigned long);
    //arg1 = true to turn switch on, false for off
    //arg2 = unit number 0-15
    //arg3 = dimmer level 0-15
    void setSwitch(bool, short, short level = 0);
  private:
    short txPin;
    unsigned long transmitterId;
    short repeat;

    void transmit(bool, short, short);
    void sendBit(bool);
    void sendPair(bool);
};

#endif
