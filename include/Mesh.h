#ifndef MESH_H
#define MESH_H

#include <openvdb/openvdb.h>
#include <openvdb/tools/MeshToVolume.h>
#include <vector>
#include <fstream>
#include <sstream>

#include "Vector.h"

namespace lux
{

    // Mesh
    // Holds triangle vertices and face data with some helpful
    // functions to load objs and create level sets
    class Mesh
    {
        public:
            Mesh(){};
            ~Mesh() = default;

            // Load vertices and faces from object file
            void loadObj(const char* filename);

            // Create a levelset with specified voxel size and 
            // half width from stored vertices and faces
            openvdb::FloatGrid::Ptr createLevelSet(float vx_size, 
                                                   float halfwidth = float(openvdb::LEVEL_SET_HALF_WIDTH)) const;

        private:
            std::vector<openvdb::Vec3s> _vertices;
            std::vector<openvdb::Vec3I> _faces;

    };

    //-----------------------------------------------------------------------------

    // Defining MeshSP as a shared pointer of Mesh
    using MeshSP = std::shared_ptr<Mesh>;

    //-----------------------------------------------------------------------------
    
    // Helper Functions
    // Functions to create meshes and level sets from meshes
    
    // Create a mesh
    MeshSP mesh();
    
    // Create a level set from a given mesh
    openvdb::FloatGrid::Ptr createLevelSet(const std::shared_ptr<Mesh>& mesh, 
                                           float vx_size, 
                                           float halfwidth = float(openvdb::LEVEL_SET_HALF_WIDTH));    
    
    //-----------------------------------------------------------------------------

} // namespace lux


#endif