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
        f.write(f'const char* {name}_{extension} = {file_as_raw_string};\n')

def embedder_bulk(input_dir, output_dir):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    for input_root, directories, files in os.walk(input_dir):
        output_root = input_root.replace(input_dir, output_dir)
        for directory in directories:
            input_directory = os.path.join(input_root, directory)
            output_directory = os.path.join(output_root, directory)
            if not os.path.exists(output_directory):
                os.makedirs(output_directory)
        for file in files:
            if not os.path.exists(output_root):
                os.makedirs(output_root)
            input_file = os.path.join(input_root, file)
            output_file = os.path.join(output_root, file + '.hpp')
            embedder(input_file, output_file)
