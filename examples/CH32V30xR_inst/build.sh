#!/bin/sh
CMAKE_EXPORT_COMPILE_COMMANDS=1 CMAKE_COLOR_DIAGNOSTICS=1 cmake -S . -B build -G Ninja
cmake --build build
