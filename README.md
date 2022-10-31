# Sharp X1/X1turbo PS/2 Keyboard Adapter
This repository contains code and a PCB design to implement a PS/2 keyboard adapter for [the Sharp X1 and X1turbo 8-bit personal computers](https://en.wikipedia.org/wiki/X1_(computer)).

# Current Status
Proof-of-concept that reads the PS/2 keyboard and generates a "pressed" event. Does not correctly send key-up events yet as the method is ambiguous and will need some experimentation - pull requests welcome.

## Known Issues
 * Doesn't work

Tested on the following computers so far:
 * CZ-852C X1turbo

Please submit a pull request adding to this list if you successfully use this on another model of X1.

# How to Build
Currently, the easiest way to build the adapter is to get ahold of an Arduino Uno or similar evaluation board. Other boards will likely work (i.e. cheap Arduino Nano eBay clones) but the pin assignments may change.

Using the Arduino IDE, you can program it with the firmware in the firmware subdirectory. You will need to add [the "PS2KeyAdvanced" library](https://www.arduino.cc/reference/en/libraries/ps2keyadvanced/) to the project using the Library Manager.

Then you can strip a common stereo headphone plug (maybe you have some broken headphones lying around?) and wire the middle ring of that plug to digital pin 13 of the Arduino. Wire the rearmost ring of the plug to ground.

For testing purposes, I've supplied the Arduino with +5V from USB, rather than get it from the tip of the X1's keyboard connector. I recommend not attempting to power from both at the same time.

Connect a PS/2 keyboard port's DATA line to pin 5, and its CLOCK pin to pin 2. You will also want to wire the PS/2 port's GND and +5V pins appropriately, otherwise the keyboard won't be powered.

# Special Thanks
 * [x1center](http://www.x1center.org/), for the amazing PDF that describes the pinout, protocol, and timing of the keyboard
 * [Kyoichi Sato](https://kyouichisato.blogspot.com/2014/07/sharp-x1-ps2.html) for the extended description of the protocol which this adapter is ultimately based off of
 * The contributors of the [PS/2 keyboard library for Arduino](https://www.pjrc.com/teensy/td_libs_PS2Keyboard.html)
 * The developer of [the PS2KeyAdvanced library for Arduino](https://github.com/techpaul/PS2KeyAdvanced)
