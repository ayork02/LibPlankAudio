#include "main.h"

Audio::Audio(const char* file)
{
	sndFile = sf_open(file, SFM_READ, &sfInfo);
	if (!sndFile) // TODO: Add file validation
	{
		std::cerr << "Error Opening File" << std::endl;
		exit(1);
	}
    paError = Pa_Initialize();
	if(paError != paNoError) // TODO: Make inline function
	{
		std::cerr << "Error Initialising PortAudio: " << Pa_GetErrorText(paError) << std::endl;
        Pa_Terminate();
		exit(1);
	}
	if(sfInfo.channels == !(1 || 2))
	{
		std::cerr << "Channel Number Not Supported" << std::endl;
		exit(1);
	}
	outputParams.channelCount = sfInfo.channels;
	outputParams.sampleFormat = paFloat32;
	outputParams.device = Pa_GetDefaultOutputDevice();
	outputParams.suggestedLatency = Pa_GetDeviceInfo(outputParams.device)->defaultLowOutputLatency;
	outputParams.hostApiSpecificStreamInfo = NULL;
	duration = sfInfo.frames / double(sfInfo.samplerate);
}

Audio::~Audio()
{
    sf_close(sndFile);
}

void Audio::printSpecs()
{

}

void Audio::play(unsigned short pos) // default pos = 0
{

}

void Audio::stop()
{

}

void Audio::pause()
{
	
}

double Audio::getTime()
{
	return currentTime;
}

void Audio::printDuration()
{
	static char* hms = new char[100];
	int hour = duration / 3600;
	double seconds = (int)duration % 3600;
	int min = seconds / 60;
	int sec = (int)seconds % 60;
	int millisec = (seconds - sec) * 100;
	sprintf(hms, "%d:%d:%d.%d", hour, min, sec, millisec);
	printf("Approximate Duration: %s", hms);
	delete[] hms;
}