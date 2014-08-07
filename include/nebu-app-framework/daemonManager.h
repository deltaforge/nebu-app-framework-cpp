
#ifndef NEBUAPPFRAMEWORK_DAEMONMANAGER_H_
#define NEBUAPPFRAMEWORK_DAEMONMANAGER_H_

#include "nebu-app-framework/vmEventHandler.h"

#include "nebu/virtualMachine.h"

#include <string>
#include <vector>

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			/** Interface for a class managing the Daemons in the application. */
			class DaemonManager : public VMEventHandler
			{
			public:
				/** Empty constructor provided for inheritance. */
				DaemonManager() { }
				/** Empty destructor provided for inheritance. */
				virtual ~DaemonManager() { };

				/** Hook used by the main loop to trigger a refresh of the Daemons. */
				virtual void refreshDaemons() = 0;
				/** Hook used to deploy new Daemons. */
				virtual void deployDaemons() = 0;

				virtual void newVMAdded(std::shared_ptr<nebu::common::VirtualMachine> vm) = 0;
				virtual void existingVMChanged(std::shared_ptr<nebu::common::VirtualMachine> vm,
						const VMEvent event) = 0;
				virtual void oldVMRemoved(const nebu::common::VirtualMachine &vm) = 0;
			};

		}
	}
}

#endif

