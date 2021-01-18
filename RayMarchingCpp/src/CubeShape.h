#pragma once

#include "IShapedObject.h"
#include "IImGuiEditable.h"
#include "Math/Math.h"
#include <imgui.h>

class CubeShape : public IShapedObject, public IImGuiEditable {
public:
    CubeShape() : mCoords(-3.0f, 0.75f, 6.0f, 0.75f)
    {
    }

    virtual void PassToShader(OpenGL::ShaderProgram& shader) override
    {
        shader.SetUniform4f("u_CubeObj", mCoords.x(), mCoords.y(), mCoords.z(), mCoords.w());
    }

    virtual void RenderImGuiEditor() override
    {
        ImGui::SliderFloat("x", &mCoords.x(), -10.0f, 10.0f);
        ImGui::SliderFloat("y", &mCoords.y(), -10.0f, 10.0f);
        ImGui::SliderFloat("z", &mCoords.z(), -10.0f, 10.0f);
        ImGui::SliderFloat("size", &mCoords.w(), 0.0f, 5.0f);
    }

    virtual std::string_view Name() override
    {
        return "Cube";
    }
private:
    Math::Vec4 mCoords;
};