//
//  CameraComponent.cpp
//  GeometryShaderTest
//
//  Created by James Hurley on 11/8/11.
//

#include <iostream>
#include <Component/CameraComponent.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
namespace helios
{
    
    CameraComponent::CameraComponent(IEntity& owner, glm::vec3 lookVector, glm::vec3 upVector, glm::vec3 anchor) : BaseComponent(owner) 
    {
        mDirectionVector = lookVector;
        mUpVector = upVector;
        mAnchorPos = anchor;
    }
    
    CameraComponent::~CameraComponent() 
    {
        
    }
    
    void 
    CameraComponent::Process()
    {
        
        /**
         *  The look vector rotates with the rotation obtained from the Position component.
         *  This means that if your look vector is pointing directly at the anchor, it will always look directly at the anchor.
         *  If it is aimed slightly above the anchor, it will always point slightly "above" the anchor (relative to the 'up' vector).
         **/
        mCurrentMat = glm::mat4(1.f);
        //static float x = 0.;
        
        glm::mat4 ppos = mPos * glm::translate(glm::mat4(1.f), -mAnchorPos);
        
        //x += 0.01;
        glm::mat3 rot(ppos);
        
        
        glm::vec3 look = rot * mDirectionVector;
        glm::vec3 up = rot * mUpVector;
        
        glm::vec4 pos = glm::translate(glm::mat4(1.f), mAnchorPos) * ppos * glm::vec4(0.f,0.f,0.f,1.f);
        
        glm::vec3 f = glm::normalize(look);
        glm::vec3 u = glm::normalize(up);
        glm::vec3 s = glm::normalize(glm::cross(f, u));
        
        u = glm::cross(s,f);
        
        glm::mat4 r(1.f);
        
        r[0][0] = s.x;
        r[1][0] = s.y;
        r[2][0] = s.z;
        r[0][1] = u.x;
        r[1][1] = u.y;
        r[2][1] = u.z;
        r[0][2] = -f.x;
        r[1][2] = -f.y;
        r[2][2] = -f.z;
        
        mCurrentMat = glm::translate(r, -glm::vec3(pos));
        
    }
    void 
    CameraComponent::Update(uint64_t) 
    {
        /** Position **/
        {
            auto & events = mOwner.GetEvents(e::kEventTargetPosition);
            if(events.size() > 0 ) {
                mPos = HEvent<glm::mat4>::GetData(events[0]);           
            }
        }
  
    }
    void 
    CameraComponent::DispatchEvents()
    {
        auto e = mOwner.PushEvent<glm::mat4>(e::kEventTargetMatrix, e::kEventMatrixCamera, mCurrentMat);
        if ( mEventElevation.push.layer ) 
        {
            mOwner.GetOwner()->PushEvent(e);
        }
    }
};
