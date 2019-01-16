#ifndef WAVE_H
#define WAVE_H

#include "main.h"

namespace PlanktonAudio
{
	class Wave : protected Audio
	{
	public:
		Wave(const char* t_file);
		~Wave();
		void printSpecs();
		void printDuration();
		void play(unsigned short t_pos = 0);
		void stop();
		void pause();
		double getTime();
	private:
		const char* m_filepath;
		float* m_data;
		boost::thread m_thread;

		void playThread(unsigned short t_pos);
	};
}

#endif