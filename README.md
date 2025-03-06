# fpv-video-receiver
A portable and custom FPV video receiver device made on a bare ATmega328P chip.
Compatible with analog VTX, working in 4.9 and 5.8 GHz freuency bands.

## Features 
- Ability to switch channels and adjust band 
- Visual display of the frequency, channnel, band and signal strength
- Device can be connected to the laptop/smartphone/tablet to display the video stream, no drivers required

------

## Hardware components
- ATmega328P microcontroller
- 2x Buttons
- Adafruit SSD1306 OLED Display
- CVBS to USB Capture Module (Analog to Digital Converter)
- RX5808 5.8GHz receiver module and suitable antenna (SPI mod required, google is your bro)
- Additional passive components (wires, 10kOhm resistors, board etc.)

# Building/flashing the firmware
Project can be compiled in the Arduino IDE and is compatible with ATmega328p boards.

## Arduino board with bootloader
Simple asf. Open project in the Arduino IDE, choose board, flash.

## Atmega chip without bootloader
If you are using raw chip, you need additional programmer device (another Arduino or specialized devicelike USBAsp). 
If there are no external crystal in your circuit, additional fuse configuration is required to use internal oscillator.
Flashing compiled binary can be done with use of AVRDUDESS GUI software, or avrdude (CLI).

## Development
Any contributed code must be focused on 1) Efficiency 2) Readability. 
Low-level code must be well-documented and tested before the commit.

## Wiring/connection diagrams 

## To-Do
- [ ] finish writing the features and the description
- [ ] Add the diagrams
- [ ] figure out the further structure of this file 
