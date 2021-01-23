#pragma once

#include "IShapedObject.h"
#include "IImGuiEditable.h"
#include "Math/Math.h"
#include <imgui.h>

class CubeShape : public IShapedObject, public IImGuiEditable {
public:
    CubeShape(Math::Vec4 coords) : mCoords(coords)
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

    virtual std::string_view SectionName() const override
    {
        return "Cube";
    }

    virtual std::string UniformsDefinitions() const override
    {
        return "uniform vec4 u_CubeObj;\n";
    }

protected:
    virtual std::string Name() const override
    {
        return SectionName().data();
    }

    virtual std::string DistFunctionCode() const override
    {
        return "vec3 size = vec3(u_CubeObj.w);\n\
                vec3 p1 = p - u_CubeObj.xyz;\n\
                vec3 d = abs(p1) - size;\n\
                return min(max(d.x, max(d.y, d.z)), 0.0) +\n\
                    length(max(d, 0.0));\n";
    }
private:
    Math::Vec4 mCoords;
};