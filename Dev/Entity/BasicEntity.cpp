#include "BasicEntity.h"

namespace helios_dev
{
    BasicEntity::BasicEntity(helios::ILayer* owner, glm::vec3 pos, int vao, int vbo, int ibo, int mvLoc, int pLoc, int normLoc, int shader) : BaseEntity(owner)
    {
        mComponents[helios::e::kComponentPosition].push_back(new ((*owner)) helios::PositionComponent((*this), pos, glm::vec3(1.f,1.f,1.f) ));

        helios::RenderableComponent * rc = new ((*owner)) helios::RenderableComponent((*this), vbo,vao,mvLoc,pLoc,-1,normLoc,shader);
       
        helios::SkeletonComponent * sc = new ((*owner)) helios::SkeletonComponent((*this));
 
        mComponents[helios::e::kComponentRenderable].push_back(rc);
        mComponents[helios::e::kComponentSkeleton].push_back(sc);
        owner->RegisterComponents(mComponents);
        rc->SetActive(true);
    }
    BasicEntity::~BasicEntity() {}
    
}