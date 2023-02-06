/*
usage:
#include<opencv.hpp>
#include"ptrRotate.h"
using namespace cv;
#define PI 3.14159265
int main(int argc, char** argv) {
	Mat img = imread(argv[1],0);
	Mat ret(img.rows, img.cols, CV_8UC1);
	int i = 0;
	while (true) {
		float theta = float(i) * PI / 180.0;
		ptrRotate<uchar>(img.data, img.rows, img.cols, theta, ret.data);
		i = (i + 5) % 360;
		imshow("ret", ret);
		waitKey(50);
	}
	return 0;
}
*/

#pragma once
#include<cmath>
inline void invRotate(float costh,float sinth,float cx,float cy,
	float X,float Y,float &x,float &y);
template<typename S>
void ptrRotate(S* ig,int rows,int cols,float theta,S* ret);
template<typename S>
void ptrRotate(S* ig, int rows, int cols,
	int cx,int cy,float theta,S* ret) {
	int n = rows * cols;
	float costh = cos(theta);
	float sinth = sin(theta);
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			float ix, iy;
			invRotate(costh, sinth, cx, cy, x, y, ix, iy);
			if (ix>=0 && iy>=0 && ix<=cols-1 && iy<=rows-1) {
				float ixInt = int(ix);
				float ixFrc = ix - ixInt;
				float iyInt = int(iy);
				float iyFrc = iy - iyInt;
				if (iyFrc <= 0.0) {
					if (ixFrc <= 0.0) {
						ret[y*cols+x] = ig[int(iyInt*cols+ixInt)];
					}else {
						float v = (1.0 - ixFrc) * float(ig[int(iyInt * cols + ixInt)]) +
							ixFrc * float(ig[int(iyInt * cols + ixInt+1)]);
						ret[y * cols + x] = S(v);
					}
				}else {
					if (ixFrc <= 0.0) {
						float v = (1.0 - iyFrc) * float(ig[int(iyInt * cols + ixInt)]) +
							iyFrc * float(ig[int((iyInt+1) * cols + ixInt)]);
						ret[y * cols + x] = S(v);
					}
					else {
						float v = (1.0 - ixFrc) * (1.0 - iyFrc) * float(ig[int(iyInt * cols + ixInt)]) +
							(1.0 - ixFrc) * iyFrc * float(ig[int((iyInt + 1) * cols + ixInt)]) +
							ixFrc * (1.0 - iyFrc) * float(ig[int(iyInt * cols + ixInt + 1)]) +
							ixFrc * iyFrc * float(ig[int((iyInt + 1) * cols + ixInt + 1)]);
						ret[y * cols + x] = S(v);
					}
				}
			}else {
				ret[y * cols + x] = 0;
			}
		}
	}
}

void invRotate(float costh, float sinth, float cx, float cy,
	float X, float Y, float& x, float& y) {
	x = costh * (X - cx) + sinth * (Y - cy) + cx;
	y = -sinth * (X - cx) + costh * (Y - cy) + cy;
}
template<typename S>
void ptrRotate(S* ig, int rows, int cols, float theta, S* ret) {
	ptrRotate<S>(ig, rows, cols, cols / 2, rows / 2, theta, ret);
}