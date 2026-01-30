# Wiring Diagrams for RC Boat with FPV

## Materials List

- Plastic board  
- Hot glue  
- Motor from an old RC car  
- Servo (Hitec)  
- Wire  
- Transmitter and receiver (FrSky FS-TH9X)

### FPV Upgrade Components

- Video transmitter and receiver (5.8 GHz, 200 mW, Boscam)  
- Camera (GoPro)  
- Goggles (Quanum V1)

## Complete Wiring Overview

### BOAT SIDE WIRING

```
===========================================
MAIN BATTERY (2S/3S LiPo 7.4V-11.1V)
===========================================
    |
    |----[Main Switch]
    |
    +-------+-------+-------+-------+
    |       |       |       |       |
    |       |       |       |       |
   GND     5V      VTX    Motor   Motor
   BAT     Buck    12V    Driver  Driver
           Conv           Power   Power
    |       |       |       |       |
    |       +-------+       +-------+
    |       |       |             |
    |    Arduino  FPV         L298N
    |    5V      Cam          Motor Driver
    |            5V
    |       
   GND (Common Ground - IMPORTANT!)


===========================================
ARDUINO CONNECTIONS
===========================================

Arduino Nano/Uno:

Pin D2  → Servo Signal (Yellow) - Rudder
Pin D3  → L298N IN2 (Motor Left Dir)
Pin D4  → L298N IN1 (Motor Left Dir)
Pin D5  → L298N ENA (Motor Left PWM)
Pin D6  → L298N ENB (Motor Right PWM)
Pin D7  → L298N IN3 (Motor Right Dir)
Pin D8  → L298N IN4 (Motor Right Dir)
Pin D9  → NRF24L01 CE
Pin D10 → NRF24L01 CSN
Pin D11 → NRF24L01 MOSI
Pin D12 → NRF24L01 MISO
Pin D13 → NRF24L01 SCK / LED Indicator
Pin A0  → Voltage Divider (Battery Monitor)

5V      → Servo Power (Red)
3.3V    → NRF24L01 VCC (CRITICAL: NOT 5V!)
GND     → Common Ground (All components)


===========================================
NRF24L01 MODULE (CRITICAL WIRING!)
===========================================

NRF24L01    →   Arduino
---------------------------
GND         →   GND
VCC         →   3.3V (NOT 5V! Will damage module!)
CE          →   D9
CSN         →   D10
SCK         →   D13
MOSI        →   D11
MISO        →   D12

NOTE: Add 10µF capacitor between VCC and GND
      on the NRF24L01 for stability


===========================================
L298N MOTOR DRIVER
===========================================

L298N       →   Connection
---------------------------
12V/VCC     →   Battery + (through switch)
GND         →   Common Ground
5V OUT      →   DO NOT USE (internal regulator)

IN1         →   Arduino D4
IN2         →   Arduino D3
IN3         →   Arduino D7
IN4         →   Arduino D8
ENA         →   Arduino D5 (PWM)
ENB         →   Arduino D6 (PWM)

OUT1        →   Left Motor +
OUT2        →   Left Motor -
OUT3        →   Right Motor +
OUT4        →   Right Motor -

NOTE: Remove 5V enable jumper if using
      battery voltage > 12V


===========================================
SERVO (RUDDER)
===========================================

Servo Wire Color:
Brown/Black  →  GND
Red          →  5V (from Arduino or BEC)
Orange/White →  Arduino D2 (Signal)


===========================================
FPV CAMERA
===========================================

Typical FPV Camera Wiring:
Red      →  5V (from Buck Converter)
Black    →  GND
Yellow   →  VTX Video IN


===========================================
VIDEO TRANSMITTER (VTX)
===========================================

VTX Pins:
Red      →  12V (or battery voltage, check specs)
Black    →  GND
Yellow   →  Camera Video OUT
         →  Antenna (MUST be connected before power!)

NOTE: NEVER power VTX without antenna!
      This will damage the transmitter!


===========================================
VOLTAGE DIVIDER (Battery Monitor)
===========================================

Battery + ----[10kΩ]----+----[10kΩ]---- GND
                         |
                    Arduino A0

For 2S (8.4V max):
Use 10kΩ and 10kΩ (divides by 2)

For 3S (12.6V max):
Use 20kΩ and 10kΩ (divides by 3)

Calculation:
Vout = Vin × (R2 / (R1 + R2))


===========================================
LED INDICATOR
===========================================

Arduino D13 → [220Ω Resistor] → LED + → LED - → GND


===========================================
POWER DISTRIBUTION
===========================================

2S/3S LiPo Battery (Main Power)
    |
    +-- Main Switch
    |
    +-- L298N (Motors) - Direct battery voltage
    |
    +-- 5V Buck Converter
    |   |
    |   +-- Arduino 5V
    |   +-- Servo 5V
    |   +-- FPV Camera 5V
    |
    +-- VTX (12V or direct battery depending on VTX)
```

---

## CONTROLLER SIDE WIRING

