#pragma once

#include "IShapedObject.h"
#include "IImGuiEditable.h"
#include "Math/Math.h"
#include <imgui.h>

class PlaneShape : public IShapedObject {
public:
    PlaneShape(float Y) : mYTranslation(Y)
    {
    }

    virtual void PassToShader(OpenGL::ShaderProgram& shader) override
    {
        shader.SetUniform1f("u_PlaneObj", mYTranslation);
    }

    virtual std::string UniformsDefinitions() const override
    {
        return "uniform float u_PlaneObj;\n";
    }

protected:
    virtual std::string Name() const override
    {
        return "Plane";
    }

    virtual std::string DistFunctionCode() const override
    {
        return "return abs(p.y - u_PlaneObj);\n";
    }
private:
    float mYTranslation;
};