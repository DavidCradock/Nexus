#pragma once
#include "../precompiled_header.h"
#include "../core/archiveData.h"
#include "../core/singleton.h"


namespace Nexus
{

	// File access via zip archive files.
	//
	// This enables you to decompress data which is contained within a compressed ZIP archive file.
	// Here's an example of this class's usage...
	// Say you have a zip file called data.zip and within that file is a compressed file called cat.tga, you'd
	// decompress and gain access to cat.tga by doing the following...
	//
	// // Obtain pointer to the SCArchive class
	// SCArchive *pArchive = SCArchive::getPointer();
	// // Now add the zip file to SCArchive so that it knows with zip files to search for.
	// // The zip file has no password in the next line...
	// pArchive->addArchive("data.zip");
	// // If the zip file did have a password, for example "secret", then you'd use this instead...
	// // Otherwise, file extraction would fail.
	// pArchive->addArchive("data.zip", "secret");
	//
	// // Now to attempt to find and decompress the file to a ArchiveData object...
	// ArchiveData theDecompressedFileData;
	// if (pArchive->loadFile("cat.tga", theDecompressedFileData))
	// {
	// 	// Decompression was successful
	// 	// theDecompressedFileData now holds the uncompressed data of "cat.tga"
	// }
	class ManagerArchives : public Singleton<ManagerArchives>
	{
	public:
		ManagerArchives();

		// Add a zip archive to the search list.
		// Adds a zip file archive to the search list which, when calling openFile(), will be searched
		// in when attempting to locate and uncompress a file. Multiple zip file archives may be added.
		// strFilename - The filename of the archive. For example "data/sprites.zip"
		// strPassword - If the zip file is password protected, passing the password here will enable access.
		// returns If the archive couldn't be added, false is returned, else true.
		bool addArchive(const std::string& strFilename, const std::string& strPassword = std::string("NoPassword"));

		// Removes an archive from the search list.
		// Allows you to remove from the search list, any previously added zip file archives.
		// strFilename - The archive file's filename to be removed
		// returns If the named archive file to be removed due to not being previously added, then false is returned.
		bool removeArchive(const std::string& strFilename);

		// Removes all archives from the list
		// This is a quick way of removing all previously added zip file archives from the search list.
		void removeAllArchives(void);

		// Decompress a file from a zip file into memory for access.
		// Searches within each added archive for the given filename to check if it exists and decompresses it to a ArchiveData object.
		// If the file was found, you can then use the getDatasize and getData methods of the ArchiveData class.
		// The contents of the data stream will be deleted regardless of whether the file was found or not.
		// strFilename This is the name of the file stored inside the zip archive to find and decompress into memory.
		// streamOutput This is the object where the uncompressed data will be stored if un-compression is successful.
		// returns If the file was found and decompressed into the ArchiveData object, true is returned, else false.
		bool loadFile(const std::string& strFilename, ArchiveData& streamOutput);

	private:

		struct ArchiveFile
		{
			std::string strFilename;	// Filename of archive file
			std::string strPassword;	// Password
		};
		std::vector <ArchiveFile> archiveList; //!< Dynamic list of each registered archive's filename
	};

}