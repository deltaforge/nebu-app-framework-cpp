
/** @file */

#ifndef NEBUAPPFRAMEWORK_CONFIGURATION_H_
#define NEBUAPPFRAMEWORK_CONFIGURATION_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#define CONFIG_APP_INTERVAL "app.interval"
#define CONFIG_APP_UUID     "app.uuid"
#define CONFIG_NEBU_URL     "nebu.url"

/** Convenience wrapper for \link nebu::app::framework::Configuration::getOption(const std::string &option) const getOption \endlink on the global instance. */
#define CONFIG_GET(x) nebu::app::framework::Configuration::getGlobalConfiguration()->getOption(x)
/** Convenience wrapper for \link nebu::app::framework::Configuration::getOptionInt(const std::string &option) const getOptionInt \endlink on the global instance. */
#define CONFIG_GETINT(x) nebu::app::framework::Configuration::getGlobalConfiguration()->getOptionInt(x)

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			/** Class holding the configuration of an application */
			class Configuration
			{
			public:
				/** Creates a Configuration with globaly set default values. */
				Configuration();
				/** Empty destructor provided for inheritance. */
				virtual ~Configuration() { }

				/** Retrieves the value of an option.
				 *  @param[in] option the name of the option.
				 *  @throws std::out_of_range if the option does not exist.
				 *  @return the value of the option.
				 */
				std::string getOption(const std::string &option) const;
				/** Retrieves the value of an option as an integer.
				 *  @param[in] option the name of the option.
				 *  @throws std::out_of_range if the option does not exist.
				 *  @return the value of the option interpreted as an integer.
				 */
				int getOptionInt(const std::string &option) const;
				/** Sets the value of an option.
				 *  Overrides the previous value if it exists.
				 *  @param[in] option the option to set.
				 *  @param[in] value the value of the option.
				 */
				void setOption(const std::string &option, const std::string &value);
				/** Retrieve a reference to an option.
				 *  @param[in] option the option to retrieve.
				 *  @return a reference to the option.
				 */
				std::string &operator[](const std::string &option);
				/** Write all configuration options to the logger */
				void logConfiguration() const;

				/** Creates a new Configuration using a command-line argument parser. */
				static void fromArguments(std::vector<std::string> &args);
				/** Adds a new command line option to the global map of accepted options. */
				static void addCommandLineOption(const std::string &cmd, const std::string &optionName);
				/** Adds a default value for a configuration option. */
				static void addDefaultValue(const std::string &optionName, const std::string &defaultValue);

				/** Retrieves the global Configuration.
				 *  @return the global Configuration.
				 */
				static std::shared_ptr<Configuration> getGlobalConfiguration();
				/** Sets the global Configuration to the given value.
				 *  @param configuration the new global Configuration.
				 */
				static void setGlobalConfiguration(std::shared_ptr<Configuration> configuration);

			private:
				static std::shared_ptr<Configuration> globalConfiguration;

				std::map<std::string, std::string> options;
				static std::map<std::string, std::string> commandLineOptions;
				static std::map<std::string, std::string> defaultValues;
			};

		}
	}
}

#endif

