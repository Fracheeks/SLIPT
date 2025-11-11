# Apollo3 Blue Solar Panel ADC Input

This project demonstrates how to safely connect a **3.3 V solar panel** to an Apollo3 Blue ADC input to measure its output voltage under varying light conditions.

### Solar Panel Specifications

| Parameter                | Value                |
|--------------------------|-------------------|
| Model                    | LL050-4.8-37       |
| Dimensions               | 55 mm × 70 mm      |
| Maximum Power (Pmax)     | 0.5 W              |
| Voltage at Max Power (Vmp)| 3.3 V             |
| Current at Max Power (Imp)| 0.105 A           |
| Open-Circuit Voltage (Voc)| 3.6 V             |
| Short-Circuit Current (Isc)| 0.116 A          |

## Voltage Divider Concept

- The Apollo3 Blue ADC input range is **0–2.0 V** when using the internal reference.  
- To safely measure the solar panel’s 3.3 V output, use a **voltage divider**.  
- This scales the input so that 3.3 V → ~2.0 V at the ADC pin.  

### Recommended Resistor Values
- R2 = 22 kΩ (top resistor, from solar panel to A0)  
- R1 = 33 kΩ (bottom resistor, from A0 to GND)  

**Calculation:**  
\[
V_{ADC} = V_{panel} \times \frac{R1}{R1 + R2} = 3.3 \times \frac{33}{55} ≈ 1.98 V
\]  

This keeps the ADC input safely below 2.0 V while maintaining good resolution.

## ADC Characteristics

- Apollo3 Blue ADC is **12-bit**, giving readings from 0 to 4095.  
- The ADC uses an **internal 2.0 V reference** by default, offering high accuracy and stability.  
- Each ADC step represents approximately **0.49 mV**.

| Reference | Max Measurable Voltage | Resolution (12-bit) | Accuracy | Typical Use |
|------------|------------------------|---------------------|-----------|--------------|
| **2.0 V (internal)** | 0 – 2.0 V | ~0.49 mV/step | Very stable (factory-trimmed) | Most precise low-voltage sensors |
| **3.3 V (VDD)** | 0 – 3.3 V | ~0.80 mV/step | Depends on supply (noisy, variable) | Higher voltage range (less precise) |

**Summary:**  
- Use **2.0 V internal reference** for best accuracy.  
- With the voltage divider, your **3.3 V solar panel** output is safely scaled to **~2.0 V**, maximizing precision and protecting the ADC input.