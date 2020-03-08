#include "VideoFeed.hpp"

VideoFeed::VideoFeed() {
	rawFeed.open(0);
}

VideoFeed::VideoFeed(std::string videoFile) {
	rawFeed.open(videoFile);
	feedInit();
}

void VideoFeed::feedInit() {
	rawFeed >> VideoFeed::displayFrame;
	VideoFeed::displayFrame.copyTo(VideoFeed::currentFrame);
	VideoFeed::displayFrame.copyTo(VideoFeed::previousFrame);
	cv::cvtColor(VideoFeed::currentFrame, VideoFeed::currentFrameBW, cv::COLOR_BGR2GRAY);
	cv::cvtColor(VideoFeed::previousFrame, VideoFeed::previousFrameBW, cv::COLOR_BGR2GRAY);
}

VideoFeed::~VideoFeed() {
	rawFeed.release();
}

bool VideoFeed::isOpened() {
	return rawFeed.isOpened();
}

std::string VideoFeed::getInfo() {
	WIDTH = rawFeed.get(CV_CAP_PROP_FRAME_WIDTH);
	HEIGHT = rawFeed.get(CV_CAP_PROP_FRAME_HEIGHT);
	FPS = rawFeed.get(CV_CAP_PROP_FPS);
	return std::to_string(WIDTH) + "x" + std::to_string(HEIGHT) + "@" + std::to_string(FPS);
}

void VideoFeed::next() {
	VideoFeed::currentFrame.copyTo(VideoFeed::previousFrame);
	VideoFeed::currentFrameBW.copyTo(VideoFeed::previousFrameBW);
	rawFeed >> VideoFeed::currentFrame;
	VideoFeed::currentFrame.copyTo(VideoFeed::displayFrame);
	if (currentFrame.empty()) {
		//throw EOFException();
		throw std::runtime_error("Current Frame Empty (video error)");
	}
	cv::cvtColor(VideoFeed::currentFrame, VideoFeed::currentFrameBW, cv::COLOR_BGR2GRAY);
}

cv::Mat *VideoFeed::getDisplayFrame() {
	return &displayFrame;
}

cv::Mat *VideoFeed::getCurrentFrame() {
	return  &currentFrame;
}

cv::Mat *VideoFeed::getCurrentFrame(bool bw) {
	if (bw) {
		return &currentFrameBW;
	} else {
		return &currentFrame;
	}
}

cv::Mat *VideoFeed::getPreviousFrame() {
	return &previousFrame;
}

cv::Mat *VideoFeed::getPreviousFrame(bool bw) {
	if (bw) {
		return &previousFrameBW;
	} else {
		return &previousFrame;
	}
}

cv::Mat *VideoFeed::getDebugFrame() {
	return &debugFrame;
}

int VideoFeed::getWidth() {
	return rawFeed.get(CV_CAP_PROP_FRAME_WIDTH);
}

int VideoFeed::getHeight() {
	return rawFeed.get(CV_CAP_PROP_FRAME_HEIGHT);
}

int VideoFeed::getCodec() {
	return CV_FOURCC('M', 'J', 'P', 'G'); //Hard Coded OpenCV Supported Codec.
	//return static_cast<int>(rawFeed.get(CV_CAP_PROP_FOURCC));
}

void VideoFeed::setWidth(int newWidth) {
	rawFeed.set(CV_CAP_PROP_FRAME_WIDTH, newWidth);
	VideoFeed::WIDTH = newWidth;
}

void VideoFeed::setHeight(int newHeight) {
	rawFeed.set(CV_CAP_PROP_FRAME_HEIGHT, newHeight);
	VideoFeed::HEIGHT = newHeight;
}

void VideoFeed::setDebugFrame(cv::Mat frame) {
	frame.copyTo(VideoFeed::debugFrame);
}

void VideoFeed::setDisplayFrame(cv::Mat frame) {
	frame.copyTo(VideoFeed::displayFrame);
}

//Internal Variables for Dynamic Changes.


void VideoFeed::setPreProccessBlur(int newVal) {
	VideoFeed::preProccessBlur = newVal;
}

void VideoFeed::setPreProccessThreshold(int newVal) {
	VideoFeed::preProccessThreshold = newVal;
}

void VideoFeed::OVERRIDE_setFrameRate(int newFPS) {
	VideoFeed::FPS = newFPS;
}

void VideoFeed::OVERRIDE_attempt_max_res() {
	rawFeed.set(CV_CAP_PROP_FRAME_WIDTH, 10000);
	rawFeed.set(CV_CAP_PROP_FRAME_HEIGHT, 10000);
}

int VideoFeed::getPreProccessBlur() {
	return preProccessBlur;
}

int VideoFeed::getPreProccessThreshold() {
	return preProccessThreshold;
}

double VideoFeed::getFrameRate() {
	return VideoFeed::FPS;
}
