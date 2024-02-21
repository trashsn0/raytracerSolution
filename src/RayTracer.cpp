#include "RayTracer.h"
#include <iostream>
/////////////////////////////////////////// LIGHTING CLASS METHODS //////////////////////////////////////////
Lighting::Lighting(const nlohmann::json& j) {
    // Mendatory member
    type = j.at("type").get<std::string>();
    
    id = Eigen::Vector3f(j.at("id")[0], j.at("id")[1], j.at("id")[2]);
    is = Eigen::Vector3f(j.at("is")[0], j.at("is")[1], j.at("is")[2]);
    

    // Optional member
    if (j.find("usecenter") != j.end()) {
        usecenter = j.at("usecenter");
    }
    if (j.find("transform") != j.end()) {
        transform << j.at("transform")[0],j.at("transform")[1],j.at("transform")[2],j.at("transform")[3],
        j.at("transform")[4],j.at("transform")[5],j.at("transform")[6],
        j.at("transform")[7],j.at("transform")[8],j.at("transform")[9],j.at("transform")[10],
        j.at("transform")[11],j.at("transform")[12],j.at("transform")[13],
        j.at("transform")[14],j.at("transform")[15] ;
    }
    if (j.find("n") != j.end()) {
        n = j.at("n");
    }
}

Point::Point(const nlohmann::json& j) : Lighting(j) {
    centre = Eigen::Vector3f(j.at("centre")[0], j.at("centre")[1], j.at("centre")[2]);
}

