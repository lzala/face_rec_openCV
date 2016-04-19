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

using namespace std;
const int NO_SAMPLES = 30;

int main(int argc, const char *argv[]) {
	if (argc != 4) {
		cout << "usage: " << argv[0] <<
			" </path/to/haar_cascade> </path/to/data.cvs> </path/to/device id>" << endl;
		cout << "\t </path/to/haar_cascade> - Path to the Haar Cascade for face detection." << endl;
		cout << "\t </path/to/data.cvs> - Path to the CSV file with the face database." << endl;
		cout << "\t <device id> - The webcam device id to grab frames from." << endl;
		exit(1);
	}

	string pathHaar = string(argv[1]);
	string pathCSV = string(argv[2]);
	int deviceId = atoi(argv[3]);

	Recognizer face(pathHaar, pathCSV, deviceId);

	int samples = NO_SAMPLES;
	vector<string> subjects;
	while (samples--) {
		face.getFrame();
		if(face.detect()) {
			subjects.push_back(face.recognize().c_str());
		}
	}
	if (subjects.size()) {
		sort(subjects.begin(), subjects.end());
#ifdef DEBUG
		for (int i = 0; i < subjects.size(); i++) {
			cout << subjects.at(i).c_str() << endl;
		}
#endif
		vector<int> idsCount;
		vector<string> idsString;
		int walk = 0, i = 0;
		while (walk < subjects.size()) {
			idsCount.push_back(count(subjects.begin() + walk, subjects.end(),
						subjects.at(walk).c_str()));
			idsString.push_back(subjects.at(walk).c_str());
			walk += idsCount.at(i);
			i++;
		}
		int max = 0, index = 0;
#ifdef DEBUG
		for (int i = 0; i < idsCount.size(); i++)
			cout << "Found: " << idsCount.at(i) << " " << idsString.at(i).c_str() << endl;
#endif
		max  = *max_element(idsCount.begin(), idsCount.end());
		index = distance(idsCount.begin(), max_element(idsCount.begin(), idsCount.end()));
		cout << idsString.at(index).c_str() << " " << (100 * max) / subjects.size() << "%" << endl;
	}
	return 0;
}
