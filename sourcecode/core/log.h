#pragma once
#include "../precompiled_header.h"
#include "singleton.h"

namespace Nexus
{

	// Logging of information
	class Log : public Singleton<Log>
	{
	public:
		Log();
		~Log();

		// Clear the log file
		void clear(void);

		// Add text to the log file
		void add(const std::string& string, bool bAddNewLine = true);

		// Return number of entries added via add()
		size_t getNumEntries(void);

		// Returns a string for an entry.
		// If entryIndex is out of bounds, an exception occurs
		std::string getEntryText(size_t entryIndex);
	private:

		std::vector<std::string> logEntry;	// A vector of strings, to hole each entry added by add() method
	};

}