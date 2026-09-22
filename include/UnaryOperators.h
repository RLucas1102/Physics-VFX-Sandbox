/******************************************
 * UnaryOperations.h
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

#ifndef UNARYOPERATORS_H
#define UNARYOPERATORS_H

namespace lux {

    // Base class
    // All field operations will derive from this abstract class 
    template<typename T>
    class UnaryFieldOperator : public Volume<T> {
        
        public:

            // Need to make these public for derived classes like Volume does
            using typename Volume<T>::volumeDataType;

            UnaryFieldOperator(const VSP<T>& a);
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
            MaskField(const VSP<T>& a);

            const volumeDataType eval(const Vector& P) const override;
    };

    // NegateField
    // Returns the negated evaluated field value at P
    template<typename T>
    class NegateField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            NegateField(const VSP<T>& a);

            const volumeDataType eval(const Vector& P) const override;
    };

    // ExpField
    // Returns the e^value at P of a field
    template<typename T>
    class ExpField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            ExpField(const VSP<T>& a);

            const volumeDataType eval(const Vector& P) const override;
    };
    

    // LogField
    // Returns the natural log(value) at P of a field
    template<typename T>
    class LogField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            LogField(const VSP<T>& a);

            const volumeDataType eval(const Vector& P) const override;
    };

    // SinField
    // Returns the Sin(value) at P of a field in radians
    template<typename T>
    class SinField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            SinField(const VSP<T>& a);

            const volumeDataType eval(const Vector& P) const override;
    };

    // CosField
    // Returns the Cos(value) at P of a field in radians
    template<typename T>
    class CosField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            CosField(const VSP<T>& a);

            const volumeDataType eval(const Vector& P) const override;
    };

    // PowField
    // Returns the Pow(value, scalar) at P of a field
    template<typename T>
    class PowField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            PowField(const VSP<T>& a, const float val);

            const volumeDataType eval(const Vector& P) const override;

        private:
            float _val;
    };

    // TranslateField
    // Returns the Translate(F) by Vector Xt 
    template<typename T>
    class TranslateField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            TranslateField(const VSP<T>& a, const Vector& xt);

            const volumeDataType eval(const Vector& P) const override;

        private:
            Vector _Xt;
    };

    // ScaleField
    // Returns the Scale(f) by float _val
    template<typename T>
    class ScaleField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            ScaleField(const VSP<T>& a, const float val);

            const volumeDataType eval(const Vector& P) const override;

        private:
            float _val;
    };

    // RotateField
    // Returns the Rotate(f) by a given angle around a given axis
    template<typename T>
    class RotateField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            RotateField(const VSP<T>& a, const float theta, const Vector& axis);

            const volumeDataType eval(const Vector& P) const override;

        private:
            float _theta;
            Vector _axis;
    };

    // DilateField
    // Inflates or deflates volumes
    template<typename T>
    class DilateField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            DilateField(const VSP<T>& a, const float val);

            const volumeDataType eval(const Vector& P) const override;

        private:
            float _val;
    };

    // ShellField
    // Hollows out a volume and gives it a thickness
    template<typename T>
    class ShellField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            ShellField(const VSP<T>& a, const float val);

            const volumeDataType eval(const Vector& P) const override;

        private:
            float _val;
    };

    // ClampField
    // Bounds volume values within a range
    template<typename T>
    class ClampField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            ClampField(const VSP<T>& a, const float fmin, const float fmax);

            const volumeDataType eval(const Vector& P) const override;

        private:
            float _fmin;
            float _fmax;
    };
}

#endif