//
//  IEntity.h
//  Helios
//
//  Created by James Hurley on 9/15/11.
//

#ifndef Helios_IEntity_h
#define Helios_IEntity_h

#include <string>
#include <vector>
#include <common.h>
#include <Event/HEvent.h>
#include <map>


#include <memory>


namespace helios
{
    class IComponent;
    //class IEvent;
    class ILayer;
    
    class IEntity
    {
    protected:
        std::map<std::string, std::vector<IEvent_ptr> > mEvents;
    public:
        virtual ~IEntity() {};
        virtual ILayer* GetOwner() = 0;
        virtual void SetOwner(ILayer*) = 0;
        virtual std::vector<IComponent*> GetComponent ( std::string )  = 0;
        
        virtual void PushEvent(std::string const & target, IEvent_ptr e) = 0;
        
        virtual std::string& GetTag() = 0;
        virtual void SetTag(std::string tag) = 0;
        
        std::vector<IEvent_ptr> & GetEvents( std::string const & target)
        {
            static std::vector<IEvent_ptr> empty;
            if(mEvents.find(target) != mEvents.end())
                return mEvents[target];
            
            return empty;
        };
        
        template <typename T>
        boost::shared_ptr<HEvent<T> > PushEvent(std::string const & target, std::string const & name, T& data) 
        {
            boost::shared_ptr< HEvent<T> > _event( new HEvent<T>(target, name, data) );
            _event->SetOwner(this);
           // mEvents[target].push_back((IEvent_ptr)_event);
            
            PushEvent(target, _event);
            
            return _event;
        };
        
        virtual void ClearEvents() = 0;        

        
    };    
};

#endif
