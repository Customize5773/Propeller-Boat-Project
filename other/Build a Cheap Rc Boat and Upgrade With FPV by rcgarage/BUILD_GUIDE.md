# Cheap RC Boat with FPV Upgrade - Complete Build Guide

## Overview
This guide will help you build an affordable RC boat with First Person View (FPV) capability for under $100.

## Parts List

### Boat Frame & Hull
- **Hull**: Foam board, plastic container, or 3D printed hull ($5-15)
- **Waterproof Container**: Plastic food container or PVC tube for electronics ($3-8)
- **Propulsion**: 2x 380/390 DC motors or 2x 2030 brushless motors ($10-20)
- **Propellers**: 2x marine propellers (30-40mm diameter) ($5-10)
- **Rudder**: Servo-controlled rudder (optional if using differential drive) ($3-8)

### Electronics - Boat Side

#### Core Components
1. **Microcontroller**: Arduino Nano or Uno ($3-8)
2. **Radio Receiver**: NRF24L01+ module with antenna ($2-5)
3. **Motor Driver**: L298N dual motor driver ($3-6)
4. **Servo**: SG90 9g servo for rudder (optional) ($2-4)

#### Power System
- **Battery**: 2S or 3S LiPo (1500-2200mAh) ($15-25)
- **Voltage Regulator**: 5V Buck converter for Arduino ($2-4)
- **Battery Alarm/Monitor**: Voltage sensor module ($1-3)

#### FPV System
- **FPV Camera**: 600TVL to 1200TVL analog camera ($15-25)
  - Alternative: Cheap action cam with WiFi ($20-40)
- **VTX (Video Transmitter)**: 5.8GHz 25-200mW ($12-20)
- **VTX Antenna**: Cloverleaf or dipole ($3-8)

#### Optional Components
- **LED Lights**: WS2812B strip or basic LEDs ($2-5)
- **Buzzer**: For low battery alarm ($1)
- **GPS Module**: For position logging ($8-15)

### Electronics - Controller Side

1. **Microcontroller**: Arduino Nano or Uno ($3-8)
2. **Radio Transmitter**: NRF24L01+ module with antenna ($2-5)
3. **Joysticks**: 2x analog joystick modules ($3-6)
4. **Buttons**: Push buttons for functions ($1-2)
5. **Display**: 0.96" OLED (optional) ($3-6)
6. **Battery**: 9V battery or 2S LiPo with holder ($5-10)
7. **Case**: 3D printed or project box ($5-15)

### FPV Viewing Options

**Option 1: FPV Goggles** ($40-150)
- Budget: Eachine EV800D ($80-100)
- DIY: Box goggles with 5.8GHz receiver module ($40-60)

**Option 2: Screen Monitor** ($25-50)
- 4.3" to 7" FPV monitor with built-in receiver

**Option 3: Phone/Tablet** (WiFi FPV only)
- Use existing device with app ($0)

### Tools Required
- Soldering iron and solder
- Wire strippers
- Hot glue gun
- Screwdriver set
- Hobby knife
- Multimeter

---

## Build Instructions

### Phase 1: Electronics Testing (Do this FIRST!)

#### Step 1: Test Radio Communication
1. Upload controller code to transmitter Arduino
2. Upload boat code to receiver Arduino
3. Connect NRF24L01 modules (CE, CSN, MOSI, MISO, SCK, VCC, GND)
4. Power both and verify serial communication
5. Check LED indicators for successful transmission

**NRF24L01 Wiring:**
```
NRF24L01    Arduino
VCC    →    3.3V (IMPORTANT: NOT 5V!)
GND    →    GND
CE     →    D9
CSN    →    D10
SCK    →    D13
MOSI   →    D11
MISO   →    D12
```

#### Step 2: Test Motor Control
1. Connect L298N to Arduino:
   - IN1, IN2, IN3, IN4 to digital pins
   - ENA, ENB to PWM pins
   - Motor outputs to motors
2. Power L298N with battery (7-12V)
3. Test motor direction and speed control
4. Ensure motors run smoothly in both directions

**L298N Wiring:**
```
L298N       Arduino
IN1    →    D4
IN2    →    D3
IN3    →    D7
IN4    →    D8
ENA    →    D5 (PWM)
ENB    →    D6 (PWM)
GND    →    GND
```

