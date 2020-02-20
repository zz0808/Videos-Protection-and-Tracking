#include<iostream>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <random>
using namespace std;
using namespace cv;

// 计算向量的中值
int computeMedian(std::vector<int> elements) {

	std::nth_element(elements.begin(), elements.begin() + elements.size() / 2, elements.end());

	return elements[elements.size() / 2];
}

// 计算中值图像
cv::Mat compute_median(std::vector<cv::Mat> vec) {

	cv::Mat medianMat(vec[0].rows, vec[0].cols, CV_8UC3, cv::Scalar(0, 0, 0));

	for (int row = 0; row < vec[0].rows; row++) {
		for (int col = 0; col < vec[0].cols; col++) {

			std::vector<int> elementB;
			std::vector<int> elementG;
			std::vector<int> elementR;

			for (int imgNum = 0; imgNum < vec.size(); imgNum++) {
				int b = vec[imgNum].at<cv::Vec3b>(row, col)[0];
				int g = vec[imgNum].at<cv::Vec3b>(row, col)[1];
				int r = vec[imgNum].at<cv::Vec3b>(row, col)[2];

				elementB.push_back(b);
				elementG.push_back(g);
				elementR.push_back(r);
			}

			medianMat.at<cv::Vec3b>(row, col)[0] = computeMedian(elementB);
			medianMat.at<cv::Vec3b>(row, col)[1] = computeMedian(elementG);
			medianMat.at<cv::Vec3b>(row, col)[2] = computeMedian(elementR);

		}
	}

	return medianMat;
}

int main() {

	std::string video_file = "1.mp4";

	cv::VideoCapture cap;
	cap.open(video_file);

	if (!cap.isOpened())
		std::cerr << "Error opening video file\n";

	// 随机取25帧图像
	std::default_random_engine engine(time(0));
	std::uniform_int_distribution<int>distribution(0, cap.get(cv::CAP_PROP_FRAME_COUNT));

	std::vector<cv::Mat> frames;
	

	for (int i = 0; i < 25; i++) {
		int fid = distribution(engine);
		cap.set(cv::CAP_PROP_POS_FRAMES, fid);
      cv::Mat frame;
		cap >> frame;
		if (frame.empty())
			continue;
		frames.push_back(frame);
	}


	cv::Mat medianFrame = compute_median(frames);

	cv::imwrite("frame.jpg", medianFrame);

	cap.set(cv::CAP_PROP_POS_FRAMES, 0);

	cv::Mat grayBackImage, frame;
	cv::cvtColor(medianFrame, grayBackImage, cv::COLOR_BGR2GRAY);

	while (1) {

		cap >> frame;

		if (frame.empty()) {
			break;
		}

		cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

		cv::Mat dimage;
		cv::absdiff(frame, grayBackImage, dimage);

		cv::threshold(dimage, dimage, 20, 255, cv::THRESH_BINARY);
		
		cv::imshow("Video", dimage);

		cv::waitKey(40);
	}
	
	cap.release();

	//cv::waitKey(0);

	return 0;
}