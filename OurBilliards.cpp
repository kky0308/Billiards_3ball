#include "OurBilliards.hpp"

Billiards::Billiards() {
	count1 = 0; count2 = 0;
	pre_ang = 0;
	main_pre.x = 0;
	main_pre.y = 0;
	sub1_pre.x = 0;
	sub1_pre.y = 0;
	sub2_pre.x = 0;
	sub2_pre.y = 0;
	c_flag = 0;
	c1_flag = 0;
}
void Billiards::clear() {
	count1 = 0; count2 = 0;
	pre_ang = 0;
	main_pre.x = 0;
	main_pre.y = 0;
	sub1_pre.x = 0;
	sub1_pre.y = 0;
	sub2_pre.x = 0;
	sub2_pre.y = 0;
	c_flag = 0;
	c1_flag = 0;
}
Billiards::~Billiards() { }

//main���� sub���� �Ÿ�
double Billiards::GetDistance(const Ball& p1, const Ball& p2) {
	return std::sqrt(double((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)));
}

//main���� sub���� �浹���� �Ǵ�
int Billiards::Ang(const Ball& p1, const Ball& p2) {
	r_flag = 0;
	//cout << " dddd: " << atan2f(p1.y - p2.y, p1.x - p2.x) * 180 / 3.1415f << endl;
	if (abs(p1.y - p2.y) <= 1 || abs(p1.x - p2.x) <= 1) {
		ang = 0;
		pre_ang = 0;
	}
	else {
		ang = atan2f(p1.y - p2.y, p1.x - p2.x) * 180 / 3.1415f;
	}
	//cout << "c x : " << p1.x << " c y : " << p1.y << " p x : " << p2.x << " p y : " << p2.y << " ���� : "<<ang << " ���� ��: " << abs(abs(ang) - abs(pre_ang)) << endl;

	if (abs(abs(ang) - abs(pre_ang)) > 8 && abs(abs(ang) - abs(pre_ang)) < 180) {
		r_flag = 1;
	}
	else {
		r_flag = 0;
	}

	pre_ang = ang;

	return r_flag;
}

//main���� ������ 2���� �� ��ο� �浹�ߴ��� ���������Ǵ�
void Billiards::collision(Mat& image, Ball& mainBall, Ball& sub1, Ball& sub2) {

	b_flag = Ang(mainBall, main_pre);
	s1_flag = Ang(sub1, sub1_pre);
	s2_flag = Ang(sub2, sub2_pre);

	if (GetDistance(sub1, sub1_pre) > 3 && sub1_pre.x != 0) {
		c_flag++;
	}
	if (GetDistance(sub2, sub2_pre) > 3 && sub2_pre.x != 0) {
		c1_flag++;
	}
	if ((GetDistance(mainBall, sub1) >= 18 && GetDistance(mainBall, sub1) < 30 && b_flag) || (c_flag == 1 && GetDistance(sub2, sub2_pre) < 3)) {
		if (s1_flag) {
			putText(image, "Collision", Point(sub1.x, sub1.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 4);
			count1++;
		}
	}
	if ((GetDistance(mainBall, sub2) >= 18 && GetDistance(mainBall, sub2) < 30 && b_flag) || (c1_flag == 1 && GetDistance(sub1, sub1_pre) < 3)) {
		if (s2_flag) {
			putText(image, "Collision", Point(sub2.x, sub2.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 4);
			count2++;
		}
	}


	main_pre.x = mainBall.x;
	main_pre.y = mainBall.y;

	sub1_pre.x = sub1.x;
	sub1_pre.y = sub1.y;

	sub2_pre.x = sub2.x;
	sub2_pre.y = sub2.y;

}

void Billiards::A_findColor(Mat& image, int min_H, int min_S, int min_V, int max_H, int max_S, int max_V, int r, int g, int b, Ball& ball) {
	//moving_check(ball);
	int range_count = 0;
	int low_hue1 = 0, low_hue2 = 0;
	int high_hue1 = 0, high_hue2 = 0;
	Mat imageHSV = image.clone();
	Mat resultImage;
	vector<cv::Vec3f> v3fCircles;
	Scalar lower(min_H, min_S, min_V);
	Scalar upper(max_H, max_S, max_V);

	if (min_H < 10) {
		range_count = 2;

		high_hue1 = 180;
		low_hue1 = min_H + 180;
		high_hue2 = max_H;
		low_hue2 = 0;
	}
	else if (max_H > 170) {
		range_count = 2;

		high_hue1 = min_H;
		low_hue1 = 180;
		high_hue2 = max_H - 180;
		low_hue2 = 0;
	}
	else {
		range_count = 1;

		low_hue1 = min_H;
		high_hue1 = max_H;
	}

	cvtColor(imageHSV, imageHSV, CV_BGR2HSV);
	resultImage = Mat::zeros(image.rows, image.cols, image.type());

	Mat img_mask1, img_mask2;
	inRange(imageHSV, lower, upper, resultImage);
	if (range_count == 2) {
		inRange(imageHSV, Scalar(low_hue2, 50, 50), Scalar(high_hue2, 255, 255), img_mask2);
		resultImage |= img_mask2;
	}

	dilate(resultImage, resultImage, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	dilate(resultImage, resultImage, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)));
	erode(resultImage, resultImage, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

	HoughCircles(resultImage, v3fCircles, CV_HOUGH_GRADIENT, 2, resultImage.rows / 16, 40, 22, 8, 11);

	for (size_t i = 0; i < v3fCircles.size(); i++) {
		Vec3i c = v3fCircles[i];
		//circle(image, Point(c[0], c[1]), c[2], Scalar(r, g, b), 2, LINE_AA);
		ball.x = c[0];
		ball.y = c[1];
		ball.radius = c[2];
	}
}

//���ΰ��� ����� �浹 Ƚ�� ī��Ʈ <���� �ʿ�>
bool Billiards::collisionWithWall(Mat& image, Rect wall, Ball ball) {
	float ballMinX = wall.x + ball.radius;
	float ballMinY = wall.y + ball.radius;
	float ballMaxX = wall.width - ball.radius;
	float ballMaxY = wall.height - ball.radius;

	if (ball.x < ballMinX) {
		//putText(image, "cushion", Point(ball.x, ball.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 4);
		return true;
	}
	else if (ball.x > ballMaxX) {
		//putText(image, "cushion", Point(ball.x, ball.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 4);
		return true;
	}
	// May cross both x and y bounds
	if (ball.y < ballMinY) {
		//putText(image, "cushion", Point(ball.x, ball.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 4);
		return true;
	}
	else if (ball.y > ballMaxY) {
		//putText(image, "cushion", Point(ball.x, ball.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 4);
		return true;
	}
	return false;
}

//1�� ���� ��
bool Billiards::is3CushionSuccess(int* cushionCount) {
	if (*cushionCount < 3) {
		*cushionCount = 0;
		cout << *cushionCount << endl;
		return false;
	}
	else {
		*cushionCount = 0;
		cout << *cushionCount << endl;
		return true;
	}
}

bool Billiards::Collision_Success() {
	if (count1 >= 1 && count2 >= 1) {
		count1, count2 = 0;
		return true;
	}
	else {
		count1, count2 = 0;
		return false;
	}
}

//true : ������
//false : �ȿ�����
bool Billiards::moving_check(Ball& _main) {
	//cout << "dididi" << endl;
	if (abs(main_pre.x - _main.x) > 50 && abs(main_pre.y - _main.y) > 50) {
		cout << "������" << endl;
		return true;
	}
	return false;
}