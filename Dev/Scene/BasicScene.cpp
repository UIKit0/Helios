#include "BasicScene.h"
#include "../Layer/BasicLayer.h"

namespace helios_dev
{
    BasicScene::BasicScene() {}
    BasicScene::~BasicScene() {}
    
    void
    BasicScene::onActive(uint64_t t)
    {
        RegisterLayer("BasicLayer", new BasicLayer((*this)));
        
        mLayers["BasicLayer"]->SetViewport(0,0,mScreenW,mScreenH);
        
        BaseScene::onActive(t);
    }
    
    void 
    BasicScene::onDeactive()
    {
        BaseScene::onDeactive();
    }
}