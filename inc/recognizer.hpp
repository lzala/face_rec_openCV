#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"


template <class T>
class Recognizer
{
private:
	Ptr<T> model;
	cv::CascadeClassifier haar_cascade;
	vector<cv::Mat> images;
	vector<int> labels;
	vector<string> names;

public:
	Recognizer();
	cv::Mat getVideoFrame(int deviceId);
	vector< Rect_<int> > detectFace();
	friend void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels,
				vector<string>& names, char separator);

}