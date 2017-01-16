#ifndef MOUSE_H_INCLUDED
#define MOUSE_H_INCLUDED

using namespace std;

//鼠标回调函数1，用于选点
void Mouse1(int m_event, int x, int y, int flags, void* param) {
	if (m_event == CV_EVENT_LBUTTONDOWN)	
		oriPoints[countPoints[0]++] = cvPoint2D32f(x, y);
}

//鼠标回调函数2，霍夫变换变化时选点
void Mouse2(int m_event, int x, int y, int flags, void* param) {
	if (m_event == CV_EVENT_LBUTTONDOWN) {
		if (countPoints[1] == 0) {
			oldStart = cvPoint(x, y);
			countPoints[1]++;
		}
		else {
			if (countPoints[1] == 1) {
				oldEnd = cvPoint(x, y);
				countPoints[1]++;
			}
			else {
				if (countPoints[1] == 2) {
					oldStart = cvPoint(x, y);
					countPoints[1] = 1;
				}
			}
		}
	}
}

//鼠标回调函数3，细化时选点
void Mouse3(int m_event, int x, int y, int flags, void* param) {
	if (m_event == CV_EVENT_LBUTTONDOWN) {
		if (countPoints[2] == 0) {
			oldStart = cvPoint(x, y);
			countPoints[2]++;
		}
		else {
			if (countPoints[2] == 1) {
				oldEnd = cvPoint(x, y);
				countPoints[2]++;
			}
			else {
				if (countPoints[2] == 2) {
					oldStart = cvPoint(x, y);
					countPoints[2] = 1;
				}
			}
		}
	}
}

//鼠标回调函数4，选小车头部
void Mouse4(int m_event, int x, int y, int flags, void* param) {
	if (headSelected)
	{
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = selection.x + CV_IABS(x - origin.x);
		selection.height = selection.y + CV_IABS(y - origin.y);

		selection.x = MAX(selection.x, 0);
		selection.y = MAX(selection.y, 0);
		selection.width = MIN(selection.width, our_size.width);
		selection.height = MIN(selection.height, our_size.height);
		selection.width -= selection.x;
		selection.height -= selection.y;
	}
	switch (m_event)
	{
	case CV_EVENT_LBUTTONDOWN:
		origin = cvPoint(x, y);
		selection = cvRect(x, y, 0, 0);
		headSelected = true;
		break;
	case CV_EVENT_LBUTTONUP:
		headSelected = false;
		if (selection.width > 0 && selection.height > 0) {
			headIsIn = true;
			cout << "Head Selected" << endl;
		}
		head_track_window = selection;
		break;
	}
}

//鼠标回调函数5，选小车尾部
void Mouse5(int m_event, int x, int y, int flags, void* param) {
	if (tailSelected)
	{
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = selection.x + CV_IABS(x - origin.x);
		selection.height = selection.y + CV_IABS(y - origin.y);

		selection.x = MAX(selection.x, 0);
		selection.y = MAX(selection.y, 0);
		selection.width = MIN(selection.width, our_size.width);
		selection.height = MIN(selection.height, our_size.height);
		selection.width -= selection.x;
		selection.height -= selection.y;
	}
	switch (m_event)
	{
	case CV_EVENT_LBUTTONDOWN:
		origin = cvPoint(x, y);
		selection = cvRect(x, y, 0, 0);
		tailSelected = true;
		break;
	case CV_EVENT_LBUTTONUP:
		tailSelected = false;
		if (selection.width > 0 && selection.height > 0) {
			tailIsIn = true;
			cout << "Tail Selected" << endl;
		}
		tail_track_window = selection;
		break;
	}
}
#endif
