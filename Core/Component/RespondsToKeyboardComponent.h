
#ifndef Helios_RespondsToKeyboardComponent_h
#define Helios_RespondsToKeyboardComponent_h

#include <Component/BaseComponent.h>

namespace helios
{
    class RespondsToKeyboardComponent : public BaseComponent
    {
    private:
        
        std::map<uint32_t, uint32_t> mRemapTable;
        
    public:
        
        RespondsToKeyboardComponent(IEntity& owner);
        ~RespondsToKeyboardComponent();
        
        void Update(uint64_t) ;
        void Process();
        void DispatchEvents();
        
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
