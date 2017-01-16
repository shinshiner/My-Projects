#ifndef CONTROL_H_INCLUDED
#define CONTROL_H_INClUDED

#include<vector>
#include<windows.h>

#include"carMath.h"

//根据小车位置对蓝牙发出信息，控制小车移动

using namespace std;
using namespace ours;

bool headSecure(int x, int y) {
	if (x > 20 && x < 480 && y>20 && y < 480) return true;
	else return false;
}

bool tailSecure(int x, int y) {
	if (x > 20 && x < 480 && y>20 && y < 480) return true;
	else return false;
}

//根据小车位置和方向确定下一步的指令
char getCommand(int x1, int y1, int x2, int y2, int &current, vector<CvPoint> &route) {
	int target;

	if (current == route.size()-1) target = 0;
	else target = current + 1;
	if (Distance(route[target], cvPoint(x1, y1)) < 32) {
		if (current + 1 == route.size()) {
			current = 0;
			target++;
		}
		else {
			current++;
			if (target + 1 == route.size()) target = 0;
			else target++;
		}
	}

	CvPoint targetPoint = route[target];
	pair<int, int> v0 = make_pair(x1 - x2, y1 - y2);
	pair<int, int> v1 = make_pair(targetPoint.x - x1, targetPoint.y - y1);
	pair<int, int> v2 = make_pair(targetPoint.x - x2, targetPoint.y - y2);
	if (SIN(v0, v2) > -sin((minA / 3)*PI / 180) && SIN(v0, v2) < sin((minA / 3)*PI / 180)) {
		if (COS(v1, v2) >= 0) {
			if (headSecure(x1, y1)) {
				direction = true;
				cmd = 'F';
				return 'F';
			}
			else {
				cout << "head lost" << endl;
				cmd = 'B';
				return 'B';
			}
		}
		else {
			if (tailSecure(x2, y2)) {
				direction = false;
				cmd = 'B';
				return 'B';
			}
			else {
				cout << "tail lost" << endl;
				cmd = 'F';
				return 'F';
			}
		}
	}
	else if (SIN(v0, v2) > 0) {
		if (headSecure(x1, y1) && direction) {
			if (cmd == 'R') {					//N = do nothing, 如果指令之前就是正确的，则不对车发出进一步指令
				cmd = 'N';
				return 'N';
			}
			else {
				cmd = 'R';
				return 'R';
			}
		}
		else if (!headSecure(x1, y1) && direction) {
			direction = false;
			if (cmd == 'l') {
				cmd = 'N';
				return 'N';
			}
			else {
				cmd = 'l';
				return 'l';
			}
		}
		else if (tailSecure(x2, y2) && !direction) {
			if (cmd == 'l') {
				cmd = 'N';
				return 'N';
			}
			else {
				cmd = 'l';
				return 'l';
			}
		}
		else if (!tailSecure(x2,y2) && !direction) {
			direction = true;
			if (cmd == 'R') {
				cmd = 'N';
				return 'N';
			}
			else {
				cmd = 'R';
				return 'R';
			}
		}
	}
	else if (SIN(v0, v2) < 0) {
		if (headSecure(x1, y1) && direction) {
			if (cmd == 'L') {
				cmd = 'N';
				return 'N';
			}
			else {
				cmd = 'L';
				return 'L';
			}
		}
		else if (!headSecure(x1, y1) && direction) {
			direction = false;
			if (cmd == 'r') {
				cmd = 'N';
				return 'N';
			}
			else {
				cmd = 'r';
				return 'r';
			}
		}
		else if (tailSecure(x2, y2) && !direction) {
			if (cmd == 'r') {
				cmd = 'N';
				return 'N';
			}
			else {
				cmd = 'r';
				return 'r';
			}
		}
		else if (!tailSecure(x2, y2) && !direction) {
			direction = true;
			if (cmd == 'L') {
				cmd = 'N';
				return 'N';
			}
			else {
				cmd = 'L';
				return 'L';
			}
		}
	}
	else
		return 'S';
}

void sendCommand(char c) {
	if (c == 'N') return;
	char ch[2];
	ch[0] = c;
	ch[1] = '\0';
	DWORD addr = 0;

	//判断连接是否成功
	if (!WriteFile(com, ch, 1, &addr, 0))	cout << "连接失败" << endl;
	else if (c == 'S') cout << "连接成功" << endl;
}

#endif
