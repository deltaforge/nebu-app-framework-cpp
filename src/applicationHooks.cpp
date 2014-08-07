
#include "nebu-app-framework/applicationHooks.h"
#include "nebu-app-framework/configuration.h"
#include "nebu-app-framework/daemonCollection.h"
#include "nebu-app-framework/topologyManager.h"
#include "nebu-app-framework/vmManager.h"
#include "nebu/appPhysRequest.h"
#include "nebu/appVirtRequest.h"
#include "nebu/nebuClient.h"
#include "nebu/restClientAdapter.h"

#include "log4cxx/basicconfigurator.h"

// Using declarations - nebu-common
using nebu::common::AppPhysRequest;
using nebu::common::AppVirtRequest;
using nebu::common::NebuClient;
using nebu::common::RestClientAdapter;
// Using declarations - standard library
using std::make_shared;
using std::shared_ptr;

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			void ApplicationHooks::prepareLogging()
			{
				log4cxx::BasicConfigurator::configure();
				log4cxx::Logger::getRootLogger()->setLevel(log4cxx::Level::getWarn());
			}

			shared_ptr<DaemonCollection> ApplicationHooks::getDaemonCollection()
			{
				if (!this->daemonCollection) {
					this->daemonCollection = make_shared<DaemonCollection>();
				}
				return this->daemonCollection;
			}

			shared_ptr<TopologyManager> ApplicationHooks::getTopologyManager()
			{
				if (!this->topologyManager) {
					shared_ptr<NebuClient> nebuClient = make_shared<NebuClient>(RestClientAdapter::getInstance(),
							CONFIG_GET(CONFIG_NEBU_URL));
					shared_ptr<AppPhysRequest> appPhysRequest = make_shared<AppPhysRequest>(nebuClient,
							CONFIG_GET(CONFIG_APP_UUID));
					this->topologyManager = make_shared<TopologyManager>(appPhysRequest);
				}
				return this->topologyManager;
			}

			shared_ptr<VMManager> ApplicationHooks::getVMManager()
			{
				if (!this->vmManager) {
					shared_ptr<NebuClient> nebuClient = make_shared<NebuClient>(RestClientAdapter::getInstance(),
							CONFIG_GET(CONFIG_NEBU_URL));
					shared_ptr<AppVirtRequest> appVirtRequest = make_shared<AppVirtRequest>(nebuClient,
							CONFIG_GET(CONFIG_APP_UUID));
					this->vmManager = make_shared<VMManager>(appVirtRequest);
				}
				return this->vmManager;
			}

		}
	}
}
