//
//  NetworkComponent.h
//  Qt3
//
//  Created by James Hurley on 10/21/11.
//

#ifndef Helios_NetworkComponent_h
#define Helios_NetworkComponent_h

#include <Component/BaseComponent.h>
#include <Event/BaseEvent.h>

namespace helios
{
    class NetworkComponent : public BaseComponent
    {
    private:
        
        IEvent_ptr mCurrentEvent;
        void DispatchEvents();
        
    public:
    
        
        
        void * operator new ( size_t size, ILayer& layer)
        {
            return layer.GetPool().alloc(e::kComponentNetwork, size);
        };
        void operator delete(void* ptr)
        {
            // Since we can't really pass much information to the delete
            // operator, we will attempt to discover the owner of the component
            // and release it from the appropriate pool.
            
            ILayer* owner;
            
            auto layers = SceneManager::Inst().GetLayers();
            
            auto it = layers.begin();
            auto ite = layers.end();
            for ( ; it != ite && !owner ; ++it )
            {
                owner = (*it).second->OwnsComponent((IComponent*)ptr, e::kComponentNetwork);
                
            }
            
            
            owner->GetPool().release(ptr, e::kComponentNetwork);
            
        };

    };
    
};

#endif
