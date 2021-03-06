#include <math.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#pragma comment (lib , "ws2_32.lib")
//#include <thread>
using namespace cv;

using namespace std;

//공의 정보를 담는 구조체
struct Ball {
	int radius;		//반지름
	char colors;	//색
	int x;			//x좌표
	int y;			//y좌표
};


class Billiards {
public:
	float ang, pre_ang;
	int count1, count2;	//main공과 sub1, sub2 부딪힌 횟수
	int r_flag, b_flag,s1_flag, s2_flag;
	int c_flag;
	int c1_flag,start_flag;
	Ball main_pre;
	Ball sub1_pre;
	Ball sub2_pre;
	Billiards();
	~Billiards();

	//공의 충돌
	double GetDistance(const Ball& p1, const Ball& p2);	//공 사이의 거리
	int Ang(const Ball& p1, const Ball& p2);			//공끼리 이루는 각도
	void collision(Mat& image, Ball& mainBall, Ball& sub1, Ball& sub2);				//공의 충돌여부
	bool Collision_Success();
	//쿠션
	bool collisionWithWall(Mat& image, Rect wall, Ball ball);	//쿠션과 공의 충돌여부
	bool is3CushionSuccess(int* cushionCount);										//3쿠션 성공여부
	void clear();
																					//공 찾기
	void A_findColor(cv::Mat& image, int min_H, int min_S, int min_V, int max_H, int max_S, int max_V, int r, int g, int b, Ball& ball);
};