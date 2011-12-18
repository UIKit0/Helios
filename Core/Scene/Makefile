include ../../Makefile.inc


SRC = BaseScene.cpp
OBJ = $(SRC:.cpp=.o)

all: components
	
components: $(OBJ)
		$(CXX) $(CXXFLAGS) $(SRC)
		
clean:
	-rm $(OBJ)
