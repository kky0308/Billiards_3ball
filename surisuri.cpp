#include "OurBilliards.hpp"

#define CFI 5
int MAINBALL = 0;	//1�� ����� 0�� ���
#define BUFSIZE 1024


int main(int argc, char** argv) {


	VideoCapture inputVideo;
	inputVideo.open(0);
	//inputVideo.open(argv[1]);

	if (!inputVideo.isOpened()) {
		cout << "Could not open webcam." << endl;
		return -1;
	}
	inputVideo.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
	inputVideo.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	inputVideo.set(CV_CAP_PROP_FRAME_HEIGHT, 1280);

	Mat src;
	Rect rect(45, 40, 450, 230);
	int cushionCount = 0;

	Billiards billiards;
	Ball ball_yellow;
	Ball ball_red;
	Ball ball_white;
	Mat fra;
	Ball mainBall;
	Ball sub1Ball;
	bool collisionSuccess = true, cushionSuccess = true;
	bool start = false;
	int START_FLAG = 1;
	int s_cnt = 0;
	int e_cnt = 0;
	bool cushion_flag = false;
	int cushion_frame_ignore = CFI;
	for (;;)
	{
		inputVideo >> src;
		cout << billiards.count1 << " " << billiards.count2 << endl;
		if (src.empty()) 	break;
		resize(src, src, Size(), 0.5, 0.5, CV_INTER_AREA);
		src = src(rect);

		//�� ���� ���� ����
	
			billiards.A_findColor(src, 119, 102, 117, 179, 255, 255, 0, 255, 0, ball_red);
			//billiards.A_findColor(src, 7, 0, 167, 179, 65, 255, 255, 0, 0, ball_white);
			//findColor2(src, fra);
			billiards.A_findColor(src, 4, 51, 150, 84, 255, 255, 255, 0, 0, ball_yellow);
			billiards.A_findColor(src, 10, 0, 97, 115, 58, 255, 0, 0, 0, ball_white);


			//billiards.A_findColor(src, 119, 102, 117, 179, 255, 255, 0, 255, 0, ball_red);
			////billiards.A_findColor(src, 7, 0, 167, 179, 65, 255, 255, 0, 0, ball_white);
			////findColor2(src, fra);
			////billiards.A_findColor(src, 4, 51, 150, 84, 255, 255, 255, 0, 0, ball_yellow);
			////billiards.A_findColor(src, 10, 0, 97, 115, 58, 255, 0, 0, 0, ball_white);

			////if (collisionSuccess && cushionSuccess && start)	//�� ó���� ����Ʈ������ ��� true�̹Ƿ� start�� �� ó�� ������ ����.
			////{
			//////���� ���� 1�� ��� �κ�
			//////thread t1;
			////}
			//if (collisionSuccess == false) {	//�Ѱ��� �����ϸ� ��Į�� �ٲ�
			//if (MAINBALL == 1) {
			//MAINBALL = 0;	mainBall = ball_white;	sub1Ball = ball_yellow;
			//}
			//else {
			//MAINBALL = 1;	mainBall = ball_yellow;	sub1Ball = ball_white;
			//}
			//}

			//start = true;
			//


			billiards.collision(src, ball_yellow, ball_white, ball_red);
			//rectangle(src, Rect(5, 5, 427, 215), Scalar(0, 0, 0));
			//��� �������� �Ǵ�
			if (cushion_flag) {
				cushion_frame_ignore--;
			}
			else {
				cushion_flag = billiards.collisionWithWall(src, Rect(5, 7, 427, 215), ball_yellow, cushionCount);
				//billiards.collisionWithWall(src, Rect(50, 40, 550, 270), ball_white, cushionCount);
			}
			if (cushion_frame_ignore == 0) {
				cushion_frame_ignore = CFI;
				cushion_flag = false;
			}
			//billiards.collisionWithWall(src, Rect(50, 40, 550, 270), ball_white, cushionCount);
			if (START_FLAG == 0) {
				if (billiards.b_flag == 1) {
					s_cnt = 1;
				}
				if (s_cnt && billiards.b_flag == 0 && billiards.s1_flag == 0 && billiards.s2_flag == 0) {
					e_cnt = 1;
				}
				//cout << billiards.b_flag << " " << billiards.s1_flag << " " << billiards.s2_flag << endl;
				if (e_cnt == 1) {
					if (billiards.Collision_Success() ) {
						//&& billiards.is3CushionSuccess(cushionCount)
						cout << "collisionSuccess" << endl;						
						billiards.clear();
					}
					else
					{
						//cout << "NEXT" << endl;
					}
					s_cnt = 0;
					START_FLAG = 1;
				}
			}
			else {
				START_FLAG = 0;
			}
	
		imshow("Video", src);
		//imshow("Vx", fra);
		char c = (char)waitKey(1);
		if (c == 27)
			break;
	}

	if (billiards.is3CushionSuccess(cushionCount)) //��� ��������
		cout << "cusionSuccess" << endl;
	if (billiards.Collision_Success()) {
		cout << "collisionSuccess" << endl;
	}

	cushionCount = 0;
	//getchar();
	return 0;
}
