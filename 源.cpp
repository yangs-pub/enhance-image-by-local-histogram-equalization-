#include <opencv2\opencv.hpp>
#include <iostream>
#include<math.h>
using namespace std;



using namespace cv;
Mat histImage(500, 500, CV_8UC3, cv::Scalar(0, 0, 0));
void huiduzhifangtu(Mat ImageGray)
{

	
	// ����ֱ��ͼ����
	const int channels[1] = { 0 };
	const int histSize[1] = { 256 };
	float pranges[2] = { 0,255 };
	const float* ranges[1] = { pranges };



	cv::MatND hist;
	// ����ֱ��ͼ
	cv::calcHist(&ImageGray, 1, channels, cv::Mat(), hist, 1,
		histSize, ranges);
	// ��ʼ����������
	int hist_w = 500;
	int hist_h = 500;
	int nHistSize = 255;
	//// ����
	int bin_w = cvRound((double)hist_w / nHistSize);
	// ��ֱ��ͼ��һ������Χ [ 0, histImage.rows ]
	normalize(hist, hist, 0, histImage.rows,
		cv::NORM_MINMAX, -1, cv::Mat());
	// ��ֱ��ͼ�����ϻ���ֱ��ͼ
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
	Mat srcImg, grayImg;//����ԭʼͼ�ͻҶȶ�
	srcImg = imread("1.jpg");//����ԭʼͼ
	if (srcImg.empty())
	{
		cout << "ͼ�����ʧ��!" << endl;
		return -1;
	}
	else
		cout << "ͼ����سɹ�!" << endl << endl;

	imshow("ԭʼͼ", srcImg);//��ʾԭʼͼ
	cvtColor(srcImg, grayImg, CV_RGB2GRAY);//��ԭʼͼ��ת��Ϊ�Ҷ�ͼ
	//MatND hist2;
	int rowNumber = grayImg.rows;//�õ�ԭͼ������
	int colNumber = grayImg.cols;//�õ�ԭͼ������
	int sumNumber = rowNumber*colNumber;//�õ�ͼ�����ظ���
	Mat dstImg(rowNumber, colNumber, CV_8UC1, Scalar(0, 0, 0));//��ʼ��ֱ��ͼ���⻯���ͼ
	double hist[256] = { 0.00 };//ֱ��ͼ
	double dhist[256] = { 0.00 };//ֱ��ͼ��һ��ͼ
	double Dhist[256] = { 0.00 };//ֱ��ͼ����ͼ��ÿһ�����ص�
	int graySum = 0,grayAve = 0;				//����ƽ���Ҷ�
	int abutgraySum = 0, abutgrayAve = 0;		//��������Ҷ�
	for (int i = 0;i<rowNumber;i++)//����ԭʼͼ�񣬵õ�ֱ��ͼ
	{
		uchar* data = grayImg.ptr<uchar>(i);
		for (int j = 0;j<colNumber;j++)
		{
			int temp = data[j];//�õ�ͼ��Ҷ�ֵ
			graySum = graySum + temp;
			hist[temp] = hist[temp] + 1;//����Ӧ�Ҷ�ֵ������ֱ��ͼ�м�1
		}
	}
	grayAve = graySum / sumNumber;
	for (int i = 0;i<256;i++)//����ֱ��ͼ���õ���һ��ֱ��ͼ�ͻ���ͼ
	{
		dhist[i] = hist[i] / sumNumber;//�õ���һ��ͼ
		for (int j = 0;j <= i;j++)
		{
			Dhist[i] = Dhist[i] + dhist[j]; //�õ�����ͼ
		}
	}


	for (int i = 1;i<rowNumber-1;i++)//�Ի���ͼΪ���ұ�õ����⻯���ͼ
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
				int temp1 = data2[j]; //���ҵ�ԭʼͼ��Ӧλ�õ�����ֵ
				int temp2 = (int)(Dhist[temp1] * 255); //�ڻ���ͼ���ҵ���Ӧ����ֵ��ӳ��ֵ

				data1[j] = temp2;//��ӳ��ֵ��ֵ��Ŀ��ͼ����Ӧֵ
			}
		}
	}
	imshow("���⻯���ͼ", dstImg+grayImg);


	huiduzhifangtu(grayImg);
	imshow("ԭֱ��ͼ", histImage);
	histImage = Mat::zeros(500, 500, CV_8UC3);
	huiduzhifangtu(dstImg);
	imshow("����ֱ��ͼ", histImage);
	waitKey(0);
	return 0;
}