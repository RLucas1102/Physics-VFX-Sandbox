/******************************************
 * ImplicitFields.C
 * 
 * The classes in this section define
 * many kinds of volumes that represent
 * a single, uncombined volume (primitives)
 * 
 * They can be combined with other volumes
 * or manipulated via unary or binary 
 * operations.
 ******************************************/

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "ImplicitFields.h"

using namespace lux;

// ---------------
// Constant Field
// ---------------
template <typename T>
ConstantField<T>::ConstantField(const T &v) : 
    _value(v)
{
}

template <typename T>
const typename Volume<T>::volumeDataType ConstantField<T>::eval(const Vector &P) const
{
    return _value;
}

template <typename T>
const typename Volume<T>::volumeGradType ConstantField<T>::grad(const Vector &P) const
{
    return {};
}

//-----------------------------------------------------------------------------

// ------------
// Plane Field
// ------------
PlaneField::PlaneField(const Vector &n, const Vector &p) : 
    _normal(n.unitvector()), 
    _pointOnSurf(p)
{
}

const float PlaneField::eval(const Vector &p) const
{
    return -(p - _pointOnSurf) * _normal; 
}

const Vector PlaneField::grad(const Vector &p) const
{
     return _normal; 
}

//-----------------------------------------------------------------------------

// -------------
// Sphere Field
// -------------
SphereField::SphereField(const float radius) : 
    _radius(radius)
{
}

const float SphereField::eval(const Vector &p) const
{
    return _radius - p.magnitude();
}

const Vector SphereField::grad(const Vector &p) const
{
    Vector result;
    if (p.magnitude() != 0) {result = -p.unitvector(); }
    else {result = Vector(0, 1, 0); }
    return result;
}

//-----------------------------------------------------------------------------

// ------------
// Torus Field
// ------------
TorusField::TorusField(const float rM, const float rm, const Vector &n) : 
    _radMajor(rM), 
    _radMinor(rm), 
    _normal(n.unitvector())
{
}

const float TorusField::eval(const Vector &p) const
{
    Vector X = p;
    Vector xPerp = X - (X * _normal) * _normal;
    float rM2 = std::pow(_radMajor, 2);
    float xPerpMag2 = std::pow(xPerp.magnitude(), 2);
    float xMag2 = std::pow(X.magnitude(), 2);
    float rm2 = std::pow(_radMinor, 2);
    return (4 * rM2 *xPerpMag2) - std::pow(xMag2 + rM2 - rm2, 2);
}

//-----------------------------------------------------------------------------

// -----------
// Cone Field
// -----------
ConeField::ConeField(const float h, const float theta, const Vector &n) : 
    _height(h), 
    _theta(theta), 
    _normal(n.unitvector())
{
}

const float ConeField::eval(const Vector &p) const
{
    Vector X = p;
    float result = 0;
    float xn = X * _normal;
    if(xn < 0) { result = xn; }
    else if (xn > _height) { result = _height - xn; }
    else if (xn > 0 && xn < _height) { result = xn - X.magnitude() * std::cos(_theta); }
    return result;
}

//-----------------------------------------------------------------------------

// ----------
// Box Field
// ----------
BoxField::BoxField(const float radius, const float smooth) : 
    _radius(radius), 
    _smooth(smooth)
{
}

const float BoxField::eval(const Vector &p) const
{
    Vector v = p;
    float x = v[0];
    float y = v[1];
    float z = v[2];
    float xp = std::pow(x, _smooth * 2);
    float yp = std::pow(y, _smooth * 2);
    float zp = std::pow(z, _smooth * 2);
    float rp = std::pow(_radius, _smooth * 2);
    return rp - xp - yp - zp;
}

//-----------------------------------------------------------------------------

// ------------------
// Icosahedron Field
// ------------------
IcosahedronField::IcosahedronField()
{
}

const float IcosahedronField::eval(const Vector &p) const
{
    Vector v = p;
    float result = 0;
    if (v.magnitude() > 1.8 * M_PI) { result = -1.8 * M_PI; }
    else {
        float x = v[0];
        float y = v[1];
        float z = v[2];
        float term1 = std::cos(x + _T*y) + std::cos(x - _T*y);
        float term2 = std::cos(y + _T*z) + std::cos(y - _T*z);
        float term3 = std::cos(z - _T*x) + std::cos(z + _T*x);
        result = term1 + term2 + term3 - 2;              
    }
    return result;
}

//-----------------------------------------------------------------------------

// --------------
// Ellipse Field
// --------------
EllipseField::EllipseField(const float rM, const float rm, const Vector &n) : 
    _radMajor(rM), 
    _radMinor(rm), 
    _normal(n.unitvector())
{
}

const float EllipseField::eval(const Vector &p) const
{
    Vector X = p;
    float z = X * _normal;
    Vector xPerp = X - z * _normal;
    float rM2 = std::pow(_radMajor, 2);
    float xPerpMag2 = std::pow(xPerp.magnitude(), 2);
    float z2 = std::pow(z, 2);
    float rm2 = std::pow(_radMinor, 2);
    return 1 - (z2/rM2) - (xPerpMag2/rm2);
}

//-----------------------------------------------------------------------------

// ---------------
// Cylinder Field
// ---------------
CylinderField::CylinderField(const float radius, const Vector &n) :
    _radius(radius), 
    _normal(n.unitvector())
{
}

const float CylinderField::eval(const Vector &p) const
{
    Vector X = p;
    float xn = X *_normal;
    float mag = (X - (xn * _normal)).magnitude();
    return _radius - mag;
}

//-----------------------------------------------------------------------------

// --------------
// Steiner Field
// --------------
SteinerField::SteinerField()
{
}

const float SteinerField::eval(const Vector &p) const
{
    Vector v = p;
    float x = v[0];
    float y = v[1];
    float z = v[2];
    float x2 = std::pow(x, 2);
    float y2 = std::pow(y, 2);
    float z2 = std::pow(z, 2);
    return -(x2*y2 + x2*z2 + y2*z2 - x*y*z);
}


//-----------------------------------------------------------------------------

// Explicit instantiations

// Constant Field
template class ConstantField<float>;
template class ConstantField<Vector>;
template class ConstantField<Matrix>;
template class ConstantField<Color>;

//-----------------------------------------------------------------------------

/***************************************************
 * Notes:
 * 
 * using typename Volume<T>::volumeDataType;
 * using typename Volume<T>::volumeGradType;
 * 
 * Compiler cannot assume that volumeDataType/volumeGradType are types
 * Volume<T> is a dependent base class which means we cannot assume its type
 * If it was Volume<float> that would be fine 
 * because we now know that volumeDataType is float
 * We create a local typedef of the type for convenient use here
 * 
 * 
 * AddField(const std::shared_ptr<Volume<T>>& a, const std::shared_ptr<Volume<T>>& b) : FieldOperator<T>(a,b) {}
 * Since FieldOperator specifies arguments will be passed, derived classes must
 * have a constructor to pass the arguments up to the base class to store them as
 * member variables
 * 
 ***************************************************/