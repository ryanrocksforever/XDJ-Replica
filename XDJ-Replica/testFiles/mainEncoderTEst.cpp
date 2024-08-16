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
REncoder testEncoder = REncoder(mux1Common, mux7Common);

const int DEBOUNCE_TIME = 5;

const int totalPots = 21;
const int totalEncoders = 5;
const int totalButtons = 100;
const int totalMIDIControls = 109;
const int totalControlsWEmpty = 116;
MIDIpot potsRead[totalMIDIControls];
MIDIswitch buttons[totalButtons]; 
MIDIenc encoders[totalEncoders];
bool jogEncoder[totalEncoders] = {false, false, false, false, false};
int encoderPos[totalEncoders] = {0,0,0,0,0};
//MIDIenc myEnc(mux0Common, mux7Common, 24);
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
  delayMicroseconds(5);
  //digitalWrite(13, LOW);
}


//switch muxs to correct pin
void switchToPin(int pin){
  int muxNum = pin/16;
  muxArray[muxNum].channel(pin%16);
  delayMicroseconds(50);

}
//special switch for the two pins
void switchToEncoder(int encoder){
  //int muxNum = ;
  muxArray[1].channel(5+encoder);

  muxArray[7].channel(encoder);
  
  delayMicroseconds(5);
}
void pots() {
 // potentiometers
  for(int i = 0; i < totalPots; i++){
    switchToPin(i);
    delayMicroseconds(75);
    potsRead[i].send();
    
  }
//encoders NOT WORKING NEED TO FIX. CHANGING CHANNELS SEEMS TO BE THE PROBLEM.
    mux1.channel(5);
    mux7.channel(0);
    encoders[0].send();
    
    //myEnc.send();
    
    // int clk = digitalRead(mux1Common);
    // int dt = digitalRead(mux7Common);
    // encoders[0].write(clk);
    // encoders[0].send();
  
  // for(int i = 0; i < 0; i++){
  //   //totalEncoders
  //     muxArray[1].channel(5+i);
  //   delayMicroseconds(5);
  //   muxArray[7].channel(i);
    
  //   delayMicroseconds(5);
  //   encoders[i].send();
    
    
    
  // //   if(jogEncoder[i]){
  // //     //SPEEEEED
  // //      encoders[i].send();
  // //     if((millis() - lastTime)> 1){
  // //         if(encoders[i].read() > 64){
  // //           encoders[i].write(encoders[i].read()-1);
  // //         } else if(encoders[i].read() < 64){
  // //           encoders[i].write(encoders[i].read()+1);
  // //         }
  // //         encoders[i].write(64);
  // //         lastTime = millis();
  // //     }

  // // } else{
  // //   encoders[i].send();
  // //   // if((millis() - lastTime)> 1){
        
  // //   //     encoders[i].write(64);
  // //   //     lastTime = millis();
  // //   // }


    
  // // }
   

  // }
  }


void buttonsHandler() {
  for (int i = 0; i < 64; i++)
  {
    switchToPin(i+32);
    //mux2.channel(15);
    buttons[i].send();
   
  }
}




/////////////////////////////////////////////////////////////////////////////////Setup and Loop//////////////////////////////////////////////////////////////////////











void setup() {

  //Assign controls to their grouping
  for (int i = 0; i < totalControlsWEmpty; i++) {
      if(i < 16){
        //pots
      potsRead[i] = MIDIpot(mux0Common, i+40);  //mux0
      } else if(i < 21){
        //pots
      potsRead[i] = MIDIpot(mux1Common, i+40); //mux1
    } else if(i < 27){
      //encoders
      encoders[i-21] =MIDIenc(mux1Common, mux7Common, i+40); // mux1
      encoders[i-21].write(64);
      //mux1 and mux 7 for clk and dt
    } else if(i < 32) {
      //buttons[i-27] = MIDIswitch(mux1Common, i+40);
      //potsRead[i] = ResponsiveAnalogRead(34, true); //mux2 Start buttons
    } else if(i < 48) {
      buttons[i-27] = MIDIswitch(34, i+40);
      //potsRead[i] = ResponsiveAnalogRead(34, true); //mux2 Start buttons
    } else if(i < 64){
      buttons[i-27] = MIDIswitch(35, i+40); //mux3
    } else if(i < 80){
      buttons[i-27] = MIDIswitch(36, i+40);//mux4
    } else if(i < 96){
      buttons[i-27] = MIDIswitch(37, i+40);//mux5
    } else if(i < 112){
      buttons[i-27] = MIDIswitch(38, i+40);//mux6
    } else if(i < 117){
      buttons[i-27] = MIDIswitch(39, i+40);//mux7
    } else {
    //do nothing
    }
  }




  pinMode(14, INPUT); //mux0
  pinMode(15, INPUT); //mux1
  pinMode(34, INPUT_PULLUP); //2
  pinMode(35, INPUT_PULLUP); //3
  pinMode(36, INPUT_PULLUP); //4
  pinMode(37, INPUT_PULLUP); //5
  pinMode(38, INPUT_PULLUP);// 6
  pinMode(39, INPUT); //7
  

  //leftFilter.setActivityThreshold(15.0f);
  //usbMIDI.setHandleNoteOff(OnNoteOff);
  //usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleControlChange(OnControlChange);
  delay(100);
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





