#include "main.h"

PlanktonAudio::Audio::Audio(const char* t_file)
{
	m_sndFile = sf_open(t_file, SFM_READ, &m_sfInfo);
	if (!m_sndFile) // TODO: Add file validation
	{
		std::cerr << "Error Opening File" << std::endl;
		exit(1);
	}
    m_paError = Pa_Initialize();
	if(m_paError != paNoError) // TODO: Make inline function
	{
		std::cerr << "Error Initialising PortAudio: " << Pa_GetErrorText(m_paError) << std::endl;
        Pa_Terminate();
		exit(1);
	}
	if(m_sfInfo.channels == !(1 || 2))
	{
		std::cerr << "Channel Number Not Supported" << std::endl;
		exit(1);
	}
	m_outputParams.channelCount = m_sfInfo.channels;
	m_outputParams.sampleFormat = paFloat32;
	m_outputParams.device = Pa_GetDefaultOutputDevice();
	m_outputParams.suggestedLatency = Pa_GetDeviceInfo(m_outputParams.device)->defaultLowOutputLatency;
	m_outputParams.hostApiSpecificStreamInfo = nullptr;
	m_duration = m_sfInfo.frames / double(m_sfInfo.samplerate);
}

PlanktonAudio::Audio::~Audio()
{
    sf_close(m_sndFile);
}

void PlanktonAudio::Audio::printSpecs()
{

}

void PlanktonAudio::Audio::play(unsigned short t_pos) // default pos = 0
{

}

void PlanktonAudio::Audio::stop()
{

}

void PlanktonAudio::Audio::pause()
{
	
}

double PlanktonAudio::Audio::getTime()
{
	return m_currentTime;
}

void PlanktonAudio::Audio::printDuration()
{
	static char* hms = new char[100];
	int hour = m_duration / 3600;
	double seconds = (int)m_duration % 3600;
	int min = seconds / 60;
	int sec = (int)seconds % 60;
	int millisec = (seconds - sec) * 100;
	sprintf(hms, "%d:%d:%d.%d", hour, min, sec, millisec);
	printf("Approximate Duration: %s", hms);
	delete[] hms;
}