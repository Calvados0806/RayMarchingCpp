#pragma once

#include "OpenGL/ShaderProgram.h"

struct IShapedObject {
    virtual ~IShapedObject() {}
    virtual void PassToShader(OpenGL::ShaderProgram& shader) = 0;
};