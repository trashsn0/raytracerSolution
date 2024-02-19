#include "RayTracer.h"
#include <iostream>
#include <fstream>

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



void RayTracer::run() {
    std::cout << "-----------------ALPHA 1 - RAYTRACER-------------------------"<< std::endl;
    ProcessJson(data);
    for (auto& output : raw_output[0]) {
        SavePPM(output["filename"]);
        std::cout << output["filename"] << " created in build directory." << std::endl;
    }
    
}

