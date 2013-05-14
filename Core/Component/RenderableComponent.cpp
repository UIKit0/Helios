//
//  RenderableComponent.cpp
//  Helios
//
//  Created by James Hurley on 9/21/11.
//

#include <iostream>
#include <Component/RenderableComponent.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <Render/IRender.h>

namespace helios
{
    RenderableComponent::RenderableComponent(IEntity& owner, TextureAtlas* atlas, std::string const & name)
    : BaseComponent(owner), mTextureName(name), mOwnsTexture(0), mTextureType(1), mTexture(atlas)
    {
        mName = e::kComponentRenderable;
        mVBO = mOwner.GetOwner()->GetRenderer()->GenerateDefaultVBO();
  
        mIBOSize = 6;
        mCurrentEvent.SetName(e::kEventRenderCommand);
        
        mCurrentEvent.SetTarget(e::kEventTargetRender);
        
        mIsActive = false;
        
        mRenderState.mask = 0;
        mRenderState.hud = 1;
        
        frame_t frame ;
        frame = (*(TextureAtlas*)mTexture)[mTextureName];
        mCurrentTMat = glm::mat3(1.0, 0.0, 0.0,
                                 0.0, 1.0, 0.0,
                                 frame.coords.minS, frame.coords.minT, 1.0) * 
                      glm::mat3(frame.coords.maxS - frame.coords.minS, 0.0, 0.0,
                                0.0, frame.coords.maxT - frame.coords.minT, 0.0,
                                0.0, 0.0, 1.0);
    }
    
