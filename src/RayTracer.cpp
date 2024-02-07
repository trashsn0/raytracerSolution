#include "RayTracer.h"
#include <iostream>
#include <typeinfo>

RayTracer::RayTracer(const nlohmann::json& j) :  data(j){

}

void RayTracer::run() {
    std::cout << "-----------------SIMPLE PARSING OF SCENE-------------------------"<< std::endl;
    for (auto& element : data.items()) {
        std::cout << "\n----------------" << element.key() <<"-------------------"<< std::endl;
        std::cout << " RAW DATA: " << std::endl;
        std::cout << element.value() << std::endl;
        std::cout << "------------------------------------------"<< std::endl;
        
    }
    std::cout << "JSON SCENE PARSED CORRECTLY" << std::endl;
}