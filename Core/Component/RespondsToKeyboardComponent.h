
#ifndef Helios_RespondsToKeyboardComponent_h
#define Helios_RespondsToKeyboardComponent_h

#include <Component/BaseComponent.h>
#include <Event/Touch.h>
namespace helios
{
    class RespondsToKeyboardComponent : public BaseComponent
    {
    private:
        std::vector<std::pair<int, int>> mCurrentKeys;
        union {
            modifier_flags_t mFlags;    
            int16_t          mModifiers;
        };
        
    public:
        
        RespondsToKeyboardComponent(IEntity& owner);
        ~RespondsToKeyboardComponent();
        
        void Update(uint64_t) ;
        void Process();
        void DispatchEvents();
        IEvent_ptr operator()() { 
            IEvent_ptr p;
            return p;
        }
        
        void* operator new (size_t size, ILayer& layer)
        {
            return layer.GetPool().alloc(e::kComponentRespondsToKeyboard, size);
        };
        void operator delete ( void * ptr )
        {           
            
            ILayer* owner=0L;
            
            auto layers = SceneManager::Inst().GetLayers();
            
            auto it = layers.begin();
            auto ite = layers.end();
            for ( ; it != ite && !owner ; ++it )
            {
                owner = (*it).second->OwnsComponent((IComponent*)ptr, e::kComponentRespondsToKeyboard);
                
            }
            
            
            owner->GetPool().release(ptr, e::kComponentRespondsToKeyboard);
        };

    };
    
};


#endif
