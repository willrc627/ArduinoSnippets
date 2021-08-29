# SPI Between Microcontrollers

## Description
Arduino Code for SPI (Bidirectional) Messaging between Microcontrollers


## Table of Contents
* [Setup](#setup)
* [Concepts](#concepts)


## Setup
Tested on Two Arduino Nanos. Wiring Diagram:
| <u>Controller</u> | <u>Peripheral</u> |
| ----------------- | ----------------- |
| COPI (MOSI)       | COPI (MOSI)       |
| MOSI (POCI)       | MOSI (POCI)       |
| CS (SS)           | CS (SS)           |
| SCK               | SCK               |
| GND               | GND               |


## Concepts
Some lessons learned during this project:
1. Only the Controller can initate communications, not Peripherals
2. SPI is "Bidirectional", meaning that <code>SPI.transfer()</code> returns a value from the <b><code>SPDR</code></b> pointer on the Peripheral
3. When <b><code>SPDR</code></b> is set during an interrupt, the set value is not sent to the Controller until the next time the Controller initiates at transfer
This means that the Controller and Peripheral will <i>always</i> be out of sync in that <code>indexController = 0</code> and <code>indexPeripheral = -1</code>
4. There is very little documentation on sending SPI messages larger than one Byte


## Questions

GitHub: [@willrc627](https://api.github.com/users/willrc627)