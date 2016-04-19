#####################################
cmake_minimum_required (VERSION 2.8)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/cmake/)
project (facerec_demo)
find_package(OpenCV)
IF (OpenCV_FOUND)
	MESSAGE(STATUS "COMPILING")
	add_executable (facerec_demo
		./src/facerec_demo.cpp
		./src/recognizer.cpp
		./inc/recognizer.hpp
	)
	target_link_libraries (facerec_demo ${OpenCV_LIBS})
ELSE()
	MESSAGE(FATAL_ERROR "OPENCV NOT FOUND")
ENDIF()
