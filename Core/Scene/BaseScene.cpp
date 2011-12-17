//
//  BaseScene.cpp
//  Helios
//
//  Created by James Hurley on 9/23/11.
//

#include <iostream>
#include <Scene/BaseScene.h>

namespace helios
{
    size_t 
    BaseScene::RegisterLayer(std::string name,ILayer* layer) 
    {
        
          if(!mLayers[name])
             mLayers[name] = layer;
        
        mLayers[name]->SetRenderer(mRenderer);
        
        return 0L;
    };
}
