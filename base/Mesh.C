#include "Mesh.h"

using namespace lux;

void Mesh::loadObj(const char *filename)
{
    std::string line;
    std::ifstream file(filename);

    if (!file) {
        std::cout << "Could not open file" << std::endl;
    }

    while (std::getline(file, line)) {

        std::stringstream ss(line);
        std::string type;
        
        ss >> type;

        if(type.compare("v") == 0) {

            openvdb::Vec3s inPos = openvdb::Vec3s(0, 0, 0);

            ss >> inPos[0] >> inPos[1] >> inPos[2];

            _vertices.push_back(inPos);
            
        }
        else if(type.compare("f") == 0) {

            openvdb::Vec3I inFace = openvdb::Vec3I(0, 0, 0);

            ss >> inFace[0] >> inFace[1] >> inFace[2];

            inFace -= openvdb::Vec3I(1,1,1);

            _faces.push_back(inFace);

        }

    }

}

openvdb::FloatGrid::Ptr Mesh::createLevelSet(const openvdb::math::Transform &xform, float halfwidth) const
{
    return openvdb::tools::meshToLevelSet<openvdb::FloatGrid>(xform, _vertices, _faces, halfwidth);
}

openvdb::FloatGrid::Ptr lux::createLevelSet(const Mesh &mesh, const openvdb::math::Transform &xform, float halfwidth)
{
    return mesh.createLevelSet(xform, halfwidth);
}