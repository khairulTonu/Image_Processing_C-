#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

float contrastStretching(int r1, int s1, int r2, int s2, int pix)
{
	float resultPix;

	if (pix >= 0 && pix <= r1)
	{
		resultPix = (s1 / r1)*pix;
	}
	else if (pix > r1&&pix <= r2)
	{
		resultPix = (((pix - r1)*(s1 - s2)) / (r1 - r2)) + s1;
	}
	else
	{
		resultPix = (((pix - r2)*(s2 - 255)) / (r2 - 255)) + s2;
	}
	return resultPix;
}

int main()
{
	Mat src = imread("lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat dest = src.clone();
	int r1, s1, r2, s2;
	cout << "Enter r1, s1, r2 ,s2: ";
	cin >> r1 >> s1 >> r2 >> s2;


	for (int y = 0; y < src.cols; y++)
	{
		for (int x = 0; x < src.rows; x++)
		{
			int pix = (int)src.at<uchar>(y, x);
			float replace_pix = contrastStretching(r1, s1, r2, s2, pix);
			dest.at<uchar>(y, x) = (uchar)replace_pix;
		}
	}

	imshow("Input", src);
	imshow("Output", dest);
	waitKey();
}