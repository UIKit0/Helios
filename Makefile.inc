CXX = clang++
CC  = clang

MAKE = make
DEBUG = -g
LIBDIR = /Users/james/git/Helios_lib
HELIOSDIR = /Users/james/git/Helios
INCLUDES = -I$(LIBDIR)/boost/ -I$(LIBDIR)/glm/ -I$(HELIOSDIR)/Core/ -I$(LIBDIR)/SOIL/src/
CXXFLAGS = -stdlib=libc++ -std=c++0x -Os -static -c $(INCLUDES)

OUT = libHelios.a

# Options:
#  GL32, GLES2, . . . [for future use]

RENDER = GL32