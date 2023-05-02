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

	void ManagerAudio::playSample(AudioSample& sample)
	{
		// Create a source voice by calling the IXAudio2::CreateSourceVoice method on an instance of the XAudio2 engine.
		// The format of the voice is specified by the values set in a WAVEFORMATEX structure.
		IXAudio2SourceVoice* pSourceVoice;
		HRESULT hr;
		if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&sample.wfx))) return;

		// Submit an XAUDIO2_BUFFER to the source voice using the function SubmitSourceBuffer.
		if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&sample.buffer)))
			return;

		// Use the Start function to start the source voice.
		// Since all XAudio2 voices send their output to the mastering voice by default, audio
		// from the source voice automatically makes its way to the audio device selected at initialization.
		// In a more complicated audio graph, the source voice would have to specify the voice to which its output should be sent.
		if (FAILED(hr = pSourceVoice->Start(0)))
			return;
	}

}

/* Example threading code

#include <mutex>
#include <thread>

std::mutex mtx;

void print_hello() {
  std::unique_lock<std::mutex> lock(mtx);
  std::cout << "Hello, world!" << std::endl;
  lock.unlock();
}

int main() {
  std::thread t1(print_hello);
  std::thread t2(print_hello);
  t1.join();
  t2.join();
  return 0;
}
*/