# Sharp X1/turbo keyboard protocol facts
## Connector
3.5mm male stereo phono jack.

Tip: +5V
Ring 1: Signal
Ring 2: GND

# Electrical characteristics
Ready signal is "active low"

# X1 and X1 Turbo keyboard protocol ("Mode A")
0x00 is sent on key up.

On the X1 Turbo keyboard (regardless of mode), accelerator keys (Shift, Ctrl, etc) are sent whenever they change(?).

Datagram format:
 * [Header](#header)
 * [Start](#start)
 * CTRL key (0 = pressed)
 * Shift key (0 = pressed)
 * Kana key (0 = pressed)
 * Caps-Lock key (0 = pressed)
 * Graph key (0 = pressed)
 * Repeat mode (0 = repeating)
 * Key input (0 = yes)
 * Numpad input (0 = yes, corresponding to `INKEY$(2)`)
 * Bits 7..0 ASCII code (0x00 = 0xFF, corresponding to `INKEY$(0)`)
 * [Stop](#stop)

### Header
Low for > 1000µs, then high for 700µs

### Start
Send a '0'

### Datagram
16-bit packet:
 * When value is `0`, low for 250µs, high for 750µs
 * When value is `1`, low for 250µs, high for 1750µs

(TODO: isn't this what zx81 tape signalling is?)

### Stop
250µs low

## Transmission Start Condition (X1 and X1 turbo)
| Condition                   | ASCII code |
|-----------------------------|------------|
|`INKEY$(0)` when a valid key is given | Code corresponding to pressed key |
|Constant while a valid key is held over time (repeat) | Code corresponding to pressed key |
|Last pressed "valid key" when released | 0x00 (OFF) |

## X1 Turbo change
Send whenever the accelerator key state changes. "Key input" bit can be 1 (=OFF) here.

## Something about the timing of released keys here??
TODO

# "Game Mode" (X1 Turbo only; "Mode B")
Used when the X1 turbo keyboard switch(???) is in "Mode B." Only supported by a few late-model games.

## Datagram format
 * Header (low 400µs, high for 200µs)
 * Start (send a '0')
 * 24-bits of key status (0 = 250µs L, 250µs H; 1 = 250µs L, 750µs H)
  * Keys Q, W, E, A, D, Z, X, C, 7, 4, 1, 8, 2, 9, 6, 3, ESC, 1, -, +, *, HTAB, Space, Return in that order (numbers = numpad) TODO: clumped into BYTES
 * Stop (250µs low)

## Transmission Start Condition
When any key in the "game key" set changes state.

If game key start condition and ASCII start condition overlap, game key is always transmitted first, then ASCII.
