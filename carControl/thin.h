#ifndef REFINE_H_INCLUDED
#define REFINE_H_INCLUDED

//细化图像

#include <vector>

using namespace std;

void thinImage(IplImage* src, IplImage* dst, int maxIterations = -1) {
	CvSize size = cvGetSize(src);
	cvCopy(src, dst);
	int count = 0;
	while (true) {
		count++;
		if (maxIterations != -1 && count > maxIterations)
			break;
		vector<pair<int, int> > mFlag;
		for (int i = 0; i < size.height; ++i) {
			for (int j = 0; j < size.width; ++j) {
				int p1 = CV_IMAGE_ELEM(dst, uchar, i, j);
				if (p1 != 1) continue;
				int p2 = (i == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j);
				int p3 = (i == 0 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j + 1);
				int p4 = (j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j + 1);
				int p5 = (i == size.height - 1 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j + 1);
				int p6 = (i == size.height - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j);
				int p7 = (i == size.height - 1 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j - 1);
				int p8 = (j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j - 1);
				int p9 = (i == 0 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j - 1);

				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) == 0)
					mFlag.push_back(make_pair(i, j));
				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6) {
					int ap = 0;
					if (p2 == 0 && p3 == 1)	++ap;
					if (p3 == 0 && p4 == 1)	++ap;
					if (p4 == 0 && p5 == 1)	++ap;
					if (p5 == 0 && p6 == 1)	++ap;
					if (p6 == 0 && p7 == 1)	++ap;
					if (p7 == 0 && p8 == 1)	++ap;
					if (p8 == 0 && p9 == 1) ++ap;
					if (p9 == 0 && p2 == 1)	++ap;

					if (ap == 1)
						if (p2*p4*p6 == 0)
							if (p4*p6*p8 == 0)
								mFlag.push_back(make_pair(i, j));
				}
			}
		}

		for (vector<pair<int, int> >::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
			CV_IMAGE_ELEM(dst, uchar, i->first, i->second) = 0;

		if (mFlag.size() == 0) break;
		else mFlag.clear();

		for (int i = 0; i < size.height; ++i) {
			for (int j = 0; j < size.width; ++j) {
				int p1 = CV_IMAGE_ELEM(dst, uchar, i, j);
				if (p1 != 1) continue;
				int p2 = (i == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j);
				int p3 = (i == 0 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j + 1);
				int p4 = (j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j + 1);
				int p5 = (i == size.height - 1 || j == size.width - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j + 1);
				int p6 = (i == size.height - 1) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j);
				int p7 = (i == size.height - 1 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i + 1, j - 1);
				int p8 = (j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i, j - 1);
				int p9 = (i == 0 || j == 0) ? 0 : CV_IMAGE_ELEM(dst, uchar, i - 1, j - 1);

				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) == 0)
					mFlag.push_back(make_pair(i, j));
				if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6) {
					int ap = 0;
					if (p2 == 0 && p3 == 1)	++ap;
					if (p3 == 0 && p4 == 1)	++ap;
					if (p4 == 0 && p5 == 1)	++ap;
					if (p5 == 0 && p6 == 1)	++ap;
					if (p6 == 0 && p7 == 1)	++ap;
					if (p7 == 0 && p8 == 1)	++ap;
					if (p8 == 0 && p9 == 1)	++ap;
					if (p9 == 0 && p2 == 1)	++ap;

					if (ap == 1)
						if (p2*p4*p8 == 0)
							if (p2*p6*p8 == 0)
								mFlag.push_back(make_pair(i, j));
				}
			}
		}

		for (vector<pair<int, int> >::iterator i = mFlag.begin(); i != mFlag.end(); ++i)
			CV_IMAGE_ELEM(dst, uchar, i->first, i->second) = 0;

		if (mFlag.size() == 0) break;
		else mFlag.clear();
	}
}

void refine(IplImage* src, IplImage* dst) {
	IplImage* pTemp = cvCloneImage(src);
	for (int i = 0; i < pTemp->height; ++i) {
		for (int j = 0; j < pTemp->width; ++j) {
			if (CV_IMAGE_ELEM(pTemp, uchar, i, j) == 255)
				CV_IMAGE_ELEM(pTemp, uchar, i, j) = 1;
		}
	}

	thinImage(pTemp, dst);

	for (int i = 0; i < dst->height; ++i)
		for (int j = 0; j < dst->width; ++j)
			if (CV_IMAGE_ELEM(dst, uchar, i, j) == 1)
				CV_IMAGE_ELEM(dst, uchar, i, j) = 255;

	cvReleaseImage(&pTemp);
}
#endif
