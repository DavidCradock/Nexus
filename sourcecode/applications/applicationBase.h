#pragma once
#include "../precompiled_header.h"

namespace Nexus
{
	// Base class to derive our app classes from
	class ApplicationBase
	{
	public:
		// Override this if you need it, by default, it does nothing.
		// This method is called only once upon initialisation after window and other stuff has been initialised
		virtual void initOnce(void);

		// Override this if you need it, by default, it does nothing.
		// This is called each time the application is set as current from the application manager.
		// AKA, we're starting this application
		virtual void onStart(void);

		// Override this if you need it, by default, it does nothing.
		// This is called each time the application is the current application and some other application is set as current from the application manager.
		// AKA, we're stopping this application
		virtual void onStop(void);

		// Pure virtual, so we need to override this
		// It is called once per program loop for the currently set application
		// Simply an update method
		// return false here if the application wants to close.
		virtual bool onUpdate(void) = 0;
	protected:

		// Called by all derived classes, to add themselves to the application manager
		void addAppToManager(const std::string& applicationName);

	};

}