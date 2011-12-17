//
//  ISpatialIndex.h
//  Helios
//
//  Created by James Hurley on 9/15/11.
//

#ifndef Helios_ISpatialIndex_h
#define Helios_ISpatialIndex_h
#include <Algo/SpatialIndex/Frustum.h>

namespace helios
{
    class IComponent;
    
    class ISpatialIndex
    {
    public:
        virtual ~ISpatialIndex () {} ;
        
        virtual void ComponentsForFrustum(Frustum frustum,  std::vector<const char *, IComponent*>& outComponents) = 0;
        virtual void AddComponentToLocation(IComponent*, glm::vec3) = 0 ;
        virtual void RemoveComponent(IComponent*) = 0;
    };
};

#endif
