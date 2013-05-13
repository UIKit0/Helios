//
//  CubeEntity.h
//  Helios
//
//  Created by James Hurley on 1/25/13.
//  Copyright (c) 2013 James Hurley. All rights reserved.
//

#ifndef __Helios__CubeEntity__
#define __Helios__CubeEntity__

#include <iostream>
#include <Helios.h>

namespace helios_dev
{
    class CubeEntity : public helios::BaseEntity
    {
    public:

        CubeEntity(helios::ILayer* owner, glm::vec3 pos, int vbo, int ibo, int mvLoc, int pLoc, int normLoc) ;
        ~CubeEntity();
    };

}
#endif /* defined(__Helios__CubeEntity__) */
