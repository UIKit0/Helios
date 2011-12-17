

#ifndef Helios_TextureAnimationComponent_h
#define Helios_TextureAnimationComponent_h
#include <Component/BaseComponent.h>
#include <string>
#include <vector>
namespace helios
{
    
    class TextureAnimationComponent : public BaseComponent
    {
    private:
        glm::mat3 mCurrentMat;
        glm::vec4 mCurrentColor;
        glm::vec2 mCurrentMinMax;
        uint64_t mCurrentTime=0L;
        
        s::Animation mCurrentAnimation, mDefaultAnimation, mQueuedAnimation;
        
        std::vector<s::TexFrame> mFrames;
        
        short mMinMaxUniformLoc = -1;
        short mColorMixLoc = -1;
        
                
        struct {
            bool mAnimationEnded : 1;
            bool mAnimationStarted : 1;
            bool mInterpolate : 1;
            bool mAnimationQueued: 1;
        } ;
    public:
        TextureAnimationComponent(IEntity& owner, bool interpolate = TRUE) : BaseComponent(owner), mInterpolate(interpolate), mAnimationEnded(FALSE), mAnimationStarted(FALSE), mAnimationQueued(FALSE) 
        {
            mName = e::kComponentTextureAnimation;
        };
        ~TextureAnimationComponent() {};
        
        void AddKeyframe(s::TexFrame& frame) {
            mFrames.push_back(frame);
        };
        
        s::Animation const & CurrentAnimation( ) { return mCurrentAnimation ; } ;
        
        void SetMinMaxUniformLoc(short loc) { mMinMaxUniformLoc = loc; };
        void SetColorMixUniformLoc(short loc) { mColorMixLoc = loc; };
        void SetAnimation(int startFrame, int endFrame, float fps, bool loop = TRUE);
        
        void SetDefaultAnimation(int startFrame, int endFrame, float fps);
        void Process();
        void Update(uint64_t) ;
        void DispatchEvents();
        IEvent_ptr operator()() { 
            
        };
                
        void* operator new (size_t size, ILayer& layer)
        {
            return layer.GetPool().alloc(e::kComponentTextureAnimation, size);
        };
        void operator delete ( void * ptr )
        {           
            
            ILayer* owner=0L;
            
            auto layers = SceneManager::Inst().GetLayers();
            
            auto it = layers.begin();
            auto ite = layers.end();
            for ( ; it != ite && !owner ; ++it )
            {
                owner = (*it).second->OwnsComponent((IComponent*)ptr, e::kComponentTextureAnimation);
                
            }
            
            
            owner->GetPool().release(ptr, e::kComponentTextureAnimation);
        };

        
    };
};


#endif
