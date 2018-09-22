#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

int main()
{
	Mat src = imread("lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	Mat destMax = src.clone();

	Mat destMin = src.clone();

	Mat destMed = src.clone();

	Mat mask;


	int maskSize;

	cout << "Enter Mask Size: " << endl;

	cin >> maskSize;

	

	

	mask.create(maskSize, maskSize, CV_32F);

	int bestPoint = maskSize / 2;

	mask.setTo(0);


	float temp, gxy;

	
	//floating value mask

	for (int y = 0; y < maskSize; y++)
	{
		for (int x = 0; x < maskSize; x++)
		{
			
			gxy = 1.0;

			mask.at<float>(y, x) = gxy;

			
			//cout << gxy << endl;
		}
	}

	

	copyMakeBorder(src, src, maskSize / 2, maskSize / 2, maskSize / 2, maskSize / 2, 0, 0);

	vector < float > v;

	for (int y = 0; y < src.rows - maskSize + 1; y++)
	{

		for (int x = 0; x < src.cols - maskSize + 1; x++)
		{
			float tmp = 0.0;
			for (int Y = 0; Y < maskSize; Y++)
			{
				for (int X = 0; X < maskSize; X++)
				{
					v.push_back((float)src.at<uchar>(y + Y, x + X)*mask.at<float>(Y,X));


				}
			}

			sort(v.begin(), v.end());



			destMax.at<uchar>(y+bestPoint, x+bestPoint) = (uchar)v[v.size()-1];

			destMin.at<uchar>(y + bestPoint, x + bestPoint) = (uchar)v[0];

			destMed.at<uchar>(y + bestPoint, x + bestPoint) = (uchar)v[v.size()/2];

			v.clear();

		}

	}


	imshow("Input", src);

	imshow("Output Maxfilter", destMax);

	imshow("Output Minfilter", destMin);

	imshow("Output Medianfilter", destMed);

	waitKey();

}