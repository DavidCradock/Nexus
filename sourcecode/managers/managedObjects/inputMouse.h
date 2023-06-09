#pragma once
#include "../../precompiled_header.h"
#include "../../math/vector2.h"

namespace Nexus
{

	// Mouse Input class, use SCInputManager to access it's "mouse" member.
	// There is an object of this class called "mouse" which is initialised and updated in the SCInputManager class.
	// Use that object to access information about the device.
	class InputMouse
	{
	public:
		InputMouse(void);
		~InputMouse(void);

		// Initialize mouse object
		bool init(LPDIRECTINPUT8 pMainDirectXinputDevice, HWND hApplicationWindow);

		// Update
		void update(bool fullscreen, unsigned int screenWidth, unsigned int screenHeight);

		// Free everything
		void release(void);

		// Returns true if the left mouse button is pressed.
		inline bool leftButDown(void) { return mouseLeft; }

		// Returns true if the middle mouse button is pressed.
		inline bool midButDown(void) { return mouseMiddle; }

		// Returns true if the right mouse button is pressed.
		inline bool rightButDown(void) { return mouseRight; }

		// Returns true if the mouse button was pressed once
		inline bool leftButtonOnce(void) { return leftButOnce[0]; }

		// Returns true if the mouse button was pressed once
		inline bool midButtonOnce(void) { return midButOnce[0]; }

		// Returns true if the mouse button was pressed once
		inline bool rightButtonOnce(void) { return rightButOnce[0]; }

		// Returns the mouse cursor position on screen, along X axis
		inline int posX(void) { return int(mouseX); }

		// Returns the mouse cursor position on screen, along Y axis
		inline int posY(void) { return int(mouseY); }

		// Returns the mouse cursor position on screen
		inline Vector2 pos(void) { Vector2 vPos(mouseX, mouseY);	return vPos; }

		// Returns the distance which the mouse has moved since the last frame, along the X axis
		inline float deltaX(void) { return mouseDeltaX; }

		// Returns the distance which the mouse has moved since the last frame, along the Y axis
		inline float deltaY(void) { return mouseDeltaY; }

		// Returns the distance which the mouse wheel has moved since the last frame.
		inline float deltaZ(void) { return mouseDeltaZ; }

		// Sets the mouse position on screen
		void setMousePos(int x, int y);

		// Gets mouse cursor delta, for GUI
		inline Vector2 getMouseDeltaGUI(void) { return Vector2(mouseX - mouseXold, mouseY - mouseYold); }

		// Returns mouse cursor position
		inline Vector2 getCursorPos(void) { return Vector2(mouseX, mouseY); }

	private:
		LPDIRECTINPUT8 lpDI;				// Pointer to main DirectX input object
		LPDIRECTINPUTDEVICE8 lpDIDeviceM;	// DirectX input device object for the mouse

		bool mouseLeft;			// Left mouse button down?
		bool leftButOnce[2];		// Element 0 holds actual return value for mouseButOnce?(). 1 holds if button was held last frame
		bool mouseMiddle;			// Middle mouse button down?
		bool midButOnce[2];		// Element 0 holds actual return value for mouseButOnce?(). 1 holds if button was held last frame
		bool mouseRight;			// Right mouse button down?
		bool rightButOnce[2];	// Element 0 holds actual return value for mouseButOnce?(). 1 holds if button was held last frame
		float mouseX;			// Position of mouse on screen (Horizontal)
		float mouseY;			// Position of mouse on screen (Vertical)
		float mouseXold;			// Old position of mouse on screen (Horizontal)
		float mouseYold;			// Old position of mouse on screen (Vertical)
		float mouseDeltaX;		// Change in X position of mouse
		float mouseDeltaY;		// Change in Y position of mouse
		float mouseDeltaZ;		// Change in Z position of mouse (Mouse wheel)
		HWND windowHandle;				// Application window handle
	};

}