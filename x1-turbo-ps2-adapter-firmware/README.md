# Sharp X1/X1turbo PS/2 Keyboard Adapter
This repository contains code and a PCB design to implement a PS/2 keyboard adapter for [the Sharp X1 and X1turbo 8-bit personal computers](https://en.wikipedia.org/wiki/X1_(computer)).

# Current Status
Proof-of-concept that generates one key packet over and over. Does not read PS/2 keyboard.

## Known Issues
 * Doesn't work

Tested on the following computers so far:
 * CZ-852C X1 turbo

Please submit a pull request if you successfully use this on another model of X1.

# How to Build

# Special Thanks
 * [x1center](http://www.x1center.org/), for the amazing PDF that describes the pinout, protocol, and timing of the keyboard
 * [Kyoichi Sato](https://kyouichisato.blogspot.com/2014/07/sharp-x1-ps2.html) for the extended description of the protocol which this adapter is ultimately based off of
 * The contributors of the [PS/2 keyboard library for Arduino](https://www.pjrc.com/teensy/td_libs_PS2Keyboard.html)
