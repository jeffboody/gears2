#!/bin/bash

# common options
# V=1 for verbose
# -B to build clean

cd project
$ANDROID_NDK/ndk-build $@