#### Step 3: Test FPV System (Bench Test)
1. Connect camera to VTX
2. Power VTX (usually 5-12V depending on model)
3. Turn on FPV goggles/monitor
4. Scan for video signal
5. Verify clear video feed

---

### Phase 2: Hull Construction

#### Option A: Foam Board Hull (Easiest)
1. Download or design hull template
2. Cut foam board pieces (5-6mm thick)
3. Glue together with hot glue or foam-safe adhesive
4. Seal seams with hot glue
5. Apply waterproof coating (mod podge, epoxy, or fiberglass)
6. Test for leaks in bathtub

#### Option B: Plastic Container Hull
1. Find watertight container (food storage, soap dish)
2. Create motor mounts using PVC or 3D printed parts
3. Drill holes for motor shafts (seal with hot glue)
4. Add ballast weight in bottom for stability

#### Option C: 3D Printed Hull
1. Find hull design on Thingiverse or design your own
2. Print in sections if necessary
3. Glue sections together
4. Seal layer lines with epoxy or acetone smoothing
5. Test waterproofing thoroughly

---

### Phase 3: Waterproofing Electronics

**Critical: This is the most important step!**

1. **Electronics Box**:
   - Use waterproof container (food storage, PVC tube with caps)
   - Create cable glands for wires (use hot glue or rubber grommets)
   - Mount Arduino, motor driver, and receiver inside
   - Add silica gel packets for moisture control

2. **Wire Sealing**:
   - Use heat shrink on all connections
   - Apply conformal coating or nail polish on circuit boards
   - Seal entry points with hot glue or silicone

3. **FPV Camera**:
   - Most FPV cameras are water-resistant, not waterproof
   - Create housing from PVC pipe or 3D printed case
   - Use clear acrylic or polycarbonate for lens cover
   - Ensure camera angle is adjustable (20-30° up recommended)

4. **VTX Placement**:
   - Mount above waterline if possible
   - Use waterproof case if mounting low
   - Keep antenna vertical and above water

---

### Phase 4: Assembly

#### Step 1: Motor Installation
1. Mount motors in hull (stern/rear of boat)
2. Install propellers on motor shafts
3. Ensure motors are below waterline
4. Angle motors slightly down (5-10°) for better thrust
5. Test motor rotation direction (propellers should push water back)

#### Step 2: Electronics Mounting
1. Place electronics box in center of hull for balance
2. Secure with velcro or hot glue
3. Keep weight as low as possible for stability
4. Connect motor wires through cable glands

#### Step 3: Power System
1. Mount battery with velcro (for easy removal)
2. Connect battery alarm/monitor
3. Add main power switch (accessible from outside)
4. Wire 5V regulator for Arduino and receiver

#### Step 4: FPV System Installation
1. Mount FPV camera at bow (front) of boat
2. Position VTX antenna vertically
3. Route video cable cleanly
4. Secure all components with hot glue
5. Keep antenna above waterline

#### Step 5: Final Touches
1. Add rudder servo if using (mount at stern)
2. Install LED lights if desired
3. Add identification (name, contact info)
4. Apply waterproof labels
5. Test all systems before sealing

---

### Phase 5: Controller Assembly

1. **Housing**:
   - 3D print controller case or use project box
   - Ensure comfortable grip
   - Add lanyard attachment point

2. **Component Layout**:
   - Mount joysticks for thumb access
   - Place buttons within easy reach
   - Position display for clear viewing
   - Install power switch

3. **Wiring**:
   - Solder joysticks to analog pins
   - Connect buttons with pull-up resistors (or use internal pull-ups)
   - Wire NRF24L01 module
   - Add power indicator LED

4. **Power**:
   - Install battery holder (9V or 2S LiPo)
   - Add charging port for LiPo (with protection circuit)

---

## Testing Procedure

### Pre-Water Tests

1. **Range Test (on land)**:
   - Test radio range with both devices powered
   - Should achieve 100-300m line of sight
   - Check for interference from WiFi/Bluetooth

2. **Motor Test**:
   - Run motors at various speeds
   - Check for overheating
   - Verify no water enters electronics box

3. **Battery Test**:
   - Run full throttle for 5 minutes
   - Monitor battery voltage
   - Check low voltage cutoff works

4. **FPV Test**:
   - Verify video clarity at various distances
   - Check for interference
   - Test video range (typically 300-1000m)

### First Water Test

