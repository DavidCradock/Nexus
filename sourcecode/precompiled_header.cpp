#include "precompiled_header.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../dependencies/stb_image_write.h"

// All libaries are below
#ifdef NDEBUG
#pragma comment (lib, "dependencies/glfw-3.3.8_build/src/Release/glfw3.lib")
#else
#pragma comment (lib, "dependencies/glfw-3.3.8_build/src/Debug/glfw3.lib")
#endif

#pragma comment(lib,"opengl32.lib")

// DirectX for input
#pragma comment(lib, "dinput8.lib")	// DirectX input lib
#pragma comment(lib, "dxguid.lib")	// DirectX global ID thing so I can use GUID_keyboard etc for creating standard devices