Area::Area(const nlohmann::json& j) : Lighting(j) {
    p1 = Eigen::Vector3f(j.at("p1")[0], j.at("p1")[1], j.at("p1")[2]);
    p2 = Eigen::Vector3f(j.at("p2")[0], j.at("p2")[1], j.at("p2")[2]);
    p3 = Eigen::Vector3f(j.at("p3")[0], j.at("p3")[1], j.at("p3")[2]);
    p4 = Eigen::Vector3f(j.at("p4")[0], j.at("p4")[1], j.at("p4")[2]);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////// OBJECT CLASS METHODS ///////////////////////////////////////////
 
Object::Object(const nlohmann::json& j) {
    // Mendatory member
    type = j.at("type").get<std::string>();
    ka = j.at("ka").get<float>();
    kd = j.at("kd").get<float>();
    ks = j.at("ks").get<float>();
    pc = j.at("pc").get<float>();
    ac = Eigen::Vector3f(j.at("ac")[0], j.at("ac")[1], j.at("ac")[2]);
    dc = Eigen::Vector3f(j.at("dc")[0], j.at("dc")[1], j.at("dc")[2]);
    sc = Eigen::Vector3f(j.at("sc")[0], j.at("sc")[1], j.at("sc")[2]);

    // Optional member
    if (j.find("comment") != j.end()) {
        comment = j.at("comment").get<std::string>();
    }
    if (j.find("transform") != j.end()) {
        transform << j.at("transform")[0],j.at("transform")[1],j.at("transform")[2],j.at("transform")[3],
        j.at("transform")[4],j.at("transform")[5],j.at("transform")[6],
        j.at("transform")[7],j.at("transform")[8],j.at("transform")[9],j.at("transform")[10],
        j.at("transform")[11],j.at("transform")[12],j.at("transform")[13],
        j.at("transform")[14],j.at("transform")[15] ;
    }
    
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

/////////////////////////////////////////// OUTPUT CLASS METHODS ///////////////////////////////////////////
 
Output::Output(const nlohmann::json& j) {
    // Mendatory member
    filename = j.at("filename").get<std::string>();
    size = Eigen::Vector2i(j.at("size")[0], j.at("size")[1]);
    centre = Eigen::Vector3f(j.at("centre")[0], j.at("centre")[1], j.at("centre")[2]);
    fov = j.at("fov").get<float>();
    up = Eigen::Vector3f(j.at("up")[0], j.at("up")[1], j.at("up")[2]);
    lookat = Eigen::Vector3f(j.at("lookat")[0], j.at("lookat")[1], j.at("lookat")[2]);
    ai = Eigen::Vector3f(j.at("ai")[0], j.at("ai")[1], j.at("ai")[2]);
    bkc = Eigen::Vector3f(j.at("bkc")[0], j.at("bkc")[1], j.at("bkc")[2]);

    // Optional member
    if (j.find("raysperpixel") != j.end()) {
        raysperpixel = j.at("raysperpixel").get<std::vector<unsigned int>>();
    }
    if (j.find("antialiasing") != j.end()) {
        antialiasing = j.at("antialiasing");
    }
    if (j.find("twosiderender") != j.end()) {
        twosiderender = j.at("twosiderender");
    }
    if (j.find("globalillum") != j.end()) {
        globalillum = j.at("globalillum");
    }
    
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////// SIMPLE RAYCASTER METHODS //////////////////////////////////////////

// Basic intersection check with a sphere
bool IntersectSphere(const Ray& ray, const Sphere& sphere) {
    // Vector from ray origin to sphere center
    Eigen::Vector3f oc = ray.origin - sphere.centre;
    float a = ray.direction.dot(ray.direction);
    float b = 2.0f * oc.dot(ray.direction);
    float c = oc.dot(oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;
    return (discriminant > 0);
}

// Basic intersection check with a rectangle
bool IntersectRectangle(const Ray& ray, const Rectangle& rect) {
    // Compute plane normal
    Eigen::Vector3f p1p2 = rect.p2 - rect.p1;
    Eigen::Vector3f p1p4 = rect.p4 - rect.p1;
    Eigen::Vector3f N = p1p2.cross(p1p4).normalized();
    // Plane equation: N.dot(X - P0) = 0, solve for X = ray.origin + t*ray.direction
    float denom = N.dot(ray.direction);
    if (std::abs(denom) > 1e-6) { // Ensure not parallel
        float t = (N.dot(rect.p1) - N.dot(ray.origin)) / denom;
        if (t >= 0) { // Check if intersection is in ray direction
            Eigen::Vector3f P = ray.origin + t * ray.direction; // Intersection point on plane
            // Edge vectors
            Eigen::Vector3f edge1 = rect.p2 - rect.p1;
            Eigen::Vector3f edge2 = rect.p3 - rect.p2;
            Eigen::Vector3f edge3 = rect.p4 - rect.p3;
            Eigen::Vector3f edge4 = rect.p1 - rect.p4;
            // Check if P inside polygon using edge tests
            bool inside = N.dot(edge1.cross(P - rect.p1)) > 0 && 
                          N.dot(edge2.cross(P - rect.p2)) > 0 && 
                          N.dot(edge3.cross(P - rect.p3)) > 0 &&
                          N.dot(edge4.cross(P - rect.p4)) > 0;
            return inside;
        }
    }
    return false;
}


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
    }
}

void RayTracer::SavePPM(std::string filename) {
    std::ofstream file(filename);
}

std::vector<std::unique_ptr<Object>> RayTracer::GeometryParser() {
    std::vector<std::unique_ptr<Object>> Output;
    for (auto& objectJson : raw_geometry[0]) {
        if (objectJson["type"] == "rectangle") {
            Output.push_back(std::make_unique<Rectangle>(objectJson));
        } else if (objectJson["type"] == "sphere") {
            Output.push_back(std::make_unique<Sphere>(objectJson));
        }
    }
    return Output;
}


void RayTracer::GeometryHandler() {
    int counter = 0;
    for (const auto& objPtr : GeometryParser()) {
        const Object& obj = *objPtr;
        counter++;
        //std::cout << "Type: " << obj.type << std::endl;
        if (!obj.comment.empty()) {
            //std::cout << "Comment: " << obj.comment << std::endl;
        }

        if (obj.type == "rectangle") {
            const Rectangle& rect = dynamic_cast<const Rectangle&>(obj);
        } else if (obj.type == "sphere") {
            const Sphere& sphere = dynamic_cast<const Sphere&>(obj);
        }
        //std::cout << "-------------------------" << std::endl;
    }
    std::cout << "Geometry Parsed correctly, "<<counter << " object(s) found." << std::endl;
}

std::vector<std::unique_ptr<Lighting>> RayTracer::LightParser() {
    std::vector<std::unique_ptr<Lighting>> Output;
    for (auto& objectJson : raw_light[0]) {
        if (objectJson["type"] == "point") {
            Output.push_back(std::make_unique<Point>(objectJson));
        } else if (objectJson["type"] == "area") {
            Output.push_back(std::make_unique<Area>(objectJson));
        }
    }
    return Output;
}


void RayTracer::LightHandler() {
    int counter = 0;
    for (const auto& objPtr : LightParser()) {
        const Lighting& lght = *objPtr;
        counter++;
        //std::cout << "Type: " << lght.type << std::endl;
        /**
        if (lght.type == "area") {
            const Area& area = dynamic_cast<const Area&>(lght);
            std::cout << "Area : ";
            std::cout << "p1: " << area.p1 << ", ";
            std::cout << "p2: " << area.p2 << ", ";
            std::cout << "p3: " << area.p3 << ", ";
            std::cout << "p4: " << area.p4 << std::endl;
        } else if (lght.type == "point") {
            const Point& point = dynamic_cast<const Point&>(lght);
            std::cout << "Centre : " << point.centre << std::endl;
        }
        std::cout << "-------------------------" << std::endl;
        */
    }
    std::cout << "Light Parsed correctly, "<<counter << " lighting(s) found." << std::endl;
}

std::vector<std::unique_ptr<Output>> RayTracer::OutputParser() {
    std::vector<std::unique_ptr<Output>> result;
    for (auto& objectJson : raw_output[0]) {
        result.push_back(std::make_unique<Output>(objectJson));
    }
    return result;
}

void RayTracer::OutputHandler() {
    for (const auto& objPtr : OutputParser()) {
        Output& output = *objPtr;
        output.InitializeCamera(); // Setup the camera based on Output attributes

        std::ofstream file(output.filename);
        file << "P3\n" << output.size.x() << " " << output.size.y() << "\n255\n";

        // Camera parameters for ray generation
        float aspect_ratio = float(output.size.x()) / output.size.y();
        float scale = std::tan(output.fov * 0.5 * M_PI / 180);

        for (int y = 0; y < output.size.y(); ++y) {
            for (int x = 0; x < output.size.x(); ++x) {
                float u = (2 * (x + 0.5) / float(output.size.x()) - 1) * scale * aspect_ratio;
                float v = (1 - 2 * (y + 0.5) / float(output.size.y())) * scale;

                // Generate ray from camera to pixel
                Eigen::Vector3f dir = (u * output.right + v * output.up + output.forward).normalized();
                Ray ray(output.position, dir);

                // Check intersection with scene geometry
                bool hit_anything = false;
                Eigen::Vector3f pixel_color = output.bkc; // Use background color by default
                for (const auto& geomPtr : GeometryParser()) {
                    if (geomPtr->type == "sphere" && IntersectSphere(ray, dynamic_cast<const Sphere&>(*geomPtr))) {
                        hit_anything = true;
                        pixel_color = Eigen::Vector3f(1, 0, 0); // Red if hit
                        break; // For simplicity, stop at the first hit
                    } else if (geomPtr->type == "rectangle" && IntersectRectangle(ray, dynamic_cast<const Rectangle&>(*geomPtr))) {
                        hit_anything = true;
                        pixel_color = Eigen::Vector3f(0, 1, 0); // Green if hit
                        break; // For simplicity, stop at the first hit
                    }
                }

                int ir = static_cast<int>(255.999 * pixel_color.x());
                int ig = static_cast<int>(255.999 * pixel_color.y());
                int ib = static_cast<int>(255.999 * pixel_color.z());

                file << ir << ' ' << ig << ' ' << ib << '\n';
            }
        }
        file.close();
        std::cout << " ---------------------------" << std::endl;
        std::cout << output.filename << " created in build directory." << std::endl;
        
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////// RUN METHOD (CALLED BY MAIN.CPP)     /////////////////////////////////
void RayTracer::run() {
    std::cout << "-----------------ALPHA 2 - RAYTRACER-------------------------"<< std::endl;
    ProcessJson(data);
    GeometryHandler();
    LightHandler();
    std::cout << "-------" << std::endl;
    std::cout << "Computing results ... " << std::endl;
    OutputHandler();
    
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////