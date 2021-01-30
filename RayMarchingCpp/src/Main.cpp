#include "RayMarchingWindow/RayMarchingWindow.h"

#include "CubeShape.h"
#include "SphereShape.h"
#include "PlaneShape.h"
#include "CroppedShapeWrapper.h"
#include "IntersectedShapeWrapper.h"
#include "InterpolatedShapeWrapper.h"

int main(void)
{
    std::unique_ptr<RayMarchingWindow> window = RayMarchingWindow::Create("Ray Marching", 1280, 720);

    auto plane = std::make_shared<PlaneShape>(0.0f, "u_PlaneObj");

    auto sphere = std::make_shared<SphereShape>(Math::Vec4(0.0f, 1.0f, 6.0f, 1.0f), "u_SphereObj");
    auto cube = std::make_shared<CubeShape>(Math::Vec4(0.0f, 1.0f, 6.0f, 0.75f), "u_CubeObj");
    auto croppedCube = std::make_shared<CroppedShapeWrapper>(cube, sphere);

    auto sphere1 = std::make_shared<SphereShape>(Math::Vec4(3.0f, 1.0f, 5.0f, 1.0f), "u_SphereObj_1");
    auto cube1 = std::make_shared<CubeShape>(Math::Vec4(3.0f, 1.0f, 5.0f, 0.75f), "u_CubeObj_1");
    auto intersected = std::make_shared<IntersectedShapeWrapper>(sphere1, cube1);
    
    auto sphere2 = std::make_shared<SphereShape>(Math::Vec4(-3.0f, 1.0f, 5.0f, 1.0f), "u_SphereObj_2");
    auto cube2 = std::make_shared<CubeShape>(Math::Vec4(-3.0f, 1.0f, 5.0f, 1.0f), "u_CubeObj_2");
    auto interpolated = std::make_shared<InterpolatedShapeWrapper>(sphere2, cube2, 0.5f);

    window->RegisterNewShape<PlaneShape>();
    window->RegisterNewShape<SphereShape>();
    window->RegisterNewShape<CubeShape>();

    window->RegisterNewObject(plane);
    window->RegisterNewObject(croppedCube);
    window->RegisterNewObject(intersected);
    window->RegisterNewObject(interpolated);

    window->RegisterEditableObject(sphere);
    window->RegisterEditableObject(cube);
    window->RegisterEditableObject(sphere1);
    window->RegisterEditableObject(cube1);
    window->RegisterEditableObject(interpolated);

    window->Run();

    return(0);
}