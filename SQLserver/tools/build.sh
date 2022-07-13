#!/usr/bin/env bash

mkdir build
cd build
cmake ../
make SQLserver
cp src/SQLserver ../SQLserver
