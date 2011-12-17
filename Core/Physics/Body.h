

#ifndef Helios_Physics_Body_h
#define Helios_Physics_Body_h
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Physics/Vertex.h>

namespace helios
{
    namespace physics
    {
        typedef size_t BODY_ID;
        enum LOD
        {
            E_LOD_BROADPHASE,
            E_LOD_MEDIUMPHASE,
            E_LOD_NARROWPHASE,
            E_LOD_COUNT
        };
        
        struct Derivative
        {
            glm::quat spin;
            glm::vec3 velocity;
            glm::vec3 force;
            glm::vec3 torque;
                       
        };
        struct State
        {
            glm::mat4 bodyToWorld;
            glm::mat4 worldToBody;
            
           
            
            glm::quat orientation;  // orientation
            glm::quat spin;         // spin
            
            glm::vec3 pos;          // pos.
            glm::vec3 vLinear;      // linear velocity
            glm::vec3 vAngular;     // angular velocity
            glm::vec3 mLinear;      // linear momentum
            glm::vec3 mAngular;     // angular momentum
            
            
            float mass;
            float invMass;
            float inertiaTensor;
            float invInertiaTensor;
                     
            void recalculate()
            {
                vLinear = mLinear * invMass;
                vAngular = mAngular * invInertiaTensor;
                glm::normalize(orientation);
                spin = 0.5 * glm::quat(vAngular.x,vAngular.y,vAngular.z, 0.f) * orientation;
                glm::mat4 mat = glm::translate(glm::mat4(1.f),pos);
                bodyToWorld = mat * glm::mat4_cast(orientation);
                worldToBody = glm::inverse(bodyToWorld);
            };
            
            static void forces(State & a, float t, glm::vec3 & force, glm::vec3 & torque)
            {
                // TODO: Add dynamic forces.
                
               // const float k = 10.;    // These should not be constant, but dependent on simulation needs
               // const float b = 1.;
                
               // return -k * p - b * v;
                force = 0.f * a.pos;
                torque = a.vAngular;
            };
            static Derivative evaluate(State& a, float t)
            {
                Derivative output;
                output.velocity = a.vLinear;
                output.spin = a.spin;
                
                forces(a,t,output.force,output.torque);
                
                return output;
            };
            static Derivative evaluate (State& a, const Derivative & d, float t, float dt )
            {
                a.pos += d.velocity * dt;
                a.mLinear += d.force * dt;
                a.orientation = a.orientation + (d.spin * dt);
                a.mAngular += d.torque * dt;
                a.recalculate();
                
                Derivative output;
                
                output.velocity = a.vLinear;
                output.spin = a.spin;
                
                forces(a,t,output.force,output.torque);
                
                return output;
                
            };
            static void integrate(State& s,float t, float dt)
            {
                Derivative a = evaluate(s,t);
                Derivative b = evaluate(s,a, t, dt*0.5f);
                Derivative c = evaluate(s,b, t, dt*0.5f);
                Derivative d = evaluate(s,c, t, dt);
                /*glm::vec3 dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
                glm::vec3 dvdt = 1.0f/6.0f * (a.dv + 2.0f*(b.dv + c.dv) + d.dv);
                
                p = p + dxdt * dt;
                v = v + dvdt * dt;
                */
                const float f = 1.f/6.f;
                s.pos += f * dt * (a.velocity + 2.f *(b.velocity + c.velocity) + d.velocity);
                s.mLinear += f * dt * (a.force + 2.f*(b.force + c.force) + d.force);
                s.orientation = s.orientation + ( f * dt * (a.spin + 2.f* (b.spin + c.spin) + d.spin) );
                s.mAngular += f * dt * (a.torque + 2.f*(b.torque + c.torque) + d.torque);
                
                s.recalculate();
            };
            
            static State interpolate(State const & a , State const & b , float mix ) 
            {
                State state = b;
                
                state.pos = glm::mix(a.pos, b.pos, mix);
                state.mLinear = glm::mix(a.mLinear,b.mLinear,mix);
                state.orientation = glm::mix(a.orientation,b.orientation,mix);
                state.mAngular = glm::mix(a.mAngular, b.mAngular, mix);
                
                state.recalculate();
                
                return state;
            }
        };
        

        class Body
        {
        private:
            
            State                mState;
            
            
            
            glm::vec3            mCenterLOD[E_LOD_COUNT];
            
            struct 
            {
                size_t start, end;
            }                    mGeometryRange[E_LOD_COUNT];
            
            float                mRadiusLOD[E_LOD_COUNT];
            
            std::vector<BODY_ID> mPotentialColliders;
            
            int                  mBodyId;
            
            
            bool                 mIsActive = true;
            
        public:
            Body(State& state);
            void SetActive(bool active) { mIsActive = active; } ;
            bool IsActive() { return mIsActive; } ;
            State& State() { return mState; } ; 
        };
    };
};

#endif
