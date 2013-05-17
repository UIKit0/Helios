
#include <iostream>
#include "MilkShape3D.h"


    void
    MilkShape3D::LoadComment(FILE* fp, std::vector<ms3d_comment_t>* comments, int32_t numComments)
    {
        comments->resize(numComments);
        for(int i = 0 ; i < numComments ; i ++)
        {
            ms3d_comment_t comment;
            comment.index=0;
            comment.commentLength=0;
            fread(&comment.index,sizeof(int),1,fp);
            fread(&comment.commentLength,sizeof(int),1,fp);
            comment.comment.resize(comment.commentLength);
            fread(&comment.comment[0], sizeof(char),comment.commentLength,fp);
            comments->at(i) = comment;
        }
    }
    int
    MilkShape3D::LoadMS3D(std::string& filename)
    {        
        printf("\nLoadMS3D::  Loading model %s...", filename.c_str()); fflush(stdin);
        
        
        FILE * fp = fopen(filename.c_str(), "rb");
        if(!fp) return -1;
        fseek(fp, 14, SEEK_SET); // Skipping header
        fread(&nNumVertices, sizeof(word), 1, fp);
        vertices.resize(nNumVertices);
        vertexExtra.resize(nNumVertices);
        fread(&vertices[0], sizeof(ms3d_vertex_t),nNumVertices, fp);
        std::vector<char> seenJoints;
     
        
        fread(&nNumTriangles, sizeof(word), 1, fp);
        triangles.resize(nNumTriangles);
        fread(&triangles[0], sizeof(ms3d_triangle_t), nNumTriangles, fp);
        fread(&nNumGroups, sizeof(word), 1, fp);
        groups.resize(nNumGroups);
        for(int i = 0 ; i < nNumGroups ; i++)
        {
            ms3d_group_t group;
            fread(&group.flags, sizeof(byte),1,fp);
            fread(&group.name, sizeof(char), 32, fp);
            fread(&group.numtriangles, sizeof(word),1 ,fp);
            group.triangleIndices.resize(group.numtriangles);
            fread(&group.triangleIndices[0], sizeof(word), group.numtriangles, fp);
            fread(&group.materialIndex, sizeof(char),1,fp);
            groups[i] = group;
        }
        fread(&nNumMaterials, sizeof(word), 1, fp);
        materials.resize(nNumMaterials);
        fread(&materials[0], sizeof(ms3d_material_t), nNumMaterials,fp);
        fread(&fAnimationFPS, sizeof(float), 1, fp);
        printf("\nAnimation FPS: %lf\n", fAnimationFPS);
        fread(&fCurrentTime, sizeof(float), 1, fp);
        fread(&iTotalFrames, sizeof(uint32_t), 1, fp);
        fread(&nNumJoints, sizeof(word),1,fp);
        joints.resize(nNumJoints);
        keyframes.resize(nNumJoints);
        jointExtra.resize(nNumJoints);

        for(int i = 0 ; i < nNumJoints ; i++)
        {
            ms3d_joint_t joint;
            ms3d_keyframe_data_t kd;
            
            fread(&joint,93,1,fp);
            
            jointMap[joint.name] = i;
            std::string parent(joint.parentName);
            
            kd.keyFramesRot.resize(joint.numKeyFramesRot);
            kd.keyFramesTrans.resize(joint.numKeyFramesTrans);
            
            fread(&kd.keyFramesRot[0], sizeof(ms3d_keyframe_rot_t), joint.numKeyFramesRot,fp);
            fread(&kd.keyFramesTrans[0],sizeof(ms3d_keyframe_pos_t),joint.numKeyFramesTrans,fp);
            
            for ( word i = 0 ; i < joint.numKeyFramesRot ; ++i )
            {
                //printf("Rot: (%lf %lf %lf) \n", kd.keyFramesRot[i].rotation[0], kd.keyFramesRot[i].rotation[1], kd.keyFramesRot[i].rotation[2]);

                // MS3D is in Roll-Pitch-Yaw order, glm::quat is looking for Pitch-Yaw-Roll
                glm::quat aa ( glm::vec3(kd.keyFramesRot[i].rotation[1],kd.keyFramesRot[i].rotation[2], kd.keyFramesRot[i].rotation[0]));
                glm::mat4 matrix(1.f);
                
                
                //printf("Quat: (%lf %lf %lf %lf)\n",aa.x,aa.y,aa.z,aa.w);
                if(parent.size() > 0)
                {
                    int p = jointMap[parent];
                    aa = keyframes[p].keyFramesQuats[i] * aa;
                }
                kd.keyFramesQuats.push_back(aa);

            }
            for (word i = 0 ; i < joint.numKeyFramesTrans ; ++i)
            {
                
                if(parent.size() > 0) {
                    kd.keyFramesTrans[i].position[0] += keyframes[jointMap[parent]].keyFramesTrans[i].position[0];
                    kd.keyFramesTrans[i].position[1] += keyframes[jointMap[parent]].keyFramesTrans[i].position[1];
                    kd.keyFramesTrans[i].position[2] += keyframes[jointMap[parent]].keyFramesTrans[i].position[2];
                }
            }
            joints[i] = joint;
            keyframes[i] = kd;
            if(feof(fp)) printf("\nencountered eof on joint %d of %d!\n", i+1, nNumJoints);
        }
        fread(&subVersion, sizeof(int32_t),1,fp);
        if(!feof(fp))
        {
            
            fread(&nNumGroupComments,sizeof(uint32_t),1,fp);
            LoadComment(fp,&groupComments,nNumGroupComments);
            fread(&nNumMaterialsComments,sizeof(uint32_t),1,fp);
            LoadComment(fp,&materialsComments,nNumMaterialsComments);
            fread(&nNumJointComments, sizeof(uint32_t),1,fp);
            LoadComment(fp,&jointComments,nNumJointComments);
            fread(&nHasModelComment, sizeof(uint32_t),1,fp);
            LoadComment(fp,&modelComment, nHasModelComment);
            fread(&subVersion,sizeof(subVersion),1,fp);
            fread(&vertexExtra[0],sizeof(ms3d_vertex_ex_t),nNumVertices,fp);
            fread(&subVersion,sizeof(subVersion),1,fp);
            fread(&jointExtra[0],sizeof(ms3d_joint_ex_t), nNumJoints,fp);
            fread(&subVersion,sizeof(subVersion),1,fp);
            fread(&modelExtra,sizeof(ms3d_model_ex_t),1,fp);
            
        }
        fclose(fp);
        return 0;
    }
    
