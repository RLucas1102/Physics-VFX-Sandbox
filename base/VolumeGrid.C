#include <openvdb/openvdb.h>
#include <openvdb/tools/Interpolation.h>

#include <memory.h>

#include "Volume.h"
#include "VolumeGrid.h"
#include "Vector.h"

using namespace lux;

void VolumeGrid::init(const openvdb::Coord &llc, const openvdb::Coord &urc, const float &vx_size, const float &dg)
{
    openvdb::initialize();

    _grid = openvdb::FloatGrid::create(dg);

    _grid->setTransform(openvdb::math::Transform::createLinearTransform(vx_size));

    _bbox = std::make_shared<openvdb::CoordBBox>(llc, urc);
}

float VolumeGrid::triLerp(const Vector &P)
{
    const openvdb::Vec3R xyz(P.X(), P.Y(), P.Z());
    
    float v = openvdb::tools::BoxSampler::sample(_grid->tree(), xyz);

    return v;
}

void VolumeGrid::stamp(const VSP<float> &f)
{
    typename openvdb::FloatGrid::Accessor accessor = _grid->getAccessor();

    openvdb::Vec3d size = _grid->transform().voxelSize();

    for (openvdb::Int32 i = _bbox->getStart().x(); i <= _bbox->getEnd().x(); i++) {
        for (openvdb::Int32 j = _bbox->getStart().y(); j <= _bbox->getEnd().y(); j++) {
            for (openvdb::Int32 k = _bbox->getStart().z(); k <= _bbox->getEnd().z(); k++) {
                Vector p(i * size.x(), j * size.y(), k * size.z());

                float val = f->eval(p);

                accessor.setValue(openvdb::Coord(i,j,k), val);

            }
        }
    }
    
}

//-----------------------------------------------------------------------------

GridField::GridField(const VGSP &g) :
    _g(g)
{
}

const float GridField::eval(const Vector &p) const
{
    return _g->triLerp(p);
}