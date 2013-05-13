//
//  RenderableComponent.h
//  Helios
//
//  Created by James Hurley on 9/21/11.
//

#ifndef Helios_RenderableComponent_h
#define Helios_RenderableComponent_h
#include <Component/BaseComponent.h>
#include <Render/TextureAtlas.hpp>
#include <glm/glm.hpp>
//#include <Event/RenderEvent.h>
#include <map>
#include <Render/RenderCommand.h>

namespace helios
{
    //typedef BaseEvent<glm::mat4> MatrixEvent;
    

    class RenderableComponent : public BaseComponent
    {
    private:
        
        /** TODO: 
         
            Remove mVBO/VAO/IBO/ETC. Add those to Material Group data.
            Add shader info to material group data.
            Force even basic sprites to use material groups.
         
         **/
        
        HEvent<std::vector<RenderCommand> > mCurrentEvent;
              
        glm::mat4 mCurrentBounds;
        glm::mat4 mCurrentMVMat;
        glm::mat4 mCurrentPMat;
        glm::mat3 mCurrentTMat;
        
        Texture*                mTexture;
        std::string             mTextureName;
       
        std::vector<UniformData_ptr> mUniformData;

        std::vector<s::MaterialGroup> mMaterialGroups;
        
        unsigned                mVBO;
        unsigned                mIBO;
        RenderState             mRenderState;
        short                   mIBOSize;
        
        struct {
            byte                mOwnsTexture : 1 ;
            byte                mTextureType : 2 ;
            byte                mExtrudeStencilShadows:1;
        };
        
        
    public:
        RenderableComponent (IEntity& owner) : BaseComponent(owner)
        { mName = e::kComponentRenderable; };
        
        RenderableComponent(IEntity& owner, TextureAtlas* atlas, std::string const & name);
        
        RenderableComponent(IEntity& owner, std::string texture);
        
        RenderableComponent(IEntity& owner, unsigned vbo, bool extrudeStencilShadow);
        

        
        ~RenderableComponent() ;
        
        void AddMaterialGroup ( s::MaterialGroup & group )  
        { 
            mMaterialGroups.push_back(group) ; 
        };
        
        void Process();
        void Update(uint64_t) ;
        void DispatchEvents();
        
        void Render(uint64_t);
        
        void AddUniform(const std::string * name, UniformData_ptr data)
        {
            
            for ( size_t i = 0 ; i < mUniformData.size() ; ++i )
            {
                if (name == (*mUniformData[i]).name) {
                    mUniformData[i] = data;
                    return;
                }
            }
            mUniformData.push_back(data);
            
        };
        void SetVBO( unsigned vbo ) { mVBO = vbo; }; 
        void SetIBO( unsigned ibo ) { mIBO = ibo; };
        void SetRenderState(RenderState r) { mRenderState = r; };
        void SetTextureAtlas(TextureAtlas* atl, std::string name)
        {
            mTexture = atl;
            mTextureType = 1;
            mTextureName = name;
        };
        IEvent_ptr operator()() { 
            if( !mIsActive ) 
            {
                std::vector<RenderCommand> & r = mCurrentEvent.GetData();
            
                for ( auto it = r.begin() ; it != r.end() ; ++it )
                {
                    (*it).ibo = 0;
                    (*it).iboSize = 0;
                    (*it).iboOffset = 0;
                }
            }
            
            return mCurrentEvent.ptr();
            
        };
        void* operator new (size_t size, ILayer& layer)
        {
            
            return layer.GetPool().alloc(e::kComponentRenderable, size);
        };
        void operator delete ( void * ptr )
        {           
            D_PRINT("Delete Renderable");
            ILayer* owner=0L;
            
            auto layers = SceneManager::Inst().GetLayers();
            
            auto it = layers.begin();
            auto ite = layers.end();
            for ( ; it != ite && !owner ; ++it )
            {
                owner = (*it).second->OwnsComponent((IComponent*)ptr, e::kComponentRenderable);
                
            }
            
            if(owner) {
                owner->GetPool().release(ptr, e::kComponentRenderable);
            } else {
                assert("Unable to find owner for this component.");
            }
        };
    };
}


#endif
