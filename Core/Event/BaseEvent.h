//
//  BaseEvent.h
//  Helios
//
//  Created by James Hurley on 9/15/11.
//

#ifndef Helios_BaseEvent_h
#define Helios_BaseEvent_h

#include <Event/IEvent.h>
#include <common.h>
namespace helios
{
   
    template <typename T>
    class BaseEvent : public IEvent
    {
    protected:
        std::string          mName;
        std::string          mTarget;
        std::string          mType;
        T                    mData;
        
    public:
    
        BaseEvent() : mName(e::kEventBase), mTarget(e::kEventTargetBase),  mType(typeid(T).name()) { };
        BaseEvent(std::string const & target, std::string const & name, T data) : mData(data), mTarget(target), mName(name) {} ;
        ~BaseEvent() 
        {
        }
        bool operator==( IEvent& rhs ) 
        {
            return (mType == rhs.GetType());
        };
        void SetTarget(std::string target)
        {
            mTarget = target;
        };
        void SetName(std::string name)
        {
            mName = name;
        };
        std::string & GetTarget()
        {
            return mTarget;
        };
        std::string & GetName()
        {
            return mName;
        };
        std::string& GetType()
        {
            return mType;
        };
        std::string & operator()()
        {
            return GetTarget();
        };
        void SetData(void* data, size_t size, const std::type_info & type) 
        {
            mType = type.name();
         
            memcpy(&mData, data, size);
            
        };
        void GetData(void* data, size_t& size, std::string& type)
        {
            size = sizeof(T);
            data = &mData;
            type = mType;
        }
        void SetData(T & data)
        {
            mData = data;
            mType = typeid(data).name();
        }
        T & GetData()
        {
            return mData;
        };    
    };
};

#endif
