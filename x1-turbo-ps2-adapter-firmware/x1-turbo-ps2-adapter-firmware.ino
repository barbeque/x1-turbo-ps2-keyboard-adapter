// The output pin, which goes to the middle ring of the keyboard connector.
#define PIN_X1_OUTPUT 13

// TODO: PS/2 input pins

// A byte structure - meant to be sent as eight active-high bits
typedef unsigned char KeyState;

// A single bit structure - meant to be sent as active-low
typedef bool Bit;

// Packet sent in both Mode A and Mode B
typedef struct {
  // Accelerators (0 = ON)
  Bit Ctrl;
  Bit Shift;
  Bit Kana;
  Bit CapsLock;
  Bit Graph;
  // Toggles (0 = YES)
  Bit isKeyRepeat;
  Bit isKeyInput;
  Bit isFromNumpad;
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

void Transmit_Bit_ModeA(const Bit& b) {
  const int fudge = 0; // smear the frames just to test if it's a timing problem
  
  // remember, Bit is active-low, so if it's true... send 0
  if(b) {
    // active-low; send a 0 - low for 250us, high for 750us
    digitalWrite(PIN_X1_OUTPUT, LOW);
    delayMicroseconds(250 + fudge);
    digitalWrite(PIN_X1_OUTPUT, HIGH);
    delayMicroseconds(750 + fudge);
  }
  else {
    // not active; send a 1 - low for 250us, high for 1750us
    digitalWrite(PIN_X1_OUTPUT, LOW);
    delayMicroseconds(250 + fudge);
    digitalWrite(PIN_X1_OUTPUT, HIGH);
    delayMicroseconds(1750 + fudge);
  }
}

void Transmit_KeyState(const KeyState& keyState) {
  // send bits 7..0 of the ASCII code of this keystate
  for(unsigned short mask = 0x80; mask > 0x00; mask /= 2) {
    unsigned char b = keyState & mask;
    Transmit_Bit_ModeA(b); // send 0x00 as 0xFF = "off"
  }
}

void Transmit_ModeA(const ModeA_Packet& keyUpdate) {
  // emit header - low for > 1000us, high for 700us
  digitalWrite(PIN_X1_OUTPUT, LOW);
  delayMicroseconds(1000);
  digitalWrite(PIN_X1_OUTPUT, HIGH);
  delayMicroseconds(700);
  
  // emit start - a zero
  Transmit_Bit_ModeA(0xFF); // actually should send a "0"
  
  // emit the first 8 state flags - all active-low bits
  for(unsigned short i = 0; i < 8; ++i) {
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
  // remember, Bit is active-low, so if it's true... send 0
  // TODO: do
}

void Transmit_ModeB(const ModeB_Packet& state) {
  // TODO: emit header
  // emit key states - each one is an active-low "bit"
  for(unsigned short i = 0; i < 24; ++i) {
    Transmit_Bit_ModeB(((Bit*)&state)[i]);
  }
  
  // TODO: emit footer
}

int index = 0;

void UpdateKeyboardState(ModeA_Packet& a, ModeB_Packet& b) {
  // TODO: actually write something to get the keyboard state from PS/2
  index = (index + 1) % 4;
  
  if(0 != index) {
    // make a fake 'pressed' event
    a.Ascii = 'T';
  }
  else {
    // make a fake 'released' event
    a.Ascii = 0x00;
  }

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
}

void loop() {
  // Set up and zero out the key state
  ModeA_Packet modeAState = {};
  ModeB_Packet modeBState = {};

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

  delay(1500); // stupid hack, but it should work
}
