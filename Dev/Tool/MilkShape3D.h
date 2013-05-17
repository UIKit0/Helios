
#ifndef Helios_Dev_Tool_MilkShape3D_h
#define Helios_Dev_Tool_MilkShape3D_h

#define MAX_VERTICES    0xFFFE
#define MAX_TRIANGLES   0xFFFE
#define MAX_GROUPS      0xFF
#define MAX_MATERIALS   0x80
#define MAX_JOINTS      0x80

#include <stdio.h>
#include <vector>
#include <map>

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>


#ifndef byte
    typedef uint8_t byte;
#endif // byte
    
#ifndef word
    typedef uint16_t word;
#endif // word
    
    
    /**** FLAGS   ****/
    
    typedef enum {
        F_SELECTED  = 0x01,
        F_HIDDEN    = 0x02,
        F_SELECTED2 = 0x04,
        F_DIRTY     = 0x08
    } E_FLAGS;

    /**** STRUCTS ****/
#pragma pack(push,1)
    typedef struct {
        byte flags;
        float vertex[3];
        char boneId;
        byte referenceCount;
        
    } ms3d_vertex_t;
    
    
    typedef struct
    {
        word    flags;                                      // SELECTED | SELECTED2 | HIDDEN
        word    vertexIndices[3];                           //
        float   vertexNormals[3][3];                        //
        float   s[3];                                       //
        float   t[3];                                       //
        byte    smoothingGroup;                             // 1 - 32
        byte    groupIndex;                                 //
    } ms3d_triangle_t;
    

    typedef struct
    {
        char            name[32];                           //
        float           ambient[4];                         //
        float           diffuse[4];                         //
        float           specular[4];                        //
        float           emissive[4];                        //
        float           shininess;                          // 0.0f - 128.0f
        float           transparency;                       // 0.0f - 1.0f
        char            mode;                               // 0, 1, 2 is unused now
        char            texture[128];                        // texture.bmp
        char            alphamap[128];                       // alpha.bmp
    } ms3d_material_t;
    
    
    typedef struct // 16 bytes
    {
        float           time;                               // time in seconds
        float           rotation[3];                        // x, y, z angles
    } ms3d_keyframe_rot_t;
    
    typedef struct // 16 bytes
    {
        float           time;                               // time in seconds
        float           position[3];                        // local position
    } ms3d_keyframe_pos_t;
    
      
    
    typedef struct
    {
        char boneIds[3];									// index of joint or -1, if -1, then that weight is ignored, since subVersion 1
        byte weights[3];									// vertex weight ranging from 0 - 100, last weight is computed by 1.0 - sum(all weights), since subVersion 1
        // weight[0] is the weight for boneId in ms3d_vertex_t
        // weight[1] is the weight for boneIds[0]
        // weight[2] is the weight for boneIds[1]
        // 1.0f - weight[0] - weight[1] - weight[2] is the weight for boneIds[2]
        unsigned int extra;									// vertex extra, which can be used as color or anything else, since subVersion 2
    } ms3d_vertex_ex_t;
    
    typedef struct
    {
        float color[3];	// joint color, since subVersion == 1
    } ms3d_joint_ex_t;
    typedef struct
    {
        float jointSize;	// joint size, since subVersion == 1
        int transparencyMode; // 0 = simple, 1 = depth buffered with alpha ref, 2 = depth sorted triangles, since subVersion == 1
        float alphaRef; // alpha reference value for transparencyMode = 1, since subVersion == 1
    } ms3d_model_ex_t;
    // -------------------------------------------------------------------------
      
    typedef struct
    {
        int index;											// index of group, material or joint
        int commentLength;									// length of comment (terminating '\0' is not saved), "MC" has comment length of 2 (not 3)
        std::vector<char> comment;	  					// comment
    } ms3d_comment_t;
    typedef struct
    {
        
        byte            flags;                              // SELECTED | DIRTY
        char            name[32];                           //
        char            parentName[32];                     //
        float           rotation[3];                        // local reference matrix
        float           position[3];
        
        word            numKeyFramesRot;                    //
        word            numKeyFramesTrans;                  //
    } ms3d_joint_t;
#pragma pack(pop)
    
    typedef struct
    {
        std::vector<glm::mat4>  keyFramesMats;
        std::vector<glm::quat>  keyFramesQuats;
        std::vector<ms3d_keyframe_rot_t> keyFramesRot;      // local animation matrices
        std::vector<ms3d_keyframe_pos_t> keyFramesTrans;    // local animation matrices

    } ms3d_keyframe_data_t;

    
    
    typedef struct
    {
        byte            flags;                              // SELECTED | HIDDEN
        char            name[32];                           //
        word            numtriangles;                       //
        std::vector<word> triangleIndices;
        //word            triangleIndices[0xFFFE];            // the groups group the triangles
        char            materialIndex;                      // -1 = no material
    } ms3d_group_t;

    
    
    class MilkShape3D
    {
    public:
        MilkShape3D(){};
        ~MilkShape3D(){};
        word nNumVertices;
        std::vector<ms3d_vertex_t> vertices;
        word nNumTriangles;
        std::vector<ms3d_triangle_t> triangles;
        word nNumGroups;
        std::vector<ms3d_group_t> groups;
        word nNumMaterials;
        std::vector<ms3d_material_t> materials;
        float    fAnimationFPS; // 4 bytes
        float    fCurrentTime; // 4 bytes
        int32_t  iTotalFrames; // 4 bytes
        word     nNumJoints; // 2 bytes
        std::vector<ms3d_keyframe_data_t> keyframes;
        std::vector<ms3d_joint_t> joints;
        int32_t  subVersion; // subVersion is = 1, 4 bytes
        uint32_t nNumGroupComments; // 4 bytes
        std::vector<ms3d_comment_t> groupComments;
        int32_t  nNumMaterialsComments;
        std::vector<ms3d_comment_t> materialsComments;
        int32_t  nNumJointComments;
        std::vector<ms3d_comment_t> jointComments;
        int32_t  nHasModelComment;
        std::vector<ms3d_comment_t> modelComment;
        std::vector<ms3d_vertex_ex_t> vertexExtra;
        std::vector<ms3d_joint_ex_t> jointExtra;
        ms3d_model_ex_t modelExtra;
        std::map<std::string, int> jointMap;
        
        static void LoadComment(FILE* fp, std::vector<ms3d_comment_t>* comments, int32_t numComments);
   
        struct Vertex
        {
            
        };
        struct BoneData
        {
            
        };
        int LoadMS3D(std::string& filename);
    };


#endif
