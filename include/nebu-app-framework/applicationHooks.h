
#ifndef NEBUAPPFRAMEWORK_APPLICATIONHOOKS_H_
#define NEBUAPPFRAMEWORK_APPLICATIONHOOKS_H_

#include <memory>
#include <string>
#include <vector>

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			class Application;
			class DaemonCollection;
			class DaemonManager;
			class TopologyManager;
			class VMManager;

			/** Interface of all hooks provided to an application built on libnebu-app-framework.
             *  The ApplicationHooks interface should be implemented by a single class in an application.
			 *  It is used for all customisation of the framework for a specific application.
			 *  Several getters allow for dependency injection of key classes into the Application class.
			 *  All actions in the main loop are directly preceded/followed by a pre/post hook, respectively,
			 *  allowing for further customisation of the application.
			 */
			class ApplicationHooks
			{
			public:
				/** Empty constructor provided for inheritance. */
				ApplicationHooks() : application() { }
				/** Empty destructor provided for inheritance. */
				virtual ~ApplicationHooks() { }

				/** Hook provided for registering configuration options.
				 *  This hook is called before command line parsing.
				 */
				virtual void registerConfigurationOptions() { }
				/** Hook provided for initialising the logging framework.
				 *  The provided implementation defaults to writing to standard output at a DEBUG level.
				 *  This hook is called after the command line arguments have been parsed, but before the
				 *  application has initialised.
				 */
				virtual void prepareLogging();
				/** Hook provided for initialising the application using command line arguments.
				 *  This hook is called after the command line has been parsed for known configuration options,
				 *  and after the logging framework has been configured.
				 *  @param[in] command the value of <code>argv[0]</code> as passed to <code>main</code>.
				 *  @param[in] arguments command line arguments passed to the application, excluding any
				 *                       arguments that have been parsed by the Configuration class.
				 */
				virtual void initialise(std::string command __attribute__((unused)),
						std::vector<std::string> &arguments __attribute__((unused))) { }

				/** Hook called at the start of the main loop, before any other action takes place. */
				virtual void preLoop() { }
				/** Hook called before the VMManager's list of VMs is refreshed */
				virtual void preRefreshVMs() { }
				/** Hook called after the VMManager's list of VMs has been refreshed */
				virtual void postRefreshVMs() { }
				/** Hook called before the TopologyManager's topology representation is refreshed */
				virtual void preRefreshTopology() { }
				/** Hook called after the TopologyManager's topology representation has been refreshed */
				virtual void postRefreshTopology() { }
				/** Hook called before the DaemonManager refreshes its information on Daemons */
				virtual void preRefreshDaemons() { }
				/** Hook called after the DaemonManager has refreshed its information on Daemons */
				virtual void postRefreshDaemons() { }
				/** Hook called before the DaemonManager deploys Daemmons */
				virtual void preDeployDaemons() { }
				/** Hook called after the DaemonManager deploys Daemons */
				virtual void postDeployDaemons() { }
				/** Hook called at the end of the main loop, before the application sleeps for a configured interval. */
				virtual void postLoop() { }

				/** Getter for a concrete DaemonManager object, should be singleton.
				 *  @return a DaemonManager object.
				 */
				virtual std::shared_ptr<DaemonManager> getDaemonManager() = 0;

				/** Getter for a DaemonCollection object, should be singleton.
				 *  The provided implementation returns a singleton of the DaemonCollection class.
				 *  @return a DaemonCollection object.
				 */
				virtual std::shared_ptr<DaemonCollection> getDaemonCollection();
				/** Getter for a TopologyManager object, should be singleton.
				 *  The provided implementation returns a singleton of the TopologyManager class.
				 *  @return a TopologyManager object.
				 */
				virtual std::shared_ptr<TopologyManager> getTopologyManager();
				/** Getter for a VMManager object, should be singleton.
				 *  The provided implementation returns a singleton of the VMManager class.
				 *  @return a VMManager object.
				 */
				virtual std::shared_ptr<VMManager> getVMManager();

				/** Setter for the Application singleton, for use by the implementing Nebu application. */
				virtual void setApplication(std::shared_ptr<Application> application)
				{
					this->application = application;
				}

			protected:
				/** The Application singleton */
				std::shared_ptr<Application> application;

			private:
				std::shared_ptr<DaemonCollection> daemonCollection;
				std::shared_ptr<TopologyManager> topologyManager;
				std::shared_ptr<VMManager> vmManager;
			};

			/** Initialization function to create application-specific hooks and singletons.
			 *  This function must be provided by the application using libnebu-app-framework.
			 *  @return an object implementing the application-specific hooks.
			 */
			std::shared_ptr<ApplicationHooks> initApplication();

		}
	}
}

#endif
