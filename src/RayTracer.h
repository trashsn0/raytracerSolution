#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "../external/json.hpp"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <vector>
#include <memory>

/////////////////////////////////////////// LIGHTING CLASS METHODS //////////////////////////////////////////
class Lighting {
    public:
    // Constructor
    explicit Lighting(const nlohmann::json& j);

    // Destructor
    virtual ~Lighting() = default;

    // Mendatory Members
    std::string type;
    Eigen::Vector3f id, is; //Diffuse and specular light intensity

    // Optional Members
    Eigen::Matrix4f transform; //4x4 affine transformation matrix in row-major representation
    bool usecenter;
    unsigned int n;


};

class Point : public Lighting {
    public :
    Eigen::Vector3f centre;

    // Explicit constructor for Point
    explicit Point(const nlohmann::json& j);
    
};

class Area : public Lighting {
    public :
    Eigen::Vector3f p1, p2, p3, p4; // Corner points
    
    // Explicit constructor for Area
    explicit Area(const nlohmann::json& j);

};


/////////////////////////////////////////// Object CLASS METHODS //////////////////////////////////////////
class Object {
public:

    // Mendatory Members
    std::string type;
    float ka, kd, ks; // Ambient, diffuse, and specular reflection coefficients
    float pc; // Phong coefficient
    Eigen::Vector3f ac, dc, sc; // Ambient, diffuse, and specular colors

    // Optional Members
    std::string comment;
    Eigen::Matrix4f transform; //4x4 affine transformation matrix in row-major representation

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
/////////////////////////////////////////// Output CLASS METHODS //////////////////////////////////////////

class Output {
public: 

    // Mendatory members
    std::string filename;
    Eigen::Vector2i size;
    Eigen::Vector3f centre;
    float fov;
    Eigen::Vector3f up;
    Eigen::Vector3f lookat, ai, bkc;

    // Optional members
    std::vector<unsigned int> raysperpixel;
    bool antialiasing, twosiderender, globalillum;
    
    // Constructor
    explicit Output(const nlohmann::json& j);

    // Destructor
    virtual ~Output() = default;

};

/////////////////////////////////////////// RayTracer CLASS METHODS //////////////////////////////////////////


class RayTracer {
public:

    RayTracer(nlohmann::json& j);
   
    void SavePPM(std::string filename);
    
    
    void run();

    // Parser Functions (Returns vector of said Class)
    void ProcessJson(nlohmann::json& j);
    std::vector<std::unique_ptr<Object>> GeometryParser(); 
    std::vector<std::unique_ptr<Lighting>> LightParser();
    std::vector<std::unique_ptr<Output>> OutputParser();


    // Handler Functions
    void GeometryHandler();
    void LightHandler();
    void OutputHandler();

private:
    nlohmann::json data;
    std::vector<nlohmann::json> raw_geometry;
    std::vector<nlohmann::json> raw_light;
    std::vector<nlohmann::json> raw_output;
};


#endif