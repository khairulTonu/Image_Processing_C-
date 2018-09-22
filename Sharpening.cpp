#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

int main()
{
	Mat src = imread("Moon.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	Mat mask;

	float sigma;

	int k;

	cout << "Enter Sigma: " << endl;
	cin >> sigma;

	cout << "Enter Sharp factor: " << endl;
	cin >> k;

	

	int maskSize;

	if ((int)sigma % 2 == 0)
		maskSize = 5 * (int)sigma + 1;
	else
		maskSize = 5 * (int)sigma;

	mask.create(maskSize, maskSize, CV_32F);

	int bestPoint = maskSize / 2;

	mask.setTo(0);

	float oneByTwoPiSigmaSquare = 1 / (2.0 * 3.1416*sigma*sigma);

	float temp, gxy;

	float mn = 300.0;

	//floating value mask

	for (int y = 0, Y = -bestPoint; y < maskSize; y++, Y++)
	{
		for (int x = 0, X = -bestPoint; x < maskSize; x++, X++)
		{
			temp = exp(-1 * (X*X + Y*Y) / (2.0 * sigma*sigma));

			gxy = oneByTwoPiSigmaSquare * temp;

			mask.at<float>(y, x) = gxy;

			mn = min(gxy, mn);

			//cout << gxy << endl;
		}
	}

	int gxyInt;

	float sum = 0.0;

	//Int value mask
	for (int y = 0; y < maskSize; y++)
	{
		for (int x = 0; x < maskSize; x++)
		{
			gxyInt = mask.at<float>(y, x) / mn;
			mask.at<float>(y, x) = gxyInt;
			//cout << mask.at<float>(y, x) << " ";
			sum += gxyInt;

		}
		cout << endl;
	}

	//bluring the src image

	copyMakeBorder(src, src, maskSize / 2, maskSize / 2, maskSize / 2, maskSize / 2, 0, 0);

	Mat destGauss = src.clone();

	Mat destTemp = src.clone();

	Mat dest = src.clone();

	for (int y = 0; y < src.rows; y++)
	{

		for (int x = 0; x < src.cols; x++)
		{
			float tmp = 0.0;
			for (int Y = 0; Y < maskSize; Y++)
			{
				for (int X = 0; X < maskSize; X++)
				{
					tmp += (float)src.at<uchar>(y + Y, x + X)*(mask.at<float>(Y, X)*(1 / sum));

				}
			}


			destGauss.at<uchar>(y + bestPoint, x + bestPoint) = (uchar)tmp;

		}

	}

	//gmask(x,y)=src-destGauss

	for (int y = 0; y < src.rows ; y++)
	{

		for (int x = 0; x < src.cols; x++)
		{
			destTemp.at<uchar>(y, x) = src.at<uchar>(y, x) - destGauss.at<uchar>(y, x);

		}

	}


	normalize(destTemp, destTemp, 0, 255, CV_MINMAX);

	//g(x,y) = src + k*destTemp

	for (int y = 0; y < src.rows - maskSize + 1; y++)
	{

		for (int x = 0; x < src.cols - maskSize + 1; x++)
		{
			dest.at<uchar>(y, x) = src.at<uchar>(y, x) + k * destTemp.at<uchar>(y, x);

		}

	}



	imshow("Input", src);

	imshow("Gaussian Output", destGauss);

	imshow("Sharp Output", dest);

	waitKey();

}