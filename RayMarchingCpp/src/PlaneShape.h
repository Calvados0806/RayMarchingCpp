#pragma once

#include "RayMarchingWindow/IShapedObject.h"
#include "RayMarchingWindow/IImGuiEditable.h"
#include "Math/Math.h"
#include <imgui.h>

class PlaneShape : public IShapedObject {
public:
    PlaneShape(float Y, const std::string& name) : mYTranslation(Y), mName(name)
    {
    }

    virtual void PassToShader(OpenGL::ShaderProgram& shader) override
    {
        shader.SetUniform1f(Name(), mYTranslation);
    }

    virtual std::string UniformsDefinitions() const override
    {
        return UNIFORM(float, Name());
    }

    virtual std::string DistFunctionCall(const std::string& fixedParam) const override
    {
        return DistFunctionName()+'(' + fixedParam + ", " + Name() + ')';
    }

    static std::string DistFunctionDefinition()
    {
        return DIST_FUNCTION_PROTOTYPE(DistFunctionName(), vec3 p, float planeObj) DIST_FUNCTION_CODE(
            p.y -= planeObj;
            return dot(p, normalize(vec3(0, 1, 0))) - sin(p.x / 10);
        );
    }

    const std::string& Name() const
    {
        return mName;
    }
private:
    static std::string DistFunctionName()
    {
        return "PlaneDist";
    }
private:
    float mYTranslation;
    const std::string mName;
};