#pragma once
#include "../precompiled_header.h"
#include "singleton.h"

namespace Nexus
{
	// Holds system memory information which is set by System::getMemInfo()
	struct MemInfo
	{
		// Holds process memory usage 
		struct MemProcess
		{
			unsigned int iPageFaultCount;				// The number of page faults.
			unsigned int iPeakWorkingSetSize;			// The peak working set size. 
			unsigned int iWorkingSetSize;				// The current working set size. 
			unsigned int iQuotaPeakPagedPoolUsage;		// The peak paged pool usage. 
			unsigned int iQuotaPagedPoolUsage;			// The current paged pool usage. 
			unsigned int iQuotaPeakNonPagedPoolUsage;	// The peak nonpaged pool usage. 
			unsigned int iQuotaNonPagedPoolUsage;		// The current nonpaged pool usage. 
			unsigned int iPagefileUsage;				// The current pagefile usage. 
			unsigned int iPeakPagefileUsage;			// The peak pagefile usage.
			unsigned int iPrivateWorkingSet;			// This is what's shown in the taskmanager for each process (in bytes)
		};

		// Holds OS memory usage
		struct MemOS
		{
			unsigned int iCommitTotal;			// Total number of pages committed by the system. 
			unsigned int iCommitLimit;			// Current maximum number of page commits that can be performed by the system. This number can change if memory is added or deleted, or if pagefiles have grown, shrunk, or been added. 
			unsigned int iCommitPeak;			// Maximum number of page commit totals that have occurred since the last reboot. 
			unsigned int iPhysicalTotal;		// Total amount, in pages, of physical memory. 
			unsigned int iPhysicalAvailable;	// Amount, in pages, of physical memory available to user processes. 
			unsigned int iSystemCache;			// Total amount, in pages, of system cache memory. 
			unsigned int iKernelTotal;			// Total amount, in pages, of the sum of the paged and nonpaged kernel pools. 
			unsigned int iKernelPaged;			// Total amount, in pages, of the paged kernel pool. 
			unsigned int iKernelNonpaged;		// Total amount, in pages, of the nonpaged kernel pool. 
			unsigned int iPageSize;				// Size, in bytes, of a page. 
			unsigned int iHandleCount;			// Total number of open handles. 
			unsigned int iProcessCount;			// Total number of processes. 
			unsigned int iThreadCount;			// Total number of threads. 
		};

		MemProcess proc;		// Process information
		MemOS os;				// Operating system information
	};

	// General OS information and misc methods
	class System : public Singleton<System>
	{
	public:
		System();

		// Returns the number of physical CPU cores on the machine
		unsigned int getNumCPUs(void);

		// Returns the application's starting directory path
		inline const std::string& getRootDir(void) { return strRootDir; }

		// Get OS's logged in user name
		inline const std::string& getUsername(void) { return strUserName; }

		// Get the directory path to user's app data directory
		// In XP this is C:\Documents and settings\USER NAME\Application data\
		// This should be used to save data to which the user shouldn't really be messing with
		inline const std::string& getUserAppDataDir(void) { return strAppDataDir; }

		// Get the directory path to user's My Documents directory
		// In XP this is C:\Documents and Settings\USER NAME\My Documents\ 
		// This should be used to save data which the user is allowed to mess around with
		inline const std::string& getUserMyDocumentsDir(void) { return strMyDocumentsDir; }

		// Get the directory path to the user's desktop
		// In XP this is C:\Documents and Settings\USER NAME\Desktop\ 
		inline const std::string& getUserDesktopDir(void) { return strDesktopDir; }

		// Get OS version's name
		//inline const CText& getOSname(void){	return _mStrOSName; }

		// Returns true if SSE is supported on machine
		//inline bool getSSEsupport(void){	return _mbSSEsupport;	}

		// Returns CPU vendor name.
		inline const std::string& getCPUvendorName(void) { return strCPUVendor; }

		// Returns desktop's initial width before application started.
		inline unsigned int getDesktopWidth(void) { return iDesktopDim[0]; }

		// Returns desktop's initial height before application started.
		inline unsigned int getDesktopHeight(void) { return iDesktopDim[1]; }

