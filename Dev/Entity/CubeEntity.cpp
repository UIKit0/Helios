//
//  CubeEntity.cpp
//  Helios
//
//  Created by James Hurley on 1/25/13.
//  Copyright (c) 2013 James Hurley. All rights reserved.
//

#include "CubeEntity.h"
namespace helios_dev
{
    CubeEntity::CubeEntity(helios::ILayer* owner, glm::vec3 pos, int vao, int vbo, int ibo, int mvLoc, int pLoc, int normLoc, int shader)
    : BaseEntity(owner)
    {
        mComponents[helios::e::kComponentPosition].push_back(new ((*owner)) helios::PositionComponent((*this), pos, glm::vec3(1.f,1.f,1.f) ));

        helios::RenderableComponent * rc = new ((*owner)) helios::RenderableComponent((*this), vbo,vao,mvLoc,pLoc,-1,normLoc,shader);

        mComponents[helios::e::kComponentRenderable].push_back(rc);
        
        owner->RegisterComponents(mComponents);
        rc->SetActive(true);
    }
    CubeEntity::~CubeEntity()
    {
        
    }
}