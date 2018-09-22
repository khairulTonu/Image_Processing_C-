#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

Mat erosion(Mat input,Mat th,int maskSize)
{

	Mat src = input.clone();
	Mat destTemp = th.clone();
	Mat dest=th.clone();
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


int main()
{
	Mat src = imread("human.tif", CV_LOAD_IMAGE_GRAYSCALE);
	Mat dest = src.clone();
	Mat destTemp = src.clone();
	Mat destBoundary = src.clone();

	int maskSize;

	cout << "Enter Mask Size: ";
	cin >> maskSize;

	Mat mask;
	mask.create(maskSize, maskSize, CV_8UC1);
	mask.setTo(255);

	//copyMakeBorder(src, src, maskSize / 2, maskSize / 2, maskSize / 2, maskSize / 2, 0, 0);
	copyMakeBorder(dest, dest, maskSize / 2, maskSize / 2, maskSize / 2, maskSize / 2, 0, 0);
	copyMakeBorder(destTemp, destTemp, maskSize / 2, maskSize / 2, maskSize / 2, maskSize / 2, 0, 0);




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

	//Boundary

	for (int y = 0; y < destTemp.rows; y++)
	{
		for (int x = 0; x < destTemp.cols; x++)
		{
			destBoundary.at<uchar>(y, x) = abs(dest.at<uchar>(y, x) - destTemp.at<uchar>(y, x));
		}
	}

	

	imshow("Input", src);
	imshow("Thresholding", destTemp);
	imshow("Erosion", dest);
	imshow("Boundary", destBoundary);
	waitKey();






}