#include "precompiled_header.h"
#include "ManagerApplications.h"
#include "../core/log.h"

namespace Nexus
{
	ManagerApplications::ManagerApplications()
	{
		currentApp = "";
	}

	void ManagerApplications::addApp(const std::string& applicationName, ApplicationBase* pTheApplication)
	{
		// Make sure it doesn't already exist
		std::map<std::string, ApplicationBase*>::iterator it = applications.find(applicationName);
		if (it != applications.end())
		{
			std::string err("ManagerApplications::addApp(");
			err.append(applicationName);
			err.append(" failed. Name already exists");
			Log::getPointer()->exception(err);
		}

		// If we get here, we can add it
		applications[applicationName] = pTheApplication;

		// If no other applications have been added, set this one up to be the currently set application
		if (currentApp.length() == 0)
		{
			currentApp = applicationName;

			// Also call it's onStart method
			pTheApplication->onStart();
		}
	}

	const std::string& ManagerApplications::getAppName(unsigned int index)
	{
		if (index >= applications.size())
		{
			std::string err("ManagerApplications::getAppName(");
			err.append(std::to_string(index));
			err.append(") failed. Invalid index given, maximum number of added applications is ");
			err.append(std::to_string(applications.size()));
			Log::getPointer()->exception(err);
		}

		std::map<std::string, ApplicationBase*>::iterator it = applications.begin();
		for (unsigned int i = 0; i < index; ++i)
		{
			it++;
		}
		return it->first;
	}

	void ManagerApplications::switchToApp(const std::string& applicationToSwitchTo)
	{
		// Attempt to find the application we're trying to switch to
		std::map<std::string, ApplicationBase*>::iterator itNewApp = applications.find(applicationToSwitchTo);
		if (itNewApp == applications.end())
		{
			std::string err("ManagerApplications::switchToApp(\"");
			err.append(applicationToSwitchTo);
			err.append("\") failed. Application by given name doesn't exist.");
			Log::getPointer()->exception(err);
		}
		// Attempt to find the application that's currently set as current
		std::map<std::string, ApplicationBase*>::iterator itOldApp = applications.find(currentApp);
		if (itOldApp == applications.end())
		{
			std::string err("ManagerApplications::switchToApp(");
			err.append(applicationToSwitchTo);
			err.append(" failed. Prior to changing to the new application, the old application called ");
			err.append(currentApp);
			err.append(" doesn't exist which is mental!");
			Log::getPointer()->exception(err);
		}

		// Change current app name and call the various methods for each
		currentApp = applicationToSwitchTo;
		itOldApp->second->onStop();
		itNewApp->second->onStart();

	}

	void ManagerApplications::switchToNextApp(void)
	{
		// If no apps are currently set
		if (0 == currentApp.size())
			return;	// Silently fail

		// If only one app exists, return
		if (1 == applications.size())
			return;

		// Find index of current application
		int indexOfCurrentApp = -1;
		for (int i = 0; i < getNumApps(); ++i)
		{
			if (currentApp == getAppName(i))
			{
				indexOfCurrentApp = i;
				break;
			}
		}
		if (-1 == indexOfCurrentApp)
			return;
		// Determine index of next app (wrap around)
		int indexOfNextApp = indexOfCurrentApp + 1;
		if (indexOfNextApp > applications.size() - 1)
			indexOfNextApp = 0;
		switchToApp(getAppName(indexOfNextApp));
	}

	void ManagerApplications::callAllApps_initOnce(void)
	{
		std::map<std::string, ApplicationBase*>::iterator it = applications.begin();
		while (it != applications.end())
		{
			it->second->initOnce();
			it++;
		}
	}

	bool ManagerApplications::callCurrentApp_onUpdate(void)
	{
		std::map<std::string, ApplicationBase*>::iterator it = applications.find(currentApp);
		if (it == applications.end())
		{
			std::string str("ApplicationManager::callCurrentApp_onUpdate(");
			str.append(currentApp);
			str.append(" failed. Unable to find the named application");
			Log::getPointer()->exception(str);
		}
		return it->second->onUpdate();
	}

}
