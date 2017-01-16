#include <opencv2/opencv.hpp>
#include<iostream>
#include<stdlib.h>

#include"cv.h"
#include"highgui.h"
#include"cxcore.h"
#include"variables.h"
#include"mouseOperation.h"
#include"control.h"
#include"init.h"
#include"thin.h"
#include"route.h"

using namespace std;
using namespace cv;

int main() {
	sendCommand('S');
	vid.open(1);

	makeWindow_1();
	makePosition_1();
	setCallBack_1();
	cvMoveWindow("原始图像", 400, 100);
	while (flag) {
		vid >> frame; //读取当前帧
		image = &IplImage(frame);
		if (!image)	return -1;
		cvShowImage("原始图像", image);

		if (countPoints[0] == 4) {	//图像四角选定
			//透视转换
			cvGetPerspectiveTransform(oriPoints, newPoints, transMat);	//transMat 变换矩阵
			cvWarpPerspective(image, perImage, transMat);						//perImage 透视后的图像
			cvShowImage("透视变换", perImage);

			//二值化
			cvCvtColor(perImage, perImage_hsv, CV_RGB2HSV);				//perImage_hsv:灰度图，hsv:颜色空间
			cvInRangeS(perImage_hsv, cvScalar(MIN(hmax, hmin), MIN(smax, smin), MIN(vmax, vmin), 0),
				cvScalar(MAX(hmax, hmin), MAX(smax, smin), MAX(vmax, vmin), 0), binImage);	//binImage 二值化图像
			cvShowImage("二值化", binImage);

			//细化
			refine(binImage, thiImage);
			cvShowImage("细化", thiImage);	//thiImage 细化图像

			//霍夫变换
			CvSeq* lines = cvHoughLines2(thiImage, storage, CV_HOUGH_PROBABILISTIC,
										1, CV_PI / 180, LECValue, LMLength, LMDistance);
			cvSetZero(houImage);
			for (int i = 0; i < lines->total; i++) {
				CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);
				cvLine(houImage, line[0], line[1], cvScalar(255));
			}
			cvShowImage("霍夫变换", houImage);
			
			cvMoveWindow("原始图像", 0, 700);
			cvMoveWindow("霍夫变换", 400, 100);
			cvMoveWindow("滑动条", 910, 100);

			//取路径点
			if (countPoints[1] == 2 || countPoints[2] == 2) {
				cout << "正在选取路径点 ······" << endl;
				cvWaitKey(1000);
				setRoute(routePoint, lines, oldStart, oldEnd);
				cout << "路径点选取完成" << endl;
				makePosition_1();	
				cvMoveWindow("路径", 400, 100);
				cvSetZero(wayImage);
				for (auto itr = routePoint.begin(); itr != routePoint.end(); itr++) {
					route.push_back(*itr);
					cvCircle(wayImage, *itr, 3, cvScalar(255));
					if (itr != routePoint.begin()) {
						cvLine(wayImage, *(++itr), *(--itr), cvScalar(255));
					}
				}
				cvShowImage("路径", wayImage);
				cvWaitKey(1);

				//询问是否重新选点
				cout << "Is that route ok?(1 = yes, 0 = no)\n";
				key = waitKey(0);
				keyReactor(key);
			}
		}
		cvWaitKey(20);	//key为按键
	}

	//开始追踪小车
	flag = true;
	makeWindow_2();
	makePosition_2();
	cvSetMouseCallback("Head", Mouse4);
	cvSetMouseCallback("Tail", Mouse5);
	cvMoveWindow("Head", 300, 100);
	cvMoveWindow("Tail", 810, 100);

	cvSetZero(posImage);
	while (flag) {
		vid >> frame; //读取当前帧
		image = &IplImage(frame);
		if (!image)	return -2;
		cvShowImage("原始图像", image);

		//透视化
		cvGetPerspectiveTransform(oriPoints, newPoints, transMat);
		cvWarpPerspective(image, perImage, transMat);
		cvShowImage("透视变换", perImage);
		cvWriteFrame(writer, perImage);

		//二值化
		cvCvtColor(perImage, perImage_hsv, CV_RGB2HSV);
		cvInRangeS(perImage_hsv, cvScalar(MIN(hmax1, hmin1), MIN(smax1, smin1), MIN(vmax1, vmin1), 0),
			cvScalar(MAX(hmax1, hmin1), MAX(smax1, smin1), MAX(vmax1, vmin1), 0), HeadImage);
		cvShowImage("Head", HeadImage);
		cvInRangeS(perImage_hsv, cvScalar(MIN(hmax2, hmin2), MIN(smax2, smin2), MIN(vmax2, vmin2), 0),
			cvScalar(MAX(hmax2, hmin2), MAX(smax2, smin2), MAX(vmax2, vmin2), 0), TailImage);
		cvShowImage("Tail", TailImage);

		//如果已经选定了头尾，则进入if
		if (headIsIn && tailIsIn) {
			cvMoveWindow("Head", 300, 700);
			cvMoveWindow("Tail", 810, 700);

			//寻找头
			cvCamShift(HeadImage, head_track_window, cvTermCriteria(3, 10, 1), &head_track_comp, &head_track_box);
			head_track_window = head_track_comp.rect;
			CV_IMAGE_ELEM(posImage, uchar, (int)head_track_box.center.y, (int)head_track_box.center.x) = 255;

			//寻找尾
			cvCamShift(TailImage, tail_track_window, cvTermCriteria(3, 10, 1), &tail_track_comp, &tail_track_box);
			tail_track_window = tail_track_comp.rect;
			CV_IMAGE_ELEM(posImage, uchar, (int)tail_track_box.center.y, (int)tail_track_box.center.x) = 255;
			
			cvMoveWindow("Tracking", 300, 100);
			cvShowImage("Tracking", posImage);
			sendCommand(getCommand((int)head_track_box.center.x, (int)head_track_box.center.y, (int)tail_track_box.center.x, (int)tail_track_box.center.y, current, route));
		}
		key = cvWaitKey(80);
		keyReactor(key);
	}
	return 0;
}
