//
//  TouchableComponent.h
//  Helios
//
//  Created by James Hurley on 9/27/11.
//

#ifndef Helios_TouchableComponent_h
#define Helios_TouchableComponent_h

#include <Component/BaseComponent.h>
#include <glm/glm.hpp>
namespace helios
{
    class TouchableComponent : public BaseComponent
    {
    private:
        
        HEvent<Touches> mCurrentEvent;
        glm::mat4 mCurrentHitbox;
        glm::mat4 mCurrentMVMat;
        glm::mat4 mCurrentPMat;
        void (^mTouchHitBlock)(HEvent<Touches>& e) ;
        
    public:
        TouchableComponent(IEntity& owner, void (^hitblock)(HEvent<Touches>&));
        ~TouchableComponent();
        IEvent_ptr operator()() { return mCurrentEvent.ptr(); };
        void DispatchEvents();
        void Process();
        void Update(uint64_t) ;
        void* operator new (size_t size, ILayer& layer)
        {
            return layer.GetPool().alloc(e::kComponentTouchable, size);
        };
        void operator delete ( void * ptr )
        {           
            
            ILayer* owner=0L;
            
            auto layers = SceneManager::Inst().GetLayers();
            
            auto it = layers.begin();
            auto ite = layers.end();
            for ( ; it != ite && !owner ; ++it )
            {
                owner = (*it).second->OwnsComponent((IComponent*)ptr, e::kComponentTouchable);
                
            }
            
            
            owner->GetPool().release(ptr, e::kComponentTouchable);
        };

    };
};

#endif
