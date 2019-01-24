#!/usr/bin/python3

import sys
import wave

def main():
    for path in sys.argv[1:]:
        with open(path, 'rb') as f:

          with wave.open(path + ".wav", 'wb') as wav:

              # Set wave header
              wav.setnchannels(1)
              wav.setsampwidth(3)
              wav.setframerate(48000)

              # Output frames
              while True:
                  data = f.read(4)
                  if data == b'':
                      break

                  # VP output is in 32 bit containers, but only 24 bit are used
                  # We assert that the last 8 bit are sign extension
                  sign_bit = data[2] & 0x80
                  assert(data[3] == (0xFF if sign_bit else 0x00))

                  frame = data[0:3]
                  wav.writeframes(frame)

    return 0


if __name__ == '__main__':
    sys.exit(main())
