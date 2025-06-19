#!/bin/bash

set -xe
gcc \
    -Wall -Wextra -pedantic \
    -o main \
    src/*.c src/*/*.c \
    -I./include -I./deps/include/ \
    -lraylib -lm \
    -Wno-all deps/include/raygui.h

./main

