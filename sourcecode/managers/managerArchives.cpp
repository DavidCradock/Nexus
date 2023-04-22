#include "precompiled_header.h"
#include "managerArchives.h"
#include "../../dependencies/ZLib/zlib.h"
#include "../../dependencies/ZLib/unzip.h"
#include "../core/utils.h"
#include "../core/log.h"

namespace Nexus
{

	ManagerArchives::ManagerArchives()
	{
		archiveList.clear();
	}

	bool ManagerArchives::addArchive(const std::string& strFilename, const std::string& strPassword)
	{
		// Try to locate file
		if (!fileExists(strFilename))
			return false;

		// Now see if the archive has already been added
		for (unsigned int i = 0; i < archiveList.size(); ++i)
		{
			if (strFilename == archiveList[i].strFilename)
				return false;
		}

		ArchiveFile tmp;
		tmp.strFilename = strFilename;
		tmp.strPassword = strPassword;
		archiveList.push_back(tmp);
		return true;
	}

	bool ManagerArchives::removeArchive(const std::string& strFilename)
	{
		if (archiveList.size() <= 0)
			return false;

		for (unsigned int i = 0; i < archiveList.size(); ++i)
		{
			if (strFilename == archiveList[i].strFilename)
			{
				archiveList.erase(archiveList.begin() + i, archiveList.begin() + i);
				return true;
			}
		}
		// Unable to remove archive file as it wasn't found.
		return false;
	}

	void ManagerArchives::removeAllArchives(void)
	{
		archiveList.clear();
	}

	bool ManagerArchives::loadFile(const std::string& strFilename, ArchiveData& streamOutput)
	{
		streamOutput.free();

		if (archiveList.size() <= 0)
			return false;	// No archives have been registered.

		for (unsigned int i = 0; i < archiveList.size(); ++i)
		{
			unzFile zipFile = unzOpen(archiveList[i].strFilename.c_str());
			if (zipFile != NULL)// Zip file opened
			{
				if (unzLocateFile(zipFile, strFilename.c_str(), 2) == UNZ_OK) // File found
				{
					// Get size of uncompressed data
					unz_file_info fileInfo;
					unzGetCurrentFileInfo(zipFile, &fileInfo, NULL, NULL, NULL, NULL, NULL, NULL);  // Get filesize
					unsigned long ulSize = fileInfo.uncompressed_size;

					// Allocate memory of the data stream
					streamOutput.free();
					streamOutput.datasize = ulSize;
					streamOutput.pData = new unsigned char[ulSize];
					if (!streamOutput.pData)
					{
						std::string err("ManagerArchives found file in archive \"");
						err.append(strFilename);
						err.append("\" but unable to allocate memory for it.");
						Log::getPointer()->exception(err);
					}

					// Decompressed the data into the data stream
					unzOpenCurrentFile(zipFile);
					unsigned long sizeRead = unzReadCurrentFile(zipFile, (unsigned char*)streamOutput.pData, ulSize);
					if (sizeRead != ulSize)
					{
						unzCloseCurrentFile(zipFile); // Close file within archive
						unzClose(zipFile);  // Close zip file
						streamOutput.free();
						std::string err("ManagerArchives found file in archive \"");
						err.append(strFilename);
						err.append("\", also allocated memory for it, but an error occured during decompression.");
						Log::getPointer()->exception(err);
					}
					return true;	// Success!
				}
			}
		}
		// Unable to find compressed file within all registered archive files
		return false;
	}

}