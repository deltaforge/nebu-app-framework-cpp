
#include "nebu-app-framework/topologyManager.h"
#include "nebu/mocks/mockAppPhysRequest.h"

#include "nebu/util/exceptions.h"

#include "log4cxx/basicconfigurator.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

// Using declarations - standard library
using std::make_shared;
using std::shared_ptr;
// Using declarations - nebu-common
using nebu::common::NebuServerException;
using nebu::common::PhysicalDataCenter;
using nebu::common::PhysicalHost;
using nebu::common::PhysicalLocalStore;
using nebu::common::PhysicalNetworkStore;
using nebu::common::PhysicalRack;
using nebu::common::PhysicalRoot;
// Using declarations - nebu-app-framework
using nebu::app::framework::TopologyManager;
// Using declarations - mocks
using nebu::test::MockAppPhysRequest;
// Using declarations - gtest/gmock
using testing::Eq;
using testing::IsNull;
using testing::NotNull;
using testing::Return;
using testing::Throw;

shared_ptr<PhysicalRoot> root = make_shared<PhysicalRoot>("root");
shared_ptr<PhysicalDataCenter> dcA = make_shared<PhysicalDataCenter>("dcA");
shared_ptr<PhysicalDataCenter> dcB = make_shared<PhysicalDataCenter>("dcB");
shared_ptr<PhysicalRack> rackAA = make_shared<PhysicalRack>("rackAA");
shared_ptr<PhysicalRack> rackAB = make_shared<PhysicalRack>("rackAB");
shared_ptr<PhysicalRack> rackBA = make_shared<PhysicalRack>("rackBA");
shared_ptr<PhysicalHost> hostAAA = make_shared<PhysicalHost>("hostAAA");
shared_ptr<PhysicalHost> hostABA = make_shared<PhysicalHost>("hostABA");
shared_ptr<PhysicalHost> hostABB = make_shared<PhysicalHost>("hostABB");
shared_ptr<PhysicalHost> hostBAA = make_shared<PhysicalHost>("hostBAA");

void rigTopology() {
	root->addDataCenter(dcA); dcA->setParent(root.get());
	root->addDataCenter(dcB); dcB->setParent(root.get());
	dcA->addRack(rackAA); rackAA->setParent(dcA.get());
	dcA->addRack(rackAB); rackAB->setParent(dcA.get());
	dcB->addRack(rackBA); rackBA->setParent(dcB.get());
	rackAA->addHost(hostAAA); hostAAA->setParent(rackAA.get());
	rackAB->addHost(hostABA); hostABA->setParent(rackAB.get());
	rackAB->addHost(hostABB); hostABB->setParent(rackAB.get());
	rackBA->addHost(hostBAA); hostBAA->setParent(rackBA.get());
}

void addTopology(shared_ptr<MockAppPhysRequest> mockRequest) {
	EXPECT_CALL(*mockRequest, getPhysicalTopology()).WillOnce(Return(root));
}

TEST(TopologyManagerTest, testConstructor) {
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(shared_ptr<MockAppPhysRequest>());

	EXPECT_THAT(topologyManager->getRoot(), NotNull());
}

TEST(TopologyManagerTest, testRefreshTopologyWithServerError) {
	shared_ptr<MockAppPhysRequest> mockRequest = make_shared<MockAppPhysRequest>();
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(mockRequest);

	EXPECT_CALL(*mockRequest, getPhysicalTopology()).WillOnce(Throw(NebuServerException("")));

	EXPECT_THAT(topologyManager->refreshTopology(), Eq(false));
}

TEST(TopologyManagerTest, testRefreshTopologyWithNullRoot) {
	shared_ptr<MockAppPhysRequest> mockRequest = make_shared<MockAppPhysRequest>();
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(mockRequest);

	EXPECT_CALL(*mockRequest, getPhysicalTopology()).WillOnce(Return(shared_ptr<PhysicalRoot>()));

	EXPECT_THAT(topologyManager->refreshTopology(), Eq(false));
}

TEST(TopologyManagerTest, testRefreshTopology) {
	shared_ptr<MockAppPhysRequest> mockRequest = make_shared<MockAppPhysRequest>();
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(mockRequest);

	addTopology(mockRequest);

	EXPECT_THAT(topologyManager->refreshTopology(), Eq(true));
	EXPECT_THAT(topologyManager->getRoot(), Eq(root));
}

TEST(TopologyManagerTest, testRefreshTopologyConsistencyAfterServerError) {
	shared_ptr<MockAppPhysRequest> mockRequest = make_shared<MockAppPhysRequest>();
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(mockRequest);

	addTopology(mockRequest);
	topologyManager->refreshTopology();

	EXPECT_CALL(*mockRequest, getPhysicalTopology()).WillOnce(Throw(NebuServerException("")));
	EXPECT_THAT(topologyManager->refreshTopology(), Eq(false));
	EXPECT_THAT(topologyManager->getRoot(), Eq(root));
}

TEST(TopologyManagerTest, testRefreshTopologyConsistencyAfterNullRoot) {
	shared_ptr<MockAppPhysRequest> mockRequest = make_shared<MockAppPhysRequest>();
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(mockRequest);

	addTopology(mockRequest);
	topologyManager->refreshTopology();

	EXPECT_CALL(*mockRequest, getPhysicalTopology()).WillOnce(Return(shared_ptr<PhysicalRoot>()));
	EXPECT_THAT(topologyManager->refreshTopology(), Eq(false));
	EXPECT_THAT(topologyManager->getRoot(), Eq(root));
}

