//
//  SkeletonComponent.h
//  Helios
//
//  Created by James Hurley on 10/17/11.
//

#ifndef Helios_SkeletonComponent_h
#define Helios_SkeletonComponent_h
#include <Component/BaseComponent.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>



namespace helios
{
    
    class SkeletonComponent : public BaseComponent
    {
         
        s::Animation mCurrentAnimation, mDefaultAnimation;
        uint64_t mCurrentTime=0L;
        
        std::vector<s::Joint> mJoints;

#if(USE_QUATERNIONS == 1)
        std::vector<glm::quat> mCurrentRots;
        std::vector<glm::vec3> mCurrentTrans;
        int mJointRotationLoc, mJointTransformLoc;
#else
        std::vector<glm::mat4> mCurrentMat;
        int mJointMatLoc;
#endif
        
        struct {
            bool mAnimationEnded : 1;
            bool mAnimationStarted : 1;
        } ;
        
    public:
        SkeletonComponent(IEntity& owner) : BaseComponent(owner) {
            mAnimationEnded = false;
            mAnimationStarted = false;
            mName = e::kComponentSkeleton;
        } ;
        ~SkeletonComponent() {};
        
        void AddJoint(s::Joint& joint)
        {
            mJoints.push_back(joint);
            //mJoints[mJointsCount++] = joint;
        };
        void SetAnimation(int startFrame, int endFrame, float fps, bool loop = TRUE);
        void SetDefaultAnimation(int startFrame, int endFrame, float fps);
        void Process();
        void Update(uint64_t) ;
        void DispatchEvents();
        IEvent_ptr operator()() { 
           
        };
#if(USE_QUATERNIONS==1)
        void SetUniformLocations(int jointRotationLoc, int jointTransformLoc)
        {
            mJointRotationLoc = jointRotationLoc;
            mJointTransformLoc = jointTransformLoc;
        };
#else
        void SetUniformLocation(int matLoc)
        {
            mJointMatLoc = matLoc;
        };
#endif
        void* operator new (size_t size, ILayer& layer)
        {
            return layer.GetPool().alloc(e::kComponentSkeleton, size);
        };
        void operator delete ( void * ptr )
        {           
            
            ILayer* owner=0L;
            
            auto layers = SceneManager::Inst().GetLayers();
            
            auto it = layers.begin();
            auto ite = layers.end();
            for ( ; it != ite && !owner ; ++it )
            {
                owner = (*it).second->OwnsComponent((IComponent*)ptr, e::kComponentSkeleton);
            }
            
            
            owner->GetPool().release(ptr, e::kComponentSkeleton);
        };

    };
};


#endif
