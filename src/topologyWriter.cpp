
#include "nebu-app-framework/topologyWriter.h"

#include "nebu/topology/physicalDataCenter.h"
#include "nebu/topology/physicalHost.h"
#include "nebu/topology/physicalRack.h"

#include <fstream>

// Using declarations - standard library
using std::endl;
using std::ofstream;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;
// Using declarations - nebu-common
using nebu::common::PhysicalDataCenter;
using nebu::common::PhysicalHost;
using nebu::common::PhysicalRack;
using nebu::common::PhysicalRoot;
using nebu::common::VirtualMachine;

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			unordered_map<string, shared_ptr<PhysicalHost>> createHostMap(shared_ptr<PhysicalRoot> topology)
			{
				unordered_map<string, shared_ptr<PhysicalHost>> map;
				for (unordered_map<string, shared_ptr<PhysicalDataCenter>>::const_iterator dc = topology->getDataCenters().begin();
						dc != topology->getDataCenters().end();
						dc++)
				{
					for (unordered_map<string, shared_ptr<PhysicalRack>>::const_iterator rack = dc->second->getRacks().begin();
							rack != dc->second->getRacks().end();
							rack++)
					{
						for (unordered_map<string, shared_ptr<PhysicalHost>>::const_iterator host = rack->second->getHosts().begin();
								host != rack->second->getHosts().end();
								host++)
						{
							map[host->first] = host->second;
						}
					}
				}
				return map;
			}

			void TopologyWriter::write(shared_ptr<PhysicalRoot> topology, vector<shared_ptr<VirtualMachine>> vms)
			{
				unordered_map<string, shared_ptr<PhysicalHost>> hostMap = createHostMap(topology);

				ofstream output(this->filename, ofstream::trunc);
				if (output.is_open()) {
					for (vector<shared_ptr<VirtualMachine>>::iterator vm = vms.begin(); vm != vms.end(); vm++) {
						if (hostMap.find((*vm)->getPhysicalHostID()) != hostMap.end()) {
							PhysicalHost *host = hostMap[(*vm)->getPhysicalHostID()].get();
							PhysicalRack *rack = host->getParent();
							PhysicalDataCenter *dc = rack->getParent();
							output << (*vm)->getHostname() << "\t/" << dc->getUUID() << "/" << rack->getUUID() << "/" << host->getUUID() << endl;
						}
					}
				}
				output.close();
			}

			void TopologyWriter::setFilename(const string &filename)
			{
				this->filename = filename;
			}

		}
	}
}
