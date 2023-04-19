#include "precompiled_header.h"
#include "archiveData.h"
#include "../managers/managerArchives.h"
#include "log.h"

namespace Nexus
{

	ArchiveData::ArchiveData()
	{
		pData = 0;
		datasize = 0;
		offset = 0;
		pDataTemp = 0;
	}

	ArchiveData::~ArchiveData()
	{
		free();
	}

	void ArchiveData::free(void)
	{
		if (pData)
		{
			delete[] pData;
			pData = 0;
		}
		datasize = 0;
		offset = 0;
		pDataTemp = 0;
	}

	bool ArchiveData::loadZipDisk(const std::string& strFilename, bool bReadInTextMode)
	{
		free();

		// First, attempt to load the file from a zip archive
		if (ManagerArchives::getPointer()->loadFile(strFilename, *this))
		{
			gotoStart();
			return true;
		}

		// If that failed, then attempt to load the data from file on disk
		FILE* f;
		if (!bReadInTextMode)
			fopen_s(&f, strFilename.c_str(), "rb");
		else
			fopen_s(&f, strFilename.c_str(), "rt");
		if (!f)
		{
			// Unable to load data as not found in any registered archives and could not be found on disk
			return false;
		}

		fseek(f, 0, SEEK_END);
		datasize = ftell(f);
		pData = new unsigned char[datasize];
		if (0 == pData)
		{
			std::string err("Failed to find file \"");
			err.append(strFilename);
			err.append("\" in zip archive, but found on disk, but failed to allocate memory for it");
			throw std::runtime_error(err);
		}
		fseek(f, 0, SEEK_SET);
		size_t amountRead = fread_s(pData, sizeof(unsigned char) * datasize, sizeof(unsigned char), datasize, f);

		if (datasize != amountRead && !bReadInTextMode)
		{
			std::string err("Failed to find file \"");
			err.append(strFilename);
			err.append("\" in zip archive, but found on disk and allocated memory for it, but there was a file I/O error.");
			throw std::runtime_error(err);
		}
		if (bReadInTextMode)
		{
			if (amountRead == 0)
			{
				gotoStart();
				fclose(f);
				return false;
			}

		}
		gotoStart();
		fclose(f);
		return true;
	}

	void ArchiveData::gotoStart(void)
	{
		offset = 0;
		pDataTemp = pData;
	}

	bool ArchiveData::gotoPos(unsigned int iOffsetFromStart)
	{
		if (iOffsetFromStart >= datasize)
			return false;
		offset = iOffsetFromStart;
		pDataTemp = pData;
		pDataTemp += sizeof(unsigned char) * offset;
		return true;
	}

	bool ArchiveData::move(int iOffset)
	{
		if (offset + iOffset >= datasize)
			return false;
		if (offset + iOffset < 0)
			return false;
		offset += iOffset;
		pDataTemp = pData;
		pDataTemp += sizeof(unsigned char) * offset;
		return true;
	}

}