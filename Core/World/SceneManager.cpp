//
//  SceneManager.cpp
//  Helios
//
//  Created by James Hurley on 9/15/11.
//

#include <iostream>
#include <World/SceneManager.h>
#include <Scene/IScene.h>
#include <Render/IRender.h>
#ifdef __BLOCKS__
#include <dispatch/dispatch.h>
#endif

namespace helios
{

    void 
    SceneManager::SetScreenSize(int w, int h, int dpi)
    {
        mScreenW = w; mScreenH =h ; mScreenDPI = dpi; 
        mRender->Prepare(w,h);
        
    }
    void
    SceneManager::Setup(IRender* render,  std::string resourceFolder, std::string configFile)
    {
        mRender = render;
        mResourceFolder = resourceFolder;
        mConfiguration = new Configuration(resourceFolder + "/" + configFile);
    }
    void
    SceneManager::ShutDown()
    {
        for ( auto it = mScenes.begin() ; it != mScenes.end() ; ++ it ) 
        {
            delete (*it).second;
        }
        delete mConfiguration;
    }
    void
    SceneManager::RegisterScene(std::string name, helios::IScene *scene)
    {
        if(!mScenes[name])
            mScenes[name] = scene;
        mScenes[name]->SetRenderer(mRender);
        mScenes[name]->SetViewport(0,0,mScreenW,mScreenH, mScreenDPI);
    }
    void
    SceneManager::UnRegisterScene(std::string name)
    {
        if(mScenes[name]){
            delete mScenes[name];
            mScenes.erase(mScenes.find(name));
        }
        
    }
    void
    SceneManager::PushEvent(IEvent_ptr e)
    {
        mCurrentScene->PushEvent(e);
    }
    void
    SceneManager::SwitchScene(std::string name)
    {
        
        if(mCurrentScene)
            mCurrentScene->onDeactive();
        
        if(mScenes[name]) 
        {
            mCurrentScene = mScenes[name];
            mCurrentScene->onActive(mCurrentTimestamp);
        }
        
    }
    TextureAtlas & 
    SceneManager::LoadAtlas ( std::string filename, std::string referencefile, short flags  ) 
    {
#ifdef __BLOCKS__
        
        if( dispatch_get_current_queue() != dispatch_get_main_queue() ) {
            //  throw "LoadAtlas must run on main thread.";
            D_PRINT("This routine may crash if run on a thread other than main and it needs to create a new texture. Be aware.");
        }
#endif
        if( mAtlasDict.find(filename) == mAtlasDict.end()) {
            mAtlasDict[filename] = new TextureAtlas( filename, referencefile, flags ) ;
        } 
        
        return *mAtlasDict[filename];
    }
    
    std::map<std::string, ILayer*>& 
    SceneManager::GetLayers() 
    { 
        return mCurrentScene->GetLayers(); 
    }
    void                 
    SceneManager::Render(uint64_t time)
    {
        mCurrentScene->Render(time);
    }
    void                 
    SceneManager::FireUpdates(uint64_t time)
    {
        mCurrentScene->Update(time);
    }
    void                 
    SceneManager::Process()
    {
        mCurrentScene->Process();
    }
    void                 
    SceneManager::DispatchEvents()
    {
        mCurrentScene->DispatchEvents();
    }
    void
    SceneManager::UpdateLayer()
    {
        mCurrentScene->UpdateScene();
        mCurrentScene->UpdateLayer();
    }
    
}
