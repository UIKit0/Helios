
#ifndef Helios_GLES2_Lights_h
#define Helios_GLES2_Lights_h
#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>

namespace helios
{
    namespace gles2
    {
        typedef enum{
            directional=0,
            point,
            spotlight
        } LIGHT_TYPE;
        
        struct Light
        {
            glm::vec3 position;
            glm::quat rotation;
            struct {
                bool cast_volume: 1;
                bool cast_map: 1;
                
            
                LIGHT_TYPE lightType: 2; // Directional, point, spotlight
            };
            uint8_t tag;
        };
        
        struct Lights
        {
            std::vector<Light> lights;
        };
        
    }
}


#endif
