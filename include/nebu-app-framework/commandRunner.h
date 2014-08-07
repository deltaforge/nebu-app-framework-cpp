
/** @file */

#ifndef NEBUAPPFRAMEWORK_COMMANDRUNNER_H_
#define NEBUAPPFRAMEWORK_COMMANDRUNNER_H_

#include <memory>
#include <string>

/** Convenience wrapper for \link nebu::app::framework::CommandRunner::runCommand(const std::string &) const runCommand \endlink on the global instance. */
#define NEBU_RUNCOMMAND(cmd) nebu::app::framework::CommandRunner::getInstance()->runCommand(cmd)

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			/** Wrapper class for executing commands using the system function.
			 *  For most applications, the NEBU_RUNCOMMAND(cmd) wrapper should be used.
			 */
			class CommandRunner
			{
			public:
				/** Empty constructor. */
				CommandRunner() { }
				/** Empty destructor provided for inheritance. */
				virtual ~CommandRunner() { }

				/** Executes a command using the system function.
				 *  @param[in] command the command to be executed.
				 *  @return the exit status of the command.
				 */
				virtual int runCommand(const std::string &command) const;

				/** Getter of the global instance of the CommandRunner class.
				 *  @return the global instance.
				 */
				static std::shared_ptr<CommandRunner> getInstance();
				/** Setter of the  global instance of the CommandRunner class.
				 *  @param[in] instance the global instance.
				 */
				static void setInstance(std::shared_ptr<CommandRunner> instance);

			private:
				static std::shared_ptr<CommandRunner> instance;
			};

		}
	}
}

#endif
