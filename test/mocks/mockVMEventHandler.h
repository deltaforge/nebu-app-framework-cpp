
#ifndef NEBUAPPFRAMEWORK_TEST_MOCKVMEVENTHANDLER_H_
#define NEBUAPPFRAMEWORK_TEST_MOCKVMEVENTHANDLER_H_

#include "nebu-app-framework/vmEventHandler.h"

#include "gmock/gmock.h"

namespace nebu
{
	namespace app
	{
		namespace framework
		{
			namespace test
			{

				class MockVMEventHandler : public VMEventHandler
				{
				public:
					MockVMEventHandler() : VMEventHandler() { }
					virtual ~MockVMEventHandler() { }

					MOCK_METHOD1(newVMAdded, void(std::shared_ptr<nebu::common::VirtualMachine> vm));
					MOCK_METHOD2(existingVMChanged, void(std::shared_ptr<nebu::common::VirtualMachine> vm,
							const VMEvent event));
					MOCK_METHOD1(oldVMRemoved, void(const nebu::common::VirtualMachine &vm));
				};

			}
		}
	}
}

#endif
