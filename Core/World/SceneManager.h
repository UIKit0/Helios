//
//  SceneManager.h
//  Helios
//
//  Created by James Hurley on 9/15/11.
//

#ifndef Helios_SceneManager_h
#define Helios_SceneManager_h

#include <Event/Events.h>


#include <thread>
#include <mutex>

#include <map>
#include <vector>

#include <World/Configuration.h>
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

        std::mutex              mMutex;

        Configuration*          mConfiguration ;
        
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

        
        const Configuration& GetConfiguration() const { return *mConfiguration ; } ;

        std::map<std::string, ILayer*>& GetLayers();
         
        void                 SwitchScene(std::string name);
        void                 RegisterScene(std::string, IScene* scene);
        void                 UnRegisterScene(std::string);
        
        IScene*              CurrentScene() const { return mCurrentScene; } ;
        IScene*              GetScene(std::string name) const { return mScenes.at(name); };
        
        void                 PushEvent(IEvent_ptr e);
        
        void                 Render(uint64_t time);
        void                 FireUpdates(uint64_t time);
        void                 Process();
        void                 DispatchEvents();
        void                 UpdateLayer();
        
        void                 SetResourceFolder(std::string folder) { mResourceFolder = folder; };
        std::string const &  GetResourceFolder() const { return mResourceFolder; } ;
        void                 SetDocumentsFolder(std::string folder) { mDocumentsFolder = folder; };
        std::string const &  GetDocumentsFolder() const { return mDocumentsFolder; };
        void                 SetScreenSize(int w,int h, int dpi) ;
        void                 GetScreenSize (int& w, int & h, int & dpi) { w = mScreenW; h = mScreenH; dpi = mScreenDPI; };
        
        TextureAtlas       & LoadAtlas ( std::string filename, std::string referencefile, short flags = 0);
        
        void                 Setup(IRender* renderer, std::string resourceFolder, std::string configFile);
        void                 ShutDown();
        void                 Background();
        void                 Foreground();
        
    };    
};


#endif
