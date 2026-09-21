/******************************************
 * BinaryOperations.h
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

#ifndef BINARYOPERATIONS_H
#define BINARYOPERATIONS_H

namespace lux {

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

}

#endif