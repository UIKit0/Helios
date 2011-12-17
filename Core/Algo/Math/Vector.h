
#ifndef Helios_Vector_h
#define Helios_Vector_h

namespace helios
{
    namespace math
    {
        struct Vector
        {
            union {
                struct {
                    float x, y, z, w;
                };
                float d[4];
            };
            
            
        };
    }
}

#endif
