

#include <iostream>
#include <Physics/Physics.h>

namespace helios
{
    namespace physics
    {
        
        Physics::Physics( uint8_t passCount )
        {
            mPassCount = passCount;
        }
        Physics::~Physics()
        {
            for ( int i = 0 ; i < E_LOD_COUNT ; ++i ) {
                mGeometry[i].clear();
            }
            mBodies.clear();
        }
        
        void 
        Physics::SubmitForSimulation(BODY_ID body)
        {
            if(mBodies[body].IsActive()) {
                mSimulateBodies.push_back(body);
                std::sort(mSimulateBodies.begin(), mSimulateBodies.end());
            }
            
        }
        
        void 
        Physics::Simulate(LOD max_lod, uint64_t currentTime)
        {
            uint64_t delta = currentTime - mCurrentTime;
            
            float td = float(delta) / 1.0e6;
            
            for ( int i = 0 ; i < mPassCount ; ++i )
            {
                for (auto it = mSimulateBodies.begin() ; it != mSimulateBodies.end() ; ++it)
                {
                    Body& b = mBodies[(*it)];
                    
                }
            }
            
            mSimulateBodies.clear();
            mCurrentTime = currentTime;
            mTotalTime += td;
        }
        
        std::vector<Vertex>& 
        Physics::GetGeometryLOD(LOD l)
        {
            return mGeometry[l];
        }
        
        BODY_ID 
        Physics::CreateBody( Body& body )
        {
            BODY_ID i = mBodies.size();
            mBodies.push_back(body);
            
            return i;
        }
        
        void 
        Physics::SubmitGeometryLOD(BODY_ID body, std::vector<Vertex>& geometry, LOD lod)
        {
            Body& b = mBodies[body];
            
        }
        void 
        Physics::SetBodyActive( bool active, BODY_ID body )
        {
            mBodies[body].SetActive(active);
        }

    }
}
