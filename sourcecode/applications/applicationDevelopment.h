#pragma once
#include "../precompiled_header.h"
#include "applicationBase.h"
#include "../graphics/vertexBuffer.h"
#include "../timing.h"
#include "../graphics/sprites/sprites.h"
#include "../graphics/textFont.h"

namespace Nexus
{
	// Minimal application class that does nothing, it's an example of how to setup a new application
	// which automatically adds itself to the application manager
	class ApplicationDevelopment : public ApplicationBase
	{
	public:
		// Automatically add this class to the application manager
		ApplicationDevelopment(std::string& applicationName);

		// Override this if you need it, by default, it does nothing.
		// This method is called only once upon initialisation after window and other stuff has been initialised
		void initOnce(void);

		// Override this if you need it, by default, it does nothing.
		// This is called each time the application is set as current from the application manager.
		// AKA, we're starting this application
		void onStart(void);

		// Override this if you need it, by default, it does nothing.
		// This is called each time the application is the current application and some other application is set as current from the application manager.
		// AKA, we're stopping this application
		void onStop(void);

		// Pure virtual, so we need to override this
		// It is called once per program loop for the currently set application
		// Return false if the app wants to shutdown
		bool onUpdate(void);
	private:
		VertexBuffer vertexBuffer;
		Timing timing;
		Sprite sprite;
		TextFont textFont;
	};

}