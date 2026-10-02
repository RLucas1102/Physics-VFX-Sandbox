#include "Light.h"

using namespace lux;

// -----------
// Base Class
// -----------
LightBase::LightBase(const Vector &inPos, const Color &inCol) :
    _pos(inPos),
    _col(inCol)
{
}

//-----------------------------------------------------------------------------

// ------------
// Point light
// ------------
PointLight::PointLight(const Vector &inPos, const Color &inCol) :
    LightBase(inPos,inCol)
{
}

void PointLight::createDSM(const VSP<float>& inGridField, 
                           const openvdb::CoordBBox& bbox,
                           float vx_size,
                           float dg)
{
    VGSP<openvdb::FloatGrid> gridTemp = grid<openvdb::FloatGrid>();

    gridTemp->init(bbox, vx_size, dg);

    typename openvdb::FloatGrid::Accessor accessor = gridTemp->getGridAccessor();

    for (auto iter = gridTemp->getBBox().beginXYZ(); iter !=  gridTemp->getBBox().endXYZ(); ++iter)
    {
        openvdb::Vec3d world = gridTemp->getGridRaw()->indexToWorld(*iter);
        Vector p(world.x(), world.y(), world.z());
        double val = 0;

        if(inGridField->eval(p) > 0.0) {
            double smax      = (_pos - p).magnitude();
            Vector direction = (_pos - p).unitvector();
            double s = 0;

            while (s < smax)
            {
                val += inGridField->eval(p) * _settings.ds;
                p += direction * _settings.ds;
                s += _settings.ds;
            }

            accessor.setValue(*iter, val);
            
        }

    }

    _DSM = Exp(gridField<openvdb::FloatGrid, float>(gridTemp) * constant(-_settings.kappa));

}

//-----------------------------------------------------------------------------

// -----------------
// Helper Functions
// -----------------

PLight lux::pointLight(const Vector &inPos, const Color &inCol)
{
    return std::make_shared<PointLight>(inPos, inCol);
}

//-----------------------------------------------------------------------------

