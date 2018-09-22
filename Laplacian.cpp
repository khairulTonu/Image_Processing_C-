#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

int main()
{
	Mat src = imread("Moon.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat dest;
	Mat sharp = src.clone();
	Mat destTemp = src.clone();

	float laplacian[9] = { 0,-1,0,
						  -1,5,-1,
						  0,-1,0 };

	Mat kernel(3, 3, CV_32F, laplacian);

	filter2D(src, dest, CV_32F, kernel, Point(-1, -1));

	normalize(dest, dest, 0, 255, CV_MINMAX,CV_8U);

	//addWeighted(src, 0.5, dest, 0.5, 0, sharp);
/*
	for (int y = 0; y < dest.rows; y++)
	{
		for (int x = 0; x < dest.cols; x++)
		{
			sharp.at<uchar>(y, x) = src.at<uchar>(y, x) + 0.5 * dest.at<uchar>(y, x);
		}
	}
	*/

	normalize(sharp, sharp, 0, 255, CV_MINMAX, CV_8U);


	imshow("Input", src);
	imshow("Laplacian Output", dest);
	imshow("Laplacian Sharpenned Output", sharp);
	waitKey();


}