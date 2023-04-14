#include "precompiled_header.h"
#include "applicationBase.h"
#include "applicationManager.h"

namespace Nexus
{
	void ApplicationBase::_addAppToManager(const std::string& applicationName)
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