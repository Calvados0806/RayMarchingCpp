#include "RayMarchingWindow/RayMarchingWindow.h"

#include "CubeShape.h"
#include "SphereShape.h"
#include "PlaneShape.h"
#include "CroppedShapeWrapper.h"
#include "IntersectedShapeWrapper.h"
#include "InterpolatedShapeWrapper.h"
#include "SinSphere.h"
#include "VaseShape.h"

int main(void)
{
    std::unique_ptr<RayMarchingWindow> window = RayMarchingWindow::Create("Ray Marching", 1280, 720);

    auto plane = std::make_shared<PlaneShape>(0.0f, "u_PlaneObj");
    float x = 0.0f;
    float y = 1.8f;
    float z = 6.0f;

    auto sphere = std::make_shared<SphereShape>(Math::Vec4(x, y, z, 1.0f), "u_SphereObj");
    auto cube = std::make_shared<CubeShape>(Math::Vec4(x, y, z, 0.75f), "u_CubeObj");

    auto vase = std::make_shared<VaseShape>(Math::Vec4(x, 0.0f, z, 1.0f), "u_VaseObj");

    auto sinSphere1 = std::make_shared<SinSphereShape>(Math::Vec4(x, y, z, 1.0f), "u_SinSphereObj");
    auto croppedSin = std::make_shared<CroppedShapeWrapper>(sphere, sinSphere1);
    auto croppedSinCube = std::make_shared<InterpolatedShapeWrapper>(cube, croppedSin, 0.5f);

    window->RegisterNewShape<PlaneShape>();
    window->RegisterNewShape<SphereShape>();
    window->RegisterNewShape<CubeShape>();
    window->RegisterNewShape<SinSphereShape>();
    window->RegisterNewShape<VaseShape>();

    window->RegisterNewObject(plane);
    window->RegisterNewObject(croppedSinCube);
    window->RegisterNewObject(vase);

    window->RegisterEditableObject(sphere);
    window->RegisterEditableObject(cube);
    window->RegisterEditableObject(sinSphere1);
    window->RegisterEditableObject(croppedSinCube);
    window->RegisterEditableObject(vase);

    window->Run();

    return(0);
}