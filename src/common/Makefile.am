AUTOMAKE_OPTS=gnu
noinst_LIBRARIES=libcommon.a
libcommon_a_SOURCES=\
										sceneparser.h \
										sceneparser.cpp \
										vector.h \
										ray.h 
AM_CXXFLAGS=-fPIC
AM_LDFLAGS=-L../json_spirit/ -ljson -L../shapes/ -lshapes
INCLUDES=-I../json_spirit/ -I../shapes/
