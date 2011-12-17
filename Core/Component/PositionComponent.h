//
//  PositionComponent.h
//  Helios
//
//  Created by James Hurley on 9/22/11.
//

#ifndef Helios_PositionComponent_h
#define Helios_PositionComponent_h
#include <Component/BaseComponent.h>
#include <glm/glm.hpp>
namespace helios
{
    class PositionComponent : public BaseComponent
    {
    private:
        
        glm::mat4   mCurrentPosition;
        glm::quat   mCurrentRotation;
     //   HEvent<glm::mat4> mCurrentEvent;
        glm::vec3   mPosition;
        glm::vec3   mScale;
        glm::vec3   mTargetPosition;
        uint64_t    mCurrentTime;
        uint64_t    mLastTime;
        float       mSpeed;
        
    public:
        PositionComponent(IEntity& owner, short x, short y, short w, short h);
        PositionComponent(IEntity& owner, glm::vec3 position, glm::vec3 scale);
        ~PositionComponent(){};
        void Process();
        void Update(uint64_t) ;
        void DispatchEvents();
        
        
        // speed unit is OpenGL Units/second
        void MoveTo(glm::vec3 pos, float speed);
        
        void Rotate(glm::quat rotation) { mCurrentRotation = rotation ; } ;
        
        IEvent_ptr operator()() { 
            return boost::shared_ptr< HEvent<glm::mat4> >( new HEvent<glm::mat4>(e::kEventTargetPosition, e::kEventPositionMatrix, mCurrentPosition ) );
        };
        void* operator new (size_t size, ILayer& layer)
        {
            return layer.GetPool().alloc(e::kComponentPosition, size);
        };
        void operator delete ( void * ptr )
        {           
            D_PRINT("Delete Position");
            ILayer* owner = 0L;
            
            auto layers = SceneManager::Inst().GetLayers();
            
            auto it = layers.begin();
            auto ite = layers.end();
            for ( ; it != ite && !owner ; ++it )
            {
                owner = (*it).second->OwnsComponent((IComponent*)ptr, e::kComponentPosition);
                
            }
            
            
            owner->GetPool().release(ptr, e::kComponentPosition);
        };

    };
}


#endif
