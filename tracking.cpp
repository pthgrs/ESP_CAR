#include <iostream>  
#include <opencv2/core/mat.hpp>  
#include <opencv2/imgcodecs.hpp>  
#include <opencv2/imgproc.hpp>  
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <raspicam/raspicam_cv.h>

#define TRACKBAR_NAME "set the color range for find"
#define WIDTH 480
#define HEIGHT 480

using namespace cv;
using namespace std;

Mat objectHistogram;
Mat globalHistogram;

void getObjectHistogram(Mat &frame, Rect object_region) 
{
	const int channels[] = { 0, 1 };
	const int histSize[] = { 64, 64 };
	float range[] = { 0, 256 };
	const float *ranges[] = { range, range };

	// Histogram in object region
	Mat objectROI = frame(object_region);
	calcHist(&objectROI, 1, channels, noArray(), objectHistogram, 2, histSize, ranges, true, false);


	// A priori color distribution with cumulative histogram
	calcHist(&frame, 1, channels, noArray(), globalHistogram, 2, histSize, ranges, true, true);


	// Boosting: Divide conditional probabilities in object area by a priori probabilities of colors
	for (int y = 0; y < objectHistogram.rows; y++) {
		for (int x = 0; x < objectHistogram.cols; x++) {
			objectHistogram.at<float>(y, x) /= globalHistogram.at<float>(y, x);
		}
	}
	normalize(objectHistogram, objectHistogram, 0, 255, NORM_MINMAX);
}
void backProjection(const Mat &frame, const Mat &histogram, Mat &bp) {
	const int channels[] = { 0, 1 };
	float range[] = { 0, 256 };
	const float *ranges[] = { range, range };
	calcBackProject(&frame, 1, channels, objectHistogram, bp, ranges);
}
int main()
{
	raspicam::RaspiCam_Cv cap;

	//웹캡에서 캡처되는 이미지 크기를 320x240으로 지정  
	cap.set(CV_CAP_PROP_FORMAT, CV_8UC3);
	cap.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);  
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT);

	if ( !cap.open() )
	{
		cout << "Cannot open raspberry pi camera." << endl;
		return -1;
	}
	
	int LowH = 150;
	int HighH = 170; // H = Hue

	int LowS = 200; 
	int HighS = 255; // S = Saturation

	int LowV = 0;
	int HighV = 255; // V = Value
	int i = 0;

	Rect prev_rect;
	Mat bp, img_gray;
	Mat img_input, img_hsv, img_binary;

	while (true)
	{
		i++;

		int fps = cap.get(CV_CAP_PROP_FPS);

		//카메라로부터 이미지를 가져옴 
		cap.grab();
		cap.retrieve(img_input);

		//HSV로 변환
		cvtColor(img_input, img_hsv, COLOR_BGR2HSV);

		// Save the image
		imwrite("/home/pi/image/cam.jpg", img_input);

		//지정한 HSV 범위를 이용하여 영상을 이진화
		inRange(img_hsv, Scalar(LowH, LowS, LowV), Scalar(HighH, HighS, HighV), img_binary);

		//morphological opening 작은 점들을 제거
		erode(img_binary, img_binary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate( img_binary, img_binary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing 영역의 구멍 메우기 
		dilate( img_binary, img_binary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
		erode(img_binary, img_binary, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

		if ( i < 100 )
		{

			//라벨링 
			Mat img_labels,stats, centroids;  
			int numOfLables = connectedComponentsWithStats(img_binary, img_labels,   
					stats, centroids, 8, CV_32S);  

			//영역박스 그리기
			int max = -1, idx=0;
			for (int j = 1; j < numOfLables; j++) {
				int area = stats.at<int>(j, CC_STAT_AREA);
				if ( max < area ) 
				{
					max = area;
					idx = j;
				}
			}

			int left = stats.at<int>(idx, CC_STAT_LEFT);
			int top  = stats.at<int>(idx, CC_STAT_TOP);
			int width = stats.at<int>(idx, CC_STAT_WIDTH);
			int height  = stats.at<int>(idx, CC_STAT_HEIGHT);

			rectangle( img_input, Point(left,top), 
					Point(left+width,top+height),Scalar(0,0,255),1);
			Rect object_region( left, top, width, height);

			getObjectHistogram(img_hsv, object_region);
			prev_rect = object_region;
		}
		else{

			TermCriteria criteria(TermCriteria::COUNT | TermCriteria::EPS,10,1);
			backProjection(img_hsv, objectHistogram, bp);
			bitwise_and(bp, img_binary, bp);
			RotatedRect rect = CamShift(bp, prev_rect, criteria);

			Rect found_rect = rect.boundingRect();
			rectangle(img_input, found_rect, Scalar(0,0,255),3);
			if (found_rect.tl().x != 0) {
				int xGap = WIDTH/2 -(found_rect.tl().x + found_rect.br().x)/2;
				int yGap = HEIGHT/2 - (found_rect.tl().y + found_rect.br().y)/2;

				printf("xGap=%d yGap=%d area=%d\n", abs(xGap), abs(yGap), found_rect.area());

				if (abs(xGap) > WIDTH*0.25) {
					// send the x, y data to RC car
					printf("Send x,y data to RC Car\n");
				}

				if (found_rect.area() < 1000) {
					// send forward signal to RC car
					printf("move forward\n");
				}
				else if (found_rect.area() > 4000) {
					// send back go astern signal to RC car
					printf("move backward\n");
				}
			}	
		}

		imshow("Binary Video", img_binary);
		imshow("Original Video", img_input);

		//ESC키 누르면 프로그램 종료
		if (waitKey(1) == 27) 
			break; 
	}
	return 0;
}
