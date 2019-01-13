#pragma once

#include "main.h"

class Wave : protected Audio
{
public:
	Wave(const char* file);
	~Wave();
	void printSpecs();
	void printDuration();
	void play(unsigned short pos = 0);
	void stop();
	void pause();
	double getTime();
private:
	const char* filepath;
	float* data;
	boost::thread thread;

	void playThread(unsigned short pos);
};
