//
//  LoadAssimp.cpp
//  hconvert
//
//  Created by James Hurley on 5/17/13.
//  Copyright (c) 2013 helios. All rights reserved.
//

#include "LoadAssimp.h"

struct Bone
{
    Bone() : j(NULL), id(0) {};
    Bone(helios::s::Joint* joint) : j(joint) {};
    Bone(const Bone  & b) : j(b.j), name(b.name), id(b.id) {};

    std::string name;
    short id;
    helios::s::Joint* j;
};

int
LoadAssimp::Load(std::string &file,
         std::vector<helios::Vertex>& vertices,
         std::vector<uint32_t>& indices,
         std::vector<helios::s::Material>& materials,
         std::vector<helios::s::Joint>& joints)
{
    int retval = 0;

    std::map<std::string, Bone> bones;
    std::vector<float> maxweight;       // since we don't currently support multiple bones per vertex with weights..
    
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(file.c_str(), aiProcess_Triangulate
                                             | aiProcess_JoinIdenticalVertices
                                             | aiProcess_GenSmoothNormals
                                             | aiProcess_ImproveCacheLocality
                                             | aiProcess_FindDegenerates
                                             | aiProcess_OptimizeGraph
                                             | aiProcess_OptimizeMeshes
                                             );

    if(scene->HasMeshes())
    {
        for ( int i = 0 ; i < scene->mNumMeshes ; ++i )
        {
            aiMesh* mesh = scene->mMeshes[i];
            for(int j = 0; j < mesh->mNumVertices ; ++j)
            {
                helios::Vertex v ;
                aiVector3D& p = mesh->mVertices[i];
                v.p = glm::vec4(p.x,p.y,p.z, 1.f);
                aiVector3D& t = mesh->mTextureCoords[i][0];
                v.s = t.x * 0xFFFF;
                v.t = t.y * 0xFFFF;
                aiVector3D& n = mesh->mNormals[i];
                v.SetNormals(glm::vec4(n.x,n.y,n.z,1.f));
                if(mesh->HasVertexColors(0)) {
                    aiColor4D& c = mesh->mColors[i][0];
                    v.r = c.r * 0xFF;
                    v.g = c.g * 0xFF;
                    v.b = c.b * 0xFF;
                    v.a = c.a * 0xFF;
                }
                vertices.push_back(v);
                maxweight.push_back(0.f);
            }

            if(mesh->HasFaces())
            {
                for(int j = 0 ; j < mesh->mNumFaces; ++j)
                {
                    aiFace& f = mesh->mFaces[j];
                    indices.insert(indices.end(), f.mIndices, f.mIndices+f.mNumIndices);
                }
            }
            if(mesh->HasBones()) {
                for(int j = 0 ; j < mesh->mNumBones ; ++j)
                {
                    aiBone* bone = mesh->mBones[i];
                    helios::s::Joint joint ;
                    joints.push_back(joint);
                    Bone b(&joints.back());
                    b.name = bone->mName.C_Str();
                    memcpy(&b.j->relMat[0][0], &bone->mOffsetMatrix[0][0], sizeof(float)*16);
                    b.id = j;
                    for(int k = 0; k < bone->mNumWeights; ++k)
                    {
                        aiVertexWeight& w = bone->mWeights[i];

                        if(w.mWeight > maxweight[w.mVertexId]) {
                            maxweight[w.mVertexId] = w.mWeight;
                            vertices[w.mVertexId].boneId = j;
                        }
                    }
                    bones[b.name] = b;
                }
            }
            
        }
    }

    return retval;
}