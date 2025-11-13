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
ESP32 GPIO ──> Base (B)  
Collector (C)── Laser ── +3.3V  
Emitter (E) ──> GND

# ESP32 Laser Transmitter (2-bit PWM)

This project uses an ESP32 to transmit data via a laser using **PWM (Pulse Width Modulation)** with **4 intensity levels** representing 2-bit symbols.

## How It Works

- **PWM** switches the laser ON/OFF rapidly to create different perceived intensities.
- Each **byte** is split into **4 symbols** (2 bits each).
- Each 2-bit symbol sets the **PWM duty cycle**:

| Symbol | Duty Cycle | Description      |
|--------|------------|----------------|
| 00     | 0%         | OFF             |
| 01     | 33%        | Low             |
| 10     | 66%        | Medium          |
| 11     | 100%       | High            |

- Each symbol is transmitted for a fixed duration (`SYMBOL_PERIOD_MS`) for decoding by a receiver.

## Features

- Transmit any string or byte stream via laser.
- Safe PWM control for the laser diode.
- Adjustable symbol duration and PWM frequency.

