#include "OurBilliards.hpp"

int main(int argc, char** argv) {

	VideoCapture inputVideo;              
	inputVideo.open(argv[1]);
	if (!inputVideo.isOpened()) {
		cout << "Could not open webcam." << endl;
		return -1;
	}

	Mat src;

	Billiards billiards;
	Ball ball_yellow;
	Ball ball_red;
	Ball ball_white;

	Rect rect(0, 20, 640, 340);
	
	int cushionCount = 0;

	for (;;) {
		inputVideo >> src;         

		if (src.empty()) break;         
		src = src(rect);
		billiards.A_findColor(src, 14, 0, 76, 103, 110, 255, 0, 0, 0, ball_white);	
		billiards.A_findColor(src, 15, 102, 50, 40, 255, 255, 0, 0, 0, ball_yellow);	
		billiards.A_findColor(src, 160, 56, 0, 180, 255, 255, 0, 255, 0, ball_red);
		//cout << billiards.collision(src, ball_red, ball_yellow, ball_white) << endl;
		billiards.collision(src, ball_red, ball_yellow, ball_white);
		//rectangle(src, Rect(0, 0, 640, 340), Scalar(0, 0, 255),3);
		billiards.collisionWithWall(src, Rect(10, 20, 620, 310), ball_white, cushionCount);
		imshow("Video", src);

		char c = (char)waitKey(1);
		if (c == 27) break;
	}

	if (billiards.is3CushionSuccess(cushionCount)) {
		cout << "cusionSuccess" << endl;
	}
	cushionCount = 0;

	return 0;
}
