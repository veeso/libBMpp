LIBS = 
INCLUDE = ../../include/
AM_CXXFLAGS = -Wall -std=c++11 -I ${INCLUDE}
AM_LDFLAGS = ../../src/.libs/libbmpp.a

noinst_PROGRAMS = bmpTestMono
bmpTestMono_SOURCES = bmpmono.cpp
bmpTestMono_LDADD = ${AM_LDFLAGS}
