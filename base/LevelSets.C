#include "LevelSets.h"

void lux::bunny(VGSP<openvdb::FloatGrid>& objGrid, float vx_size) {
    
    // Define mesh objects
    MeshSP objMesh = mesh();
    objMesh->loadObj("models/bunnyFixed/bunny_fixed.obj");
    centerMesh(objMesh);

    // Define grids to store volumes and colors
    std::cout << "Creating Grids. . ." << std::endl;

    float objVxSize = vx_size;
    objGrid = grid<openvdb::FloatGrid>();
    objGrid->initLevelSet(createLevelSet(objMesh, objVxSize));

}