/******************************************
 * ImplicitFields.h
 * 
 * The classes in this section define
 * many kinds of volumes that represent
 * a single, uncombined volume (primitives)
 * 
 * They can be combined with other volumes
 * or manipulated via unary or binary 
 * operations.
 ******************************************/

#ifndef IMPLICITFIELDS_H
#define IMPLICITFIELDS_H

namespace lux {

    // Constant Field
    // Should return the given value for any evaluated point
    template<typename T>
    class ConstantField : public Volume<T> {

        using typename Volume<T>::volumeDataType;
        using typename Volume<T>::volumeGradType;

        public:
            ConstantField(const T& v);
            ~ConstantField() = default;

            const volumeDataType eval(const Vector& P) const override;
            const volumeGradType grad(const Vector& P) const override;
        
        private:
            volumeDataType _value;
            volumeGradType _gradValue;

    };

    // Plane Field
    // Creates a plane volume defined by a point on the surface and its normal
    class PlaneField : public Volume<float> {

        public:
            PlaneField(const Vector& n, const Vector& p);
            ~PlaneField() = default;

            const float eval(const Vector& p) const override;
            const Vector grad(const Vector& p) const override;

        private:
            Vector _normal;
            Vector _pointOnSurf;
    };

    // Sphere Field
    // Creates a sphere volume defined by a radius
    class SphereField : public Volume<float> {

        public:
            SphereField(const float radius);
            ~SphereField() = default;

            const float eval(const Vector& p) const override;
            const Vector grad(const Vector& p) const override;

        private:
            float _radius;
    };

    // Torus Field
    // Creates a Torus volume defined by a radius major, minor, and normal
    class TorusField : public Volume<float> {

        public:
            TorusField(const float rM, const float rm, const Vector& n);  
            ~TorusField() = default;

            const float eval(const Vector& p) const override;

        private:
            float _radMajor, _radMinor;
            Vector _normal;
    };

    // Cone Field
    // Creates a Cone volume defined by a h, theta, and normal
    class ConeField : public Volume<float> {

        public:
            ConeField(const float h, const float theta, const Vector& n);
            ~ConeField() = default;

            const float eval(const Vector& p) const override;
        private:
            float _height, _theta;
            Vector _normal;
    };

    // Box Field
    // Creates a Box volume defined by a radius and smoothness parameter
    class BoxField : public Volume<float> {

        public:
            BoxField(const float radius, const float smooth);
            ~BoxField() = default;

            const float eval(const Vector& p) const override;

        private:
            float _radius, _smooth;
    };

    // Icosahedron Field
    // Creates an Icosahedron volume
    class IcosahedronField : public Volume<float> {

        public:
            IcosahedronField();
            ~IcosahedronField() = default;

            const float eval(const Vector& p) const override;

        private:
            float _T = 1.61803399;
    };

    // Ellipse Field
    // Creates an Ellipse volume defined by a radius major, minor, and normal
    class EllipseField : public Volume<float> {

        public:
            EllipseField(const float rM, const float rm, const Vector& n);
            ~EllipseField() = default;

            const float eval(const Vector& p) const override;

        private:
            float _radMajor, _radMinor;
            Vector _normal;
    };

    // Cylinder Field
    // Creates a cylinder volume defined by a radius and normal
    class CylinderField : public Volume<float> {

        public:
            CylinderField(const float radius, const Vector& n);
            ~CylinderField() = default;

            const float eval(const Vector& p) const override;

        private:
            float _radius;
            Vector _normal;
    };

    // Steiner Field
    // Creates a Steiner patch volume
    class SteinerField : public Volume<float> {

        public:
            SteinerField();
            ~SteinerField() = default;

            const float eval(const Vector& p) const override;
    };

} 

#endif