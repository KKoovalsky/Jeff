# Building, running and testing

This is intended to be run under Linux, but it might work for Windows as well with few tweaks. 

To prepare for building and testing one should run from the `firmware/` directory:

```
mkdir -p build/build_{host,device}_{debug,release}
```

This will prepare empty directories for host-side and device-side builds, for Debug and Release configuration.

# Building firmware

```
cd build/build_device_release
cmake ../../device # CMAKE_BUILD_TYPE=Release by default
cmake --build .
```

The debug build:

```
cd build/build_device_release
cmake -DCMAKE_BUILD_TYPE=Debug ../../device
cmake --build .
```

# Setting up the device

Currently, NUCLEO-L432KC (STM32L432KC-Nucleo) is used, so all the flasher scripts use `openocd`. The Nucleo board has
on-board programmer, but STLinkV2 or JLink could work too. Anyway, the requirements:

* [openocd](https://openocd.org/)
* `python3`
* `ruby`
* Python packages: `python3 -m pip install click pyserial`

## Connecting the Serial port

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

## Flashing

```
cmake --build . --target distortion_app-flash
```

# Device tests

CTest module is used to run the tests. One can use `--verbose` flag to see more output, or `--output-on-failure`.

## 1. Audio sampler tests

Basic tests, which check the basic module operation (without signal generations checks) can be run with:

```
ctest -R test_threaded_audio_sampler
```

To see whether sampling works properly, connect function generator to the input circuit. You can connect it before the
DC offset circuit or after it, directly to the ADC input pin A0, on Nucleo board (PA0 on the MCU). Be wary that if
connecting before the DC offset circuit, the signal must be ±1.6V (no DC offset), or if connecting after it the DC
offset must be applied to 1.65 V, so the signal amplitude shall ba 1.6V.

Multiple tests are created for various input signal frequencies. For each test, corresponding input signal frequency
must be applied for the sine wave:

```
ctest -R test_sample_sine_100hz
ctest -R test_sample_sine_1khz
ctest -R test_sample_sine_10khz
ctest -R test_sample_sine_20khz
```

The test calculates FFT and checks whether the corresponding spike is in the signal. SNR is calculated basing on the
power of the corresponding spike: power of the spike is divided by the sum of powers of other frequencies. If the SNR is
higher than 0.5 (or around that value), then the test is successful.

## 2. Audio DAC tests

Running basic module tests, for firmware correctness:

```
ctest -R test_audio_dac_basic
```

To verify the DAC, one can flash 1kHz sine wave generator:

```
cmake --build . --target test_audio_dac_generates_sine_1khz-flash
```

Then connect oscilloscope to the Nucleo board pin A3 (PA4) to verify whether ~ <0;3V3>, 1kHz sine wave is at the output.
The sine wave will of course be stepwise.

## 3. Test audio chain

To verify basic application without any guitar effect, one can run:

```
cmake --build . --target test_audio_chain-flash
```

The test will use `JeffAppWrapper` and apply `DummyGuitarEffect` onto the signal. It means that the input will be
duplicated at the output. Once again, the input signal can be with or without the DC offset, depending on where is the
input signal applied (see the explanation in the [Audio sampler tests](#1-audio-sampler-tests) section). The output
signal can be observed on the Nucleo board pin A3 (PA4). One can e.g. apply sine wave and change it frequency. The
output signal shall change accordingly.

# 4. Test clock generation for filter cutoff frequency

We use SCF filters for post-DAC and anti-aliasing, thus we need to generate clock for the filters. To generate the clock
only (run the app that starts the clock and no more), run:

```
cmake --build . --target test_filter_cutoff_setter_clock-flash
```

Observe the clock on the A5 Nucleo board pin (PA6).

# 5. Miscellaneous automatic tests

```
ctest -R test_benchmark_timer

# Tests various functions of the serial_logger. See tests/device/test_serial_logger for more details.
ctest -L serial_logger
```

# Host-side tests

The host-side tests verify the logic of the application, test the high level modules, etc. To build and run them:

```
cd build/build_host_release
cmake ../../host # CMAKE_BUILD_TYPE=Release by default
cmake --build .
ctest
```

To build with the debug configuration:

```
cd build/build_host_debug
cmake -DCMAKE_BUILD_TYPE=Debug ../../host
```

# Debugging

`gdb_multiarch` must be installed on the host machine. To debug:

```
cd build/build_device_debug
cmake --build . --target run_gdb_server

# ... from another console ...
cp ../../scripts/.gdbinit .
gdb-multiarch path/to/the/binary
```
