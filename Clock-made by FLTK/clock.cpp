//    本程序利用FLTK的GUI库实现了指针时钟和数字时钟的图形界面

//    本程序利用了两个类——数字时钟和图形时钟类来实现

//    v3.0
//——made by 叶泽林
//——2016.11.19

#include <FL/Fl.h>
#include <FL/Fl_Box.h>
#include <FL/Fl_Window.h>
#include <FL/fl_draw.H>
#include <Graph.h>
#include <Simple_window.h>
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <sstream>
#include <iostream>
#include <cmath>

using namespace Graph_lib;
using namespace std;

#define clock_R 270  //定义了一些图中构件的尺寸常量，方便修改时使用
#define scale_R_big 260
#define scale_R_small 261
#define text_R 230
#define clock_x 350
#define clock_y 375
#define hourP_len 115
#define minuteP_len 170
#define secondP_len 200
#define PI 3.1415926

//角度弧度转换函数
double dtor(double d) { return (d / 180.0*PI); }

//
//
//图形时钟类
class CLOCK{
private:
	Circle *clock, *mid;
	Line *hourP, *minuteP, *secondP, *scale[60];
	double hourtha, mintha, sectha;
	Text *t1, *t2, *t3, *t4, *t5, *t6, *t7, *t8, *t9, *t10, *t11, *t12;
public:
	//默认构造函数
	CLOCK() {}
	//析构函数
	~CLOCK();
	//画固定不动的图像（如时钟轮廓）
	void draw_stable(Simple_window *win);
	//画变动的图像（如指针）
	void draw_change(Simple_window *win, int hour, int min, int sec);
	//刷新画面前移除变动图像
	void DEDRAW(Simple_window *win);
};

//析构函数实现，析构所有指针
CLOCK::~CLOCK(){
	delete clock;
	delete hourP;
	delete minuteP;
	delete secondP;
	delete mid;
	delete t1;
	delete t2;
	delete t3;
	delete t4;
	delete t5;
	delete t6;
	delete t7;
	delete t8;
	delete t9;
	delete t10;
	delete t11;
	delete t12;
}

void CLOCK::draw_stable(Simple_window *win){
	//画钟的轮廓
	clock = new Circle(Point(clock_x, clock_y), clock_R);
	mid = new Circle(Point(clock_x, clock_y), 4);

	//画刻度
	Line_style big_scale(Line_style::solid, 4);
	Line_style small_scale(Line_style::solid, 2);
	for (int i = 0; i < 60; ++i)
		if (i % 5 == 0){
			scale[i] = new Line(Point(clock_x - clock_R*cos(dtor(i * 6)), clock_y - clock_R*sin(dtor(i * 6))),
								Point(clock_x - scale_R_big*cos(dtor(i * 6)), clock_y - scale_R_big*sin(dtor(i * 6))));
			scale[i]->set_style(big_scale);
			scale[i]->set_color(FL_RED);
		}
		else{
			scale[i] = new Line(Point(clock_x - clock_R*cos(dtor(i * 6)), clock_y - clock_R*sin(dtor(i * 6))),
								Point(clock_x - scale_R_small*cos(dtor(i * 6)), clock_y - scale_R_small*sin(dtor(i * 6))));
			scale[i]->set_style(small_scale);
		}
	
	//画钟上的文字（由于GUI库的圆并不真正“圆”，因此有些坐标需要微调，所以没有用循环实现）
	t1 = new Text(Point(clock_x + 0.5 * text_R - 3, clock_y - 0.87 * text_R + 3), "1");  t1->set_font_size(20);
	t2 = new Text(Point(clock_x + 0.87 * text_R - 3, clock_y - 0.5 * text_R + 1), "2");   t2->set_font_size(20);
	t3 = new Text(Point(clock_x + text_R - 3, clock_y + 10), "3");                         t3->set_font_size(20);
	t4 = new Text(Point(clock_x + 0.87 * text_R - 3, clock_y + 0.5 * text_R + 10), "4");   t4->set_font_size(20);
	t5 = new Text(Point(clock_x + 0.5 * text_R - 3, clock_y + 0.87 * text_R + 2), "5");   t5->set_font_size(20);
	t6 = new Text(Point(clock_x - 3, clock_y + text_R + 14), "6");                         t6->set_font_size(20);
	t7 = new Text(Point(clock_x - 0.5 * text_R - 3, clock_y + 0.87 * text_R - 2), "7");   t7->set_font_size(20);
	t8 = new Text(Point(clock_x - 0.87 * text_R - 3, clock_y + 0.5 * text_R - 2), "8");   t8->set_font_size(20);
	t9 = new Text(Point(clock_x - text_R - 4, clock_y + 11), "9");                         t9->set_font_size(20);
	t10 = new Text(Point(clock_x - 0.87 * text_R - 4, clock_y - 0.5 * text_R + 6), "10"); t10->set_font_size(20);
	t11 = new Text(Point(clock_x - 0.5 * text_R - 7, clock_y - 0.87 * text_R + 4), "11"); t11->set_font_size(20);
	t12 = new Text(Point(clock_x - 8, clock_y - text_R - 2), "12");                       t12->set_font_size(20);

	//将图像放入窗口
	win->attach(*clock);
	win->attach(*mid);
	win->attach(*t1);
	win->attach(*t2);
	win->attach(*t3);
	win->attach(*t4);
	win->attach(*t5);
	win->attach(*t6);
	win->attach(*t7);
	win->attach(*t8);
	win->attach(*t9);
	win->attach(*t10);
	win->attach(*t11);
	win->attach(*t12);
	for (int i = 0; i < 60; ++i) win->attach(*scale[i]);
}

