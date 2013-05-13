
#ifndef Helios_RespondsToMouseComponent_h
#define Helios_RespondsToMouseComponent_h

#include <Component/BaseComponent.h>
#include <Event/Touch.h>

namespace helios
{
    class RespondsToMouseComponent : public BaseComponent
    {
    private:
        std::vector<char> mCurrentButtons;
        struct {
            int x, y;
            } mPos;
        
    public:
        
        RespondsToMouseComponent(IEntity& owner);
        ~RespondsToMouseComponent();
        
        void Update(uint64_t) ;
        void Process();
        void DispatchEvents();
        
        IEvent_ptr operator()() { 
            IEvent_ptr p;
            return p;
        };
        
        void* operator new (size_t size, ILayer& layer)
        {
            return layer.GetPool().alloc(e::kComponentRespondsToMouse, size);
        };
        void operator delete ( void * ptr )
        {           
            
            ILayer* owner=0L;
            
            auto layers = SceneManager::Inst().GetLayers();
            
            auto it = layers.begin();
            auto ite = layers.end();
            for ( ; it != ite && !owner ; ++it )
            {
                owner = (*it).second->OwnsComponent((IComponent*)ptr, e::kComponentRespondsToMouse);
                
            }
            
            
            owner->GetPool().release(ptr, e::kComponentRespondsToMouse);
        };

    };
    
};


#endif
