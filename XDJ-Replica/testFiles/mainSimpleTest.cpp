/*
  This example is for a potentiometer (POT) or any variable resistor (SENSOR)
  such as a photo cell, FSR, flex resistor, thermistor etc.
                 _________
         3.3v___|         |
   Analog Pin___|   10k   |
          GND___|   POT   |
                |_________|

  3.3v---(SENSOR)---\
                     )---Analog Pin
  GND------/\/\/----/
            10k
*/

#include "MIDIcontroller.h"

byte MIDIchannel = 5;
const int potPin = 23;  // Change this to the ANALOG pin you want to use

byte MIDIchannel2 = 6;
const int switchPin = 2; //any digital pin
const int ledPin = 13;   //Set an LED to show the state of a latch button.


const int encPinA = 20;   // Change these numbers to
const int encPinB = 19;   // the two pins your encoder is on.
MIDIenc myEnc(encPinA, encPinB, 24);
// MIDIenc myEnc(encPinA, encPinB, 24, PER_VALUE); // Faster but not for use with detented encoders.
// MIDIenc myEnc(encPinA, encPinB, PROGRAM_CHANGE); // Send program change instead of CC# 4

/* MIDIpot parameters are:
      1) pin (required)
      2) CC number (required)
      3) a secondary on/off CC# (optional)
*/

MIDIpot myPot(potPin, 22);
// MIDIpot myPot(potPin, 22, 9); // CC# 9 OFF is sent when you turn the pot all the way down.
                              // CC# 9 ON is sent when you start turning it up again.

MIDIswitch myInput(switchPin, 21);

void setup(){
  // UNCOMMENT ANY OF THE FOLLOWING LINES TO CHANGE THE DEFAULTS
  myPot.inputRange(20, 650); // Specify the usable analog range for the pot
  myPot.outputRange(127, 0); // Reverse the direction of the pot
  // myPot.outputRange(20, 120); // Restrict value to 20 ~ 120

  myEnc.write(64); // Initialize the encoder to 64
}

int lastTime = millis();
int onoffval = 64;
void loop(){
  myPot.send();
  myInput.send();
  myEnc.send();


  

  //myEnc.send();
  // if(myEnc.read() > 64){
  //     myEnc.write(127);
  //   } else if(myEnc.read()<64){
  //     myEnc.write(0);
  //   } 
  //   //myEnc.send();
  //   myEnc.write(64);
  if((millis() - lastTime)> 1){
    
   myEnc.write(64);
    lastTime = millis();
  }
  //myEnc.write(64); // Initialize the encoder to 64
  


// This prevents crashes that happen when incoming usbMIDI is ignored.
  while(usbMIDI.read()){}

// Also uncomment this if compiling for standard MIDI
//  while(MIDI.read()){}
}