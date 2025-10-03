# arduino-lcd-slicer
Arduino sketch that demonstrates controlling an I2C 20x4 LCD with a potentiometer. The potentiometer reading determines how many characters of a predefined message are shown on the first row of the LCD (capped at 20 characters). Debug information including the raw analog reading and the exact displayed text is printed to the Serial Monitor.

## Hardware Requirements

- Arduino Nano
- LCD1602 with I2C interface
- Potentiometer (10kΩ)

## Wiring Instructions

- Nano A4 → LCD SDA  
- Nano A5 → LCD SCL  
- Potentiometer center pin → A0  
- Potentiometer sides → 5V and GND  

## Schematic
![LCD Slicer Schematic](docs/lcd_pot_schematic.png)

Editable source: [schematic.fzz](docs/lcd_pot_schematic.fzz)
