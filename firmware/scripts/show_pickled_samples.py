#!/usr/bin/env python3
import click
import pickle
from matplotlib import pyplot as plt


@click.command()
@click.argument('pickled_samples_file', type=click.File('rb'))
def show_pickled_plot(pickled_samples_file):
    """Shows pickled samples with matplotlib.

    The path to the serialized samples shall be found under path
    PICKLED_PLOT_FILE.

    """

    samples = pickle.load(pickled_samples_file)
    plt.plot(samples)
    plt.title(pickled_samples_file.name)
    plt.show()


if __name__ == "__main__":
    show_pickled_plot()
