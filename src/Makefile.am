
AM_CPPFLAGS = -std=c++0x -Wall -Wextra -pthread -I${top_srcdir}/include/
if TESTS_ENABLED
AM_CPPFLAGS += --coverage -O0 -g -fno-default-inline -fno-inline
endif

src_SOURCES = application.cpp \
	applicationHooks.cpp \
	commandRunner.cpp \
	configuration.cpp \
	daemonCollection.cpp \
	daemon.cpp \
	main.cpp \
	topologyManager.cpp \
	topologyWriter.cpp \
	vmManager.cpp

lib_LTLIBRARIES = libnebu-app-framework.la
libnebu_app_framework_la_SOURCES = $(src_SOURCES)
