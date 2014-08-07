
#include "nebu-app-framework/vmManager.h"

#include "nebu/util/exceptions.h"

#include "log4cxx/logger.h"

#include <set>

// Using declarations - standard library
using std::list;
using std::make_shared;
using std::set;
using std::shared_ptr;
using std::string;
using std::unordered_map;
using std::vector;
// Using declarations - nebu-common
using nebu::common::NebuServerException;
using nebu::common::VirtualMachine;
using nebu::common::VMStatus;

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("nebu.app.framework.VMManager"));

#define FOREACH_EVENTHANDLER(itName) \
	for (list<shared_ptr<VMEventHandler>>::iterator itName = this->vmEventHandlers.begin(); \
		 itName != this->vmEventHandlers.end(); \
		 itName++)

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			bool VMManager::refreshVMList()
			{
				vector<string> vmIDs;
				try {
					vmIDs = this->appVirtRequest->getVirtualMachineIDs();
				} catch (NebuServerException &ex) {
					LOG4CXX_WARN(logger, "Could not refresh VM list\n" + ex.what());
					return false;
				}

				vector<string> vmIDsFiltered(vmIDs);
				bool succes = true;

				succes &= this->addNewVMs(vmIDsFiltered);
				succes &= this->updateVMs(vmIDsFiltered);
				this->removeVMs(vmIDs);

				return succes;
			}

			bool VMManager::addNewVMs(vector<string> &retrievedVMIds)
			{
				bool succes = true;
				for (vector<string>::iterator it = retrievedVMIds.begin();
					 it != retrievedVMIds.end();
					 )
				{
					if (this->vmList.find(*it) == this->vmList.end()) {
						try {
							shared_ptr<VirtualMachine> newVM = make_shared<VirtualMachine>(
									this->appVirtRequest->getVirtualMachine(*it));
							LOG4CXX_INFO(logger, "Detected new VM with hostname '" + newVM->getHostname() +
									"' (id: " + newVM->getUUID() + ")");
							LOG4CXX_DEBUG(logger, "\tHost: " << newVM->getPhysicalHostID() <<
									", store: " << newVM->getPhysicalStoreID() << ", status: " <<
									static_cast<unsigned int>(newVM->getStatus()));
							this->addVM(newVM);
						} catch (NebuServerException &ex) {
							LOG4CXX_WARN(logger, "Missing information on a new vm\n" + ex.what());
							succes = false;
						}

						retrievedVMIds.erase(it);
					} else {
						it++;
					}
				}
				return succes;
			}

			bool VMManager::updateVMs(vector<string> &filteredVMIds)
			{
				bool succes = true;
				for (vector<string>::iterator it = filteredVMIds.begin();
					 it != filteredVMIds.end();
					 it++)
				{
					shared_ptr<VirtualMachine> vm = this->vmList[*it];
					try {
						VirtualMachine updatedVM = this->appVirtRequest->getVirtualMachine(*it);
						if (vm->getStatus() != updatedVM.getStatus()) {
							LOG4CXX_INFO(logger, "Detected change in VM with hostname '" << vm->getHostname() <<
									"' (id: " << vm->getUUID() << ")");
							LOG4CXX_DEBUG(logger, "\tStatus from " << static_cast<unsigned int>(vm->getStatus()) <<
									" to " << static_cast<unsigned int>(updatedVM.getStatus()));

							this->updateVM(vm, updatedVM);
						}
					} catch (NebuServerException &ex) {
						// TODO: Decide: should the VM status be set to UNKNOWN?
						LOG4CXX_WARN(logger, "Missing information for a VM update\n" + ex.what());
						succes = false;
					}
				}
				return succes;
			}

			void VMManager::removeVMs(vector<string> &retrievedVMIds)
			{
				vector<shared_ptr<VirtualMachine>> knownVMs = this->getVMs();
				set<string> vmIDSet(retrievedVMIds.begin(), retrievedVMIds.end());

				for (vector<shared_ptr<VirtualMachine>>::iterator it = knownVMs.begin();
					 it != knownVMs.end();
					 it++)
				{
					if (vmIDSet.find((*it)->getUUID()) == vmIDSet.end()) {
						this->removeVM(*it);
					}
				}
			}

			vector<shared_ptr<VirtualMachine>> VMManager::getVMs() const
			{
				vector<shared_ptr<VirtualMachine>> res;
				for (unordered_map<string, shared_ptr<VirtualMachine>>::const_iterator it = this->vmList.begin();
						it != this->vmList.end();
						it++)
				{
					res.push_back(it->second);
				}
				return res;
			}

			shared_ptr<VirtualMachine> VMManager::getVM(const string &uuid) const
			{
				if (this->hasVM(uuid)) {
					return this->vmList.at(uuid);
				} else {
					return shared_ptr<VirtualMachine>();
				}
			}

			void VMManager::addVM(shared_ptr<VirtualMachine> vm)
			{
				this->vmList[vm->getUUID()] = vm;

				FOREACH_EVENTHANDLER(ev)
				{
					ev->get()->newVMAdded(vm);
				}
			}

			void VMManager::updateVM(shared_ptr<VirtualMachine> vm, const VirtualMachine &updated)
			{
				if (vm->getStatus() != updated.getStatus()) {
					vm->setStatus(updated.getStatus());

					VMEvent event;
					switch (vm->getStatus())
					{
					case VMStatus::ON:
						event = VMEvent::POWERED_ON;
						break;
					case VMStatus::OFF:
						event = VMEvent::POWERED_OFF;
						break;
					case VMStatus::UNKNOWN:
						event = VMEvent::UNKNOWN;
						break;
					}

					FOREACH_EVENTHANDLER(ev)
					{
						ev->get()->existingVMChanged(vm, event);
					}
				}
			}

			void VMManager::removeVM(shared_ptr<VirtualMachine> vm)
			{
				this->vmList.erase(vm->getUUID());

				FOREACH_EVENTHANDLER(ev)
				{
					ev->get()->oldVMRemoved(*vm);
				}
			}

			bool VMManager::hasVM(const string &uuid) const
			{
				return (this->vmList.count(uuid) > 0);
			}

			void VMManager::registerVMEventHandler(shared_ptr<VMEventHandler> eventHandler)
			{
				this->vmEventHandlers.push_back(eventHandler);
			}

		}
	}
}

