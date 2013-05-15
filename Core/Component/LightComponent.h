//
//  LightComponent.h
//  Helios
//
//  Created by James Hurley on 9/21/11.
//

#ifndef Helios_LightComponent_h
#define Helios_LightComponent_h
#include <Component/BaseComponent.h>
#include <Render/TextureAtlas.hpp>
#include <glm/glm.hpp>
//#include <Event/RenderEvent.h>
#include <map>
#include <Render/RenderCommand.h>

namespace helios
{

    class LightComponent : public BaseComponent
    {
    private:

        /** TODO:

         Remove mVBO/VAO/IBO/ETC. Add those to Material Group data.
         Add shader info to material group data.
         Force even basic sprites to use material groups.

         **/

        HEvent< LightCommand > mCurrentEvent;

        glm::mat4 mCurrentMVMat;
        glm::mat4 mCurrentPos;
        glm::vec4 mColor;
        bool mIsShadowCaster;
    public:
        LightComponent(IEntity& owner) : BaseComponent(owner)
        { mName = e::kComponentLight; };

        LightComponent(IEntity& owner, glm::vec4 color, bool isShadowCaster = false);

        ~LightComponent() ;

        void Process();
        void Update(uint64_t) ;
        void DispatchEvents();

        void Render(uint64_t);

        void Color(glm::vec4 color) { mColor = color ; } ;
        
        IEvent_ptr operator()() {

            return mCurrentEvent.ptr();

        };
        void* operator new (size_t size, ILayer& layer)
        {

            return layer.GetPool().alloc(e::kComponentLight, size);
        };
        void operator delete ( void * ptr )
        {
            D_PRINT("Delete Light");
            ILayer* owner=0L;

            auto layers = SceneManager::Inst().GetLayers();

            auto it = layers.begin();
            auto ite = layers.end();
            for ( ; it != ite && !owner ; ++it )
            {
                owner = (*it).second->OwnsComponent((IComponent*)ptr, e::kComponentLight);

            }

            if(owner) {
                owner->GetPool().release(ptr, e::kComponentLight);
            } else {
                assert("Unable to find owner for this component.");
            }
        };
    };
}


#endif
