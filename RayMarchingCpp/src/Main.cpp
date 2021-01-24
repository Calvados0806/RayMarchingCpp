#include "RayMarchingWindow/RayMarchingWindow.h"

#include "CubeShape.h"
#include "SphereShape.h"
#include "PlaneShape.h"
#include "CroppedShapeWrapper.h"

int main(void)
{
    std::unique_ptr<RayMarchingWindow> window = Window::Create<RayMarchingWindow>("Ray Marching", 1280, 720);

    auto plane = std::make_shared<PlaneShape>(0.0f, "u_PlaneObj");
    auto sphere = std::make_shared<SphereShape>(Math::Vec4(0.0f, 1.0f, 6.0f, 1.0f), "u_SphereObj");
    auto cube = std::make_shared<CubeShape>(Math::Vec4(0.0f, 1.0f, 6.0f, 0.75f), "u_CubeObj");
    auto croppedCube = std::make_shared<CroppedShapeWrapper>(cube, sphere);

    window->RegisterNewShape<PlaneShape>();
    window->RegisterNewShape<SphereShape>();
    window->RegisterNewShape<CubeShape>();

    window->RegisterNewObject(plane);
    window->RegisterNewObject(croppedCube);

    window->RegisterEditableObject(sphere);
    window->RegisterEditableObject(cube);

    window->Run();

    return(0);
}