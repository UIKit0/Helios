
#ifndef __Helios__LoadH3DMESH__
#define __Helios__LoadH3DMESH__

#include <iostream>
#include <common.h>
#include <Render/Vertex.h>

namespace helios
{
    class LoadH3DMESH
    {
    public:
        static int Load(std::string & file, std::vector<helios::Vertex> & vertices, std::vector<uint32_t>& indices, std::vector<s::Material> & mats, std::vector<s::Joint>& joints);
    };
}

#endif
