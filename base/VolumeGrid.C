
#include "VolumeGrid.h"

using namespace lux;


// ------------
// Volume Grid
// ------------
template<typename GridType>
void VolumeGrid<GridType>::init(const openvdb::Coord &llc, 
                                const openvdb::Coord &urc, 
                                float vx_size, 
                                const GridValue &dg)
{
    _grid = GridType::create(dg);

    _grid->setTransform(openvdb::math::Transform::createLinearTransform(vx_size));

    _grid->setGridClass(openvdb::GRID_FOG_VOLUME);

    _bbox = std::make_shared<openvdb::CoordBBox>(llc, urc);
}

template <typename GridType>
void VolumeGrid<GridType>::init(const openvdb::CoordBBox &bbox, 
                                float vx_size,
                                const typename GridTypes<T>::GridValue& dg)
{
    _grid = GridType::create(dg);

    _grid->setTransform(openvdb::math::Transform::createLinearTransform(vx_size));

    _grid->setGridClass(openvdb::GRID_FOG_VOLUME);

    _bbox = std::make_shared<openvdb::CoordBBox>(bbox.getStart(), bbox.getEnd());

}

template<typename GridType>
void VolumeGrid<GridType>::init(const GridPtr& grid)
{
    _grid = GridType::deepCopyGrid(grid);
    
    _grid->setGridClass(openvdb::GRID_LEVEL_SET);

    _bbox = std::make_shared<openvdb::CoordBBox>(grid->evalActiveVoxelBoundingBox());
}

template<typename GridType>
GridValue VolumeGrid<GridType>::triLerp(const Vector &P)
{
    const openvdb::Vec3d xyz(P.X(), P.Y(), P.Z());

    openvdb::Vec3d index = _grid->worldToIndex(xyz);
    
    GridValue v = openvdb::tools::BoxSampler::sample(_grid->tree(), index);

    return v;
}

template<typename GridType>
void VolumeGrid<GridType>::stamp(const VSP<T> &f)
{
    typename GridType::Accessor accessor = _grid->getAccessor();

    for ( auto iter = _bbox->beginXYZ(); iter != _bbox->endXYZ(); ++iter)
    {
        openvdb::Vec3d world = _grid->indexToWorld(*iter);

        Vector p(world.x(), world.y(), world.z());

        auto val = f->eval(p);

        accessor.setValue(*iter, val);
    }

}

//-----------------------------------------------------------------------------


// Explicit instantiations

// VolumeGrid
template class VolumeGrid<openvdb::FloatGrid>;
template class VolumeGrid<openvdb::Vec3SGrid>;

// ----------------------------------------------------------------------------

/***************************************************
 * Notes:
 * 
 * [0] Making functions in struct static to call without
 *     instantiating an object of struct type
 * 
 * [1] For GridField, I was struggling with compiling the eval.
 *     Even though the types, volumeDataType and GridValue,
 *     evaluate to a float or color depending on the input type,
 *     my child eval must return the same type as parent.
 *     (volumeDataType != GridValue)
 * 
 * [2] using GridType = typename GridTypes<T>::GridType;
 *     using GridValue = typename GridTypes<T>::GridValue;
 *     Have to define the types like this and cannot
 *     just use the GridType directly
 *      
 *
 * 
 ***************************************************/