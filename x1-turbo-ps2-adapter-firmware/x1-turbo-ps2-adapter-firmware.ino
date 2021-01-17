#include "PS2Keyboard.h"

// The output pin, which goes to the middle ring of the X1's keyboard connector.
#define PIN_X1_OUTPUT 13

// data pin of the PS/2 port
#define PIN_PS2_DATA 5
// clock pin of the PS/2 port; should be wired to an interrupt-capable pin of the Arduino
#define PIN_PS2_INTERRUPT PD2

PS2Keyboard keyboard;

// A byte structure - meant to be sent as eight active-high bits
typedef unsigned char KeyState;

// A single bit structure - meant to be sent as active-low
typedef bool Bit;

// Packet sent in both Mode A and Mode B
typedef struct {
  // Toggles
  Bit isFromNumpad;
  Bit isKeyInput;
  Bit isKeyRepeat;
  
  // Accelerators
  Bit Graph;
  Bit CapsLock;
  Bit Kana;
  Bit Shift;
  Bit Ctrl;  

  // The actual key that was pushed
  KeyState Ascii; // 0 = Off ("Key Up")
} ModeA_Packet;

// Packet sent in Mode B
typedef struct {
  Bit Q;
  Bit W;
  Bit E;
  Bit A;
  Bit D;
  Bit Z;
  Bit X;
  Bit C;
  Bit NumPad7;
  Bit NumPad4;
  Bit NumPad1;
  Bit NumPad8;
  Bit NumPad2;
  Bit NumPad9;
  Bit NumPad6;
  Bit NumPad3;
  Bit Esc;
  Bit NumRow1;
  Bit Minus;
  Bit Plus;
  Bit Asterisk;
  Bit HTab;
  Bit Space;
  Bit Return;
} ModeB_Packet;

void Transmit_Low_ModeA() {
  // send a 0 - low for 250us, high for 750us, total of 1000us
  digitalWrite(PIN_X1_OUTPUT, LOW);
  delayMicroseconds(250);
  digitalWrite(PIN_X1_OUTPUT, HIGH);
  delayMicroseconds(750);
}

void Transmit_High_ModeA() {
  // send a 1 - low for 250us, high for 1750us, total of 2000us
  digitalWrite(PIN_X1_OUTPUT, LOW);
  delayMicroseconds(250);
  digitalWrite(PIN_X1_OUTPUT, HIGH);
  delayMicroseconds(1750);
}

void Transmit_Bit_ModeA(const Bit& b) {
  // remember, Bit is active-low, so if it's true... send 0
  if(b) {
    // active-low
    Transmit_Low_ModeA();
  }
  else {
    // not active, send high
    Transmit_High_ModeA();
  }
}

void Transmit_KeyState(const KeyState& keyState) {
  // send bits 7..0 of the ASCII code of this keystate
  for(unsigned char mask = 0x80; mask > 0x00; mask >>= 1) { // verified correct
    unsigned char b = keyState & mask;
    Transmit_Bit_ModeA(!b); // send the ASCII code "straight" as per https://kyouichisato.blogspot.com/2014/07/sharp-x1-ps2.html
  }
}

void Transmit_ModeA(const ModeA_Packet& keyUpdate) {
  // emit header - low for > 1000us, high for 700us
  digitalWrite(PIN_X1_OUTPUT, LOW);
  delayMicroseconds(1000);
  digitalWrite(PIN_X1_OUTPUT, HIGH);
  delayMicroseconds(700);
  
  // emit start - a zero - the two guides don't agree if this is needed
  //Transmit_Bit_ModeA(0xFF); // this is correct - PDF says 250 + 750us, so it's an active low
  
  // emit the first 8 state flags - all active-low bits
  for(unsigned char i = 0; i < 8; ++i) {
    // TODO: Test this loop
    Transmit_Bit_ModeA(((Bit*)&keyUpdate)[i]);
  }
  
  // emit the key being pressed (KeyState) again as active-low bits
  Transmit_KeyState(keyUpdate.Ascii);
  
  // emit the "STOP" footer - low for 250us
  digitalWrite(PIN_X1_OUTPUT, LOW);
  delayMicroseconds(250);

  // return to high now that the frame has stopped
  digitalWrite(PIN_X1_OUTPUT, HIGH);
}

