#include "precompiled_header.h"
#include "applicationBase.h"
#include "../managers/managerApplications.h"

namespace Nexus
{
	void ApplicationBase::_addAppToManager(const std::string& applicationName)
	{
		ManagerApplications* pAM = ManagerApplications::getPointer();
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