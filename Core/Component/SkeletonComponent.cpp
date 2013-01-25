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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
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
        {
            auto e = mOwner.GetEvents(e::kEventTargetAction);
            bool endAnimation = TRUE;
            bool gotActionEnd = FALSE ;
            
            for ( auto it = e.begin() ; it != e.end() ; ++it )
            {
                auto k = HEvent<const char>::GetData((*it));
                
                if(mAnimationMap.find((*it)->GetName()) != mAnimationMap.end() && k != 2 ) {
                    std::pair<int, int> anim = mAnimationMap[(*it)->GetName()];
                    SetAnimation(anim.first, anim.second, 30.f, TRUE);
                    endAnimation = FALSE;
                    break;
                } else if(k == 2){
                    gotActionEnd = TRUE;
                }
            }
            if(endAnimation && gotActionEnd)
            {
                mCurrentAnimation = mDefaultAnimation;
            }
        }
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
        int id = (startKeyframe ^ 2) + (endKeyframe ^3) + ((int)fps ^5) + (loop) ;

        if( mCurrentAnimation.id == id ) return ;

        float delta = 1.f / fps;
        
        float startTarget = startKeyframe * delta;
        float endTarget = endKeyframe * delta;

        mCurrentAnimation.id = id ;
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
    void
    SkeletonComponent::LoadAnimationMap(const std::string & file)
    {
        boost::property_tree::ptree pt;
        try
        {
            boost::property_tree::read_json(file, pt);
        }
        catch(boost::property_tree::json_parser::json_parser_error &je)
        {
            std::cout << "Error parsing: " << je.filename() << " on line: " << je.line() << std::endl;
            std::cout << je.message() << std::endl;
        }
        BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, pt.get_child("keyframes"))
        {

            int i = 0 ;
            
            std::string action = v.first.data() ;

           
         
            BOOST_FOREACH(const boost::property_tree::ptree::value_type &s, v.second)
            {
                switch(i) {
                    case 0 :
                        mAnimationMap[action].first =  s.second.get_value<int>() ;
                    default:
                        mAnimationMap[action].second =  s.second.get_value<int>() ;
                }
                i++;
            }
            
        }
    }
}
