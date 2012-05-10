#include <Component/RespondsToMouseComponent.h>

namespace helios
{
    RespondsToMouseComponent::RespondsToMouseComponent(IEntity& owner) : BaseComponent(owner)
    {
        
    }
    RespondsToMouseComponent::~RespondsToMouseComponent()
    {
        
    }
    void 
    RespondsToMouseComponent::Update(uint64_t t) 
    {
        auto & events = mOwner.GetOwner()->GetEvents(e::kEventTargetMouse);
        
        for ( auto it = events.begin() ; it != events.end() ; ++it )
        {
            if((*it)->GetName() == e::kEventMousePos)
            {
                auto k = HEvent<std::pair<int, int> >::GetData((*it));
                D_PRINT("-> Got pos: %d,%d\n",k.first,k.second);
            }
        }
        
    }
    void 
    RespondsToMouseComponent::Process()
    {
        
    }
    void 
    RespondsToMouseComponent::DispatchEvents()
    {
        
    }
}