#ifndef IMPLICITFIELDS_H
#define IMPLICITFIELDS_H

#include "Volume.h"
#include "Vector.h"

namespace lux {

    template<typename T>
    class ConstantField : public Volume<T> {

        using typename Volume<T>::volumeDataType;
        using typename Volume<T>::volumeGradType;

        public:
            ConstantField(const T v) : _value(v), _gradValue() {}
            ~ConstantField() = default;

            const volumeDataType eval(const Vector& P) const override { return _value; }
            const volumeGradType grad(const Vector& P) const override { return {}; }
        
        private:
            volumeDataType _value;
            volumeGradType _gradValue;

    };

} 

#endif

/***************************************************
 * Notes:
 * 
 * using typename Volume<T>::volumeDataType;
 * using typename Volume<T>::volumeGradType;
 * 
 * Compiler cannot assume that volumeDataType/volumeGradType are types
 * Volume<T> is a dependent base class which means we cannout assume its type
 * If it was Volume<float> that would be fine 
 * because we now know that volumeDataType is float
 * We create a local typedef of the type for convenient use here
 * 
 ***************************************************/