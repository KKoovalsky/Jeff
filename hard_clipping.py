#!/usr/bin/env python3
import wave
import struct
import pickle


def main():
    in_file_base = 'C_power_chord_4_strokes_mono'
    in_file = '{}.wav'.format(in_file_base)

    in_samples = read_samples_from_mono_wav(in_file)

    threshold = 0.6
    window_length = 64
    out_samples = hard_clip_symmetrical_with_moving_window(
        in_samples, threshold, window_length)

    out_samples = normalize(out_samples)

    out_file_base = '{}_hard_clip_windowed_{}_thresh_{}'.format(
        in_file_base, window_length, threshold)
    out_file = '{}.wav'.format(out_file_base)
    write_samples_to_mono_wav(out_file, out_samples)
    samples_to_file(out_file_base, out_samples)


def read_samples_from_mono_wav(path: str) -> list:
    in_signal_mono = wave.open(path)
    num_samples = in_signal_mono.getnframes()
    in_samples_raw = in_signal_mono.readframes(num_samples)
    return list(struct.unpack('<{}h'.format(num_samples), in_samples_raw))


def hard_clip_symmetrical_with_moving_window(
        samples: list, threshold: float, window_length: int) -> list:
    num_samples = len(samples)
    out_samples = list()
    for i in range(num_samples):
        window_begin = max(i - window_length + 1, 0)
        window_end = i + 1
        window = samples[window_begin:window_end]
        intermediate_out_samples = hard_clip_symmetrical(window, threshold)
        out_samples.append(int(round(intermediate_out_samples[-1])))
    return out_samples


def hard_clip_symmetrical(samples: list, threshold: float):
    peak = absolute_maximum(samples)
    maximum = peak * threshold
    minimum = -maximum
    return [clamp(v, maximum, minimum) for v in samples]


def clamp(value, max_value, min_value):
    if value > max_value:
        return max_value
    elif value < min_value:
        return min_value
    else:
        return value


def write_samples_to_mono_wav(path: str, samples: list) -> None:
    num_samples = len(samples)
    samples_raw = struct.pack('<{}h'.format(num_samples), *samples)

    with wave.open(path, 'w') as out_wave:
        out_wave.setnchannels(1)
        out_wave.setsampwidth(2)
        out_wave.setframerate(44100)
        out_wave.writeframes(samples_raw)


def samples_to_file(path_without_extension: str, samples: list) -> None:
    path = '{}.raw.pickle'.format(path_without_extension)
    with open(path, 'wb') as f:
        pickle.dump(samples, f)


def normalize(samples: list) -> list:
    peak = absolute_maximum(samples)
    sample_max = 32767.0
    normalization_factor = sample_max / peak
    result = [int(round(v * normalization_factor)) for v in samples]
    return result


def absolute_maximum(samples: list) -> int:
    peak_positive = max(samples)
    peak_negative = min(samples)
    return max(peak_positive, abs(peak_negative))


if __name__ == "__main__":
    main()