		// Returns desktop's initial refresh rate in Hz
		inline unsigned int getDesktopHz(void) { return iDesktopHz; }

		// Returns desktop's initial aspect ratio.
		// Here's some typical values...\n
		// 1.33 for typical 4:3 ratio such as 1280x960 or 800x600
		// 1.25 for almost 4:3 ratio such as 1280x1024
		// 1.78 for typical 16:9 ratio such as 1600x900
		inline float getDesktopAspectRatio(void) { return fDesktopRatio; }

		// Places memory information about the process and system into the given SMemInfo structure
		// memInfo A SMemInfo structure which will hold memory info. 
		void getMemInfo(MemInfo& memInfo);

		// Returns CPU speed in MHz
		float getCPUFreq(void);

		// Given a window title for a process, returns the window's handle.
		// If the process is not running, NULL is returned, else the window handle is returned.
		// strAppWindowTitle The title of the window owned by a process. 
		HWND isProcessRunning(const std::string& strAppWindowTitle);

		// Given a window title for a process, returns true if the process's window is in the foreground, IE active
		// returns true if the process which owns the given window title is in the foreground, else false. 
		// strAppWindowTitle The title of the window which is owned by the process. 
		bool isProcessInForeground(const std::string& strAppWindowTitle);

		// Copies the given text to the system's clipboard
		// Returns true if the text was successfully copied.
		bool copyTextToClipboard(const std::string& strText);

		// Obtains the current display mode width and height
		// iWidth Holds the current width, in pixels
		// iHeight Holds the current height, in pixels 
		void getCurrentDisplayModeDims(unsigned int& iWidth, unsigned int& iHeight);

		// Synthesize the event of the left mouse button being clicked at the specified absolute screen position
		// iMousePosX The absolute screen coords where the mouse will click whereby 0 = left edge of screen.
		// iMousePosY The absolute screen coords where the mouse will click whereby 0 = top edge of screen. 
		void SynthesizeMouseLeftButton(unsigned int iMousePosX, unsigned int iMousePosY);

		// Changes the display mode to the one specified
		void changeScreenmode(int iWidth, int iHeight, int iBitdepth, int iFrequency);

	private:
		std::string strRootDir;				// App's root directory
		std::string strUserName;			// Logged in user's name
		//	std::string strOSName;			// OS version name
		std::string strCPUVendor;			// CPU Vendor name
		std::string strAppDataDir;			// Stores directory path to user's app data directory (In XP this is C:\Documents and settings\USER NAME\Application data\)
		std::string strMyDocumentsDir;		// Stores directory path to user's My Documents directory (In XP this is C:\Documents and Settings\USER NAME\My Documents\)
		std::string strDesktopDir;			// Stores directory path to user's Desktop directory (In XP this is C:\Documents and Settings\USER NAME\Desktop\)
		//	bool _mbSSEsupport;				// SSE supported by hardware and OS?
		unsigned int iDesktopDim[2];		// Desktop window's width and height
		bool bCPUFreqCalculated;			// Has the CPU frequency already been calculated?
		float fCPUFreqHz;					// Holds CPU frequency in Hz
		unsigned int iDesktopHz;			// Desktop window's initial vsync rate of the monitor
		float fDesktopRatio;				// Desktop window's aspect ratio

		// Saves the application's root directory into _mStrRootDir on app startup
		void setAppRootDir(void);

		// Sets logged in user's name
		void setUsername(void);

		// Detects desktop's dimensions and stores in _miDesktopDim. Also computes aspest ratio
		void setDesktopDims(void);

		// Detects desktop's refresh rate and stores in _miDesktopHz
		void setDesktopHz(void);

		// Sets vendor name
		void setVendorName(const char* szVendorName);

		// Calculates CPU time 
		void setCPUFreq(void);

		// Stores directory path to user's app data directory (In XP this is C:\Documents and settings\USER NAME\Application data\)
		void setUserAppDataDir(void);

		// Stores directory path to user's My Documents directory (In XP this is C:\Documents and Settings\USER NAME\My Documents\)
		void setUserMyDocumentsDir(void);

		// Stores directory path to user's Desktop directory (In XP this is C:\Documents and Settings\USER NAME\Desktop\)
		void setUserDesktopDir(void);
	};

}