#!/usr/bin/env python3

import numpy as np
from scipy.fft import fft, fftfreq
from matplotlib import pyplot as plt
import click

from hard_clipping import hard_clip_symmetrical


@click.command()
@click.option('--threshold',
              type=float,
              default=0.5,
              help='Relative clipping threshold in range (0;1).')
@click.option('--number_of_periods',
              type=int,
              default=5,
              help='Number of periods of the sine wave being clipped.')
@click.option('--sine_frequency_hz',
              type=int,
              required=True,
              help='Frequency of the sine wave being clipped, in Herz.')
@click.option('--sampling_frequency_hz',
              type=int,
              default=44100,
              help='Sampling frequency in Herz.')
def hard_clipped_signal_dft_to_file(threshold,
                                    number_of_periods,
                                    sine_frequency_hz,
                                    sampling_frequency_hz):
    """Saves DFT of hard clipped sine wave to file.

    The script will apply hard clipping with specified threshold,
    calculate FFT after applying the clipping and will store the
    FFT plot to a file.

    \b
    The output file name is: hard_clipped_sine_<sine frequency>_\
<sampling_frequency>_<threshold>_<number of periods of the sine>.png

    """

    sine = make_sine(number_of_periods, sine_frequency_hz,
                     sampling_frequency_hz)
    clipped_sine = hard_clip_symmetrical(sine, threshold)
    filename = 'hard_clipped_sine_dft_{}_{}_{}_{}.png'.format(
        sine_frequency_hz, sampling_frequency_hz, threshold, number_of_periods)
    plot_fft_to_file(clipped_sine, sampling_frequency_hz, filename)


def make_sine(number_of_periods: int,
              sine_frequency: int,
              sampling_frequency: int):
    samples_per_period = int(round(sampling_frequency / sine_frequency))
    number_of_samples = samples_per_period * number_of_periods
    x = np.arange(number_of_samples)
    y = np.sin(2 * np.pi * sine_frequency / sampling_frequency * x)
    return y


def plot(signal):
    plt.plot(signal)
    plt.show()


def plot_fft_to_file(time_domain_signal, sampling_frequency_hz, filename):
    xf = fftfreq(len(time_domain_signal), 1 / sampling_frequency_hz)
    yf = np.abs(fft(time_domain_signal))
    plt.plot(xf, yf)
    plt.savefig(filename)


if __name__ == "__main__":
    hard_clipped_signal_dft_to_file()
