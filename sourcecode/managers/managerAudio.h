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

		// Creates a new object
		// You'll need to call loadAll() afterwards
		AudioSample* addSample(const std::string& name);

		// Returns a pointer to the named object.
		// Throws an exception if the object couldn't be found
		AudioSample* getSample(const std::string& name);

		// Returns true if the named object exists, else false.
		bool getSampleExists(const std::string& name);

		// Removes an object.
		// Throws an exception if the named object doesn't exist.
		void removeSample(const std::string& name);

		// Loads all added samples
		void loadAll(void);

		// Plays the named sample
		void playSample(const std::string& name);

		// Stops the names sample
		void stopSample(const std::string& name);
	private:
		IXAudio2* pXAudio2;
		IXAudio2MasteringVoice* pMasterVoice;
		std::map<std::string, AudioSample*> mapSamples;	// Holds each named sample
	};

}