include ../Makefile.inc

SRC = main.cpp ./Scene/BasicScene.cpp ./Layer/BasicLayer.cpp ./Tool/MilkShape3D.cpp ./Entity/BasicEntity.cpp ./Entity/CameraEntity.cpp
FLAGS = $(INCLUDES) -stdlib=libc++ -std=c++0x $(DEBUG) -framework OpenGL -framework Cocoa -lHelios -L.. -lSOIL -L$(LIBDIR)/SOIL/src
OUT = helios_dev
all: dev
	
dev:
	-mkdir build
	$(CXX) $(FLAGS) -lglfw $(SRC) -o  $(OUT)
	bash bundle.sh $(OUT)
	mv $(OUT) $(OUT).app/Contents/MacOS
	mv $(OUT).app build
	-mv $(OUT).dSYM build
	cp $(HELIOSDIR)/Core/Render/Shaders/$(RENDER)/* ./build/$(OUT).app/Contents/Resources
	cp ./Resources/* ./build/$(OUT).app/Contents/Resources
	
clean:
	-rm -rf build/$(OUT)*