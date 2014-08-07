
#include "nebu-app-framework/commandRunner.h"

#include "log4cxx/logger.h"

// Using declarations - standard library
using std::make_shared;
using std::shared_ptr;
using std::string;

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("nebu.app.framework.CommandRunner"));

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			shared_ptr<CommandRunner> CommandRunner::instance;

			int CommandRunner::runCommand(const string &command) const
			{
				LOG4CXX_DEBUG(logger, "Executing shell command: '" << command << "'");
				int result = system(command.c_str());
				LOG4CXX_DEBUG(logger, "Shell command returned with: " << result);
				return result;
			}

			shared_ptr<CommandRunner> CommandRunner::getInstance()
			{
				if (!CommandRunner::instance) {
					CommandRunner::setInstance(make_shared<CommandRunner>());
				}
				return CommandRunner::instance;
			}

			void CommandRunner::setInstance(shared_ptr<CommandRunner> instance)
			{
				CommandRunner::instance = instance;
			}

		}
	}
}
