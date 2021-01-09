typedef unsigned char KeyState;

// Set to true here, sent as active low
typedef bool Bit;

struct ModeA_Packet {
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
}

void Transmit_Bit_ModeA(const Bit& b) {
  // remember, Bit is active-low, so if it's true... send 0
}

void Transmit_ModeA(const ModeA_Packet& keyUpdate) {
  
}

struct ModeB_Packet {
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
}

void Transmit_Bit_ModeB(const Bit& b) {
  // remember, Bit is active-low, so if it's true... send 0
}

void Transmit_ModeB(const ModeB_Packet& state) {
  
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
