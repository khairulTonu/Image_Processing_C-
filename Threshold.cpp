#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

void thresholdReturn(Mat &mat, Mat src, float miu, bool invert) {
	int max = 255, min = 0;
	if (invert) {
		max = 0;
		min = 255;
	}
	int s, r;
	for (int y = 0; y<src.rows; y++) {
		for (int x = 0; x<src.cols; x++) {
			s = src.at<uchar>(y, x);
			if (s <= miu && s >= 0) {
				r = min;
			}
			else {
				r = max;
			}
			mat.at<uchar>(y, x) = r;
		}
	}
}

int main()
{
	float Tp = 100000, Tc = 0, val, miu1, miu2;
	cout << "enter cut-up value : ";
	cin >> val;

	Mat I = imread("lena.jpg", 0);
	Mat mask = I.clone();
	mask.setTo(255);
	Scalar meanSc;

	imshow("Input Image", I);
	cvWaitKey(0);

	meanStdDev(I, meanSc, mask);
	Tc = meanSc[0];

	cout << Tc << endl;

	while (abs(Tp - Tc)>val) {
		Tp = Tc;
		mask = I.clone();
		thresholdReturn(mask, I, Tp, false);
		meanStdDev(I, mask, meanSc, mask);
		miu1 = meanSc[0];

		mask = I.clone();
		thresholdReturn(mask, I, Tp, true);
		meanStdDev(I, meanSc, mask);
		miu2 = meanSc[0];

		Tc = (miu1 + miu2) / 2;

		cout << Tc << endl;
	}



	thresholdReturn(I, I, Tc, false);

	imshow("Output Image", I);
	cvWaitKey(0);

	cvDestroyAllWindows();
}