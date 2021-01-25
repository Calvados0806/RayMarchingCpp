#pragma once

#include "RayMarchingWindow/IShapedObject.h"
#include "RayMarchingWindow/IImGuiEditable.h"


class InterpolatedShapeWrapper : public IShapedObject, public IImGuiEditable {
public:
    InterpolatedShapeWrapper(std::shared_ptr<IShapedObject> firstShape, std::shared_ptr<IShapedObject> secondShape, float grade)
        : mFirst(firstShape), mSecond(secondShape), mGrade(grade)
    {
    }

    virtual void PassToShader(OpenGL::ShaderProgram& shader) override
    {
        mFirst->PassToShader(shader);
        mSecond->PassToShader(shader);
        shader.SetUniform1f("u_IterpolateGrade", mGrade);
    }

    virtual std::string UniformsDefinitions() const override
    {
        return mFirst->UniformsDefinitions() + mSecond->UniformsDefinitions() + UNIFORM(float, "u_IterpolateGrade");
    }

    virtual std::string DistFunctionCall(const std::string& fixedParam) const override
    {
        return "mix(" + mFirst->DistFunctionCall(fixedParam) + ", " + mSecond->DistFunctionCall(fixedParam) + ", clamp(u_IterpolateGrade, 0.0, 1.0))";
    }

    void RenderImGuiEditor()
    {
        ImGui::SliderFloat("grade", &mGrade, 0.0f, 1.0f);
    }

    std::string_view SectionName() const
    {
        return "InterpolatedObject";
    }
private:
    std::shared_ptr<IShapedObject> mFirst;
    std::shared_ptr<IShapedObject> mSecond;
    float mGrade;
};
