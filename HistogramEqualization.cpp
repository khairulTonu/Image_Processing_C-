#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;


Mat prCalculation(Mat arrCnt,int M,int N)
{
	Mat pr = arrCnt.clone();
	for (int y = 0; y < 256; y++)
	{
		float temp = pr.at<float>(0, y);
		pr.at<float>(0, y) = temp / (M*N);
		//cout << temp/(M*N)<< endl;

	}
	return pr;

}

Mat plot(Mat prAfter)
{
	Mat pt,prAfterTemp;
	pt.create(300, 256, CV_8UC1);
	pt.setTo(0);
	prAfterTemp = prAfter.clone();

	for (int y = 0; y < 256; y++)
	{
		int size = prAfterTemp.at<float>(0, y);
		for (int x = 299; x >= (int)(300 - (300 * prAfterTemp.at<float>(0, y))); x--)
		{
			pt.at<uchar>(x, y)=255;
		}
		//cout << (int)(300 - (300 * prAfterTemp.at<float>(0, y))) << endl;
	}

	return pt;
}

int main()
{
	Mat src = imread("veg.tif", CV_LOAD_IMAGE_GRAYSCALE);
	Mat dest = src.clone();
	Mat destTemp = src.clone();

	int M = src.rows;
	int N = src.cols;
	//int arrCnt[255];
	//memset(arrCnt, 0, sizeof(arrCnt));

	Mat pr;
	Mat arrCnt;
	pr.create(1, 256, CV_32F);
	pr.setTo(0);
	arrCnt.create(1, 256, CV_32F);
	arrCnt.setTo(0);

	for (int y = 0; y < M; y++)
	{
		for (int x = 0; x < N; x++)
		{
			int cnt = (int)src.at<uchar>(y, x);

			arrCnt.at<float>(0, cnt) += 1;
		}
	}

	pr = prCalculation(arrCnt, M, N).clone();

	/*for (int i = 0; i < 256; i++)
	{
		cout << pr.at<float>(0,i) << endl;

	}*/

	Mat s;
	s.create(1, 256, CV_32F);

	for (int y = 0; y < 256; y++)
	{
		float sum = 0.0;
		for (int x = 0; x <= y; x++)
		{
			
			sum = sum + pr.at<float>(0, x);
		}

		if (255 * sum - (int)(255 * sum)>0.5)
		{
			s.at<float>(0, y) = (int)(255 * sum)+1;
		}
		else {
			s.at<float>(0, y) = (int)255 * sum;
		}
	}

	for (int y = 0; y < M; y++)
	{
		for (int x = 0; x < N; x++)
		{
			int pix = (int)dest.at<uchar>(y, x);
			int replace = (int)s.at<float>(0, pix);
			dest.at<uchar>(y, x) = (uchar)replace;
		}
	}

	Mat prAfter;
	prAfter.create(1, 256, CV_32F);
	prAfter.setTo(0);
	prAfter = prCalculation(s, M, N).clone();

	Mat arrCntAfter;
	arrCntAfter.create(1, 256, CV_32F);
	arrCntAfter.setTo(0);

	for (int y = 0; y < M; y++)
	{
		for (int x = 0; x < N; x++)
		{
			int cnt = (int)dest.at<uchar>(y, x);

			arrCntAfter.at<float>(0, cnt) += 1;
		}
	}

	prAfter = prCalculation(arrCntAfter, M, N).clone();

	normalize(prAfter, prAfter, 0, 1, CV_MINMAX);

	normalize(pr, pr, 0, 1, CV_MINMAX);


	for (int i = 0; i < 256; i++)
	{
		//cout << 300*prAfter.at<float>(0, i) << endl;

	}

	Mat ptAfter,pt;
	ptAfter.create(300, 256, CV_8UC1);
	ptAfter.setTo(0);
	ptAfter = plot(prAfter).clone();

	pt.create(300, 256, CV_8UC1);
	pt.setTo(0);
	pt = plot(pr).clone();
	

	imshow("Input", src);
	imshow("Input Plot", pt);
	imshow("Output", dest);
	imshow("Output Plot", ptAfter);
	waitKey();



}