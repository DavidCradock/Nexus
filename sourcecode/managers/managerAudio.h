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
		ManagerAudio();

		void playSample(AudioSample& sample);
	private:
		IXAudio2* pXAudio2;
		IXAudio2MasteringVoice* pMasterVoice;
	};

}