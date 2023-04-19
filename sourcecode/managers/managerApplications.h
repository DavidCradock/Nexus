#pragma once
#include "../precompiled_header.h"
#include "../core/singleton.h"
#include "../applications/applicationBase.h"

namespace Nexus
{

	// App manager class to handle all apps which are derived from CAppBase found in "app base.h"
	// By default, the current application to be run each loop is the CAppDevelopment application
	// This can be changed by called ApplicationManager::getPointer()->switchToApp("YourApplicationName");
	class ManagerApplications : public Singleton<ManagerApplications>
	{
	public:
		ManagerApplications();

		// Return the number of applications added to this manager
		size_t getNumApps(void) { return applications.size(); }

		// Return application name at specified index into map
		// If invalid index given, a critical error occurs
		const std::string& getAppName(unsigned int index);

		// Switches from the current application, to the named one.
		// If invalid application name is given, an exception is thrown
		// This'll call onStop() of the currently set class, then call onStart() of the new set class.
		// If the class name given is already set as current, it's call onStop() then onStart() of that class, kinda like a reset.
		void switchToApp(const std::string& applicationToSwitchTo);

		// Will switch to the next app
		void switchToNextApp(void);

		// Returns the name of the currently set application
		std::string getCurrentAppName(void) { return currentApp; }

		// Add a new application class object pointer derived from the CAppBass class to the manager
		// If the name of the application already exists, a critical error occurs.
		// This has to be called inside the CAppBass derived class's constructor.. CExampleApp(CText &strAppName);
		// The first time this is called from the class's constructor, that application is set as currently active.
		void addApp(const std::string& applicationName, ApplicationBase* pTheApplication);

		// Calls all added applications' initOnce() methods
		void callAllApps_initOnce(void);

		// Calls currently set application's onUpdate() method
		// Returns false if the application wants to shutdown
		bool callCurrentApp_onUpdate(void);

	private:
		std::map<std::string, ApplicationBase*> applications;	// Hash map which holds each named application
		std::string currentApp;						// Application name of the current application.
	};
}