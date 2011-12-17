//
//  Frustum.h
//  Helios
//
//  Created by James Hurley on 9/15/11.
//

#ifndef Helios_Frustum_h
#define Helios_Frustum_h

#include <vector>
#include <glm/glm.hpp>

namespace helios
{
    struct Plane
    {
        glm::vec3 vertices[4];
    };
    
    struct Frustum
    {
        Plane planes[6];
    };
    
};

#endif
