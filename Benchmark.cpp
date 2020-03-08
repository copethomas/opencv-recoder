#include "Benchmark.hpp"

Benchmark::Benchmark() {
	Benchmark::lastTimeStep = std::time(nullptr);
}

Benchmark::~Benchmark() {
	//Nothing to Destroy
}

const double Benchmark::getAvg() {
	return Benchmark::runningAvg;
}


void Benchmark::Start() {
	Benchmark::sampleCount++;
	Benchmark::t1 = cv::getTickCount();
}

void Benchmark::Stop() {
	Benchmark::t2 = cv::getTickCount();
	Benchmark::runningAvg -= Benchmark::runningAvg / Benchmark::sampleCount;
	Benchmark::runningAvg += ((t2 - t1) / cv::getTickFrequency()) / Benchmark::sampleCount;
}

const double Benchmark::getFPS() {
	return Benchmark::currentFPS;
}

void Benchmark::updateFPS() {
	std::time_t now = std::time(nullptr);
	if (lastTimeStep != now) {
		lastTimeStep = now;
		currentFPS = FPSCounter;
		FPSCounter = 0;
	} else {
		FPSCounter++;
	}
}
