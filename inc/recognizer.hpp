#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

template <class T>
class Recognizer
{
private:
	std::Ptr<T> model;
	cv::CascadeClassifier haar_cascade;
	std::vector<cv::Mat> images;
	std::vector<int> labels;
	std::vector<std::string> names;

public:
	Recognizer();
	cv::Mat getVideoFrame(int deviceId);
	std::vector< cv::Rect_<int> > detectFace();
	friend void read_csv(const std::string& filename, std::vector<cv::Mat>& images,
				std::vector<int>& labels, std::vector<string>& names, char separator);

}