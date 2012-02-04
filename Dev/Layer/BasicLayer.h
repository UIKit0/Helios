#ifndef Helios_Dev_Layer_BasicLayer_h
#define Helios_Dev_Layer_BasicLayer_h
#include <Helios.h>

namespace helios_dev
{
    class BasicLayer : public helios::BaseLayer
    {
        
    private:
        void LoadMS3D(std::string &file,
                             std::vector<helios::Vertex>& vertices, 
                             std::vector<unsigned short>& indices, 
                             std::vector<helios::s::MaterialGroup>& materials,
                             std::vector<helios::s::Joint>& joints);
        
        void quaternionFromEuler(glm::quat& q, float roll, float pitch, float yaw);
        
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