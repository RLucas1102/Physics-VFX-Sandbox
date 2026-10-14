#ifndef LEVELSETS_H
#define LEVELSETS_H

#include <openvdb/openvdb.h>

#include "VolumeGrid.h"
#include "Mesh.h"
#include "FieldInterface.h"

namespace lux {

    void bunny(VGSP<openvdb::FloatGrid>& objGrid, float vx_size);

    void ajax(VGSP<openvdb::FloatGrid>& objGrid, float vx_size);

    void humanoid(VGSP<openvdb::FloatGrid>& objGrid, VGSP<openvdb::Vec3SGrid>& colorGrid);

}


#endif