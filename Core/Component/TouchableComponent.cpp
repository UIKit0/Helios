//
//  TouchableComponent.cpp
//  Helios
//
//  Created by James Hurley on 9/27/11.
//

#include <iostream>
#include <Component/TouchableComponent.h>
#include <Block.h>

namespace helios
{
    TouchableComponent::TouchableComponent(IEntity& owner, void (^hitblock)(HEvent<Touches>&)) : BaseComponent(owner)
    {
        mTouchHitBlock = Block_copy(hitblock);
        mName = e::kComponentTouchable;
    }
    TouchableComponent::~TouchableComponent()
    {
        Block_release(mTouchHitBlock);
    }
    void
    TouchableComponent::Process()
    {
        glm::mat4 hitbox(mCurrentHitbox);
        glm::mat4 mvp(mCurrentPMat * mCurrentMVMat);

        //TouchEvent* e  = (TouchEvent*)mCurrentEvent;
        auto t = mCurrentEvent.GetData();
         
        
        // TODO:  We need to do a raycast hit test rather than this current hit test in order to work properly with 3D.
        // Unproject touch pos and cast ray from zNear to zFar, test intersection with hit boxes.
        {
            auto touches = t.touches;
             
            glm::vec4 low(hitbox * glm::vec4(-0.5f,-0.5f,-0.5f,1.f)), high(hitbox * glm::vec4(0.5f,0.5f,0.5f,1.f));
            
            // Translate based on model-view * projection
            low = mvp * low;
            high = mvp * high;
            
            // homogenous divide to get normalized device coordinates
            low.x /= low.w;
            low.y /= low.w;
            low.z /= low.w;
            high.x /= high.w;
            high.y /= high.w;
            high.z /= high.w;
            
            int screenX, screenY, screenW, screenH;
            mOwner.GetOwner()->GetViewport(screenX,screenY,screenW,screenH);
            
            
            // convert the normalized device coordinates from [-1,1] back to screen-space coordinates
            short hw = screenW>>1;
            short hh = screenH>>1;
            low.x = low.x * hw + hw;
            low.y = low.y * hh + hh;
            low.z = low.z * 0.5 + 0.5;
            high.x = high.x * hw + hw;
            high.y = high.y * hh + hh;
            high.z = high.z * 0.5 + 0.5;
            
            // 
            // Hit-test
            for (auto it = touches.begin() ; it != touches.end() ; ++it)
            {
      
      //          D_PRINT("Touch: (%d, %d) Low: (%lf, %lf, %lf) High: (%lf, %lf, %lf)", (*it).x, (*it).y, low.x, low.y, low.z, high.x, high.y, high.z);
                
                (*it).flags |= 0x10 * ((*it).x > low.x && (*it).x < high.x && (*it).y > low.y && (*it).y < high.y); // hit
                
                if( (*it).flags & 0x10 ) {
                    
                    mCurrentEvent.SetTarget(e::kEventTargetTouchHit);
                }
                
            }

        }
        
    }
    void
    TouchableComponent::DispatchEvents()
    {
        
        if(mCurrentEvent.GetTarget() == e::kEventTargetTouchHit)
        {
            auto e = mOwner.PushEvent<Touches>(e::kEventTargetTouchHit, mCurrentEvent.GetName(), mCurrentEvent.GetData());
            if(mTouchHitBlock)
                mTouchHitBlock(mCurrentEvent);
            if ( mEventElevation.push.layer ) 
            {
                mOwner.GetOwner()->PushEvent(e);
            }
            if( mEventElevation.push.scene )
            {
                mOwner.GetOwner()->GetOwner().PushEvent(e);
            }
        } 
        mCurrentEvent.GetData().touches.clear();
        mCurrentEvent.SetTarget("");
    }
    void 
    TouchableComponent::Update(uint64_t) 
    {
        /** Position **/
        {
            auto & events = mOwner.GetEvents(e::kEventTargetPosition);
            if(events.size() > 0 ) {
                mCurrentHitbox = HEvent<glm::mat4>::GetData(events[0]);           
            }
        }
        /** Matrices **/
        {
            auto & events = mOwner.GetOwner()->GetEvents(e::kEventTargetMatrix);
            mCurrentMVMat = glm::mat4(0.f);
            mCurrentPMat = glm::mat4(0.f);
            
            for (auto it = events.begin() ; it != events.end() ; ++ it)
            {
                if((*it)->GetName() == e::kEventMatrixModelView)
                {
                    
                    mCurrentMVMat= HEvent<glm::mat4>::GetData((*it));
                } 
                else if((*it)->GetName() == e::kEventMatrixProjection)
                {
                    mCurrentPMat = HEvent<glm::mat4>::GetData((*it));
     
                }
            }
            
        }
        /** Touches **/
        {
            auto & events = mOwner.GetOwner()->GetEvents(e::kEventTargetTouch);
            //mCurrentEvent = HEvent<Touches>::GetEvent(e);
            
            if(events.size() > 0)
            {
                mCurrentEvent = HEvent<Touches>::GetEvent(events[0]);
            }
        }
        
        

        
        
    }
}
