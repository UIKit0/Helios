//
//  SceneManager.h
//  Helios
//
//  Created by James Hurley on 9/15/11.
//

#ifndef Helios_SceneManager_h
#define Helios_SceneManager_h

#include <Event/Events.h>

#ifdef _LIBCPP_STABLE_APPLE_ABI
    #include <thread>
    #include <mutex>
#else
    #include <boost/utility.hpp>
    #include <boost/scoped_ptr.hpp>
    #include <boost/thread/thread.hpp>
    #include <boost/thread/once.hpp>
#endif

#include <boost/property_tree/ptree.hpp>

#include <map>
#include <vector>
#include <Render/TextureAtlas.hpp>
#include <common.h>

namespace helios
{
    
    class CharacterModel;
    class ILayer;
    class IRender;
    class IModel;
    class IScene;
    
    class SceneManager
    {
    private:
        
        uint64_t                mCurrentTimestamp;
#ifdef _LIBCPP_STABLE_APPLE_ABI
        std::mutex              mMutex;
#else
        boost::mutex            mMutex;
#endif
        boost::property_tree::ptree mSettings;
        
        //ILayer*                 mCurrentLayer = 0L;
        IScene*                 mCurrentScene = 0L;
        IRender*                mRender = 0L;
        
        std::map<std::string, IScene*> mScenes;
        std::string             mResourceFolder;
        std::string             mDocumentsFolder;
        
        std::map<std::string, TextureAtlas*> mAtlasDict;
        
        short mScreenW, mScreenH, mScreenDPI;
        
    public:
       
        static SceneManager& Inst()
        {
            static SceneManager s_instance;
            return s_instance;
        }
        
         
        std::map<std::string, ILayer*>& GetLayers();
         
        void                 SwitchScene(std::string name);
        void                 RegisterScene(std::string, IScene* scene);
        void                 UnRegisterScene(std::string);
        
        IScene*              CurrentScene() { return mCurrentScene; } ;
        IScene*              GetScene(std::string name) { return mScenes[name]; };
        
       // ILayer*              CurrentLayer() { return mCurrentLayer; } ;
       
       // TODO: Replace keyboard/touch events with PushEvent
        void                 PushEvent(IEvent_ptr e);
        
        void                 onScreenTouchBegan(helios::HEvent<helios::Touches>& e);
        void                 onScreenTouchMoved(helios::HEvent<helios::Touches>& e);
        void                 onScreenTouchEnded(helios::HEvent<helios::Touches>& e);
        void                 onScreenTouchCancelled();
        void                 onKeyDown(helios::HEvent<helios::KeyEvent>& k);
        void                 onKeyUp(helios::HEvent<helios::KeyEvent>& k);
        void                 onKeyModifierChanged(helios::HEvent<helios::KeyEvent>& k);
        
        void                 Render(uint64_t time);
        void                 FireUpdates(uint64_t time);
        void                 Process();
        void                 DispatchEvents();
        void                 UpdateLayer();
        
        void                 NetworkRecv(IEvent_ptr e);
        void                 NetworkSend(IEvent_ptr e);
        
        void                 SetResourceFolder(std::string folder) { mResourceFolder = folder; };
        std::string const &  GetResourceFolder() const { return mResourceFolder; } ;
        void                 SetDocumentsFolder(std::string folder) { mDocumentsFolder = folder; };
        std::string const &  GetDocumentsFolder() const { return mDocumentsFolder; };
        void                 SetScreenSize(int w,int h, int dpi) ;
        void                 GetScreenSize (int& w, int & h, int & dpi) { w = mScreenW; h = mScreenH; dpi = mScreenDPI; };
        
        TextureAtlas       & LoadAtlas ( std::string filename, std::string referencefile, short flags = 0);
        
        void                 Setup(IRender* renderer, std::string resourceFolder);
        void                 ShutDown();
        void                 Background();
        void                 Foreground();
        
        boost::property_tree::ptree & Settings() { return mSettings; } ;
    };    
};


#endif
