#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

Mat erosion(Mat input, Mat th, int maskSize)
{

	Mat src = input.clone();
	Mat destTemp = th.clone();
	Mat dest = th.clone();
	bool check = false;
	int s = maskSize / 2;

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			check = false;
			for (int maskCol = 0; maskCol < maskSize; maskCol++)
			{
				for (int maskRow = 0; maskRow < maskSize; maskRow++)
				{
					if (destTemp.at<uchar>(y + maskCol, x + maskRow) != 255)
					{
						check = true;
						break;
					}
				}
				if (check)
				{
					break;
				}
			}

			if (check)
			{
				dest.at<uchar>(y + s, x + s) = 0;
			}
			else
			{
				dest.at<uchar>(y + s, x + s) = 255;
			}
		}
	}

	return dest;

}


Mat dilation(Mat input, Mat th, int maskSize)
{

	Mat src = input.clone();
	Mat destTemp = th.clone();
	Mat dest = th.clone();
	bool check = false;
	int s = maskSize / 2;

	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			check = false;
			for (int maskCol = 0; maskCol < maskSize; maskCol++)
			{
				for (int maskRow = 0; maskRow < maskSize; maskRow++)
				{
					if (destTemp.at<uchar>(y + maskCol, x + maskRow) == 255)
					{
						check = true;
						break;
					}
				}
				if (check)
				{
					break;
				}
			}

			if (check)
			{
				dest.at<uchar>(y + s, x + s) = 255;
			}
			else
			{
				dest.at<uchar>(y + s, x + s) = 0;
			}
		}
	}

	return dest;

}


int main()
{
	Mat src = imread("finger.tif", CV_LOAD_IMAGE_GRAYSCALE);
	Mat dest = src.clone();
	Mat destTemp = src.clone();
	Mat destBoundary = src.clone();
	Mat destEr1 = src.clone();
	Mat destEr2 = src.clone();
	Mat destDi1 = src.clone();
	Mat destDi2 = src.clone();

	int maskSize;

	cout << "Enter Mask Size: ";
	cin >> maskSize;

	Mat mask;
	mask.create(maskSize, maskSize, CV_8UC1);
	mask.setTo(255);

	//copyMakeBorder(src, src, maskSize / 2, maskSize / 2, maskSize / 2, maskSize / 2, 0, 0);
	copyMakeBorder(dest, dest, maskSize / 2, maskSize / 2, maskSize / 2, maskSize / 2, 0, 0);
	copyMakeBorder(destTemp, destTemp, maskSize / 2, maskSize / 2, maskSize / 2, maskSize / 2, 0, 0);
	copyMakeBorder(destEr1, destEr1, maskSize / 2, maskSize / 2, maskSize / 2, maskSize / 2, 0, 0);
	copyMakeBorder(destEr2, destEr2, maskSize / 2, maskSize / 2, maskSize / 2, maskSize / 2, 0, 0);
	copyMakeBorder(destDi1, destDi1, maskSize / 2, maskSize / 2, maskSize / 2, maskSize / 2, 0, 0);
	copyMakeBorder(destDi2, destDi2, maskSize / 2, maskSize / 2, maskSize / 2, maskSize / 2, 0, 0);





	for (int y = 0; y < destTemp.rows; y++)
	{
		for (int x = 0; x < destTemp.cols; x++)
		{
			if (destTemp.at<uchar>(y, x)>200)
			{
				destTemp.at<uchar>(y, x) = 255;
			}
			else {
				destTemp.at<uchar>(y, x) = 0;
			}
		}
	}



	dest = erosion(src, destTemp, maskSize).clone();

	destEr1 = erosion(src, destTemp, maskSize).clone();
	destDi1 = dilation(src, destEr1, maskSize).clone();
	destDi2 = dilation(src, destDi1, maskSize).clone();
	destEr2 = erosion(src, destDi2, maskSize).clone();

	imshow("Input", src);
	imshow("Erosion1", destEr1);
	imshow("Dilation1", destDi1);
	imshow("Dilation2", destDi2);
	imshow("Opening Closing", destEr2);
	waitKey();


}