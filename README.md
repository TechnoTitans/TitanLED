# TitanLED
FRC 1683's code for the LED lights on our numbers!

## How to Use the Physical Numbers
Using any of the buttons found on the back of the numbers, cycle between the different preprogrammed modes (one button on any number changes all at once). When finished, cycle to the "Battery Saving Mode" to conserve the power in the bank.

## Modes
- Mode #0: Battery Saving Mode
- Mode #1/2: Blue/Red-White Chase
- Mode #3: VU Meter Effect
- Mode #4/5: Solid-Blue/Red Color
- Mode #6/7: Alternating Red/Blue Color

## Repo Map
- fetchMACAddress/fetchMACAddress.ino: used to fetch the unique MAC addresses of each Adafruit Sparkle Motion Stick Board
- tester/
  - default_test: used to test wire connections between the LED light strips and the Adafruit Sparkle Motion Stick board
  - testReceiver and testSender: used to check if communication between numbers works
    - testSender: sends a random set of numbers
    - testReceiver: receives that set of numbers and prints them out
- v1: the code for the old, flimsy set of numbers
- v2: the code for the new, corrugated plastic set of numbers

## Structure of Code for Each Number
```
number_x.ino
├── libraries, hardware
├── chase effect tuning
├── MACAddresses
├── VU meter tuning
├── globals, ESPNOW, I2S init, reading peak amp function, "helpers"
├── Effects
│   ├── Mode #0:
│   ├── Mode #1:
│   ├── Mode #2: 
│   ├── Mode #3:
│   ├── Mode #4:
│   ├── ...
└──  set up/loop function

```
