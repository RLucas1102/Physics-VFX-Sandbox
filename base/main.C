#include <iostream>
#include <memory.h>

#include "Volume.h"
#include "FieldInterface.h"
#include "Vector.h"
#include "Matrix.h"

using namespace lux;

int main() {

    /**
     * Test: Volumes can be created 
     */
    // Scalar volumes
    VSP<float> a = constant(4.0f);
    VSP<float> b = constant(3.0f);

    // Vector volumes
    VSP<Vector> Va = constant(Vector(1,1,1));
    VSP<Vector> Vb = constant(Vector(2,2,2));

    // Matrix volumes
    VSP<Matrix> Ma = constant(Matrix(3));
    VSP<Matrix> Mb = constant(Matrix(4));

    /**
     * Test: Volumes can call evaluate functions
     */
    // Evaluate scalar volumes
    std::cout << evaluate(a, Vector(3,3,3)) << std::endl; //4
    std::cout << evaluate(b, Vector(3,3,3)) << std::endl; //3

    // Evaluate vector volumes
    std::cout << evaluate(Va, Vector(3,3,3)).__str__() << std::endl; //(1,1,1) 
    std::cout << evaluate(Vb, Vector(3,3,3)).__str__() << std::endl; //(2,2,2)

    // Evaluate matrix volumes
    std::cout << evaluate(Ma, Vector(3,3,3)).__str__() << std::endl; //Matrix(3)
    std::cout << evaluate(Mb, Vector(3,3,3)).__str__() << std::endl; //Matrix(4)

    /**
     * Test: Volumes can be added
     */
    // Create a scalar AddField
    VSP<float> c = add(a, b);
    VSP<float> c2 = a + b;

    // Create a vector AddField
    VSP<Vector> Vc = Va + Vb;

    // Create a matrix AddField
    VSP<Matrix> Mc = Ma + Mb;

    // Evaluate added scalar volumes
    std::cout << evaluate(c, Vector(3,3,3)) << std::endl; //7
    std::cout << evaluate(c2, Vector(3,3,3)) << std::endl; //7

    // Evaluate added vector volumes
    std::cout << evaluate(Vc, Vector(3,3,3)).__str__() << std::endl; //(3,3,3)

    // Evaluate added matrix volumes
    std::cout << evaluate(Mc, Vector(3,3,3)).__str__() << std::endl; //Matrix(7)

    /**
     * Test: Volumes can call grad functions
     */
    // Find gradient of a scalar volume
    std::cout << (gradient(a, Vector(3,3,3))).__str__() << std::endl; //(0,0,0)
    std::cout << (gradient(Va, Vector(3,3,3))).__str__() << std::endl; //Matrix(0)
    std::cout << (gradient(c2, Vector(3,3,3))).__str__() << std::endl; //(0,0,0)



    return 0;
}