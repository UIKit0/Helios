//
//  SkeletonComponent.cpp
//  Helios
//
//  Created by James Hurley on 10/17/11.
//

#include <iostream>
#include <Component/SkeletonComponent.h>
#include <Render/RenderCommand.h>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace helios
{
    void 
    SkeletonComponent::Process()
    {
       // mCurrentFrames.clear();
        
#if(USE_QUATERNIONS==1)
        mCurrentRots.clear();
        mCurrentTrans.clear();
#else
        mCurrentMat.clear();
#endif
        float mix;
        glm::quat q ;
        glm::vec3 v;
        if(mCurrentAnimation.currentTime >= mCurrentAnimation.endTime)
        {
            if(mCurrentAnimation.loop)
            {
                mCurrentAnimation.currentTime = mCurrentAnimation.startTime;
                
            } else {
                mCurrentAnimation = mDefaultAnimation;
                mAnimationEnded = true;
            }
        }
        
        for (  size_t i = 0 ; i != mJoints.size() ; ++i)
        {
            s::Frame previous, next;
            
            
            /** This is a disaster. FIX. **/
            for ( size_t j =0 ; j < mJoints[i].keyframes.size() ; ++j )
            {
                if(mJoints[i].keyframes[j].time > mCurrentAnimation.currentTime)
                {
                    next = mJoints[i].keyframes[j];
                    if(next.time > mCurrentAnimation.endTime)
                    {
                        for ( size_t k = 0 ; k < mJoints[i].keyframes.size() ; ++k)
                        {
                            if(mJoints[i].keyframes[k].time <= next.time && mJoints[i].keyframes[k+1].time > next.time)
                            {
                                next = mJoints[i].keyframes[k];
                                break;
                            }
                        }
                    }
                    break;
                }
                previous = mJoints[i].keyframes[j];
            }
            /* -------------------------------- */
            mix = (mCurrentAnimation.currentTime - previous.time) / (next.time - previous.time);
#if( USE_QUATERNIONS == 1 )
            
            // slerp
            //q=previous.rotation;
            q= glm::mix(previous.rotation, next.rotation, mix);
            // lerp
            v = glm::mix(previous.translation, next.translation, mix);
            //v = previous.translation;
            
            if(mJoints[i].parent >= 0)
            {
                q = q * mCurrentRots[mJoints[i].parent];
                v += mCurrentTrans[mJoints[i].parent] ;
                
            }
            s::Frame current;
            current.rotation = q;
            current.translation = v;
            current.time = mCurrentAnimation.currentTime;
            //mCurrentFrames.push_back(current);
            mCurrentRots.push_back(q);
            mCurrentTrans.push_back(v);
#else
            glm::mat4 m=glm::interpolate(previous.mat, next.mat, mix);
            
            // perform matrix concatenation
            if(mJoints[i].parent >=0 )
                m = mCurrentMat[mJoints[i].parent] * m;
            
            mCurrentMat.push_back(m);
#endif
        }
      
        
    }
    void 
    SkeletonComponent::Update(uint64_t t) 
    {
        
        uint64_t delta = t - mCurrentTime;
        if(mCurrentTime == 0)
        {
            delta = 0;
        }
        mCurrentTime = t;
        mCurrentAnimation.currentTime += (float)(delta / 1.0e6f);
    }
    void 
    SkeletonComponent::DispatchEvents()
    {
       
        
        
#if(USE_QUATERNIONS == 1)
        std::vector<UniformData> v;
        if(mCurrentRots.size() > 0) {
            
        
        {
            UniformData ud(UniformData::kUTVec4, mJointRotationLoc);
            ud.SetData<float*>(&mCurrentRots[0][0],mCurrentRots.size());
        
            v.push_back(ud);
        }
        {
            UniformData ud(UniformData::kUTVec3, mJointTransformLoc);
            ud.SetData<float*>(&mCurrentTrans[0][0], mCurrentTrans.size());
            v.push_back(ud);
        }
        auto e = mOwner.PushEvent<std::vector<UniformData> >(e::kEventTargetSkeleton, e::kEventSkeleton, v);
        }
#else
        if(mCurrentMat.size() > 1)
        {
            UniformData_ptr ud( new UniformData(UniformData::kUTMat4, mJointMatLoc));
            ud->SetData<float*>(&mCurrentMat[0][0][0], mCurrentMat.size());
            auto e = mOwner.PushEvent<UniformData_ptr>(e::kEventTargetUniformData, e::kEventUniformData, ud);
            if ( mEventElevation.push.layer ) 
            {
                mOwner.GetOwner()->PushEvent(e);
            }
        } else if ( mCurrentMat.size() > 0)
        {
            UniformData_ptr u( new UniformData(UniformData::kUTMat4, mJointMatLoc));
            u->SetData<glm::mat4>(mCurrentMat[0]);
            
            auto e = mOwner.PushEvent<UniformData_ptr>(e::kEventTargetUniformData, e::kEventUniformData, u);
            if ( mEventElevation.push.layer ) 
            {
                mOwner.GetOwner()->PushEvent(e);
            }
        }
        if ( mAnimationEnded ) {
            
            bool ended = mAnimationEnded;
            
            auto e = mOwner.PushEvent<bool>(e::kEventTargetAnimation, e::kEventAnimationEnd, ended);
            
            /** TODO: Elevate specific event target types 
             **/
             
            mOwner.GetOwner()->PushEvent(e);
            mAnimationEnded = false;
        }
        if ( mAnimationStarted ) {
            bool started = mAnimationStarted;
            
            auto e = mOwner.PushEvent<bool>(e::kEventTargetAnimation, e::kEventAnimationStart, started);
            
            mOwner.GetOwner()->PushEvent(e);
            
            mAnimationStarted = false;
        }
#endif
        
        
        
    }
    void 
    SkeletonComponent::SetAnimation(int startKeyframe, int endKeyframe, float fps, bool loop)
    {
        float delta = 1.f / fps;
        
        float startTarget = startKeyframe * delta;
        float endTarget = endKeyframe * delta;
        
        mCurrentAnimation.startTime = startTarget;
        mCurrentAnimation.endTime = endTarget;
        mCurrentAnimation.fpsDelta = delta;
        mCurrentAnimation.loop = loop;
        mCurrentAnimation.currentTime = startTarget;
        mAnimationStarted = true;
        
    }
    void
    SkeletonComponent::SetDefaultAnimation(int startKeyframe, int endKeyframe, float fps)
    {
        float delta = 1.f / fps;
        
        float startTarget = startKeyframe * delta;
        float endTarget = endKeyframe * delta;
        
        mDefaultAnimation.startTime = startTarget;
        mDefaultAnimation.endTime = endTarget;
        mDefaultAnimation.fpsDelta = delta;
        mDefaultAnimation.loop = TRUE;
        mDefaultAnimation.currentTime = startTarget;
        
        mCurrentAnimation = mDefaultAnimation;
    }
}
