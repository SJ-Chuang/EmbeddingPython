#ifndef EMBEDDINGPYTHON_H
#define EMBEDDINGPYTHON_H

#include <iostream>
#include "Python.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace empy{
	class PyModule: public PyObject {
	public:
		PyModule(const char* _name, PyObject* _module) {
			name = string(_name);
			module = _module;
		}
		PyModule getattr(const char* _name) {
			string newname = name + "." + string(_name);
			PyModule attr(newname.c_str(), PyObject_GetAttrString(module, _name));
			return attr;
		}
		PyObject* operator()(PyObject* args) const {
			return PyObject_CallObject(module, args);
		}
	public:
		string name = "PyModule";
		PyObject* module;
	};
	class transforms {
	public:
		PyObject* mat2PyList(Mat imgMat) {
			PyObject* pyList = PyList_New(imgMat.total() * imgMat.channels());
			int idx = 0;
			for (int y = 0; y < imgMat.rows; ++y) {
				for (int x = 0; x < imgMat.cols; ++x) {
					for (int c = 0; c < imgMat.channels(); ++c) {
						PyList_SetItem(pyList, idx, Py_BuildValue("i", (int)imgMat.at<Vec3b>(y, x)[c]));
						idx++;
					}
				}
			}
			return pyList;
		}

		Mat PyList2mat(int height, int width, PyObject* pyList) {
			Mat imgMat(height * width, 3, CV_8U);
			for (int idx = 0; idx < PyList_Size(pyList); ++idx) {
				imgMat.at<UINT8>(idx / 3, idx % 3) = _PyLong_AsInt(PyList_GetItem(pyList, idx));
			}
			Mat outMat = imgMat.reshape(3, height);
			return outMat;
		}
	};
	class PyManager {
	public:
		PyManager() { Py_Initialize(); }
		~PyManager() { Py_Finalize(); }
		PyModule pyimport(const char* name) {
			PyRun_SimpleString("import sys; sys.path.insert(0, '')");
			PyModule m(name, PyImport_Import(PyUnicode_FromString(name)));
			return m;
		}
		void execute(const char* content) {
			PyRun_SimpleString(content);
		}
	};
}

ostream& operator<<(ostream& os, empy::PyModule& m) {
	os << "<PyModule '" << m.name << "'>";
	return os;
}

#endif