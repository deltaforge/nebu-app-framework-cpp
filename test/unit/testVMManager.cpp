
#include "nebu-app-framework/vmManager.h"
#include "nebu/mocks/mockAppVirtRequest.h"
#include "nebu/util/exceptions.h"
#include "mocks/mockVMEventHandler.h"

#include "log4cxx/basicconfigurator.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

// Using declarations - standard library
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::vector;
// Using declarations - nebu-app-framework
using nebu::app::framework::VMEvent;
using nebu::app::framework::VMManager;
// Using declarations - nebu-common
using nebu::common::NebuServerException;
using nebu::common::VirtualMachine;
using nebu::common::VMStatus;
// Using declarations - nebu mocks
using nebu::app::framework::test::MockVMEventHandler;
using nebu::test::MockAppVirtRequest;
// Using declarations - gtest/gmock
using testing::AtLeast;
using testing::Eq;
using testing::IsNull;
using testing::Pointee;
using testing::Return;
using testing::Throw;
using testing::_;

VirtualMachine vmAOff("vmA");
VirtualMachine vmBOff("vmB");
VirtualMachine vmCOff("vmC");
VirtualMachine vmAOn("vmA");
VirtualMachine vmBOn("vmB");
VirtualMachine vmCOn("vmC");
VirtualMachine vmCUnknown("vmC");
vector<string> vmList { "vmA", "vmB", "vmC" };

void prepareVMs() {
	vmAOff.setStatus(VMStatus::OFF); vmBOff.setStatus(VMStatus::OFF); vmCOff.setStatus(VMStatus::OFF);
	vmAOn.setStatus(VMStatus::ON); vmBOn.setStatus(VMStatus::ON); vmCOn.setStatus(VMStatus::ON);
	vmCUnknown.setStatus(VMStatus::UNKNOWN);
}

void rigFailAllVMs(shared_ptr<MockAppVirtRequest> mockRequest) {
	EXPECT_CALL(*mockRequest, getVirtualMachineIDs()).WillOnce(Return(vmList));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmA")).WillOnce(Throw(NebuServerException("")));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmB")).WillOnce(Throw(NebuServerException("")));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmC")).WillOnce(Throw(NebuServerException("")));
}

void rigFailVMB(shared_ptr<MockAppVirtRequest> mockRequest) {
	EXPECT_CALL(*mockRequest, getVirtualMachineIDs()).WillOnce(Return(vmList));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmA")).WillOnce(Return(vmAOff));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmB")).WillOnce(Throw(NebuServerException("")));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmC")).WillOnce(Return(vmCOff));
}

void rigFailVMC(shared_ptr<MockAppVirtRequest> mockRequest) {
	EXPECT_CALL(*mockRequest, getVirtualMachineIDs()).WillOnce(Return(vmList));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmA")).WillOnce(Return(vmAOff));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmB")).WillOnce(Return(vmBOn));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmC")).WillOnce(Throw(NebuServerException("")));
}

void rigSucces(shared_ptr<MockAppVirtRequest> mockRequest) {
	EXPECT_CALL(*mockRequest, getVirtualMachineIDs()).WillOnce(Return(vmList));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmA")).WillOnce(Return(vmAOff));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmB")).WillOnce(Return(vmBOn));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmC")).WillOnce(Return(vmCOff));
}

void rigUpdatedPowerOnA(shared_ptr<MockAppVirtRequest> mockRequest) {
	EXPECT_CALL(*mockRequest, getVirtualMachineIDs()).WillOnce(Return(vmList));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmA")).WillOnce(Return(vmAOn));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmB")).WillOnce(Return(vmBOn));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmC")).WillOnce(Return(vmCOff));
}

void rigUpdatedPowerOffB(shared_ptr<MockAppVirtRequest> mockRequest) {
	EXPECT_CALL(*mockRequest, getVirtualMachineIDs()).WillOnce(Return(vmList));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmA")).WillOnce(Return(vmAOff));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmB")).WillOnce(Return(vmBOff));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmC")).WillOnce(Return(vmCOff));
}

void rigUpdatedUnknownC(shared_ptr<MockAppVirtRequest> mockRequest) {
	EXPECT_CALL(*mockRequest, getVirtualMachineIDs()).WillOnce(Return(vmList));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmA")).WillOnce(Return(vmAOff));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmB")).WillOnce(Return(vmBOn));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmC")).WillOnce(Return(vmCUnknown));
}

