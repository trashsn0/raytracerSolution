#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "../external/json.hpp"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <vector>


class RayTracer {
public:

    RayTracer(nlohmann::json& j);
    void ProcessJson(nlohmann::json& j);
    void SavePPM(std::string filename);
    void Output();
    void Geometry();
    void run();

private:
    nlohmann::json data;
    std::vector<nlohmann::json> raw_geometry;
    std::vector<nlohmann::json> raw_light;
    std::vector<nlohmann::json> raw_output;
};

class Object {
public:
    std::string type;
    float ka, kd, ks; // Ambient, diffuse, and specular reflection coefficients
    float pc; // Phong coefficient
    Eigen::Vector3f ac, dc, sc; // Ambient, diffuse, and specular colors

    // Default constructor
    Object() = default;

    // Constructor from JSON
    explicit Object(const nlohmann::json& j);

    // Virtual destructor for proper cleanup of derived types
    virtual ~Object() = default;

};

class Sphere : public Object {
public:
    float radius;
    Eigen::Vector3f centre;
    // Explicit constructor for Sphere
    explicit Sphere(const nlohmann::json& j);
};

class Rectangle : public Object {
public:
    Eigen::Vector3f p1, p2, p3, p4; // Corner points
    
    // Explicit constructor for Rectangle
    explicit Rectangle(const nlohmann::json& j);
};


#endif