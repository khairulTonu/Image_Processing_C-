#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

int main()
{
	Mat src = imread("lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat dest = src.clone();
	Mat destTemp = src.clone();

	float gamma;

	cout << "Enter gamma: " << endl;

	cin >> gamma;

	float smax = pow(255, gamma);

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			float r = (float)src.at<uchar>(y, x);
			float s = pow(r, gamma);

			s = (s / smax) * 255;

			dest.at<uchar>(y, x) = (uchar)s;



		}
	}

	//normalize(dest, destTemp, 0, 255, CV_MINMAX);

	imshow("Input", src);
	imshow("Output", dest);
	waitKey();
}