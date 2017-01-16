#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED

#include<list>
#include<windows.h>

using namespace std;
using namespace cv;

int key = -1;
int countPoints[3] = { 0 };
int hmin = 0, hmax = 255, smin = 0, smax = 255, vmin = 0, vmax = 110;
int hmin1 = 110, hmax1 = 255, smin1 = 130, smax1 = 255, vmin1 = 40, vmax1 = 255;
int hmin2 = 0, hmax2 = 30, smin2 = 80, smax2 = 255, vmin2 = 70, vmax2 = 255;
int LECValue = 17, LMLength = 20, LMDistance = 20;
int maxD = 8, maxA = 160, minDP2P = 50, minA = 20;
int current = 0;

bool headIsIn = false, tailIsIn = false, headSelected = false, tailSelected = false;
bool flag = true;
bool tbOpen = false;
bool direction = true;

char cmd = 'S';

//----------------------------------------------------------//

CvSize our_size = cvSize(500, 500);	//图片尺寸

IplImage* perImage = cvCreateImage(our_size, IPL_DEPTH_8U, 3);			//透视转换
IplImage* perImage_hsv = cvCreateImage(our_size, IPL_DEPTH_8U, 3);		//灰度图
IplImage* binImage = cvCreateImage(our_size, IPL_DEPTH_8U, 1);			//二值化
IplImage* thiImage = cvCreateImage(our_size, IPL_DEPTH_8U, 1);			//细化
IplImage* houImage = cvCreateImage(our_size, IPL_DEPTH_8U, 1);			//霍夫变换
IplImage* wayImage = cvCreateImage(our_size, IPL_DEPTH_8U, 1);			//路线简化图
IplImage* HeadImage = cvCreateImage(our_size, IPL_DEPTH_8U, 1);			//车头选取图像
IplImage* TailImage = cvCreateImage(our_size, IPL_DEPTH_8U, 1);			//车尾选取图像
IplImage* posImage = cvCreateImage(our_size, IPL_DEPTH_8U, 1);
IplImage *image;

CvMat* transMat = cvCreateMat(3, 3, CV_32FC1);
CvPoint2D32f oriPoints[4];
CvPoint2D32f newPoints[4] = { cvPoint2D32f(0, 0),cvPoint2D32f(our_size.width, 0),cvPoint2D32f(0, our_size.height),cvPoint2D32f(our_size.width, our_size.height) };
CvPoint oldStart, oldEnd;
CvPoint origin;

list<CvPoint> routePoint;
vector<CvPoint> route;

CvRect head_track_window, tail_track_window, selection;
CvMemStorage* storage = cvCreateMemStorage();
CvConnectedComp head_track_comp, tail_track_comp;
CvBox2D head_track_box, tail_track_box;

HANDLE com = CreateFile(L"COM5", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
CvVideoWriter* writer = cvCreateVideoWriter("vido.avi", CV_FOURCC('D', 'I', 'V', 'X'), 20, our_size);

VideoCapture vid;	//声明视频读入类
Mat frame;			//定义一个Mat变量，用于存储每一帧的图像
#endif
