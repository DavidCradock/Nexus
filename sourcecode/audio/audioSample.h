#pragma once
#include "../precompiled_header.h"

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

namespace Nexus
{
	class AudioSample
	{
		friend class ManagerAudio;
	public:
		AudioSample();
		~AudioSample();

		// Loads the sample data from the named file
		// If an error occurs, an exception happens
		void load(std::string filename);
	private:
		HRESULT findChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
		HRESULT readChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
		XAUDIO2_BUFFER buffer = { 0 };
		WAVEFORMATEXTENSIBLE wfx = { 0 };
		IXAudio2SourceVoice* pSourceVoice;
	};
}