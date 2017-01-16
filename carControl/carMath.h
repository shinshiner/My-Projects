#ifndef MYMATH_H_INCLUDED
#define MYMATH_H_INCLUDED

#include<cmath>

#define PI 3.141592653589793

using namespace std;

//将程序中需要用到的一些数学运算定义在ours的名空间里
namespace ours {
	double SIN(pair<int, int> v1, pair<int, int> v2) {
		double v1_abs = sqrt(v1.first*v1.first + v1.second*v1.second);
		double v2_abs = sqrt(v2.first*v2.first + v2.second*v2.second);
		return (v1.first*v2.second - v1.second*v2.first) / (v1_abs*v2_abs);
	}

	double COS(pair<int, int> v1, pair<int, int> v2) {
		double v1_abs = sqrt(v1.first*v1.first + v1.second*v1.second);
		double v2_abs = sqrt(v2.first*v2.first + v2.second*v2.second);
		return (v1.first*v2.first + v1.second*v2.second) / (v1_abs*v2_abs);
	}

	double Distance(const CvPoint &p1, const CvPoint &p2) {
		return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
	}

	bool operator==(const CvPoint &p1, const CvPoint &p2) {
		if (p1.x == p2.x && p1.y == p2.y) return true;
		else return false;
	}

	bool operator!=(const CvPoint &p1, const CvPoint &p2) {
		if (p1 == p2) return false;
		else return true;
	}

	//线类
	class line {
	private:
		double A;
		double B;
		double C;
		CvPoint startPoint;
		CvPoint endPoint;
	public:
		line(CvPoint &p1, CvPoint &p2) {
			startPoint = p1;
			endPoint = p2;
			A = p2.y - p1.y;
			B = p1.x - p2.x;
			C = p1.y*(p2.x - p1.x) - p1.x*(p2.y - p1.y);
		}
		const CvPoint & getStart() {return startPoint;}
		const CvPoint & getEnd() {return endPoint;}
		double length() {return Distance(startPoint, endPoint);}
		double delta(const CvPoint &p) {return sqrt((A*p.x + B*p.y + C)*(A*p.x + B*p.y + C) / (A*A + B*B));}
	};
}
#endif
