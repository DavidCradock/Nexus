#include "precompiled_header.h"
#include "audioSample.h"
#include "../../core/log.h"
#include "../audioManager.h"

namespace Nexus
{
	AudioSample::AudioSample()
	{
        iVecVoicesIndex = 0;
	}

	AudioSample::~AudioSample()
	{
        for (int iVoice = 0; iVoice < vecVoices.size(); iVoice++)
        {
            vecVoices[iVoice]->DestroyVoice();
        }
        delete buffer.pAudioData;
	}

    HRESULT AudioSample::findChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
    {
        HRESULT hr = S_OK;
        if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
            Log::getPointer()->exception("AudioSample::findChunk() failed. Invalid set file pointer."); //return HRESULT_FROM_WIN32(GetLastError());

        DWORD dwChunkType;
        DWORD dwChunkDataSize;
        DWORD dwRIFFDataSize = 0;
        DWORD dwFileType;
        DWORD bytesRead = 0;
        DWORD dwOffset = 0;

        while (hr == S_OK)
        {
            DWORD dwRead;
            if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
                Log::getPointer()->exception("AudioSample::findChunk() failed. Unable to read file."); //hr = HRESULT_FROM_WIN32(GetLastError());

            if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
                Log::getPointer()->exception("AudioSample::findChunk() failed. Unable to read file."); //hr = HRESULT_FROM_WIN32(GetLastError());

            switch (dwChunkType)
            {
            case fourccRIFF:
                dwRIFFDataSize = dwChunkDataSize;
                dwChunkDataSize = 4;
                if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
                    Log::getPointer()->exception("AudioSample::findChunk() failed. Unable to read file."); //hr = HRESULT_FROM_WIN32(GetLastError());
                break;

            default:
                if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
                    Log::getPointer()->exception("AudioSample::findChunk() failed. Invalid set file pointer."); //return HRESULT_FROM_WIN32(GetLastError());
            }

            dwOffset += sizeof(DWORD) * 2;

            if (dwChunkType == fourcc)
            {
                dwChunkSize = dwChunkDataSize;
                dwChunkDataPosition = dwOffset;
                return S_OK;
            }

            dwOffset += dwChunkDataSize;

            if (bytesRead >= dwRIFFDataSize) 
                Log::getPointer()->exception("AudioSample::findChunk() failed. bytesRead >= dwRIFFDataSize."); //return S_FALSE;
        }
        return S_OK;
    }

    HRESULT AudioSample::readChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
    {
        HRESULT hr = S_OK;
        if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
            Log::getPointer()->exception("AudioSample::readChunkData() failed. Invalid set file pointer.");//return HRESULT_FROM_WIN32(GetLastError());
        DWORD dwRead;
        if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
            Log::getPointer()->exception("AudioSample::readChunkData() failed. Unable to read file."); //hr = HRESULT_FROM_WIN32(GetLastError());
        return hr;
    }

    void AudioSample::load(std::string filename)
    {
        // Open the file
        HANDLE hFile = CreateFile(
            filename.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);

        if (INVALID_HANDLE_VALUE == hFile)
            Log::getPointer()->exception("AudioSample::load() failed. Invalid file handle.");

        if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
            Log::getPointer()->exception("AudioSample::load() failed. Invalid set file pointer.");

        // Locate the 'RIFF' chunk in the audio file, and check the file type.
        DWORD dwChunkSize;
        DWORD dwChunkPosition;
        //check the file type, should be fourccWAVE or 'XWMA'
        findChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
        DWORD filetype;
        readChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
        if (filetype != fourccWAVE)
            Log::getPointer()->exception("AudioSample::load() failed. File type is not wave.");

        // Locate the 'fmt ' chunk, and copy its contents into a WAVEFORMATEXTENSIBLE structure.
        findChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
        readChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

        // Locate the 'data' chunk, and read its contents into a buffer.
        //fill out the audio data buffer with the contents of the fourccDATA chunk
        findChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
        BYTE* pDataBuffer = new BYTE[dwChunkSize];
        readChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

        // Populate an XAUDIO2_BUFFER structure.
        buffer.AudioBytes = dwChunkSize;        // size of the audio buffer in bytes
        buffer.pAudioData = pDataBuffer;        // buffer containing audio data
        buffer.Flags = XAUDIO2_END_OF_STREAM;   // tell the source voice not to expect any data after this buffer
    }
}