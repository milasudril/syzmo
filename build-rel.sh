#!/bin/bash

echo "Creating makefile for remote target"
wand 'platforms[target[x86;Windows]] profile[release]'
chmod o+rw Makefile-server-rel
echo "Building local target"
wand 'profile[release]'
