#include "precompiled_header.h"
#include "system.h"
#include "log.h"

namespace Nexus
{

	System::System()
	{
		_setAppRootDir();
		//	_setOSname();
		_setUsername();
		//	_setSSEsupport();
		_setDesktopDims();
		_setDesktopHz();
		_setUserAppDataDir();
		_setUserMyDocumentsDir();
		_setUserDesktopDir();
		_mbCPUFreqCalculated = false;
	}

	void System::_setAppRootDir(void)
	{
		char* szDir = NULL;
		int iBufSize = GetCurrentDirectory(0, szDir);
		szDir = new char[iBufSize];
		GetCurrentDirectory(iBufSize, szDir);
		_mStrRootDir.clear();
		_mStrRootDir.append(szDir);
		delete[] szDir;
	}

	void System::_setUsername(void)
	{
		DWORD dwBufSize = 0;
		char* szName = NULL;
		GetUserName(szName, &dwBufSize);
		szName = new char[dwBufSize];
		GetUserName(szName, &dwBufSize);
		_mStrUserName.clear();
		_mStrUserName.append(szName);
		delete[] szName;
	}

	/*
	void SCSystem::_setOSname(void)
	{
		OSVERSIONINFOEX osvi;
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		GetVersionEx((LPOSVERSIONINFO)&osvi);

		_mStrOSName = "Unknown Windows version";

		if (4 == osvi.dwMajorVersion)	// The operating system is Windows NT 4.0, Windows Me, Windows 98, or Windows 95.
		{
			if (0 == osvi.dwMinorVersion)	// Windows NT 4.0, or Windows 95
			{
				if (VER_PLATFORM_WIN32_WINDOWS == osvi.dwPlatformId)
					_mStrOSName = "Windows 95";
				else
					_mStrOSName = "Windows NT 4.0";
			}
			else if (10 == osvi.dwMinorVersion)	// Win98
				_mStrOSName = "Windows 98";
			else if (90 == osvi.dwMinorVersion)	// WinME
				_mStrOSName = "Windows ME";
		}
		else if (5 == osvi.dwMajorVersion)	// The operating system is Windows Server 2003 R2, Windows Server 2003, Windows XP, or Windows 2000.
		{
			if (0 == osvi.dwMinorVersion)	// Windows 2000
				_mStrOSName = "Windows 2000";
			else if (1 == osvi.dwMinorVersion)	// Windows XP
				_mStrOSName = "Windows XP";
			else if (2 == osvi.dwMinorVersion)	// Windows 2003 Server or R2
			{
				if (GetSystemMetrics(SM_SERVERR2) == 0)
					_mStrOSName = "Windows Server 2003";
				else
					_mStrOSName = "Windows Server 2003 R2";
			}
		}
		else if (6 == osvi.dwMajorVersion)	// The operating system is Windows Vista, Windows Server 2008, Windows Server 2008 R2 or Windows 7.
		{
			if (0 == osvi.dwMinorVersion)	// Windows Vista or Windows Server 2008
			{
				if (osvi.wProductType == VER_NT_WORKSTATION)
					_mStrOSName = "Windows Vista";
				else
					_mStrOSName = "Windows Server 2008";
			}
			else if (1 == osvi.dwMinorVersion)	// Windows 7 or Windows Server 2008 R2
			{
				if (osvi.wProductType == VER_NT_WORKSTATION)
					_mStrOSName = "Windows 7";
				else
					_mStrOSName = "Windows Server 2008 R2";
			}
		}
	}
	*/

