#include "RayMarchingWindow/RayMarchingWindow.h"

int main(void)
{
    std::unique_ptr<RayMarchingWindow> window = Window::Create<RayMarchingWindow>("Ray Marching", 1280, 720);

    std::shared_ptr<PlaneShape> plane(new PlaneShape(0.0f, "u_PlaneObj"));
    std::shared_ptr<SphereShape> sphere(new SphereShape(Math::Vec4(0.0f, 1.0f, 6.0f, 1.0f), "u_SphereObj"));
    std::shared_ptr<CubeShape> cube(new CubeShape(Math::Vec4(-3.0f, 0.75f, 6.0f, 0.75f), "u_CubeObj"));

    window->RegisterNewShape<PlaneShape>();
    window->RegisterNewShape<SphereShape>();
    window->RegisterNewShape<CubeShape>();

    window->RegisterNewObject(plane);
    window->RegisterNewObject(sphere);
    window->RegisterNewObject(cube);

    window->RegisterEditableObject(sphere);
    window->RegisterEditableObject(cube);

    window->Run();

    return(0);
}