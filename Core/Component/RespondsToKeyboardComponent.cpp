#include <Component/RespondsToKeyboardComponent.h>
#include <glm/glm.hpp>
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
            auto k = HEvent<KeyEvent>::GetData((*it));
            
            if((*it)->GetName() == e::kEventKeyDown)
            {
                mCurrentKeys.push_back(std::pair<int,int>(k.keyCode,0));
            }
            else if((*it)->GetName() == e::kEventKeyUp)
            {                
                auto it = std::find(mCurrentKeys.begin(),mCurrentKeys.end(),std::pair<int,int>(k.keyCode,0));

                if( it == mCurrentKeys.end())
                {
                    it = std::find(mCurrentKeys.begin(), mCurrentKeys.end(), std::pair<int,int>(k.keyCode,1));
                }
                (*it).second = 2;
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

        std::vector<std::vector<std::pair<int,int> >::iterator> toErase;
        
        for ( auto it = mCurrentKeys.begin() ; it != mCurrentKeys.end() ; ++it)
        {
            int k = (*it).first;
            int stage = (*it).second ;
            
            std::string action = helios::SceneManager::Inst().GetConfiguration().GetActionForKey(k) ;

            std::shared_ptr<HEvent<const char> > p ( new HEvent<const char>(e::kEventTargetAction, action, stage));

            if(stage == 0 )
            {
                (*it).second++;
            }
            else if(stage == 2)
            {
                toErase.push_back(it);
            }
            mOwner.PushEvent(e::kEventTargetAction, p);
        }

        for( auto it = toErase.begin() ; it != toErase.end() ; ++it )
        {
            mCurrentKeys.erase((*it)) ;
        }
        
    }
}