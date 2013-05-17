#include <map>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include "LoadMS3D.h"

int convert_mesh(std::string& filename, std::string& outfile);

struct Header
{
    unsigned short fileformat;
    unsigned short version;
} ;

static const Header _header = { 0x4E05, 0x0001 } ;

int main(int argc, char * const argv[])
{
    int c;
    std::string filename, out_file;
    bool is_mesh = false; // only option for now.
    bool got_output = false;
    
    while ( (c = getopt(argc,argv, "m:o:")) != -1) {
        switch(c)
        {
            case 'm':
                filename = optarg;
                is_mesh = true;
                break;
            case 'o':
                out_file = optarg;
                got_output = true;
                break;
            default:
                break;
        }
    }

    if(is_mesh && got_output)
    {
        return convert_mesh(filename, out_file);
    }
    else
    {
        std::cout << "Usage: hconvert -m [mesh.ms3d] -o [mesh.h3dmesh]" << std::endl;
    }
    return 0;
}
int convert_mesh(std::string& filename, std::string& outfile)
{
    int retval = 0;
    uint32_t size = 0;
    
    std::vector<helios::Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<helios::s::Material> mats;
    std::vector<helios::s::Joint> joints;
    
    // For now we just loadms3d
    int ret = LMS3D::LoadMS3D(filename, vertices, indices, mats, joints);

    if(ret < 0) {
        std::cerr << "Unable to open " << filename << " for input." << std::endl;
        return ret;
    }
    FILE * of = fopen(outfile.c_str(), "wb");
    if(!of)
    {
        std::cerr << "Unable to open " << outfile << " for writing.";
        return -1;
    }

    fwrite(&_header, sizeof(_header), 1, of);

    {
        std::cout << "Outputting " << vertices.size() << " vertices" << std::endl;
        // Vertices
        size = static_cast<uint32_t>(vertices.size() * sizeof(helios::Vertex));
        fwrite(&size, sizeof(size), 1, of);
        fwrite(&vertices[0], sizeof(helios::Vertex), vertices.size(), of);

    }
    {
        std::cout << "Outputting " << indices.size() << " indices" << std::endl;
        // Indices
        size = static_cast<uint32_t>(indices.size() * sizeof(uint32_t));
        fwrite(&size, sizeof(size),1,of);
        fwrite(&indices[0], sizeof(uint32_t), indices.size(), of);

    }
    {
        std::cout << "Outputting " << joints.size() << " joints" << std::endl;
        // Joints
        size = static_cast<uint32_t>(joints.size());
        fwrite(&size, sizeof(size),1,of);
        
        for ( size_t i = 0 ; i < joints.size() ; ++ i)
        {
            helios::s::JointBase *jb = &joints[i];
            
            std::cout << "[ " << jb->jointid << "] Outputting " << joints[i].keyframes.size() << " frames" << std::endl;
            joints[i].frameCount = static_cast<uint32_t>(joints[i].keyframes.size());
            fwrite(jb, sizeof(helios::s::JointBase), 1, of);
            fwrite(&joints[i].keyframes[0], sizeof(helios::s::Frame), joints[i].keyframes.size(), of);
        }

    }
    {
        // Materials
        std::cout << "Outputting " << mats.size() << " materials" << std::endl;
        size = static_cast<uint32_t>(mats.size() * sizeof(helios::s::Material));
        fwrite(&size, sizeof(size), 1, of);
        fwrite(&mats[0], mats.size(), sizeof(helios::s::Material), of);
    }

    fclose(of);
    return retval;
}
