
include Makefile.inc
COMPONENTS_DIR = ./Core/Component
RENDER_DIR = ./Core/Render
MEMORY_DIR = ./Core/Memory
SCENE_DIR = ./Core/Scene
WORLD_DIR = ./Core/World

all: dir helios archive

dir: 
	-mkdir objects
	
helios: common components memory render scene world

common: ./Core/common.o
	$(CXX) $(CXXFLAGS) ./Core/common.cpp
	cp ./Core/*.o objects
	
components: 
	$(MAKE) -C $(COMPONENTS_DIR)
	cp $(COMPONENTS_DIR)/*.o objects
memory:
	$(MAKE) -C $(MEMORY_DIR)
	cp $(MEMORY_DIR)/*.o objects

scene: 
	$(MAKE) -C $(SCENE_DIR)
	cp $(SCENE_DIR)/*.o objects	
world:
	$(MAKE) -C $(WORLD_DIR)
	cp $(WORLD_DIR)/*.o objects
render:
	 $(MAKE) -C $(RENDER_DIR)
	cp $(RENDER_DIR)/*.o objects
	
archive:
	ar rcs $(OUT) ./objects/*.o
	
clean:
	$(MAKE) -C $(COMPONENTS_DIR) clean
	$(MAKE) -C $(MEMORY_DIR) clean
	$(MAKE) -C $(SCENE_DIR) clean
	$(MAKE) -C $(WORLD_DIR) clean
	$(MAKE) -C $(RENDER_DIR) clean
	-rm objects/*.o
	-rm libHelios.a
	-rm *.o
	