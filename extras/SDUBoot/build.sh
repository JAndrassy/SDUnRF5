#!/bin/sh -x

ARDUINO=arduino
SKETCH_NAME="SDUBoot.ino"
SKETCH="$PWD/$SKETCH_NAME"
BUILD_PATH="$PWD/build"
OUTPUT_PATH="../../src/boot"

if [[ "$OSTYPE" == "darwin"* ]]; then
	ARDUINO="/Applications/Arduino.app/Contents/MacOS/Arduino"
fi

buildSDUBootSketch() {
	BOARD=$1
	DESTINATION=$2

	$ARDUINO --verify --board $BOARD --preserve-temp-files --pref build.path="$BUILD_PATH" --pref build.float_flags=-fno-delete-null-pointer-checks $SKETCH
	cat "$BUILD_PATH/$SKETCH_NAME.bin" | xxd -i > $DESTINATION
	rm -rf "$BUILD_PATH"
}

mkdir -p "$OUTPUT_PATH"

buildSDUBootSketch "sandeepmistry:nRF5:SeeedArchLink" "$OUTPUT_PATH/archlink.h"
buildSDUBootSketch "sandeepmistry:nRF5:SeeedArchLink:softdevice=s110" "$OUTPUT_PATH/archlink_s110.h"
buildSDUBootSketch "sandeepmistry:nRF5:SeeedArchLink:softdevice=s130" "$OUTPUT_PATH/archlink_s130.h"
