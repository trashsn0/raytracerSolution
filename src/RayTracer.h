#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "../external/json.hpp"
#include <vector>


class RayTracer {
public:

    RayTracer(nlohmann::json& j);
    void ProcessJson(nlohmann::json& j);
    void SavePPM(std::string filename);
    void run();

private:
    nlohmann::json data;
    std::vector<nlohmann::json> raw_geometry;
    std::vector<nlohmann::json> raw_light;
    std::vector<nlohmann::json> raw_output;
};

#endif