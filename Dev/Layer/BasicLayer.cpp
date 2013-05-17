#include "BasicLayer.h"
#include "../Entity/BasicEntity.h"
#include "../Entity/CameraEntity.h"
#include "../Entity/CubeEntity.h"

#include <Loader/LoadH3DMESH.h>
#include <glm/gtx/normal.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/euler_angles.hpp>
namespace helios_dev
{
    BasicLayer::BasicLayer(helios::IScene& owner) : helios::BaseLayer(owner) {} ;
    BasicLayer::~BasicLayer() {};

    void
        BasicLayer::onActive(uint64_t t)
    {
        BaseLayer::onActive(t);
        
        mPool.reset( new helios::Pool(10) );
        std::vector<helios::Vertex> verts;
        std::vector<uint32_t> ind;
        std::vector<helios::s::Material> mats;
        std::vector<helios::s::Joint> joints;
        std::string b_folder = helios::SceneManager::Inst().GetResourceFolder();
        std::string file =  b_folder + "/zombie02.h3dmesh";

        int retval = helios::LoadH3DMESH::Load(file, verts, ind, mats, joints );

        if(retval < 0)
        {
            return;
        }
        {
            int vbo = mRender->GenerateVBO(&verts[0], sizeof(helios::Vertex), sizeof(helios::Vertex) * verts.size());
            int ibo = mRender->GenerateIBO(&ind[0], ind.size());
            
            BasicEntity* e = new BasicEntity(this, glm::vec3(0.f,0.f,0.f),  vbo, ibo);

            helios::RenderableComponent* rc = (helios::RenderableComponent*)e->GetComponent(helios::e::kComponentRenderable)[0];
            helios::SkeletonComponent * sc = (helios::SkeletonComponent*)e->GetComponent(helios::e::kComponentSkeleton)[0];

            for( auto it = mats.begin() ; it != mats.end() ; ++it)
            {
                helios::s::MaterialGroup m ((*it));

                auto tex = std::string(it->texturename);
                if(mTextures.find(tex) == mTextures.end())
                {
                    std::shared_ptr<helios::Texture> pTex(new helios::Texture(b_folder + tex));
                    mTextures[tex] = pTex;
                }
                m.tex = mTextures[tex]->id();
                m.ibo = ibo;
                rc->AddMaterialGroup(m);
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
            helios::Vertex v [4] ;
            unsigned short minS = 0;
            unsigned short maxS = 0xFFFF;
            unsigned short minT = 0;
            unsigned short maxT = 0xFFFF;
            v [0] = helios::Vertex( glm::vec4(0.f,0.f,0.f,1.f), minS,maxT );
            v [1] = helios::Vertex( glm::vec4(50.f,0.f,0.f,1.f), maxS,maxT );
            v [2] = helios::Vertex( glm::vec4(0.f,0.f,50.f,1.f), minS,minT );
            v [3] = helios::Vertex( glm::vec4(50.f,0.f,50.f,1.f), maxS,minT );
            for ( int i = 0 ; i < 4 ; ++i )
            {
                v[i].r = 200;
                v[i].g = 170;
                v[i].b = 40;
            }
            int vbo = mRender->GenerateVBO(&v[0], sizeof(helios::Vertex), sizeof(helios::Vertex) * 4);
            uint32_t indices [6] = {  2 , 1 , 0, 2, 3, 1  } ;
            int ibo = mRender->GenerateIBO(indices, 6);
            
            CubeEntity* e = new CubeEntity(this, glm::vec3(-25.f,0.f,-25.f), vbo, ibo );
            helios::s::MaterialGroup mg ;
            
            mg.ibo = ibo;
            mg.index_start = 0;
            mg.index_end = 6;
            mg.tex = 0;
            auto rc = static_cast<helios::RenderableComponent*>(e->GetComponent(helios::e::kComponentRenderable)[0]);
            rc->AddMaterialGroup(mg);
            
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

    
}