```
===========================================
CONTROLLER ARDUINO CONNECTIONS
===========================================

Arduino Nano/Uno:

Pin D2  → Button 1 (Lights) + Pull-up
Pin D3  → Button 2 (FPV) + Pull-up
Pin D4  → LED Indicator
Pin D9  → NRF24L01 CE
Pin D10 → NRF24L01 CSN
Pin D11 → NRF24L01 MOSI
Pin D12 → NRF24L01 MISO
Pin D13 → NRF24L01 SCK
Pin A0  → Joystick 1 VRx (Throttle)
Pin A1  → Joystick 1 VRy (Steering)

5V      → Joystick VCC, Button Pull-ups
3.3V    → NRF24L01 VCC
GND     → Common Ground


===========================================
JOYSTICK MODULE
===========================================

Each Joystick Module:
GND     →   Arduino GND
+5V     →   Arduino 5V
VRx     →   Arduino A0 (X-axis)
VRy     →   Arduino A1 (Y-axis)
SW      →   Optional button (not used in basic code)


===========================================
PUSH BUTTONS
===========================================

Button 1 (Lights):
One side → Arduino D2
Other side → GND
(Arduino internal pull-up resistor enabled in code)

Button 2 (FPV):
One side → Arduino D3
Other side → GND
(Arduino internal pull-up resistor enabled in code)

Alternative with external pull-up:
Button → Arduino Pin
Button → 10kΩ → 5V


===========================================
OLED DISPLAY (Optional)
===========================================

0.96" I2C OLED:
GND     →   Arduino GND
VCC     →   Arduino 5V
SCL     →   Arduino A5 (or SCL pin)
SDA     →   Arduino A4 (or SDA pin)


===========================================
CONTROLLER POWER
===========================================

Option 1: 9V Battery
9V Battery + → Arduino VIN
9V Battery - → Arduino GND

Option 2: 2S LiPo
2S LiPo + → Arduino VIN (through switch)
2S LiPo - → Arduino GND

Option 3: USB Power Bank
USB 5V → Arduino 5V pin
USB GND → Arduino GND
```

---

## DETAILED CONNECTION NOTES

### Critical Power Requirements

1. **NRF24L01 Module**: 
   - MUST use 3.3V (NOT 5V!)
   - Current draw: 10-50mA (can spike to 100mA during transmission)
   - Add 10µF capacitor across VCC/GND for stability
   - Use short wires (<10cm)

2. **Arduino**:
   - Can accept 7-12V on VIN pin
   - 5V regulated output available
   - Max current from 5V pin: ~500mA
   - 3.3V output: ~50mA max

3. **L298N Motor Driver**:
   - Input voltage: 5-35V (typically 7.4-11.1V for our use)
   - Logic voltage: 5V
   - Max current per channel: 2A (with heatsink)
   - Enable pins MUST be PWM for speed control

4. **Servo**:
   - Voltage: 4.8-6V
   - Current: 100-500mA (under load)
   - May require separate BEC if using many servos

5. **FPV Camera**:
   - Voltage: 5V (some cameras accept wider range)
   - Current: 80-120mA typical
   - MUST have clean power (use LC filter if noise issues)

6. **VTX**:
   - Voltage: 7-12V (check your specific model!)
   - Current: 200-500mA depending on power output
   - MUST have antenna connected before power!

### Common Ground Rule

**CRITICAL**: All components MUST share a common ground!
- Battery GND
- Arduino GND
- Motor driver GND
- NRF24L01 GND
- FPV system GND

Use thick wire (18-22 AWG) for ground connections.

### Wire Gauge Recommendations

- Main battery to motor driver: 16-18 AWG
- Motor driver to motors: 18-20 AWG
- Arduino power: 22-24 AWG
- Signal wires (servo, sensors): 24-26 AWG
- NRF24L01: 24-26 AWG (short as possible)

### Connector Types

- Battery: XT30 or XT60 (for easy disconnect)
- Motors: Solder directly or use bullet connectors
- Servo: Standard 3-pin servo connector
- FPV video: RCA, JST, or direct solder

---

## TESTING PROCEDURE

### Step 1: Power Test (No motors connected!)
1. Connect battery through main switch
2. Verify 5V buck converter output
3. Check Arduino power LED
4. Measure all voltages with multimeter

### Step 2: Component Test
1. Test NRF24L01 communication (Serial monitor)
2. Test servo movement
3. Test LED indicators

### Step 3: Motor Test (in air, not water!)
1. Connect motors to L298N
2. Upload code and test at low speed
3. Verify direction control
4. Check for overheating

### Step 4: FPV Test
1. Connect camera to VTX
2. Attach antenna to VTX
3. Power on and check video
4. Test at various positions

---

## TROUBLESHOOTING WIRING ISSUES

### No Power
- Check battery charge
- Verify main switch works
- Check all ground connections
- Test buck converter output

### NRF24L01 Not Working
- Verify 3.3V (NOT 5V!)
- Check all 7 pin connections
- Add capacitor if not present
- Try different NRF24L01 module (they're often defective)
- Reduce wire length

### Motors Not Running
- Check L298N power connections
- Verify PWM pins are used
- Check motor wire connections
- Test motors directly with battery

### Servo Jitter
- Separate servo power with capacitor
- Use external BEC for servo
- Check for EMI from motors
- Add ferrite bead to servo wire

### Poor Video Quality
- Add LC filter to VTX power
- Separate video ground
- Use shielded video cable
- Move VTX away from motors
- Check antenna connection

### Intermittent Operation
- Check all solder joints
- Verify secure connectors
- Look for loose wires
- Check for water ingress

---

## SAFETY CHECKS BEFORE FIRST USE

□ All connections secure and soldered
□ No exposed bare wires
□ Battery voltage correct for all components
□ NRF24L01 on 3.3V (NOT 5V)
□ VTX antenna connected
□ Servo moves freely
□ Motors spin correct direction
□ Failsafe code tested
□ Low voltage cutoff tested
□ Radio range tested on land
□ Waterproofing verified
□ Emergency stop accessible
□ All grounds connected

---

This wiring guide should be used in conjunction with the main build guide
and Arduino code files provided in this project.
