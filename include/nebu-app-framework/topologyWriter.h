
#ifndef NEBUAPPFRAMEWORK_TOPOLOGYWRITER_H_
#define NEBUAPPFRAMEWORK_TOPOLOGYWRITER_H_

#include "nebu/topology/physicalRoot.h"
#include "nebu/virtualMachine.h"

#include <vector>

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			/** Utility class used to output the topology to a file. */
			class TopologyWriter
			{
			public:
				/** Empty constructor. */
				TopologyWriter() { }
				/** Empty destructor provided for inheritance. */
				virtual ~TopologyWriter() { }

				/** Writes a mapping from hostname to physical location to a file.
				 *  Uses the filename specified through \link setFilename(const std::string &) setFilename \endlink
				 *  @param[in] topology the physical topology hosting the virtualised application.
				 *  @param[in] vms a vector of VirtualMachines to map to the topology.
				 */
				virtual void write(std::shared_ptr<nebu::common::PhysicalRoot> topology,
						std::vector<std::shared_ptr<nebu::common::VirtualMachine>> vms);

				/** Sets a filename for use by the writer.
				 *  @param filename the filename to set.
				 */
				virtual void setFilename(const std::string &filename);

			protected:
				std::string filename;
			};

		}
	}
}

#endif
