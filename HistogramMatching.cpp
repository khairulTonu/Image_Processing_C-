#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;


Mat prCalculation(Mat arrCnt, int M, int N)
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
	Mat pt, prAfterTemp;
	pt.create(300, 256, CV_8UC1);
	pt.setTo(0);
	prAfterTemp = prAfter.clone();

	for (int y = 0; y < 256; y++)
	{
		int size = prAfterTemp.at<float>(0, y);
		for (int x = 299; x >= (int)(300 - (300 * prAfterTemp.at<float>(0, y))); x--)
		{
			pt.at<uchar>(x, y) = 255;
		}
		//cout << (int)(300 - (300 * prAfterTemp.at<float>(0, y))) << endl;
	}

	return pt;
}

int main()
{
	Mat src = imread("lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat dest = src.clone();
	Mat destTemp = src.clone();
	Mat destEqz = src.clone();
	float sigma,u;

	cout << "Enter Sigma: ";
	cin >> sigma;
	cout << "Enter meu: ";
	cin >> u;

	int M = src.rows;
	int N = src.cols;
	//int arrCnt[255];
	//memset(arrCnt, 0, sizeof(arrCnt));

	Mat pr,prGauss;
	Mat arrCnt;
	pr.create(1, 256, CV_32F);
	pr.setTo(0);
	arrCnt.create(1, 256, CV_32F);
	arrCnt.setTo(0);
	prGauss.create(1, 256, CV_32F);
	prGauss.setTo(0);

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

	//Gaussian

	float oneby2piSigma;
	Mat gauss,gaussPt;
	gauss.create(1, 256, CV_32F);
	gauss.setTo(0);

	gaussPt.create(300, 256, CV_8UC1);
	gaussPt.setTo(0);

	oneby2piSigma = 1/(sqrt(2 * 3.1416)*sigma);

	for (int x = 0; x < 256; x++)
	{
		gauss.at<float>(0, x) = oneby2piSigma * exp((-(u-x)*(u-x)) / (2 * sigma*sigma));
	}
	
	/*for (int i = 0; i < 256; i++)
	{
		cout << 300*gauss.at<float>(0, i) << endl;
	}
	*/


	Mat s;
	s.create(1, 256, CV_32F);
	s.setTo(0);

	for (int y = 0; y < 256; y++)
	{
		float sum = 0.0;
		for (int x = 0; x <= y; x++)
		{

			sum = sum + pr.at<float>(0, x);
		}

		if (255 * sum - (int)(255 * sum)>0.5)
		{
			s.at<float>(0, y) = (int)(255 * sum) + 1;
		}
		else {
			s.at<float>(0, y) = (int)255 * sum;
		}

		//cout << s.at<float>(0, y) << endl;
	}


	//For G(Z)

	Mat g;
	g.create(1, 256, CV_32F);
	g.setTo(0);

	for (int y = 0; y < 256; y++)
	{
		float sum = 0.0;
		for (int x = 0; x <= y; x++)
		{

			sum = sum + gauss.at<float>(0, x);
		}

		if (255 * sum - (int)(255 * sum)>=0.5)
		{
			g.at<float>(0, y) = (int)(255 * sum) + 1;
		}
		else {
			g.at<float>(0, y) = (int)255 * sum;
		}

		//cout << g.at<float>(0, y) << endl;
	}
	

	Mat gs;
	gs.create(1, 256, CV_32F);
	gs.setTo(0);
	int k;
	for (int i = 0; i < 256; i++)
	{
		int mn = 99999;
		for (int j = 0; j < 256; j++)
		{
			int val = abs(g.at<float>(0, i) - s.at<float>(0, j));

			if (val < mn)
			{
				mn = val;
				k = j;
			}

		}
		gs.at<float>(0, i) = k;
		//cout << k << endl;
	}


	for (int y = 0; y < M; y++)
	{
		for (int x = 0; x < N; x++)
		{
			int pix = (int)dest.at<uchar>(y, x);
			int replace = (int)gs.at<float>(0, pix);
			dest.at<uchar>(y, x) = (uchar)replace;
		}
	}

	for (int y = 0; y < M; y++)
	{
		for (int x = 0; x < N; x++)
		{
			int pix = (int)destEqz.at<uchar>(y, x);
			int replace = (int)s.at<float>(0, pix);
			destEqz.at<uchar>(y, x) = (uchar)replace;
		}
	}



	Mat prAfter,prAfterEqz;
	prAfter.create(1, 256, CV_32F);
	prAfter.setTo(0);
	prAfter = prCalculation(gs, M, N).clone();

	Mat arrCntAfter,arrCntAfterEqz;
	arrCntAfter.create(1, 256, CV_32F);
	arrCntAfter.setTo(0);

	arrCntAfterEqz.create(1, 256, CV_32F);
	arrCntAfterEqz.setTo(0);

	for (int y = 0; y < M; y++)
	{
		for (int x = 0; x < N; x++)
		{
			int cnt = (int)dest.at<uchar>(y, x);

			arrCntAfter.at<float>(0, cnt) += 1;
		}
	}

	for (int y = 0; y < M; y++)
	{
		for (int x = 0; x < N; x++)
		{
			int cnt = (int)destEqz.at<uchar>(y, x);

			arrCntAfterEqz.at<float>(0, cnt) += 1;
		}
	}



	prAfter = prCalculation(arrCntAfter, M, N).clone();

	prAfterEqz = prCalculation(arrCntAfterEqz, M, N).clone();

	normalize(prAfter, prAfter, 0, 1, CV_MINMAX);
	normalize(prAfterEqz, prAfterEqz, 0, 1, CV_MINMAX);

	normalize(pr, pr, 0, 1, CV_MINMAX);

	normalize(gauss, gauss, 0, 1, CV_MINMAX);


	for (int i = 0; i < 256; i++)
	{
		//cout << 300*prAfter.at<float>(0, i) << endl;

	}

	gaussPt = plot(gauss).clone();

	Mat ptAfter, pt,ptAfterEqz;
	ptAfter.create(300, 256, CV_8UC1);
	ptAfter.setTo(0);
	ptAfter = plot(prAfter).clone();

	ptAfterEqz.setTo(0);
	ptAfterEqz = plot(prAfterEqz).clone();

	pt.create(300, 256, CV_8UC1);
	pt.setTo(0);
	pt = plot(pr).clone();


	imshow("Input", src);
	imshow("Gaussian Plot", gaussPt);
	imshow("Input Plot", pt);
	imshow("Equalization Output", destEqz);
	imshow("Equalization Output Plot", ptAfterEqz);
	imshow("Matching Output", dest);
	imshow("Matching Output Plot", ptAfter);
	waitKey();



}