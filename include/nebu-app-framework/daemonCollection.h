
#ifndef NEBUAPPFRAMEWORK_DAEMONCOLLECTION_H_
#define NEBUAPPFRAMEWORK_DAEMONCOLLECTION_H_

#include "nebu-app-framework/daemon.h"

#include <map>
#include <memory>
#include <set>

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			/** Holds a collection of Daemons in the system for easy retrieval.
			 *  Provides several accessor functions to retrieve a subset of all daemons, e.g., based on their type.
			 */
			class DaemonCollection
			{
			public:
				/** Empty constructor. */
				DaemonCollection() { }
				/** Virtual destructor provided for inheritance. */
				virtual ~DaemonCollection() { }

				/** Getter for a set containing all Daemons.
				 *  @return set of Daemon objects.
				 */
				virtual std::set<std::shared_ptr<Daemon>> getDaemons();
				/** Getter for the set of Daemons with a filter based on DaemonType.
				 *  @param[in] type the type of Daemon to return.
				 *  @return set of all Daemons of DaemonType type.
				 */
				virtual std::set<std::shared_ptr<Daemon>> getDaemonsForType(DaemonType type);
				/** Getter for the set of Daemons with a filter based on DaemonType and launched status.
				 *  @param[in] type the type of Daemon to return.
				 *  @return filtered set of Daemons.
				 */
				virtual std::set<std::shared_ptr<Daemon>> getUnlaunchedDaemonsForType(DaemonType type);
				/** Getter for the set of Daemons with a filter based on DaemonType and unlaunched status.
				 *  @param[in] type the type of Daemon to return.
				 *  @return filtered set of Daemons.
				 */
				virtual std::set<std::shared_ptr<Daemon>> getLaunchedDaemonsForType(DaemonType type);
				/** Getter for the set of Daemons with a custom filter.
				 *  @param[in] includeInResults a predicate that should be true iff the Daemon should be included
				 *                          in the result set of the function.
				 *  @return filtered set of Daemons.
				 */
				virtual std::set<std::shared_ptr<Daemon>> getDaemonsFiltered(bool (*includeInResults)(std::shared_ptr<Daemon>));

				/** Adds a Daemon to the collection.
				 *  @param[in] daemon the Daemon to add.
				 */
				virtual void addDaemon(std::shared_ptr<Daemon> daemon);

			private:
				std::map<int, std::set<std::shared_ptr<Daemon>>> daemons;
			};

		}
	}
}

#endif
