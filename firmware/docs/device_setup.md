# Device setup

## Connecting programmer

TODO: define this section.

## Connecting serial port to the device

Logs are collected from the device. Currently, they are outputted on A7 Nucleo pin (PA2 on the MCU). Connect this pin
to the RXD pin on the USB-UART converter.

The config is 8 data bits, no parity, 1 stop bit, 115200 baud rate.

To make use of the automation of the tests `JEFF_SERIAL_PORT` CMake cache variable must be set. 
To set it run from the appropriate build directory:

```
cmake -DJEFF_SERIAL_PORT:STRING=<device_name> ../../device
```

e.g. on Linux:

```
cmake -DJEFF_SERIAL_PORT:STRING=/dev/ttyUSB0 ../../device
```
