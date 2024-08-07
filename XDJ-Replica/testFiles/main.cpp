#include <Arduino.h>
//XDJ-rx3 style controller, midi integration made to be used with mixxx dj running on a raspberry pi
//TODO integrate mux switiching, confirm midi functionality

#include <ResponsiveAnalogRead.h>
#include <Bounce2.h>
#include <CD74HC4067.h>
#include <REncoder.h>
#include <MIDI.h>




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



// // set up multiplexers
// CD74HC4067 mux0(2, 3, 4, 5);  // ANALOG create a new CD74HC4067 object with its four control pins
// const int mux0Common = 14; // select a pin to share with the 16 channels of the CD74HC4067
// CD74HC4067 mux1(6,7,8,9);  //ANALOG create a new CD74HC4067 object with its four control pins
// const int mux1Common = 15; // select a pin to share with the 16 channels of the CD74HC4067
// CD74HC4067 mux2(10,11,12,13);  // create a new CD74HC4067 object with its four control pins
// const int mux2Common = 34; // select a pin to share with the 16 channels of the CD74HC4067
// CD74HC4067 mux3(16,17,18,19);  // create a new CD74HC4067 object with its four control pins
// const int mux3Common = 35; // select a pin to share with the 16 channels of the CD74HC4067
// CD74HC4067 mux4(20,21,22,23);  // create a new CD74HC4067 object with its four control pins
// const int mux4Common = 36; // select a pin to share with the 16 channels of the CD74HC4067
// CD74HC4067 mux5(24,25,26,27);  // create a new CD74HC4067 object with its four control pins
// const int mux5common = 37; // select a pin to share with the 16 channels of the CD74HC4067
// CD74HC4067 mux6(28,29,30,31);  // create a new CD74HC4067 object with its four control pins
// const int mux6Common = 38; // select a pin to share with the 16 channels of the CD74HC4067
// CD74HC4067 mux7(32,33,-1,-1);  // create a new CD74HC4067 object with its four control pins
// const int mux7Common = 39; // select a pin to share with the 16 channels of the CD74HC4067
// CD74HC4067 muxArray[8] = {mux0, mux1, mux2, mux3, mux4, mux5, mux6, mux7};
// /*
// template for control generation

// header decleration

// ResponsiveAnalogRead "control_short_name"(MUX#SigPin, true);





//simple version for testing
const int DEBOUNCE_TIME = 5;

ResponsiveAnalogRead pot1(23, true);
ResponsiveAnalogRead pot2(22, true);
ResponsiveAnalogRead pot3(21, true);
ResponsiveAnalogRead pot4(20, true);

Bounce button1(2, DEBOUNCE_TIME);
Bounce button2(3, DEBOUNCE_TIME);
Bounce button3(4, DEBOUNCE_TIME);
Bounce button4(5, DEBOUNCE_TIME);

REncoder encoder1(6, 7);
REncoder encoder2(8, 9);



// const int totalPots = 27;
// const int totalEncoders = 6;
// const int totalButtons = 77;
// const int totalMIDIControls = 109;
// const int totalControlsWEmpty = 116;
// ResponsiveAnalogRead potsRead[totalMIDIControls];
// Bounce buttons[totalButtons]; 
// REncoder* encoders[totalEncoders];
// int encoderPos[totalEncoders] = {0,0,0,0,0,0};


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
  digitalWrite(13, HIGH);
  Serial.println(device + " " + value);
  usbMIDI.sendControlChange(device, value, 1);
  delayMicroseconds(100);
  digitalWrite(13, LOW);
}


//switch muxs to correct pin
// void switchToPin(int pin){
//   int muxNum = pin/16;
//   muxArray[muxNum].channel(pin%16);

// }
// //special switch for the two pins
// void switchToEncoder(int encoder){
//   //int muxNum = ;
//   muxArray[1].channel(5+encoder);
//   if(encoder<=1){
//     muxArray[6].channel(14+encoder);
//   } else {
//     muxArray[7].channel(encoder-2);
//   }
// }
void pots() {
  //potentiometers


  pot1.update();
  pot2.update();
  pot3.update();
  pot4.update();
  if (pot1.hasChanged()) {
    Serial.println(pot1.getValue());
    midiSend(1, map(pot1.getValue(), 1023, 0, 0, 127));
  }
  if (pot2.hasChanged()) {
    midiSend(2, map(pot2.getValue(), 1023, 0, 0, 127));
  }
  if (pot3.hasChanged()) {
    midiSend(3, map(pot3.getValue(), 1023, 0, 0, 127));
  }
  if (pot4.hasChanged()) {
    midiSend(4, map(pot4.getValue(), 1023, 0, 0, 127));
  }

  REncoder::Event encoderEvent = encoder1.reState();

    switch (encoderEvent) {
      case REncoder::Event::REncoder_Event_Rotate_CW: 
        Serial.println("Rotation CW to: " 
          + String(encoder1.getPosition()));
          midiSend(5, map(encoder1.getPosition(), 127, 0, 0, 127));
      break;

      case REncoder::Event::REncoder_Event_Rotate_CCW: 
        Serial.println("Rotation CCW to: " 
          + String(encoder1.getPosition()));
          midiSend(6, map(127-encoder1.getPosition(), 127, 0, 0, 127));
      break;
    }
//encoders

  }


void buttonsHandler() {
  button1.update();
   if (button1.fallingEdge())
    {
      usbMIDI.sendNoteOn (7, 127, 1);
    }
    else if (button1.risingEdge())
    {
      usbMIDI.sendNoteOff (7, 0, 1);
    }
    
    

}




/////////////////////////////////////////////////////////////////////////////////Setup and Loop//////////////////////////////////////////////////////////////////////











void setup() {

  // //Assign controls to their grouping
  // for (int i = 0; i < totalControlsWEmpty; i++) {
  //     if(i < 16){
  //       //pots
  //     potsRead[i] = ResponsiveAnalogRead(14, true);  //mux0
  //     } else if(i < 21){
  //       //pots
  //     potsRead[i] = ResponsiveAnalogRead(15, true); //mux1
  //   } else if(i < 27){
  //     //encoders
  //     encoders[i-21] = new REncoder(15, 39);
  //     //mux1 and mux 7 for clk and dt
  //   } else if(i < 48) {
  //     buttons[i-27] = Bounce(34, DEBOUNCE_TIME);
  //     potsRead[i] = ResponsiveAnalogRead(34, true); //mux2 Start buttons
  //   } else if(i < 64){
  //     buttons[i-27] = Bounce(35, DEBOUNCE_TIME); //mux3
  //   } else if(i < 80){
  //     buttons[i-27] = Bounce(36, DEBOUNCE_TIME);//mux4
  //   } else if(i < 96){
  //     buttons[i-27] = Bounce(37, DEBOUNCE_TIME);//mux5
  //   } else if(i < 112){
  //     buttons[i-27] = Bounce(38, DEBOUNCE_TIME);//mux6
  //   } else if(i < 117){
  //     buttons[i-27] = Bounce(39, DEBOUNCE_TIME);//mux7
  //   } else {
  //   //do nothing
  //   }
  // }

  Serial.begin(9600);
  pinMode(13, OUTPUT);

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  

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





