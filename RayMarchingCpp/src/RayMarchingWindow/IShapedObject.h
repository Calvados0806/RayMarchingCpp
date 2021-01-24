#pragma once

#include "OpenGL/ShaderProgram.h"

#define UNIFORM(type, name) "uniform " #type " " + name + ";\n"
#define DIST_FUNCTION_PROTOTYPE(name, ...) "float " + name + "(" #__VA_ARGS__ ")"
#define DIST_FUNCTION_CODE(...) "{ " #__VA_ARGS__ " }"

struct IShapedObject {
    virtual ~IShapedObject() {}
    virtual void PassToShader(OpenGL::ShaderProgram& shader) = 0;
    virtual std::string UniformsDefinitions() const = 0;
    virtual std::string DistFunctionCall(const std::string& fixedParam) const = 0;
};