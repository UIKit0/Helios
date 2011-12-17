
#ifndef Helios_Octree_h
#define Helios_Octree_h
#include <Algo/SpatialIndex/ISpatialIndex.h>
#include <vector>
#include <glm/glm.hpp>

namespace helios
{
    
    class Octree : public ISpatialIndex
    {
    private:
        
        struct Node
        {
            Node* children[8] = {0L};
            
        };
        
        glm::vec3 mMaximums;
        std::vector<Node> mNodes;
    public:
        virtual void ComponentsForFrustum(Frustum frustum,  std::vector<const char *, IComponent*>& outComponents) = 0;
        virtual void AddComponentToLocation(IComponent*, glm::vec3) = 0 ;
        virtual void RemoveComponent(IComponent*) = 0;
    }
}


#endif
