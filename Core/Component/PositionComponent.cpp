//
//  PositionComponent.cpp
//  Helios
//
//  Created by James Hurley on 9/22/11.
//

#include <iostream>
#include <Component/PositionComponent.h>
#include <Event/HEvent.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtc/quaternion.hpp>

namespace helios
{
    PositionComponent::PositionComponent(IEntity& owner, short x, short y, short w, short h)
    : BaseComponent(owner)
    {
        
        mPosition = glm::vec3(x,y,0.f);
        mScale = glm::vec3(w,h,0.f);
        mTargetPosition = mPosition;
        mName = e::kComponentPosition;

    }
    PositionComponent::PositionComponent(IEntity& owner, glm::vec3 position, glm::vec3 scale) 
    : BaseComponent(owner), mPosition(position), mScale(scale), mTargetPosition(position)
    {
        mName = e::kComponentPosition;
    }
    void 
    PositionComponent::Process()
    {
        BaseComponent::Process();
        
        if(mTargetPosition != mPosition)
        {
            uint64_t delta = mCurrentTime - mLastTime;
            
            float t = float(delta) / 1.0e6;
            
            float dist = mSpeed * t;
            glm::vec3 pdelta = mTargetPosition - mPosition;
            float distance_to_target = sqrtf ( pdelta.x * pdelta.x + pdelta.y * pdelta.y + pdelta.z * pdelta.z ) ;
            
            if ( distance_to_target < dist ) 
                mPosition = mTargetPosition;
            else
                mPosition += glm::normalize(pdelta) * dist;
        }
     
        glm::mat4 mat (1.0f);
        
        mat *= glm::translate(glm::mat4(1.f), mPosition);
        
        mat *= glm::mat4_cast(mCurrentRotation);
        
        mat *= glm::scale(glm::mat4(1.f), mScale);
        
        mCurrentPosition = mat;

        
    }
    void 
    PositionComponent::Update(uint64_t time) 
    { 
        mLastTime = mCurrentTime;
        
        if(mCurrentTime==0) mLastTime = time;
        
        mCurrentTime = time;
        {
            auto e = mOwner.GetEvents(e::kEventTargetAction);
        
            for ( auto it = e.begin() ; it != e.end() ; ++it )
            {
                if((*it)->GetName() == e::kActionMoveForward) {
                    //glm::vec3 v = HEvent<glm::vec3>::GetData((*it));
                    // TODO: save movement vector for Process pass
                    mPosition.z += 0.1;
                }
                else if((*it)->GetName() == e::kActionRotate) {
                   // glm::quat q = HEvent<glm::quat>::GetData((*it));
                    // TODO: save rotation quat for Process pass.
                }
            }
        }
        
        
    }
    void 
    PositionComponent::MoveTo(glm::vec3 pos, float speed)
    {
        mTargetPosition = pos;
        mSpeed = speed;        
    }
    void 
    PositionComponent::DispatchEvents()
    {
        auto e = mOwner.PushEvent<glm::mat4>(e::kEventTargetPosition, e::kEventPositionMatrix, mCurrentPosition);
        if ( mEventElevation.push.layer ) 
        {
            mOwner.GetOwner()->PushEvent(e);
        }
    }
}
