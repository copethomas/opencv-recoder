#pragma once
#include "CustomExceptions.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>


class VideoFeed {
public:
	VideoFeed();
	VideoFeed(std::string videoFile);
	~VideoFeed();
	bool isOpened();
	std::string getInfo();
	void next();
	cv::Mat *getDisplayFrame();
	cv::Mat *getCurrentFrame();
	cv::Mat *getCurrentFrame(bool bw);
	cv::Mat *getPreviousFrame();
	cv::Mat *getPreviousFrame(bool bw);
	cv::Mat *getDebugFrame();
	int getWidth();
	int getHeight();
	int getCodec();
	void setWidth(int newWidth);
	void setHeight(int newHeight);
	void setDebugFrame(cv::Mat frame);
	void setDisplayFrame(cv::Mat frame);
	void setPreProccessBlur(int newVal);
	void setPreProccessThreshold(int newVal);
	void OVERRIDE_setFrameRate(int newFPS);
	void OVERRIDE_attempt_max_res();
	int getPreProccessBlur();
	int getPreProccessThreshold();
	double getFrameRate();
	void feedInit();

private:
	cv::VideoCapture rawFeed;
	double WIDTH = 0;
	double HEIGHT = 0;
	double FPS = 0;
	int preProccessBlur = 1;
	int preProccessThreshold = 0;
	cv::Mat displayFrame, currentFrame, currentFrameBW, previousFrame, previousFrameBW, debugFrame; //Store one frame of the video.
};