# Device tests

To run the device tests serial port must be connected to the device and the device must be flashable using `openocd`.
See [setup](device_setup.md) for more details.

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