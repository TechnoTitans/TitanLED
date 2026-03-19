# TitanLED
FRC 1683's code for the LED lights on our numbers!

## Structure
```
number_x.ino
├── libraries, hardware
├── tuning
│   ├── chase effect tuning
│   ├── VU meter tuning
├── globals, I2S init, reading peak amp function, "helpers"
├── Effects
│   ├── Mode #0:
│   ├── Mode #1:
│   ├── Mode #2: smoothing sound input; mapping layers; assign colors per layer; 
│   ├── Mode #3:
│   ├── Mode #4:
└──  set up/loop function

```

## Modes
- Mode #0: Battery Saving Mode
- Mode #1: Blue-White Chase
- Mode #2: VU Meter Effect
- Mode #3: Red-White Chase
- Mode #4: Solid-Blue Color
