#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "../external/json.hpp"

class RayTracer {
public:

    RayTracer(const nlohmann::json& j);
    void run();

private:
    nlohmann::json data;
};

#endif