void rigUpdatedPowerOnCFailA(shared_ptr<MockAppVirtRequest> mockRequest) {
	EXPECT_CALL(*mockRequest, getVirtualMachineIDs()).WillOnce(Return(vmList));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmA")).WillOnce(Throw(NebuServerException("")));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmB")).WillOnce(Return(vmBOn));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmC")).WillOnce(Return(vmCOn));
}

void rigRemovedAC(shared_ptr<MockAppVirtRequest> mockRequest) {
	vector<string> shrunkList { "vmB" };
	EXPECT_CALL(*mockRequest, getVirtualMachineIDs()).WillOnce(Return(shrunkList));
	EXPECT_CALL(*mockRequest, getVirtualMachine("vmB")).WillOnce(Return(vmBOn));
}

TEST(VMManagerTest, testConstructorSetsVMsEmpty) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	EXPECT_THAT(vmManager.getVMs().size(), Eq(0));
}

TEST(VMManagerTest, testRefreshVMListWithServerError) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	EXPECT_CALL(*mockRequest, getVirtualMachineIDs()).WillOnce(Throw(NebuServerException("")));

	EXPECT_THAT(vmManager.refreshVMList(), Eq(false));
}

TEST(VMManagerTest, testRefreshVMListWithEmptyList) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	EXPECT_CALL(*mockRequest, getVirtualMachineIDs()).WillOnce(Return(vector<string>()));

	EXPECT_THAT(vmManager.refreshVMList(), Eq(true));
	EXPECT_THAT(vmManager.getVMs().size(), Eq(0));
}

TEST(VMManagerTest, testRefreshVMListWithServerErrorOnVirtualMachineInfo) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigFailVMB(mockRequest);

	EXPECT_THAT(vmManager.refreshVMList(), Eq(false));
}

TEST(VMManagerTest, testRefreshVMListWithSucces) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigSucces(mockRequest);

	EXPECT_THAT(vmManager.refreshVMList(), Eq(true));
}

TEST(VMManagerTest, testRefreshVMListRepeated) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigFailVMB(mockRequest);
	EXPECT_THAT(vmManager.refreshVMList(), Eq(false));

	rigFailVMC(mockRequest);
	EXPECT_THAT(vmManager.refreshVMList(), Eq(false));

	rigSucces(mockRequest);
	EXPECT_THAT(vmManager.refreshVMList(), Eq(true));
}

TEST(VMManagerTest, testRefreshVMListAddVMsWithServerErrorAllVirtualMachines) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigFailAllVMs(mockRequest);
	vmManager.refreshVMList();

	EXPECT_THAT(vmManager.getVMs().size(), Eq(0));
}

TEST(VMManagerTest, testRefreshVMListAddVMsWithServerErrorOneVirtualMachines) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigFailVMB(mockRequest);
	vmManager.refreshVMList();

	EXPECT_THAT(vmManager.getVMs().size(), Eq(2));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmAOff))));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmCOff))));
}

TEST(VMManagerTest, testRefreshVMListAddVMsSucces) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigSucces(mockRequest);
	vmManager.refreshVMList();

	EXPECT_THAT(vmManager.getVMs().size(), Eq(3));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmAOff))));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmBOn))));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmCOff))));
}

TEST(VMManagerTest, testRefreshVMListAddVMsSuccesAfterRetry) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigFailVMB(mockRequest);
	vmManager.refreshVMList();

	rigSucces(mockRequest);
	vmManager.refreshVMList();

	EXPECT_THAT(vmManager.getVMs().size(), Eq(3));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmAOff))));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmBOn))));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmCOff))));
}

TEST(VMManagerTest, testRefreshVMListUdpateVMsPowerOn) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigSucces(mockRequest);
	vmManager.refreshVMList();

	rigUpdatedPowerOnA(mockRequest);
	vmManager.refreshVMList();

	EXPECT_THAT(vmManager.getVMs().size(), Eq(3));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmAOn))));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmBOn))));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmCOff))));
}

