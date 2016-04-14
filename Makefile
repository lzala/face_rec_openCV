all:
	g++ -ggdb `pkg-config --cflags opencv` -o `basename facerec_demo.cpp .cpp` facerec_demo.cpp `pkg-config --libs opencv`

clean:
	rm facerec_demo

