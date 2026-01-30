# RC Boat with Ultrasonic Sensors - Setup Guide

## Components Needed

### Electronics
- 1x Arduino Uno/Nano
- 1x L298N Motor Driver Module (or L293D Motor Shield)
- 2x DC Motors (with propellers or paddle wheels)
- 4x HC-SR04 Ultrasonic Sensors
- 1x RF 433MHz Receiver Module
- 1x RF 433MHz Transmitter/Remote
- 1x Battery Pack (7.4V LiPo or 6x AA batteries)
- Jumper wires
- Breadboard (optional)

### Mechanical
- Boat hull (foam, plastic, or 3D printed)
- Wooden platform/deck
- Motor mounts
- Waterproof enclosure for electronics
- Silicone sealant

---

## Wiring Diagram

### Motor Driver (L298N) Connections

```
L298N Module          Arduino
-----------------------------------
ENA                   (Connect to 5V for full speed, or PWM pin for speed control)
IN1                   Pin 5  (Motor Left Forward)
IN2                   Pin 6  (Motor Left Backward)
IN3                   Pin 9  (Motor Right Forward)
IN4                   Pin 10 (Motor Right Backward)
ENB                   (Connect to 5V for full speed)
GND                   GND
12V                   Battery + (7.4V-12V)
5V                    (Can power Arduino if using 7.4V+ battery)

Motor A (Left)        Connect to OUT1 and OUT2
Motor B (Right)       Connect to OUT3 and OUT4
```

### Ultrasonic Sensors Connections

```
Sensor Position    TRIG Pin    ECHO Pin
-------------------------------------------
Forward            Pin 2       Pin 3
Right              Pin 4       Pin 7
Left               Pin 8       Pin 11
Backward           Pin 12      Pin 13

All Sensors:
VCC → 5V
GND → GND
```

### RF Receiver Module

```
RF 433MHz Receiver    Arduino
-----------------------------------
VCC                   5V
GND                   GND
DATA                  Pin 2 (for interrupt) or A0
```

---

## Pin Summary

```
Arduino Pin    Component
--------------------------------
2              Ultrasonic FWD TRIG / RF Receiver (choose one)
3              Ultrasonic FWD ECHO
4              Ultrasonic RGT TRIG
5              Motor Left Forward
6              Motor Left Backward
7              Ultrasonic RGT ECHO
8              Ultrasonic LFT TRIG
9              Motor Right Forward
10             Motor Right Backward
11             Ultrasonic LFT ECHO
12             Ultrasonic BWD TRIG
13             Ultrasonic BWD ECHO
A0             RF Receiver (alternative)
```

---

## Assembly Steps

### 1. Prepare the Hull
- Ensure hull is waterproof
- Test buoyancy with all components
- Add weight distribution for stability

### 2. Mount Motors
- Position motors at rear of boat
- Ensure propellers are below waterline
- Secure with waterproof adhesive/screws

### 3. Install Sensors
- **Forward sensor**: Front of boat, angled slightly down
- **Left/Right sensors**: Sides, pointing outward at 45°
- **Backward sensor**: Rear, above motor area
- Protect sensors with waterproof coating (clear epoxy)

### 4. Electronics Placement
- Arduino and motor driver in waterproof enclosure
- Elevate electronics above potential water level
- Leave antenna outside enclosure for RF reception
- Ensure battery is secure and balanced

### 5. Power System
- Use separate power for motors (7.4V-12V)
- Arduino can be powered from L298N 5V output
- Add on/off switch for easy control

---

## Software Setup

### 1. Install Arduino IDE
Download from: https://www.arduino.cc/en/software

### 2. Install Required Libraries
```
Tools → Manage Libraries → Search and Install:
- RCSwitch (for RF control version)
```

### 3. Upload Code
1. Connect Arduino via USB
2. Select correct board: Tools → Board → Arduino Uno/Nano
3. Select correct port: Tools → Port
4. Open `.ino` file
5. Click Upload button

### 4. Calibrate Remote
1. Upload the RCSwitch version code
2. Open Serial Monitor (115200 baud)
3. Press each button on remote
4. Note the code numbers displayed
5. Update the `processCommand()` function with your codes

---

## Testing Procedure

### 1. Bench Test (Out of Water)
- [ ] Power on system
- [ ] Test each motor individually
- [ ] Verify all sensors read correctly
- [ ] Test remote control range
- [ ] Check obstacle detection

### 2. Water Test (Shallow Water First)
- [ ] Test buoyancy and stability
- [ ] Verify motors provide adequate thrust
- [ ] Test turning radius
- [ ] Check waterproofing
- [ ] Test obstacle avoidance

---

## Troubleshooting

### Motors Not Working
- Check motor driver connections
- Verify battery voltage (should be 7.4V+)
- Test motors directly with battery
- Check PWM pin assignments

### Sensors Reading Incorrectly
- Ensure sensors are level
- Check for water droplets on sensors
- Verify trigger/echo pin connections
- Test one sensor at a time

### No RF Reception
- Check antenna connection
- Verify receiver module power
- Ensure remote has fresh battery
- Test receiver module separately

### Boat Spinning
- Check motor connections (may be reversed)
- Verify both motors same model/speed
- Balance weight distribution
- Adjust code motor speeds

---

## Safety Tips

1. **Waterproofing**: Use silicone sealant, hot glue, and waterproof enclosures
2. **Battery Safety**: Never short circuit; use proper connectors
3. **Testing**: Always test in controlled environment first
4. **Range**: Stay within visual range of boat
5. **Recovery**: Attach a fishing line for easy retrieval

---

## Advanced Features (Optional)

### Add GPS Module
Track boat position and enable return-to-home feature

### Add Camera
Mount waterproof camera for FPV (First Person View)

### Autonomous Mode
Use sensors for autonomous navigation around obstacles

### LED Indicators
Add status LEDs for power, RF signal, and obstacle detection

### Speed Control
Implement variable speed control via remote

---

## Code Versions

### Version 1: Basic RC Control with Obstacle Avoidance
File: `RC_Boat_Ultrasonic.ino`
- Manual remote control
- 4-direction ultrasonic sensing
- Automatic obstacle stopping

### Version 2: RCSwitch Library Implementation  
File: `RC_Boat_RCSwitch.ino`
- Professional RF library
- Better signal processing
- Easier remote calibration

---

## Maintenance

- Rinse with fresh water after use (if used in salt water)
- Check all connections regularly
- Reapply waterproofing as needed
- Keep battery charged
- Store in dry location

---

## Resources

- Arduino Reference: https://www.arduino.cc/reference/en/
- L298N Tutorial: https://lastminuteengineers.com/l298n-dc-stepper-driver-arduino-tutorial/
- HC-SR04 Guide: https://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/
- RCSwitch Library: https://github.com/sui77/rc-switch

---

## License
This project is open source. Feel free to modify and improve!

**Happy Boating! 🚤**
