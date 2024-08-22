
#include <Control_Surface.h> // Include the Control Surface library
// Include the display interface you'd like to use
#include <Display/DisplayInterfaces/DisplayInterfaceSSD1306.hpp>

// ----------------------------- MIDI Interface ----------------------------- //
// ========================================================================== //

/*
   Instantiate a MIDI interface to use for the Control Surface.
*/

USBMIDI_Interface midi;
// USBDebugMIDI_Interface midi(115200);

// ----------------------------- Display setup ------------------------------ //
// ========================================================================== //

/*
   Instantiate and initialize the SSD1306 OLED display
*/

constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;

constexpr int8_t OLED_DC = 17;    // Data/Command pin of the display
constexpr int8_t OLED_reset = -1; // Use the external RC circuit for reset
constexpr int8_t OLED_CS = 10;    // Chip Select pin of the display

constexpr uint32_t SPI_Frequency = SPI_MAX_SPEED;

// Instantiate the displays
Adafruit_SSD1306 display0 {
  SCREEN_WIDTH, SCREEN_HEIGHT, &SPI,          OLED_DC,
  OLED_reset,   OLED_CS,       SPI_Frequency,
};

Adafruit_SSD1306 display1 {
  SCREEN_WIDTH, SCREEN_HEIGHT, &SPI,          OLED_DC,
  OLED_reset,   OLED_CS,       SPI_Frequency,
};

// --------------------------- Display interface ---------------------------- //
// ========================================================================== //

// Implement the display interface, specifically, the begin and drawBackground
// methods.


String effects[8] = {"Reverb", "Reverb", "Reverb", "Reverb", "Reverb", "Reverb", "Reverb", "Reverb"};
String filterEffects[8] = {"Filter", "Space", "Noise", "Crush", "Sweep", "Dub Echo", "Reverb", "Reverb"};



String effectOld= "None"; 
String beatOld = "0";
String deckOld = "0";
String statusOld = "OFF";
void updateDisplay(String effect, String beat, String deck, String status) {
    display0.clearDisplay();
    display0.setTextColor(WHITE);
    display0.setTextSize(2);
    display0.setFont(NULL);
    display0.setCursor(0, 5);
    display0.setTextWrap(0);
    display0.setCursor(22, 5);
    if(effect != ""){
        display0.println(effect);
        effectOld = effect;
    } else{
        display0.println(effectOld);
    }
    
    display0.setCursor(0, 25);
    display0.setTextWrap(0);
    display0.setCursor(46, 25);
    if(beat != ""){
        display0.println(beat);
        beatOld = beat;
    }else{
        display0.println(beatOld);
    }
    
    display0.setTextWrap(0);
    display0.setCursor(22, 41);
    if(deck != 0){
        display0.println("Deck: " + deck);
        deckOld = deck;
    }else{
        display0.println("Deck: " + deckOld);
    }
    display0.setTextSize(1);
    
    if(deck != 0){
        display0.print("EFFECT: "+ status);
        statusOld = status;
    }else{
        display0.print("EFFECT: "+ statusOld);
    }
    display0.display();
}



void updateDisplay1(String effect, String beat, String deck) {
    display0.clearDisplay();
    display0.setTextColor(WHITE);
    display0.setTextSize(2);
    display0.setFont(NULL);
    display0.setCursor(0, 5);
    display0.setTextWrap(0);
    display0.setCursor(22, 5);
    if(effect != ""){
        display0.println(effect);
        effectOld = effect;
    } else{
        display0.println(effectOld);
    }
    
    display0.setCursor(0, 25);
    display0.setTextWrap(0);
    display0.setCursor(46, 25);
    if(beat != ""){
        display0.println(beat);
        beatOld = beat;
    }else{
        display0.println(beatOld);
    }
    
    display0.setTextWrap(0);
    display0.setCursor(22, 41);
    if(deck != 0){
        display0.println("Deck: " + deck);
        deckOld = deck;
    }else{
        display0.println(deckOld);
    }
    
    display0.display();
}




void handleMidiInput(ChannelMessage cm){
    switch (cm.getData1()) //use note number to decide 
    {
    case 1:
        /* note 1, main effect unit */
        switch (cm.getData2())
        {
        case 1:
            updateDisplay(effects[1], "", "", "");
            break;
        //TODO do all the effects
        default:
            break;
        }
        break;
    
    case 2:
        /* note 2, filter secondary effect unit. COPY case 1  */
        break;
    
    case 3:
        /* chanel 3, main effect beat value ex y/ */
        break;
    
    case 4:
        /* code */
        break;
    
    case 5:
        /* code */
        break;
    
    default:
        break;
    }
}

struct MyMIDI_Callbacks : MIDI_Callbacks {
 
  // Callback for channel messages (notes, control change, pitch bend, etc.).
  void onChannelMessage(MIDI_Interface &, ChannelMessage cm) override {
    
    // Print the message
    Serial << F("Channel message: ")                                        //
           << hex << cm.header << ' ' << cm.data1 << ' ' << cm.data2 << dec //
           << F(" on cable ") << cm.cable.getOneBased() << endl;
  }
 
  // Callback for system exclusive messages
  void onSysExMessage(MIDI_Interface &, SysExMessage se) override {
    // Print the message
    Serial << F("System Exclusive message: [") << se.length << "] " //
           << AH::HexDump(se.data, se.length)                       //
           << F(" on cable ") << se.cable.getOneBased() << endl;
  }
 
  // Callback for real-time messages
  void onRealTimeMessage(MIDI_Interface &, RealTimeMessage rt) override {
    // Print the message
    Serial << F("Real-time message: ") //
           << hex << rt.message << dec //
           << F(" on cable ") << rt.cable.getOneBased() << endl;
  }
 
} callback;

// -------------------------- MIDI Input Elements --------------------------- //
// ========================================================================== //

/*
   Define all elements that listen for MIDI messages.
*/







// --------------------------------- Setup ---------------------------------- //
// ========================================================================== //

void setup() {
  // Correct relative mode for MCU rotary encoders
  RelativeCCSender::setMode(MACKIE_CONTROL_RELATIVE);
  Control_Surface.begin(); // Initialize Control Surface
  midi.begin();
  midi.setCallbacks(callback);
}

// ---------------------------------- Loop ---------------------------------- //
// ========================================================================== //

void loop() {
  Control_Surface.loop(); // Refresh all elements
}