	/*
	void SCSystem::_setSSEsupport(void)
	{
		bool bOSsupportsSSE = true;
		bool bCPUsupportSSE = false;

		// Check CPU for SSE support & get vendor name
		char szVendorName[13];	SecureZeroMemory(szVendorName, sizeof(char)*13);
		char *psz = szVendorName;
		__try
		{
			_asm
			{
				// Vendor name
				mov		eax,		0				// Moving zero into eax makes the CPUID instruction return vendor name in ebx,edx,ecx
				CPUID								// Perform CPUID
				mov		esi,		psz				// Move pointer to chVendorname into esi register
				mov		[esi],		ebx				// Move first 4 bytes into psz
				mov		[esi+4],	edx				// Move next 4
				mov		[esi+8],	ecx				// And again

				// SSE support
				mov		eax,		1				// Moving one into eax makes the CPUID instruction return features bits in edx
				CPUID								// Perform CPUID
				test	edx,		02000000h		// SSE supported?
				jz		_NOSSE						// If the above test failed, jump to SSE not supported label, else...
				mov		[bCPUsupportSSE], 1			// set CPU support bool to true, then jump to end
				jmp		_END
	_NOSSE:
				mov		[bCPUsupportSSE], 0			// No SSE :(
	_END:
			}
		}
		__except(EXCEPTION_EXECUTE_HANDLER)	// Handle exception, continue afterwards.
		{
			if (_exception_code() == STATUS_ILLEGAL_INSTRUCTION)
				bCPUsupportSSE = false;
			bCPUsupportSSE = false;
		}

		_setVendorName(szVendorName);

		// Attempt an SSE instruction, using Microsoft's extension to C++ exception handling so that we can recover from the exception, instead
		// of process termination occurring
		__try
		{
			_asm xorps xmm0, xmm0
		}
		__except(EXCEPTION_EXECUTE_HANDLER)	// Handle exception, continue afterwards.
		{
			if (_exception_code() == STATUS_ILLEGAL_INSTRUCTION)
				bOSsupportsSSE = false;
			bOSsupportsSSE = false;
		}

		if (bOSsupportsSSE && bCPUsupportSSE)
			_mbSSEsupport = true;
		else
			_mbSSEsupport = false;
	}
	*/

	void System::_setVendorName(const char* _mStrCPUVendor)
	{
		_mStrCPUVendor = _mStrCPUVendor;
	}

	void System::_setDesktopDims(void)
	{
		//	RECT rcDesktop;
		//	GetClientRect(GetDesktopWindow(), &rcDesktop);
		//	_miDesktopDim[0] = rcDesktop.right - rcDesktop.left;
		//	_miDesktopDim[1] = rcDesktop.bottom - rcDesktop.top;
		DEVMODE dmCurrent;
		EnumDisplaySettings(NULL,	// Current device which this thread is running on
			ENUM_CURRENT_SETTINGS,
			&dmCurrent);
		_miDesktopDim[0] = dmCurrent.dmPelsWidth;
		_miDesktopDim[1] = dmCurrent.dmPelsHeight;

		// Compute aspect ratio
		if (_miDesktopDim[1] < 1)
		{
			std::string err("SCSystem::_setDesktopDims() Has wrongly detected desktop dimensions (");
			err.append(std::to_string(_miDesktopDim[0]));
			err.append("x");
			err.append(std::to_string(_miDesktopDim[1]));
			err.append(")");
			Log::getPointer()->exception(err);
		}

		_mfDesktopRatio = float(_miDesktopDim[0]) / float(_miDesktopDim[1]);
	}

	void System::_setDesktopHz(void)
	{
		DEVMODE dmCurrent;
		EnumDisplaySettings(NULL,	// Current device which this thread is running on
			ENUM_CURRENT_SETTINGS,
			&dmCurrent);
		_miDesktopHz = dmCurrent.dmDisplayFrequency;
	}

