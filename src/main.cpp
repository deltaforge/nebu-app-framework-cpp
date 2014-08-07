
#include "nebu-app-framework/application.h"
#include "nebu-app-framework/applicationHooks.h"
#include "nebu-app-framework/configuration.h"

#include <string>
#include <vector>

// Using declarations - standard library
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::vector;

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			int main(int argc, char *argv[])
			{
				shared_ptr<ApplicationHooks> applicationHooks = initApplication();
				applicationHooks->registerConfigurationOptions();

				vector<string> arguments;
				for (int i = 1; i < argc; i++) {
					arguments.push_back(argv[i]);
				}
				Configuration::fromArguments(arguments);

				applicationHooks->prepareLogging();

				applicationHooks->initialise(argv[0], arguments);

				shared_ptr<Application> application = make_shared<Application>(
						applicationHooks->getDaemonManager(),
						applicationHooks->getTopologyManager(),
						applicationHooks->getVMManager());

				application->setApplicationHooks(applicationHooks);
				applicationHooks->setApplication(application);

				application->mainLoop();

				return 0;
			}

		}
	}
}


int main(int argc, char *argv[])
{
	return nebu::app::framework::main(argc, argv);
}
