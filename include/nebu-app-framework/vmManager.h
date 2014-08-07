
#ifndef NEBUAPPFRAMEWORK_VMMANAGER_H_
#define NEBUAPPFRAMEWORK_VMMANAGER_H_

#include "nebu-app-framework/vmEventHandler.h"

#include "nebu/appVirtRequest.h"
#include "nebu/virtualMachine.h"

#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			/** Manages the list and states of VirtualMachines in the application. */
			class VMManager
			{
			public:
				/** Creates a VMManager using the given connection to the Nebu middleware.
				 *  @param[in] appVirtRequest a connection to the Nebu middleware for the AppVirtRequest.
				 */
				VMManager(std::shared_ptr<nebu::common::AppVirtRequest> appVirtRequest) :
					vmEventHandlers(), appVirtRequest(appVirtRequest), vmList() { }
				/** Empty destructor provided for inheritance. */
				virtual ~VMManager() { }

				/** Refreshes the list of VMs and detects any changes in that list.
				 *  @return true iff no exceptions occured in contacting the middleware.
				 */
				virtual bool refreshVMList();

				/** Retrieves a list of all VirtualMachines known to the VMManager.
				 *  @return a vector of VirtualMachines.
				 */
				virtual std::vector<std::shared_ptr<nebu::common::VirtualMachine>> getVMs() const;
				/** Retrieves a single VirtualMachine by its unique identifier.
				 *  @param[in] uuid the unique ID of the VM.
				 *  @return the VirtualMachine.
				 */
				virtual std::shared_ptr<nebu::common::VirtualMachine> getVM(const std::string &uuid) const;
				/** Checks if a VirtualMachine with the given identifier exists.
				 *	@return true iff the VirtualMachine exists.
				 */
				virtual bool hasVM(const std::string &uuid) const;

				/** Registers a VMEventHandler, allowing it to receive notifications of events detected
				 *  in refreshVMList().
				 *  @param[in] eventHandler the VMEventhandler to register.
				 */
				virtual void registerVMEventHandler(std::shared_ptr<VMEventHandler> eventHandler);

			protected:
				void addVM(std::shared_ptr<nebu::common::VirtualMachine> vm);
				void updateVM(std::shared_ptr<nebu::common::VirtualMachine> vm,
						const nebu::common::VirtualMachine &updated);
				void removeVM(std::shared_ptr<nebu::common::VirtualMachine> vm);

				bool addNewVMs(std::vector<std::string> &retrievedVMIds);
				bool updateVMs(std::vector<std::string> &filteredVMIds);
				void removeVMs(std::vector<std::string> &retrievedVMIds);

			private:
				std::list<std::shared_ptr<VMEventHandler>> vmEventHandlers;
				std::shared_ptr<nebu::common::AppVirtRequest> appVirtRequest;
				std::unordered_map<std::string, std::shared_ptr<nebu::common::VirtualMachine>> vmList;
			};

		}
	}
}

#endif

