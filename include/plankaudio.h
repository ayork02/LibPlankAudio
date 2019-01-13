#pragma once

#include <iostream>
#include <string>
#include <portaudio.h>
#include <sndfile.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <csignal>
#include <boost/thread.hpp>

class Audio
{
protected:
	double duration; // duration in seconds
	PaStreamParameters outputParams;
	PaError paError;
	SNDFILE *sndFile;
	SF_INFO sfInfo;
	PaStream *stream;
	double currentTime;
	bool paused = false;

	Audio(const char* file); // opens audio device
	~Audio();
	virtual void play(unsigned short pos = 0);
	virtual void printSpecs();
	virtual void stop();
	virtual void pause();
public:
	void printDuration();
	double getTime();
};

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

inline PaSampleFormat get_sample_format(int sf_format);