void CLOCK::draw_change(Simple_window *win, int hour, int min, int sec){
	//计算角度
	sectha = PI / 2.0 - sec / 30.0 * PI;
	mintha = PI / 2.0 - (min * 60 + sec) / 1800.0 * PI;
	hourtha = PI / 2.0 - ((hour % 12) * 3600 + min * 60 + sec) / 21600.0 * PI;

	//创建指针对象
	hourP = new Line(Point(clock_x, clock_y), Point(clock_x + hourP_len * cos(hourtha), clock_y - hourP_len * sin(hourtha)));
	minuteP = new Line(Point(clock_x, clock_y), Point(clock_x + minuteP_len * cos(mintha), clock_y - minuteP_len * sin(mintha)));
	secondP = new Line(Point(clock_x, clock_y), Point(clock_x + secondP_len * cos(sectha), clock_y - secondP_len * sin(sectha)));

	//设置参数
	Line_style clock_edge(Line_style::solid, 6);
	clock->set_style(clock_edge);
	Line_style minP_style(Line_style::solid, 3), hourP_style(Line_style::solid, 5);
	secondP->set_color(FL_RED);
	minuteP->set_style(minP_style);
	hourP->set_style(hourP_style);

	//将对象放入窗口
	win->attach(*minuteP);
	win->attach(*secondP);
	win->attach(*hourP);
}

void CLOCK::DEDRAW(Simple_window *win){
	win->detach(*minuteP);
	win->detach(*secondP);
	win->detach(*hourP);
}

//
//
//数字时钟类
class NUM_CLOCK{
private:
	string hourstr, minstr, secstr;
	Text *hourT, *minT, *secT, *hourTomin, *minTosec;
public:
	//默认构造函数
	NUM_CLOCK() {}
	//析构函数
	~NUM_CLOCK();
	//画固定部分
	void draw_stable(Simple_window *win);
	//画变动部分
	void draw_change(Simple_window *win, int hour, int min, int sec);
	//刷新画面前移除变动图像
	void DEDRAW(Simple_window *win);
};

//析构函数实现，析构所有指针
NUM_CLOCK::~NUM_CLOCK(){
	delete hourT;
	delete minT;
	delete secT;
	delete hourTomin;
	delete minTosec;
}

void NUM_CLOCK::draw_stable(Simple_window *win){
	//创建对象
	hourTomin = new Text(Point(800, 375), ":");   hourTomin->set_font_size(80);
	minTosec = new Text(Point(930, 375), ":");    minTosec->set_font_size(80);

	//将对象放入窗口
	win->attach(*hourTomin);
	win->attach(*minTosec);
}

void NUM_CLOCK::draw_change(Simple_window *win, int hour, int min, int sec){
	//将“分”的信息从整数转化为字符串，给数字时钟用
	minstr.clear();
	hourstr.clear();
	secstr.clear();
	if (min < 10){
		minstr.push_back('0');
		minstr.push_back(min + '0');
	}
	else{
		minstr.push_back('0' + (min / 10));
		minstr.push_back('0' + (min % 10));
	}

	//将“秒”的信息从整数转化为字符串，给数字时钟用
	if (sec < 10){
		secstr.push_back('0');
		secstr.push_back(sec + '0');
	}
	else{
		secstr.push_back('0' + (sec / 10));
		secstr.push_back('0' + (sec % 10));
	}

	//将“时”的信息从整数转化为字符串，给数字时钟用
	hourstr.push_back('0' + (hour / 10));
	hourstr.push_back('0' + (hour % 10));

	hourT = new Text(Point(700, 375), hourstr); hourT->set_font_size(80);
	minT = new Text(Point(830, 375), minstr); minT->set_font_size(80);
	secT = new Text(Point(960, 375), secstr); secT->set_font_size(80);

	win->attach(*hourT);
	win->attach(*minT);
	win->attach(*secT);
}

void NUM_CLOCK::DEDRAW(Simple_window *win){
	win->detach(*hourT);
	win->detach(*minT);
	win->detach(*secT);
}

int main()
{
	Simple_window *win;
	
	win = new Simple_window(Point(100, 100), 1100, 750, "Clock");

	long long all_time;
	struct tm timeinfo;

	//获取当前时间
	time_t curtime = time(0);
	localtime_s(&timeinfo, &curtime);

	//计算总秒数
	all_time = 3600 * timeinfo.tm_hour + 60 * timeinfo.tm_min + timeinfo.tm_sec - 1;

	CLOCK myclock;
	NUM_CLOCK numclock;
	myclock.draw_stable(win);
	numclock.draw_stable(win);
	while (true){
		win->redraw();
		myclock.DEDRAW(win);
		numclock.DEDRAW(win);
		all_time++;
		int hour = all_time / 3600, min = (all_time - 3600 * hour) / 60, sec = (all_time - 3600 * hour - 60 * min);
		myclock.draw_change(win,hour,min,sec);
		numclock.draw_change(win,hour,min,sec);
		Fl::wait();
		Sleep(1000);
	}

	return Fl::run();
}
