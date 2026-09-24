/******************************************
 * FieldInterface.C
 * 
 * Lucas Robenolt
 * 
 * These are all helper functions to 
 * instantiate volumes and operators
 * in order to keep the actual classes
 * separate
 ******************************************/

#include "FieldInterface.h"

namespace lux {

// Field Helper Functions
// ----------------------------------------------------------------------------
template <typename T>
volumeDataType<T> evaluate(const VSP<T>& f, const Vector& P) {
    return f->eval(P);
}

template <typename T>
volumeGradType<T> gradient(const VSP<T>& f, const Vector& P) {
    return f->grad(P);
}

// ----------------------------------------------------------------------------

// Field Volume Creation Helper Functions
// ----------------------------------------------------------------------------
template <typename T>
VSP<T> constant(const T& v) {
    return std::make_shared<ConstantField<T>>(v);
}

VSP<float> plane(const Vector& n, const Vector& p) {
    return std::make_shared<PlaneField>(n, p);
}

VSP<float> sphere(const float radius) {
    return std::make_shared<SphereField>(radius);
}

VSP<float> torus(const float rM, const float rm, const Vector& n) {
    return std::make_shared<TorusField>(rM, rm, n);
}

VSP<float> cone(const float h, const float theta, const Vector& n) {
    return std::make_shared<ConeField>(h, theta, n);
}

VSP<float> box(const float radius, const float smooth) {
    return std::make_shared<BoxField>(radius, smooth);
}

VSP<float> icosahedron() {
    return std::make_shared<IcosahedronField>();
}

VSP<float> ellipse(const float rM, const float rm, const Vector& n) {
    return std::make_shared<EllipseField>(rM, rm, n);
}

VSP<float> cylinder(const float radius, const Vector& n) {
    return std::make_shared<CylinderField>(radius, n);
}

VSP<float> steiner() {
    return std::make_shared<SteinerField>();
}

VSP<float> grid(const VGSP g) {
    return std::make_shared<GridField>(g);
}

// ----------------------------------------------------------------------------

// Unary Field Operator Helper Functions
// ----------------------------------------------------------------------------
template <typename T>
VSP<T> mask(const VSP<T>& a)
{
    return std::make_shared<MaskField<T>>(a);
}

template <typename T>
VSP<T> negate(const VSP<T>& a)
{
    return std::make_shared<NegateField<T>>(a);
}   

template <typename T>
VSP<T> Exp(const VSP<T>& a)
{
    return std::make_shared<ExpField<T>>(a);
}

template <typename T>
VSP<T> Log(const VSP<T>& a)
{
    return std::make_shared<LogField<T>>(a);
}

template <typename T>
VSP<T> Sin(const VSP<T>& a)
{
    return std::make_shared<SinField<T>>(a);
}

template <typename T>
VSP<T> Cos(const VSP<T>& a)
{
    return std::make_shared<CosField<T>>(a);
}

template <typename T>
VSP<T> Pow(const VSP<T>& a, const float val)
{
    return std::make_shared<PowField<T>>(a, val);
}

template <typename T>
VSP<T> translate(const VSP<T>& a, const Vector& xt)
{
    return std::make_shared<TranslateField<T>>(a, xt);
}

template <typename T>
VSP<T> scale(const VSP<T>& a, const float val)
{
    return std::make_shared<ScaleField<T>>(a, val);
}

template <typename T>
VSP<T> rotate(const VSP<T>& a, const float theta, const Vector& axis)
{
    return std::make_shared<RotateField<T>>(a, theta, axis);
}

template <typename T>
VSP<T> dilate(const VSP<T>& a, const float val)
{
    return std::make_shared<DilateField<T>>(a, val);
}

template <typename T>
VSP<T> shell(const VSP<T>& a, const float val)
{
    return std::make_shared<ShellField<T>>(a, val);
}

template <typename T>
VSP<T> clamp(const VSP<T>& a, const float fmin, const float fmax) 
{
    return std::make_shared<ClampField<T>>(a, fmin, fmax);
}

// ----------------------------------------------------------------------------

// Binary Field Operator Helper Functions
// ----------------------------------------------------------------------------
template <typename T>
VSP<T> add(const VSP<T>& a, 
           const VSP<T>& b) 
{
    return std::make_shared<AddField<T>>(a,b);
}

template <typename T>
VSP<T> sub(const VSP<T>& a, 
           const VSP<T>& b) 
{
    return std::make_shared<SubtractField<T>>(a,b);
}

template <typename T, typename U>
VSP<T> multiply(const VSP<T>& a,
                const VSP<U>& b)
{
    return std::make_shared<MultiplyField<T,U>>(a,b);
}

template <typename T, typename U>
VSP<T> divide(const VSP<T>& a,
              const VSP<U>& b)
{
    return std::make_shared<DivideField<T,U>>(a,b);
}

template <typename T>
VSP<T> Union(const VSP<T>& a, 
             const VSP<T>& b) 
{
    return std::make_shared<UnionField<T>>(a,b);
}

template <typename T>
VSP<T> Intersection(const VSP<T>& a, 
                    const VSP<T>& b) 
{
    return std::make_shared<IntersectionField<T>>(a,b);
}

template <typename T>
VSP<T> Cutout(const VSP<T>& a, 
              const VSP<T>& b) 
{
    return std::make_shared<CutoutField<T>>(a,b);
}

template <typename T>
VSP<T> Blend(const VSP<T>& a, 
             const VSP<T>& b,
             const float alpha1,
             const float alpha2)
{
    return std::make_shared<BlendField<T>>(a,b, alpha1, alpha2);
}

// ----------------------------------------------------------------------------

// Operator Overloads
// ----------------------------------------------------------------------------
template <typename T>
VSP<T> operator+(const VSP<T>& a, 
                 const VSP<T>& b)
{
    return std::make_shared<AddField<T>>(a,b);
}

template <typename T>
VSP<T> operator-(const VSP<T>& a, 
                 const VSP<T>& b)
{
    return std::make_shared<SubtractField<T>>(a,b);
}

template <typename T, typename U>
VSP<T> operator*(const VSP<T>& a,
                 const VSP<U>& b)
{
    return std::make_shared<MultiplyField<T,U>>(a,b);
}

template <typename T, typename U>
VSP<T> operator/(const VSP<T>& a,
                 const VSP<U>& b)
{
    return std::make_shared<DivideField<T,U>>(a,b);
}

template <typename T>
VSP<T> operator-(const VSP<T>& a)
{
    return std::make_shared<NegateField<T>>(a);
}

// ----------------------------------------------------------------------------

// Explicit instantiations

// Evaluate
template volumeDataType<float> evaluate(const VSP<float>& f, const Vector& P);
template volumeDataType<Vector> evaluate(const VSP<Vector>& f, const Vector& P);
template volumeDataType<Matrix> evaluate(const VSP<Matrix>& f, const Vector& P);
template volumeDataType<Color> evaluate(const VSP<Color>& f, const Vector& P);

// Gradient
template volumeGradType<float> gradient(const VSP<float>& f, const Vector& P);
template volumeGradType<Vector> gradient(const VSP<Vector>& f, const Vector& P);

// Constant
template VSP<float> constant(const float& v);
template VSP<Vector> constant(const Vector& v);
template VSP<Matrix> constant(const Matrix& v);
template VSP<Color> constant(const Color& v);

// Mask
template VSP<float> mask(const VSP<float>& a);

// Negate
template VSP<float> negate(const VSP<float>& a);
template VSP<Vector> negate(const VSP<Vector>& a);
template VSP<Matrix> negate(const VSP<Matrix>& a);
template VSP<Color> negate(const VSP<Color>& a);

// Exp
template VSP<float> Exp(const VSP<float>& a);

// Log
template VSP<float> Log(const VSP<float>& a);

// Sin
template VSP<float> Sin(const VSP<float>& a);

// Cos
template VSP<float> Cos(const VSP<float>& a);

// Pow
template VSP<float> Pow(const VSP<float>& a, const float val);

// Translate
template VSP<float> translate(const VSP<float>& a, const Vector& xt);

// Scale
template VSP<float> scale(const VSP<float>& a, const float val);
template VSP<Color> scale(const VSP<Color>& a, const float val);

// Rotate
template VSP<float> rotate(const VSP<float>& a, const float theta, const Vector& axis);

// Dilate
template VSP<float> dilate(const VSP<float>& a, const float val);

// Shell
template VSP<float> shell(const VSP<float>& a, const float val);

// Clamp
template VSP<float> clamp(const VSP<float>& a, const float fmin, const float fmax); 

// Add
template VSP<float> add(const VSP<float>& a, const VSP<float>& b);
template VSP<Vector> add(const VSP<Vector>& a, const VSP<Vector>& b);
template VSP<Matrix> add(const VSP<Matrix>& a, const VSP<Matrix>& b);
template VSP<Color> add(const VSP<Color>& a, const VSP<Color>& b); 

// Subtract
template VSP<float> sub(const VSP<float>& a, const VSP<float>& b);
template VSP<Vector> sub(const VSP<Vector>& a, const VSP<Vector>& b);
template VSP<Matrix> sub(const VSP<Matrix>& a, const VSP<Matrix>& b);
template VSP<Color> sub(const VSP<Color>& a, const VSP<Color>& b); 

// Multiply
template VSP<float> multiply(const VSP<float>& a, const VSP<float>& b);
template VSP<Vector> multiply(const VSP<Vector>& a, const VSP<float>& b);
template VSP<Matrix> multiply(const VSP<Matrix>& a, const VSP<float>& b);
template VSP<Color> multiply(const VSP<Color>& a, const VSP<float>& b);

// Divide
template VSP<float> divide(const VSP<float>& a, const VSP<float>& b);
template VSP<Vector> divide(const VSP<Vector>& a, const VSP<float>& b);
template VSP<Matrix> divide(const VSP<Matrix>& a, const VSP<float>& b);
template VSP<Color> divide(const VSP<Color>& a, const VSP<float>& b);

// Union
template VSP<float> Union(const VSP<float>& a, const VSP<float>& b);

// Intersection
template VSP<float> Intersection(const VSP<float>& a, const VSP<float>& b);

// Cutout
template VSP<float> Cutout(const VSP<float>& a, const VSP<float>& b);

// Blend
template VSP<float> Blend(const VSP<float>& a, const VSP<float>& b, const float alpha1, const float alpha2);

// Add operator
template VSP<float> operator+(const VSP<float>& a, const VSP<float>& b);
template VSP<Vector> operator+(const VSP<Vector>& a, const VSP<Vector>& b);
template VSP<Matrix> operator+(const VSP<Matrix>& a, const VSP<Matrix>& b);
template VSP<Color> operator+(const VSP<Color>& a, const VSP<Color>& b);

// Subtract operator
template VSP<float> operator-(const VSP<float>& a, const VSP<float>& b);
template VSP<Vector> operator-(const VSP<Vector>& a, const VSP<Vector>& b);
template VSP<Matrix> operator-(const VSP<Matrix>& a, const VSP<Matrix>& b);
template VSP<Color> operator-(const VSP<Color>& a, const VSP<Color>& b);

// Multiply operator
template VSP<float> operator*(const VSP<float>& a, const VSP<float>& b);
template VSP<Vector> operator*(const VSP<Vector>& a, const VSP<float>& b);
template VSP<Matrix> operator*(const VSP<Matrix>& a, const VSP<float>& b);
template VSP<Color> operator*(const VSP<Color>& a, const VSP<float>& b);

// Divide operator
template VSP<float> operator/(const VSP<float>& a, const VSP<float>& b);
template VSP<Vector> operator/(const VSP<Vector>& a, const VSP<float>& b);
template VSP<Matrix> operator/(const VSP<Matrix>& a, const VSP<float>& b);
template VSP<Color> operator/(const VSP<Color>& a, const VSP<float>& b);

// Negate operator
template VSP<float> operator-(const VSP<float>& a);
template VSP<Vector> operator-(const VSP<Vector>& a);
template VSP<Matrix> operator-(const VSP<Matrix>& a);
template VSP<Color> operator-(const VSP<Color>& a);

}

// ----------------------------------------------------------------------------

/***************************************************
 * Notes:
 * 
 * Cannot put definition in a .C file because templates
 * are just patterns, not classes. The class is
 * created when it is needed at compile time. If main.C
 * needs an add<float> function it will find the
 * declaration, but not the definition because
 * it was never created when we compiled FieldInterface.C
 * which would be compiled first in this order.
 * 
 * 
 * template <typename T>
 * using volumeDataType = typename Volume<T>::volumeDataType;
 * 
 * This is different from the one we created in the classes.
 * Essentially, we are saying to create a type called
 * volumeDataType here in this file. But, to do so, 
 * look in Volume<T> for what volumeDataType should 
 * be. This one is creating a new alias template
 * 
 * The other example in ImplicitFields.h is introducing the
 * type from Volume<T> to the derived class's scope.
 * This one is importing the type into the class. However,
 * I could put the above syntax into the classes and it
 * would do essentially the same thing as what is already
 * there.
 * 
 ***************************************************/
