#include "precompiled_header.h"
#include "applicationManager.h"
#include "../log.h"

namespace Nexus
{
	ApplicationManager::ApplicationManager()
	{
		currentApp = "";
	}

	void ApplicationManager::addApp(const std::string& applicationName, ApplicationBase* pTheApplication)
	{
		// Make sure it doesn't already exist
		std::map<std::string, ApplicationBase*>::iterator it = applications.find(applicationName);
		if (it != applications.end())
		{
			std::string str("ApplicationManager::addApp(");
			str.append(applicationName);
			str.append(" failed. Name already exists");
			Log log;
			log.addException(str);
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

	const std::string& ApplicationManager::getAppName(unsigned int index)
	{
		if (index >= applications.size())
		{
			std::string str("ApplicationManager::getAppName(");
			str.append(std::to_string(index));
			str.append(") failed. Invalid index given, maximum number of added applications is ");
			str.append(std::to_string(applications.size()));
			Log log;
			log.addException(str);
		}

		std::map<std::string, ApplicationBase*>::iterator it = applications.begin();
		for (unsigned int i = 0; i < index; ++i)
		{
			it++;
		}
		return it->first;
	}

	void ApplicationManager::switchToApp(const std::string& applicationToSwitchTo)
	{
		// Attempt to find the application we're trying to switch to
		std::map<std::string, ApplicationBase*>::iterator itNewApp = applications.find(applicationToSwitchTo);
		if (itNewApp == applications.end())
		{
			std::string str("ApplicationManager::switchToApp(\"");
			str.append(applicationToSwitchTo);
			str.append("\") failed. Application by given name doesn't exist.");
			Log log;
			log.addException(str);
		}
		// Attempt to find the application that's currently set as current
		std::map<std::string, ApplicationBase*>::iterator itOldApp = applications.find(currentApp);
		if (itOldApp == applications.end())
		{
			std::string str("ApplicationManager::switchToApp(");
			str.append(applicationToSwitchTo);
			str.append(" failed. Prior to changing to the new application, the old application called ");
			str.append(currentApp);
			str.append(" doesn't exist which is mental!");
			Log log;
			log.addException(str);
		}

		// Change current app name and call the various methods for each
		currentApp = applicationToSwitchTo;
		itOldApp->second->onStop();
		itNewApp->second->onStart();

	}

	void ApplicationManager::switchToNextApp(void)
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

	void ApplicationManager::callAllApps_initOnce(void)
	{
		std::map<std::string, ApplicationBase*>::iterator it = applications.begin();
		while (it != applications.end())
		{
			it->second->initOnce();
			it++;
		}
	}

	bool ApplicationManager::callCurrentApp_onUpdate(void)
	{
		std::map<std::string, ApplicationBase*>::iterator it = applications.find(currentApp);
		if (it == applications.end())
		{
			std::string str("ApplicationManager::callCurrentApp_onUpdate(");
			str.append(currentApp);
			str.append(" failed. Unable to find the named application");
			throw std::runtime_error(str);
		}
		return it->second->onUpdate();
	}

}
