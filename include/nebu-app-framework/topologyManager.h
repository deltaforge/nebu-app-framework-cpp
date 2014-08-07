
#ifndef NEBUAPPFRAMEWORK_TOPOLOGYMANAGER_H_
#define NEBUAPPFRAMEWORK_TOPOLOGYMANAGER_H_

#include "nebu/appPhysRequest.h"
#include "nebu/topology/physicalDataCenter.h"
#include "nebu/topology/physicalHost.h"
#include "nebu/topology/physicalRack.h"
#include "nebu/topology/physicalRoot.h"

#include <string>

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			/** Manages the representation of the physical topology as exposed to the application. */
			class TopologyManager
			{
			public:
				/** Creates a new TopologyManager using a given connection to the Nebu middleware.
				 *  @param[in] appPhysRequest a connection to the Nebu middleware for the AppPhysRequest.
				 */
				TopologyManager(std::shared_ptr<nebu::common::AppPhysRequest> appPhysRequest);
				/** Empty destructor provided for inheritance. */
				virtual ~TopologyManager() { };

				/** Refreshes the representation of the physical topology.
				 *  @return true iff the refresh succeeded.
				 */
				virtual bool refreshTopology();

				/** Getter for the root of the physical topology.
				 *  @return the PhysicalRoot of the topology.
				 */
				virtual std::shared_ptr<nebu::common::PhysicalRoot> getRoot() const;

				/** Getter for PhysicalHost based on its unique identifier in the toplogy.
				 *  @param[in] hostID the unique ID of the host.
				 *  @return the requested host, or an empty pointer if it was not found.
				 */
				virtual std::shared_ptr<nebu::common::PhysicalHost> getHostByID(const std::string &hostID) const;
				/** Getter for PhysicalRack based on its unique identifier in the toplogy.
				 *  @param[in] rackID the unique ID of the rack.
				 *  @return the requested rack, or an empty pointer if it was not found.
				 */
				virtual std::shared_ptr<nebu::common::PhysicalRack> getRackByID(const std::string &rackID) const;
				/** Getter for PhysicalDataCenter based on its unique identifier in the toplogy.
				 *  @param[in] dataCenterID the unique ID of the data center.
				 *  @return the requested data center, or an empty pointer if it was not found.
				 */
				virtual std::shared_ptr<nebu::common::PhysicalDataCenter> getDataCenterByID(
						const std::string &dataCenterID) const;

				/** Getter for the ID of the rack containing the specified host.
				 *  @param[in] hostID the unique ID of a host.
				 *  @return the unique ID of the rack containing the host,
				 *          or ID_UNKNOWN if the host is not found.
				 */
				virtual std::string getRackIDForHost(const std::string &hostID) const;
				/** Getter for the ID of the data center containing the specified host.
				 *  @param[in] hostID the unique ID of a host.
				 *  @return the unique ID of the data center containing the host,
				 *          or ID_UNKNOWN if the host is not found.
				 */
				virtual std::string getDataCenterIDForHost(const std::string &hostID) const;

				/** A constant representing an entity that has not been found. */
				static const std::string ID_UNKNOWN;

			private:
				std::shared_ptr<nebu::common::AppPhysRequest> appPhysRequest;
				std::shared_ptr<nebu::common::PhysicalRoot> physicalRoot;

				std::shared_ptr<nebu::common::PhysicalHost> findHost(
						std::shared_ptr<nebu::common::PhysicalDataCenter> haystack, const std::string &hostID) const;
				std::shared_ptr<nebu::common::PhysicalHost> findHost(
						std::shared_ptr<nebu::common::PhysicalRack> haystack, const std::string &hostID) const;
				std::shared_ptr<nebu::common::PhysicalRack> findRack(
						std::shared_ptr<nebu::common::PhysicalDataCenter> haystack, const std::string &rackID) const;
			};

		}
	}
}

#endif
