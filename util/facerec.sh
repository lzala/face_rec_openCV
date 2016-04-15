#!/bin/bash

function help {
	echo "APP Menu"
	echo "Available options:"
	echo "	-update  - Update CVS file"
	echo "	-run     - Launch the application"
	echo "	-build   - Compile the application"
}

opt=0;

function update {
	./create_cvs.py ../data > data.txt
}

if [ "$1" == "-update" ] || [ "$2" == "-update" ]; then
	((opt++))
	update
	echo "Update..."
	cat ../data data.txt
	ls -l data.txt
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
	./../build/facerec_demo haarcascade_frontalface_default.xml data.txt 0
fi
if [ "$1" == "-help" ]; then
	((opt++))
	help
fi
if (($opt != $#)); then
	echo "Invalid arguments! $1 , $2"
	help
fi
