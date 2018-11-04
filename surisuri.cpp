#include "OurBilliards.hpp"

#define CFI 5
#define BUFSIZE 1024

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int MAINBALL = 0;	//1이 노란색 0이 흰색

void findColor2(Mat& image, Mat& resultImage, int min_H, int min_S, int min_V, int max_H, int max_S, int max_V, int r, int g, int b) {
	// hue is between 0 and 360
	Mat imageHSV = image.clone();
	Mat threshImg;

	vector<cv::Vec3f> v3fCircles;
	Scalar lower(min_H, min_S, min_V);
	Scalar upper(max_H, max_S, max_V);

	cvtColor(imageHSV, imageHSV, CV_BGR2HSV);
	resultImage = Mat::zeros(image.rows, image.cols, image.type());

	inRange(imageHSV, lower, upper, resultImage);

	//erode(resultImage, resultImage, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	dilate(resultImage, resultImage, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	erode(resultImage, resultImage, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	//dilate(resultImage, resultImage, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

	HoughCircles(resultImage, v3fCircles, CV_HOUGH_GRADIENT, 2, resultImage.rows / 16, 40, 20, 9, 11);


	for (size_t i = 0; i < v3fCircles.size(); i++)
	{
		Vec3i c = v3fCircles[i];
		//circle(image, Point(c[0], c[1]), c[2], Scalar(r, g, b), 2, LINE_AA);
	}
}

int main(int argc, char** argv) {


	VideoCapture inputVideo;
	//inputVideo.open(0);

	inputVideo.open(0);

	if (!inputVideo.isOpened()) {
		cout << "Could not open webcam." << endl;
		return -1;
	}
	inputVideo.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
	inputVideo.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	inputVideo.set(CV_CAP_PROP_FRAME_HEIGHT, 1280);

	Mat src;
	Mat xx;
	Rect rect(45, 40, 555, 280);
	int cushionCount = 0;

	Billiards billiards;
	Ball ball_yellow;
	Ball ball_red;
	Ball ball_white;
	Ball &ball_main = ball_white;
	Ball &ball_sub = ball_yellow;
	Mat fra;
	int flag = 0;
	Ball mainBall;
	Ball sub1Ball;
	bool collisionSuccess = true, cushionSuccess = true;

	int prev_cushion_state = 0;
	int cur_cushion_state = 0;

	int timer = INT_MAX;
	int temp = 50;


	WSADATA      wsaData;
	SOCKET      hSocket;
	SOCKADDR_IN servAddr;

	int port = 8888;
	char   *message = "1";

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (hSocket == INVALID_SOCKET)
		ErrorHandling("hSocket() error!");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	inet_pton(AF_INET, "192.168.0.14", &servAddr.sin_addr.s_addr);   // 현재 PC IP주소 사용


	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("connect() error!");
	}


	for (;;)
	{
		inputVideo >> src;

		//cout << billiards.count1 << " " << billiards.count2 << endl;
		if (src.empty()) 	break;
		resize(src, src, Size(), 0.5, 0.5, CV_INTER_AREA);
		src = src(rect);
		//imshow("Video1", src);
		Mat res1;
		Mat res2;
		Mat res3;
		//각 공의 정보 저장
		/*findColor2(src, res1, 116, 153, 102, 179, 255, 255, 0, 0, 255);
		findColor2(src, res2, 5, 20, 22, 74, 255, 255, 200, 0, 0);
		findColor2(src, res3, 0, 0, 148, 109, 18, 152, 0, 0, 0);*/

		billiards.A_findColor(src, 116, 114, 102, 179, 255, 255, 0, 0, 255, ball_red);

		billiards.A_findColor(src, 5, 20, 22, 74, 255, 255, 200, 0, 0, ball_yellow);

		billiards.A_findColor(src, 0, 0, 148, 179, 102, 152, 0, 0, 0, ball_white);

		/*	billiards.A_findColor(src, 0, 0, 80, 90, 90, 255,
		0, 0, 255, ball_red);
		billiards.A_findColor(src, 0, 50, 50, 100, 210, 210,
		0, 0, 0, ball_yellow);
		billiards.A_findColor(src, 100, 100, 100, 255, 255, 255,
		255, 160, 0, ball_white);*/


		rectangle(src, Rect(12, 12, 525, 260), Scalar(0, 0, 0));
		//Ball _main = ball_white;
		if (flag == 0) {
			billiards.collision(src, ball_yellow, ball_white, ball_red);
			if (billiards.Collision_Success()) {
				timer = temp;
			}

			//쿠션 성공여부 판단40, 30, 560, 300
			if (billiards.collisionWithWall(src, Rect(12, 13, 525, 260), ball_yellow)) {
				cur_cushion_state = 1;
				if (prev_cushion_state == 0) {
					cushionCount++;
					cout << cushionCount << endl;
					prev_cushion_state = 1;
					timer = temp;
				}
			}
			else {
				cur_cushion_state = 0;
				prev_cushion_state = 0;
			}
		}
		else {
			billiards.collision(src, ball_white, ball_yellow, ball_red);
			if (billiards.Collision_Success()) {
				timer = temp;
			}

			//쿠션 성공여부 판단40, 30, 560, 300
			if (billiards.collisionWithWall(src, Rect(12, 13, 525, 260), ball_white)) {
				cur_cushion_state = 1;
				if (prev_cushion_state == 0) {
					cushionCount++;
					cout << cushionCount << endl;
					prev_cushion_state = 1;
					timer = temp;
				}
			}
			else {
				cur_cushion_state = 0;
				prev_cushion_state = 0;
			}
		}
		//imshow("Video1", src);
		/*imshow("res", res1);
		imshow("yellow", res2);
		imshow("white", res3);*/
		//imshow("Vx", fra);


		/*if (billiards.is3CushionSuccess(&cushionCount)) {
		message = "1";
		printf("Cushion : %s\n", message);
		send(hSocket, message, sizeof(message), 0);
		}
		if (billiards.Collision_Success()) {
		message = "0";
		printf("Collision : %s\n", message);
		send(hSocket, message, sizeof(message), 0);
		}
		if (billiards.is3CushionSuccess(&cushionCount) && billiards.Collision_Success()) {
		message = "0";
		printf("Cushion&Collision : %s\n", message);
		send(hSocket, message, sizeof(message), 0);
		}*/

		char c = (char)waitKey(1);
		if (c == 27)
			break;

		//// 터ㅓㅓㅓㅓㅓㅓ언
		timer--;
		if (timer == 0) {
			cout << "턴 끝끝끝" << endl;
			//보내고
			if (billiards.is3CushionSuccess(&cushionCount) && billiards.Collision_Success()) { //쿠션 성공여부
				cout << "1점보내기" << endl;
				message = "0";
				send(hSocket, message, sizeof(message), 0);
			}
			else {
				cout << "실패보내기" << endl;
				message = "1";
				send(hSocket, message, sizeof(message), 0);
				flag += 1;
				flag = flag % 2;
			}

			timer = INT_MAX;
		}


	}
	/*
	if (billiards.is3CushionSuccess(cushionCount)) { //쿠션 성공여부
	cout << "cusionSuccess" << endl;
	}
	if (billiards.Collision_Success()) {
	cout << "collisionSuccess" << endl;
	}
	*/
	cushionCount = 0;
	//getchar();
	closesocket(hSocket);
	WSACleanup();
	return 0;
}
