// The output pin, which goes to the middle ring of the keyboard connector.
#define PIN_OUTPUT 2

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
  // remember, Bit is active-low, so if it's true... send 0
  if(b) {
    // active-low; send a 0 - low for 250us, high for 750us
    digitalWrite(PIN_OUTPUT, LOW);
    delayMicroseconds(250);
    digitalWrite(PIN_OUTPUT, HIGH);
    delayMicroseconds(750);
  }
  else {
    // not active; send a 1 - low for 250us, high for 1750us
    digitalWrite(PIN_OUTPUT, LOW);
    delayMicroseconds(250);
    digitalWrite(PIN_OUTPUT, HIGH);
    delayMicroseconds(1750);
  }
}

void Transmit_KeyState(const KeyState& keyState) {
  // send bits 7..0 of the ASCII code of this keystate
  Transmit_Bit_ModeA(keyState & 0x80);
  Transmit_Bit_ModeA(keyState & 0x40);
  Transmit_Bit_ModeA(keyState & 0x20);
  Transmit_Bit_ModeA(keyState & 0x10);
  Transmit_Bit_ModeA(keyState & 0x08);
  Transmit_Bit_ModeA(keyState & 0x04);
  Transmit_Bit_ModeA(keyState & 0x02);
  Transmit_Bit_ModeA(keyState & 0x01);
}

void Transmit_ModeA(const ModeA_Packet& keyUpdate) {
  // emit header - low for > 1000us, high for 700us
  digitalWrite(PIN_OUTPUT, LOW);
  delayMicroseconds(1024);
  digitalWrite(PIN_OUTPUT, HIGH);
  delayMicroseconds(700);
  
  // emit start - a zero
  Transmit_Bit_ModeA(0xFF); // actually should send a "0"
  
  // emit the first 8 values - all active-low bits
  for(unsigned short i = 0; i < 8; ++i) {
    Transmit_Bit_ModeA(((Bit*)&keyUpdate)[i]);
  }
  // emit the KeyState as active-high bits
  Transmit_KeyState(keyUpdate.Ascii);
  
  // emit the "STOP" footer - low for 250us
  digitalWrite(PIN_OUTPUT, LOW);
  delayMicroseconds(250);
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
  // TODO: do this right, not just with this dumb state machine
  index = (index + 1) % 2;
  
  if(0 == index) {
    // make a fake 'pressed' event
    a.Ascii = 'R';
  }
  else {
    // make a fake 'released' event
    a.Ascii = 0x00;
  }
}

bool isModeB = false;

void setup() {
  isModeB = false;
  index = 0;

  pinMode(PIN_OUTPUT, OUTPUT);
}

void loop() {
  // Set up and zero out the key state
  ModeA_Packet modeAState;
  memset(&modeAState, 0, sizeof(ModeA_Packet));  
  ModeB_Packet modeBState;
  memset(&modeBState, 0, sizeof(ModeB_Packet));

  // TODO: detect mode switch
  // collect key state and generate packets
  UpdateKeyboardState(modeAState, modeBState);

  if(isModeB) {
    Transmit_ModeB(modeBState);
    Transmit_ModeA(modeAState);
  } 
  else {
    Transmit_ModeA(modeAState);
  }

  delay(500); // stupid hack, but it should work
}
