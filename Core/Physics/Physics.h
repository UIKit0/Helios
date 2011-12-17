

#ifndef Helios_Physics_h
#define Helios_Physics_h
#include <glm/glm.hpp>
#include <vector>
#include <Physics/Body.h>

namespace helios
{
    namespace physics
    {

        enum MODE
        {
            E_MODE_AABB,
        };
        class Physics
        {
        
        private:
            
                       
            uint64_t          mCurrentTime;
            std::vector<Body> mBodies;
            std::vector<Vertex>  mGeometry[E_LOD_COUNT];
            std::vector<BODY_ID> mSimulateBodies;
           
            float             mTotalTime;
            
            MODE              mCollisionMode;
            
            uint8_t mPassCount;
            
            
            
        public:
            
            Physics( uint8_t passCount = 1);
            ~Physics();
            
            Body const & GetBody(BODY_ID body) { return mBodies[body]; };
            
            void SubmitForSimulation(BODY_ID body);
            
            void Simulate(LOD max_lod, uint64_t currentTime);
            
            std::vector<Vertex>& GetGeometryLOD(LOD l);
            
            BODY_ID CreateBody( Body& body );
            
            void SubmitGeometryLOD(BODY_ID body, std::vector<Vertex>& geometry, LOD lod);
            
            void SetBodyActive( bool active, BODY_ID body );
        };
    };
};


#endif
