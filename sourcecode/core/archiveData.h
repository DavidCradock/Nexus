#pragma once
#include "../precompiled_header.h"

namespace Nexus
{
	// Access and modify file data from archives or files.
	class ArchiveData
	{
		friend class ArchiveManager;
	public:
		ArchiveData();
		~ArchiveData();

		// Deallocates all memory
		void free(void);

		// Returns a pointer to the data stored within this object for reading from.
		unsigned char* getData(void) { return pData; }

		// Returns the length/size of the data within this object in bytes.
		unsigned long getDataSize(void) { return datasize; }

		// Load data into this object from a file stored inside a zip file, or a file on disk.
		//
		// This first attempts to call the SCArchive class to try to decompress the named file's data
		// from all zip files registered with the SCArchive class. If that fails (due to all the zip files
		// (if any) being added to the SCArchive class not containing the named file), then this attempts
		// to fill this objects data with data loaded from a file on disk.\n
		// The ArchiveData object will be freed regardless or whether this operation is successful or not.
		// strFilename The name of the file to load it's data into this object.
		// returns false if the loading of the data failed, due to file not being found (either in zip archive or file on disk).
		bool loadZipDisk(const std::string& strFilename, bool bReadInTextMode = false);

		// Set position to the start.
		void gotoStart(void);

		// Goto specified offset position from the start.
		//
		// iOffsetFromStart The position within the object, offset from the start
		// returns false if the given offset goes past the end, else true.
		bool gotoPos(unsigned int iOffsetFromStart);

		// Move from current position by specified amount.
		// iOffset the amount the move within the object 
		// returns false if the given offset would move the new position out of bounds of the object.
		bool move(int iOffset);

		// Get current offset from start of object
		// returns the current offset position from beginning of the data
		inline unsigned int getPos(void) { return offset; }

		// Read a type from data stream and move pointer/offset forward
		// out This is a type that will hold the data being read from the file.
		// returns false if the type could not be read.
		template <class Type> bool read(Type& out);

	private:
		unsigned char* pData;		// Pointer to the data
		unsigned long datasize;		// Size of the data
		unsigned int offset;		// Current offset in bytes
		unsigned char* pDataTemp;	// Pointer to data at current offset
	};

	template<class Type> bool ArchiveData::read(Type& out)
	{
		unsigned int iTypeSize = sizeof(Type);

		// Past end of array?
		if (offset + iTypeSize > datasize)
			return false;

		// Store value
		out = *(Type*)pDataTemp;

		// Increment pointer and position
		pDataTemp += iTypeSize;
		offset += iTypeSize;
		return true;
	}

}