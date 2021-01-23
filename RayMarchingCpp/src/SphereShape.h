#pragma once

#include "IShapedObject.h"
#include "IImGuiEditable.h"
#include "Math/Math.h"
#include <imgui.h>

class SphereShape : public IShapedObject, public IImGuiEditable {
public:
    SphereShape(Math::Vec4 coords) : mCoords(coords)
    {
    }

    virtual void PassToShader(OpenGL::ShaderProgram& shader) override
    {
        shader.SetUniform4f("u_SphereObj", mCoords.x(), mCoords.y(), mCoords.z(), mCoords.w());
    }

    virtual void RenderImGuiEditor() override
    {
        ImGui::SliderFloat("x", &mCoords.x(), -10.0f, 10.0f);
        ImGui::SliderFloat("y", &mCoords.y(), -10.0f, 10.0f);
        ImGui::SliderFloat("z", &mCoords.z(), -10.0f, 10.0f);
        ImGui::SliderFloat("radius", &mCoords.w(), 0.0f, 5.0f);
    }

    virtual std::string_view SectionName() const override
    {
        return "Sphere";
    }

    virtual std::string UniformsDefinitions() const override
    {
        return "uniform vec4 u_SphereObj;\n";
    }
    
protected:
    virtual std::string Name() const override
    {
        return SectionName().data();
    }

    virtual std::string DistFunctionCode() const override
    {
        return "return length(p - u_SphereObj.xyz) - u_SphereObj.w;\n";
    }
private:
    Math::Vec4 mCoords;
};