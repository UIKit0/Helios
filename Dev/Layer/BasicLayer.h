#ifndef Helios_Dev_Layer_BasicLayer_h
#define Helios_Dev_Layer_BasicLayer_h
#include <Helios.h>

namespace helios_dev
{
    class BasicLayer : public helios::BaseLayer
    {
        
        std::map<std::string, std::shared_ptr<helios::Texture> > mTextures;
        
    public:
        
        BasicLayer(helios::IScene& owner);
        ~BasicLayer();
        
        void onActive(uint64_t t);
        void onDeactive();
        void SetViewport( int x, int y, int w, int h, int dpi = 72 ) ;
        void Render(uint64_t t);
    };
};

#endif