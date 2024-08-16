#include <Arduino.h>
//XDJ-rx3 style controller, midi integration made to be used with mixxx dj running on a raspberry pi
//TODO integrate mux switiching, confirm midi functionality

#include <ResponsiveAnalogRead.h>
#include <Bounce2.h>
#include <CD74HC4067.h>
#include <REncoder.h>
#include <MIDI.h>
#include "MIDIcontroller.h"

byte MIDIchannel = 5;




// set up multiplexers
CD74HC4067 mux0(2, 3, 4, 5);  // ANALOG create a new CD74HC4067 object with its four control pins
const int mux0Common = 6; // select a pin to share with the 16 channels of the CD74HC4067

/*
template for control generation

header decleration

ResponsiveAnalogRead "control_short_name"(MUX#SigPin, true);



*/
//REncoder testEncoder = REncoder(mux1Common, mux7Common);

const int DEBOUNCE_TIME = 5;

const int totalPots = 13;
const int totalEncoders = 4; // not right now
const int totalButtons = 16;
const int totalMIDIControls = 109;
const int totalControlsWEmpty = 116;


MIDIenc enc0 = MIDIenc(32, 31, 96);
MIDIenc enc1 = MIDIenc(30, 29, 97);
MIDIenc enc2 = MIDIenc(28, 27, 98);
MIDIenc enc3 = MIDIenc(33, 34, 99);

MIDIswitch extraBut = MIDIswitch(35, 84);
MIDIswitch extraBut1 = MIDIswitch(36, 85);
MIDIswitch extraBut2 = MIDIswitch(37, 86);
MIDIpot potsRead[totalPots];
MIDIswitch buttons[totalButtons]; 
MIDIenc encoders[4]= {enc0, enc1, enc2, enc3};
bool jogEncoder[totalEncoders] = {true, false, false, true};
int encoderPos[totalEncoders] = {0,0,0,0};
//MIDIenc myEnc(mux0Common, mux7Common, 24);
int lastTime = millis();
int lastTime1 = millis();


int values[21];

void OnControlChange(byte channel, byte control, byte value) {
  if (channel == 1) {
    if (control == 10) {
      //analogWrite(6, value);
                                
  }
}
}

void midiSend(int device, int value) {
  //digitalWrite(13, HIGH);
  Serial.println(device + " " + value);
  usbMIDI.sendControlChange(device, value, 1);
  delayMicroseconds(5);
  //digitalWrite(13, LOW);
}


//switch muxs to correct pin
void switchToPin(int pin){
 // int muxNum = pin/16;
  mux0.channel(pin);
  delayMicroseconds(5);

}
//special switch for the two pins

void pots() {
 // potentiometers
  for(int i = 0; i < totalPots; i++){
    
    potsRead[i].send();
    
  }

  for(int i = 0; i < totalEncoders; i++){
      if(jogEncoder[i]){
      //SPEEEEED
       encoders[i].send();
      if((millis() - lastTime)> 100){
          if(encoders[i].read() > 64){
            encoders[i].write(encoders[i].read()-1);
          } else if(encoders[i].read() < 64){
            encoders[i].write(encoders[i].read()+1);
          }
          encoders[i].write(64);
          //lastTime = millis();
      }

  } else{
    encoders[i].send();
    if((millis() - lastTime1)> 1){
        
        encoders[i].write(64);
        //lastTime1 = millis();
    }
    
    
  }
  }
  if((millis() - lastTime)> 100){
          lastTime = millis();
      }
  if((millis() - lastTime1)> 1){
        
        lastTime1 = millis();
    }


    
   
  }


void buttonsHandler() {
  for (int i = 0; i < totalButtons; i++)
  {
    switchToPin(i);
    //mux2.channel(15);
    buttons[i].send();
   
  }
  extraBut.send();
  extraBut1.send();
  extraBut2.send();
}




/////////////////////////////////////////////////////////////////////////////////Setup and Loop//////////////////////////////////////////////////////////////////////











void setup() {

    for(int i = 14; i <14 + totalPots; i++){
        potsRead[i-14] = MIDIpot(i, 22+i);
    }
    for(int i = 0;  i < totalButtons; i++){
        buttons[i] = MIDIswitch(mux0Common, 50+i);
    }

  //Assign controls to their grouping




    pinMode(6, INPUT_PULLUP); //2
  pinMode(14, INPUT); //mux0
  pinMode(15, INPUT); //mux1
  //pinMode(34, INPUT_PULLUP); //2
  pinMode(35, INPUT_PULLUP); //3
  pinMode(36, INPUT_PULLUP); //4
  pinMode(37, INPUT_PULLUP); //5
  pinMode(38, INPUT_PULLUP);// 6
  pinMode(39, INPUT); //7
  

  //leftFilter.setActivityThreshold(15.0f);
  //usbMIDI.setHandleNoteOff(OnNoteOff);
  //usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleControlChange(OnControlChange);
  
}





void loop() {

//handle pots and encoders
  pots();

//handle buttons
  buttonsHandler();

  while (usbMIDI.read())
  {

  }
}


  /*
  crossfader.update();
  if (crossfader.hasChanged()) {
    midiSend(24, map(crossfader.getValue(), 1023, 0, 0, 127));
  }

  leftVolume.update();
  if (leftVolume.hasChanged()) {
    midiSend(26, map(leftVolume.getValue(), 1023, 0, 0, 127));
  }

  rightVolume.update();
  if (rightVolume.hasChanged()) {
    midiSend(25, map(rightVolume.getValue(), 1023, 0, 0, 127));
  }
  */
  



// void effects() {
//   int leftEffect1 = map(analogRead(A4), 0, 1023, 0, 1);
//   if (leftEffect1 != values[0]) {
//     values[0] = leftEffect1;
//     digitalWrite(13, HIGH);
//     if (leftEffect1 == 1) {
//       usbMIDI.sendNoteOn(29, 127, 1);
//       delayMicroseconds(100);
//       usbMIDI.sendNoteOff(29, 0, 1);
//     } else {
//       usbMIDI.sendNoteOn(30, 127, 1);
//       delayMicroseconds(100);
//       usbMIDI.sendNoteOff(30, 0, 1);
//     }
//     digitalWrite(13, LOW);
//   }

//   int rightEffect1 = map(analogRead(A3), 0, 1023, 0, 1);
//   if (rightEffect1 != values[1]) {
//     values[1] = rightEffect1;
//     digitalWrite(13, HIGH);
//     if (rightEffect1 == 1) {
//       usbMIDI.sendNoteOn(31, 127, 1);
//       delayMicroseconds(1);
//       usbMIDI.sendNoteOff(31, 0, 1);
//     } else {
//       usbMIDI.sendNoteOn(32, 127, 1);
//       delayMicroseconds(1);
//       usbMIDI.sendNoteOff(32, 0, 1);
//     }
//     digitalWrite(13, LOW);
//   }
// }





