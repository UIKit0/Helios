

#include <iostream>
#include <Component/TextureAnimationComponent.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Render/RenderCommand.h>

namespace helios
{
    void 
    TextureAnimationComponent::SetAnimation(int startFrame, int endFrame, float fps, bool loop)
    {
        float delta = 1.f / fps;
        
        float startTarget = startFrame * delta;
        float endTarget = endFrame * delta;
        
        mCurrentAnimation.startTime = startTarget;
        mCurrentAnimation.endTime = endTarget;
        mCurrentAnimation.fpsDelta = delta;
        mCurrentAnimation.loop = loop;
        mCurrentAnimation.currentTime = startTarget;
        //mAnimationQueued = true;
        mAnimationStarted = true;
    };
    void 
    TextureAnimationComponent::SetDefaultAnimation(int startFrame, int endFrame, float fps)
    {
        float delta = 1.f / fps;
        
        float startTarget = startFrame * delta;
        float endTarget = endFrame * delta;
        
        mDefaultAnimation.startTime = startTarget;
        mDefaultAnimation.endTime = endTarget;
        mDefaultAnimation.fpsDelta = delta;
        mDefaultAnimation.loop = TRUE;
        mDefaultAnimation.currentTime = startTarget;
        
        mCurrentAnimation = mDefaultAnimation;
    }
    void 
    TextureAnimationComponent::Process()
    {
        s::TexFrame prev, next;
        
        if ( mCurrentAnimation.currentTime > mCurrentAnimation.endTime) 
        {
            if(mCurrentAnimation.loop)
            {
                mCurrentAnimation.currentTime = mCurrentAnimation.startTime;
                
            } else {
               // if(!mAnimationQueued) {
                    mCurrentAnimation = mDefaultAnimation;
                    mAnimationEnded = true;
                /*} else {
                    if(mCurrentAnimation.startTime != mDefaultAnimation.startTime || mCurrentAnimation.endTime != mDefaultAnimation.endTime)
                        mAnimationEnded = true;
                    
                    mCurrentAnimation = mQueuedAnimation;
                    mAnimationQueued = false;
                    
                    mAnimationStarted = true;
                }*/
            }
        }
        //D_PRINT("%lf", mCurrentAnimation.currentTime);
     //   next.time = mCurrentAnimation.endTime;
        
        for ( auto it = mFrames.begin() ; it != mFrames.end () ; ++it )
        {
            if((*it).time > mCurrentAnimation.currentTime )
            {
                next = (*it);
                break;
            }
            prev = (*it);
        }
        
        
        if( mInterpolate && mCurrentAnimation.currentTime != mCurrentAnimation.startTime && mCurrentAnimation.currentTime != mCurrentAnimation.endTime ) {
            float mix = (mCurrentAnimation.currentTime - prev.time) / (next.time - prev.time);
            glm::vec4 p ( prev.minS, prev.minT, prev.maxS, prev.maxT );
            glm::vec4 n ( next.minS, next.minT, next.maxS, next.maxT );
            glm::vec4 m = glm::mix(p, n, mix);
            glm::vec4 c1 ( float(prev.color[0]) / 255.f, float(prev.color[1]) / 255.f, float(prev.color[2]) / 255.f, float(prev.color[3]) / 255.f);
            glm::vec4 c2 ( float(next.color[0]) / 255.f, float(next.color[1]) / 255.f, float(next.color[2]) / 255.f, float(next.color[3]) / 255.f);
            
            mCurrentColor = glm::mix(c1,c2,mix);
            
            mCurrentMat = glm::translate(glm::mat3(1.f), glm::vec2(m.x,m.y));
            mCurrentMat *= glm::scale(glm::mat3(1.f), glm::vec2(m.z - m.x, m.w - m.y));
            mCurrentMinMax.x = m.x;
            mCurrentMinMax.y = m.z;
        } 
        else {
            mCurrentMat = glm::translate(glm::mat3(1.f), glm::vec2(prev.minS, prev.minT));
            mCurrentMat *= glm::scale(glm::mat3(1.f), glm::vec2(prev.maxS - prev.minS, prev.maxT - prev.minT));    
           
            mCurrentColor = glm::vec4( float(prev.color[0]) / 255.f, float(prev.color[1]) / 255.f, float(prev.color[2]) / 255.f, float(prev.color[3]) / 255.f);
            mCurrentMinMax.x = prev.minS;
            mCurrentMinMax.y = prev.maxS;
        }
    }
    void 
    TextureAnimationComponent::Update(uint64_t t) 
    {
        //uint64_t delta = 16666;
        uint64_t delta = t - mCurrentTime;
        if(mCurrentTime == 0)
        {
            delta = 0;
        }
        
        
        mCurrentTime = t;
        mCurrentAnimation.currentTime += float(delta) / 1.0e6f;
    };
    void 
    TextureAnimationComponent::DispatchEvents()
    {
        mOwner.PushEvent< glm::mat3 >(e::kEventTargetTexture, e::kEventTextureMatrix, mCurrentMat);
       // mOwner.PushEvent< uint8_t[4] >(e::kEventTargetTexture, e::kEventTextureMix, mCurrentColor);
        {
        UniformData_ptr u( new UniformData(UniformData::kUTVec3, mMinMaxUniformLoc));
        glm::vec3 t(mCurrentMinMax,0.f);
        u->SetData<glm::vec3>(t);
        
        auto e = mOwner.PushEvent<UniformData_ptr>(e::kEventTargetUniformData, e::kEventUniformData, u);
        if ( mEventElevation.push.layer ) 
        {
            mOwner.GetOwner()->PushEvent(e);
        }
        }
        {
            UniformData_ptr u ( new UniformData(UniformData::kUTVec4, mColorMixLoc));
            u->SetData<glm::vec4>(mCurrentColor);
            auto e = mOwner.PushEvent<UniformData_ptr>(e::kEventTargetUniformData, e::kEventUniformData, u);
            
        }
        
        
    }
}
