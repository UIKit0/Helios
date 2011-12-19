#include "BasicLayer.h"

namespace helios_dev
{
    BasicLayer::BasicLayer(helios::IScene& owner) : helios::BaseLayer(owner) {} ;
    BasicLayer::~BasicLayer() {};
    
    void
    BasicLayer::onActive(uint64_t t)
    {
        BaseLayer::onActive(t);
        
    }
    
    void
    BasicLayer::onDeactive()
    {
        
    }
    
    void
    BasicLayer::Render(uint64_t t)
    {
        glClearColor(0.2,0.3,0.5,0.);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}