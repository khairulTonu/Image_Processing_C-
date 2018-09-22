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

	Mat mag = src.clone();

	Mat xmask,ymask;

	float sigma;

	cout << "Enter Sigma: " << endl;

	cin >> sigma;

	int maskSize;

	if ((int)sigma % 2 == 0)
		maskSize = 5 * (int)sigma + 1;
	else
		maskSize = 5 * (int)sigma;

	xmask.create(maskSize, maskSize, CV_32F);
	ymask.create(maskSize, maskSize, CV_32F);

	int bestPoint = maskSize / 2;

	xmask.setTo(0);
	ymask.setTo(0);

	//float oneByTwoPiSigmaSquare = 1 / (2.0 * 3.1416*sigma*sigma);

	float temp, gxy;

	float mnX = 300.0;
	float mnY = 300.0;

	//floating value mask

	for (int y = 0, Y = -bestPoint; y < maskSize; y++, Y++)
	{
		for (int x = 0, X = -bestPoint; x < maskSize; x++, X++)
		{
			temp = exp(-1 * (X*X + Y*Y) / (2.0 * sigma*sigma));

			gxy = -1 * X * temp;

			xmask.at<float>(y, x) = gxy;

			mnX = min(gxy, mnX);

			//cout << gxy << " ";
		}
		cout << endl;
	}

	for (int y = 0, Y = -bestPoint; y < maskSize; y++, Y++)
	{
		for (int x = 0, X = -bestPoint; x < maskSize; x++, X++)
		{
			temp = exp(-1 * (X*X + Y*Y) / (2.0 * sigma*sigma));

			gxy = -1 * Y * temp;

			ymask.at<float>(y, x) = gxy;

			mnY = min(gxy, mnY);

			//cout << gxy << " ";
		}
		cout << endl;
	}

	int gxyInt;

	float sum = 0.0;

	//Int value mask
	for (int y = 0; y < maskSize; y++)
	{
		for (int x = 0; x < maskSize; x++)
		{
			gxyInt = xmask.at<float>(y, x) / mnX;
			xmask.at<float>(y, x) = gxyInt;
			//cout << mask.at<float>(y, x) << " ";
			sum += gxyInt;

		}
		cout << endl;
	}

	for (int y = 0; y < maskSize; y++)
	{
		for (int x = 0; x < maskSize; x++)
		{
			gxyInt = ymask.at<float>(y, x) / mnY;
			ymask.at<float>(y, x) = gxyInt;
			//cout << mask.at<float>(y, x) << " ";
			sum += gxyInt;

		}
		cout << endl;
	}

	Mat outx,outy;

	

	//bluring the src image

	copyMakeBorder(dest, dest, maskSize / 2, maskSize / 2, maskSize / 2, maskSize / 2, 0, 0);
	filter2D(src, outx, CV_32FC1, xmask, Point(-1, -1));
	filter2D(src, outy, CV_32FC1, ymask, Point(-1, -1));




	/*for (int y = 0; y < src.rows; y++)
	{

		for (int x = 0; x <	 src.cols; x++)
		{
			float tmp = 0.0;
			for (int Y = 0; Y < maskSize; Y++)
			{
				for (int X = 0; X < maskSize; X++)
				{
					tmp += (float)src.at<uchar>(y + Y, x + X)*(mask.at<float>(Y, X)*(1 / sum));

				}
			}


			dest.at<uchar>(y + bestPoint, x + bestPoint) = (uchar)tmp;

		}

	}*/


	for (int y = 0; y < outx.rows; y++)
	{
		for (int x = 0; x < outx.cols; x++)
		{
			float pxlX = (float)outx.at<uchar>(y, x);
			float pxlY = (float)outy.at<uchar>(y, x);
			float pxlG = sqrt((pxlX*pxlX) + (pxlY*pxlY));
			mag.at<uchar>(y, x) = (uchar)pxlG;
		}
	}

	//normalize(mag, mag, 0, 255, CV_MINMAX, CV_32FC1);



	imshow("Input", src);
	//namedWindow("Mask", CV_WINDOW_FREERATIO);
	//imshow("Mask", mask);

	imshow("OutputX", outx);
	imshow("OutputY", outy);
	imshow("Magnitude", mag);

	waitKey();

}