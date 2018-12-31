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
	double getTime();
private:
	const char* filepath;
	short* data;
	boost::thread thread;

	void playPCM16(unsigned short pos);	
};
