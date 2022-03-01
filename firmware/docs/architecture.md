# Firmware Architecture

## The Audio Chain

The audio chain represents the natural signal flow:

![AudioChain](diagrams/audio_chain.drawio.png)

The basic principle is that there is an `AudioChain` class which takes three interfaces:

* `AudioSampler` - which shall sample the input and provide the samples to the `AudioChain`, which then applies guitar 
effect on the signal by calling the ...
* `GuitarEffect` - interface, which shall apply the guitar effect; the implementer defines what type of effect it is,
distortion, flanger, chorus, etc.
* `AudioDac` - the mutated samples are supplied by batches to this module, which shall then be converted to analog
signal.

This splitting enables design which is simpler to handle. 
`AudioSampler` and `AudioDac` abstract hardware modules, `GuitarEffect` abstracts a guitar effect, which shall be 
testable on the host machine. 

The batch of samples is a templated type, because its type is an implementation detail. Currently, the batches are 
handled through callbacks, but it shall be refactored to make `AudioSampler` and `AudioDac` awaitable.

Samples are organized in batches, because in the end they would have to be organized in batches. Effects operate on 
windows and window is a group (batch) of samples. Moreover, we prevent from frequent context switching. In the case 
the interrupt is raised on each sample, it explodes the number of context switches. This is a typical pattern in DSP,
that samples are grouped into batches.

There is `AudioChainConfig::BatchOfSamples` type defined which defines the type which is currently used as the type
for batch of samples. It defines the batch size as well (the window size). The window size must be a predefined 
config constant, to know in advance the buffer sizes DMA operates on.

The `ThreadedAudioSampler` and `ThreadedAudioDac` implement the hardware-abstracting interfaces. They operate on the
DMA interrupts and they work in similar fashion. DMA operates on the buffer which is twice the size of the window.
Then, the DMA raises interrupts when half of the buffer is filled and when the buffer is fully filled. The DMA fills
the buffer continuously, the buffer is a circular (ring) buffer. The size of the DMA buffer is twice the window size, 
because on each half-buffer update a full window may be retrieved. This allows continuous sampling without pauses,
and without overwriting of the values.

![dma_operation](diagrams/dma_operation.drawio.png)

For the `ThreadedAudioSampler`, on each half-piece update, the freshly updated half-piece is copied from the
DMA buffer, the raw samples are converted according to voltage levels and offset, and such converted samples are 
propagated to the `AudioChain`.

The `ThreadedAudioDac` works in similar way. On each half-piece transfer end, the `AudioChain` is asked for new samples,
which are copied to the DMA buffer (which is obviously not the same DMA buffer used by the `ThreadedAudioSampler`). The 
batch is copied to the DMA buffer in such a way to overwrite the previously transferred half-piece.

# Guitar Effects

## Distortion

To flash the app:

```
cmake --build . --target distortion_app-flash
```

Distortion tests can be found under `test_basic_windowed_distortion_with_memory.cpp`.

Basic distortion operation is that the peaks are sheared. Since we handle samples in batches, the implementation
must be adapted a bit.

The principle of operation is that for each sample, a window is considered:

![windowing](diagrams/windowing.drawio.png)

The computation window is two concatenated batches of samples: the previous batch supplied to the distortion effect
and the current one.

The distortion has a parameter, which is a **Threshold** in range (0;1), which defines the effect's intensity. The lower
the threshold the higher the intensity. The threshold corresponds to the place on the signal peak where the signal
should be sheared.

Then, for each window:

1. Find absolute maximum = **AbsMax**.
2. Get the clipping threshold: **AbsClipThresh = AbsMax * Threshold**.
3. Get the absolute value of the last sample within the window: **AbsS**.
4. If **AbsS** is higher than **AbsClipThresh**, then new value is equal to **AbsClipThresh**, preserving the original 
sign (+ or -) of the sample.
5. Otherwise, leave the value untouched.

Let's consider a simple example, with a window size equal to 4:

![distortion_example](diagrams/distortion_example.drawio.png)

This implementation is naive and we could optimize it a little, by e.g. introducing window steps, to prevent from
finding maximum for each window, but for each 2-step, or 4-step, ... It might be configurable. Other optimization
may be intelligent maximum finding, which would find absolute maximums for non-overlapping ranges. The current
time complexity is `O(n^2)`, where `n` is the window size. This doesn't create problems so far, because the current 
implementation fits into the schedule.

# Creating new Guitar Effects

1. (optional) Simulate the effect's operation using some Python (or other language) scripting. See `hard_clipping` 
examples in the `scripts/` directory. This step should basically at least conclude: "It might work!".
2. Implement and test the new guitar effect on the host machine. Put the implementation to the `src/common/` directory
and the tests under `tests/host/`. See `test_basic_windowed_distortion_with_memory.cpp` as an example of the tests.
3. Implement benchmarks run on the device for the effect. See `test_distortion_benchmark.cpp` as an example. The call
to `apply()` must not take less than 1.2 ms. The 1.2 ms comes from the period of time between supply of each consecutive
batches of samples. Having a window size 64, and 44.1 kHz sampling rate, each batch comes after around 1.45 ms from
the previous one (64 / 44.1 kHz ~= 1.45 ms). We assume some margin for ADC, DAC and context switching.
4. Create a new application under the `device/` directory (similar to `distortion_app`). Make use of `JeffAppWrapper`,
which is a wrapper on top of the `AudioChain`, but it already instantiates the `AudioSampler` and `AudioDac` 
implementers for you.
5. Flash the app and play the guitar!

