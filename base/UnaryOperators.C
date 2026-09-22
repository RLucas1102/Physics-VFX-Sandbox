/******************************************
 * UnaryOperations.C
 * 
 * Lucas Robenolt
 * 
 * The classes in this section define 
 * different operations that can be
 * performed on one field
 * 
 * These classes are created through helper
 * functions defined in FieldInterface.h
 * See FieldInterface.h for more info
 ******************************************/

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "UnaryOperators.h"

using namespace lux;

// -----------------------
// Unary Field Operations
// -----------------------
template<typename T>
UnaryFieldOperator<T>::UnaryFieldOperator(const VSP<T>& a) : 
    _a(a)
{   
}

//-----------------------------------------------------------------------------

// ---------------
// Mask Operation
// ---------------
template<typename T>
MaskField<T>::MaskField(const VSP<T>& a) : 
    UnaryFieldOperator<T>(a) 
{    
}

template<typename T>
const typename Volume<T>::volumeDataType MaskField<T>::eval(const Vector& P) const 
{
    if (this->_a->eval(P) > 0) { return 1; }
    else { return 0; }
}

//-----------------------------------------------------------------------------

// ---------------
// Clamp Operation
// ---------------
template<typename T>
ClampField<T>::ClampField(const VSP<T>& a, const float fmin, const float fmax) : 
    UnaryFieldOperator<T>(a), 
    _fmin(fmin), 
    _fmax(fmax)
{    
}

template<typename T>
const typename Volume<T>::volumeDataType ClampField<T>::eval(const Vector& P) const 
{
    float result = 0;
    if (this->_a->eval(P) <= _fmin) { result = _fmin; }
    else if (this->_a->eval(P) > _fmin && this->_a->eval(P) < _fmax){ result = this->_a->eval(P); }
    else if (this->_a->eval(P) >= _fmax) { result = _fmax; }
    return result;
}

//-----------------------------------------------------------------------------

// -----------------
// Negate Operation
// -----------------
template<typename T>
NegateField<T>::NegateField(const VSP<T>& a) : 
    UnaryFieldOperator<T>(a) 
{    
}

template<typename T>
const typename Volume<T>::volumeDataType NegateField<T>::eval(const Vector& P) const 
{
    return -this->_a->eval(P);
} 

//-----------------------------------------------------------------------------

// ---------------
// Exp Operation
// ---------------
template<typename T>
ExpField<T>::ExpField(const VSP<T>& a) : 
    UnaryFieldOperator<T>(a) 
{    
}

template<typename T>
const typename Volume<T>::volumeDataType ExpField<T>::eval(const Vector& P) const 
{
    return std::exp(this->_a->eval(P)); 
}

//-----------------------------------------------------------------------------

// ---------------
// Log Operation
// ---------------
template<typename T>
LogField<T>::LogField(const VSP<T>& a) : 
    UnaryFieldOperator<T>(a) 
{    
}

template<typename T>
const typename Volume<T>::volumeDataType LogField<T>::eval(const Vector& P) const 
{
   return std::log(this->_a->eval(P));
}

//-----------------------------------------------------------------------------

// ---------------
// Sin Operation
// ---------------
template<typename T>
SinField<T>::SinField(const VSP<T>& a) : 
    UnaryFieldOperator<T>(a) 
{    
}

template<typename T>
const typename Volume<T>::volumeDataType SinField<T>::eval(const Vector& P) const 
{
   return std::sin(this->_a->eval(P));
}

//-----------------------------------------------------------------------------

// ---------------
// Cos Operation
// ---------------
template<typename T>
CosField<T>::CosField(const VSP<T>& a) : 
    UnaryFieldOperator<T>(a) 
{    
}

template<typename T>
const typename Volume<T>::volumeDataType CosField<T>::eval(const Vector& P) const 
{
   return std::cos(this->_a->eval(P));
}

//-----------------------------------------------------------------------------

// ---------------
// Pow Operation
// ---------------
template<typename T>
PowField<T>::PowField(const VSP<T>& a, const float val) : 
    UnaryFieldOperator<T>(a), 
    _val(val) 
{    
}

template<typename T>
const typename Volume<T>::volumeDataType PowField<T>::eval(const Vector& P) const 
{
    return std::pow(this->_a->eval(P), _val);
}

//-----------------------------------------------------------------------------

// ---------------
// Translate Operation
// ---------------
template<typename T>
TranslateField<T>::TranslateField(const VSP<T>& a, const Vector& xt) : 
    UnaryFieldOperator<T>(a), 
    _Xt(xt) 
{    
}

template<typename T>
const typename Volume<T>::volumeDataType TranslateField<T>::eval(const Vector& P) const 
{
    return this->_a->eval(P - _Xt);
}

//-----------------------------------------------------------------------------

// ---------------
// Scale Operation
// ---------------
template<typename T>
ScaleField<T>::ScaleField(const VSP<T>& a, const float val) : 
    UnaryFieldOperator<T>(a), 
    _val(val) 
{    
}

template<typename T>
const typename Volume<T>::volumeDataType ScaleField<T>::eval(const Vector& P) const 
{
    return this->_a->eval(P / _val);
}

//-----------------------------------------------------------------------------

// ---------------
// Rotate Operation
// ---------------
template<typename T>
RotateField<T>::RotateField(const VSP<T>& a, const float theta, const Vector& axis) : 
    UnaryFieldOperator<T>(a), 
    _theta(-theta), 
    _axis(axis.unitvector())
{    
}

template<typename T>
const typename Volume<T>::volumeDataType RotateField<T>::eval(const Vector& P) const 
{
    Vector X = P;
    float Cos = std::cos(_theta);
    float ax = _axis * X;
    Vector xa = X^_axis;
    Vector result = X * Cos + _axis * ax * (1 - Cos) + xa * std::sin(_theta);
    return this->_a->eval(result);
}

//-----------------------------------------------------------------------------

// ---------------
// Dilate Operation
// ---------------
template<typename T>
DilateField<T>::DilateField(const VSP<T>& a, const float val) : 
    UnaryFieldOperator<T>(a), 
    _val(val)
{    
}

template<typename T>
const typename Volume<T>::volumeDataType DilateField<T>::eval(const Vector& P) const 
{
    return this->_a->eval(P) + _val;
}

//-----------------------------------------------------------------------------

// ---------------
// Shell Operation
// ---------------
template<typename T>
ShellField<T>::ShellField(const VSP<T>& a, const float val) : 
    UnaryFieldOperator<T>(a), 
    _val(val)
{    
}

template<typename T>
const typename Volume<T>::volumeDataType ShellField<T>::eval(const Vector& P) const 
{
    return std::min(this->_a->eval(P) + _val / 2, -(this->_a->eval(P) - _val / 2));
}

//-----------------------------------------------------------------------------

// Explicit instantiations

// Mask Field
template class MaskField<float>;

// Clamp Field
template class ClampField<float>;

// Negate Field
template class NegateField<float>;
template class NegateField<Vector>;
template class NegateField<Matrix>;
template class NegateField<Color>;

// Exp Field
template class ExpField<float>;

// Log Field
template class LogField<float>;

// Sin Field
template class SinField<float>;

// Cos Field
template class CosField<float>;

// Pow Field
template class PowField<float>;

// Translate Field
template class TranslateField<float>;

// Scale Field
template class ScaleField<float>;
template class ScaleField<Color>;

// Rotate Field
template class RotateField<float>;

// Dilate Field
template class DilateField<float>;

// Shell Field
template class ShellField<float>;

//-----------------------------------------------------------------------------


