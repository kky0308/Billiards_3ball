#include <math.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cmath>
#include <vector>
//#include <thread>
using namespace cv;

using namespace std;

//���� ������ ��� ����ü
struct Ball {
	int radius;		//������
	char colors;	//��
	int x;			//x��ǥ
	int y;			//y��ǥ
};


class Billiards {
public:
	float ang, pre_ang;
	int count1, count2;	//main���� sub1, sub2 �ε��� Ƚ��
	int r_flag, b_flag,s1_flag, s2_flag;
	int c_flag;
	int c1_flag,start_flag;
	Ball main_pre;
	Ball sub1_pre;
	Ball sub2_pre;
	Billiards();
	~Billiards();

	//���� �浹
	double GetDistance(const Ball& p1, const Ball& p2);	//�� ������ �Ÿ�
	int Ang(const Ball& p1, const Ball& p2);			//������ �̷�� ����
	void collision(Mat& image, Ball& mainBall, Ball& sub1, Ball& sub2);				//���� �浹����
	bool Collision_Success();
	//���
	bool collisionWithWall(Mat& image, Rect wall, Ball ball, int& cushionCount);	//��ǰ� ���� �浹����
	bool is3CushionSuccess(int& cushionCount);										//3��� ��������
	void clear();
																					//�� ã��
	void A_findColor(cv::Mat& image, int min_H, int min_S, int min_V, int max_H, int max_S, int max_V, int r, int g, int b, Ball& ball);
};