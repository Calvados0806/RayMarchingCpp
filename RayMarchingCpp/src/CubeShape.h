#pragma once

#include "IShapedObject.h"
#include "IImGuiEditable.h"
#include "Math/Math.h"
#include <imgui.h>

class CubeShape : public IShapedObject, public IImGuiEditable {
public:
    CubeShape(Math::Vec4 coords, const std::string& name) : mCoords(coords), mName(name)
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
        ImGui::SliderFloat("size", &mCoords.w(), 0.0f, 5.0f);
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
        return DistFunctionName()+'(' + fixedParam + ", " + Name() + ')';
    }

    static std::string DistFunctionDefinition()
    {
        return DIST_FUNCTION_PROTOTYPE(DistFunctionName(), vec3 p, vec4 cubeObj) DIST_FUNCTION_CODE(
            vec3 size = vec3(cubeObj.w);
            vec3 p1 = p - cubeObj.xyz;
            vec3 d = abs(p1) - size;
            return min(max(d.x, max(d.y, d.z)), 0.0) +
                    length(max(d, 0.0));
        );
    }

    const std::string& Name() const
    {
        return mName;
    }
private:
    static std::string DistFunctionName()
    {
        return "CubeDist";
    }
private:
    Math::Vec4 mCoords;
    const std::string mName;
};