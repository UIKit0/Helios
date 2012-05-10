include ../../Makefile.inc


SRC = CameraComponent.cpp PositionComponent.cpp RenderableComponent.cpp SkeletonComponent.cpp TextureAnimationComponent.cpp TouchableComponent.cpp RespondsToKeyboardComponent.cpp RespondsToMouseComponent.cpp
OBJ = $(SRC:.cpp=.o)

all: components
	
components: $(OBJ)
		$(CXX) $(CXXFLAGS) $(SRC)
		
clean:
	-rm $(OBJ)
