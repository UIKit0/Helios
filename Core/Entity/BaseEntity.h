//
//  BaseEntity.h
//  Helios
//
//  Created by James Hurley on 9/19/11.
//

#ifndef Helios_BaseEntity_h
#define Helios_BaseEntity_h

#include <Entity/IEntity.h>
#include <Component/IComponent.h>
#include <Layer/ILayer.h>
#include <Event/HEvent.h>

namespace helios
{
    class BaseEntity : public IEntity
    {
    protected:
        
        ILayer* mOwner;
        std::map<std::string, std::vector<IComponent*> > mComponents;
        std::string mTag;
        
    public:
        BaseEntity(ILayer* owner) : mOwner(owner) 
        { 
            static int tag = 0;
            
            mTag = "entity" + tag;
            
            tag++;
        };
        BaseEntity(ILayer* owner, std::string tag) : mOwner(owner), mTag(tag) {};
                                            
        ~BaseEntity()
        {
            auto it = mComponents.begin();
            
            while(it != mComponents.end())
            {
                auto jt = (*it).second.begin() ;
                for ( ; jt != (*it).second.end() ; ++jt ) {
                   // D_PRINT("Deleting component... %s", (*jt)->Name().c_str());
                    delete (*jt);
                   
                }
                 ++it;
            }
            
            mOwner->UnRegisterComponents(mComponents); 
            mComponents.clear();

        };
        
        virtual void PushEvent( std::string const & target, IEvent_ptr e) 
        {
            e->SetOwner(this);
            mEvents[target].push_back(e);
        };
        
        ILayer* GetOwner() { return mOwner; } ;
        
        void SetOwner(ILayer* owner) { mOwner = owner; } ;

        virtual std::vector<IComponent*> GetComponent ( std::string mask )
        {
            if(mComponents.find(mask) != mComponents.end()) 
                return mComponents[ mask ];
            
            return std::vector<IComponent*>();
        };
        virtual void ClearEvents() 
        {
            for( auto it = mEvents.begin() ; it != mEvents.end() ; ++it)
            {
               // D_PRINT("Clearing %s\n", it->first.c_str());
                it->second.clear();
            }
            mEvents.clear();
        };
        virtual std::string& GetTag() { return mTag; } ;
        virtual void SetTag(std::string tag) { mTag = tag; };
    };    
};

#endif
