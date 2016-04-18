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


int main(int argc, const char *argv[]) {
	if (argc != 4) {
		cout << "usage: " << argv[0] <<
			" </path/to/haar_cascade> </path/to/csv.ext> </path/to/device id>" << endl;
		cout << "\t </path/to/haar_cascade> - Path to the Haar Cascade for face detection." << endl;
		cout << "\t </path/to/data.cvs> - Path to the CSV file with the face database." << endl;
		cout << "\t <device id> - The webcam device id to grab frames from." << endl;
		exit(1);
	}

	string fn_haar = string(argv[1]);
	string fn_csv = string(argv[2]);
	int deviceId = atoi(argv[3]);

	Recognizer face(fn_haar, fn_csv, deviceId);

	while(1) {
		face.getFrame();
		if(face.detect()) {
			cout << face.recognize().c_str() << endl;
		}
	}

	return 0;
}