	unsigned int System::getNumCPUs(void)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		return unsigned int(si.dwNumberOfProcessors);
	}

	void System::getMemInfo(SMemInfo& memInfo)
	{
		SecureZeroMemory(&memInfo, sizeof(SMemInfo));

		HANDLE hAppProcess = GetCurrentProcess();
		if (!hAppProcess)	// Found application process handle?
			return;

		// Retreive and store process mem usage
		PROCESS_MEMORY_COUNTERS_EX pmc;
		if (GetProcessMemoryInfo(hAppProcess, (PPROCESS_MEMORY_COUNTERS)&pmc, sizeof(PROCESS_MEMORY_COUNTERS_EX)))
		{
			memInfo.proc.iPageFaultCount = (unsigned int)pmc.PageFaultCount;
			memInfo.proc.iPagefileUsage = (unsigned int)pmc.PagefileUsage;
			memInfo.proc.iPeakPagefileUsage = (unsigned int)pmc.PeakPagefileUsage;
			memInfo.proc.iPeakWorkingSetSize = (unsigned int)pmc.PeakWorkingSetSize;
			memInfo.proc.iQuotaNonPagedPoolUsage = (unsigned int)pmc.QuotaNonPagedPoolUsage;
			memInfo.proc.iQuotaPagedPoolUsage = (unsigned int)pmc.QuotaPagedPoolUsage;
			memInfo.proc.iQuotaPeakNonPagedPoolUsage = (unsigned int)pmc.QuotaPeakNonPagedPoolUsage;
			memInfo.proc.iQuotaPeakPagedPoolUsage = (unsigned int)pmc.QuotaPeakPagedPoolUsage;
			memInfo.proc.iWorkingSetSize = (unsigned int)pmc.WorkingSetSize;
			memInfo.proc.iPrivateWorkingSet = (unsigned int)pmc.PeakWorkingSetSize;
		}

		PERFORMACE_INFORMATION pin;
		if (GetPerformanceInfo(&pin, sizeof(PERFORMACE_INFORMATION)))
		{
			memInfo.os.iCommitLimit = (unsigned int)pin.CommitLimit;
			memInfo.os.iCommitPeak = (unsigned int)pin.CommitPeak;
			memInfo.os.iCommitTotal = (unsigned int)pin.CommitTotal;
			memInfo.os.iHandleCount = (unsigned int)pin.HandleCount;
			memInfo.os.iKernelNonpaged = (unsigned int)pin.KernelNonpaged;
			memInfo.os.iKernelPaged = (unsigned int)pin.KernelPaged;
			memInfo.os.iKernelTotal = (unsigned int)pin.KernelTotal;
			memInfo.os.iPageSize = (unsigned int)pin.PageSize;
			memInfo.os.iPhysicalAvailable = (unsigned int)pin.PhysicalAvailable;
			memInfo.os.iPhysicalTotal = (unsigned int)pin.PhysicalTotal;
			memInfo.os.iProcessCount = (unsigned int)pin.ProcessCount;
			memInfo.os.iSystemCache = (unsigned int)pin.SystemCache;
			memInfo.os.iThreadCount = (unsigned int)pin.ThreadCount;
		}
	}

	float System::getCPUFreq(void)
	{
		_setCPUFreq();
		return _mfCPUFreqHz;
	}

	void System::_setCPUFreq(void)
	{
		if (_mbCPUFreqCalculated)
			return;
		_mbCPUFreqCalculated = true;

		DWORD iSize = sizeof(DWORD);
		DWORD iMHz;
		HKEY hKey;
		if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", NULL, KEY_QUERY_VALUE, &hKey))
		{
			_mfCPUFreqHz = -1;
			return;
		}
		RegQueryValueEx(hKey, "~MHz", NULL, NULL, (LPBYTE)&iMHz, &iSize);
		RegCloseKey(hKey);
		_mfCPUFreqHz = float(iMHz);

		/* Grrr, not accurate
		CTimer timer;
		float fTime1 = 0.0f;
		float fTime2 = 0.0f;
		unsigned int iTicks1;
		unsigned int iTicks2;
		int iNotFirstLoop = 0;
		timer.update();
		for (;;)
		{
			fTime1 += timer.getST();
			timer.update();

			if (fTime1 > 0.0f && iNotFirstLoop>0)
			{
				__asm
				{
					xor	eax, eax
					xor	ebx, ebx
					xor	ecx, ecx
					xor	edx, edx
					_emit	0x0f						// CPUID
					_emit	0xa2
					_emit	0x0f						// RTDSC
					_emit	0x31
					mov	[iTicks1], eax
				}
				break;
			}
			++iNotFirstLoop;
		}

		iNotFirstLoop = 0;
		timer.update();
		for (;;)
		{
			fTime2 += timer.getST();
			timer.update();
			if (fTime2 > 1.0f && iNotFirstLoop>0)
			{
				__asm
				{
					xor	eax, eax
					xor	ebx, ebx
					xor	ecx, ecx
					xor	edx, edx
					_emit	0x0f						// CPUID
					_emit	0xa2
					_emit	0x0f						// RTDSC
					_emit	0x31
					mov	[iTicks2], eax
				}
				break;
			}
			 ++iNotFirstLoop;
		}
		unsigned int iTotalTicks = iTicks2 - iTicks1;
		_mfCPUFreqHz = float(iTotalTicks) / 1000000.0f;
		*/
	}

	HWND System::isProcessRunning(const std::string& strAppWindowTitle)
	{
		HWND handle = NULL;
		handle = FindWindow(NULL, strAppWindowTitle.c_str());
		return handle;
	}

	bool System::isProcessInForeground(const std::string& strAppWindowTitle)
	{
		HWND hWnd = isProcessRunning(strAppWindowTitle.c_str());
		if (!hWnd)
			return false;
		HWND hWndForegroundWindow = GetForegroundWindow(); // Get current active window's handle
		if (hWndForegroundWindow != hWnd)
			return false;
		return true;
	}

	bool System::copyTextToClipboard(const std::string& strText)
	{
		if (!OpenClipboard(NULL))
		{
			// Unable to open the system's clipboard
			return false;
		}

		EmptyClipboard();

		HGLOBAL hglb = GlobalAlloc(GMEM_MOVEABLE, sizeof(TCHAR) * (int)(strText.size() + 1));
		LPTSTR lptstrCopy = (LPTSTR)GlobalLock(hglb);
		memcpy(lptstrCopy, (TCHAR*)strText.c_str(), sizeof(TCHAR) * (int)(strText.size() + 1));
		GlobalUnlock(hglb);

		// Specify the data format that we are sending to the clipboard
		SetClipboardData(CF_TEXT, hglb);

		CloseClipboard();
		return true;
	}
	void System::getCurrentDisplayModeDims(unsigned int& iWidth, unsigned int& iHeight)
	{
		DEVMODE dm;
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
		iWidth = dm.dmPelsWidth;
		iHeight = dm.dmPelsHeight;
	}

	void System::SynthesizeMouseLeftButton(unsigned int iMousePosX, unsigned int iMousePosY)
	{
		// Get current display resolution
		unsigned int iDims[2];
		getCurrentDisplayModeDims(iDims[0], iDims[1]);

		INPUT input;
		SecureZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_MOUSE;

		// Set position as normalised absolute values
		input.mi.dx = (LONG)((65535.0f / float(iDims[0])) * float(iMousePosX));
		input.mi.dy = (LONG)((65535.0f / float(iDims[1])) * float(iMousePosY));

		// Move mouse
		input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		SendInput(1, &input, sizeof(INPUT));
		Sleep(10);
		// Send lmb down
		input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &input, sizeof(INPUT));
		Sleep(10);

		// Send lmb up
		input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
		SendInput(1, &input, sizeof(INPUT));
	}

	void System::changeScreenmode(int iWidth, int iHeight, int iBitdepth, int iFrequency)
	{
		DEVMODE dm;
		memset(&dm, 0, sizeof(dm));
		dm.dmSize = sizeof(dm);
		dm.dmPelsWidth = iWidth;
		dm.dmPelsHeight = iHeight;
		if (iBitdepth == 24)
			iBitdepth += 8;
		dm.dmBitsPerPel = iBitdepth;
		dm.dmDisplayFrequency = iFrequency;
		dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
		if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			Log::getPointer()->exception("changeScreenmode() failed to change display settings.");
	}

	void System::_setUserAppDataDir(void)
	{
		char szTmp[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, szTmp);
		_mStrAppDataDir = szTmp;
		_mStrAppDataDir.append("\\");
	}

	void System::_setUserMyDocumentsDir(void)
	{
		char szTmp[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szTmp);
		_mStrMyDocumentsDir = szTmp;
		_mStrMyDocumentsDir.append("\\");
	}

	void System::_setUserDesktopDir(void)
	{
		char szTmp[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, NULL, SHGFP_TYPE_CURRENT, szTmp);
		_mStrDesktopDir = szTmp;
		_mStrDesktopDir.append("\\");
	}

}