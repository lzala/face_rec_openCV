/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels,
				vector<string>& names, char separator = ';') {
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, classlabel, name, prev_name = "";
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel, separator);
		getline(liness, name);
		if(!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
			if (name != prev_name) {
				names.push_back(name);
				prev_name = name;
			}
		}
	}
}

int main(int argc, const char *argv[]) {
	if (argc != 4) {
		cout << "usage: " << argv[0] <<
			" </path/to/haar_cascade> </path/to/csv.ext> </path/to/device id>" << endl;
		cout << "\t </path/to/haar_cascade> - Path to the Haar Cascade for face detection." << endl;
		cout << "\t </path/to/csv.ext> - Path to the CSV file with the face database." << endl;
		cout << "\t <device id> - The webcam device id to grab frames from." << endl;
		exit(1);
	}

	string fn_haar = string(argv[1]);
	string fn_csv = string(argv[2]);
	int deviceId = atoi(argv[3]);

	vector<Mat> images;
	vector<int> labels;
	vector<string> names;

	try {
		read_csv(fn_csv, images, labels, names);
	} catch (cv::Exception& e) {
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		exit(1);
	}

	// Create a FaceRecognizer and train it on the given images:
	Ptr<FaceRecognizer> model = createLBPHFaceRecognizer();
	model->train(images, labels);
	CascadeClassifier haar_cascade;
	haar_cascade.load(fn_haar);

	// Get a handle to the Video device:
	VideoCapture cap(deviceId);
	// Check if we can use this device at all:
	if(!cap.isOpened()) {
		cerr << "Capture Device ID " << deviceId << "cannot be opened." << endl;
		return -1;
	}

	Mat video_frame;
	for(;;) {
		cap >> video_frame;
		// Clone the current video_frame:
		Mat original = video_frame.clone();
		// Convert the current video_frame to grayscale:
		Mat gray;
		cvtColor(original, gray, CV_BGR2GRAY);
		// Find the faces in the video_frame
		vector< Rect_<int> > faces;
		// In order to speed up the algorithm only the main (biggest) face is detected
		int flags = CV_HAAR_FIND_BIGGEST_OBJECT;
		haar_cascade.detectMultiScale(gray, faces, 1.1, 3, flags);
		if (faces.size()) {
			Rect face_i = faces[0];
			Mat face = gray(face_i);
			model->set("threshold", 50.0);
			double predicted_confidence = 0.0;
			int prediction = -1;
			model->predict(face, prediction, predicted_confidence);
			if (prediction >= 0) {
				cout << "Prediction " << prediction << " Confidence " <<
					predicted_confidence << " Name " << names.at(prediction).c_str() << endl;
				rectangle(original, face_i, CV_RGB(0, 255,0), 1);
				// Create the text we will annotate the box with:
				string box_text = format("Prediction = %d Name = %s ", prediction, names.at(prediction).c_str());
				// Calculate the position for annotated text (make sure we don't
				// put illegal values in there):
				int pos_x = std::max(face_i.tl().x - 10, 0);
				int pos_y = std::max(face_i.tl().y - 10, 0);
				// And now put it into the image:
				putText(original, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
			}
		}
		// Show the result:
		imshow("face_recognizer", original);
		// And display it:
		char key = (char) waitKey(20);
		// Exit this loop on escape:
		if(key == 27)
			break;
		}
	return 0;
}
