#include "precompiled_header.h"
#include "inputDeviceManager.h"
#include "../core/log.h"

namespace Nexus
{

	InputDeviceManager::InputDeviceManager()
	{
		directInput = NULL;
	}

	void InputDeviceManager::init(HWND hApplicationWindow)
	{
		instance = GetModuleHandle(NULL);
		windowHandle = hApplicationWindow;
		if (!instance)
			Log::getPointer()->exception("InputDeviceManager::init() failed. Unable to obtain application instance handle.");

		// Init main directX input object
		if (DI_OK != DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL))
			Log::getPointer()->exception("InputDeviceManager::init() failed. Unable to create main DirectX input object.");

		// Init joystick
		if (!joy.init(directInput, windowHandle))// Error only returned if joystick detected, but init failed.
			Log::getPointer()->exception("InputDeviceManager::init() failed. Joystick detected, but unable to initialise it.");

		// Init keyboard
		if (!key.init(directInput, windowHandle))
			Log::getPointer()->exception("InputDeviceManager::init() failed. Unable to initialise keyboard.");

		// Init mouse
		if (!mouse.init(directInput, windowHandle))
			Log::getPointer()->exception("InputDeviceManager::init() failed. Unable to initialise mouse.");
	}

	void InputDeviceManager::shutdown(void)
	{
		joy.release();
		key.release();
		mouse.release();
		if (directInput)
		{
			directInput->Release();
			directInput = NULL;
		}
	}

	void InputDeviceManager::update(bool fullscreen, unsigned int screenWidth, unsigned int screenHeight)
	{
		joy.update();
		key.update();
		mouse.update(fullscreen, screenWidth, screenHeight);
	}

}