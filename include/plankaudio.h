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
	double currentTime;

	Audio(const char* file); // opens audio device
	~Audio();
	virtual void play(unsigned short pos = 0);
	virtual void printSpecs();
	virtual void stop();
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
	double getTime();
private:
	const char* filepath;
	short* data;
	boost::thread thread;

	void playPCM16(unsigned short pos);	
};

inline PaSampleFormat get_sample_format(int sf_format);