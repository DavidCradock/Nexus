#include "precompiled_header.h"
#include "audioManager.h"
#include "../core/log.h"


namespace Nexus
{
	AudioManager::AudioManager()
	{
		HRESULT hr;
		hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(hr))
			Log::getPointer()->exception("AudioManager() failed to initialise COM.");

		pXAudio2 = nullptr;
		if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
			Log::getPointer()->exception("AudioManager() failed to create instance of the XAudio2 engine.");

		pMasterVoice = nullptr;
		if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
			Log::getPointer()->exception("AudioManager() failed to create mastering voice.");

		// Create a volume meter effect.
//		IXAudio2Effect* volumeMeter = nullptr;
//		HRESULT hr = pXAudio2->CreateEffect(CLSID_XAudio2_VolumeMeter, nullptr, &volumeMeter);
//		if (FAILED(hr)) {
			// Handle error.
//		}
		IUnknown* pXAPO;
		hr = XAudio2CreateVolumeMeter(&pXAPO);
	}

	void AudioManager::addSample(const std::string& name, unsigned int iMaxNumberVoices)
	{
		// Make sure valid params are passed
		if (iMaxNumberVoices < 1)
			Log::getPointer()->exception("AudioManager::addSample() given iMaxNumberVoices < 1.");

		// Resource already exists?
		std::map<std::string, AudioSample*>::iterator itr = mapSamples.find(name);
		if (mapSamples.end() != itr)
		{
			return;
		}

		// If we get here, we have got to create, then add the resource
		AudioSample* pNewRes = new AudioSample();
		mapSamples[name] = pNewRes;

		// Load in the sample data
		pNewRes->load(name);

		// Create voices for the sample
		HRESULT hr;
		for (int iSample = 0; iSample < (int)iMaxNumberVoices; iSample++)
		{
			IXAudio2SourceVoice* pVoice;
			// Create a source voice by calling the IXAudio2::CreateSourceVoice method on an instance of the XAudio2 engine.
			// The format of the voice is specified by the values set in a WAVEFORMATEX structure.
			if (FAILED(hr = pXAudio2->CreateSourceVoice(&pVoice, (WAVEFORMATEX*)&pNewRes->wfx)))
				Log::getPointer()->exception("ManagerAudio::addSample() failed to create source voice.");
			pNewRes->vecVoices.push_back(pVoice);
		}
	}

	bool AudioManager::getSampleExists(const std::string& name)
	{
		std::map<std::string, AudioSample*>::iterator itr = mapSamples.find(name);
		if (itr == mapSamples.end())
			return false;
		return true;
	}

	void AudioManager::removeSample(const std::string& name)
	{
		// Resource doesn't exist in the group?
		std::map<std::string, AudioSample*>::iterator itr = mapSamples.find(name);
		if (mapSamples.end() == itr)
		{
			std::string err("AudioManager::removeSample(\"");
			err.append(name);
			err.append("\") failed because the named object couldn't be found.");
			Log::getPointer()->exception(err);
		}

		// Stop playback of all voices
		stopSample(name);

		// Destroy the resource
		delete itr->second;
		mapSamples.erase(itr);
	}

	void AudioManager::playSample(const std::string& name, float fVolume, float fPlaybackSpeed)
	{
		std::map<std::string, AudioSample*>::iterator itr = mapSamples.find(name);
		if (itr == mapSamples.end())
			Log::getPointer()->exception("AudioManager::playSample() failed to play sample as it doesn't exist.");

		HRESULT hr;
		AudioSample* audioSample = itr->second;
		IXAudio2SourceVoice* pSourceVoice = audioSample->vecVoices[audioSample->iVecVoicesIndex];
		pSourceVoice->Stop();
		pSourceVoice->FlushSourceBuffers();
		hr = pSourceVoice->SubmitSourceBuffer(&audioSample->buffer);
		if (FAILED(hr))
			Log::getPointer()->exception("AudioManager::playSample() failed. Error submitting source buffer.");

		// Set voice volume, pan and frequency
		pSourceVoice->SetVolume(fVolume);
		pSourceVoice->SetFrequencyRatio(fPlaybackSpeed);

		hr = pSourceVoice->Start(0);
		if (FAILED(hr))
			Log::getPointer()->exception("AudioManager::playSample() failed during call to pSourceVoice->Start()");
		
		// Deal with voice index
		audioSample->iVecVoicesIndex++;
		if (audioSample->iVecVoicesIndex >= audioSample->vecVoices.size())
			audioSample->iVecVoicesIndex = 0;

		/*
		// Create a source voice by calling the IXAudio2::CreateSourceVoice method on an instance of the XAudio2 engine.
		// The format of the voice is specified by the values set in a WAVEFORMATEX structure.
		HRESULT hr;
		if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&audioSample->wfx))) return;

		// Submit an XAUDIO2_BUFFER to the source voice using the function SubmitSourceBuffer.
		if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&audioSample->buffer)))
			return;

		// Use the Start function to start the source voice.
		// Since all XAudio2 voices send their output to the mastering voice by default, audio
		// from the source voice automatically makes its way to the audio device selected at initialization.
		// In a more complicated audio graph, the source voice would have to specify the voice to which its output should be sent.
		hr = pSourceVoice->Start(0);
		if (FAILED(hr))
			Log::getPointer()->exception("ManagerAudio::playSample() failed during call to pSourceVoice->Start()");
		*/
	}

	void AudioManager::stopSample(const std::string& name)
	{
		std::map<std::string, AudioSample*>::iterator itr = mapSamples.find(name);
		if (itr == mapSamples.end())
			Log::getPointer()->exception("AudioManager::stopSample() failed to play sample as it doesn't exist.");

		HRESULT hr;
		AudioSample* audioSample = itr->second;
		for (int i = 0; i < audioSample->vecVoices.size(); i++)
		{
			IXAudio2SourceVoice* pSourceVoice = audioSample->vecVoices[i];
			hr = pSourceVoice->Stop();
			if (FAILED(hr))
				Log::getPointer()->exception("AudioManager::stopSample() failed. Error calling pSourceVoice->Stop().");
			hr = pSourceVoice->FlushSourceBuffers();
			if (FAILED(hr))
				Log::getPointer()->exception("AudioManager::stopSample() failed. Error calling pSourceVoice->FlushSourceBuffers().");
		}
		audioSample->iVecVoicesIndex = 0;

	}

	int AudioManager::getNumberVoicesPlaying(const std::string& name)
	{
		std::map<std::string, AudioSample*>::iterator itr = mapSamples.find(name);
		if (itr == mapSamples.end())
			Log::getPointer()->exception("AudioManager::getNumberVoicesPlaying() failed to play sample as it doesn't exist.");

		int iNumberVoicesPlaying = 0;
		XAUDIO2_VOICE_STATE state;
		AudioSample* audioSample = itr->second;
		for (int i = 0; i < audioSample->vecVoices.size(); i++)
		{
			audioSample->vecVoices[i]->GetState(&state);
			if (state.BuffersQueued > 0)
				iNumberVoicesPlaying++;
		}
		return iNumberVoicesPlaying;
	}

	unsigned int AudioManager::getMemoryUsage(void)
	{
		XAUDIO2_PERFORMANCE_DATA pPerformanceData;// = 0;
		pXAudio2->GetPerformanceData(&pPerformanceData);
		return pPerformanceData.MemoryUsageInBytes;
	}

	float AudioManager::getVolumeLevel(void)
	{
		return 0;
	}

}

