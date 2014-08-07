
#include "nebu-app-framework/configuration.h"

#include "log4cxx/logger.h"

#include <sstream>

// Using declarations - standard library
using std::make_shared;
using std::map;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::vector;

static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("nebu.app.framework.Configuration"));

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			shared_ptr<Configuration> Configuration::globalConfiguration;

			map<string, string> Configuration::commandLineOptions {
				{ "--interval", CONFIG_APP_INTERVAL },
				{ "--app",      CONFIG_APP_UUID },
				{ "--nebu",     CONFIG_NEBU_URL }
			};
			map<string, string> Configuration::defaultValues {
				{ CONFIG_APP_INTERVAL, "60" },
				{ CONFIG_APP_UUID, "" },
				{ CONFIG_NEBU_URL, "http://localhost:8080" }
			};

			Configuration::Configuration()
			{
				this->options = Configuration::defaultValues;
			}

			shared_ptr<Configuration> Configuration::getGlobalConfiguration()
			{
				if (!Configuration::globalConfiguration) {
					Configuration::setGlobalConfiguration(make_shared<Configuration>());
				}
				return Configuration::globalConfiguration;
			}

			void Configuration::setGlobalConfiguration(shared_ptr<Configuration> configuration)
			{
				Configuration::globalConfiguration = configuration;
			}

			string Configuration::getOption(const string &option) const
			{
				return this->options.at(option);
			}
			int Configuration::getOptionInt(const string &option) const
			{
				stringstream intAsString(this->getOption(option));
				int result;
				intAsString >> result;
				return result;
			}
			void Configuration::setOption(const string &option, const string &value)
			{
				this->options[option] = value;
			}
			string &Configuration::operator[](const string &option)
			{
				return this->options[option];
			}

			void Configuration::logConfiguration() const
			{
				for (map<string, string>::const_iterator it = this->options.begin();
					it != this->options.end();
					it++)
				{
					LOG4CXX_INFO(logger, it->first << ": " << it->second);
				}
			}

			void Configuration::fromArguments(vector<string> &args)
			{
				shared_ptr<Configuration> cfg = make_shared<Configuration>();
				unsigned int index = 0;
				while (index < args.size()) {
					string arg = args[index];
					if (Configuration::commandLineOptions.count(arg) > 0) {
						string option = Configuration::commandLineOptions[arg];
						string value = "";
						if (index + 1 < args.size()) {
							value = args[index + 1];
						}
						cfg->setOption(option, value);

						if (index + 1 < args.size()) {
							args.erase(args.begin() + index, args.begin() + index + 2);
						} else {
							args.erase(args.begin() + index);
						}
					} else {
						index++;
					}
				}
				Configuration::setGlobalConfiguration(cfg);
			}

			void Configuration::addCommandLineOption(const string &cmd, const string &optionName)
			{
				Configuration::commandLineOptions[cmd] = optionName;
			}

			void Configuration::addDefaultValue(const string &optionName, const string &defaultValue)
			{
				Configuration::defaultValues[optionName] = defaultValue;
			}

		}
	}
}

