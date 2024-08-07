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

/*
//REncoder rEncoder(3 /* CLK Pin, 4 /* DT Pin );

void setup() {
  Serial.begin(115200);

  rEncoder.setMinEncoderPosition(-2);
  rEncoder.setMaxEncoderPosition(3);
}

void loop() {
  REncoder::Event encoderEvent = rEncoder.reState();

  switch (encoderEvent) {
    case REncoder::Event::REncoder_Event_Rotate_CW: 
      Serial.println("Rotation CW to: " 
        + String(rEncoder.getPosition()));
    break;

    case REncoder::Event::REncoder_Event_Rotate_CCW: 
      Serial.println("Rotation CCW to: " 
        + String(rEncoder.getPosition()));
    break;
  }
}

*/



// set up multiplexers
CD74HC4067 mux0(2, 3, 4, 5);  // ANALOG create a new CD74HC4067 object with its four control pins
const int mux0Common = 14; // select a pin to share with the 16 channels of the CD74HC4067
CD74HC4067 mux1(6,7,8,9);  //ANALOG create a new CD74HC4067 object with its four control pins
const int mux1Common = 15; // select a pin to share with the 16 channels of the CD74HC4067
CD74HC4067 mux2(10,11,12,13);  // create a new CD74HC4067 object with its four control pins
const int mux2Common = 34; // select a pin to share with the 16 channels of the CD74HC4067
CD74HC4067 mux3(16,17,18,19);  // create a new CD74HC4067 object with its four control pins
const int mux3Common = 35; // select a pin to share with the 16 channels of the CD74HC4067
CD74HC4067 mux4(20,21,22,23);  // create a new CD74HC4067 object with its four control pins
const int mux4Common = 36; // select a pin to share with the 16 channels of the CD74HC4067
CD74HC4067 mux5(24,25,26,27);  // create a new CD74HC4067 object with its four control pins
const int mux5common = 37; // select a pin to share with the 16 channels of the CD74HC4067
CD74HC4067 mux6(28,29,30,31);  // create a new CD74HC4067 object with its four control pins
const int mux6Common = 38; // select a pin to share with the 16 channels of the CD74HC4067
CD74HC4067 mux7(32,33,-1,-1);  // create a new CD74HC4067 object with its four control pins
const int mux7Common = 39; // select a pin to share with the 16 channels of the CD74HC4067
CD74HC4067 muxArray[8] = {mux0, mux1, mux2, mux3, mux4, mux5, mux6, mux7};
/*
template for control generation

header decleration

ResponsiveAnalogRead "control_short_name"(MUX#SigPin, true);



*/


const int DEBOUNCE_TIME = 5;

const int totalPots = 27;
const int totalEncoders = 6;
const int totalButtons = 77;
const int totalMIDIControls = 109;
const int totalControlsWEmpty = 116;
MIDIpot potsRead[totalMIDIControls];
MIDIswitch buttons[totalButtons]; 
MIDIenc encoders[totalEncoders];
int encoderPos[totalEncoders] = {0,0,0,0,0,0};

int lastTime = millis();

// ResponsiveAnalogRead crossfader(A9, true);
// ResponsiveAnalogRead leftVolume(A8, true);
// ResponsiveAnalogRead rightVolume(A7, true);
// ResponsiveAnalogRead leftFilter(A6, true);
// ResponsiveAnalogRead rightFilter(A5, true);
// ResponsiveAnalogRead leftEffect1(A4, true);
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
  delayMicroseconds(100);
  //digitalWrite(13, LOW);
}


//switch muxs to correct pin
void switchToPin(int pin){
  int muxNum = pin/16;
  muxArray[muxNum].channel(pin%16);

}
//special switch for the two pins
void switchToEncoder(int encoder){
  //int muxNum = ;
  muxArray[1].channel(5+encoder);
  if(encoder<=1){
    muxArray[6].channel(14+encoder);
  } else {
    muxArray[7].channel(encoder-2);
  }
}
void pots() {
  //potentiometers
  for(int i = 0; i < totalPots; i++){
    switchToPin(i);
    potsRead[i].send();
    
  }
//encoders
  for(int i = 0; i < totalEncoders; i++){
    switchToEncoder(i);
    encoders[i].send();
    if((millis() - lastTime)> 1){
        
        encoders[i].write(64);
        lastTime = millis();
    }

  }
  }


void buttonsHandler() {
  for (int i = 0; i < totalButtons; i++)
  {
    switchToPin(i+32);
    buttons[i].send();
   
  }
}




/////////////////////////////////////////////////////////////////////////////////Setup and Loop//////////////////////////////////////////////////////////////////////











void setup() {

  //Assign controls to their grouping
  for (int i = 0; i < totalControlsWEmpty; i++) {
      if(i < 16){
        //pots
      potsRead[i] = MIDIpot(14, i);  //mux0
      } else if(i < 21){
        //pots
      potsRead[i] = MIDIpot(15, i); //mux1
    } else if(i < 27){
      //encoders
      encoders[i-21] =MIDIenc(15, 39, i);
      //mux1 and mux 7 for clk and dt
    } else if(i < 48) {
      buttons[i-27] = MIDIswitch(34, i);
      //potsRead[i] = ResponsiveAnalogRead(34, true); //mux2 Start buttons
    } else if(i < 64){
      buttons[i-27] = MIDIswitch(35, i); //mux3
    } else if(i < 80){
      buttons[i-27] = MIDIswitch(36, i);//mux4
    } else if(i < 96){
      buttons[i-27] = MIDIswitch(37, i);//mux5
    } else if(i < 112){
      buttons[i-27] = MIDIswitch(38, i);//mux6
    } else if(i < 117){
      buttons[i-27] = MIDIswitch(39, i);//mux7
    } else {
    //do nothing
    }
  }




  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(36, INPUT);
  pinMode(37, INPUT);
  pinMode(38, INPUT);
  pinMode(39, INPUT);
  

  //leftFilter.setActivityThreshold(15.0f);
  //usbMIDI.setHandleNoteOff(OnNoteOff);
  //usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleControlChange(OnControlChange);
  delay(100);
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  if (channel == 1) {
    switch (note) {
      case 1:
        analogWrite(6, 50);
        break;
      case 2:
        analogWrite(7, 3);
        break;
      case 3:
        analogWrite(8, 50);
        break;
      case 4:
        analogWrite(9, 50);
        break;

      case 11:
        analogWrite(36, 50);
        break;
      case 12:
        analogWrite(35, 3);
        break;
      case 13:
        analogWrite(10, 50);
        break;
      case 14:
        analogWrite(11, 3);
        break;
    }
  }
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  if (channel == 1) {
    switch (note) {
      case 1:
        analogWrite(6, 0);
        break;
      case 2:
        analogWrite(7, 0);
        break;
      case 3:
        analogWrite(8, 0);
        break;
      case 4:
        analogWrite(9, 0);
        break;

      case 11:
        analogWrite(36, 0);
        break;
      case 12:
        analogWrite(35, 0);
        break;
      case 13:
        analogWrite(10, 0);
        break;
      case 14:
        analogWrite(11, 0);
        break;
    }
  }
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





