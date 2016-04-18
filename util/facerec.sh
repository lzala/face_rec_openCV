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
	./../build/facerec_demo haarcascade_frontalface_default.xml data.cvs 0
fi
if [ "$1" == "-help" ]; then
	((opt++))
	help
fi
if (($opt != $#)); then
	echo "Invalid arguments! $1 , $2"
	help
fi
