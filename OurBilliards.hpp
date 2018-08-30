#include <math.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cmath>
#include <vector>
#include <thread>

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
	int r_flag, b_flag;
	Ball center;
	Billiards();
	~Billiards();

	//공의 충돌
	double GetDistance(const Ball& p1, const Ball& p2);	//공 사이의 거리
	int Ang(const Ball& p1, const Ball& p2);			//공끼리 이루는 각도
	void collision(Mat& image, Ball& mainBall, Ball& sub1, Ball& sub2);				//공의 충돌여부
	bool Collision_Success();
	//쿠션
	void collisionWithWall(Mat& image, Rect wall, Ball ball, int& cushionCount);	//쿠션과 공의 충돌여부
	bool is3CushionSuccess(int& cushionCount);										//3쿠션 성공여부
	
	//공 찾기
	void A_findColor(Mat& image, int min_H, int min_S, int min_V, int max_H, int max_S, int max_V, int r, int g, int b, Ball& ball);
};