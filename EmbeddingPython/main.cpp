#include <iostream>
#include "Python.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

PyObject* mat2PyList(Mat imgMat) {
	PyObject* imgList = PyList_New(imgMat.total() * imgMat.channels());
	int idx = 0;
	for (int y = 0; y < imgMat.rows; ++y) {
		for (int x = 0; x < imgMat.cols; ++x) {
			for (int c = 0; c < imgMat.channels(); ++c) {
				PyList_SetItem(imgList, idx, Py_BuildValue("i", (int)imgMat.at<Vec3b>(y, x)[c]));
				idx++;
			}
		}
	}
	return imgList;
}

Mat PyList2mat(int height, int width, PyObject* imgList) {
	Mat imgMat(height * width, 3, CV_8U);
	for (int idx = 0; idx < PyList_Size(imgList); ++idx) {
		imgMat.at<UINT8>(idx / 3, idx % 3) = _PyLong_AsInt(PyList_GetItem(imgList, idx));
	}
	Mat outMat = imgMat.reshape(3, height);
	return outMat;
}

int main(int argc, char* argv[]) {
	Py_Initialize();
	
	PyRun_SimpleString("print('Hello World from Embedded Python!')");
	
	int a = 36;
	PyObject* math = PyImport_Import(PyUnicode_FromString("math"));
	PyObject* sqrt = PyObject_GetAttrString(math, "sqrt");
	PyObject* val = PyObject_CallObject(sqrt, Py_BuildValue("(i)", a));
	cout << "The square root of " << a << " is " << _PyLong_AsInt(val) << endl;
	
	int b = 5;
	int c = 4;
	PyRun_SimpleString("import sys; sys.path.insert(0, '')");
	PyObject* func = PyImport_Import(PyUnicode_FromString("func"));
	PyObject* dot = PyObject_GetAttrString(func, "dot");
	PyObject* result = PyObject_CallObject(dot, Py_BuildValue("(i, i)", b, c));
	cout << b << " x " << c << " = " << _PyLong_AsInt(result) << endl;
		
	Mat img = imread("image.jpg");
	if (img.empty()) {
		cout << "img is empty" << endl;
		return 0;
	}
		
	PyObject* imgList = mat2PyList(img);
	PyObject* gaussianblur = PyObject_GetAttrString(func, "gaussianblur");
	PyObject* blur = PyObject_CallObject(gaussianblur, Py_BuildValue("(i, i, O)", img.rows, img.cols, imgList));
	Mat blur_img = PyList2mat(img.rows, img.cols, blur);
	imwrite("blur.jpg", blur_img);
	
	Py_Finalize();
	return 0;
}