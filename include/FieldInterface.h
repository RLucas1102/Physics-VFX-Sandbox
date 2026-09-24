/******************************************
 * FieldInterface.h
 * 
 * Lucas Robenolt
 * 
 * These are all helper functions to 
 * instantiate volumes and operators
 * in order to keep the actual classes
 * separate
 ******************************************/

#ifndef FIELDINTERFACE_H
#define FIELDINTERFACE_H

#include <memory>

#include "Volume.h"
#include "VolumeGrid.h"
#include "ImplicitFields.h"
#include "UnaryOperators.h"
#include "BinaryOperators.h"

namespace lux {

    template <typename T>
    using volumeDataType = typename Volume<T>::volumeDataType;

    template <typename T>
    using volumeGradType = typename Volume<T>::volumeGradType;

    // Field Helper Functions
    // ----------------------------------------------------------------------------
    template <typename T>
    volumeDataType<T> evaluate(const VSP<T>& f, const Vector& P);

    template <typename T>
    volumeGradType<T> gradient(const VSP<T>& f, const Vector& P);

    // ----------------------------------------------------------------------------

    // Field Volume Creation Helper Functions
    // ----------------------------------------------------------------------------
    template <typename T>
    VSP<T> constant(const T& v);

    VSP<float> plane(const Vector& n, const Vector& p);

    VSP<float> sphere(const float radius);

    VSP<float> torus(const float rM, const float rm, const Vector& n);

    VSP<float> cone(const float h, const float theta, const Vector& n);

    VSP<float> box(const float radius, const float smooth);

    VSP<float> icosahedron();

    VSP<float> ellipse(const float rM, const float rm, const Vector& n);

    VSP<float> cylinder(const float radius, const Vector& n);

    VSP<float> steiner();

    VSP<float> grid(const VGSP g);

    // ----------------------------------------------------------------------------

    // Unary Field Operator Helper Functions
    // ----------------------------------------------------------------------------
    template <typename T>
    VSP<T> mask(const VSP<T>& a);

    template <typename T>
    VSP<T> negate(const VSP<T>& a);
    
    template <typename T>
    VSP<T> Exp(const VSP<T>& a);

    template <typename T>
    VSP<T> Log(const VSP<T>& a);

    template <typename T>
    VSP<T> Sin(const VSP<T>& a);
    
    template <typename T>
    VSP<T> Cos(const VSP<T>& a);

    template <typename T>
    VSP<T> Pow(const VSP<T>& a, 
               const float val);

    template <typename T>
    VSP<T> translate(const VSP<T>& a, 
                     const Vector& xt);

    template <typename T>
    VSP<T> scale(const VSP<T>& a, 
                 const float val);

    template <typename T>
    VSP<T> rotate(const VSP<T>& a, 
                  const float theta, 
                  const Vector& axis);

    template <typename T>
    VSP<T> dilate(const VSP<T>& a, 
                  const float val);

    template <typename T>
    VSP<T> shell(const VSP<T>& a, 
                 const float val);

    template <typename T>
    VSP<T> clamp(const VSP<T>& a, 
                 const float fmin, 
                 const float fmax);

    // ----------------------------------------------------------------------------

    // Binary Field Operator Helper Functions
    // ----------------------------------------------------------------------------
    template <typename T>
    VSP<T> add(const VSP<T>& a, 
               const VSP<T>& b);

    template <typename T>
    VSP<T> sub(const VSP<T>& a, 
               const VSP<T>& b);

    template <typename T, typename U>
    VSP<T> multiply(const VSP<T>& a,
                    const VSP<U>& b);

    template <typename T, typename U>
    VSP<T> divide(const VSP<T>& a,
                  const VSP<U>& b);

    template <typename T>
    VSP<T> Union(const VSP<T>& a, 
                 const VSP<T>& b);

    template <typename T>
    VSP<T> Intersection(const VSP<T>& a, 
                        const VSP<T>& b);

    template <typename T>
    VSP<T> Cutout(const VSP<T>& a, 
                  const VSP<T>& b);

    template <typename T>
    VSP<T> Blend(const VSP<T>& a, 
                 const VSP<T>& b,
                 const float alpha1,
                 const float alpha2);

    // ----------------------------------------------------------------------------

    // Operator Overloads
    // ----------------------------------------------------------------------------
    template <typename T>
    VSP<T> operator+(const VSP<T>& a, 
                     const VSP<T>& b);

    template <typename T>
    VSP<T> operator-(const VSP<T>& a, 
                     const VSP<T>& b);

    template <typename T, typename U>
    VSP<T> operator*(const VSP<T>& a,
                     const VSP<U>& b);

    template <typename T, typename U>
    VSP<T> operator/(const VSP<T>& a,
                     const VSP<U>& b);

    template <typename T>
    VSP<T> operator-(const VSP<T>& a);

    // ----------------------------------------------------------------------------
}

#endif