
/** @file
 *  Provides a global hook for initialising the application-specific ApplicationHooks.
 */

#include "nebu-app-framework/applicationHooks.h"

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			/** Global function for initialisation of a singleton implementing the ApplicationHooks interface. */
			std::shared_ptr<ApplicationHooks> initApplication();

		}
	}
}
