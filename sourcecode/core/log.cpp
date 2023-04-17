#include "precompiled_header.h"
#include "log.h"

namespace Nexus
{

	Log::Log()
	{
	}

	Log::~Log()
	{
	}

	void Log::clear(void)
	{
		std::fstream fs;
		fs.open("log.txt", std::ios::out | std::ios::trunc);
		fs.close();

		logEntry.clear();
	}

	void Log::add(const std::string& string, bool bAddNewLine)
	{
		std::fstream fs;
		fs.open("log.txt", std::ios::out | std::ios::app);
		fs << string.c_str();
		if (bAddNewLine)
			fs << "\n";
		fs.close();

		logEntry.push_back(string);
	}

	size_t Log::getNumEntries(void)
	{
		return logEntry.size();
	}

	std::string Log::getEntryText(size_t entryIndex)
	{
		if (entryIndex >= logEntry.size())
		{
			std::string err("Log::getEntryText() with given index of ");
			err.append(std::to_string(entryIndex));
			err.append(" failed as given index is invalid");
			throw std::runtime_error(err);
		}
		return logEntry[entryIndex];
	}
}