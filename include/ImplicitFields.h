#ifndef IMPLICITFIELDS_H
#define IMPLICITFIELDS_H

#include <iostream>

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

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

    // Cone Field
    // Creates a Cone volume defined by a h, theta, and normal
    class ConeField : public Volume<float> {

        public:
            ConeField(const float h, const float theta, const Vector& n): _height(h), _theta(theta), _normal(n.unitvector()) {}
            ~ConeField() = default;

            const float eval(const Vector& p) const override { 
                Vector X = p;
                float result = 0;
                float xn = X * _normal;
                if(xn < 0) { result = xn; }
                else if (xn > _height) { result = _height - xn; }
                else if (xn > 0 && xn < _height) { result = xn - X.magnitude() * std::cos(_theta); }
                return result;
            }

        private:
            float _height, _theta;
            Vector _normal;
    };

    // Box Field
    // Creates a Box volume defined by a radius and smoothness parameter
    class BoxField : public Volume<float> {

        public:
            BoxField(const float radius, const float smooth): _radius(radius), _smooth(smooth) {}
            ~BoxField() = default;

            const float eval(const Vector& p) const override {
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

        private:
            float _radius, _smooth;
    };

    // Icosahedron Field
    // Creates an Icosahedron volume
    class IcosahedronField : public Volume<float> {

        public:
            IcosahedronField() {}
            ~IcosahedronField() = default;

            const float eval(const Vector& p) const override {
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

        private:
            float _T = 1.61803399;
    };

    // Ellipse Field
    // Creates an Ellipse volume defined by a radius major, minor, and normal
    class EllipseField : public Volume<float> {

        public:
            EllipseField(const float rM, const float rm, const Vector& n): _radMajor(rM), _radMinor(rm), _normal(n.unitvector()) {}
            ~EllipseField() = default;

            const float eval(const Vector& p) const override { 
                Vector X = p;
                float z = X * _normal;
                Vector xPerp = X - z * _normal;
                float rM2 = std::pow(_radMajor, 2);
                float xPerpMag2 = std::pow(xPerp.magnitude(), 2);
                float z2 = std::pow(z, 2);
                float rm2 = std::pow(_radMinor, 2);
                return 1 - (z2/rM2) - (xPerpMag2/rm2);
            }

        private:
            float _radMajor, _radMinor;
            Vector _normal;
    };

    // Cylinder Field
    // Creates a cylinder volume defined by a radius and normal
    class CylinderField : public Volume<float> {

        public:
            CylinderField(const float radius, const Vector& n): _radius(radius), _normal(n.unitvector()) {}
            ~CylinderField() = default;

            const float eval(const Vector& p) const override { 
                Vector X = p;
                float xn = X *_normal;
                float mag = (X - (xn * _normal)).magnitude();
                return _radius - mag;
            }

        private:
            float _radius;
            Vector _normal;
    };

    // Steiner Field
    // Creates a Steiner patch volume
    class SteinerField : public Volume<float> {

        public:
            SteinerField() {}
            ~SteinerField() = default;

            const float eval(const Vector& p) const override {
                Vector v = p;
                float x = v[0];
                float y = v[1];
                float z = v[2];
                float x2 = std::pow(x, 2);
                float y2 = std::pow(y, 2);
                float z2 = std::pow(z, 2);
                return -(x2*y2 + x2*z2 + y2*z2 - x*y*z);
            }
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

    // ExpField
    // Returns the e^value at P of a field
    template<typename T>
    class ExpField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            ExpField(const VSP<T>& a) : UnaryFieldOperator<T>(a) {}

            const volumeDataType eval(const Vector& P) const override { return std::exp(this->_a->eval(P)); }
    };

    // LogField
    // Returns the natural log(value) at P of a field
    template<typename T>
    class LogField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            LogField(const VSP<T>& a) : UnaryFieldOperator<T>(a) {}

            const volumeDataType eval(const Vector& P) const override { return std::log(this->_a->eval(P)); }
    };

    // SinField
    // Returns the Sin(value) at P of a field in radians
    template<typename T>
    class SinField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            SinField(const VSP<T>& a) : UnaryFieldOperator<T>(a) {}

            const volumeDataType eval(const Vector& P) const override { return std::sin(this->_a->eval(P)); }
    };

    // CosField
    // Returns the Cos(value) at P of a field in radians
    template<typename T>
    class CosField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            CosField(const VSP<T>& a) : UnaryFieldOperator<T>(a) {}

            const volumeDataType eval(const Vector& P) const override { return std::cos(this->_a->eval(P)); }
    };

    // PowField
    // Returns the Pow(value, scalar) at P of a field
    template<typename T>
    class PowField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            PowField(const VSP<T>& a, const float val) : UnaryFieldOperator<T>(a), _val(val) {}

            const volumeDataType eval(const Vector& P) const override { return std::pow(this->_a->eval(P), _val); }

        private:
            float _val;
    };

    // TranslateField
    // Returns the Translate(F) by Vector Xt 
    template<typename T>
    class TranslateField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            TranslateField(const VSP<T>& a, const Vector& xt) : UnaryFieldOperator<T>(a), _Xt(xt) {}

            const volumeDataType eval(const Vector& P) const override { return this->_a->eval(P - _Xt); }

        private:
            Vector _Xt;
    };

    // ScaleField
    // Returns the Scale(f) by float _val
    template<typename T>
    class ScaleField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            ScaleField(const VSP<T>& a, const float val) : UnaryFieldOperator<T>(a), _val(val) {}

            const volumeDataType eval(const Vector& P) const override { return this->_a->eval(P / _val); }

        private:
            float _val;
    };

    // RotateField
    // Returns the Rotate(f) by a given angle around a given axis
    template<typename T>
    class RotateField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            RotateField(const VSP<T>& a, const float theta, const Vector& axis) : 
            UnaryFieldOperator<T>(a), _theta(-theta), _axis(axis.unitvector()) {}

            const volumeDataType eval(const Vector& P) const override { 
                Vector X = P;
                float Cos = std::cos(_theta);
                float ax = _axis * X;
                Vector xa = X^_axis;
                Vector result = X * Cos + _axis * ax * (1 - Cos) + xa * std::sin(_theta);
                return this->_a->eval(result);
            }

        private:
            float _theta;
            Vector _axis;
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

    // SubtractFields
    // Two fields can be combined via a subtract operation
    template<typename T>
    class SubtractField : public BinaryFieldOperator<T, T> {

        using typename Volume<T>::volumeDataType;

        public:
            SubtractField(const VSP<T>& a, const VSP<T>& b) : BinaryFieldOperator<T, T>(a,b) {}

            const volumeDataType eval(const Vector& P) const override { 
                return this->_a->eval(P) - this->_b->eval(P); 
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

    // DivideFields
    // Two fields can be combined via a divide operation
    // DivideField can take in two different fields
    // Implemented to take the type of the leftmost field
    // Ex: Color / Float = Volume<Color>
    template<typename T, typename U>
    class DivideField : public BinaryFieldOperator<T, U> {

        using typename Volume<T>::volumeDataType;

        public:
            DivideField(const VSP<T>& a, const VSP<U>& b) : BinaryFieldOperator<T, U>(a,b) {}

            const volumeDataType eval(const Vector& P) const override { 
                return this->_a->eval(P) / this->_b->eval(P); 
            }
    };

    // UnionFields
    // Two fields can be combined via a max operation and join them together
    template<typename T>
    class UnionField : public BinaryFieldOperator<T, T> {

        using typename Volume<T>::volumeDataType;

        public:
            UnionField(const VSP<T>& a, const VSP<T>& b) : BinaryFieldOperator<T, T>(a,b) {}

            const volumeDataType eval(const Vector& P) const override { 
                return std::max(this->_a->eval(P), this->_b->eval(P)); 
            }
    };

    // IntersectionFields
    // Two fields can be combined via a min operation and join them where they both exist
    template<typename T>
    class IntersectionField : public BinaryFieldOperator<T, T> {

        using typename Volume<T>::volumeDataType;

        public:
            IntersectionField(const VSP<T>& a, const VSP<T>& b) : BinaryFieldOperator<T, T>(a,b) {}

            const volumeDataType eval(const Vector& P) const override { 
                return std::min(this->_a->eval(P), this->_b->eval(P)); 
            }
    };

    // CutoutFields
    // Two fields can be combined via a min operation and then negating one field to cut the shape
    template<typename T>
    class CutoutField : public BinaryFieldOperator<T, T> {

        using typename Volume<T>::volumeDataType;

        public:
            CutoutField(const VSP<T>& a, const VSP<T>& b) : BinaryFieldOperator<T, T>(a,b) {}

            const volumeDataType eval(const Vector& P) const override { 
                return std::min(this->_a->eval(P), -this->_b->eval(P)); 
            }
    };

    // BlendFields
    // Two fields can be combined via Blinn blend which is a smooth union operation
    template<typename T>
    class BlendField : public BinaryFieldOperator<T, T> {

        using typename Volume<T>::volumeDataType;

        public:
            BlendField(const VSP<T>& a, const VSP<T>& b, const float alpha1, const float alpha2) : 
            BinaryFieldOperator<T, T>(a,b), _alpha1(alpha1), _alpha2(alpha2){}

            const volumeDataType eval(const Vector& P) const override { 
                return std::exp(_alpha1 * this->_a->eval(P)) + std::exp(_alpha2 * this->_b->eval(P)) - 2;
            }

        private:
            float _alpha1;
            float _alpha2;

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