    RenderableComponent::RenderableComponent(IEntity& owner, std::string texture)
    : BaseComponent(owner), mOwnsTexture(1), mTextureType(0)
    {
        mName = e::kComponentRenderable;
        D_PRINT("Generating texture...");
        mTexture = new Texture(texture);
        D_PRINT("Done.");
        mVBO = mOwner.GetOwner()->GetRenderer()->GenerateDefaultVBO();  

        mIBOSize = 6;
        mCurrentEvent.SetName(e::kEventRenderCommand);
        mCurrentTMat = glm::mat3(1.f);
        mCurrentEvent.SetTarget(e::kEventTargetRender);
        mIsActive = false;
        
        mRenderState.mask = 0;
        mRenderState.hud = 1;
        
        
        frame_t frame ;
        frame.coords.maxS = 1.f;
        frame.coords.maxT = 1.f;
        frame.coords.minS = 0.f;
        frame.coords.minT = 0.f;
        mCurrentTMat = glm::mat3(1.0, 0.0, 0.0,
                                 0.0, 1.0, 0.0,
                                 frame.coords.minS, frame.coords.minT, 1.0) * 
                      glm::mat3(frame.coords.maxS - frame.coords.minS, 0.0, 0.0,
                                0.0, frame.coords.maxT - frame.coords.minT, 0.0,
                                0.0, 0.0, 1.0);
    }
    RenderableComponent::RenderableComponent(IEntity& owner, unsigned vbo, RenderState renderMask)
    : BaseComponent(owner), mVBO(vbo), mOwnsTexture(0), mTextureType(0), mRenderState(renderMask)
    {
        mName = e::kComponentRenderable;
        mCurrentEvent.SetName(e::kEventRenderCommand);
        mCurrentTMat = glm::mat3(1.f);
        mCurrentEvent.SetTarget(e::kEventTargetRender);
        mIsActive = false;
    };
    RenderableComponent::~RenderableComponent()
    {
        if(mOwnsTexture)
        {
            delete mTexture;
        }

    }
    void 
    RenderableComponent::Process()
    {
        
        BaseComponent::Process();
        
        
        mCurrentEvent.GetData().clear();
        
        RenderCommand com;
        
        glm::mat4 pos = mCurrentBounds;
        glm::mat4 mv = mCurrentMVMat;
        glm::mat4 p  = mCurrentPMat;
        
        glm::mat4 er =  mv * pos; // Multiply ModelView * Position matrices to create MV uniform
        com.vbo = mVBO;
        //com.ibo = mIBO;
        com.state = mRenderState;
        
        // Generate texture matrix
        // 
        glm::mat3 tex;
        
        tex = mCurrentTMat;
       
        // Generate normal matrix
        glm::mat3 norm = glm::inverseTranspose(glm::mat3(mv*pos));
        
        {
            UniformData u(UniformData::kUTMat4, &e::kVertexUniformModelView);
            u.SetData<glm::mat4>(er);
        
            com.uniforms.push_back(UniformData_ptr(new UniformData(u)));

        }
        {
            UniformData u(UniformData::kUTMat4, &e::kVertexUniformProjection);
            u.SetData<glm::mat4>(p);
            com.uniforms.push_back(UniformData_ptr(new UniformData(u)));

        }
        {
            UniformData u(UniformData::kUTMat3, &e::kVertexUniformTexMat);
            u.SetData<glm::mat3>(tex);
            com.uniforms.push_back(UniformData_ptr(new UniformData(u)));
        }
        {
            UniformData u(UniformData::kUTMat3, &e::kVertexUniformNormalMat);
            u.SetData<glm::mat3>(norm);
            com.uniforms.push_back(UniformData_ptr(new UniformData(u)));
        }

        com.uniforms.insert(com.uniforms.begin(), mUniformData.begin(), mUniformData.end());
        
        glm::mat4 mvp = p * er;
        glm::vec4 dpos = mvp * glm::vec4(0.f,0.f,0.f,1.f);
        
        com.depth = ((dpos.z / dpos.w) * 0.5f + 0.5f) * 0xFFFF;

        for ( auto it = mMaterialGroups.begin() ; it != mMaterialGroups.end() ; ++it )
        {
            com.tex = (*it).tex;
            com.ibo = (*it).ibo;
            com.iboOffset = (*it).iboRange.start;
            com.iboSize   = (*it).iboRange.end - com.iboOffset;
            mCurrentEvent.GetData().push_back( com );
            
        }
        if(!mMaterialGroups.size())
        {
            if(mTexture) {
                com.tex =  mTexture->id();
            } else {
                com.tex = 0;
            }
            com.ibo = mIBO;
            com.iboOffset = 0;
            com.iboSize = mIBOSize;
            mCurrentEvent.GetData().push_back( com );
        }
        
        
        mCurrentEvent.SetTarget(e::kEventTargetRender);
        mCurrentEvent.SetTarget(e::kEventRenderCommand);
    
        
       
    }
    void 
    RenderableComponent::Update(uint64_t) 
    {
        /** Position - Could be moved to Matrix target **/
        /** Position **/
        {
            auto & events = mOwner.GetEvents(e::kEventTargetPosition);
            if(events.size() > 0 ) {
                mCurrentBounds = HEvent<glm::mat4>::GetData(events[0]);           
            }
        }
        /** Matrices **/
        {
            auto & events = mOwner.GetOwner()->GetEvents(e::kEventTargetMatrix);
            mCurrentMVMat = glm::mat4(0.f);
            mCurrentPMat = glm::mat4(0.f);
            
            for (auto it = events.begin() ; it != events.end() ; ++ it)
            {
                if((*it)->GetName() == e::kEventMatrixModelView)
                {
                    
                    mCurrentMVMat= HEvent<glm::mat4>::GetData((*it));
                } 
                else if((*it)->GetName() == e::kEventMatrixProjection)
                {
                    mCurrentPMat = HEvent<glm::mat4>::GetData((*it));
                    
                }
            }
            
        }
       /** Add uniforms pushed by other components. **/
        {
            auto & events = mOwner.GetEvents(e::kEventTargetUniformData);
            for ( auto it = events.begin() ; it != events.end() ; ++it)
            {
                if((*it)->GetName() == e::kEventUniformData)
                {

                    auto data = HEvent<UniformData_ptr>::GetData((*it));
                    AddUniform((*data).name,data);

                }
            }
        }
        /** Texture Mat **/
        {
            auto & events = mOwner.GetEvents(e::kEventTargetTexture);
            
            for ( auto it = events.begin() ; it != events.end () ; ++it )
            {
                mCurrentTMat = HEvent<glm::mat3>::GetData((*it));
            }
             
        }
    }
    void 
    RenderableComponent::DispatchEvents()
    {
        
    }
    
};
