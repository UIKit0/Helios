#ifndef Helios_Dev_Scene_BasicScene_h
#define Helios_Dev_Scene_BasicScene_h
#include <Helios.h>

namespace helios_dev
{
    class BasicScene : public helios::BaseScene
    {
    public:
        BasicScene();
        ~BasicScene();
        
        void onActive(uint64_t t);
        void onDeactive();
    };
};

#endif