TEST(VMManagerTest, testRefreshVMListUdpateVMsPowerOff) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigSucces(mockRequest);
	vmManager.refreshVMList();

	rigUpdatedPowerOffB(mockRequest);
	vmManager.refreshVMList();

	EXPECT_THAT(vmManager.getVMs().size(), Eq(3));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmAOff))));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmBOff))));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmCOff))));
}

TEST(VMManagerTest, testRefreshVMListUdpateVMsUnknown) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigSucces(mockRequest);
	vmManager.refreshVMList();

	rigUpdatedUnknownC(mockRequest);
	vmManager.refreshVMList();

	EXPECT_THAT(vmManager.getVMs().size(), Eq(3));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmAOff))));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmBOn))));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmCUnknown))));
}

TEST(VMManagerTest, testRefreshVMListUdpateVMsConsistency) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigSucces(mockRequest);
	vmManager.refreshVMList();
	rigUpdatedPowerOnCFailA(mockRequest);
	vmManager.refreshVMList();

	EXPECT_THAT(vmManager.getVMs().size(), Eq(3));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmAOff))));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmBOn))));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmCOn))));
}

TEST(VMManagerTest, testRefreshVMListRemoveVMs) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigSucces(mockRequest);
	vmManager.refreshVMList();
	rigRemovedAC(mockRequest);
	vmManager.refreshVMList();

	EXPECT_THAT(vmManager.getVMs().size(), Eq(1));
	EXPECT_THAT(vmManager.getVMs(), testing::Contains(Pointee(Eq(vmBOn))));
}

TEST(VMManagerTest, testHasVMEmptyList) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	EXPECT_THAT(vmManager.hasVM("vmNotFound"), Eq(false));
}

TEST(VMManagerTest, testHasVMFalse) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigSucces(mockRequest);
	vmManager.refreshVMList();

	EXPECT_THAT(vmManager.hasVM("vmNotFound"), Eq(false));
}

TEST(VMManagerTest, testHasVMTrue) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigSucces(mockRequest);
	vmManager.refreshVMList();

	EXPECT_THAT(vmManager.hasVM("vmA"), Eq(true));
}

TEST(VMManagerTest, testGetVMEmptyList) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	EXPECT_THAT(vmManager.getVM("vmNotFound"), IsNull());
}

TEST(VMManagerTest, testGetVMNonExistent) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigSucces(mockRequest);
	vmManager.refreshVMList();

	EXPECT_THAT(vmManager.getVM("vmNotFound"), IsNull());
}

TEST(VMManagerTest, testGetVMSucces) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	VMManager vmManager(mockRequest);

	rigSucces(mockRequest);
	vmManager.refreshVMList();

	EXPECT_THAT(vmManager.getVM("vmA"), Pointee(Eq(vmAOff)));
}

TEST(VMManagerTest, testNotifyAddVM) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	shared_ptr<MockVMEventHandler> mockEventHandler = make_shared<MockVMEventHandler>();
	VMManager vmManager(mockRequest);
	vmManager.registerVMEventHandler(mockEventHandler);

	EXPECT_CALL(*mockEventHandler, newVMAdded(Pointee(Eq(vmAOff))));
	EXPECT_CALL(*mockEventHandler, newVMAdded(Pointee(Eq(vmBOn))));
	EXPECT_CALL(*mockEventHandler, newVMAdded(Pointee(Eq(vmCOff))));

	rigSucces(mockRequest);
	vmManager.refreshVMList();
}

TEST(VMManagerTest, testNotifyMultipleAddVM) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	shared_ptr<MockVMEventHandler> mockEventHandlerA = make_shared<MockVMEventHandler>();
	shared_ptr<MockVMEventHandler> mockEventHandlerB = make_shared<MockVMEventHandler>();
	VMManager vmManager(mockRequest);
	vmManager.registerVMEventHandler(mockEventHandlerA);
	vmManager.registerVMEventHandler(mockEventHandlerB);

	EXPECT_CALL(*mockEventHandlerA, newVMAdded(Pointee(Eq(vmAOff))));
	EXPECT_CALL(*mockEventHandlerA, newVMAdded(Pointee(Eq(vmBOn))));
	EXPECT_CALL(*mockEventHandlerA, newVMAdded(Pointee(Eq(vmCOff))));
	EXPECT_CALL(*mockEventHandlerB, newVMAdded(Pointee(Eq(vmAOff))));
	EXPECT_CALL(*mockEventHandlerB, newVMAdded(Pointee(Eq(vmBOn))));
	EXPECT_CALL(*mockEventHandlerB, newVMAdded(Pointee(Eq(vmCOff))));

	rigSucces(mockRequest);
	vmManager.refreshVMList();
}

