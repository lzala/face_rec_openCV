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

#include "../inc/recognizer.hpp"

void readCVS(const std::string& filename, std::vector<cv::Mat>& images,std::vector<int>& labels,
				std::vector<std::string>& names, char separator = ';')
{
	std::ifstream file(filename.c_str(), std::ifstream::in);
	if (!file) {
		std::string error_message =
			"No valid input file was given, please check the given filename.";
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

Recognizer::Recognizer(std::string fn_haar, std::string fn_csv, int deviceId)
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
	cv::VideoCapture cap(deviceId);
	if(!cap.isOpened()) {
		std::cerr << "Capture Device ID " << deviceId << "cannot be opened." << std::endl;
		return;
	}
	capture = cap;
}

void Recognizer::getFrame(void)
{
	cv::Mat vFrame;
	capture >> vFrame;
	frame = vFrame.clone();
}

int Recognizer::detect(void)
{
	cvtColor(frame, gray, CV_BGR2GRAY);
	// In order to speed up the algorithm only the main (biggest) face is detected
	int flags = CV_HAAR_FIND_BIGGEST_OBJECT;
	haar_cascade.detectMultiScale(gray, faces, 1.1, 3, flags);
	return faces.size();
}

std::string Recognizer::recognize(void)
{
	std::string name("");
	double confidence = 0.0;
	int prediction = -1;
	cv::Mat face = gray(faces[0]);
	model->set("threshold", 50.0);

	model->predict(face, prediction, confidence);
	if (prediction >= 0) {
		name = names.at(prediction).c_str();
		std::cout << "Prediction " << prediction << " Confidence " <<
			confidence << " Name " << name << std::endl;
		cv::rectangle(frame, faces[0], CV_RGB(0, 255,0), 1);
		cv::string box_text = cv::format(" Name = %s ", name.c_str());
		int pos_x = std::max(faces[0].tl().x - 10, 0);
		int pos_y = std::max(faces[0].tl().y - 10, 0);
		cv::putText(frame, box_text, cv::Point(pos_x, pos_y),
			cv::FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
	}
	cv::imshow("face_recognizer", frame);
	char key = (char) cv::waitKey(20);
	return name;
}
