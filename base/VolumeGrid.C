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

template <typename GridT>
void VolumeGrid<GridT>::initLevelSet(const GridT &grid)
{
    _grid = grid->deepCopy();

    _grid->setGridClass(openvdb::GRID_LEVEL_SET);

    _bbox = std::make_shared<openvdb::CoordBBox>(grid->evalActiveVoxelBoundingBox());
}

template<typename GridType>
typename VolumeGrid<GridType>::GridV VolumeGrid<GridType>::triLerp(const Vector &P)
{
    const openvdb::Vec3d xyz(P.X(), P.Y(), P.Z());

    openvdb::Vec3d index = _grid->worldToIndex(xyz);
    
    GridV v = openvdb::tools::BoxSampler::sample(_grid->tree(), index);

    return v;
}

template<typename GridType>
void VolumeGrid<GridType>::stamp(const VSP<GridV> &f)
{
    typename GridType::Accessor accessor = _grid->getAccessor();

    for ( auto iter = _bbox->beginXYZ(); iter != _bbox->endXYZ(); ++iter)
    {
        openvdb::Vec3d world = _grid->indexToWorld(*iter);

        Vector p(world.x(), world.y(), world.z());

        GridV val = f->eval(p);

        accessor.setValue(*iter, val);
    }

}

//-----------------------------------------------------------------------------

// -----------
// Grid Field
// -----------
template <typename GridT, typename GridV>
GridField<GridT, GridV>::GridField(const VGSP<GridT> &g) :
    _g(g)
{
}

template <typename GridT, typename GridV>
const typename Volume<GridV>::volumeDataType lux::GridField<GridT, GridV>::eval(const Vector &p) const
{
    return _g->triLerp(p);;
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

template <typename GridT, typename GridV>
VSP<GridV> lux::gridField(const VGSP<GridT>& g)
{
    return std::make_shared<GridField<GridT, GridV>>(g);
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

// Grid Field
template class GridField<openvdb::FloatGrid, float>;
template class GridField<openvdb::Vec3SGrid, openvdb::Vec3s>;

// Grid Field Creation Helper
template VSP<float> lux::gridField(const VGSP<openvdb::FloatGrid>& g);
template VSP<openvdb::Vec3s> lux::gridField(const VGSP<openvdb::Vec3SGrid>& g);