TEST(VMManagerTest, testNotifyUpdatePowerOn) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	shared_ptr<MockVMEventHandler> mockEventHandler = make_shared<MockVMEventHandler>();
	VMManager vmManager(mockRequest);
	vmManager.registerVMEventHandler(mockEventHandler);

	EXPECT_CALL(*mockEventHandler, newVMAdded(_)).Times(AtLeast(0));
	EXPECT_CALL(*mockEventHandler, existingVMChanged(Pointee(Eq(vmAOn)), VMEvent::POWERED_ON));

	rigSucces(mockRequest);
	vmManager.refreshVMList();
	rigUpdatedPowerOnA(mockRequest);
	vmManager.refreshVMList();
}

TEST(VMManagerTest, testNotifyMultipleUpdatePowerOn) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	shared_ptr<MockVMEventHandler> mockEventHandlerA = make_shared<MockVMEventHandler>();
	shared_ptr<MockVMEventHandler> mockEventHandlerB = make_shared<MockVMEventHandler>();
	VMManager vmManager(mockRequest);
	vmManager.registerVMEventHandler(mockEventHandlerA);
	vmManager.registerVMEventHandler(mockEventHandlerB);

	EXPECT_CALL(*mockEventHandlerA, newVMAdded(_)).Times(AtLeast(0));
	EXPECT_CALL(*mockEventHandlerA, existingVMChanged(Pointee(Eq(vmAOn)), VMEvent::POWERED_ON));
	EXPECT_CALL(*mockEventHandlerB, newVMAdded(_)).Times(AtLeast(0));
	EXPECT_CALL(*mockEventHandlerB, existingVMChanged(Pointee(Eq(vmAOn)), VMEvent::POWERED_ON));

	rigSucces(mockRequest);
	vmManager.refreshVMList();
	rigUpdatedPowerOnA(mockRequest);
	vmManager.refreshVMList();
}

TEST(VMManagerTest, testNotifyUpdatePowerOff) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	shared_ptr<MockVMEventHandler> mockEventHandler = make_shared<MockVMEventHandler>();
	VMManager vmManager(mockRequest);
	vmManager.registerVMEventHandler(mockEventHandler);

	EXPECT_CALL(*mockEventHandler, newVMAdded(_)).Times(AtLeast(0));
	EXPECT_CALL(*mockEventHandler, existingVMChanged(Pointee(Eq(vmBOff)), VMEvent::POWERED_OFF));

	rigSucces(mockRequest);
	vmManager.refreshVMList();
	rigUpdatedPowerOffB(mockRequest);
	vmManager.refreshVMList();
}

TEST(VMManagerTest, testNotifyMultipleUpdatePowerOff) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	shared_ptr<MockVMEventHandler> mockEventHandlerA = make_shared<MockVMEventHandler>();
	shared_ptr<MockVMEventHandler> mockEventHandlerB = make_shared<MockVMEventHandler>();
	VMManager vmManager(mockRequest);
	vmManager.registerVMEventHandler(mockEventHandlerA);
	vmManager.registerVMEventHandler(mockEventHandlerB);

	EXPECT_CALL(*mockEventHandlerA, newVMAdded(_)).Times(AtLeast(0));
	EXPECT_CALL(*mockEventHandlerA, existingVMChanged(Pointee(Eq(vmBOff)), VMEvent::POWERED_OFF));
	EXPECT_CALL(*mockEventHandlerB, newVMAdded(_)).Times(AtLeast(0));
	EXPECT_CALL(*mockEventHandlerB, existingVMChanged(Pointee(Eq(vmBOff)), VMEvent::POWERED_OFF));

	rigSucces(mockRequest);
	vmManager.refreshVMList();
	rigUpdatedPowerOffB(mockRequest);
	vmManager.refreshVMList();
}

