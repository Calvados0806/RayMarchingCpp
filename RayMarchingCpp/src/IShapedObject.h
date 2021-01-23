#pragma once

#include "OpenGL/ShaderProgram.h"

struct IShapedObject {
    virtual ~IShapedObject() {}
    virtual void PassToShader(OpenGL::ShaderProgram& shader) = 0;
    virtual std::string UniformsDefinitions() const = 0;

    std::string DistFunctionDefinition() const
    {
        return "float " + Name() + "Dist(vec3 p)\n\
        {" + DistFunctionCode() + "}\n";
    }

    std::string DistFunctionName() const
    {
        return Name() + "Dist";
    }

protected:
    virtual std::string Name() const = 0;
    virtual std::string DistFunctionCode() const = 0;
};