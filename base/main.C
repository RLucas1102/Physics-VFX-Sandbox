#include <iostream>
#include <memory.h>

#include "Volume.h"
#include "FieldInterface.h"
#include "Vector.h"
#include "Matrix.h"
#include "Color.h"

using namespace lux;

int main() {

    /**
     * Test: Volumes can be created 
     */
    // Scalar volumes
    // Constant
    VSP<float> a = constant(4.0f);
    VSP<float> b = constant(3.0f);

    // Plane
    Vector normal = Vector(1,0,0);
    Vector point  = Vector(1,0,1);
    VSP<float> planeA = plane(normal, point);

    // Sphere
    float radius = 1;
    VSP<float> sphereA = sphere(radius);
    VSP<float> sphereA_den = mask(sphereA);

    // Vector volumes
    VSP<Vector> Va = constant(Vector(1,1,1));
    VSP<Vector> Vb = constant(Vector(2,2,2));

    // Matrix volumes
    VSP<Matrix> Ma = constant(Matrix(3));
    VSP<Matrix> Mb = constant(Matrix(4));

    // Color volumes
    // Red
    VSP<Color> Cm = constant(Color(1,0,0,0));

    /**
     * Test: Volumes can call evaluate functions
     */
    // Evaluate scalar volumes
    // Constant
    std::cout << evaluate(a, Vector(3,3,3)) << std::endl; //4
    std::cout << evaluate(b, Vector(3,3,3)) << std::endl; //3

    // Plane
    std::cout << evaluate(planeA, Vector(0,0,0)) << std::endl; //[(0,0,0) - (1,0,1)] * (1,0,0) = -1
    std::cout << evaluate(planeA, Vector(3,3,3)) << std::endl; //[(3,3,3) - (1,0,1)] * (1,0,0) = 2

    // Sphere
    std::cout << evaluate(sphereA, Vector(0,0,0)) << std::endl; // 1
    std::cout << evaluate(sphereA, Vector(2,2,2)) << std::endl; // -2.4641
    std::cout << evaluate(sphereA_den, Vector(0,0,0)) << std::endl; // 1
    std::cout << evaluate(sphereA_den, Vector(2,2,2)) << std::endl; // 0

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
    // Adding two constants
    VSP<float> c = add(a, b);
    VSP<float> c2 = a + b;

    // Adding a constant and a plane
    VSP<float> c3 = a + planeA;

    // Create a vector AddField
    VSP<Vector> Vc = Va + Vb;

    // Create a matrix AddField
    VSP<Matrix> Mc = Ma + Mb;

    // Evaluate added scalar volumes
    // Added constants
    std::cout << evaluate(c, Vector(3,3,3)) << std::endl; //7
    std::cout << evaluate(c2, Vector(3,3,3)) << std::endl; //7

    // Added constant and plane
    std::cout << evaluate(c3, Vector(0,0,0)) << std::endl; //4 + -1 = 3
    std::cout << evaluate(c3, Vector(3,3,3)) << std::endl; //4 + 2 = 6

    // Evaluate added vector volumes
    std::cout << evaluate(Vc, Vector(3,3,3)).__str__() << std::endl; //(3,3,3)

    // Evaluate added matrix volumes
    std::cout << evaluate(Mc, Vector(3,3,3)).__str__() << std::endl; //Matrix(7)

    /**
     * Test: Volumes can be multiplied
     */

    VSP<Color> sphereA_red = Cm * sphereA_den;

    //Evaluate color field
    std::cout << evaluate(sphereA_red, Vector(0,0,0)).__str__() << std::endl; // (1,0,0,0) Red
    std::cout << evaluate(sphereA_red, Vector(2,2,2)).__str__() << std::endl; // (0,0,0,0) Black

    /**
     * Test: Volumes can call grad functions
     */
    // Find gradient of a scalar volume
    std::cout << (gradient(a, Vector(3,3,3))).__str__() << std::endl; //(0,0,0)
    std::cout << (gradient(Va, Vector(3,3,3))).__str__() << std::endl; //Matrix(0)
    std::cout << (gradient(c2, Vector(3,3,3))).__str__() << std::endl; //(0,0,0)
    std::cout << (gradient(planeA, Vector(3,3,3))).__str__() << std::endl; //normal = (1,0,0)
    std::cout << (gradient(c3, Vector(3,3,3))).__str__() << std::endl; //(0,0,0) + (1,0,0)



    return 0;
}