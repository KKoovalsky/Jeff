import click
import fileinput


@click.command()
@click.argument('source_file', type=click.Path(exists=True))
def main(source_file):
    remove_dummy_header_inclusion(source_file)


def remove_dummy_header_inclusion(source_file):
    dummy_header_include = '#include "dummy_header.hpp"'
    print('Removing line: "{}" in file: {}'.format(dummy_header_include,
                                                   source_file))
    for line in fileinput.input([source_file], inplace=True):
        print(line.replace(dummy_header_include, ''), end='')


if __name__ == '__main__':
    main()