TEST(VMManagerTest, testNotifyUpdateStatusUnknown) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	shared_ptr<MockVMEventHandler> mockEventHandler = make_shared<MockVMEventHandler>();
	VMManager vmManager(mockRequest);
	vmManager.registerVMEventHandler(mockEventHandler);

	EXPECT_CALL(*mockEventHandler, newVMAdded(_)).Times(AtLeast(0));
	EXPECT_CALL(*mockEventHandler, existingVMChanged(Pointee(Eq(vmCUnknown)), VMEvent::UNKNOWN));

	rigSucces(mockRequest);
	vmManager.refreshVMList();
	rigUpdatedUnknownC(mockRequest);
	vmManager.refreshVMList();
}

TEST(VMManagerTest, testNotifyMultipleUpdateStatusUnknown) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	shared_ptr<MockVMEventHandler> mockEventHandlerA = make_shared<MockVMEventHandler>();
	shared_ptr<MockVMEventHandler> mockEventHandlerB = make_shared<MockVMEventHandler>();
	VMManager vmManager(mockRequest);
	vmManager.registerVMEventHandler(mockEventHandlerA);
	vmManager.registerVMEventHandler(mockEventHandlerB);

	EXPECT_CALL(*mockEventHandlerA, newVMAdded(_)).Times(AtLeast(0));
	EXPECT_CALL(*mockEventHandlerA, existingVMChanged(Pointee(Eq(vmCUnknown)), VMEvent::UNKNOWN));
	EXPECT_CALL(*mockEventHandlerB, newVMAdded(_)).Times(AtLeast(0));
	EXPECT_CALL(*mockEventHandlerB, existingVMChanged(Pointee(Eq(vmCUnknown)), VMEvent::UNKNOWN));

	rigSucces(mockRequest);
	vmManager.refreshVMList();
	rigUpdatedUnknownC(mockRequest);
	vmManager.refreshVMList();
}

TEST(VMManagerTest, testNotifyRemoveVM) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	shared_ptr<MockVMEventHandler> mockEventHandler = make_shared<MockVMEventHandler>();
	VMManager vmManager(mockRequest);
	vmManager.registerVMEventHandler(mockEventHandler);

	EXPECT_CALL(*mockEventHandler, newVMAdded(_)).Times(AtLeast(0));
	EXPECT_CALL(*mockEventHandler, oldVMRemoved(Eq(vmAOff)));
	EXPECT_CALL(*mockEventHandler, oldVMRemoved(Eq(vmCOff)));

	rigSucces(mockRequest);
	vmManager.refreshVMList();
	rigRemovedAC(mockRequest);
	vmManager.refreshVMList();
}

TEST(VMManagerTest, testNotifyMultipleRemoveVM) {
	shared_ptr<MockAppVirtRequest> mockRequest = make_shared<MockAppVirtRequest>();
	shared_ptr<MockVMEventHandler> mockEventHandlerA = make_shared<MockVMEventHandler>();
	shared_ptr<MockVMEventHandler> mockEventHandlerB = make_shared<MockVMEventHandler>();
	VMManager vmManager(mockRequest);
	vmManager.registerVMEventHandler(mockEventHandlerA);
	vmManager.registerVMEventHandler(mockEventHandlerB);

	EXPECT_CALL(*mockEventHandlerA, newVMAdded(_)).Times(AtLeast(0));
	EXPECT_CALL(*mockEventHandlerA, oldVMRemoved(Eq(vmAOff)));
	EXPECT_CALL(*mockEventHandlerA, oldVMRemoved(Eq(vmCOff)));
	EXPECT_CALL(*mockEventHandlerB, newVMAdded(_)).Times(AtLeast(0));
	EXPECT_CALL(*mockEventHandlerB, oldVMRemoved(Eq(vmAOff)));
	EXPECT_CALL(*mockEventHandlerB, oldVMRemoved(Eq(vmCOff)));

	rigSucces(mockRequest);
	vmManager.refreshVMList();
	rigRemovedAC(mockRequest);
	vmManager.refreshVMList();
}

int main(int argc, char **argv) {
	log4cxx::BasicConfigurator::configure();
	log4cxx::Logger::getRootLogger()->setLevel(log4cxx::Level::getOff());\
	testing::InitGoogleMock(&argc, argv);
	prepareVMs();
	return RUN_ALL_TESTS();
}
