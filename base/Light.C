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
    // Create Deep Shadow Map in here
}

// -------------------------------------------------------
