AUTOMAKE_OPTS=gnu
bin_PROGRAMS=rtrace
rtrace_SOURCES=\
	main.cpp \
	rtrace.h \
	rtrace.cpp

AM_CXXFLAGS=-fPIC
INCLUDES=-I../common/ -I../shapes
AM_LDFLAGS=-L../common -lcommon -L../shapes -lshapes -L../json_spirit -ljson
