#include "BasicLayer.h"
#include "../Entity/BasicEntity.h"
#include "../Entity/CameraEntity.h"
#include "../Tool/MilkShape3D.h"
#include "../Entity/CubeEntity.h"

#include <glm/gtx/normal.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace helios_dev
{
    BasicLayer::BasicLayer(helios::IScene& owner) : helios::BaseLayer(owner) {} ;
    BasicLayer::~BasicLayer() {};

    void generateQuad(std::vector<helios::Vertex>& verts, const glm::vec3 pos, std::vector<unsigned short> &indices, const glm::vec3 normal, const glm::vec2 size)
    {
        glm::vec3 f (normal);
        glm::vec3 right ( glm::cross(f, glm::vec3(0.f,1.f,0.f))) ;
        glm::vec3 up (glm::cross(right, f));

        f = glm::normalize(f);
        right = glm::normalize(right);
        up = glm::normalize(up);
        size_t offset = verts.size();
        
        {
            helios::Vertex v;
            v.p = glm::vec4(pos + (right * size[0] * 0.5f) + (up * size[1] * 0.5f), 1.f);
            v.SetNormals(glm::vec4(normal,1.f));
            unsigned char  c[4] = { 200, 100, 100, 255 };
            memcpy(v.c,c,4);
            v.boneId = 0;
            verts.push_back(v);
        }
        {
            helios::Vertex v;
            v.p = glm::vec4(pos + (right * size[0] * 0.5f) + (up * -size[1] * 0.5f), 1.f);
            v.SetNormals(glm::vec4(normal,1.f));
            unsigned char  c[4] = { 200, 100, 100, 255 };
            memcpy(v.c,c,4);
            v.boneId = 0;
            verts.push_back(v);
        }
        {
            helios::Vertex v;
            v.p = glm::vec4(pos + (right * -size[0] * 0.5f) + (up * size[1] * 0.5f), 1.f);
            v.SetNormals(glm::vec4(normal,1.f));
            unsigned char  c[4] = { 200, 100, 100, 255 };
            memcpy(v.c,c,4);
            v.boneId = 0;
            verts.push_back(v);
        }
        {
            helios::Vertex v;
            v.p = glm::vec4(pos + (right * -size[0] * 0.5f) + (up * -size[1] * 0.5f), 1.f);
            v.SetNormals(glm::vec4(normal,1.f));
            unsigned char  c[4] = { 200, 100, 100, 255 };
            memcpy(v.c,c,4);
            v.boneId = 0;
            verts.push_back(v);
        }
       // {  0 , 1 , 2, 1, 3, 2  } ;
        indices.push_back(offset+3);
        indices.push_back(offset);
        indices.push_back(offset+2);
        indices.push_back(offset);
        indices.push_back(offset+1);
        indices.push_back(offset+2);
    }
    void
        BasicLayer::onActive(uint64_t t)
    {
        BaseLayer::onActive(t);

        mPool.reset( new helios::Pool(10) );
        std::vector<helios::Vertex> verts;
        std::vector<unsigned short> ind;
        std::vector<helios::s::MaterialGroup> mats;
        std::vector<helios::s::Joint> joints;
        std::string b_folder = helios::SceneManager::Inst().GetResourceFolder();
        std::string file =  b_folder + "/zombie02.ms3d";

              
        LoadMS3D(file, verts, ind, mats, joints);
        int vbo = mRender->GenerateVBO(&verts[0], sizeof(helios::Vertex), sizeof(helios::Vertex) * verts.size());
        int ibo = mRender->GenerateIBO(&ind[0], ind.size());

        {
           
            BasicEntity* e = new BasicEntity(this, glm::vec3(0.f,0.f,0.f),  vbo, ibo);

            helios::RenderableComponent* rc = (helios::RenderableComponent*)e->GetComponent(helios::e::kComponentRenderable)[0];
            helios::SkeletonComponent * sc = (helios::SkeletonComponent*)e->GetComponent(helios::e::kComponentSkeleton)[0];

            for( auto it = mats.begin() ; it != mats.end() ; ++it)
            {
                (*it).ibo = ibo;
           
                rc->AddMaterialGroup((*it));
            }

            for ( auto it = joints.begin() ; it != joints.end() ; ++it)
            {
                sc->AddJoint((*it));
            }
            sc->SetDefaultAnimation(177,205, 30.f);
           sc->LoadAnimationMap(b_folder + "/zombie02.json");
           
            mEntities.push_back(std::shared_ptr<helios::IEntity>(e));
        }
        {
            CameraEntity * e = new CameraEntity(this,glm::vec3(0.,25.,50.),  glm::vec3(0.,0.,5.), glm::vec3(0.,-0.5,-1.), glm::vec3(0.,1.,0.));
            mEntities.push_back(std::shared_ptr<helios::IEntity>(e));
        }

    }

    void
        BasicLayer::onDeactive()
    {

    }

    void
        BasicLayer::Render(uint64_t t)
    {
        mRender->ClearViewport();

        BaseLayer::Render(t);
    }

    void
        BasicLayer::SetViewport( int x, int y, int w, int h, int dpi ) 
    {
        BaseLayer::SetViewport(x, y, w, h);
        float aspect_w = (float)w/(float)h;
        mP = glm::infinitePerspective (45.f, aspect_w, 0.1f);
    }

    
    void 
        BasicLayer::quaternionFromEuler(glm::quat& q, float roll, float pitch, float yaw)
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
    }

// TODO: Create wrapper class to do this
    void
        BasicLayer::LoadMS3D(std::string &file,
        std::vector<helios::Vertex>& vertices, 
        std::vector<unsigned short>& indices, 
        std::vector<helios::s::MaterialGroup>& materials,
        std::vector<helios::s::Joint>& joints)
    {
        MilkShape3D *ms3d = new MilkShape3D();
        std::string b_folder = helios::SceneManager::Inst().GetResourceFolder();
        ms3d->LoadMS3D(file);

        ;
        std::map<std::string,helios::Texture*> textures;
        std::vector<int> seenBones;
        std::vector<int> boneIds;
        for ( auto jt = ms3d->groups.begin() ; jt != ms3d->groups.end() ; ++jt)
        {
            helios::s::MaterialGroup mg;
            size_t indexStart = indices.size();
            size_t currentIndex = indexStart;

            ms3d_material_t & mat = ms3d->materials[(*jt).materialIndex];

            std::string tex ( mat.texture );
            if(tex.size() > 0) {
                size_t fn = tex.find_last_of("/");
                tex = tex.substr(fn);
                if(textures[tex] == 0L)
                {
                    std::string l (b_folder + tex);
                    D_PRINT("Loading %s\n", l.c_str());
                    textures[tex] = new helios::Texture(l);
                }
                mg.tex = (*(textures[tex]))();
            }
            mg.iboRange.start = indexStart;

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
                    indices.push_back(currentIndex++);
                }

            }
            mg.iboRange.end = currentIndex;
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
            quaternionFromEuler(j.relQuat,_x,_y,_z);
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

        delete ms3d;


    }
}