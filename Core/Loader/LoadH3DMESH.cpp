//
//  LoadH3DMESH.cpp
//  Helios
//
//  Created by James Hurley on 5/17/13.
//  Copyright (c) 2013 James Hurley. All rights reserved.
//

#include "LoadH3DMESH.h"
namespace helios
{
     int
    LoadH3DMESH::Load(std::string & file, std::vector<helios::Vertex> & vertices, std::vector<uint32_t>& indices, std::vector<s::Material> & mats, std::vector<s::Joint>& joints)
    {
        FILE *fp = fopen(file.c_str(), "rb");
        if(!fp) return -1;

        uint16_t header, version;

        fread(&header, sizeof(header), 1, fp);
        fread(&version, sizeof(version), 1, fp);

        if(header != 0x4E05 || version != 0x0001)
        {
            fclose(fp);
            return -2;
        }

        uint32_t size;

        {
            // Vertices
            fread(&size, sizeof(size), 1, fp);
            
            vertices.resize(size / sizeof(Vertex));
            fread(&vertices[0], size, 1, fp);
        }
        {
            // Indices
            fread(&size, sizeof(size), 1, fp);
            
            indices.resize(size / sizeof(uint32_t));
            fread(&indices[0], size, 1, fp);
        }
        {
            // joints
            fread(&size, sizeof(size), 1, fp);
            joints.resize(size);
           
            for ( int i = 0 ; i < size ; ++i )
            {
                fread(&joints[i], sizeof(s::JointBase), 1, fp);
                joints[i].keyframes.resize(joints[i].frameCount);
                
                fread(&joints[i].keyframes[0], sizeof(s::Frame), joints[i].frameCount, fp);
            }
        }
        {
            // materials
            
            fread(&size, sizeof(size), 1, fp);
            
            mats.resize(size / sizeof(s::Material));
            fread(&mats[0], size, 1, fp);
        }
        return 0;
    }
}