
#include "nebu-app-framework/commandRunner.h"

#include "log4cxx/basicconfigurator.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

// Using declarations - standard library
using std::make_shared;
using std::shared_ptr;
// Using declarations - nebu-app-framework
using nebu::app::framework::CommandRunner;
// Using declarations - gtest/gmock
using testing::Eq;
using testing::NotNull;

TEST(CommandRunnerTest, testGetDefaultInstance) {
	EXPECT_THAT(CommandRunner::getInstance(), NotNull());
}

TEST(CommandRunnerTest, testGetSetInstance) {
	shared_ptr<CommandRunner> cmdRunner = make_shared<CommandRunner>();
	CommandRunner::setInstance(cmdRunner);

	EXPECT_THAT(CommandRunner::getInstance(), Eq(cmdRunner));

	CommandRunner::setInstance(make_shared<CommandRunner>());
}

TEST(CommandRunnerTest, testTrueCommand) {
	shared_ptr<CommandRunner> cmdRunner = CommandRunner::getInstance();

	int result = cmdRunner->runCommand("true");
	EXPECT_THAT(WEXITSTATUS(result), Eq(0));
}

TEST(CommandRunnerTest, testFalseCommand) {
	shared_ptr<CommandRunner> cmdRunner = CommandRunner::getInstance();

	int result = cmdRunner->runCommand("false");
	EXPECT_THAT(WEXITSTATUS(result), Eq(1));
}

TEST(CommandRunnerTest, testExitCommandWithStatusCode) {
	shared_ptr<CommandRunner> cmdRunner = CommandRunner::getInstance();

	int result = cmdRunner->runCommand("exit 123");
	EXPECT_THAT(WEXITSTATUS(result), Eq(123));
}

int main(int argc, char **argv) {
	log4cxx::BasicConfigurator::configure();
	log4cxx::Logger::getRootLogger()->setLevel(log4cxx::Level::getOff());
	testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}

