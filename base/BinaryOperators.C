/******************************************
 * BinaryOperations.C
 * 
 * Lucas Robenolt
 * 
 * The classes in this section define 
 * different operations that can be
 * performed to combine two fields.
 * Operations are represented as fields
 * themselves because they can be evaluated
 * at any point in the field like primitives.
 * Combining fields essentially creates a
 * new field
 * 
 * These classes are created through helper
 * functions defined in FieldInterface.h
 * See FieldInterface.h for more info
 ******************************************/

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "BinaryOperators.h"

using namespace lux;

// ------------------------
// Binary Field Operations
// ------------------------
template<typename T, typename U>
BinaryFieldOperator<T, U>::BinaryFieldOperator(const VSP<T>& a, const VSP<U>& b) : 
    _a(a), 
    _b(b) 
{   
}

//-----------------------------------------------------------------------------

// --------------
// Add Operation
// --------------
template<typename T>
AddField<T>::AddField(const VSP<T>& a, const VSP<T>& b) : 
    BinaryFieldOperator<T, T>(a, b) 
{    
}

template<typename T>
const typename Volume<T>::volumeDataType AddField<T>::eval(const Vector& P) const 
{
    return this->_a->eval(P) + this->_b->eval(P);
} 

//-----------------------------------------------------------------------------

// -------------------
// Subtract Operation
// -------------------
template<typename T>
SubtractField<T>::SubtractField(const VSP<T>& a, const VSP<T>& b) : 
    BinaryFieldOperator<T, T>(a, b) 
{    
}

template<typename T>
const typename Volume<T>::volumeDataType SubtractField<T>::eval(const Vector& P) const 
{
    return this->_a->eval(P) - this->_b->eval(P);
}

//-----------------------------------------------------------------------------

// -------------------
// Multiply Operation
// -------------------
template<typename T, typename U>
MultiplyField<T,U>::MultiplyField(const VSP<T>& a, const VSP<U>& b) : 
    BinaryFieldOperator<T, U>(a,b)
{    
}

template<typename T, typename U>
const typename Volume<T>::volumeDataType MultiplyField<T,U>::eval(const Vector& P) const 
{
    return this->_a->eval(P) * this->_b->eval(P); 
}

//-----------------------------------------------------------------------------

// -----------------
// Divide Operation
// -----------------
template<typename T, typename U>
DivideField<T,U>::DivideField(const VSP<T>& a, const VSP<U>& b) : 
    BinaryFieldOperator<T, U>(a,b)
{    
}

template<typename T, typename U>
const typename Volume<T>::volumeDataType DivideField<T,U>::eval(const Vector& P) const 
{
    return this->_a->eval(P) / this->_b->eval(P); 
}

//-----------------------------------------------------------------------------

// ----------------
// Union Operation
// ----------------
template<typename T>
UnionField<T>::UnionField(const VSP<T>& a, const VSP<T>& b) : 
    BinaryFieldOperator<T, T>(a, b) 
{    
}

template<typename T>
const typename Volume<T>::volumeDataType UnionField<T>::eval(const Vector& P) const 
{
    return std::max(this->_a->eval(P), this->_b->eval(P)); 
}

//-----------------------------------------------------------------------------

// -----------------------
// Intersection Operation
// -----------------------
template<typename T>
IntersectionField<T>::IntersectionField(const VSP<T>& a, const VSP<T>& b) : 
    BinaryFieldOperator<T, T>(a, b) 
{    
}

template<typename T>
const typename Volume<T>::volumeDataType IntersectionField<T>::eval(const Vector& P) const 
{
    return std::min(this->_a->eval(P), this->_b->eval(P)); 
}

//-----------------------------------------------------------------------------

// -----------------
// Cutout Operation
// -----------------
template<typename T>
CutoutField<T>::CutoutField(const VSP<T>& a, const VSP<T>& b) : 
    BinaryFieldOperator<T, T>(a, b) 
{    
}

template<typename T>
const typename Volume<T>::volumeDataType CutoutField<T>::eval(const Vector& P) const 
{
    return std::min(this->_a->eval(P), -this->_b->eval(P)); 
}

//-----------------------------------------------------------------------------

// -----------------
// Blend Operation
// -----------------
template<typename T>
BlendField<T>::BlendField(const VSP<T>& a, const VSP<T>& b, const float alpha1, const float alpha2) : 
    BinaryFieldOperator<T, T>(a,b), _alpha1(alpha1), _alpha2(alpha2) 
{    
}

template<typename T>
const typename Volume<T>::volumeDataType BlendField<T>::eval(const Vector& P) const 
{
    return std::exp(_alpha1 * this->_a->eval(P)) + std::exp(_alpha2 * this->_b->eval(P)) - 2; 
}

//-----------------------------------------------------------------------------

// Explicit instantiations

// Add Field
template class AddField<float>;
template class AddField<Vector>;
template class AddField<Matrix>;
template class AddField<Color>;

// Subtract Field
template class SubtractField<float>;
template class SubtractField<Vector>;
template class SubtractField<Matrix>;
template class SubtractField<Color>;

// Multiply Field
template class MultiplyField<float, float>;
template class MultiplyField<Vector, float>;
template class MultiplyField<Matrix, float>;
template class MultiplyField<Color, float>;

// Divide Field
template class DivideField<float, float>;
template class DivideField<Vector, float>;
template class DivideField<Matrix, float>;
template class DivideField<Color, float>;

// Union Field
template class UnionField<float>;

// Intersection Field
template class IntersectionField<float>;

// Cutout Field
template class CutoutField<float>;

// Blend Field
template class BlendField<float>;

//-----------------------------------------------------------------------------