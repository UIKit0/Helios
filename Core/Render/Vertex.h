
#ifndef Helios_Vertex_h
#define Helios_Vertex_h
#include <glm/glm.hpp>
#include <map>
namespace helios
{
    struct Vertex
    {
        glm::vec4 p;
        
        unsigned short s, t , boneId, extrudable;
        

        union
        {
            unsigned char c[4];
            struct {
                unsigned char r, g, b, a;  
            };
            uint32_t _c;
        };
        union
        {
            unsigned char n[4];
            struct {
                unsigned char nx, ny, nz, nw;
            };
            uint32_t _n;
        };

        Vertex() : p(0.), s(0), t(0), boneId(0), _c(0), _n(0), extrudable(0) {};
        Vertex(glm::vec4 pos, unsigned short S, unsigned short T) : p(pos), s(S), t(T), boneId(0), _c(0), _n(0), extrudable(0) {};
        
        glm::vec4 GetNormals() 
        {
            glm::vec4 _n = glm::vec4((float)nx / 255.f, (float)ny / 255.f, (float)nz / 255.f, (float)nw / 255.f);
            _n *= 2.f;
            _n -= 1.f;
            
            return _n;
        };
        
        void SetNormals(glm::vec4 norms)
        {
            glm::vec4 _n = glm::normalize(norms);
            
            _n = (_n * 0.5f) + 0.5f;
            nx = _n.x * 0xFF;
            ny = _n.y * 0xFF;
            nz = _n.z * 0xFF;
            nw = _n.w * 0xFF;
        };
        
    };
    struct Edge
    {
        Vertex a, b;
        
        bool operator==(Edge const & in) const
        {
            return ((a.p == in.a.p && b.p == in.b.p) || (a.p == in.b.p && b.p == in.a.p));            
        };
        bool operator<(Edge const & in) const
        {
            return (glm::dot(a.p,b.p)) < (glm::dot(in.a.p, in.b.p));
        }
        
    };
    inline std::vector<Vertex> reorderTriangle(std::vector<Vertex> inPoints, glm::vec3 inSurfaceNormal)
    {
        glm::vec3 normal = glm::cross(glm::vec3(inPoints[0].p), glm::vec3(inPoints[1].p));
        
        if( acosf(glm::dot(normal, inSurfaceNormal)) > M_PI_2 )
        {
            Vertex swap;
            
            swap = inPoints[3];
            inPoints[3] = inPoints[0];
            inPoints[0] = inPoints[3];
        }
        
        return inPoints;
    }
    inline bool edgeSeen(Edge e, std::map<Edge,bool>& seen)
    {
        Edge e2 ;
        e2.a = e.b;
        e2.b = e.a;
        
        return ((seen.find(e) != seen.end()) || (seen.find(e2) != seen.end()));
    }
    
    inline size_t findTriangleForEdge(std::vector<Vertex>& vertices, Edge e, std::map<Edge,bool>& seen, size_t cur_idx, size_t vsize, Edge& outEdge)
    {
        
        //size_t vsize = vertices.size();
        for( size_t i = cur_idx ; i < vsize ; i += 3 )
        {
            Edge a, b, c;
            a.a = vertices[i];
            a.b = vertices[i+1];
            b.a = vertices[i+1];
            b.b = vertices[i+2];
            c.a = vertices[i+2];
            c.b = vertices[i];
            
            if(!edgeSeen(a,seen) && e == a)
            {
                seen[a] = true;
                // if(glm::dot(e.a.n,a.a.n) < 0.95) {
                outEdge = a;
                return i;
                // } 
                // return 0;
                
            }
            else if(!edgeSeen(b,seen) && e == b)
            {
                seen[b] = true;
                
                // if(glm::dot(e.a.n,b.a.n) < 0.95) {
                outEdge = b;
                return i;
                // }
                // return 0;
            }
            else if(!edgeSeen(c,seen) && e == c)
            {
                seen[c] = true;
                // if(glm::dot(e.a.n,c.a.n) < 0.95) {
                outEdge = c;
                return i;
                // }
                // return 0;
            }
            /* if((!edgeSeen(a) && e == a ||!edgeSeen(b) && e == b ||!edgeSeen(c) && e == c))
             {
             return i;
             }  */          
        }
        return 0;
    }
    inline void generateDegenerateQuads(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
    {
        uint32_t vsize = static_cast<uint32_t>(vertices.size());
        uint32_t cur_idx = vsize;
        std::map<Edge,bool> seen;
        
        for ( uint32_t i = 0 ; i < vsize ; i+=3 )
        {
            Edge a,b,c;
            a.a = vertices[i];
            a.b = vertices[i+1];
            b.a = vertices[i+1];
            b.b = vertices[i+2];
            c.a = vertices[i+2];
            c.b = vertices[i];
            size_t n;
            Edge e;
            
            bool ea, eb, ec;
            bool sa, sb, sc;
            
            ea = ((sa = !edgeSeen(a,seen)) && (n = findTriangleForEdge(vertices,a, seen, i+3, vsize, e)) > 0);
            
            eb = ((sb = !edgeSeen(b,seen)) && (n = findTriangleForEdge(vertices,b,seen, i+3, vsize,e)) > 0);
            
            ec = ((sc = !edgeSeen(c,seen)) && (n = findTriangleForEdge(vertices,c,seen, i+3, vsize,e)) > 0);
            
            if((ea || !sa) && (eb || !sb) && (ec || !sc))
            {
                if(ea)
                {
                    // Generate two degenerate triangles
                    Vertex _a, _b, _c, _d;
                    _a = a.a;
                    _b = a.b;
                    _c = e.a;
                    _d = e.b;
                    vertices.push_back(_a);
                    vertices.push_back(_b);
                    vertices.push_back(_c);
                    vertices.push_back(_d);
                    indices.push_back(cur_idx);
                    indices.push_back(cur_idx+1);
                    indices.push_back(cur_idx+2);
                    indices.push_back(cur_idx+1);
                    indices.push_back(cur_idx+3);
                    indices.push_back(cur_idx+2);
                    
                }  
                if(eb)
                {
                    // Generate two degenerate triangles
                    Vertex _a, _b, _c, _d;
                    _a = b.a;
                    _b = b.b;
                    _c = e.a;
                    _d = e.b;
                    vertices.push_back(_a);
                    vertices.push_back(_b);
                    vertices.push_back(_c);
                    vertices.push_back(_d);
                    indices.push_back(cur_idx);
                    indices.push_back(cur_idx+1);
                    indices.push_back(cur_idx+2);
                    indices.push_back(cur_idx+1);
                    indices.push_back(cur_idx+3);
                    indices.push_back(cur_idx+2);
                    
                }
                
                if(ec)
                {
                    // Generate two degenerate triangles
                    Vertex _a, _b, _c, _d;
                    _a = c.a;
                    _b = c.b;
                    _c = e.a;
                    _d = e.b;
                    vertices.push_back(_a);
                    vertices.push_back(_b);
                    vertices.push_back(_c);
                    vertices.push_back(_d);
                    indices.push_back(cur_idx);
                    indices.push_back(cur_idx+1);
                    indices.push_back(cur_idx+2);
                    indices.push_back(cur_idx+1);
                    indices.push_back(cur_idx+3);
                    indices.push_back(cur_idx+2);
                    
                }
            
            } 
            vertices[i].extrudable = 1;
            vertices[i+1].extrudable = 1;
            vertices[i+2].extrudable = 1;

        }
    }
};
#endif
