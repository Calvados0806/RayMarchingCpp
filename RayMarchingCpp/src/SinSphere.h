#pragma once

#include "RayMarchingWindow/IShapedObject.h"
#include "RayMarchingWindow/IImGuiEditable.h"
#include "Math/Math.h"
#include <imgui.h>

class SinSphereShape : public IShapedObject, public IImGuiEditable {
public:
    SinSphereShape(Math::Vec4 coords, const std::string& name) : mCoords(coords), mName(name)
    {
    }

    virtual void PassToShader(OpenGL::ShaderProgram& shader) override
    {
        shader.SetUniform4f(Name(), mCoords.x(), mCoords.y(), mCoords.z(), mCoords.w());
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
        return Name();
    }

    virtual std::string UniformsDefinitions() const override
    {
        return UNIFORM(vec4, Name());
    }

    virtual std::string DistFunctionCall(const std::string& fixedParam) const override
    {
        return DistFunctionName() + '(' + fixedParam + ", " + Name() + ')';
    }

    static std::string DistFunctionDefinition()
    {
        return DIST_FUNCTION_PROTOTYPE(DistFunctionName(), vec3 p, vec4 sphereObj) DIST_FUNCTION_CODE(
            vec3 d = p - sphereObj.xyz;
            d = wrapSpace(d, 25);
            return (length(d) - sphereObj.w - sin(p.x*40 + u_Time*3)*0.05)*0.5;
        );
    }

    const std::string& Name() const
    {
        return mName;
    }
private:
    static std::string DistFunctionName()
    {
        return "SinSphereDist";
    }
private:
    Math::Vec4 mCoords;
    const std::string mName;
};