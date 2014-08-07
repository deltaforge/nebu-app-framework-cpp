
#include "nebu-app-framework/daemon.h"

// Using declarations - standard library
using std::shared_ptr;
using std::string;
using std::stringstream;
// Using declarations - nebu-common
using nebu::common::VirtualMachine;

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			Daemon::Daemon(shared_ptr<VirtualMachine> hostVM) : launched(false), hostVM(hostVM)
			{
				this->discoveryTime = time(NULL);
			}

			time_t Daemon::secondsSinceDiscovery() const
			{
				return time(NULL) - this->discoveryTime;
			}

// TODO: Is this general enough for inclusion in the framework?
//
//			string Daemon::getFQDN() const
//			{
//				Configuration &config = Configuration::getGlobalConfiguration();
//				string domain = config["nebu.domain"];
//				return this->getHostname() + domain;
//			}

		}
	}
}

