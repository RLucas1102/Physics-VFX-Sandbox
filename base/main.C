#include <iostream>
#include <memory.h>

#include "Volume.h"
#include "ImplicitFields.h"
#include "FieldInterface.h"
#include "Fields.h"
#include "Vector.h"
#include "Matrix.h"

using namespace lux;

int main() {
    
    // I guess we are not supposed to be calling ConstantVolume directly
    // ------------------------------------------------------------------
    // lux::ConstantVolume* a = new ConstantVolume(4);
    // lux::ConstantVolume* b = new ConstantVolume(3);

    // std::cout << a.eval(lux::Vector(3,3,3)) << std::endl;
    // std::cout << b.eval(lux::Vector(3,3,3)) << std::endl;

    // lux::ScalarField c = a + b;

    // std::shared_ptr<ConstantVolume> a = std::make_shared<ConstantVolume>(4);
    // std::shared_ptr<ConstantVolume> b = std::make_shared<ConstantVolume>(3);

    // std::cout << a->eval(Vector(3,3,3)) << std::endl;
    // std::cout << b->eval(Vector(3,3,3)) << std::endl;
    // ------------------------------------------------------------------

    // This gave a linking error which made me assume 
    // I should not be calling ConstantVolume directly
    // --------------------------------------------------
    // ScalarField af(a);
    // ScalarField bf(b);

    // std::cout << af->eval(Vector(3,3,3)) << std::endl;
    // std::cout << bf->eval(Vector(3,3,3)) << std::endl;

    // ScalarField cf = add(af, bf);
    // --------------------------------------------------

    // Linking error was coming from constant(const lux::Vector& v)
    ScalarField a = constant(4);
    ScalarField b = constant(3);
    ScalarField c = a + b;

    std::cout << evaluate(a, Vector(3,3,3)) << std::endl; // 4
    std::cout << evaluate(b, Vector(3,3,3)) << std::endl; // 3
    std::cout << evaluate(c, Vector(3,3,3)) << std::endl; // 7

    // Scalar volumes
    std::shared_ptr<Volume<float>> a2 = std::make_shared<ConstantField<float>>(4);
    std::shared_ptr<Volume<float>> b2 = std::make_shared<ConstantField<float>>(3);

    // Vector volumes
    std::shared_ptr<Volume<Vector>> Va2 = std::make_shared<ConstantField<Vector>>(Vector(1,1,1));
    std::shared_ptr<Volume<Vector>> Vb2 = std::make_shared<ConstantField<Vector>>(Vector(2,2,2));

    // Matrix volumes
    std::shared_ptr<Volume<Matrix>> Ma2 = std::make_shared<ConstantField<Matrix>>(Matrix(3));
    std::shared_ptr<Volume<Matrix>> Mb2 = std::make_shared<ConstantField<Matrix>>(Matrix(4));

    // Evaluate scalar volumes
    std::cout << a2->eval(Vector(3,3,3)) << std::endl; //4
    std::cout << b2->eval(Vector(3,3,3)) << std::endl; //3

    // Evaluate vector volumes
    std::cout << (Va2->eval(Vector(3,3,3))).__str__() << std::endl; //(1,1,1)
    std::cout << (Vb2->eval(Vector(3,3,3))).__str__() << std::endl; //(2,2,2)

    // Evaluate matrix volumes
    std::cout << (Ma2->eval(Vector(3,3,3))).__str__() << std::endl; //Matrix(3)
    std::cout << (Mb2->eval(Vector(3,3,3))).__str__() << std::endl; //Matrix(4)

    // Should not be able to instaniate a FieldOperator class directly
    // FieldOperator<float> s;

    // Create a scalar AddField
    // std::shared_ptr<Volume<float>> c2 = std::make_shared<AddField<float>>(a2, b2); 
    std::shared_ptr<Volume<float>> c2 = add(a2, b2);
    std::shared_ptr<Volume<float>> c3 = a2 + b2;

    // Create a vector AddField
    std::shared_ptr<Volume<Vector>> Vc2 = Va2 + Vb2;

    // Create a matrix AddField
    std::shared_ptr<Volume<Matrix>> Mc2 = Ma2 + Mb2;

    // Evaluate added scalar volumes
    std::cout << c2->eval(Vector(3,3,3)) << std::endl; //7
    std::cout << c3->eval(Vector(3,3,3)) << std::endl; //7

    // Evaluate added vector volumes
    std::cout << (Vc2->eval(Vector(3,3,3))).__str__() << std::endl; //(3,3,3)

    // Evaluate added matrix volumes
    std::cout << (Mc2->eval(Vector(3,3,3))).__str__() << std::endl; //Matrix(7)

    return 0;
}