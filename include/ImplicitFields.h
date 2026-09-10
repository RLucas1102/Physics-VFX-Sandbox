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

    // Plane Field
    // Creates a plane volume defined by a point on the surface and its normal
    class PlaneField : public Volume<float> {

        public:
            PlaneField(const Vector& n, const Vector& p) : _normal(n.unitvector()), _pointOnSurf(p) {}
            ~PlaneField() = default;

            const float eval(const Vector& p) const override { return -(p - _pointOnSurf) * _normal; }
            const Vector grad(const Vector& p) const override { return _normal; }

        private:
            Vector _normal;
            Vector _pointOnSurf;
    };

    // Sphere Field
    // Creates a sphere volume defined by a radius
    class SphereField : public Volume<float> {

        public:
            SphereField(const float radius): _radius(radius) {}
            ~SphereField() = default;

            const float eval(const Vector& p) const override { return _radius - p.magnitude(); }
            const Vector grad(const Vector& p) const override {
                Vector result;
                if (p.magnitude() != 0) {result = -p.unitvector(); }
                else {result = Vector(0, 1, 0); }
                return result;
            }

        private:
            float _radius;
    };

    // Torus Field
    // Creates a Torus volume defined by a radius major, minor, and normal
    class TorusField : public Volume<float> {

        public:
            TorusField(const float rM, const float rm, const Vector& n): _radMajor(rM), _radMinor(rm), _normal(n.unitvector()) {}
            ~TorusField() = default;

            const float eval(const Vector& p) const override { 
                Vector X = p;
                Vector xPerp = X - (X * _normal) * _normal;
                float rM2 = std::pow(_radMajor, 2);
                float xPerpMag2 = std::pow(xPerp.magnitude(), 2);
                float xMag2 = std::pow(X.magnitude(), 2);
                float rm2 = std::pow(_radMinor, 2);
                return (4 * rM2 *xPerpMag2) - std::pow(xMag2 + rM2 - rm2, 2);
            }

        private:
            float _radMajor, _radMinor;
            Vector _normal;
    };
    
    // ------------------------------------------------------------------------------------

    /******************************************
     * Section: Unary Field Operations
     * 
     * The classes in this section define 
     * different operations that can be
     * performed on one field
     * 
     * These classes are created through helper
     * functions defined in FieldInterface.h
     * See FieldInterface.h for more info
     ******************************************/

    // Base class
    // All field operations will derive from this abstract class 
    template<typename T>
    class UnaryFieldOperator : public Volume<T> {
        
        public:

            // Need to make these public for derived classes like Volume does
            using typename Volume<T>::volumeDataType;

            UnaryFieldOperator(const VSP<T>& a) : _a(a) {}
            ~UnaryFieldOperator() = default;

            virtual const volumeDataType eval(const Vector& P) const = 0;

        protected:
            VSP<T> _a; 
    };

    // MaskField
    // Positive values return 1 and negative values return 0
    template<typename T>
    class MaskField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            MaskField(const VSP<T>& a) : UnaryFieldOperator<T>(a) {}

            const volumeDataType eval(const Vector& P) const override { 
                if (this->_a->eval(P) > 0) { return 1; }
                else { return 0; }
            }
    };

    // NegateField
    // Returns the negated evaluated field value at P
    template<typename T>
    class NegateField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            NegateField(const VSP<T>& a) : UnaryFieldOperator<T>(a) {}

            const volumeDataType eval(const Vector& P) const override { return -this->_a->eval(P); }
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
     * at any point in the field like primitives.
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
            using typename Volume<T>::volumeDataType;

            BinaryFieldOperator(const VSP<T>& a, const VSP<U>& b) : _a(a), _b(b) {}
            ~BinaryFieldOperator() = default;

            virtual const volumeDataType eval(const Vector& P) const = 0;

        protected:
            VSP<T> _a; 
            VSP<U> _b;
    };

    // AddFields
    // Two fields can be combined via an add operation
    // Add fields cannot add fields of different types, so we enforce that here 
    // by never using U
    template<typename T>
    class AddField : public BinaryFieldOperator<T, T> {

        using typename Volume<T>::volumeDataType;

        public:
            AddField(const VSP<T>& a, const VSP<T>& b) : BinaryFieldOperator<T, T>(a,b) {}

            const volumeDataType eval(const Vector& P) const override { 
                return this->_a->eval(P) + this->_b->eval(P); 
            }
    };

    // MultiplyFields
    // Two fields can be combined via a multiplication operation
    // MultiplyField can take in two different fields
    // Implemented to take the type of the leftmost field
    // Ex: Color * Float = Volume<Color>
    template<typename T, typename U>
    class MultiplyField : public BinaryFieldOperator<T, U> {

        using typename Volume<T>::volumeDataType;

        public:
            MultiplyField(const VSP<T>& a, const VSP<U>& b) : BinaryFieldOperator<T, U>(a,b) {}

            const volumeDataType eval(const Vector& P) const override { 
                return this->_a->eval(P) * this->_b->eval(P); 
            }
    };


    // ------------------------------------------------------------------------------------

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