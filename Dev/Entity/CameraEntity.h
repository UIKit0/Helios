#ifndef Helios_Dev_Entity_CameraEntity_h
#define Helios_Dev_Entity_CameraEntity_h
#include <Helios.h>
namespace helios_dev
{
    class CameraEntity : public helios::BaseEntity
        {
        public:

            CameraEntity(helios::ILayer* owner, glm::vec3 pos,glm::vec3 anchor, glm::vec3 direction, glm::vec3 up) ;
            ~CameraEntity() {};

        };
}
#endif