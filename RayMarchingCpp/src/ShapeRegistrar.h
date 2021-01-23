#pragma once

#include <vector>
#include <memory>
#include <string_view>

#include "IShapedObject.h"
#include "OpenGL/ShaderSource.h"

class ShapeRegistrar {
public:
	void Register(const std::vector<std::shared_ptr<IShapedObject>>& shapes, OpenGL::ShaderSource& source);
	void GenerateSceneDistanceFunction(OpenGL::ShaderSource& source);
private:
	std::vector<std::string> mRegisteredFunctions;
	static std::string_view sUniformMarker;
	static std::string_view sDistFunctionsMarker;
	static std::string_view sSceneDistFunctionCodeMarker;
};