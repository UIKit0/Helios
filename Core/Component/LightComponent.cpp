//
//  LightComponent.cpp
//  Helios
//
//  Created by James Hurley on 5/15/13.
//  Copyright (c) 2013 James Hurley. All rights reserved.
//

#include "LightComponent.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <Render/IRender.h>

namespace helios
{
    LightComponent::LightComponent(IEntity& owner, glm::vec4 color, bool isShadowCaster)
    : BaseComponent(owner), mColor(color), mIsShadowCaster(isShadowCaster)
    {
        mName = e::kComponentLight;
        
        mCurrentEvent.SetName(e::kEventRenderCommand);

        mCurrentEvent.SetTarget(e::kEventTargetRender);

        mIsActive = false;

    }
 
    LightComponent::~LightComponent()
    {
    }
    void
    LightComponent::Process()
    {

        BaseComponent::Process();



        LightCommand com;

        glm::vec4 l_pos = mCurrentMVMat * mCurrentPos * glm::vec4(0.f,0.f,0.f,1.f);

        com.pos = l_pos;
        com.color = mColor;
        com.isShadowCaster = mIsShadowCaster;

        mCurrentEvent.SetData(com);

        mCurrentEvent.SetTarget(e::kEventTargetRender);
        mCurrentEvent.SetTarget(e::kEventRenderCommand);

    }
    void
    LightComponent::Update(uint64_t)
    {
        /** Position **/
        {
            auto & events = mOwner.GetEvents(e::kEventTargetPosition);
            if(events.size() > 0 ) {
                mCurrentPos = HEvent<glm::mat4>::GetData(events[0]);
            }
        }
        /** Matrices **/
        {
            auto & events = mOwner.GetOwner()->GetEvents(e::kEventTargetMatrix);
            mCurrentMVMat = glm::mat4(0.f);

            for (auto it = events.begin() ; it != events.end() ; ++ it)
            {
                if((*it)->GetName() == e::kEventMatrixModelView)
                {
                    mCurrentMVMat= HEvent<glm::mat4>::GetData((*it));
                }

            }

        }
    }
    void
    LightComponent::DispatchEvents()
    {
        
    }
    
};
