#ifndef SETROUTE_H_INCLUDED
#define SETROUTE_H_INCLUDED

//得到小车前进路径

#include<list>

#include"carMath.h"

using namespace std;
using namespace ours;

bool inLineArea( ours::line* l, const CvPoint &p, int maxD) {
	pair<int, int> v1 = make_pair(l->getStart().x - p.x, l->getStart().y - p.y);
	pair<int, int> v2 = make_pair(l->getEnd().x - p.x, l->getEnd().y - p.y);
	pair<int, int> v0 = make_pair(l->getStart().x - l->getEnd().x, l->getStart().y - l->getEnd().y);

	if (COS(v0, v1)*COS(v0, v2) < 0)
		if (l->delta(p) < maxD)	return true;
		else return false;

	else return false;
}

//后退条件
bool turnBack(const CvPoint &p1, const CvPoint &p2, const CvPoint &p3, int maxA) {
	pair<int, int> v1 = make_pair(p2.x - p1.x, p2.y - p1.y);
	pair<int, int> v2 = make_pair(p3.x - p2.x, p3.y - p2.y);
	if (COS(v1,v2) < cos(PI*maxA / 180)) return true;
	else return false;
}

//转弯条件
bool turnAround(const CvPoint &p1, const CvPoint &p2, const CvPoint &p3, int minA) {
	pair<int, int> v1 = make_pair(p2.x - p1.x, p2.y - p1.y);
	pair<int, int> v2 = make_pair(p3.x - p2.x, p3.y - p2.y);
	if (COS(v1,v2) < cos(PI*minA / 180)) return true;
	else return false;
}

//删去霍夫变换后多余的线段
void deleteInvalidLines(list<CvPoint*> &lineList) {
	ours::line *t;
	bool flag;

	for (list<CvPoint*>::iterator i = lineList.begin(); i != lineList.end();) {
		flag = false;
		for (list<CvPoint*>::iterator j = lineList.begin(); j != lineList.end(); j++) {
			if (j == i) continue;
			else {
				t = new ours::line((*j)[0], (*j)[1]);
				if (inLineArea(t, (*i)[0], maxD) && inLineArea(t, (*i)[1], maxD)) {
					lineList.erase(i++);
					flag = true;
					break;
				}
				delete t;
			}
		}
		if (!flag) i++;
	}
}

//得到小车路径
void setRoute(list<CvPoint> &routePoint, CvSeq* lines, CvPoint oldStart, CvPoint oldEnd) {
	list<CvPoint*> lineList;
	for (int i = 0; i < lines->total; i++) lineList.push_back((CvPoint*)cvGetSeqElem(lines, i));

	deleteInvalidLines(lineList);

	CvPoint startPoint = lineList.front()[0], endPoint = lineList.front()[0], tempPoint;
	double distance1 = Distance(oldStart, lineList.front()[0]);
	double distance2 = Distance(oldEnd, lineList.front()[0]);
	double tempDistance;

	for (list<CvPoint*>::iterator itr = lineList.begin(); itr != lineList.end(); itr++) {
		for (int i = 0; i < 2; i++) {
			if ((tempDistance = Distance(oldStart, (*itr)[i])) < distance1) {
				distance1 = tempDistance;
				startPoint = (*itr)[i];
			}
			if ((tempDistance = Distance(oldEnd, (*itr)[i])) < distance2) {
				distance2 = tempDistance;
				endPoint = (*itr)[i];
			}
		}
	}

	routePoint.push_back(startPoint);
	bool flag = false;
	for (list<CvPoint*>::iterator itr = lineList.begin(); itr != lineList.end(); itr++) {
		for (int i = 0; i < 2; i++) {
			if ((*itr)[i] == startPoint) {
				tempPoint = (*itr)[1 - i];
				lineList.erase(itr);
				flag = true;
				break;
			}
		}
		if (flag) break;
	}

	routePoint.push_back(tempPoint);
	list<CvPoint*>::iterator tempLine = lineList.end();
	while (tempPoint != endPoint) {
		if (lineList.empty())
			break;
		if (tempLine != lineList.end()) {
			tempPoint = (tempPoint == (*tempLine)[0]) ? ((*tempLine)[1]) : (*tempLine)[0];
			routePoint.push_back(tempPoint);
			lineList.erase(tempLine);
			tempLine = lineList.end();
			continue;
		}
		tempDistance = Distance(tempPoint, lineList.front()[0]);
		int irecord = 0;
		tempLine = lineList.begin();
		for (list<CvPoint*>::iterator itr = lineList.begin(); itr != lineList.end(); itr++) {
			for (int i = 0; i < 2; i++) {
				if (Distance(tempPoint, (*itr)[i]) < tempDistance) {
					irecord = i;
					tempLine = itr;
					tempDistance = Distance(tempPoint, (*itr)[i]);
				}
			}
		}
		tempPoint = (*tempLine)[irecord];
		CvPoint nextPoint = (tempPoint == (*tempLine)[0]) ? (*tempLine)[1] : (*tempLine)[0];
		if (turnBack(routePoint.back(), tempPoint, nextPoint, maxA)) {
			tempPoint = routePoint.back();
			lineList.erase(tempLine);
			tempLine = lineList.end();
		}
		routePoint.push_back(tempPoint);
	}
	cout << "Line Deletion Finished!\n" << endl;

	list<CvPoint>::iterator begin, end;
	begin = routePoint.begin();
	begin++;
	begin++;
	end = routePoint.end();
	end--;
	for (list<CvPoint>::iterator itr = begin; itr != end;) {
		list<CvPoint>::iterator tempitr1, tempitr2;
		bool flag = false;
		tempitr1 = itr;
		tempitr1--;
		tempitr2 = tempitr1;
		tempitr2--;
		if (turnBack(*tempitr2, *tempitr1, *itr, maxA)) {
			cout << "Delete A Point " << (*itr).x << ' ' << (*itr).y << endl;
			routePoint.erase(itr++);
			flag = true;
		}
		if (!flag) {
			tempitr1 = tempitr2 = itr;
			tempitr1--;
			tempitr2++;
			if (Distance(*tempitr1, *itr) < minDP2P && !turnAround(*tempitr1, *itr, *tempitr2, minA) || Distance(*tempitr1, *itr) < 10) {
				cout << "Delete B Point " << (*itr).x << ' ' << (*itr).y << endl;
				routePoint.erase(itr++);
				flag = true;
			}
		}
		if (!flag) itr++;
	}
	cout << "Point Deletion Finished!\n" << endl;
}

#endif
