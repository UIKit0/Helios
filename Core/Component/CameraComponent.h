
#ifndef Helios_CameraComponent_h
#define Helios_CameraComponent_h
#include <Component/BaseComponent.h>
#include <glm/glm.hpp>

namespace helios
{
    class CameraComponent : public BaseComponent
    {
    private:
                
        glm::mat4 mCurrentMat;
        glm::mat4 mPos;
        
        glm::vec3 mDirectionVector;
        glm::vec3 mUpVector;
        glm::vec3 mAnchorPos;
        
    public:
        
        CameraComponent(IEntity & owner, glm::vec3 lookVector, glm::vec3 upVector, glm::vec3 anchor) ;
        ~CameraComponent() ;
        
        void Process();
        void Update(uint64_t) ;
        void DispatchEvents();
        IEvent_ptr operator()() { 
            
        };
        
        void* operator new (size_t size, ILayer& layer)
        {
            return layer.GetPool().alloc(e::kComponentCamera, size);
        };
        void operator delete ( void * ptr )
        {           
            
            ILayer* owner=0L;
            
            auto layers = SceneManager::Inst().GetLayers();
            
            auto it = layers.begin();
            auto ite = layers.end();
            for ( ; it != ite && !owner ; ++it )
            {
                owner = (*it).second->OwnsComponent((IComponent*)ptr, e::kComponentCamera);
                
            }
            
            
            owner->GetPool().release(ptr, e::kComponentCamera);
        };
    };
};


#endif
