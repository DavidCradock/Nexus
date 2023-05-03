#pragma once
#include "../precompiled_header.h"
#include "../core/singleton.h"
#include "../audio/audioSample.h"

namespace Nexus
{

	// Is responsible for audio.
	// Used XAudio2?
	class ManagerAudio : public Singleton<ManagerAudio>
	{
	public:
		friend class AudioSample;
		ManagerAudio();

		// Creates a new object and loads it's data in
		// iMaxNumberVoices is the total number of times this sample can be played back simultaneously
		void addSample(const std::string& name, unsigned int iMaxNumberVoices = 8);

		// Returns true if the named object exists, else false.
		bool getSampleExists(const std::string& name);

		// Removes an object.
		// Throws an exception if the named object doesn't exist.
		void removeSample(const std::string& name);

		// Plays the named sample
		// Multiple instances of this sample can be played back to iMaxNumberVoices parsed to addSample()
		void playSample(const std::string& name, float fVolume = 1.0f, float fPlaybackSpeed = 1.0f);

		// Stops playback of all instances of the named sample.
		void stopSample(const std::string& name);

		// Returns memory usage in bytes
		unsigned int getMemoryUsage(void);
	private:
		IXAudio2* pXAudio2;
		IXAudio2MasteringVoice* pMasterVoice;
		std::map<std::string, AudioSample*> mapSamples;	// Holds each named sample and it's data

	};

}