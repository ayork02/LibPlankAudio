#include "wave.h"

Wave::Wave(const char* file)
	: Audio(file)
{
	filepath = file;
	if((sfInfo.format & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV)
	{
		std::cerr << "Invalid WAVE File" << std::endl;
	}
}

Wave::~Wave()
{
	thread.join();
}

void Wave::play(unsigned short pos) // default pos = 0
{
	if(paused == true)
	{
		paused = false;
		pos = currentTime;
	}
	thread = boost::thread(boost::bind(&Wave::playPCM, this, pos));
}

void Wave::printSpecs()
{
	FILE *file;
	file = fopen(filepath, "rb");
	int read = 0;
	char riff[4];
	read = fread(riff, 4, 1, file);
	unsigned int overallSize; // overall size of file in bytes
	read = fread(reinterpret_cast<char*>(&overallSize), 4, 1, file);
	std::cout << "File Size: " << overallSize << " bytes" << std::endl;
	char wavChunk[4];
	read = fread(wavChunk, 4, 1, file);
	char fmtChunkMarker[4];
	if(memcmp(fmtChunkMarker, "fmt ", 4) != 0) // it works, don't ask questions
	{
		read = fread(fmtChunkMarker, 4, 1, file);
	}
	unsigned int fmtLength; // length of the format data
	read = fread(reinterpret_cast<char*>(&fmtLength), 4, 1, file);
	unsigned short fmtType; // 1-PCM, 3-IEEE float, 6-8bit A law, 7-8bit mu law
	read = fread(reinterpret_cast<char*>(&fmtType), 2, 1, file);
	std::string fmtName;
	if(fmtType == 1)
		fmtName = "PCM";
	else if(fmtType == 6)
		fmtName = "A-law";
	else if (fmtType == 7)
		fmtName = "Mu-law";
	std::cout << "Format Type: " << fmtName << std::endl;
	unsigned short numChannels; // number of channels
	read = fread(reinterpret_cast<char*>(&numChannels), 2, 1, file);
	std::cout << "Channels: " << numChannels << std::endl;
	unsigned int sampleRate; // blocks per second
	read = fread(reinterpret_cast<char*>(&sampleRate), 4, 1, file);
	std::cout << "Sample Rate: " << sampleRate << std::endl;
	unsigned int byteRate; // sampleRate * channels * bitsPerSample
	read = fread(reinterpret_cast<char*>(&byteRate), 4, 1, file);
	std::cout << "Byte Rate: " << byteRate << " Bit Rate: " << byteRate * 8 << std::endl;
	unsigned short blockAlign; // channels * bitsPerSample/8
	read = fread(reinterpret_cast<char*>(&blockAlign), 2, 1, file);
	std::cout << "Block Alignment: " << blockAlign << std::endl;
	unsigned short bitsPerSample; // 8 - 8 bits, 16 - 16 bits etc.
	read = fread(reinterpret_cast<char*>(&bitsPerSample), 2, 1, file);
	std::cout << "Bits per Sample: " << bitsPerSample << std::endl;
	char dataChunkHeader[4]; // DATA string or FLLr string	
	char subChunkLength;
	while (true) // skip over other subchunks
	{
		fread(dataChunkHeader, 4, 1, file);
		if (memcmp(dataChunkHeader, "data", 4) != 0)
		{
			read = fread(reinterpret_cast<char*>(&subChunkLength), 4, 1, file);
			fseek(file, subChunkLength, SEEK_CUR);
		}
		else
		{
			break;
		}
	}
	unsigned int dataSize; // size of chunk to be read
	read = fread(reinterpret_cast<char*>(&dataSize), 4, 1, file);
	std::cout << "Size of Data Chunk: " << dataSize << std::endl;
	unsigned int numSamples; // 8 * dataSize / (channels * bitsPerSample)
	numSamples = (8 * dataSize) / (numChannels * bitsPerSample);
	std::cout << "Number of Samples: " << numSamples << std::endl;
	unsigned int sampleSize; // size of each sample in bytes
	sampleSize = (numChannels * bitsPerSample) / 8;
	std::cout << "Size of Each Sample: " << sampleSize << " bytes" << std::endl;
	fclose(file);
}

void Wave::printDuration()
{
	Audio::printDuration();
}

double Wave::getTime()
{
	Audio::getTime();
}

void Wave::playPCM(unsigned short pos)
{
	try
	{
		sf_count_t readCount;
		sf_count_t position;
		data = (float*)malloc(sfInfo.frames * sfInfo.channels * sizeof(float));
		paError = Pa_OpenStream(
        	&stream, NULL, &outputParams,
        	sfInfo.samplerate, 1024, paClipOff,
        	NULL, NULL);
    	if(paError != paNoError)
		{
			std::cerr << "Error Opening Stream: " << Pa_GetErrorText(paError) << std::endl;
        	Pa_Terminate();
			delete[] data;
			exit(1);
		}
    	paError = Pa_StartStream(stream);
    	if(paError != paNoError)
		{
			std::cerr << "Error Starting Stream: " << Pa_GetErrorText(paError) << std::endl;
        	Pa_Terminate();
			delete[] data;
			exit(1);
		}
		sf_count_t startFrame;
		startFrame = pos * sfInfo.samplerate;
		sf_seek(sndFile, startFrame, SEEK_SET);
		while((readCount = sf_readf_float(sndFile, data, 1024)))
    	{
			boost::this_thread::interruption_point();
        	paError = Pa_WriteStream(stream, data, 1024);
			position = sf_seek(sndFile, 0, SEEK_CUR);
			currentTime = (position / double(sfInfo.frames)) * duration;
        	if (paError != paNoError)
        	{
            	std::cerr << "Error Writing to Stream: " << Pa_GetErrorText(paError) << std::endl;
        		Pa_AbortStream(stream);
				Pa_Terminate();
				delete[] data;
				exit(1);
			}
		}
		paError = Pa_CloseStream(stream);
		if(paError != paNoError)
		{
			std::cerr << "Error Closing Stream: " << Pa_GetErrorText(paError) << std::endl;
        	Pa_Terminate();
			delete[] data;
			exit(1);
		}
		delete[] data;
	}
	catch(boost::thread_interrupted Interruption)
	{
		std::cout << "Playback Stopped" << std::endl;
		Pa_CloseStream(stream);
		return;
	}
	catch(std::exception& e)
	{
		std::cerr << "Error During Playback: " << e.what() << std::endl;
		return;
	}
}

void Wave::pause()
{
	paused = true;
	thread.interrupt();
	thread.join();
}

void Wave::stop()
{
	thread.interrupt();
	thread.join();
}