TEST(TopologyManagerTest, testGetDataCenterByIDNotFound) {
	shared_ptr<MockAppPhysRequest> mockRequest = make_shared<MockAppPhysRequest>();
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(mockRequest);

	addTopology(mockRequest);
	topologyManager->refreshTopology();

	EXPECT_THAT(topologyManager->getDataCenterByID("dcC"), IsNull());
}

TEST(TopologyManagerTest, testGetDataCenterByIDFound) {
	shared_ptr<MockAppPhysRequest> mockRequest = make_shared<MockAppPhysRequest>();
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(mockRequest);

	addTopology(mockRequest);
	topologyManager->refreshTopology();

	EXPECT_THAT(topologyManager->getDataCenterByID("dcA"), Eq(dcA));
	EXPECT_THAT(topologyManager->getDataCenterByID("dcB"), Eq(dcB));
}

TEST(TopologyManagerTest, testGetRackByIDNotFound) {
	shared_ptr<MockAppPhysRequest> mockRequest = make_shared<MockAppPhysRequest>();
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(mockRequest);

	addTopology(mockRequest);
	topologyManager->refreshTopology();

	EXPECT_THAT(topologyManager->getRackByID("rackNotFound"), IsNull());
}

TEST(TopologyManagerTest, testGetRackByIDFound) {
	shared_ptr<MockAppPhysRequest> mockRequest = make_shared<MockAppPhysRequest>();
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(mockRequest);

	addTopology(mockRequest);
	topologyManager->refreshTopology();

	EXPECT_THAT(topologyManager->getRackByID("rackAA"), Eq(rackAA));
	EXPECT_THAT(topologyManager->getRackByID("rackAB"), Eq(rackAB));
	EXPECT_THAT(topologyManager->getRackByID("rackBA"), Eq(rackBA));
}

TEST(TopologyManagerTest, testGetHostByIDNotFound) {
	shared_ptr<MockAppPhysRequest> mockRequest = make_shared<MockAppPhysRequest>();
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(mockRequest);

	addTopology(mockRequest);
	topologyManager->refreshTopology();

	EXPECT_THAT(topologyManager->getHostByID("hostNotFound"), IsNull());
}

TEST(TopologyManagerTest, testGetHostByIDFound) {
	shared_ptr<MockAppPhysRequest> mockRequest = make_shared<MockAppPhysRequest>();
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(mockRequest);

	addTopology(mockRequest);
	topologyManager->refreshTopology();

	EXPECT_THAT(topologyManager->getHostByID("hostAAA"), Eq(hostAAA));
	EXPECT_THAT(topologyManager->getHostByID("hostABA"), Eq(hostABA));
	EXPECT_THAT(topologyManager->getHostByID("hostABB"), Eq(hostABB));
	EXPECT_THAT(topologyManager->getHostByID("hostBAA"), Eq(hostBAA));
}

TEST(TopologyManagerTest, testGetRackIDForHostNotFound) {
	shared_ptr<MockAppPhysRequest> mockRequest = make_shared<MockAppPhysRequest>();
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(mockRequest);

	addTopology(mockRequest);
	topologyManager->refreshTopology();

	EXPECT_THAT(topologyManager->getRackIDForHost("hostNotFound"), Eq(TopologyManager::ID_UNKNOWN));
}

TEST(TopologyManagerTest, testGetRackIDForHostFound) {
	shared_ptr<MockAppPhysRequest> mockRequest = make_shared<MockAppPhysRequest>();
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(mockRequest);

	addTopology(mockRequest);
	topologyManager->refreshTopology();

	EXPECT_THAT(topologyManager->getRackIDForHost("hostAAA"), Eq("rackAA"));
	EXPECT_THAT(topologyManager->getRackIDForHost("hostABA"), Eq("rackAB"));
	EXPECT_THAT(topologyManager->getRackIDForHost("hostABB"), Eq("rackAB"));
	EXPECT_THAT(topologyManager->getRackIDForHost("hostBAA"), Eq("rackBA"));
}

TEST(TopologyManagerTest, testGetDataCenterIDForHostNotFound) {
	shared_ptr<MockAppPhysRequest> mockRequest = make_shared<MockAppPhysRequest>();
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(mockRequest);

	addTopology(mockRequest);
	topologyManager->refreshTopology();

	EXPECT_THAT(topologyManager->getDataCenterIDForHost("hostNotFound"), Eq(TopologyManager::ID_UNKNOWN));
}

TEST(TopologyManagerTest, testGetDataCenterIDForHostFound) {
	shared_ptr<MockAppPhysRequest> mockRequest = make_shared<MockAppPhysRequest>();
	shared_ptr<TopologyManager> topologyManager = make_shared<TopologyManager>(mockRequest);

	addTopology(mockRequest);
	topologyManager->refreshTopology();

	EXPECT_THAT(topologyManager->getDataCenterIDForHost("hostAAA"), Eq("dcA"));
	EXPECT_THAT(topologyManager->getDataCenterIDForHost("hostABA"), Eq("dcA"));
	EXPECT_THAT(topologyManager->getDataCenterIDForHost("hostABB"), Eq("dcA"));
	EXPECT_THAT(topologyManager->getDataCenterIDForHost("hostBAA"), Eq("dcB"));
}

int main(int argc, char **argv) {
	log4cxx::BasicConfigurator::configure();
	log4cxx::Logger::getRootLogger()->setLevel(log4cxx::Level::getOff());
	testing::InitGoogleMock(&argc, argv);
	rigTopology();
	return RUN_ALL_TESTS();
}

