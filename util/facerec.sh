#!/bin/bash

opt=0;

function help {
	echo "APP Menu"
	echo "Available options:"
	echo "	-update  - Update CVS file"
	echo "	-run     - Launch the application"
	echo "	-build   - Compile the application"
}

function update {
	./create_cvs.py ../data > data.cvs
}

if [ ! -d "../build" ]; then
	mkdir ../build && cd ../build
	cmake ..
	cd ../util
fi

if [ "$1" == "-update" ] || [ "$2" == "-update" ]; then
	((opt++))
	update
	echo "Update..."
	cat ../data data.cvs
	ls -l data.cvs
fi
if [ "$1" == "-build" ] || [ "$2" == "-build" ]; then
	((opt++))
	echo "Build..."
	make -C ../build
fi
if [ "$1" == "-run" ] || [ "$2" == "-run" ]; then
	((opt++))
	update
	echo "Run..."
	# Redirect strerr output to $var and stdout output to stdout file
	var="$(./../build/facerec_demo haarcascade_frontalface_default.xml data.cvs 0 2>&1 > stdout )"
	if [ ! -z "$var" ]; then
		# Reset webcam usb device - found based on vid:pid
		sudo ./usbreset /dev/bus/usb/001/$(expr "$(lsusb | grep 046d:0825)" : '.*Device\ \([0-9]\+\)')
		sleep 5 # Timeout needed for the webcam usb enumeration after forced reset
		./../build/facerec_demo haarcascade_frontalface_default.xml data.cvs 0
	else
		cat stdout
	fi
	rm stdout
fi
if [ "$1" == "-help" ]; then
	((opt++))
	help
fi
if (($opt != $#)); then
	echo "Invalid arguments! $1 , $2"
	help
fi
