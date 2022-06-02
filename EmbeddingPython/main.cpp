#include <iostream>
#include "Python.h"
#include <opencv2/opencv.hpp>
#include "EmbeddingPython.h"

using namespace std;
using namespace cv;
using namespace empy;

int main(int argc, char* argv[]) {
	// Create a new PyManager
	PyManager* pm = new PyManager();

	// Execute python with string
	pm->execute("print('Hello World from Embedded Python!')");

	// Import the built-in module
	PyModule math = pm->pyimport("math");
	PyObject* val = Py_BuildValue("(i)", 36);
	PyObject* root = math.getattr("sqrt")(val);
	printf("The square root of 36 is %i.\n", _PyLong_AsInt(root));

	// Import the custom module (Place func.py in the same folder as main.cpp)
	PyModule func = pm->pyimport("func");
	PyObject* vals = Py_BuildValue("(i, i)", 4, 5);
	PyObject* dot_result = func.getattr("dot")(vals);
	printf("4 x 5 = %i\n", _PyLong_AsInt(dot_result));

	// Gaussian blur an image with custom module
	transforms T;
	Mat img = imread("image.jpg");
	PyObject* imgList = T.mat2PyList(img); // Transforms a Mat to a python list
	PyObject* imgInfo = Py_BuildValue("(i, i, O)", img.rows, img.cols, imgList);
	PyObject* blurList = func.getattr("gaussianblur")(imgInfo); // Blur an image
	Mat blur = T.PyList2mat(img.rows, img.cols, blurList); // Transforms a python list to a Mat
	imwrite("blur.jpg", blur);
	
	// Delete the PyManager
	delete pm;

	return 0;
}