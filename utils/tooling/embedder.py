#!/usr/bin/python

import sys
import os

def embedder(input_file, output_file):
    name, extension = os.path.splitext(os.path.basename(input_file))
    if extension[0] == '.':
        extension = extension[1:]

    with open(input_file, 'r') as f:
        text_file = f.read()

    file_as_raw_string = f"R\"{extension}(\n{text_file}\n){extension}\""

    with open(output_file, 'w') as f:
        f.write('#pragma once\n\n')
        f.write('const char* ' + name + ' = ' + file_as_raw_string + ';\n')


def main():
    if len(sys.argv) != 3:
        print("Usage: python embedder.py <input file> <output.hpp>")
        return

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    embedder(input_file, output_file)

if __name__ == "__main__":
    main()
