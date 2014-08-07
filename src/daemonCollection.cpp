
#include "nebu-app-framework/daemonCollection.h"

// Using declarations - standard library
using std::map;
using std::set;
using std::shared_ptr;
using std::static_pointer_cast;

namespace nebu
{
	namespace app
	{
		namespace framework
		{

			set<shared_ptr<Daemon>> DaemonCollection::getDaemons()
			{
				set<shared_ptr<Daemon>> daemons;
				for (map<int, set<shared_ptr<Daemon>>>::iterator typeSet = this->daemons.begin();
					 typeSet != this->daemons.end();
					 typeSet++)
				{
					daemons.insert(typeSet->second.begin(), typeSet->second.end());
				}
				return daemons;
			}

			set<shared_ptr<Daemon>> DaemonCollection::getDaemonsForType(DaemonType type)
			{
				int itype = static_cast<int>(type);
				if (this->daemons.find(itype) != this->daemons.end()) {
					return this->daemons[itype];
				} else {
					return set<shared_ptr<Daemon>>();
				}
			}

			set<shared_ptr<Daemon>> DaemonCollection::getUnlaunchedDaemonsForType(DaemonType type)
			{
				int itype = static_cast<int>(type);
				if (this->daemons.find(itype) != this->daemons.end()) {
					set<shared_ptr<Daemon>> daemons;
					for (set<shared_ptr<Daemon>>::iterator daemon = this->daemons[itype].begin();
						 daemon != this->daemons[itype].end();
						 daemon++)
					{
						if (!(*daemon)->hasLaunched()) {
							daemons.insert(*daemon);
						}
					}
					return daemons;
				} else {
					return set<shared_ptr<Daemon>>();
				}
			}

			set<shared_ptr<Daemon>> DaemonCollection::getLaunchedDaemonsForType(DaemonType type)
			{
				int itype = static_cast<int>(type);
				if (this->daemons.find(itype) != this->daemons.end()) {
					set<shared_ptr<Daemon>> daemons;
					for (set<shared_ptr<Daemon>>::iterator daemon = this->daemons[itype].begin();
						 daemon != this->daemons[itype].end();
						 daemon++)
					{
						if ((*daemon)->hasLaunched()) {
							daemons.insert(*daemon);
						}
					}
					return daemons;
				} else {
					return set<shared_ptr<Daemon>>();
				}
			}

			set<shared_ptr<Daemon>> DaemonCollection::getDaemonsFiltered(
            		bool (*includeInResults)(std::shared_ptr<Daemon>))
			{
				set<shared_ptr<Daemon>> daemons;
				for (map<int, set<shared_ptr<Daemon>>>::iterator typeSet = this->daemons.begin();
						typeSet != this->daemons.end();
				     	typeSet++)
				{
					for (set<shared_ptr<Daemon>>::iterator daemon = typeSet->second.begin();
							daemon != typeSet->second.end();
							daemon++)
					{
						if (includeInResults(*daemon)) {
							daemons.insert(*daemon);
						}
					}
				}
				return daemons;
			}

			void DaemonCollection::addDaemon(shared_ptr<Daemon> daemon)
			{
				DaemonType type = daemon->getType();
				int itype = static_cast<int>(type);
				if (this->daemons.find(itype) == this->daemons.end()) {
					this->daemons[itype] = set<shared_ptr<Daemon>>();
				}

				this->daemons[itype].insert(daemon);
			}

		}
	}
}

