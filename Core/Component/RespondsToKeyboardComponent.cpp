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
                
                //D_PRINT("-> Got key: %d\n",k.keyCode);
               
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
        // todo: create remapping system for actions
        
        glm::vec3 v(0.,0.,0.);
        std::vector<std::vector<std::pair<int,int> >::iterator> toErase;
        
        for ( auto it = mCurrentKeys.begin() ; it != mCurrentKeys.end() ; ++it)
        {
            int k = (*it).first;
            int stage = (*it).second ;
            
            std::string action = helios::SceneManager::Inst().GetConfiguration().GetActionForKey(k) ;

            boost::shared_ptr<HEvent<const char> > p ( new HEvent<const char>(e::kEventTargetAction, action, stage));

            if(stage == 0 )
            {
                (*it).second++;
            }
            else if(stage == 2)
            {
                std::cout << "Marking " << (*it).first << std::endl ; 
                toErase.push_back(it);
            }
            mOwner.PushEvent(e::kEventTargetAction, p);
        }

        for( auto it = toErase.begin() ; it != toErase.end() ; ++it )
        {
            std::cout << "Erasing " << (*(*it)).first << std::endl ;
            mCurrentKeys.erase((*it)) ;
        }
        
        if(!(v.x == 0. && v.y == 0. && v.z == 0.) && mCurrentKeys.size() > 0) {
            v = glm::normalize(v);
        

        }
    }
}