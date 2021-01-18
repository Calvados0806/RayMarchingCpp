#pragma once

#include "IShapedObject.h"
#include "IImGuiEditable.h"
#include "Math/Math.h"
#include <imgui.h>

class PlaneShape : public IShapedObject {
public:
    PlaneShape() : mYTranslation(0.0f)
    {
    }

    virtual void PassToShader(OpenGL::ShaderProgram& shader) override
    {
        shader.SetUniform1f("u_PlaneObj", mYTranslation);
    }
private:
    float mYTranslation;
};