#include <Component/RespondsToKeyboardComponent.h>

namespace helios
{
    RespondsToKeyboardComponent::RespondsToKeyboardComponent(IEntity& owner) : BaseComponent(owner)
    {
        
    }
    RespondsToKeyboardComponent::~RespondsToKeyboardComponent()
    {
        
    }
    void 
    RespondsToKeyboardComponent::Update(uint64_t t) 
    {
        auto & events = mOwner.GetOwner()->GetEvents(e::kEventTargetKeyboard);
        
        for ( auto it = events.begin() ; it != events.end() ; ++it )
        {
            if((*it)->GetName() == e::kEventKeyDown)
            {
                auto k = HEvent<KeyEvent>::GetData((*it));
                D_PRINT("-> Got key: %d\n",k.keyCode);
            }
        }
        
    }
    void 
    RespondsToKeyboardComponent::Process()
    {
        
    }
    void 
    RespondsToKeyboardComponent::DispatchEvents()
    {
        
    }
}