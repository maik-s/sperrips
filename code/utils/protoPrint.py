#!/usr/bin/python3

import proto.Syscall_pb2
import os
import sys
from enum import Enum
import argparse

parser = argparse.ArgumentParser(description='Parse and pretty print a protobuf binary file.')
parser.add_argument('--recording', dest="is_recording", action='store_true', help='Parse a recording protobuf file.')
parser.add_argument('--replaying', dest="is_recording", action='store_false', help='Parse a replaying protobuf file.')
parser.add_argument('-f', '--file',  dest="file", action="store", type=str, default=None, help="The file to parse.")
args = parser.parse_args()

filepath = None

if args.is_recording:
  filename = "recording.bin"
else:
  filename = "differences.bin"
if args.file == None:
  filepath = os.path.join("/home/ubuntu/diff/home", filename)
else:
  filepath = args.file

path = os.path.dirname(os.path.abspath(__file__))

serialized = None

if args.is_recording:
  serialized = proto.Syscall_pb2.Recording()
else:
  serialized = proto.Syscall_pb2.Differences()

try:
  f = open(filepath, "rb")
  serialized.ParseFromString(f.read())
  print(serialized)
  f.close()
except IOError:
  print (filepath + ": Could not open file.  Creating a new one.")
