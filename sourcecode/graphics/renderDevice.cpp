#include "precompiled_header.h"
#include "renderDevice.h"
#include "../log.h"


namespace Nexus
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
		RenderDevice::getPointer()->windowResized(width, height);
	}

	RenderDevice::RenderDevice()
	{
		pWindow = 0;
	}
	void RenderDevice::createWindow(std::string applicationName)
	{
		// Load settings from settings file
		if (!settings.load())	// Attempt to load from file
			settings.save();	// Save the defaults to file for next run
		
		if (0 != pWindow)
			Log::getPointer()->addException("RenderDevice::createWindow() called, but window already exists.");
		if (!glfwInit())
			Log::getPointer()->addException("RenderDevice::createWindow() failed. Failed to init window system.");


		// Get primary monitor and it's dimensions
		GLFWmonitor* pPrimaryMonitor = glfwGetPrimaryMonitor();
		if (!pPrimaryMonitor)
		{
			Log::getPointer()->addException("RenderDevice::createWindow() failed. Unable to find primary monitor!");
		}
		int iMonitorWidth = 0;
		int iMonitorHeight = 0;
		glfwGetMonitorWorkarea(pPrimaryMonitor, NULL, NULL, &iMonitorWidth, &iMonitorHeight);

		// Create the window and set resize callback
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		if (settings.screenmode == 2)	// Decorated window
		{
			// Reduce size if window won't fit
			if (settings.screenWidth >= (unsigned int)iMonitorWidth)
				settings.screenWidth = iMonitorWidth - int(iMonitorWidth * 0.1f);
			if (settings.screenHeight >= (unsigned int)iMonitorHeight)
				settings.screenHeight = iMonitorHeight - int(iMonitorHeight * 0.1f);
			pWindow = glfwCreateWindow(settings.screenWidth, settings.screenHeight, applicationName.c_str(), NULL, NULL);
		}
		else if (settings.screenmode == 1)	// window fullscreen
			pWindow = glfwCreateWindow(settings.screenWidth, settings.screenHeight, applicationName.c_str(), NULL, NULL);
		else // Fullscreen
		{
			pWindow = glfwCreateWindow(settings.screenWidth, settings.screenHeight, applicationName.c_str(), pPrimaryMonitor, NULL);
		}
		if (0 == pWindow)
		{
			glfwTerminate();
			Log::getPointer()->addException("RenderDevice::createWindow() failed. Failure to create window.");
		}
		glfwMakeContextCurrent(pWindow);
		glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);

		// Center the window if windowed
		if (2 == settings.screenmode)
		{
			int iNewWindowPosX = iMonitorWidth / 2;
			int iNewWindowPosY = iMonitorHeight / 2;
			iNewWindowPosX -= settings.screenWidth / 2;
			iNewWindowPosY -= settings.screenHeight / 2;
			glfwSetWindowPos(pWindow, iNewWindowPosX, iNewWindowPosY);
		}

		// Obtain all extensions using the GLAD library
		// No more "glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTexture");" etc!
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			Log::getPointer()->addException("RenderDevice::createWindow() failed to obtain OpenGL extensions");

		glViewport(0, 0, settings.screenWidth, settings.screenHeight);	// Set OpenGL viewport based on window size

		setVsync(bool(settings.vsyncEnabled));
	}

	void RenderDevice::closeWindow(void)
	{
		glfwTerminate();
	}

	bool RenderDevice::updateWindow(void)
	{
		if (!pWindow)
			Log::getPointer()->addException("RenderDevice::updateWindow called, but window hasn't been created.");
		if (glfwWindowShouldClose(pWindow))
		{
			return false;
		}
		
		glfwPollEvents();
		return true;
		
	}

	void RenderDevice::swapBuffers(void)
	{
		if (!pWindow)
			Log::getPointer()->addException("RenderDevice::swapBuffers() called, but window doesn't exist.");
		glfwSwapBuffers(pWindow);
	}

	HWND RenderDevice::getWindowHandle(void)
	{
		if (!pWindow)
			return NULL;
		return glfwGetWin32Window(pWindow);
	}

	void RenderDevice::windowResized(int newWidth, int newHeight)
	{
		settings.screenWidth = newWidth;
		settings.screenHeight = newHeight;
	}

	int RenderDevice::getWindowWidth(void)
	{
		return settings.screenWidth;
	}

	int RenderDevice::getWindowHeight(void)
	{
		return settings.screenHeight;
	}

	bool RenderDevice::getWindowFullscreen(void)
	{
		if (0 == settings.screenmode)
			return true;
		return false;
	}

	// Set Vsync on or off
	void RenderDevice::setVsync(bool bVSyncOn)
	{
		glfwSwapInterval(int(bVSyncOn));
	}
}