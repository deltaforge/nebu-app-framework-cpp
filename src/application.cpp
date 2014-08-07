
#include "nebu-app-framework/application.h"
#include "nebu-app-framework/applicationHooks.h"
#include "nebu-app-framework/configuration.h"
#include "nebu-app-framework/daemonManager.h"
#include "nebu-app-framework/topologyManager.h"
#include "nebu-app-framework/vmManager.h"

#include "log4cxx/logger.h"

#include <unistd.h>

// Using declarations - standard library
using std::shared_ptr;

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("nebu.app.framework.Application"));

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			Application::Application(shared_ptr<DaemonManager> daemonManager,
					shared_ptr<TopologyManager> topologyManager, shared_ptr<VMManager> vmManager) :
					stopLoop(false), daemonManager(daemonManager), topologyManager(topologyManager),
					vmManager(vmManager)
			{
				vmManager->registerVMEventHandler(daemonManager);
			}

			int Application::mainLoop()
			{
				while (!this->stopLoop) {
					LOG4CXX_TRACE(logger, "PreLoop");
					this->applicationHooks->preLoop();

					LOG4CXX_TRACE(logger, "PreRefreshVMs");
					this->applicationHooks->preRefreshVMs();
					LOG4CXX_TRACE(logger, "RefreshVMs");
					this->vmManager->refreshVMList();
					LOG4CXX_TRACE(logger, "PostRefreshVMs");
					this->applicationHooks->postRefreshVMs();

					LOG4CXX_TRACE(logger, "PreRefreshTopology");
					this->applicationHooks->preRefreshTopology();
					LOG4CXX_TRACE(logger, "RefreshTopology");
					this->topologyManager->refreshTopology();
					LOG4CXX_TRACE(logger, "PostRefreshTopology");
					this->applicationHooks->postRefreshTopology();

					LOG4CXX_TRACE(logger, "PreRefreshDaemons");
					this->applicationHooks->preRefreshDaemons();
					LOG4CXX_TRACE(logger, "RefreshDaemons");
					this->daemonManager->refreshDaemons();
					LOG4CXX_TRACE(logger, "PostRefreshDaemons");
					this->applicationHooks->postRefreshDaemons();

					LOG4CXX_TRACE(logger, "PreDeployDaemons");
					this->applicationHooks->preDeployDaemons();
					LOG4CXX_TRACE(logger, "DeployDaemons");
					this->daemonManager->deployDaemons();
					LOG4CXX_TRACE(logger, "PostDeployDaemons");
					this->applicationHooks->postDeployDaemons();

					LOG4CXX_TRACE(logger, "PostLoop");
					this->applicationHooks->postLoop();

					LOG4CXX_DEBUG(logger, "Waiting for next round...");
					sleep(CONFIG_GETINT(CONFIG_APP_INTERVAL));
				}

				return 0;
			}

		}
	}
}
