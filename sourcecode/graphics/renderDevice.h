#pragma once
#include "../precompiled_header.h"
#include "../singleton.h"
#include "../settings.h"

namespace Nexus
{
	class RenderDevice : public Singleton<RenderDevice>
	{
	public:
		RenderDevice();

		// Attempts to create the window and setup OpenGL.
		// If an error occurred, an exception is thrown
		void createWindow(std::string applicationName = "Nexus");

		// Close window
		void closeWindow(void);

		// Update the window
		// Returns false if the window has been asked to close
		bool updateWindow(void);

		// Attempts to flip the backbuffer
		void swapBuffers(void);

		// Returns the window handle
		HWND getWindowHandle(void);

		// Called from window resize callback
		void windowResized(int newWidth, int newHeight);

		// Returns current window width
		int getWindowWidth(void);

		// Returns current window height
		int getWindowHeight(void);

		// Returns whether window is in fullscreen or not
		bool getWindowFullscreen(void);

		// Set Vsync on or off
		void setVsync(bool bVSyncOn);
	private:
		GLFWwindow* pWindow;
		Settings settings;	// Has default values
	};


}