#!/bin/bash

# Generate timestamp
echo `date` > app/src/main/res/raw/timestamp.raw

# Build APK
./gradlew assemble
