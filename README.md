# Ryan's broke person xdj
This is my attempt to make a dj deck with the same functionality as an XDJ-RX3 for very cheap. The main computer is a Raspberry Pi 4 with a 7 inch touchscreen for the main screen. This pi is running Mixxx, the dj software, and is using a skin called Pioneered which is a look alike to actual Pioneer products. The midi controller is a teensy 4.1, using 8 CD74HC ... multiplexers to give a total of 128 pins. There are a crap ton of buttons, and a bunch of anolog pots. I am also using cheap ky-040 rotary encoders for right now. The decks need to be upgraded to a optical encoder at some point.

# TODO
need to confirm midi functionality.
need to create mappings for mixxx, likley based on ddj flx4
need to design deck and mounting
need to assemble and actually make it work


# Original details below
# Pappenspieler
The Pappenspieler is a MIDI DJ Controller based on the Teensy 3.6. You can use it to control any MIDI Software like Virtual DJ. 

### Setup
#### Dependencies
Things like libraies used in this project
- https://github.com/dxinteractive/ResponsiveAnalogRead

#### Hardware

#### Virtual DJ
Copy files from VDJ Config Pappenspieler over to "%userprofile%\Documents\VirtualDJ". These will add the necessary configuraitons to Virtual DJ.
