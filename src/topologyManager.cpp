
#include "nebu-app-framework/topologyManager.h"

#include "log4cxx/logger.h"

#include "nebu/topology/physicalDataCenter.h"
#include "nebu/topology/physicalHost.h"
#include "nebu/topology/physicalRack.h"
#include "nebu/topology/physicalRoot.h"
#include "nebu/util/exceptions.h"

// Using declarations - standard library
using std::make_shared;
using std::shared_ptr;
using std::string;
// Using declarations - nebu-common
using nebu::common::AppPhysRequest;
using nebu::common::NebuServerException;
using nebu::common::PhysicalDataCenter;
using nebu::common::PhysicalHost;
using nebu::common::PhysicalRack;
using nebu::common::PhysicalRoot;
using nebu::common::Traits;

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("nebu.app.framework.TopologyManager"));

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			const std::string TopologyManager::ID_UNKNOWN = "";

			TopologyManager::TopologyManager(shared_ptr<AppPhysRequest> appPhysRequest) :
					appPhysRequest(appPhysRequest), physicalRoot(make_shared<PhysicalRoot>(TopologyManager::ID_UNKNOWN))
			{

			}

			bool TopologyManager::refreshTopology()
			{
				try {
					LOG4CXX_INFO(logger, "Refreshing topology");
					shared_ptr<PhysicalRoot> physicalRoot = appPhysRequest->getPhysicalTopology();
					if (physicalRoot) {
						this->physicalRoot = physicalRoot;
						LOG4CXX_DEBUG(logger, "Topology refresh succeeded");
						return true;
					} else {
						LOG4CXX_WARN(logger, "Topology refresh returned an invalid tree");
						return false;
					}
				} catch (NebuServerException &ex) {
					LOG4CXX_WARN(logger, "Could not refresh topology\n" + ex.what());
					return false;
				}
			}

			shared_ptr<PhysicalRoot> TopologyManager::getRoot() const
			{
				return this->physicalRoot;
			}

			shared_ptr<PhysicalHost> TopologyManager::getHostByID(const string &hostID) const
			{
				for (Traits<PhysicalDataCenter>::Map::const_iterator it = physicalRoot->getDataCenters().cbegin();
					 it != physicalRoot->getDataCenters().cend();
					 it++)
				{
					shared_ptr<PhysicalHost> host = findHost(it->second, hostID);
					if (host) {
						return host;
					}
				}
				LOG4CXX_DEBUG(logger, "Host with ID " << hostID << " is not part of the topology");
				return shared_ptr<PhysicalHost>();
			}

			shared_ptr<PhysicalRack> TopologyManager::getRackByID(const string &rackID) const
			{
				for (Traits<PhysicalDataCenter>::Map::const_iterator it = physicalRoot->getDataCenters().cbegin();
					 it != physicalRoot->getDataCenters().cend();
					 it++)
				{
					shared_ptr<PhysicalRack> rack = findRack(it->second, rackID);
					if (rack) {
						return rack;
					}
				}
				LOG4CXX_DEBUG(logger, "Rack with ID " << rackID << " is not part of the topology");
				return shared_ptr<PhysicalRack>();
			}

			shared_ptr<PhysicalDataCenter> TopologyManager::getDataCenterByID(const string &dataCenterID) const
			{
				Traits<PhysicalDataCenter>::Map dataCenters = physicalRoot->getDataCenters();
				if (dataCenters.find(dataCenterID) != dataCenters.end()) {
					return dataCenters[dataCenterID];
				} else {
					LOG4CXX_DEBUG(logger, "Data center with ID " << dataCenterID << " is not part of the topology");
					return shared_ptr<PhysicalDataCenter>();
				}
			}

			shared_ptr<PhysicalHost> TopologyManager::findHost(shared_ptr<PhysicalDataCenter> haystack,
					const string &hostID) const
			{
				for (Traits<PhysicalRack>::Map::const_iterator it = haystack->getRacks().cbegin();
					 it != haystack->getRacks().cend();
					 it++)
				{
					shared_ptr<PhysicalHost> host = findHost(it->second, hostID);
					if (host) {
						return host;
					}
				}
				return shared_ptr<PhysicalHost>();
			}

			shared_ptr<PhysicalHost> TopologyManager::findHost(shared_ptr<PhysicalRack> haystack,
					const string &hostID) const
			{
				Traits<PhysicalHost>::Map hosts = haystack->getHosts();
				if (hosts.find(hostID) != hosts.end()) {
					return hosts[hostID];
				} else {
					return shared_ptr<PhysicalHost>();
				}
			}

			shared_ptr<PhysicalRack> TopologyManager::findRack(shared_ptr<PhysicalDataCenter> haystack,
					const string &rackID) const
			{
				Traits<PhysicalRack>::Map racks = haystack->getRacks();
				if (racks.find(rackID) != racks.end()) {
					return racks[rackID];
				} else {
					return shared_ptr<PhysicalRack>();
				}
			}

			string TopologyManager::getRackIDForHost(const string &hostID) const
			{
				shared_ptr<PhysicalHost> host = getHostByID(hostID);
				if (!host) {
					return TopologyManager::ID_UNKNOWN;
				}

				return host->getParent()->getUUID();
			}

			string TopologyManager::getDataCenterIDForHost(const string &hostID) const
			{
				shared_ptr<PhysicalHost> host = getHostByID(hostID);
				if (!host) {
					return TopologyManager::ID_UNKNOWN;
				}

				return host->getParent()->getParent()->getUUID();
			}

		}
	}
}
