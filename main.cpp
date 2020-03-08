//OpenCV_Recorder
//A Simple Tool to Record Video from a Webcam using OpenCV
//Created by Thomas Cope
//------------------------------
#include "Logger.hpp"
#include "VideoFeed.hpp"
#include "Benchmark.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp> 

#include <string>
#include <iostream>
#include <cstdlib>

void getHelp() {
	std::cout << "./OpenCV_Recorder <video file> <auto_pause> <resolution> <gui> <record_fps>" << std::endl;
	std::cout << "A Simple Tool to Record Video from a Webcam using Open CV" << std::endl;
	std::cout << "Created by Thomas Cope" << std::endl;
	std::cout << "\tauto_pause = Auto Pause Video Writter on start. yes/no" << std::endl;
	std::cout << "\tresolution = Specify 'auto' to auto select the max resolution or specify widthxheight (eg 1280x720), or 'no' to use the default  " << std::endl;
	std::cout << "\tgui = Show GUI? yes/no" << std::endl;
	std::cout << "\trecord_fps = What FPS should the video be recorded at? (30fps?)" << std::endl;
	std::cout << "Note: You must supply all args." << std::endl;
}

int main(int argc, char** argv) {
	Logger logger;
	Benchmark bench;
	VideoFeed *feed;
	if (argc != 6) {
		getHelp();
		return EXIT_FAILURE;
	}
	logger.Log(logger.LOG_NOTICE, "OpenCV_Recorder");
	logger.Log(logger.LOG_NOTICE, "by Thomas Cope");
	logger.setLevel(logger.LOG_DEBUG);
	cv::setUseOptimized(true);
	logger.Log(logger.LOG_DEBUG, "Built with OpenCV Version:" + cv::getBuildInformation().substr(33, 7));
	if (cv::useOptimized()) {
		logger.Log(logger.LOG_DEBUG, "Optimized OpenCV Code enabled");
	} else {
		logger.Log(logger.LOG_WARNING, "You are running a unoptimized version of OpenCV or Optimization is not available on your system.");
	}
	//Read Args
	bool autoPause = false;
	std::string resolution = argv[3];
	std::string autoPauseIN = argv[2];
	std::string showGUIIN = argv[4];
	bool showGUI = true;
	int recordFPS = std::atoi(argv[5]);
	if (autoPauseIN == "yes") {
		autoPause = true;
		logger.Log(logger.LOG_NOTICE, "Will auto Pause.");
	}
	if (showGUIIN == "no") {
		showGUI = false;
		logger.Log(logger.LOG_NOTICE, "Will NOT show GUI.");
	}
	logger.Log(logger.LOG_INFO, "Using Web Cam for Video Stream");
	feed = new VideoFeed();
	if (!feed->isOpened()) {
		logger.Log(logger.LOG_CRIT, "Failed to open Stream (File not found? Bad Permissions? Not Connected?)");
		return EXIT_FAILURE;
	} else {
		feed->feedInit();
		logger.Log(logger.LOG_INFO, "Stream Info: " + feed->getInfo());
		if (resolution == "auto") {
			logger.Log(logger.LOG_INFO, "Attempting to set Max Resolution...");
			feed->OVERRIDE_attempt_max_res();
			logger.Log(logger.LOG_INFO, "New Stream Info: " + feed->getInfo());
		} else if (resolution != "no") {
			//Set Manual Res. TODO
			int xLoc = resolution.find_first_of("x");
			int width = std::atoi(resolution.substr(0, xLoc).c_str());
			int height = std::atoi(resolution.substr(xLoc+1,resolution.length()-xLoc).c_str());
			logger.Log(logger.LOG_NOTICE, "Setting Manual Width: " + std::to_string(width));
			feed->setWidth(width);
			logger.Log(logger.LOG_NOTICE, "Setting Manual Height: " + std::to_string(height));
			feed->setHeight(height);
		}
	}
	if (feed->getFrameRate() <= 0 || feed->getFrameRate() > 60) {
		logger.Log(logger.LOG_ERR, "Webcam is Reporting a frame rate less than 0 or higher than 60! Webcam Frame Rate set to 30.");
		feed->OVERRIDE_setFrameRate(30);
	}
	char key; // Hold key press.
	const char* WIN_NAME = "Live Feed";
	logger.Log(logger.LOG_INFO, "Creating Video Writter (.avi format)...");
	std::string outputFile = argv[1];
	cv::VideoWriter outputVideo;
	cv::Size S = cv::Size(feed->getWidth(),feed->getHeight());
	int feedCodec = feed->getCodec();
	int frameRate = feed->getFrameRate();
	char feedCodecName[] = { (char)(feedCodec & 0XFF) , (char)((feedCodec & 0XFF00) >> 8),(char)((feedCodec & 0XFF0000) >> 16),(char)((feedCodec & 0XFF000000) >> 24), 0 };
	logger.Log(logger.LOG_NOTICE, "VideoWriter set to Record at: " + std::to_string(recordFPS) + " fps.");
	logger.Log(logger.LOG_NOTICE, "Writting out to: " + outputFile + " Details: " + std::to_string(feed->getWidth()) + "x" + std::to_string(feed->getHeight()) + "@" + std::to_string(recordFPS) + " Codec: " + feedCodecName);
	outputVideo.open(outputFile, feedCodec, recordFPS, S, true);
	if (!outputVideo.isOpened()) {
		logger.Log(logger.LOG_ERR, "Could not open the output video for write: " + outputFile);
		return EXIT_FAILURE;
	}
	logger.Log(logger.LOG_NOTICE, "ONLINE and Ready! Controls: 'p' to Pause, 'q' to Save and Quit.");
	logger.Log(logger.LOG_NOTICE, "Writting to File...");
	long frameCounter = 0;
	bool writeEnabled = true;
	bool fpsWarningOnce = true;
	if (autoPause) {
		writeEnabled = false;
		logger.Log(logger.LOG_INFO, "VideoWriter Auto Paused");
	}
	int stabliseWait = 0;
	while (true) {
		try {
			bench.Start();
			feed->next();
			bench.Stop();
			bench.updateFPS();
			if (writeEnabled && stabliseWait++ >= 100) {
				outputVideo.write(*feed->getCurrentFrame());
				frameCounter++;
			}
		} catch (const std::exception& e) {
			std::string tmp = "--> Exception Caught! - ";
			tmp.append(e.what());
			tmp.append(" <--");
			logger.Log(logger.LOG_CRIT, tmp);
			break;
		}
		//FPS Warning
		if ((bench.getFPS() > 1) && fpsWarningOnce && ( (bench.getFPS() <= (recordFPS - 5)) || (bench.getFPS() >= (recordFPS - 5)))) {
			std::cout << "*** Warning ***" << std::endl;
			std::cout << "You have set to write a video file at " << std::to_string(recordFPS) << " fps however your webcam is currently processing at " << bench.getFPS() << " fps. This can cause recording issues like sped up or slowed down footage. I have found that webcams runs at a different FPS depeding on what Image they are recording and the amount of light they recive. If you encounder errors please try moving the webcam arround to get the best FPS or matching the video file FPS with the webcam FPS."  << std::endl;
			std::cout << "*** Warning ***" << std::endl;
			fpsWarningOnce = false;
		}
		//Draw Hud
		cv::rectangle(*feed->getDisplayFrame(), cv::Rect(0, 0, 600, 30), cv::Scalar(0, 0, 0), -1);
		std::string HUDText = "OpenCV Recorder ~ Running at: " + std::to_string(bench.getFPS()) +  "fps - Wrote " + std::to_string(frameCounter) + " frames...";
		cv::putText(*feed->getDisplayFrame(), HUDText, cv::Point(5, 15), cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255));
		//Update Screen
		if (showGUI) {
			cv::imshow(WIN_NAME, *feed->getDisplayFrame());
		}
		key = (char)cv::waitKey(1); 
		if (key == 27 || key == 113) { //If ESC or 'q' is Pressed.
			logger.Log(logger.LOG_INFO, "User Selected to Save and Quit");
			break; //Abort
		}
		if (key == 112) { //'p' for Pause
			if (writeEnabled) {
				logger.Log(logger.LOG_INFO, "VideoWriter Paused");
				writeEnabled = false;
			} else {
				logger.Log(logger.LOG_INFO, "VideoWriter Enabled");
				writeEnabled = true;
			}
		}
	}
	//Save and Quit
	logger.Log(logger.LOG_INFO, "Closing Feeds...");
	outputVideo.release();
	delete feed;
	logger.Log(logger.LOG_INFO, "Wrote " + std::to_string(frameCounter) + " frames!");
	logger.Log(logger.LOG_INFO, "File Saved!");
	logger.Log(logger.LOG_INFO, "Application Exit!");
	return EXIT_SUCCESS;
}