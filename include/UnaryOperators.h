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

    // DilateField
    // Inflates or deflates volumes
    template<typename T>
    class DilateField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            DilateField(const VSP<T>& a, const float val) : UnaryFieldOperator<T>(a), _val(val) {}

            const volumeDataType eval(const Vector& P) const override { return this->_a->eval(P) + _val; }

        private:
            float _val;
    };

    // ShellField
    // Hollows out a volume and gives it a thickness
    template<typename T>
    class ShellField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            ShellField(const VSP<T>& a, const float val) : UnaryFieldOperator<T>(a), _val(val) {}

            const volumeDataType eval(const Vector& P) const override { 
                return std::min(this->_a->eval(P) + _val / 2, -(this->_a->eval(P) - _val / 2));
            }

        private:
            float _val;
    };

    // ClampField
    // Bounds volume values within a range
    template<typename T>
    class ClampField : public UnaryFieldOperator<T> {

        using typename Volume<T>::volumeDataType;

        public:
            ClampField(const VSP<T>& a, const float fmin, const float fmax) : 
            UnaryFieldOperator<T>(a), _fmin(fmin), _fmax(fmax) {}

            const volumeDataType eval(const Vector& P) const override { 
                float result = 0;
                if (this->_a->eval(P) <= _fmin) { result = _fmin; }
                else if (this->_a->eval(P) > _fmin && this->_a->eval(P) < _fmax){ result = this->_a->eval(P); }
                else if (this->_a->eval(P) >= _fmax) { result = _fmax; }
                return result;
            }

        private:
            float _fmin;
            float _fmax;
    };
}

#endif