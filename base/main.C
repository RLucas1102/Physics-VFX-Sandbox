#include <iostream>
#include <memory.h>

#include "Volume.h"
#include "Fields.h"
#include "Vector.h"

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

    return 0;
}