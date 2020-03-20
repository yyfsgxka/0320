#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	//练习一对下图先进行二值化，然后分别进行腐蚀、膨胀、开运算和闭运算。
	
	Mat element= getStructuringElement(MORPH_RECT, Size(15, 15));
	Mat pengzhang;
	cv::Mat fushi;
	cv::Mat kai;
	cv::Mat bi;
	Mat srcMat = imread("coin.png", 0);
	imshow("coin", srcMat);
	//膨胀
	dilate(srcMat, pengzhang, element,Point(-1, -1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	imshow("pengzhang", pengzhang);
	//腐蚀
	erode(srcMat, fushi, element, Point(-1, -1), 1, BORDER_CONSTANT, morphologyDefaultBorderValue());
	imshow("fushi", fushi);
	//开
	morphologyEx(srcMat, kai, MORPH_OPEN, element);
	imshow("kai", kai);
	//闭
	morphologyEx(srcMat, bi, MORPH_CLOSE, element);
	imshow("bi", bi);
	waitKey(0);



	//练习二连通域标记
	//对下图先进行二值化，然后进行连通域标记，并绘制出每个连通域的 外接四边形（bounding box），并使用 cout << ，将硬币的个数 输 出至状态栏。
	Mat frmMat = imread("coin.png", 0);
	Mat binaryMat;
	Mat labelMat;
	Mat	statsMat;
	Mat	centrMat;
	int font_face = cv::FONT_HERSHEY_COMPLEX;
	double font_scale = 1.5;
	int thickness = 1;
	int baseline;

	int elementSize = 5;

	cout << "------program starts------ " << endl;

	if (frmMat.empty())
	{
		cout << "load image error!" << endl;
	}

	
	//定义
	Size kenerlSize;
	kenerlSize.height = 3;
	kenerlSize.width = 3;
	//腐蚀运算
	cv::Mat element1 = getStructuringElement(MORPH_RECT, Size(3, 3));
	//定义参数形状为矩形，定义长宽高，定义锚点（自定义）  
	morphologyEx(frmMat, binaryMat, MORPH_CLOSE, element1);
	//原图，后图，闭运算，参数
	//获得连通域
	int nComp = cv::connectedComponentsWithStats(binaryMat,
		labelMat,
		statsMat,
		centrMat,
		8,
		CV_32S);

	//输出连通域信息

	for (int i = 0; i < nComp; i++)
	{
		//各个连通域的统计信息保存在stasMat中
		cout << "connected Components NO. " << i << endl;
		cout << "pixels = " << statsMat.at<int>(i, 4) << endl;
		cout << "width = " << statsMat.at<int>(i, 2) << endl;
		cout << "height = " << statsMat.at<int>(i, 3) << endl;
		cout << endl;
	}

	//-1,nComp包括背景
	cout << "the total of connected Components = " << nComp - 1 << endl;
	//通过颜色表现连通域
	//显示用图像
	Mat  resultMat;
	resultMat = cv::Mat::zeros(srcMat.size(), CV_8UC3);
	std::vector<cv::Vec3b> colors(nComp);
	//背景使用黑色
	colors[0] = cv::Vec3b(0, 0, 0);
	//使用随机数产生函数randu，随机产生颜色
	for (int n = 1; n < nComp; n++)
	{
		colors[n] = cv::Vec3b(rand() / 255, rand() / 255, rand() / 255);
	}

	//对所有像素按照连通域编号进行着色
	for (int y = 0; y < srcMat.rows; y++)
	{
		for (int x = 0; x < srcMat.cols; x++)
		{
			int label = labelMat.at<int>(y, x);
			CV_Assert(0 <= label && label <= nComp);

			resultMat.at<cv::Vec3b>(y, x) = colors[label];


		}
	}

	//绘制bounding box


	for (int i = 1; i < nComp; i++)
	{

		char num[10];
		sprintf_s(num, "%d", i);

		Rect bndbox;
		//bounding box左上角坐标
		bndbox.x = statsMat.at<int>(i, 0);
		bndbox.y = statsMat.at<int>(i, 1);
		//bouding box的宽和长 
		bndbox.width = statsMat.at<int>(i, 2);
		bndbox.height = statsMat.at<int>(i, 3);
		//绘制
		rectangle(resultMat, bndbox, CV_RGB(255, 255, 255), 1, 8, 0);
		//连通域编号
		cv::putText(resultMat, num, Point(bndbox.x, bndbox.y), font_face, 1, cv::Scalar(0, 255, 255), thickness, 8, 0);
	}

	//将连通域数量绘制在图片上
	//设置绘制文本的相关参数
	char text[20];
	int length = sprintf_s(text, "%d", nComp - 1);

	//获取文本框的长宽
	cv::Size text_size = cv::getTextSize(text, font_face, font_scale, thickness, &baseline);
	//将文本框居中绘制
	cv::Point origin;
	origin.x = 0;
	origin.y = text_size.height;
	cv::putText(resultMat, text, origin, font_face, font_scale, cv::Scalar(0, 255, 255), thickness, 8, 0);

	imshow("binaryMat", binaryMat);
	imshow("results", resultMat);
	imshow("frame", frmMat);
	moveWindow("frame", 0, 20);
	moveWindow("binaryMat", srcMat.cols, 20);
	moveWindow("results", srcMat.cols * 2, 20);
	waitKey(0);

	//练习3
	//使用大津法分割下图，并对其进行连通域标记，利用图像形态学中所 学的知识实现自动计算原点个数。
	cv::Mat binaryMat3;
	cv::Mat labelMat3;
	cv::Mat statsMat3;
	cv::Mat centrMat3;
	cv::Mat resultMat3;
	cv::Mat srcMat3 = imread("‪C:\\Users\\YangYF\\Desktop\\2.jpg", 0);
	int font_face = cv::FONT_HERSHEY_COMPLEX;
	double font_scale = 1.5;
	int thickness = 1;
	int baseline;

	int elementSize = 5;

	cout << "------program starts------ " << endl;

	if (srcMat3.empty())
	{
		cout << "load image error!" << endl;
	}

	//二值化
	cv::threshold(srcMat3, binaryMat3, 0, 255, THRESH_OTSU);
	//定义
	Size kenerlSize;
	kenerlSize.height = 3;
	kenerlSize.width = 3;
	//腐蚀运算
	cv::Mat element3 = getStructuringElement(MORPH_RECT, Size(3, 3));
	//定义参数形状为矩形，定义长宽高，定义锚点（自定义）  
	morphologyEx(binaryMat, binaryMat, MORPH_CLOSE, element3);
	//原图，后图，闭运算，参数
	//获得连通域
	int nComp = cv::connectedComponentsWithStats(binaryMat3,
		labelMat3,
		statsMat3,
		centrMat3,
		8,
		CV_32S);

	//输出连通域信息

	for (int i = 0; i < nComp; i++)
	{
		//各个连通域的统计信息保存在stasMat中
		cout << "connected Components NO. " << i << endl;
		cout << "pixels = " << statsMat3.at<int>(i, 4) << endl;
		cout << "width = " << statsMat3.at<int>(i, 2) << endl;
		cout << "height = " << statsMat3.at<int>(i, 3) << endl;
		cout << endl;
	}

	//-1,nComp包括背景
	cout << "the total of connected Components = " << nComp - 1 << endl;
	//通过颜色表现连通域
	//显示用图像
	resultMat3 = cv::Mat::zeros(srcMat3.size(), CV_8UC3);
	std::vector<cv::Vec3b> colors3(nComp);
	//背景使用黑色
	colors3[0] = cv::Vec3b(0, 0, 0);
	//使用随机数产生函数randu，随机产生颜色
	for (int n = 1; n < nComp; n++)
	{
		colors3[n] = cv::Vec3b(rand() / 255, rand() / 255, rand() / 255);
	}

	//对所有像素按照连通域编号进行着色
	for (int y = 0; y < srcMat3.rows; y++)
	{
		for (int x = 0; x < srcMat3.cols; x++)
		{
			int label = labelMat3.at<int>(y, x);
			CV_Assert(0 <= label && label <= nComp);

			resultMat3.at<cv::Vec3b>(y, x) = colors[label];


		}
	}

	//绘制bounding box


	for (int i = 1; i < nComp; i++)
	{

		char num[10];
		sprintf_s(num, "%d", i);

		Rect bndbox;
		//bounding box左上角坐标
		bndbox.x = statsMat3.at<int>(i, 0);
		bndbox.y = statsMat3.at<int>(i, 1);
		//bouding box的宽和长 
		bndbox.width = statsMat3.at<int>(i, 2);
		bndbox.height = statsMat3.at<int>(i, 3);
		//绘制
		rectangle(resultMat3, bndbox, CV_RGB(255, 255, 255), 1, 8, 0);
		//连通域编号
		cv::putText(resultMat3, num, Point(bndbox.x, bndbox.y), font_face, 1, cv::Scalar(0, 255, 255), thickness, 8, 0);
	}

	//将连通域数量绘制在图片上
	//设置绘制文本的相关参数
	char text3[20];
	int length = sprintf_s(text, "%d", nComp - 1);

	//获取文本框的长宽
	cv::Size text_size = cv::getTextSize(text, font_face, font_scale, thickness, &baseline);
	//将文本框居中绘制
	cv::Point origin;
	origin.x = 0;
	origin.y = text_size.height;
	cv::putText(resultMat3, text, origin, font_face, font_scale, cv::Scalar(0, 255, 255), thickness, 8, 0);

	imshow("binaryMat", binaryMat3);
	imshow("results", resultMat3);
	imshow("frame", srcMat3);
	moveWindow("frame", 0, 20);
	moveWindow("binaryMat", srcMat3.cols, 20);
	moveWindow("results", srcMat3.cols * 2, 20);
	waitKey(0);
	return 0;
	return 0;


}
