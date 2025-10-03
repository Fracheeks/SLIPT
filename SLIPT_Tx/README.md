# ESP32 Laser Control

This project demonstrates how to control a **650 nm red laser diode** with an ESP32 using the ESP-IDF framework.  
The ESP32 drives the laser through an **NPN transistor switch** with a series resistor to limit current and protect the diode.  

## Laser Specifications
- **Operating voltage:** 5 V  
- **Wavelength:** 650 nm (red)  
- **Output power:** ~5 mW  
- **Operating current:** ≤ 40 mA  

## Hardware Setup
- ESP32 GPIO pin (e.g., GPIO26) → Base resistor → NPN transistor (e.g., S8050/S8085)  
- Laser diode + series resistor connected between **3.3V** and transistor collector  

### Simplified Wiring
ESP32 GPIO ──[Rb=650Ω]──> Base (B)  
Collector (C)── Laser ──[Rc=75Ω]── +3.3V  
Emitter (E) ──> GND

## Software
The ESP32 toggles the GPIO pin to switch the laser **ON/OFF**, or can use **PWM** to set discrete intensity levels.  

Example behavior:  
- GPIO HIGH → Laser ON  
- GPIO LOW → Laser OFF  

