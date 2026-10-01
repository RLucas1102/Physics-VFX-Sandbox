#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "FieldInterface.h"
#include "Color.h"
#include "Vector.h"

namespace lux
{
    class Scene {

        public:

            // Scene is a singleton
            static std::shared_ptr<Scene> Instance();
            ~Scene() = default;

            // Initialize the scene with objects, light, etc.
            void init();

            // Update objects the scene in some way per frame
            void update();

        private:

            bool initialized;

            int frame;

            // Data containers for scene's contents
            std::vector<Mesh>       _objects;
            std::vector<Light>      _lights;
            Camera                  _cam;
            std::vector<VSP<Color>> _materials;
            std::vector<VSP<float>> _volumes;

            // Static scene exists in class as a whole
            static std::shared_ptr<Scene> pScene;

            // Private constructor
            Scene() {};

    };

    std::shared_ptr<Scene> CreateScene();
    
} // namespace lux


#endif