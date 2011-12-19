#include "CameraEntity.h"

namespace helios_dev
{
    CameraEntity::CameraEntity(helios::ILayer* owner, glm::vec3 pos,glm::vec3 anchor, glm::vec3 direction, glm::vec3 up) : helios::BaseEntity(owner)
       {
           mComponents[helios::e::kComponentPosition].push_back(new ((*owner)) helios::PositionComponent((*this), pos, glm::vec3(1.f,1.f,1.f) ));
           helios::CameraComponent * cc = new ((*owner)) helios::CameraComponent((*this), direction, up, anchor);

           helios::elevation_t e;
           e.push.entity = 0;
           e.push.layer = 1;

           e.pull.entity = 1;
           cc->SetEventElevation(e);

           mComponents[helios::e::kComponentCamera].push_back(cc);

           owner->RegisterComponents(mComponents);

       }
    
}