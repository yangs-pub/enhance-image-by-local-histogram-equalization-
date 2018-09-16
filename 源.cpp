#include <opencv2\opencv.hpp>
#include <iostream>
#include<math.h>
using namespace std;



using namespace cv;
Mat histImage(500, 500, CV_8UC3, cv::Scalar(0, 0, 0));
void huiduzhifangtu(Mat ImageGray)
{

	
	// 定义直方图参数
	const int channels[1] = { 0 };
	const int histSize[1] = { 256 };
	float pranges[2] = { 0,255 };
	const float* ranges[1] = { pranges };



	cv::MatND hist;
	// 计算直方图
	cv::calcHist(&ImageGray, 1, channels, cv::Mat(), hist, 1,
		histSize, ranges);
	// 初始化画布参数
	int hist_w = 500;
	int hist_h = 500;
	int nHistSize = 255;
	//// 区间
	int bin_w = cvRound((double)hist_w / nHistSize);
	// 将直方图归一化到范围 [ 0, histImage.rows ]
	normalize(hist, hist, 0, histImage.rows,
		cv::NORM_MINMAX, -1, cv::Mat());
	// 在直方图画布上画出直方图
	for (int i = 1; i < nHistSize; i++)
	{
		line(histImage, cv::Point(bin_w*(i - 1),
			hist_h - cvRound(hist.at<float>(i - 1))),
			cv::Point(bin_w*(i),
				hist_h - cvRound(hist.at<float>(i))),
			cv::Scalar(0, 0, 255), 2, 8, 0);
	}
	
	
}
int main()
{
	Mat srcImg, grayImg;//声明原始图和灰度度
	srcImg = imread("1.jpg");//载入原始图
	if (srcImg.empty())
	{
		cout << "图像加载失败!" << endl;
		return -1;
	}
	else
		cout << "图像加载成功!" << endl << endl;

	imshow("原始图", srcImg);//显示原始图
	cvtColor(srcImg, grayImg, CV_RGB2GRAY);//将原始图像转化为灰度图
	//MatND hist2;
	int rowNumber = grayImg.rows;//得到原图像行数
	int colNumber = grayImg.cols;//得到原图像列数
	int sumNumber = rowNumber*colNumber;//得到图像像素个数
	Mat dstImg(rowNumber, colNumber, CV_8UC1, Scalar(0, 0, 0));//初始化直方图均衡化后的图
	double hist[256] = { 0.00 };//直方图
	double dhist[256] = { 0.00 };//直方图归一化图
	double Dhist[256] = { 0.00 };//直方图积分图，每一个像素点
	int graySum = 0,grayAve = 0;				//声明平均灰度
	int abutgraySum = 0, abutgrayAve = 0;		//声明邻域灰度
	for (int i = 0;i<rowNumber;i++)//遍历原始图像，得到直方图
	{
		uchar* data = grayImg.ptr<uchar>(i);
		for (int j = 0;j<colNumber;j++)
		{
			int temp = data[j];//得到图像灰度值
			graySum = graySum + temp;
			hist[temp] = hist[temp] + 1;//将相应灰度值数量在直方图中加1
		}
	}
	grayAve = graySum / sumNumber;
	for (int i = 0;i<256;i++)//遍历直方图，得到归一化直方图和积分图
	{
		dhist[i] = hist[i] / sumNumber;//得到归一化图
		for (int j = 0;j <= i;j++)
		{
			Dhist[i] = Dhist[i] + dhist[j]; //得到积分图
		}
	}


	for (int i = 1;i<rowNumber-1;i++)//以积分图为查找表得到均衡化后的图
	{
		uchar* data1 = dstImg.ptr<uchar>(i);
		uchar* data2 = grayImg.ptr<uchar>(i);
		uchar* data3 = grayImg.ptr<uchar>(i-1);
		uchar* data4 = grayImg.ptr<uchar>(i+1);

		for (int j = 1;j<colNumber-1;j++)
		{
			
			int jubu[9] = { data2[j - 1] + data2[j] + data2[j + 1] + data3[j - 1] + data3[j] + data3[j + 1] + data4[j - 1] + data4[j] + data4[j + 1] };
			
			int n = 9;
			float sum = 0;
			for (int p = 0;p<n;p++)
				sum += jubu[p];
			float jubupingjun = sum / n;
			float sum1 = 0;
			for (int q = 0;q<n;q++)
				sum1 = (jubu[q] - jubupingjun)*(jubu[q] - jubupingjun);
			float jubufangcha = sum1 / n;
			
			if ((jubufangcha > 80)&&(jubupingjun<0.4*grayAve))
			
			
			{
				int temp1 = data2[j]; //查找到原始图相应位置的像素值
				int temp2 = (int)(Dhist[temp1] * 255); //在积分图中找到相应像素值的映射值

				data1[j] = temp2;//将映射值赋值给目标图像相应值
			}
		}
	}
	imshow("均衡化后的图", dstImg+grayImg);


	huiduzhifangtu(grayImg);
	imshow("原直方图", histImage);
	histImage = Mat::zeros(500, 500, CV_8UC3);
	huiduzhifangtu(dstImg);
	imshow("改良直方图", histImage);
	waitKey(0);
	return 0;
}