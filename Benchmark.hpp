#pragma once
#include <opencv2/core/core.hpp>
#include <ctime>

class Benchmark {
public:
	Benchmark();
	~Benchmark();
	const double getAvg();
	void Start();
	void Stop();
	const double getFPS();
	void updateFPS();

private:
	int64 t1, t2, sampleCount = 0;
	double runningAvg = 0;
	double currentFPS = 0;
	double FPSCounter = 0;
	std::time_t lastTimeStep;

};