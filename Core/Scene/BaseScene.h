//
//  BaseScene.h
//  Qt3
//
//  Created by James Hurley on 9/22/11.
//

#ifndef Helios_BaseScene_h
#define Helios_BaseScene_h
#include <Scene/IScene.h>
#include <Layer/ILayer.h>
//#include <Event/TouchEvent.h>
#include <Render/IRender.h>
#include <Event/Events.h>

namespace helios
{
    class BaseScene : public IScene
    {
    protected:
        
        std::map<std::string, ILayer*> mLayers;
        IRender* mRenderer = 0L;
        short mScreenX, mScreenY, mScreenW, mScreenH, mScreenDPI;
        float mScreenDensityScale;                  // i.e. 1.0 for non-retina displays, 2.0 for retina (300+ dpi) screens
        std::map<std::string, std::vector<IEvent_ptr> > mEvents;
        
    public:
        // ---------------------------------------------------------------------
        BaseScene() {};
        ~BaseScene() {};
        
        
        virtual void PushEvent(IEvent_ptr e) 
        {
            mEvents[e->GetTarget()].push_back(e);
        };
        virtual std::vector<IEvent_ptr> & GetEvents( std::string const & target) 
        {
            return mEvents[target];
        };
        
        // ---------------------------------------------------------------------
        virtual size_t RegisterLayer(std::string name,ILayer* layer);
        
        // ---------------------------------------------------------------------
        virtual size_t UnRegisterLayer(std::string name,ILayer* layer)
        {
            mLayers.erase(mLayers.find(name)); 
            return 0;
        };
        // ---------------------------------------------------------------------
        std::map<std::string,ILayer*> & GetLayers()
        { 
            return mLayers; 
        } ;
        // ---------------------------------------------------------------------
        virtual void onScreenTouchBegan(HEvent<Touches>& e) 
        {
            for ( auto it = mLayers.begin() ; it != mLayers.end() ; ++ it) 
            {
                if((*it).second->IsActive())
                    (*it).second->onScreenTouchBegan(e);
            }
        };
        // ---------------------------------------------------------------------
        virtual void onScreenTouchMoved(HEvent<Touches>& e)  
        {
            for ( auto it = mLayers.begin() ; it != mLayers.end() ; ++ it) 
            {
                if((*it).second->IsActive())
                    (*it).second->onScreenTouchMoved(e);
            }
        };
        // ---------------------------------------------------------------------
        virtual void onScreenTouchEnded(HEvent<Touches>& e)  
        {
            for ( auto it = mLayers.begin() ; it != mLayers.end() ; ++ it) 
            {
                if((*it).second->IsActive())
                    (*it).second->onScreenTouchEnded(e);
            }
        };
        // ---------------------------------------------------------------------
        virtual void onScreenTouchCanceled() 
        {
            for ( auto it = mLayers.begin() ; it != mLayers.end() ; ++ it) 
            {
                if((*it).second->IsActive())
                    (*it).second->onScreenTouchCanceled();
            }
        };
        // ---------------------------------------------------------------------
        virtual void onKeyDown(helios::HEvent<helios::KeyEvent>& k)
        {
            
        };
        // ---------------------------------------------------------------------
        virtual void onKeyUp(helios::HEvent<helios::KeyEvent>& k)
        {
            
        };
        // ---------------------------------------------------------------------
        virtual void onKeyModifierChanged(helios::HEvent<helios::KeyEvent>& k)
        {
            
        };
        // ---------------------------------------------------------------------
        
        virtual void onActive(uint64_t time)
        {
            for ( auto it = mLayers.begin() ; it != mLayers.end() ; ++ it) 
            {
                if(!(*it).second->IsActive())
                    (*it).second->onActive(time);
            }
        };
        // ---------------------------------------------------------------------
        virtual void onDeactive() 
        {
            for ( auto it = mLayers.begin() ; it != mLayers.end() ; ++ it) 
            {
                if((*it).second->IsActive())
                    (*it).second->onDeactive();
            }
        };
        // ---------------------------------------------------------------------
        virtual void UpdateScene()
        {
            
        }
        // ---------------------------------------------------------------------
        virtual void UpdateLayer()
        {
            for ( auto it = mLayers.begin() ; it != mLayers.end() ; ++it)
            {
                if((*it).second->IsActive())
                    (*it).second->UpdateLayer();
            }
            
        };
        // ---------------------------------------------------------------------
        virtual void Update(uint64_t time) 
        {
            for ( auto it = mLayers.begin() ; it != mLayers.end() ; ++ it) 
            {
                if((*it).second->IsActive())
                    (*it).second->Update(time);
            }
        };
        // ---------------------------------------------------------------------
        virtual void Process() 
        {
            for ( auto it = mLayers.begin() ; it != mLayers.end() ; ++ it) 
            {
                if((*it).second->IsActive())
                    (*it).second->Process();
            }
        };
        // ---------------------------------------------------------------------
        virtual void DispatchEvents() {
            for ( auto it = mLayers.begin() ; it != mLayers.end() ; ++ it) 
            {
                if((*it).second->IsActive())
                    (*it).second->DispatchEvents();
            }
        };
        // ---------------------------------------------------------------------
        virtual void Render(uint64_t time) 
        {
            mRenderer->SetViewport(mScreenX,mScreenY,mScreenW,mScreenH);
            mRenderer->ClearViewport();
            for ( auto it = mLayers.begin() ; it != mLayers.end() ; ++ it) 
            {
                if((*it).second->IsActive())
                    (*it).second->Render(time);
            }
        };
        // ---------------------------------------------------------------------
        virtual void SetViewport(short x, short y, short w, short h, int dpi)
        {
            mScreenX = x;
            mScreenY = y;
            mScreenW = w;
            mScreenH = h;
            mScreenDPI = dpi;
        };
        // ---------------------------------------------------------------------
        virtual void SetRenderer(IRender* renderer) 
        {
            mRenderer = renderer;
        };
        // ---------------------------------------------------------------------
        virtual void NetworkRecv(IEvent_ptr e) 
        {
            for( auto it = mLayers.begin() ; it != mLayers.end() ; ++it )
            {
                if((*it).second->IsActive())
                    (*it).second->NetworkRecv(e);
            }
        };
        virtual void NetworkSend(IEvent_ptr e)
        {
            
        };
    };
};


#endif
