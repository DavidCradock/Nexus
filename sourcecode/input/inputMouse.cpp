#include "precompiled_header.h"
#include "inputMouse.h"

namespace Nexus
{

	InputMouse::InputMouse(void)
	{
		windowHandle = NULL;
		_mlpDIDeviceM = NULL;
		leftButOnce[0] = false;
		leftButOnce[1] = false;
		midButOnce[0] = false;
		midButOnce[1] = false;
		rightButOnce[0] = false;
		rightButOnce[1] = false;
		mouseLeft = false;
		mouseMiddle = false;
		mouseRight = false;
		mouseX = 0;
		mouseY = 0;
		mouseXold = 0;
		mouseYold = 0;
		mouseDeltaX = 0.0f;
		mouseDeltaY = 0.0f;
		mouseDeltaZ = 0.0f;
	}

	InputMouse::~InputMouse(void)
	{
	}

	bool InputMouse::init(LPDIRECTINPUT8 pMainDirectXinputDevice, HWND hApplicationWindow)
	{
		windowHandle = hApplicationWindow;

		_mlpDI = pMainDirectXinputDevice;
		if (!_mlpDI)
			return false;

		// Create mouse device
		_mlpDI->CreateDevice(GUID_SysMouse, &_mlpDIDeviceM, NULL);
		if (_mlpDIDeviceM == NULL)
			return false;

		// Set mouse data format
		HRESULT hr = _mlpDIDeviceM->SetDataFormat(&c_dfDIMouse);
		if (FAILED(hr))
			return false;

		// Set mouse behavior
		hr = _mlpDIDeviceM->SetCooperativeLevel(hApplicationWindow, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
		//	hr = _mlpDIDeviceM->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr))
			return false;

		// Acquire the Mouse
		hr = _mlpDIDeviceM->Acquire();
		if (FAILED(hr))
			return false;

		return true;	// Success!
	}

	void InputMouse::update(bool fullscreen, unsigned int screenWidth, unsigned int screenHeight)
	{
		DIMOUSESTATE mouseState;	// Struct to store state of mouse
		HRESULT hr = _mlpDIDeviceM->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
		if (FAILED(hr))
		{
			if (hr = DIERR_INPUTLOST)
			{
				hr = _mlpDIDeviceM->Acquire();
				if (FAILED(hr))
				{
					// Device lost
					mouseLeft = false;
					mouseRight = false;
					mouseMiddle = false;
					leftButOnce[0] = leftButOnce[1] = false;
					rightButOnce[0] = rightButOnce[1] = false;
					midButOnce[0] = midButOnce[1] = false;
					return;
				}
			}
		}

		// Left mouse button down?
		if (mouseState.rgbButtons[0] == 0x80)	mouseLeft = true;
		else	mouseLeft = false;

		// Middle mouse button down?
		if (mouseState.rgbButtons[2] == 0x80)	mouseMiddle = true;
		else	mouseMiddle = false;

		// Right mouse button down?
		if (mouseState.rgbButtons[1] == 0x80)	mouseRight = true;
		else	mouseRight = false;

		// Update button once stuff
		if (mouseLeft) // Is mouse button down?
		{
			if (leftButOnce[1] == false) // Wasn't down on last update
			{
				leftButOnce[1] = true;
				leftButOnce[0] = true;
			}
			else  // Was down last time
			{
				leftButOnce[0] = false;  // Return value
			}
		}
		else
		{
			// No longer pressed
			leftButOnce[1] = false;
			leftButOnce[0] = false;
		}
		if (mouseRight) // Is mouse button down?
		{
			if (rightButOnce[1] == false) // Wasn't down on last update
			{
				rightButOnce[1] = true;
				rightButOnce[0] = true;
			}
			else  // Was down last time
			{
				rightButOnce[0] = false;  // Return value
			}
		}
		else
		{
			// No longer pressed
			rightButOnce[1] = false;
			rightButOnce[0] = false;
		}
		if (mouseMiddle) // Is mouse button down?
		{
			if (midButOnce[1] == false) // Wasn't down on last update
			{
				midButOnce[1] = true;
				midButOnce[0] = true;
			}
			else  // Was down last time
			{
				midButOnce[0] = false;  // Return value
			}
		}
		else
		{
			// No longer pressed
			midButOnce[1] = false;
			midButOnce[0] = false;
		}

		// Update members
		mouseDeltaX = (float)mouseState.lX;					// Change in position along X
		mouseDeltaY = (float)mouseState.lY;					// Change in position along Y
		mouseDeltaZ = (float)mouseState.lZ;					// Change in position along Z (Wheel)

		// Store old position...
		mouseXold = mouseX;
		mouseYold = mouseY;

		// Cursor stuff
		POINT point;
		GetCursorPos(&point);
		if (fullscreen)
		{
			mouseX = (float)point.x;
			//		mouseY = SCRenderDevice::getPointer()->getWindowHeight() - (float)point.y - 1;
			mouseY = (float)point.y;
		}
		else
		{
			WINDOWINFO wndInfo;
			GetWindowInfo(windowHandle, &wndInfo);

			mouseX = (float)point.x;
			mouseX -= wndInfo.rcClient.left;
			/*
			mouseY = SCRenderDevice::getPointer()->getWindowHeight() - (float)point.y - 1;
			mouseY += wndInfo.rcClient.top;
			*/
			mouseY = (float)point.y;
			mouseY -= wndInfo.rcClient.top;
		}
		/*
			// Get window dimensions (Not including borders or menu bar, only client area)
			WINDOWINFO wiWindow;
			GetWindowInfo(windowHandle, &wiWindow);

			// Modify the rcWindow value to take into account the app's title bar
			AdjustWindowRect(&wiWindow.rcWindow, WS_POPUPWINDOW|WS_CAPTION, false);

			// Get position of system mouse cursor
			POINT mPos;	GetCursorPos(&mPos);

			// Set app's cursor position
			mouseX = float(mPos.x - wiWindow.rcClient.left);
			mouseY = float(mPos.y - wiWindow.rcClient.top);
		*/
	}

	void InputMouse::release(void)
	{
		if (_mlpDIDeviceM)	// Mouse object exists?
		{
			_mlpDIDeviceM->Unacquire();
			_mlpDIDeviceM->Release();
			_mlpDIDeviceM = NULL;
		}
	}

	void InputMouse::setMousePos(int x, int y)
	{
		mouseX = float(x);
		mouseY = float(y);
		mouseXold = mouseX;
		mouseYold = mouseY;
		mouseDeltaX = mouseDeltaY = 0;

		// Get window dimensions (Not including borders or menu bar, only client area)
		WINDOWINFO wiWindow;
		GetWindowInfo(windowHandle, &wiWindow);
		// Modify the rcWindow value to take into account the app's title bar
		AdjustWindowRect(&wiWindow.rcWindow, WS_POPUPWINDOW | WS_CAPTION, false);
		POINT mPos;
		mPos.x = wiWindow.rcClient.left + (int)x;
		mPos.y = wiWindow.rcClient.top + (int)y;
		SetCursorPos(mPos.x, mPos.y);
	}

}