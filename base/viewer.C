#include <iostream>
#include <memory.h>

#include "Volume.h"
#include "FieldInterface.h"
#include "Vector.h"
#include "Matrix.h"
#include "Color.h"

using namespace lux;

int main() {

    // Sphere
    float radius = 1;
    VSP<float> sphereA = sphere(radius);
    VSP<float> sphereA_den = mask(sphereA);

    // Color volumes
    // Red
    VSP<Color> Cm = constant(Color(1,0,0,0));

    VSP<Color> sphereA_red = Cm * sphereA_den;

    //Evaluate color field
    std::cout << evaluate(sphereA_red, Vector(0,0,0)).__str__() << std::endl; // (1,0,0,0) Red
    std::cout << evaluate(sphereA_red, Vector(2,2,2)).__str__() << std::endl; // (0,0,0,0) Black

    return 0;
}