1. **Shallow Water Test**:
   - Test in ankle-deep water first
   - Check for leaks immediately
   - Retrieve and dry if water detected

2. **Stability Test**:
   - Check boat floats level
   - Add ballast weight if needed
   - Ensure proper trim (slightly bow-up is good)

3. **Propulsion Test**:
   - Test forward/reverse
   - Check turning response
   - Verify adequate power

4. **FPV Test on Water**:
   - Check video quality underway
   - Verify antenna stays above water
   - Test range gradually

---

## Tuning and Optimization

### Improve Speed
- Use higher KV brushless motors (2000-3000 KV)
- Increase propeller size (within motor limits)
- Reduce weight
- Streamline hull shape
- Use 3S LiPo instead of 2S

### Improve Stability
- Lower center of gravity (add ballast)
- Widen hull beam (width)
- Add stabilizer fins
- Move battery forward/backward for balance

### Improve Control
- Adjust steering sensitivity in code
- Fine-tune deadzone values
- Add exponential curve to controls
- Implement rate limiting for smoother control

### Improve FPV
- Upgrade to higher quality camera (1200TVL+)
- Use circular polarized antennas (cloverleaf + pagoda)
- Increase VTX power (check local regulations)
- Add OSD (On-Screen Display) for battery voltage, signal strength

### Extend Range
- Use LR (Long Range) radio system (LoRa, ExpressLRS)
- Upgrade to higher gain antennas
- Use 2.4GHz for control, 5.8GHz for video
- Consider 1.2GHz for extended video range (check regulations)

---

## Troubleshooting

### Radio Issues
**Problem**: No connection between controller and boat
- Check NRF24L01 wiring (especially 3.3V not 5V!)
- Verify address matches in both codes
- Add 10µF capacitor across VCC/GND on NRF24L01
- Try different RF channel

**Problem**: Short range or intermittent connection
- Use NRF24L01+PA+LNA version with external antenna
- Keep modules away from metal objects
- Check battery voltage (weak battery = weak signal)
- Reduce other RF interference sources

### Motor Issues
**Problem**: Motors don't run or run weakly
- Check battery voltage (should be >7V for 2S)
- Verify L298N connections
- Check motor driver isn't overheating
- Ensure PWM pins are used correctly

**Problem**: Motors run in wrong direction
- Swap motor wire polarity
- Or modify code to reverse direction

### FPV Issues
**Problem**: No video signal
- Verify VTX has power (check with multimeter)
- Match VTX and goggles/monitor frequencies
- Check camera connection
- Ensure VTX antenna is connected BEFORE powering on

**Problem**: Poor video quality
- Move VTX antenna away from motors/ESCs
- Add LC filter to power supply
- Use shielded video cable
- Increase VTX power

**Problem**: Video cuts out at distance
- Upgrade antennas (circular polarized)
- Check for obstructions
- Ensure antenna stays vertical
- Consider higher power VTX

### Waterproofing Issues
**Problem**: Water enters electronics box
- Recheck all cable entry points
- Apply more hot glue/silicone
- Consider using marine sealant
- Add secondary containment (ziplock bag)

---

## Safety Guidelines

### Battery Safety
- Never overcharge LiPo batteries
- Store at 3.8V per cell (storage charge)
- Use LiPo-safe charging bag
- Dispose of damaged batteries properly
- Never leave charging unattended

### Operating Safety
- Always maintain visual line of sight
- Check local regulations for RC boats and FPV
- Avoid swimming areas and busy waterways
- Have recovery plan (long stick, kayak, etc.)
- Don't operate in rain or storms
- Bring spare battery and tools

### Electrical Safety
- Disconnect battery when working on electronics
- Use fuses on main power lines
- Insulate all connections properly
- Test with multimeter before first power-up
- Keep water away from charging batteries

---

## Upgrades and Future Improvements

### Short Term
1. Add GPS for position tracking
2. Install LED navigation lights
3. Add buzzer for low battery warning
4. Implement return-to-home function
5. Add water temperature sensor

### Medium Term
1. Upgrade to brushless motors and ESCs
2. Add 4G/5G telemetry for unlimited range
3. Install DVR for recording flights
4. Add gimbal for camera stabilization
5. Implement autonomous waypoint navigation

### Long Term
1. Build custom PCB to reduce size
2. Add sonar for depth/obstacle detection
3. Implement computer vision for object tracking
4. Create underwater inspection capabilities
5. Build competition-grade racing boat

