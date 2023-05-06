#include "precompiled_header.h"
#include "inputJoystick.h"
#include "../managers/inputDeviceManager.h"

namespace Nexus
{

	// Function prototypes for joysticks
	BOOL CALLBACK enumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, void* pContext)
	{
		HRESULT hr;

		// Obtain an interface to the enumerated joystick.
		hr = InputDeviceManager::getPointer()->joy.directInput->CreateDevice(pdidInstance->guidInstance, &InputDeviceManager::getPointer()->joy.device, NULL);

		// If it failed, then we can't use this joystick. (Maybe the user unplugged
		// it while we were in the middle of enumerating it.)
		if (FAILED(hr))
			return DIENUM_CONTINUE;

		// Stop enumeration. Note: we're just taking the first joystick we get. You
		// could store all the enumerated joysticks and let the user pick.
		return DIENUM_STOP;
	}

	InputJoystick::InputJoystick(void)
	{
		device = NULL;
	}

	InputJoystick::~InputJoystick(void)
	{
	}

	bool InputJoystick::init(LPDIRECTINPUT8 pMainDirectXinputDevice, HWND hApplicationWindow)
	{
		joystickDetected = true;
		forcefeedbackDetected = true;
		numButtons = 0;
		numPOVS = 0;
		numAxis = 0;
		HRESULT hr;

		directInput = pMainDirectXinputDevice;
		if (!directInput)
		{
			joystickDetected = false;
			forcefeedbackDetected = false;
			return false;
		}

		// Enumerate available joysticks and create object for each one found
		// Check for force feedback devices first
		if (FAILED(hr = directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, enumJoysticksCallback, NULL, DIEDFL_FORCEFEEDBACK | DIEDFL_ATTACHEDONLY)))
		{
			// No force feedback, check for normal joystick
			forcefeedbackDetected = false;
			if (FAILED(hr = directInput->EnumDevices(DI8DEVCLASS_GAMECTRL, enumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY)))
			{
				joystickDetected = false;
				return true;
			}
		}

		// Do we have a joystick?
		if (device == NULL)
		{
			joystickDetected = false;
			forcefeedbackDetected = false;
			return true;
		}

		// Set joystick data format
		hr = device->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))
		{
			joystickDetected = false;
			forcefeedbackDetected = false;
			return false;
		}

		// Set joystick behavior
		hr = device->SetCooperativeLevel(hApplicationWindow, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr))
		{
			joystickDetected = false;
			forcefeedbackDetected = false;
			return false;
		}

		// Get joystick capabilities
		DIDEVCAPS	capsJ;
		capsJ.dwSize = sizeof(capsJ);
		hr = device->GetCapabilities(&capsJ);
		if (FAILED(hr))
		{
			joystickDetected = false;
			forcefeedbackDetected = false;
			return false;
		}

		// Set joystick capability members
		numButtons = capsJ.dwButtons;
		numPOVS = capsJ.dwPOVs;
		numAxis = capsJ.dwAxes;

		// Set ranges -1000 to 1000
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_DEVICE;
		diprg.diph.dwObj = 0;
		diprg.lMin = -1000;
		diprg.lMax = +1000;
		device->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Turn off auto center if force feedback
		if (forcefeedbackDetected == true)
		{
			DIPROPDWORD DIPropAutoCenter;
			DIPropAutoCenter.diph.dwSize = sizeof(DIPropAutoCenter);
			DIPropAutoCenter.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			DIPropAutoCenter.diph.dwHow = DIPH_DEVICE;
			DIPropAutoCenter.diph.dwObj = 0;
			DIPropAutoCenter.dwData = DIPROPAUTOCENTER_OFF;
			hr = device->SetProperty(DIPROP_AUTOCENTER, &DIPropAutoCenter.diph);
		}

		// Acquire joystick
		hr = device->Acquire();
		ZeroMemory(&_mjs, sizeof(DIJOYSTATE2));
		return true;
	}

	void InputJoystick::update(void)
	{
		HRESULT hr;
		if (device == NULL)
			return;

		// Poll joystick, ready for reading
		if (DIERR_INPUTLOST == device->Poll())
		{
			// Device lost!
			device->Acquire();
		}

		// Read state of joystick
		hr = device->GetDeviceState(sizeof(DIJOYSTATE2), &_mjs);
		if (FAILED(hr))
			return;

		// Adjust values for hats
		if (_mjs.rgdwPOV[0] != -1)
			_mjs.rgdwPOV[0] /= 100;
	}

	void InputJoystick::release(void)
	{
		if (device)	// Joystick object exists?
		{
			device->Unacquire();
			device->Release();
			device = NULL;
		}
	}

}