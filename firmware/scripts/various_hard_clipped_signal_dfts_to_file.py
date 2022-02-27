#!/usr/bin/env python3


import itertools
import subprocess


def main():
    sampling_frequency_hz = 100000
    sine_frequencies_hz = [1000, 2000, 4000, 8000, 16000]
    thresholds = [0.8, 0.4, 0.2, 0.1]

    combinations = list(itertools.product(sine_frequencies_hz, thresholds))
    for combination in combinations:
        sine_frequency_hz, threshold = combination
        command = ['./hard_clipped_signal_dft_to_file.py',
                   '--sine_frequency_hz={}'.format(sine_frequency_hz),
                   '--sampling_frequency_hz={}'.format(sampling_frequency_hz),
                   '--threshold={}'.format(threshold)]
        subprocess.run(command)


if __name__ == "__main__":
    main()
