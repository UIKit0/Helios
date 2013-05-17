//
//  BaseLayer.h
//  Helios
//
//  Created by James Hurley on 9/15/11.
//

#ifndef Helios_BaseLayer_h
#define Helios_BaseLayer_h
#include <Layer/ILayer.h>
#include <glm/glm.hpp>
#include <Memory/Pool.h>
#include <Render/IRender.h>
#include <Component/IComponent.h>
#include <Algo/SpatialIndex/ISpatialIndex.h>
#include <common.h>
#include <Component/RenderableComponent.h>
#include <Component/LightComponent.h>
#include <Scene/IScene.h>


#include <Event/Events.h>

#include <Entity/IEntity.h>

namespace helios
{   
    class ISimulator;
    
    class BaseLayer : public ILayer
    {
    protected:
        
        std::shared_ptr<ISpatialIndex> mSpatialIndex;
        IScene&         mOwner;
        
        glm::mat4 mMV, mP;                      // modelView, projection
        uint64_t  mCurrentTimestamp;
        
        std::vector<std::shared_ptr<IEntity> > mEntities;
        std::map<std::string, std::vector<std::shared_ptr<IComponent> > > mComponents;
        std::map<std::string, unsigned> mShaders;
        std::map<std::string, unsigned> mVAOs;
        std::map<unsigned, std::map<std::string, int> > mUniforms;
        std::map<std::string, std::vector<IEvent_ptr> > mEvents;
        
        unsigned mCurrentShader;
        unsigned mCurrentVAO;
        
        std::shared_ptr<Pool>       mPool;
        std::shared_ptr<IRender>    mRender;
        std::shared_ptr<ISimulator> mSimulator;
        

        
        struct 
        {
            short x, y, w, h, dpi;
        } mScreen;
        
        unsigned char mSkipRender = 0;
        
        bool mIsActive = FALSE;
    public:
        BaseLayer(IScene& owner) : mScreen( { 0, 0, 0, 0 } ), mOwner(owner)
        {
            
        };
        ~BaseLayer() {};
        
        bool IsActive() { return mIsActive ;} ;
        // ---------------------------------------------------------------------
        virtual IScene& GetOwner() { return mOwner; };
        // ---------------------------------------------------------------------
        
        virtual unsigned GetShader(std::string n)
        {
            return mShaders[n];
        };
        virtual unsigned GetCurrentShader()
        {
            return mCurrentShader;
        };
        // ---------------------------------------------------------------------
        
        virtual unsigned GetVAO(std::string n)
        {
            return mVAOs[n];
        };
        
        virtual unsigned GetCurrentVAO() 
        {
            return mCurrentVAO;
        };
        // ---------------------------------------------------------------------
        
        
        void SetViewport ( int x, int y, int w, int h, int dpi = 72 )
        {
            mScreen.x = x;
            mScreen.y = y;
            mScreen.w = w;
            mScreen.h = h;
            mScreen.dpi = dpi;
        };
        void GetViewport ( int& x, int& y, int &w, int &h)
        {
            x = mScreen.x;
            y = mScreen.y;
            w = mScreen.w;
            h = mScreen.h;            
        };

        // ---------------------------------------------------------------------
        
        virtual void onActive(uint64_t time) 
        {
            mIsActive = TRUE;
        };
        
        virtual void onDeactive() 
        {
            mIsActive = FALSE;
        };
        // ---------------------------------------------------------------------
        
        virtual void UpdateLayer()
        {
            std::vector<std::shared_ptr<IComponent> > & r = mComponents[e::kComponentRenderable];
            for (auto it = r.begin() ; it != r.end() ; ++it)
            {
                if(!(*it)->IsActive()) (*it)->SetActive(true);
            }
        };
        
        virtual void Update(uint64_t time)
        {
            {
                auto e = GetEvents(e::kEventTargetMatrix);
                for (auto it = e.begin() ; it != e.end() ; ++it)
                {
                    if( (*it)->GetName() == e::kEventMatrixCamera )
                    {
                        mMV = HEvent<glm::mat4>::GetData((*it));
                    }
                }
            }
            mEvents[e::kEventTargetMatrix].push_back(std::shared_ptr<HEvent<glm::mat4> >(new HEvent<glm::mat4>(e::kEventTargetMatrix, e::kEventMatrixModelView, mMV)));
                                             
            mEvents[e::kEventTargetMatrix].push_back(std::shared_ptr<HEvent<glm::mat4> >(new HEvent<glm::mat4>(e::kEventTargetMatrix, e::kEventMatrixProjection, mP)));
            
            
            {
                auto it = mComponents.begin();
                auto ite = mComponents.end();
                
               
                
                for ( ; it != ite ; ++ it )
                {
                    for ( auto jt = (*it).second.begin() ; jt != (*it).second.end() ; ++ jt )
                    {
                        (*jt)->Update(time);
                    }
                    
                }
            }
            mEvents.clear();
        };
        // ---------------------------------------------------------------------
        
