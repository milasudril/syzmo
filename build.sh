#!/bin/bash

echo "Creating makefile for remote target"
wand 'platforms[target[x86;Windows]]'
chmod o+rw Makefile-server-dbg
echo "Building local target"
wand

