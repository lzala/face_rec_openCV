/*
 * (C) Copyright 2016
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

//#define DEBUG

class Recognizer {

private:
	cv::CascadeClassifier haar_cascade;
	cv::Ptr<cv::FaceRecognizer> model;
	cv::VideoCapture capture;
	cv::Mat frame;
	cv::Mat gray;
	std::vector< cv::Rect_<int> > faces;
	std::vector<std::string> names;
	std::vector<cv::Mat> images;
	std::vector<int> labels;


public:
	Recognizer(std::string fn_haar, std::string fn_csv, int deviceId);
	void getFrame();
	int detect(void);
	std::string recognize(void);
	friend void readCVS(const std::string& filename, std::vector<cv::Mat>& images,
				std::vector<int>& labels, std::vector<std::string>& names, char separator);

};
