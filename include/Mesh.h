#ifndef MESH_H
#define MESH_H

#include <openvdb/openvdb.h>
#include <openvdb/tools/MeshToVolume.h>
#include <vector>
#include <fstream>
#include <sstream>

namespace lux
{

    class Mesh
    {
        public:
            Mesh(){};
            ~Mesh() = default;

            void loadObj(const char* filename);

            openvdb::FloatGrid::Ptr createLevelSet(const openvdb::math::Transform &xform, 
                                                   float halfwidth = float(openvdb::LEVEL_SET_HALF_WIDTH)) const;

        private:
            std::vector<openvdb::Vec3s> _vertices;
            std::vector<openvdb::Vec3I> _faces;

    };
    
    openvdb::FloatGrid::Ptr createLevelSet(const Mesh& mesh, 
                                           const openvdb::math::Transform &xform, 
                                           float halfwidth = float(openvdb::LEVEL_SET_HALF_WIDTH));    

} // namespace lux


#endif