
#include "VolumeGrid.h"

using namespace lux;

template<typename T>
void VolumeGrid<T>::init(const openvdb::Coord &llc, 
                         const openvdb::Coord &urc, 
                         const float &vx_size, 
                         const typename GridTypes<T>::GridValue &dg)
{
    openvdb::initialize();

    _grid = GridTypes<T>::create(dg);

    _grid->setTransform(openvdb::math::Transform::createLinearTransform(vx_size));

    _grid->setGridClass(openvdb::GRID_FOG_VOLUME);

    _bbox = std::make_shared<openvdb::CoordBBox>(llc, urc);
}

template<typename T>
typename GridTypes<T>::GridValue VolumeGrid<T>::triLerp(const Vector &P)
{
    const openvdb::Vec3d xyz(P.X(), P.Y(), P.Z());

    openvdb::Vec3d index = _grid->worldToIndex(xyz);
    
    // GridTypes<T>::GridValue = ...
    auto v = GridTypes<T>::fromGrid(openvdb::tools::BoxSampler::sample(_grid->tree(), index));

    return v;
}

template<typename T>
void VolumeGrid<T>::stamp(const VSP<T> &f)
{
    typename GridTypes<T>::GridAccessor accessor = _grid->getAccessor();

    openvdb::Vec3d size = _grid->transform().voxelSize();

    for ( auto iter = _bbox->beginXYZ(); iter != _bbox->endXYZ(); ++iter)
    {
        openvdb::Vec3d world = _grid->indexToWorld(*iter);

        Vector p(world.x(), world.y(), world.z());

        // GridTypes<T>::GridValue = ...
        auto val = GridTypes<T>::toGrid(f->eval(p));

        accessor.setValue(*iter, val);
    }
    

}

//-----------------------------------------------------------------------------

template<typename T>
GridField<T>::GridField(const VGSP<T> &g) :
    _g(g)
{
}

template<typename T>
const typename Volume<T>::volumeDataType GridField<T>::eval(const Vector &p) const
{
    return _g->triLerp(p);
}

// ----------------------------------------------------------------------------

// Explicit instantiations

// VolumeGrid
template class VolumeGrid<float>;
template class VolumeGrid<Color>;

// GridField
template class GridField<float>;
template class GridField<Color>;

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