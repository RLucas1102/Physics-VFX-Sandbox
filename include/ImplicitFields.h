#ifndef IMPLICITFIELDS_H
#define IMPLICITFIELDS_H

#include <iostream>

#include "Volume.h"
#include "Vector.h"

namespace lux {

    // Defining std::shared_ptr<Volume<T>> as type VSP
    template <typename T>
    using VSP = std::shared_ptr<Volume<T>>;

    /******************************************
     * Section: Simple volumes
     * 
     * The classes in this section define
     * many kinds of volumes that represent
     * a single, uncombined volume
     * 
     * They can be combined with other volumes
     * See Section: Field Operations
     ******************************************/

    // Constant Field
    // Should return the given value for any evaluated point
    template<typename T>
    class ConstantField : public Volume<T> {

        using typename Volume<T>::volumeDataType;
        using typename Volume<T>::volumeGradType;

        public:
            ConstantField(const T& v) : _value(v) {}
            ~ConstantField() = default;

            const volumeDataType eval(const Vector& P) const override { return _value; }
            const volumeGradType grad(const Vector& P) const override { return {}; }
        
        private:
            volumeDataType _value;
            volumeGradType _gradValue;

    };
    
    // ------------------------------------------------------------------------------------


    /******************************************
     * Section: Binary Field Operations
     * 
     * The classes in this section define 
     * different operations that can be
     * performed to combine two fields.
     * Operations are represented as fields
     * themselves because they can be evaluated
     * the same at any point in the field.
     * Combining fields essentially creates a
     * new field
     * 
     * These classes are created through helper
     * functions defined in FieldInterface.h
     * See FieldInterface.h for more info
     ******************************************/

    // Base class
    // All field operations will derive from this abstract class 
    template<typename T, typename U>
    class BinaryFieldOperator : public Volume<T> {
        
        public:

            // Need to make these public for derived classes like Volume does
            // Enforcing that the GradType will be defined by T only
            using typename Volume<T>::volumeDataType;
            using typename Volume<T>::volumeGradType;

            BinaryFieldOperator(const VSP<T>& a, const VSP<U>& b) : _a(a), _b(b) {}
            ~BinaryFieldOperator() = default;

            virtual const volumeDataType eval(const Vector& P) const = 0;
            virtual const volumeGradType grad(const Vector& P) const = 0;

        protected:
            VSP<T> _a; 
            VSP<U> _b;
    };

    // AddFields
    // Two fields can be combined via an add operation
    template<typename T, typename U>
    class AddField : public BinaryFieldOperator<T, U> {

        using typename Volume<T>::volumeDataType;
        using typename Volume<T>::volumeGradType;

        public:
            AddField(const VSP<T>& a, const VSP<U>& b) : BinaryFieldOperator<T, U>(a,b) {}

            const volumeDataType eval(const Vector& P) const override { 
                return this->_a->eval(P) + this->_b->eval(P); 
            }

            const volumeGradType grad(const Vector& P) const override {
                return {};
            }

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
 * 
 * AddField(const std::shared_ptr<Volume<T>>& a, const std::shared_ptr<Volume<T>>& b) : FieldOperator<T>(a,b) {}
 * Since FieldOperator specifies arguments will be passed, derived classes must
 * have a constructor to pass the arguments up to the base class to store them as
 * member variables
 * 
 ***************************************************/