//
//  LoadAssimp.h
//  hconvert
//
//  Created by James Hurley on 5/17/13.
//  Copyright (c) 2013 helios. All rights reserved.
//

#ifndef __hconvert__LoadAssimp__
#define __hconvert__LoadAssimp__

#include <iostream>
#include <glm/gtx/normal.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <Helios.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class LoadAssimp
{
public:
    static int Load(std::string &file,
                        std::vector<helios::Vertex>& vertices,
                        std::vector<uint32_t>& indices,
                        std::vector<helios::s::Material>& materials,
                    std::vector<helios::s::Joint>& joints);
};
#endif /* defined(__hconvert__LoadAssimp__) */