        virtual void Process() 
        {

            auto it = mComponents.begin();
            auto ite = mComponents.end();
            
            
            for ( ; it != ite ; ++ it )
            {

                    auto jt = (*it).second.begin();
                    auto jte = (*it).second.end();
                    
                    for ( ; jt != jte ; ++ jt ) 
                    {
                    
                        (*jt)->Process();
                    }
            }
        };
        // ---------------------------------------------------------------------
        
        virtual void DispatchEvents()
        {
            
           for ( auto it = mEntities.begin() ; it != mEntities.end() ; ++ it )
            {
                (*it)->ClearEvents();
            }
            
            // Dispatch events.
            auto it = mComponents.begin();
            auto ite = mComponents.end();
            
            for ( ; it != ite ; ++ it )
            {
                auto jt = (*it).second.begin();
                auto jte = (*it).second.end();
                for ( ; jt != jte ; ++ jt ) 
                {
                    (*jt)->DispatchEvents();
                }
            }
        };
        // ---------------------------------------------------------------------
        
        virtual void Render(uint64_t time) 
        {
            auto renderables = mComponents[e::kComponentRenderable];
            auto lights = mComponents[e::kComponentLight];
            for ( auto it = renderables.begin() ; it != renderables.end() ; ++ it ) 
            {
                IEvent_ptr p = (*(std::static_pointer_cast<RenderableComponent>(*it)))();
                mRender->PushRenderCommand(helios::HEvent<std::vector<helios::RenderCommand> >::GetData(p));
            }
            std::vector<LightCommand> lightCommands;
            for( auto it = lights.begin() ; it != lights.end() ; ++it)
            {
                IEvent_ptr p = (*(std::static_pointer_cast<helios::LightComponent>(*it)))();
                //mRender->PushLight(helios::HEvent<helios::RenderCommand>::GetData(p));
            }
            mRender->SetViewport(mScreen.x,mScreen.y,mScreen.w,mScreen.h);
            mRender->Render();
        };
        // ---------------------------------------------------------------------
        
        void SetRenderer(IRender* renderer)
        {
            mRender.reset(renderer);
        };
        std::shared_ptr<IRender> GetRenderer()
        {
            return mRender;
        };
        // ---------------------------------------------------------------------
        
        void RegisterComponent(IComponent* component)
        {
            std::shared_ptr<IComponent> ptr(component);

            mComponents[ component->Name() ].push_back ( ptr ) ;
        };
        
        void RegisterComponents(std::map<std::string, std::vector<IComponent*> > components)
        {
           auto it = components.begin();
            
            while(it != components.end())
            {
                for ( auto jt = (*it).second.begin() ; jt != (*it).second.end() ; ++jt) 
                    this->RegisterComponent((*jt));
                ++it;
            }
            
        };
        // ---------------------------------------------------------------------
        
        void UnRegisterComponent(IComponent* component)
        {
            UnRegisterComponent(component, component->Name());
        };
        void UnRegisterComponent(IComponent* component, std::string mask)
        {
            auto it = mComponents[mask].begin();
            while (it != mComponents[mask].end())
            {
                if( (*it).get() == component )
                {
                    it = mComponents[mask].erase(it);
                } else {
                    ++it;
                }
            } 
        };
        void UnRegisterComponents(std::map<std::string, std::vector<IComponent*> > components)
        {
            auto it = components.begin();
            
            while(it != components.end())
            {
                for (auto jt = (*it).second.begin() ; jt != (*it).second.end() ; ++jt)
                    this->UnRegisterComponent((*jt), (*it).first);
                ++it;
            }
            
        };
        
        // ---------------------------------------------------------------------
        
        Pool & GetPool() {
        
            return (*mPool); 
        };
        // ---------------------------------------------------------------------
        
        ILayer* OwnsComponent(IComponent* component, std::string name)
        {
            auto it = mComponents[ name ].begin();
            auto ite = mComponents[ name ].end();
            
            for ( ; it != ite ; ++it )
            {
                if( (*it).get() == component )
                {
                    return this;
                }
            }
            return 0L;
            
        };
        // ---------------------------------------------------------------------
        
        virtual std::map<std::string, int>& Uniforms() { return mUniforms[mCurrentShader]; };
        // ---------------------------------------------------------------------
                
        virtual void PushEvent(IEvent_ptr e) 
        {
            mEvents[e->GetTarget()].push_back(e);
        };
        virtual std::vector<IEvent_ptr> & GetEvents( std::string const & target)
        {
            static std::vector<IEvent_ptr> empty;
            if(mEvents.find(target) != mEvents.end()) {
                return mEvents[target];
            }
           
            return empty;
        };
    };
};

#endif
