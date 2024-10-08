#include "MIDIcontroller.h"

byte MIDIchannel = 5;

const int selector_pin_a = 2;
const int selector_pin_b = 3;
const int selector_pin_c = 4;
//const int selector_pin_d = 3; // uncomment only if using MUX-16

const int mux_input_pin = 14; // Change this to the ANALOG pin you want to use
MIDIswitch* Pots[8]; // change to Pots[16] if using MUX-16

void setup(){
  pinMode(selector_pin_a, OUTPUT);
  pinMode(selector_pin_b, OUTPUT);
  pinMode(selector_pin_c, OUTPUT);
//  pinMode(selector_pin_d, OUTPUT); // only if using MUX-16

  for(int i=0; i<8; i++){ // change to i<16 if using MUX-16
    Pots[i] = new MIDIswitch(mux_input_pin, 22+i);
    // CC 12~ are probably safe to use but you can use any CC number you like.

    //Pots[i]->inputRange(15, 1000); // helps reach the full output range
  }
}

void loop(){
  for(int i=0; i<8; i++){ // change to i<16 if using MUX-16
    //digitalWrite(selector_pin_d, (i&15)>>3); // uncomment only if using MUX-16
    digitalWrite(selector_pin_c, (i&7)>>2);
    digitalWrite(selector_pin_b, (i&3)>>1);
    digitalWrite(selector_pin_a, (i&1));
    delayMicroseconds(5);
    Pots[i]->send();
    
  }  


// This prevents crashes that happen when incoming usbMIDI is ignored.
  while(usbMIDI.read()){}

// Also uncomment this if compiling for standard MIDI
//  while(MIDI.read()){}
}
