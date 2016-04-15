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
class Recognizer {

private:
	cv::Ptr<cv::FaceRecognizer> model;
	cv::CascadeClassifier haar_cascade;
	cv::Mat frame;
	cv::Mat gray;
	cv::VideoCapture capture;
	std::vector< cv::Rect_<int> > faces;
	std::vector<cv::Mat> images;
	std::vector<int> labels;
	std::vector<std::string> names;

public:
	Recognizer(std::string fn_haar, std::string fn_csv, int deviceId);
	void getFrame();
	void detect(void);
	std::string recognize(void);
	friend void readCVS(const std::string& filename, std::vector<cv::Mat>& images,
				std::vector<int>& labels, std::vector<std::string>& names, char separator);

};

static void readCVS(const std::string& filename, std::vector<cv::Mat>& images,std::vector<int>& labels,
				std::vector<std::string>& names, char separator = ';')
{
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file) {
		std::string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	std::string line, path, classlabel, name, prev_name = "";
	while (getline(file, line)) {
		std::stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel, separator);
		getline(liness, name);
		if(!path.empty() && !classlabel.empty()) {
			images.push_back(cv::imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
			if (name != prev_name) {
				names.push_back(name);
				prev_name = name;
			}
		}
	}
}

template <class T>
Recognizer<T>::Recognizer(std::string fn_haar, std::string fn_csv, int deviceId)
{
	try {
		readCVS(fn_csv, images, labels, names);
	} catch (cv::Exception& e) {
		std::cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << std::endl;
		exit(1);
	}
	model = cv::createLBPHFaceRecognizer();
	model->train(images, labels);
	haar_cascade.load(fn_haar);
	// Get a handle to the Video device:
	cv::VideoCapture cap(deviceId);
	// Check if we can use this device at all:
	if(!cap.isOpened()) {
		std::cerr << "Capture Device ID " << deviceId << "cannot be opened." << std::endl;
		return;
	}
	capture = cap;
}

template <class T>
void Recognizer<T>::getFrame(void)
{
	cv::Mat vFrame;
	capture >> vFrame;
	frame = vFrame.clone();
}

template <class T>
void Recognizer<T>::detect(void)
{
	cvtColor(frame, gray, CV_BGR2GRAY);
	// In order to speed up the algorithm only the main (biggest) face is detected
	int flags = CV_HAAR_FIND_BIGGEST_OBJECT;
	haar_cascade.detectMultiScale(gray, faces, 1.1, 3, flags);
}

template <class T>
std::string Recognizer<T>::recognize(void)
{
	std::string name("");
	if (faces.size()) {
		cv::Mat face = gray(faces[0]);
		model->set("threshold", 150.0);
		double confidence = 0.0;
		int prediction = -1;
		model->predict(face, prediction, confidence);
		if (prediction >= 0) {
			name = names.at(prediction).c_str();
			std::cout << "Prediction " << prediction << " Confidence " <<
				confidence << " Name " << name << std::endl;
			cv::rectangle(frame, faces[0], CV_RGB(0, 255,0), 1);
			// Create the text we will annotate the box with:
			cv::string box_text = cv::format(" Name = %s ",  names.at(prediction).c_str());
			// Calculate the position for annotated text (make sure we don't
			// put illegal values in there):
			int pos_x = std::max(faces[0].tl().x - 10, 0);
			int pos_y = std::max(faces[0].tl().y - 10, 0);
			// And now put it into the image:
			cv::putText(frame, box_text, cv::Point(pos_x, pos_y), cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
		}
	}
	// Show the result:
	cv::imshow("face_recognizer", frame);
	// And display it:
	char key = (char) cv::waitKey(20);
	return name;
}
