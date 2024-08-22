
#include <Wire.h>

#include <Control_Surface.h>

#include <AH/Hardware/MCP23017Encoders.hpp>
#include <AH/Hardware/ExtendedInputOutput/MCP23017.hpp>

// Type for the MCP23017 encoders (translates encoder pulses to position)
using WireType = decltype(Wire);     // The type of I²C driver to use
using EncoderPositionType = uint8_t; // The type for saving encoder positions
using MCPEncoderType = MCP23017Encoders<WireType, EncoderPositionType>;

using namespace ExtIO; // Use the extended input/output functions
MCP23017<WireType> mcp1 {
  Wire, // The I²C driver to use
  0x04//,  // The I²C address offset (depends on the state of the address pins)
  //2,    // Optional: interrupt pin to detect input changes
};
MCP23017<WireType> mcp2 {
  Wire, // The I²C driver to use
  0x06//,  // The I²C address offset (depends on the state of the address pins)
  //2,    // Optional: interrupt pin to detect input changes
};

// Type for the MIDI encoders (translates position to MIDI messages)
struct CCMCPEncoder : GenericMIDIRotaryEncoder<MCPEncoderType::MCP23017Encoder,
                                               RelativeCCSender> {
  CCMCPEncoder(MCPEncoderType::MCP23017Encoder enc, MIDIAddress address,
               int16_t multiplier = 1, uint8_t pulsesPerStep = 4)
    : GenericMIDIRotaryEncoder(std::move(enc), address, multiplier,
                               pulsesPerStep, {}) {}
};

USBDebugMIDI_Interface midi;



// Encoders
// Create an object that manages the 8 encoders connected to the MCP23017.
MCPEncoderType enc {Wire, 0x0, 12};
//                  │     │    └─ Interrupt pin
//                  │     └────── Address offset
//                  └──────────── I²C interface



// Instantiate 8 MIDI rotary encoders.
CCMCPEncoder ccencoders[] {
  {
    enc[0],       // The encoder to use
    MCU::V_POT_1, // The MIDI address
    1,            // Encoder speed multiplier
    4,            // Number of pulses per physical "click" of the encoder
  },
  {enc[1], MCU::V_POT_2},
  {enc[2], MCU::V_POT_3},
  {enc[3], MCU::V_POT_4},
  {enc[4], MCU::V_POT_5},
  {enc[5], MCU::V_POT_6},
  {enc[6], MCU::V_POT_7},
  {enc[7], MCU::V_POT_8},
};

void setup() {
  Control_Surface.begin();
  Wire.begin(); // Must be called before enc.begin()
  Wire.setClock(800000);
  enc.begin(); // Initialize the MCP23017
}

void loop() {
  enc.update();
  Control_Surface.loop();
}