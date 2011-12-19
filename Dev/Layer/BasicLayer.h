#ifndef Helios_Dev_Layer_BasicLayer_h
#define Helios_Dev_Layer_BasicLayer_h
#include <Helios.h>

namespace helios_dev
{
    class BasicLayer : public helios::BaseLayer
    {
    public:
        
        BasicLayer(helios::IScene& owner);
        ~BasicLayer();
        
        void onActive(uint64_t t);
        void onDeactive();
        void Render(uint64_t t);
    };
};

#endif