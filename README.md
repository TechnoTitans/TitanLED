# TitanLED
FRC 1683's code for the LED lights on our numbers!

## Structure
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
│   ├── Mode #2: smoothing sound input; mapping layers; assign colors per layer; 
│   ├── Mode #3:
│   ├── Mode #4:
│   ├── Mode #5:
└──  set up/loop function

```

## Modes
- Mode #0: Battery Saving Mode
- Mode #1: Blue-White Chase
- Mode #2: VU Meter Effect
- Mode #3: Red-White Chase
- Mode #4: Solid-Blue Color
- Mode #5: Solid-Red Color

## Purpose for Each Tester File
- default_test: used to test wire connections between the LED light strips and the Adafruit Sparkle Motion Stick board
- testReceiver/testSender: used to check if communication between numbers works
  - testSender: sends a random set of numbers
  - testReceiver: receives that set of numbers and prints them out

## How to Use
Using any of the buttons found on the back of the numbers, cycle between the different preprogrammed modes (one button on any number changes all at once). When finished, cycle to the "Battery Saving Mode" to conserve the power in the bank.
