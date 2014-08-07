
#include "nebu-app-framework/daemon.h"

#include "log4cxx/basicconfigurator.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

// Using declarations - standard library
using std::make_shared;
using std::shared_ptr;
using std::string;
// Using declarations - nebu-common
using nebu::common::VirtualMachine;
// Using declarations - nebu-app-framework
using nebu::app::framework::Daemon;
using nebu::app::framework::DaemonType;
// Using declarations - gtest/gmock
using testing::Eq;

class StubDaemon : public Daemon
{
public:
	StubDaemon(shared_ptr<VirtualMachine> vm) : Daemon(vm) { }
	virtual ~StubDaemon() { }

	virtual bool launch() { return false; }
	virtual DaemonType getType() const { return 0; }

	void clearLaunched() { this->launched = false; }
	void setLaunched() { this->launched = true; }
};

TEST(DaemonTest, testConstructorSetsVM) {
	shared_ptr<VirtualMachine> vm = make_shared<VirtualMachine>("Some VM");
	StubDaemon daemon(vm);

	EXPECT_THAT(daemon.getHostVM(), Eq(vm));
}

TEST(DaemonTest, testConstructorDefaultsLaunchedToFalse) {
	shared_ptr<VirtualMachine> vm = make_shared<VirtualMachine>("Some VM");
	StubDaemon daemon(vm);

	EXPECT_THAT(daemon.hasLaunched(), Eq(false));
}

TEST(DaemonTest, testGetHostname) {
	string hostname = "Some Hostname";
	string altHostname = "Updated Hostname";
	shared_ptr<VirtualMachine> vm = make_shared<VirtualMachine>("Some VM");
	StubDaemon daemon(vm);

	vm->setHostname(hostname);
	EXPECT_THAT(daemon.getHostname(), Eq(hostname));
	vm->setHostname(altHostname);
	EXPECT_THAT(daemon.getHostname(), Eq(altHostname));
}

TEST(DaemonTest, testHasLaunchedFalse) {
	shared_ptr<VirtualMachine> vm = make_shared<VirtualMachine>("Some VM");
	StubDaemon daemon(vm);

	daemon.clearLaunched();
	EXPECT_THAT(daemon.hasLaunched(), Eq(false));
}

TEST(DaemonTest, testHasLaunchedTrue) {
	shared_ptr<VirtualMachine> vm = make_shared<VirtualMachine>("Some VM");
	StubDaemon daemon(vm);

	daemon.setLaunched();
	EXPECT_THAT(daemon.hasLaunched(), Eq(true));
}

int main(int argc, char **argv) {
	log4cxx::BasicConfigurator::configure();
	log4cxx::Logger::getRootLogger()->setLevel(log4cxx::Level::getOff());
	testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
