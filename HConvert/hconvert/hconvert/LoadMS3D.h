//
//  LoadMS3D.h
//  hconvert
//
//  Created by James Hurley on 5/17/13.
//  Copyright (c) 2013 helios. All rights reserved.
//

#ifndef __hconvert__LoadMS3D__
#define __hconvert__LoadMS3D__

#include <iostream>
#include <Helios.h>
#include "MilkShape3D.h"

#include <glm/gtx/normal.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/euler_angles.hpp>
class LMS3D
{
public:
    inline static void quaternionFromEuler(glm::quat& q, float roll, float pitch, float yaw)
    {
        float cr = cosf(roll / 2.f);
        float sr = sinf(roll / 2.f);
        float cp = cosf(pitch / 2.f);
        float sp = sinf(pitch / 2.f);
        float cy = cosf(yaw / 2.f);
        float sy = sinf(yaw / 2.f);

        q.w = (cr * cp * cy + sr * sp * sy);
        q.x = (sr * cp * cy - cr * sp * sy);
        q.y = (cr * sp * cy + sr * cp * sy);
        q.z = (cr * cp * sy - sr * sp * cy);
    };

    static int LoadMS3D(std::string &file,
               std::vector<helios::Vertex>& vertices,
               std::vector<uint32_t>& indices,
               std::vector<helios::s::Material>& materials,
               std::vector<helios::s::Joint>& joints)
    {
        MilkShape3D *ms3d = new MilkShape3D();
        std::string b_folder = helios::SceneManager::Inst().GetResourceFolder();
        int ret = ms3d->LoadMS3D(file);
        if(ret < 0) return ret;
        ;
        //std::map<std::string,helios::Texture*> textures;
        std::vector<int> seenBones;
        std::vector<int> boneIds;
        for ( auto jt = ms3d->groups.begin() ; jt != ms3d->groups.end() ; ++jt)
        {
            helios::s::Material mg = {0};
            size_t indexStart = indices.size();
            size_t currentIndex = indexStart;

            ms3d_material_t & mat = ms3d->materials[(*jt).materialIndex];

            std::string tex ( mat.texture );
            if(tex.size() > 0) {
                size_t fn = tex.find_last_of("/");
                tex = tex.substr(fn);
                memcpy(mg.texturename, tex.c_str(), (tex.size() > 36 ? 36 : tex.size()));
                       
                //if(textures[tex] == 0L)
               // {
                //    std::string l (b_folder + tex);
               //     D_PRINT("Loading %s\n", l.c_str());
                    //textures[tex] = new helios::Texture(l);
               // }
               // mg.tex = (*(textures[tex]))();
            }
            mg.shininess = mat.shininess;
            
            mg.index_start = static_cast<uint32_t>(indexStart);

            for(int i = 0 ; i < (*jt).numtriangles ; ++i)
            {
                auto it = (ms3d->triangles.begin()+(*jt).triangleIndices[i]);

                for ( int j = 0 ; j < 3 ; ++j )
                {
                    helios::Vertex a;
                    {
                        ms3d_vertex_t & va = ms3d->vertices[(*it).vertexIndices[j]];
                        float v [4] = { va.vertex[0], va.vertex[1], va.vertex[2], 1.f };
                        a.p = glm::vec4(v[0],v[1],v[2],v[3]);
                        a.boneId = va.boneId;

                        if(std::find(boneIds.begin(), boneIds.end(), a.boneId) == boneIds.end())
                        {
                            boneIds.push_back(a.boneId);
                        }
                    }
                    {
                        float n[4] = {(*it).vertexNormals[j][0], (*it).vertexNormals[j][1], (*it).vertexNormals[j][2],1.f };
                        a.SetNormals(glm::vec4(n[0],n[1],n[2],n[3]));
                    }
                    a.s = (*it).s[j] * 0xFFFF;
                    a.t = (*it).t[j] * 0xFFFF;
                    a.r = 0;
                    a.g = 0;
                    a.b = 0;
                    a.a = 255;


                    vertices.push_back(a);
                    indices.push_back(static_cast<uint32_t>(currentIndex++));
                }

            }
            mg.index_end = static_cast<uint32_t>(currentIndex);
            materials.push_back(mg);
        }
        std::sort(boneIds.begin(), boneIds.end());

        bool first = TRUE;

        for ( auto it = ms3d->joints.begin() ; it != ms3d->joints.end() ; ++it)
        {

            helios::s::Joint j;
            j.absMat = glm::mat4(1.f);
            j.relMat = glm::mat4(1.f);

            float pos [3] = { (*it).position[0], (*it).position[1], (*it).position[2] };

            float _x = (*it).rotation[0], _y = (*it).rotation[1], _z = (*it).rotation[2];

            j.relMat = glm::yawPitchRoll(_x,_y,_z);
            //        quaternionFromEuler(j.relQuat,_x,_y,_z);
            j.absTrans = j.relTrans = glm::vec3(pos[0], pos[1], pos[2]);
            j.relMat *= glm::translate(glm::mat4(1.0),j.relTrans);

            std::string name((*it).name);
            std::string parent((*it).parentName);

            if(parent.size() > 0)
            {

                j.absTrans += joints[ms3d->jointMap[parent]].absTrans;
                j.parent = ms3d->jointMap[parent];
                j.absMat = joints[ms3d->jointMap[parent]].absMat * j.relMat;

            } else {
                j.parent = -1;
                j.absTrans = j.relTrans;
                j.absMat = j.relMat;
            }
            joints.push_back(j);

            first = FALSE;
        }

        // Translate vertices by the inverse of the absolute joint translations so that we can
        // directly apply rotations and concatenate with parent joints.
        for(auto it = vertices.begin() ; it != vertices.end() ; ++it)
        {
            glm::mat4 m;
            glm::vec3 relTrans = glm::vec3(joints[(*it).boneId].absTrans);

            m *= glm::translate(glm::mat4(1.f),relTrans);

            m = glm::inverse(m);

            (*it).p = m * (*it).p;
        }
        helios::generateDegenerateQuads(vertices, indices);


        auto  jt = ms3d->keyframes.begin();

        for (auto it = joints.begin() ; it < joints.end() ; ++it, ++jt)
        {

            for (int i = 0 ; i < (*jt).keyFramesRot.size() ; ++i)
            {
                glm::mat4 m ;


                glm::vec3 tt(0.f);
                glm::quat qt;

                helios::s::Frame k;


                float x, y, z;
                x = (*jt).keyFramesRot[i].rotation[0];
                y = (*jt).keyFramesRot[i].rotation[1];
                z = (*jt).keyFramesRot[i].rotation[2];
                glm::quat q;
                quaternionFromEuler(q, x,y,z);
                glm::mat4 ypr = glm::mat4_cast(q) ;
                
                // Transform by the relative location and rotate.  Concatenation with parent occurs after frame interpolation.
                m = ((*it).relMat * ypr) ;
                
                k.mat = m;
                
                k.time = (*jt).keyFramesRot[i].time;
                
                (*it).keyframes.push_back(k);
                
            }
            
        }
        return 0;
    }

    };
#endif /* defined(__hconvert__LoadMS3D__) */
