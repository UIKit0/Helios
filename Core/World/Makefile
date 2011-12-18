include ../../Makefile.inc


SRC = SceneManager.cpp
OBJ = $(SRC:.cpp=.o)

all: components
	
components: $(OBJ)
		$(CXX) $(CXXFLAGS) $(SRC)
		
clean:
	-rm $(OBJ)
