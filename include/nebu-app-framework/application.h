
#ifndef NEBUMONGO_APPLICATION_H_
#define NEBUMONGO_APPLICATION_H_

#include <memory>

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			class ApplicationHooks;
			class DaemonManager;
			class TopologyManager;
			class VMManager;

			/** The application loop for a program using libnebu-app-framework. */
			class Application
			{
			public:
				/** Constructor for a Nebu application.
				 *  @param[in] daemonManager a concrete DaemonManager.
				 *  @param[in] topologyManager a concrete TopologyManager.
				 *  @param[in] vmManager a concrete VMManager.
				 */
				Application(std::shared_ptr<DaemonManager> daemonManager,
						std::shared_ptr<TopologyManager> topologyManager, std::shared_ptr<VMManager> vmManager);
				/** Empty destructor provided for inheritance. */
				virtual ~Application() { }

				/** Setter for application-specific hooks.
				 *  @param[in] applicationHooks the hooks used for the specific Nebu application
				 *             built using this framework.
				 */
				virtual void setApplicationHooks(std::shared_ptr<ApplicationHooks> applicationHooks)
				{
					this->applicationHooks = applicationHooks;
				}

				/** Starts the main loop of the application.
				 *  @return exit code.
				 */
				virtual int mainLoop();
				/** Triggers a shutdown of the application.
				 *  The main loop of the application will complete its current iteration.
				 *  Afterwards, the application stops and exits gracefully.
				 */
				virtual void shutdown() { this->stopLoop = true; }

			private:
				bool stopLoop;

				std::shared_ptr<ApplicationHooks> applicationHooks;
				std::shared_ptr<DaemonManager> daemonManager;
				std::shared_ptr<TopologyManager> topologyManager;
				std::shared_ptr<VMManager> vmManager;
			};

		}
	}
}

#endif
