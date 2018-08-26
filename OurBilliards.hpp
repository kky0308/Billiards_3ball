#include <math.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cmath>
#include <vector>
using namespace cv;
using namespace std;


struct Ball {
	int radius;
	char colors;
	int x;
	int y;
};


class Billiards {
public:
	float ang,pre_ang;
	int count1, count2;
	int r_flag,b_flag;
	Ball center;
	Billiards();
	~Billiards();
	double GetDistance(const Ball& p1, const Ball& p2);
	int Ang(const Ball& p1, const Ball& p2);
	void A_findColor(Mat& image, int min_H, int min_S, int min_V, int max_H, int max_S, int max_V, int r, int g, int b, Ball& ball);
	bool collision(Mat& image, Ball& red, Ball& yellow, Ball& white);
	bool collisionWithWall(Mat& image, Rect wall, Ball ball, int& cushionCount);
	bool is3CushionSuccess(int& cushionCount);
};