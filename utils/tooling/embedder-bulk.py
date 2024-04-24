#!/usr/bin/python

import sys
import os
from embedder import embedder

def main():
    if len(sys.argv) != 3:
        print("Usage: python embedder-bulk.py <input dir> <output dir>")
        return
    
    input_dir = sys.argv[1]
    output_dir = sys.argv[2]
    if output_dir[-1] != '/':
        output_dir += '/'

    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    for input_root, directories, files in os.walk(input_dir):
        output_root = input_root.replace(input_dir, output_dir)
        for directory in directories:
            input_directory = os.path.join(input_root, directory)
            output_directory = os.path.join(output_root, directory)
            if not os.path.exists(output_directory):
                os.makedirs(output_directory)
            print(input_directory, output_directory)
        for file in files:
            if not os.path.exists(output_root):
                os.makedirs(output_root)
            input_file = os.path.join(input_root, file)
            output_file = os.path.join(output_root, file + '.hpp')
            print(input_file, output_file)
            embedder(input_file, output_file)

if __name__ == "__main__":
    main()