void Transmit_Bit_ModeB(const Bit& b) {
  // Mode B doesn't seem to do the active-low stuff?

  if(b) {
    // 1 - 250us L, 750us H
    digitalWrite(PIN_X1_OUTPUT, LOW);
    delayMicroseconds(250);
    digitalWrite(PIN_X1_OUTPUT, HIGH);
    delayMicroseconds(750);
  }
  else {
    // 0 - 250us L, 250us H
    digitalWrite(PIN_X1_OUTPUT, LOW);
    delayMicroseconds(250);
    digitalWrite(PIN_X1_OUTPUT, HIGH);
    delayMicroseconds(250);  
  }
}

void Transmit_ModeB(const ModeB_Packet& state) {
  // TODO: Untested!
  digitalWrite(PIN_X1_OUTPUT, LOW);
  delayMicroseconds(400);
  digitalWrite(PIN_X1_OUTPUT, HIGH);
  delayMicroseconds(200);  

  // send a '0' to start the frame
  Transmit_Bit_ModeB(0x0);

  // emit key states - each one is an active-low "bit"
  for(unsigned short i = 0; i < 24; ++i) {
    Transmit_Bit_ModeB(((Bit*)&state)[i]);
  }
  
  // emit footer
  digitalWrite(PIN_X1_OUTPUT, LOW);
  delayMicroseconds(250);

  // continue...
  digitalWrite(PIN_X1_OUTPUT, HIGH);
}

int index = 0;

void UpdateKeyboardState(ModeA_Packet& a, ModeB_Packet& b) {
  // NOTES:
  // - caps lock and kana LATCH, so detect them and set global state
  // - the "last key released" logic is not translating properly from the PDF so try again later

  // poll the PS/2 keyboard two ways - ASCII and key state
  // this is kind of awkward since i'm never sure which order to do this in,
  // because i suspect it's "advancing" the queue and dropping data...
  // ...after all, readScanCode is a hack I bodged in.
  //char asciiKey = keyboard.read();
  uint8_t rawScancode = keyboard.readScanCode();
  char asciiKey = 'T'; // hack

  // TODO: i think i need to re-do the scancode -> ASCII stuff.
  // this could be what blew me up last time.

  // I need to be able to detect [make] and [break] codes, and convert to ASCII.

  bool isBreakCode = (rawScancode == 0xf0);
  // TODO: handle extended code (0xe0)

  Serial.print("Raw scancode = ");
  Serial.print(rawScancode, HEX);
  Serial.print("ASCII code = ");
  Serial.println(asciiKey);
  
  if(!isBreakCode) {
    a.Ascii = asciiKey;
    a.isKeyInput = 0x01;

    if(a.Ascii & 0x20) { // uppercase
      a.Shift = 1;
    }

    Serial.println("Got key.");
  }
  else {
    // Keyboard break
    a.Ascii = 0x00; // fake "released" event
  }

  // TODO: handle caps-lock state (^= 0x20)
  // TODO: handle graph state (|= 0x80)

  // TODO: set a state based on keyboard poll
  // TODO: set b state based on keyboard poll
}

bool isModeB = false;

void setup() {
  isModeB = false;
  index = 0;

  pinMode(PIN_X1_OUTPUT, OUTPUT);
  digitalWrite(PIN_X1_OUTPUT, HIGH);

  Serial.begin(9600);

  delay(1000); // hack

  keyboard.begin(PIN_PS2_DATA, PIN_PS2_INTERRUPT);
}

void loop() {
  // Set up and zero out the key state
  ModeA_Packet modeAState = {};
  ModeB_Packet modeBState = {};

  if(!keyboard.available()) {
    return;
  }

  // TODO: detect mode switch to enable mode B mode
  
  // collect key state and generate packets
  UpdateKeyboardState(modeAState, modeBState);

  if(isModeB) {
    Transmit_ModeB(modeBState);
    Transmit_ModeA(modeAState);
  } 
  else {
    Transmit_ModeA(modeAState);
  }
}
