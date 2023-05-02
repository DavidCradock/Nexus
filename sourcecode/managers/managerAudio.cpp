#include "precompiled_header.h"
#include "managerAudio.h"
#include "../core/log.h"


namespace Nexus
{
	ManagerAudio::ManagerAudio()
	{
		HRESULT hr;
		hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(hr))
			Log::getPointer()->exception("ManagerAudio() failed to initialise COM.");

		pXAudio2 = nullptr;
		if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
			Log::getPointer()->exception("ManagerAudio() failed to create instance of the XAudio2 engine.");

		pMasterVoice = nullptr;
		if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
			Log::getPointer()->exception("ManagerAudio() failed to create mastering voice.");
	}

	AudioSample* ManagerAudio::addSample(const std::string& name)
	{
		// Resource already exists?
		std::map<std::string, AudioSample*>::iterator itr = mapSamples.find(name);
		if (mapSamples.end() != itr)
		{
			return (AudioSample*)itr->second;
		}

		// If we get here, we have got to create, then add the resource
		AudioSample* pNewRes = new AudioSample();
		mapSamples[name] = pNewRes;

		// Find the object to return a pointer to it
		itr = mapSamples.find(name);
		return (AudioSample*)itr->second;
	}

	AudioSample* ManagerAudio::getSample(const std::string& name)
	{
		// Resource doesn't exist?
		std::map<std::string, AudioSample*>::iterator itr = mapSamples.find(name);
		if (mapSamples.end() == itr)
		{
			std::string err("ManagerAudio::get(\"");
			err.append(name);
			err.append("\"");
			err.append(" failed. As the named object doesn't exist.");
			Log::getPointer()->exception(err);
		}
		return (AudioSample*)itr->second;
	}

	bool ManagerAudio::getSampleExists(const std::string& name)
	{
		std::map<std::string, AudioSample*>::iterator itr = mapSamples.find(name);
		if (itr == mapSamples.end())
			return false;
		return true;
	}

	void ManagerAudio::removeSample(const std::string& name)
	{
		// Resource doesn't exist in the group?
		std::map<std::string, AudioSample*>::iterator itr = mapSamples.find(name);
		if (mapSamples.end() == itr)
		{
			std::string err("ManagerAudio::removeSample(\"");
			err.append(name);
			err.append("\") failed because the named object couldn't be found.");
			Log::getPointer()->exception(err);
		}

		// Destroy the resource
		delete itr->second;
		mapSamples.erase(itr);
	}

	void ManagerAudio::loadAll(void)
	{
		std::map<std::string, AudioSample*>::iterator itr = mapSamples.begin();
		// If nothing to load
		if (itr == mapSamples.end())
			return;
		while (itr != mapSamples.end())
		{
			itr->second->load(itr->first);
			itr++;
		}
	}

	void ManagerAudio::playSample(const std::string& name)
	{
		AudioSample* audioSample = getSample(name);

		// Use the Start function to start the source voice.
		// Since all XAudio2 voices send their output to the mastering voice by default, audio
		// from the source voice automatically makes its way to the audio device selected at initialization.
		// In a more complicated audio graph, the source voice would have to specify the voice to which its output should be sent.
		HRESULT hr = audioSample->pSourceVoice->Start(0);
		if (FAILED(hr))
			Log::getPointer()->exception("ManagerAudio::playSample() failed during call to pSourceVoice->Start()");

	}

	void ManagerAudio::stopSample(const std::string& name)
	{
		AudioSample* audioSample = getSample(name);
		HRESULT hr = audioSample->pSourceVoice->Stop();
		if (FAILED(hr))
			Log::getPointer()->exception("ManagerAudio::stopSample() failed during call to pSourceVoice->Stop()");

	}
}

