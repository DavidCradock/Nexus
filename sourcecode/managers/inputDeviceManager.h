#pragma once
#include "../precompiled_header.h"
#include "../core/singleton.h"
#include "managedObjects/inputJoystick.h"
#include "managedObjects/inputKeyboard.h"
#include "managedObjects/inputMouse.h"

namespace Nexus
{
	// Main device input class for mouse, keyboard and joystick data. 
	// This class automatically manages three objects, one of each class...
	// InputJoystick, member name = joy - for accessing joystick data.
	// InputKeyboard, member name = key - for accessing keyboard data.
	// InputMouse, member name = mouse - for accessing mouse data.
	// It initialises, updates and shuts down those devices automatically.
	// To gain access to either of the input devices, simply access it's representing member.
	// For more information on how to use them, see their respective classes.
	// A brief example to determine if the excape key is pressed...
	// if (InputDeviceManager::getPointer()->key.pressed(KC_ESCAPE))
	// { 
	//    // Do something, like exit
	// }
	class InputDeviceManager : public Singleton<InputDeviceManager>
	{
	public:
		InputDeviceManager();

		// Initialise all input devices
		void init(HWND hApplicationWindow);

		// Updates all input states
		void update(bool fullscreen, unsigned int screenWidth, unsigned int screenHeight);

		// Shutdown all input devices
		void shutdown(void);

		InputJoystick joy;	// Joystick object
		InputKeyboard key;	// Keyboard object
		InputMouse	mouse;	// Mouse object
	private:

		LPDIRECTINPUT8 directInput;	// Main DirectX input object
		HINSTANCE instance;		// Handle to application's instance
		HWND windowHandle;			// Handle to application's window
	};

}