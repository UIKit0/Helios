#include "BasicEntity.h"

namespace helios_dev
{
    BasicEntity::BasicEntity(helios::ILayer* owner, glm::vec3 pos,  int vbo, int ibo) : BaseEntity(owner)
    {
        mComponents[helios::e::kComponentPosition].push_back(new ((*owner)) helios::PositionComponent((*this), pos, glm::vec3(1.f,1.f,1.f) ));

        helios::RenderableComponent * rc = new ((*owner)) helios::RenderableComponent((*this), vbo, true);
       
        helios::SkeletonComponent * sc = new ((*owner)) helios::SkeletonComponent((*this));
 
        // TODO: Amalgamate respond to touch/mouse/keyboard to input component?
       
        helios::RespondsToKeyboardComponent * kc = new ((*owner)) helios::RespondsToKeyboardComponent((*this));
        
        helios::RespondsToMouseComponent * mc = new ((*owner)) helios::RespondsToMouseComponent((*this));
        
        mComponents[helios::e::kComponentRenderable].push_back(rc);
        mComponents[helios::e::kComponentSkeleton].push_back(sc);
        mComponents[helios::e::kComponentRespondsToKeyboard].push_back(kc);
        mComponents[helios::e::kComponentRespondsToMouse].push_back(mc);
        owner->RegisterComponents(mComponents);
        rc->SetActive(true);
    }
    BasicEntity::~BasicEntity() {}
    
}