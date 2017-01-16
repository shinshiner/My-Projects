#ifndef INITIATE_H_INCLUDED
#define INITIATE_H_INCLUDED

void makeWindow_1() {
	cvNamedWindow("原始图像");
	cvNamedWindow("透视变换");
	cvNamedWindow("二值化");
	cvNamedWindow("细化");
	cvNamedWindow("霍夫变换");
	cvNamedWindow("路径");
	cvNamedWindow("滑动条");
	tbOpen = false;

	cvResizeWindow("滑动条", 380, 430);
	cvCreateTrackbar("Hmin", "滑动条", &hmin, 255, 0);
	cvCreateTrackbar("Hmax", "滑动条", &hmax, 255, 0);
	cvCreateTrackbar("Smin", "滑动条", &smin, 255, 0);
	cvCreateTrackbar("Smax", "滑动条", &smax, 255, 0);
	cvCreateTrackbar("Vmin", "滑动条", &vmin, 255, 0);
	cvCreateTrackbar("Vmax", "滑动条", &vmax, 255, 0);
	cvCreateTrackbar("LECValue", "滑动条", &LECValue, 200, 0);
	cvCreateTrackbar("LMLength", "滑动条", &LMLength, 200, 0);
	cvCreateTrackbar("LMDistance", "滑动条", &LMDistance, 200, 0);
}

//隐藏所有窗口
void makePosition_1() {
	cvMoveWindow("原始图像", 0, 700);
	cvMoveWindow("透视变换", 70, 700);
	cvMoveWindow("二值化", 140, 700);
	cvMoveWindow("细化", 210, 700);
	cvMoveWindow("霍夫变换", 280, 700);
	cvMoveWindow("路径", 350, 700);
	if (!tbOpen)
		cvMoveWindow("滑动条", 420, 700);
}

void setCallBack_1() {
	cvSetMouseCallback("原始图像", Mouse1);
	cvSetMouseCallback("霍夫变换", Mouse2);
	cvSetMouseCallback("细化", Mouse3);
}

void makeWindow_2() {
	cvNamedWindow("原始图像");
	cvNamedWindow("透视变换");
	cvNamedWindow("Head");
	cvNamedWindow("Tail");
	cvNamedWindow("Head滑动条");
	cvNamedWindow("Tail滑动条");
	cvNamedWindow("Tracking");

	cvResizeWindow("Head滑动条", 380, 300);
	cvCreateTrackbar("Hmin", "Head滑动条", &hmin1, 255, 0);
	cvCreateTrackbar("Hmax", "Head滑动条", &hmax1, 255, 0);
	cvCreateTrackbar("Smin", "Head滑动条", &smin1, 255, 0);
	cvCreateTrackbar("Smax", "Head滑动条", &smax1, 255, 0);
	cvCreateTrackbar("Vmin", "Head滑动条", &vmin1, 255, 0);
	cvCreateTrackbar("Vmax", "Head滑动条", &vmax1, 255, 0);

	cvResizeWindow("Tail滑动条", 380, 300);
	cvCreateTrackbar("Hmin", "Tail滑动条", &hmin2, 255, 0);
	cvCreateTrackbar("Hmax", "Tail滑动条", &hmax2, 255, 0);
	cvCreateTrackbar("Smin", "Tail滑动条", &smin2, 255, 0);
	cvCreateTrackbar("Smax", "Tail滑动条", &smax2, 255, 0);
	cvCreateTrackbar("Vmin", "Tail滑动条", &vmin2, 255, 0);
	cvCreateTrackbar("Vmax", "Tail滑动条", &vmax2, 255, 0);
}

void makePosition_2() {
	cvMoveWindow("原始图像", 0, 700);
	cvMoveWindow("透视变换", 70, 700);
	cvMoveWindow("Head", 140, 700);
	cvMoveWindow("Tail", 210, 700);
	cvMoveWindow("Tracking", 280, 700);
	cvMoveWindow("Head滑动条", 350, 700);
	cvMoveWindow("Tail滑动条", 420, 700);
}

void setCallBack_2() {
	cvSetMouseCallback("Head", Mouse4);
	cvSetMouseCallback("Tail", Mouse5);
}

void keyReactor(int key) {
	if (key == '0') {
		countPoints[1] = 0;
		countPoints[2] = 0;
		routePoint.clear();
		cvSetZero(wayImage);
		makePosition_1();
		cvMoveWindow("霍夫变换", 400, 100);
		cvMoveWindow("滑动条", 910, 100);
		flag = 1;
	}
	if (key == '1') {
		cvDestroyAllWindows();
		cout << "Routine 1 Finished!\n" << endl;
		flag = false;
	}
	if (key == '2') {
		makePosition_2();
		cvMoveWindow("Head", 300, 100);
		cvMoveWindow("Head滑动条", 810, 100);
	}
	if (key == '3') {
		makePosition_2();
		cvMoveWindow("Tail", 300, 100);
		cvMoveWindow("Tail滑动条", 810, 100);
	}
	
	if (key == 27) {
		for (int i = 0; i < 5; i++)	sendCommand('S');
		cvReleaseVideoWriter(&writer);
		CloseHandle(com);
		flag = false;
	}
}

#endif
