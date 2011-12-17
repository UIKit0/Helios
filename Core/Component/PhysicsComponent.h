//
//  PhysicsComponent.h
//  Helios
//
//  Created by James Hurley on 10/12/11.
//

#ifndef Helios_PhysicsComponent_h
#define Helios_PhysicsComponent_h

#include <Component/BaseComponent.h>

#include <glm/glm.hpp>


namespace helios
{
    class ISimulator;
    
    class PhysicsComponent : public BaseComponent
    {
    private:
        ISimulator* mSimulator;
        
    public:
        
        PhysicsComponent(IEntity& owner, ISimulator* simulator);
        ~PhysicsComponent();
        
        
        void Process();
        void Update(uint64_t) ;
        void DispatchEvents();
        
        void* operator new (size_t size, ILayer& layer)
        {
            return layer.GetPool().alloc(e::kComponentPhysics, size);
        };
        void operator delete ( void * ptr )
        {           
            
            ILayer* owner=0L;
            
            auto layers = SceneManager::Inst().GetLayers();
            
            auto it = layers.begin();
            auto ite = layers.end();
            for ( ; it != ite && !owner ; ++it )
            {
                owner = (*it).second->OwnsComponent((IComponent*)ptr, e::kComponentPhysics);
                
            }
            
            
            owner->GetPool().release(ptr, e::kComponentPhysics);
        };
    };
};


#endif
