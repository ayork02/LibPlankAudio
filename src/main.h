#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <string>
#include <portaudio.h>
#include <sndfile.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <csignal>
#include <boost/thread.hpp>

namespace PlanktonAudio
{
	class Audio
	{
	protected:
		double const m_duration; // duration in seconds
		PaStreamParameters m_outputParams;
		PaError m_paError;
		SNDFILE *m_sndFile;
		SF_INFO m_sfInfo;
		PaStream *m_stream;
		double m_currentTime;
		bool m_paused = false;

		Audio(const char* t_file); // opens audio device
		~Audio();
		virtual void play(unsigned short t_pos = 0);
		virtual void printSpecs();
		virtual void stop();
		virtual void pause();
	public:
		void printDuration();
		double getTime();
	};
}

inline PaSampleFormat get_sample_format(int t_sfFormat);

#endif