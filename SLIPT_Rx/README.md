# ESP32 Solar Panel ADC Input

This project demonstrates how to safely connect a **solar panel** to an ESP32 GPIO pin (e.g., GPIO26) using a **voltage divider**.  
The divider reduces the panel voltage to safe levels for the ESP32 ADC input.  

## Hardware Setup
- Solar panel positive → Top resistor (R1) → GPIO26  
- GPIO26 → Bottom resistor (R2) → GND (ESP32 ground)  

### Example Resistor Values
- R1 = 15 kΩ  
- R2 = 33 kΩ  
- Max panel voltage = 4.8 V → GPIO sees ~3.3 V  

### Simplified Wiring
Solar Panel + ──[R1=15kΩ]──> GPIO26  
GPIO26 ──[R2=33kΩ]──> GND  

> Ensure the panel negative is connected to ESP32 GND for a common reference.  

## Software / ADC Setup
- Configure the ADC on GPIO26 for **12-bit resolution**.  
- Use **11 dB attenuation** to increase input voltage range for accurate reading.  
- Read the voltage via ADC and convert to actual voltage using the divider ratio.  

Example behavior:  
- ADC value increases as panel voltage increases  
- Maximum ADC reading corresponds to 3.3 V at the GPIO input  

## Notes
- GPIO inputs draw negligible current.  
- The voltage divider limits current from the panel, not the GPIO.  
- Optional: add a small series resistor (100–1kΩ) for extra protection.  
- The ADC conversion formula with 12-bit resolution (0–4095) and 11 dB attenuation can be used to compute the panel voltage:

