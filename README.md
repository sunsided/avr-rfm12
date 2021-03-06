# RFM12 on ATmega328P

## A wireless sensor network using the RFM12 transceiver

An AVR implementation in progress.

### Configuration

* ATmega328P running at 16MHz (Arduino Nano + Duemilanove)
* SPI configured in mode 0 (`CPOL=0`, `CPHA=0`) with prescaler 16 (`SPR1=0`, `SPR0=1`, `SPI2X=0`), resulting in 1MHz SPI speed.
  According to the data sheet, SPI rate during FIFO access must be smaller than 2.5MHz with the default 10MHz oscillator in place:

  > Note: During FIFO access fSCK cannot be higher than fref /4, where f ref is the crystal oscillator frequency. 

  A substantially lower SPI rate (i.e. lower than transmit frequency) results in buffer underruns on the transmitter side (`RGUR` in Status Register), obviously.

### The wires.

Wiring is basically as follows, although I eventually decided to invest in one of JeeLabs' [RFM12B breakout](http://jeelabs.com/products/rfm12b-board) boards.

<p align="center">
  <img src="https://raw.github.com/sunsided/avr-rfm12/master/screenshots/breadboard.png" alt="Breadboard"/>
</p>

### ... and something visual.

From an early demo, leaving the power amplifier enabled for a second before and after transmission.

<p align="center">
  <img src="https://raw.github.com/sunsided/avr-rfm12/master/screenshots/rfm12.shot-1.jpg" alt="SDR Capture"/>
</p>

Later, the `47`th transmission of `HOLYCOW` prefixed by magic pattern `0x01020304` (payload size `8`).

<p align="center">
  <img src="https://raw.github.com/sunsided/avr-rfm12/master/screenshots/rfm12.shot-2.jpg" alt="HTerm snapshot of a successful transmission"/>
</p>
