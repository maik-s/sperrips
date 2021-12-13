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
  filepath = os.path.join("/root/deb_diff/root/", filename)
else:
  filepath = args.file

path = os.path.dirname(os.path.abspath(__file__))


args.is_recording = filepath.endswith("recording.bin") # automatically detect rec/repl phase

serialized = None

if args.is_recording:
  serialized = proto.Syscall_pb2.Recording()
else:
  serialized = proto.Syscall_pb2.Differences()

try:
  f = open(filepath, "rb")
  serialized.ParseFromString(f.read())
  f.close()

  mmap = serialized.syscall[5]
  mmap.return_value.ptr = 0xdeadbeef

  f2 = open(filepath, "wb+")
  f2.write(serialized.SerializeToString())
  f2.close()
except IOError as e:
  print (filepath + ": Could not open file.  Creating a new one." + e)
