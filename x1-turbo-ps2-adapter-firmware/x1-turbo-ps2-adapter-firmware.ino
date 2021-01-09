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
  KeyState Ascii; // 0 = Off
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
}

void Transmit_KeyState(const KeyState& keyState) {
  
}

void Transmit_ModeA(const ModeA_Packet& keyUpdate) {
  // TODO: emit the headers
  // emit the first 8 values - all active-low bits
  for(unsigned short i = 0; i < 8; ++i) {
    Transmit_Bit_ModeA(((Bit*)&keyUpdate)[i]);
  }
  // emit the KeyState as active-high bits
  Transmit_KeyState(keyUpdate.Ascii);
  
  // TODO: emit the footers
}

void Transmit_Bit_ModeB(const Bit& b) {
  // remember, Bit is active-low, so if it's true... send 0
}

void Transmit_ModeB(const ModeB_Packet& state) {
  // TODO: emit header
  // emit key states - each one is an active-low "bit"
  for(unsigned short i = 0; i < 24; ++i) {
    Transmit_Bit_ModeB(((Bit*)&state)[i]);
  }
  
  // TODO: emit footer
}

bool isModeB = false;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  ModeA_Packet modeAState;
  ModeB_Packet modeBState;

  // TODO: detect mode switch
  // TODO: collect key state and generate packets

  if(isModeB) {
    Transmit_ModeB(modeBState);
    Transmit_ModeA(modeAState);
  } 
  else {
    Transmit_ModeA(modeAState);
  }

  // if mode A:
  //  send mode A
  // if mode B:
  //  send mode B
  //  send mode A
}
