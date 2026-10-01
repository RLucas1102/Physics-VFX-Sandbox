#include "VolumeGrid.h"

using namespace lux;

// ------------
// Volume Grid
// ------------
template <typename GridType>
void lux::VolumeGrid<GridType>::init(const openvdb::Coord &llc, 
                                     const openvdb::Coord &urc, 
                                     float vx_size, 
                                     const GridV &dg)
{
    _grid = GridType::create(dg);

    _grid->setTransform(openvdb::math::Transform::createLinearTransform(vx_size));

    _grid->setGridClass(openvdb::GRID_FOG_VOLUME);

    _bbox = std::make_shared<openvdb::CoordBBox>(llc, urc);
}

//-----------------------------------------------------------------------------

// -----------------
// Helper Functions
// -----------------
template <typename GridType>
VGSP<GridType> lux::grid()
{
    return std::make_shared<VolumeGrid<GridType>>();
}

//-----------------------------------------------------------------------------

// ------------------------
// Explicit instantiations
// ------------------------

// Volume Grid
template class VolumeGrid<openvdb::FloatGrid>;
template class VolumeGrid<openvdb::Vec3SGrid>;

// Volume Grid Creation Helper
template VGSP<openvdb::FloatGrid> lux::grid();
template VGSP<openvdb::Vec3SGrid> lux::grid();