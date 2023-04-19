#include "precompiled_header.h"
#include "managerInputDevices.h"
#include "../core/log.h"

namespace Nexus
{

	ManagerInputDevices::ManagerInputDevices()
	{
		directInput = NULL;
	}

	void ManagerInputDevices::init(HWND hApplicationWindow)
	{
		instance = GetModuleHandle(NULL);
		windowHandle = hApplicationWindow;
		if (!instance)
			throw std::runtime_error("ManagerInputDevices::init() failed. Unable to obtain application instance handle.");

		// Init main directX input object
		if (DI_OK != DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL))
			throw std::runtime_error("ManagerInputDevices::init() failed. Unable to create main DirectX input object.");

		// Init joystick
		if (!joy.init(directInput, windowHandle))// Error only returned if joystick detected, but init failed.
			throw std::runtime_error("ManagerInputDevices::init() failed. Joystick detected, but unable to initialise it.");

		// Init keyboard
		if (!key.init(directInput, windowHandle))
			throw std::runtime_error("ManagerInputDevices::init() failed. Unable to initialise keyboard.");

		// Init mouse
		if (!mouse.init(directInput, windowHandle))
			throw std::runtime_error("ManagerInputDevices::init() failed. Unable to initialise mouse.");
	}

	void ManagerInputDevices::shutdown(void)
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

	void ManagerInputDevices::update(bool fullscreen, unsigned int screenWidth, unsigned int screenHeight)
	{
		joy.update();
		key.update();
		mouse.update(fullscreen, screenWidth, screenHeight);
	}

}