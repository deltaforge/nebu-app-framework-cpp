
AM_CPPFLAGS = -std=c++0x -Wall -Wextra -pthread
if TESTS_ENABLED
AM_CPPFLAGS += --coverage -O0 -g -fno-default-inline -fno-inline
endif

include MakefileTestList.am

TESTS = $(integration_TESTS) $(unit_TESTS)
check_PROGRAMS = $(TESTS)

$(check_PROGRAMS): $(top_srcdir)/src/.libs/libnebu-app-framework.a

AM_LDFLAGS = -Wl,--whole-archive $(top_srcdir)/src/.libs/libnebu-app-framework.a -Wl,--no-whole-archive \
       $(NEBU_COMMON_LIBS) $(LOG4CXX_LIBS) $(TINYXML2_LIBS) -lrestclient-cpp $(top_srcdir)/testlibs/gmock.a -lgcov
AM_CPPFLAGS += -Itest/ -I$(top_srcdir)/include -isystem $(top_srcdir)/testlibs/gtest/include -isystem $(top_srcdir)/testlibs/gmock/include \
               $(NEBU_COMMON_CFLAGS) $(LOG4CXX_CFLAGS) $(TINYXML2_CFLAGS)

