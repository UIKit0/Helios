#ifndef Helios_Dev_Entity_BasicEntity_h
#define Helios_Dev_Entity_BasicEntity_h
#include <Helios.h>

namespace helios_dev
{
    class BasicEntity : public helios::BaseEntity
    {
    public:
        
        BasicEntity(helios::ILayer* owner, glm::vec3 pos, int vao, int vbo, int ibo, int mvLoc, int pLoc, int normLoc, int shader) ;
        ~BasicEntity();
    };
    
};

#endif