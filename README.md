# arduino_dram4164_tester
A simple 4164 DRAM tester for Commodore 64 RAM chips using an Arduino UNO

This is a modified version of the DRAM test program by marekl73 found here:

https://ezcontents.org/4164-dynamic-ram-arduino

Schematics are identical to what you find on the arduino playground except there is an additional LED.  (GREEN on pin 12 and RED on pin 7).  Use 220 ohm resistors for each.

https://create.arduino.cc/editor/marekl73/b565f0c6-8055-41e2-a38b-35dc9009f218/preview

## Usage

The test begins by flashing the GREEN LED 3 times quickly at power up.

It runs through 6 tests:

1. RANDOM PATTERN 1
2. RANDOM PATTERN 2
3. ALL 1's
4. ALL 0's
5. ALTERNATING PATTERN 1
6. ALTERNATING PATTERN 2 

The LEDs will alternate RED/GREEN while a test is running. This is normal.

After each test, it blinks GREEN as many times as the test # above.
If all tests pass, it will slow blink GREEN until shutdown.
If at any time there was an error, it will show solid RED.

Status messages are printed to serial but is not necessary for operation.

I make no guarantees for this program.  It does not test DRAM refresh, only store/load matches expected values.  I have used it on 120ns, 150ns and 200ns 4164 DRAM and it seems to work for me.  Use at your own risk.