---

## Estimated Costs

### Budget Build (~$80-100)
- Hull: Foam board DIY ($5)
- Electronics: Arduino + NRF24L01 ($10)
- Motors: 2x 380 DC motors ($12)
- Motor Driver: L298N ($5)
- Battery: 2S LiPo 1500mAh ($15)
- FPV Cam: 600TVL ($15)
- VTX: 25mW 5.8GHz ($12)
- Goggles: DIY box goggles ($30)
- Misc parts: $10

### Mid-Range Build (~$150-200)
- 3D printed hull ($20)
- Quality electronics ($25)
- Brushless motors 2x ($30)
- ESCs 2x ($20)
- 3S LiPo 2200mAh ($25)
- 1200TVL camera ($20)
- 200mW VTX ($18)
- Budget FPV goggles ($80)
- Misc upgrades: $20

### High-End Build (~$300-400)
- Professional hull ($50)
- Flight controller (iNav/Ardupilot) ($50)
- High-performance motors ($60)
- Quality ESCs ($40)
- 4S LiPo + charger ($60)
- HD FPV system (DJI/Walksnail) ($200)
- Quality goggles ($150)
- Telemetry system: $40

---

## Code Modifications

### Adding GPS Tracking
```cpp
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial gpsSerial(RX_PIN, TX_PIN);

void setup() {
  gpsSerial.begin(9600);
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
  
  if (gps.location.isValid()) {
    float lat = gps.location.lat();
    float lng = gps.location.lng();
    // Send coordinates via telemetry
  }
}
```

### Adding Return-to-Home
```cpp
float homeLat, homeLng;
bool homeSet = false;

void setHome() {
  if (gps.location.isValid()) {
    homeLat = gps.location.lat();
    homeLng = gps.location.lng();
    homeSet = true;
  }
}

void returnToHome() {
  if (!homeSet) return;
  
  float currentLat = gps.location.lat();
  float currentLng = gps.location.lng();
  
  // Calculate bearing to home
  float bearing = calculateBearing(currentLat, currentLng, homeLat, homeLng);
  
  // Adjust steering to head toward home
  // Implementation depends on your setup
}
```

### Adding Telemetry Data
```cpp
struct TelemetryData {
  float latitude;
  float longitude;
  float speed;
  float heading;
  float batteryVoltage;
  int rssi;
  unsigned long uptime;
};

void sendTelemetry() {
  TelemetryData data;
  data.latitude = gps.location.lat();
  data.longitude = gps.location.lng();
  data.speed = gps.speed.kmph();
  data.heading = gps.course.deg();
  data.batteryVoltage = readBatteryVoltage();
  data.rssi = getRSSI();
  data.uptime = millis();
  
  // Send via radio or serial
}
```

---

## Resources and Links

### Design Resources
- Thingiverse: Search "RC boat hull"
- Printables: RC boat designs
- RCGroups.com: RC boat forum
- YouTube: "DIY RC boat" tutorials

### Parts Suppliers
- AliExpress: Cheap electronics
- Banggood: FPV and RC parts
- Amazon: Fast shipping options
- Local hobby shops: Immediate availability

### Software Tools
- Arduino IDE: Programming
- Fusion 360: 3D design (free for hobbyists)
- Betaflight Configurator: If using flight controller
- Mission Planner: For autonomous navigation

### Learning Resources
- Arduino Project Hub
- Instructables: RC boat projects
- Oscar Liang: FPV guide
- Painless360: RC tutorials

---

## Maintenance

### After Each Use
1. Rinse with fresh water (if used in saltwater)
2. Dry all components thoroughly
3. Check for water ingress
4. Inspect propellers for damage
5. Check battery voltage
6. Clean camera lens

### Weekly (if used frequently)
1. Check all connections for corrosion
2. Re-apply hot glue if needed
3. Test radio range
4. Inspect hull for cracks
5. Clean motor bearings

### Monthly
1. Full waterproofing inspection
2. Replace worn propellers
3. Check motor performance
4. Update firmware if needed
5. Balance and charge all batteries

---

## Legal Considerations

- Check local regulations for RC boats
- FPV may require amateur radio license (depends on power/frequency)
- Respect no-wake zones
- Stay away from swimming areas
- Follow boating right-of-way rules
- Consider insurance for expensive builds

---

**Have fun building and happy boating! 🚤**
