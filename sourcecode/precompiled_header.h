#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include "../dependencies/glad4.6_core/include/glad/glad.h"
#include "../dependencies/glfw-3.3.8_source/include/GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32	// So we can use glfwGetWin32Window function for passing of window handle to input manager
#include "../dependencies/glfw-3.3.8_source/include/GLFW/glfw3native.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb_image.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../dependencies/stb_image_write.h"

#include <algorithm>
#include <chrono>
#include <map>
#include <fstream>
#include <iostream>
#include <psapi.h>						// Used in system.cpp for GetPerformanceInfo() etc.
#include <shellapi.h>					// Executing a process / ShellGetFolderPath
#include <Shlobj.h>						// For SHGetFolderPath() etc in core\system.cpp
#include <Shlobj_core.h>				// see above
#include <sstream>						// For loading shaders
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

// Multithreading
#include <mutex>
#include <thread>

// OpenGL
#include <gl/gl.h>
#include <gl/glu.h>
#include "../dependencies/OpenGL/glext.h"
#include "../dependencies/OpenGL/wgl.h"

// DirectX for direct input
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

// XAudio2
//#include <hrtfapoapi.h>
#include <x3daudio.h>
//#include <xapo.h>
#include <xapobase.h>
#include <xapofx.h>
#include <xaudio2.h>
#include <xaudio2fx.h>