#include "Light.h"

// Base Class
// -------------------------------------------------------
LightBase::Light(const Vector &inPos, const Color &inCol) :
    _pos(inPos),
    _col(inCol)
{
}

// -------------------------------------------------------

// Point light
// -------------------------------------------------------
PointLight::PointLight(const Vector &inPos, const Color &inCol) :
    Light(inPos,inCol)
{
}

void PointLight::createDSM(const VSP<float> gridField)
{
    VGSP<float> grid = std::make_shared<VolumeGrid<float>>();

    VGSP<float> inGrid = gridField->getGridRaw();

    grid->init(inGrid->getBBox(), inGrid->getXform().voxelSize, 0.0f);

    GridTypes<float>::GridAccessor accessor = grid->getGridAccessor();

    for (auto iter = grid->getBBox()->beginXYZ(); iter !=  grid->getBBox()->endXYZ(); ++iter)
    {
        openvdb::Vec3d world = grid->indexToWorld(*iter);
        Vector p(world.x(), world.y(), world.z());
        double val = 0;

        if(gridField->eval(p) > 0.0) {
            double smax      = (_pos - p).magnitude();
            double direction = (_pos - p).unitvector();
            double s = 0;

            while (s < smax)
            {
                val += gridField->eval(p) * _settings.ds;
                X += direction * _settings.ds;
                s += _settings.ds;
            }

            accessor.setValue(*iter, val);
            
        }

    }

    _DSM = exp(constant(-kappa) * grid(grid));

}

// -------------------------------------------------------
