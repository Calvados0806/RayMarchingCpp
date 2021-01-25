#pragma once

#include "RayMarchingWindow/IShapedObject.h"


class IntersectedShapeWrapper : public IShapedObject {
public:
    IntersectedShapeWrapper(std::shared_ptr<IShapedObject> firstShape, std::shared_ptr<IShapedObject> secondShape)
        : mFirst(firstShape), mSecond(secondShape)
    {
    }

    virtual void PassToShader(OpenGL::ShaderProgram& shader) override
    {
        mFirst->PassToShader(shader);
        mSecond->PassToShader(shader);
    }

    virtual std::string UniformsDefinitions() const override
    {
        return mFirst->UniformsDefinitions() + mSecond->UniformsDefinitions();
    }

    virtual std::string DistFunctionCall(const std::string& fixedParam) const override
    {
        return "smin(" + mFirst->DistFunctionCall(fixedParam) + ", " + mSecond->DistFunctionCall(fixedParam) + ", -u_SmoothMinValue)";
    }
private:
    std::shared_ptr<IShapedObject> mFirst;
    std::shared_ptr<IShapedObject> mSecond;
};
