#include "RayTracer.h"
#include <iostream>
#include <fstream>

/////////////////////////////////////////// OBJECT CLASS METHODS ///////////////////////////////////////////
 
Object::Object(const nlohmann::json& j) {
    type = j.at("type").get<std::string>();
    ka = j.at("ka").get<float>();
    kd = j.at("kd").get<float>();
    ks = j.at("ks").get<float>();
    pc = j.at("pc").get<float>();
    ac = Eigen::Vector3f(j.at("ac")[0], j.at("ac")[1], j.at("ac")[2]);
    dc = Eigen::Vector3f(j.at("dc")[0], j.at("dc")[1], j.at("dc")[2]);
    sc = Eigen::Vector3f(j.at("sc")[0], j.at("sc")[1], j.at("sc")[2]);
}

Sphere::Sphere(const nlohmann::json& j) : Object(j) {
    centre = Eigen::Vector3f(j.at("centre")[0], j.at("centre")[1], j.at("centre")[2]);
    radius = j.at("radius").get<float>();
}

Rectangle::Rectangle(const nlohmann::json& j) : Object(j) {
    p1 = Eigen::Vector3f(j.at("p1")[0], j.at("p1")[1], j.at("p1")[2]);
    p2 = Eigen::Vector3f(j.at("p2")[0], j.at("p2")[1], j.at("p2")[2]);
    p3 = Eigen::Vector3f(j.at("p3")[0], j.at("p3")[1], j.at("p3")[2]);
    p4 = Eigen::Vector3f(j.at("p4")[0], j.at("p4")[1], j.at("p4")[2]);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////// RAYTRACER CLASS METHODS ///////////////////////////////////////////
RayTracer::RayTracer(nlohmann::json& j) :  data(j){
}

void RayTracer::ProcessJson(nlohmann::json& j) {
    for (auto& element : j.items()) {
        if (element.key() == "geometry") {
            raw_geometry.push_back(element.value());
        } else if (element.key() == "light") {
            raw_light.push_back(element.value());
        } else if (element.key() == "output") {
            raw_output.push_back(element.value());
        }
        std::cout << element.key() << " FOUND >>> SAVED IN PRIVATE "<< "raw_"+element.key() << std::endl;
    }
    std::cout << "JSON SCENE PARSED CORRECTLY." << std::endl;
}

void RayTracer::SavePPM(std::string filename) {
    std::ofstream file(filename);
}

void RayTracer::Geometry() {
    for (auto& objectJson : raw_geometry[0]) {
        
        if (objectJson["type"] == "rectangle") {
            Rectangle rectangleObject(objectJson);
            std::cout << "Rectangle Created !" << std::endl;
        } else if (objectJson["type"] == "sphere") {
            Sphere sphereObject(objectJson);
            std::cout << "Sphere Created !" << std::endl;
        }
    }
}

void RayTracer::Output() {
    for (auto& output : raw_output[0]) {
        SavePPM(output["filename"]);
        std::cout << output["filename"] << " created in build directory." << std::endl;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////// RUN METHOD (CALLED BY MAIN.CPP)     /////////////////////////////////
void RayTracer::run() {
    std::cout << "-----------------ALPHA 1 - RAYTRACER-------------------------"<< std::endl;
    ProcessJson(data);
    Geometry();
    Output();
    
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////