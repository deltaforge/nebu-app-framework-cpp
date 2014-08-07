
/** @file */

#ifndef NEBUAPPFRAMEWORK_DAEMON_H_
#define NEBUAPPFRAMEWORK_DAEMON_H_

#include "nebu/virtualMachine.h"

#include <memory>
#include <time.h>

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			/** Represents the type of a Daemon.
			 *  Values can be assigned by an application, and are not explicitly used by the framework.
			 */
			typedef unsigned int DaemonType;

			/**	Represents a daemon managed by the application.
			 *  Daemons are managed by a DaemonManager and are contained in a DaemonCollection.
			 */
			class Daemon
			{
			public:
				/** Creates a new Daemon on the given VirtualMachine.
				 *  @param hostVM the VirtualMachine hosting the Daemon.
				 */
				Daemon(std::shared_ptr<nebu::common::VirtualMachine> hostVM);
				/** Empty destructor provided for inheritance */
				virtual ~Daemon() { }

				/** Getter for the VirtualMachine hosting the Daemon, as specified in the constructor.
				 *  @return the VirtualMachine hosting the Daemon.
				 */
				virtual std::shared_ptr<nebu::common::VirtualMachine> getHostVM() const
				{
					return this->hostVM;
				}

				/** Getter for the hostname of the VirtualMachine that hosts the Daemon.
				 *  @return the hostname of the hosting VirtualMachine.
				 */
				virtual std::string getHostname() const
				{
					return hostVM->getHostname();
				}

				/** Launches the Daemon.
				 *  Must be implemented by subclasses, as launching is specific to the type of Daemon.
				 *  @return true iff the launch was succesful.
				 */
				virtual bool launch() = 0;
				/** Checks if the Daemon has been launched.
				 *  @return true iff the Daemon has launched succesfully.
				 */
				virtual bool hasLaunched() const
				{
					return this->launched;
				}

				/** Getter for the type of the Daemon.
				 *  Must be implemented, and should be unique for different types of Daemons.
				 *  @return the type of the Daemon.
				 */
				virtual DaemonType getType() const = 0;

				/** Caclulates the amount of seconds that have passed since the Daemon was discovered.
				 *  The 'discovery time' of the Daemon is defined as the moment the Daemon object was created.
				 *  @return amount of seconds elapsed since discovery.
				 */
				virtual time_t secondsSinceDiscovery() const;

			protected:
				/** The discovery time of the Daemon. */
				time_t discoveryTime;
				/** True iff the Daemon has been launched succesfully. */
				bool launched;
				/** The VirtualMachine hosting the Daemon. */
				std::shared_ptr<nebu::common::VirtualMachine> hostVM;
			};

		}
	}
}

#endif
