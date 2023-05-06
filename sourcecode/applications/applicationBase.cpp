#include "precompiled_header.h"
#include "applicationBase.h"
#include "../managers/ApplicationManager.h"

namespace Nexus
{
	void ApplicationBase::addAppToManager(const std::string& applicationName)
	{
		ApplicationManager* pAM = ApplicationManager::getPointer();
		pAM->addApp(applicationName, this);
	}

	void ApplicationBase::initOnce(void)
	{
	}

	void ApplicationBase::onStart(void)
	{
	}

	void ApplicationBase::onStop(void)
	{
	}
}