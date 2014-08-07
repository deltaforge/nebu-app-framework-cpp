
#ifndef NEBUAPPFRAMEWORK_VMEVENTHANDLER_H_
#define NEBUAPPFRAMEWORK_VMEVENTHANDLER_H_

#include "nebu/virtualMachine.h"

#include <memory>

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			/** Types of VirtualMachine-related events. */
			enum class VMEvent {
				POWERED_ON,
				POWERED_OFF,
				UNKNOWN
			};

			/** Interface for entities that can be notified of VMEvents.
			 *  VMEventHandlers can be registered with the VMManager to be notified of changes in the system.
			 */
			class VMEventHandler
			{
			public:
				/** Empty constructor provided for inheritance. */
				VMEventHandler() { };
				/** Empty destructor provided for inheritance. */
				virtual ~VMEventHandler() { };

				/** Hook that is called when a new VirtualMachine is detected by the application.
				 *  @param[in] vm the newly discovered VirtualMachine.
				 */
				virtual void newVMAdded(std::shared_ptr<nebu::common::VirtualMachine> vm) = 0;
				/** Hook that is called when an existing VirtualMachine has changed.
				 *  @param[in] vm the VirtualMachine that has changed.
				 *  @param[in] event a VMEvent describing the change.
				 */
				virtual void existingVMChanged(std::shared_ptr<nebu::common::VirtualMachine> vm,
						const VMEvent event) = 0;
				/** Hook that is called when an old VirtualMachine has left the system.
				 *  @param[in] vm the VirtualMachine that has left the system.
				 */
				virtual void oldVMRemoved(const nebu::common::VirtualMachine &vm) = 0;

			};

		}
	}
}

#endif
