include ../../Makefile.inc

OBJ = $(SRC:.cpp=.o)

ifeq ($(RENDER), GLES2)
DIR = ./GLES2
else
DIR = ./GL32
endif

SRC = RenderCommand.cpp $(DIR)/Render.cpp

all: components
	
components: $(OBJ)
		$(CXX) $(CXXFLAGS) $(SRC)
		
clean:
	-rm $(OBJ)
